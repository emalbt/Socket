#define ENABLE_NETWORK
#define WEBCAM

// Standard include
#include <iostream>

// Camera include
#include "/usr/include/opencv2/opencv.hpp"
#include "/usr/include/opencv2/highgui/highgui.hpp"

// Other files include
#include "types.h"
#include "camera_functions.h"
#include "indentation.h"
#include "loadcell.h"
#include "opencv_functions.h"
#include "vibration_functions.h"
#include "wireless_communicator.h"
#include "QB_functions.h"
#include "accelerometers.h"

#define sign(X) (X>=0.0)?48:49

// CAMERA MACROS
#define VIDEO_WIDTH 	320
#define VIDEO_HEIGHT 	240
#define OPENCV_WIDTH 	160
#define OPENCV_HEIGHT 	120
#define ROI_X0	    	 40
#define ROI_Y0      	 45
#define ROI_WIDTH   	220
#define ROI_HEIGHT  	150
#define FPS 			 30

// APPLICATION MACROS
#define FINGER_CODE_US   50000  // 100000 us -> 10 Hz frequency
#define SENSATION_CODE_US 2000  //666, sample time (us): 666 us -> 1.5 KHz frequency (SET > 500 us)
#define MOTOR_US 	    100000
#define BACKGROUND_THR 	   210	// EVE: 200
#define IND_THR 			 2	// right: 2 mm, before: 5 mm

// ROBOT MACROS
#define PIXELS_TO_MM  0.46875 // 160 px -> (7.5/2) mm

// EXPERIMENTS MACROS
#define NUM_SPECIMENS 4

using namespace std;
using namespace cv;

// Global network variables declaration
char buf[MAXDATASIZE];

// Information from robot
short int button;
short int posX, posY, posZ;
double robot_stiffness = 0.0;
short int accX, accY, accZ;
CPhidgetSpatialHandle accel = 0;
short int rest_acc[3] = {0,0,0};
short int acc[3] = {0,0,0};
bool first_run_acc = true;

// APPLICATION VARIABLES SETUP
CvRect roi_rect = cvRect(ROI_X0, ROI_Y0, ROI_WIDTH, ROI_HEIGHT);
IplImage* ocv_img = cvCreateImage(cvSize(OPENCV_WIDTH, OPENCV_HEIGHT), IPL_DEPTH_8U, 3);
IplImage* thr_img = cvCreateImage(cvSize(OPENCV_WIDTH, OPENCV_HEIGHT), IPL_DEPTH_8U, 1);
IplImage* bin_img = cvCreateImage(cvSize(OPENCV_WIDTH, OPENCV_HEIGHT), IPL_DEPTH_8U, 1);
CvMat* encoded;
const static int encodeParams[] = {CV_IMWRITE_JPEG_QUALITY, 5};
uint32_t num_pixels;
CvPoint center    = cvPoint(0,0);
bool is_touching = false;

// INDENTATION ESTIMATE VARIABLES SETUP
double indentation_mm = 0, old_indentation = 0;

// LOAD CELL VARIABLES
CPhidgetBridgeHandle bridge;
double force;
double force_offset;

double stiffness = 0.0;

// CAMERA VARIABLES
IplImage* cam_img;
CvCapture* cam;
IplImage* video_img;

// ROBOT VARIABLES SETUP
Point3D robot = {0,0,0};

short int ref0, ref1;

// GLOBAL FLAGS
short int cmd;
int send_img = 0;
bool connected = true;
bool connected_main = true;
int increase_kvib = 0;
bool stop_flag;
bool send_info_flag = false;
bool dft321 = true;
bool show_image = false;
bool change_motor_ref = false;
bool stiffness_calibration = false;
bool stiffness_dynamic = false;

FILE* stiffness_cal_file;

FILE* fd;
float as;
float v_curr = 0.0;
short int a_old = 0;
float stiff_ref = 0;
bool first_stiffness = true;

float st_vector[NUM_SPECIMENS] = {0.98, 0.72, 0.98, 0.72};       // 0.88, 0.39
char* file[NUM_SPECIMENS] = {"Sil_mix_5.dat", "Sil_140_5.dat", "Sil_mix_25.dat", "Sil_140_25.dat"};
int file_idx = 0;

float K_vib = 35.0;     //35.0
float k_vib = K_vib;

void intHandler(int s){
//    cout << "Terminating program..." << endl;
    connected = false;
    connected_main = false;
}

void change_material_prop(){

    // SET STIFFNESS
    robot_stiffness = st_vector[file_idx]; // + 0.2;
    stiff_ref = robot_stiffness;
    first_stiffness = true;
    if (file_idx/2 == 1)
        k_vib = 0.12*K_vib;         //0.12
    else
        k_vib = K_vib;
    //cout << "k: " << k_vib << endl;
//    cout << "\nCHOSEN STIFFNESS: " << st_vector[file_idx] << endl;

    // LOAD FILE FOR ACCELERATION RENDERING
    load_interp_parameters(file[file_idx]);
 //   get_current_force(&force_offset);

//    cout << "New specimen: " << file[file_idx] << endl;

}

void* motors(void* data){
	//robot_stiffness = stiff_ref;
	while(connected){
        robot_stiffness = stiff_ref;
        if (first_stiffness && robot_stiffness){
            short int angle = (short int)(204.6*robot_stiffness-23.6);
            short int ref0 = ANGLE_SF*angle;
            set_motor_ref(ref0, -ref0);

//           cout << "MOTOR STIFFNESS SET TO: " << robot_stiffness << " N/mm" << endl;

            first_stiffness = false;
            sleep(1);
            send_PWM(0);
            usleep(250000);
            send_PWM(0);
    //        get_current_force(&force_offset);
    //     cout << "MOTOR ref:" << ref0 << "; ANGLE: " << get_angle() << endl;
        }

		//while(stop_flag);
	}
	pthread_exit(NULL);
}

/*********** FINGER DETECTION THREAD ************/
void* finger_code(void* data){

    timeval start, start_old;
    start_old.tv_sec = start_old.tv_usec = 0;
    uint32_t diff = 0;

    init_adc();

    double distance[NUM_SENSORS];

    float robot_x = 0.0, old_robot_x = 0.0;

// AREA DETECTION AND INDENTATION ESTIMATE
    while(connected){
		gettimeofday(&start, NULL);
		diff = time_diff(start_old, start);
		start_old.tv_sec = start.tv_sec;
		start_old.tv_usec = start.tv_usec;
        if (FINGER_CODE_US >= diff)
            usleep(FINGER_CODE_US - diff);

/************************ AREA DETECTION PART ************************/

    // Read new frame
#ifdef WEBCAM
	cam_img = cvQueryFrame(cam);
#endif

    cvSetImageROI(cam_img, roi_rect);
    get_images(thr_img, cam_img, OPENCV_WIDTH, OPENCV_HEIGHT);
    cvThreshold(thr_img, bin_img, BACKGROUND_THR, 255, CV_THRESH_BINARY_INV);

    // Calculate area and center
    num_pixels = get_area(bin_img);
    is_touching = get_center(bin_img, thr_img, num_pixels, &center);

    // Speed computation (mm/s)
    if (is_touching){
        robot_x = -(PIXELS_TO_MM * (center.x - OPENCV_WIDTH/2));
        v_curr = fabs((robot_x - old_robot_x) / float(FINGER_CODE_US/1e6)); //0.5*v_curr + 0.5*
    }
    else{
        robot_x = 0;
        v_curr = 0.0;
    }

    if (v_curr < 5.0)
        v_curr = 0.0;

    if (v_curr > 60){
 //       printf("V out of range: %f\n", v_curr);
        v_curr = 0.0;
    }
 //   else
 //       printf("V: %f\n", v_curr);

    //printf("R: %f, %f, %f\n", robot_x, old_robot_x, float(FINGER_CODE_US/1e6));

    old_robot_x = robot_x;

/***************** INDENTATION ESTIMATE & COMMAND TO ROBOT PART (Z) ***************/
    if (is_touching)
        indentation_mm = read_indentation(distance, robot.x, robot.y);
    else
        indentation_mm = 0.0;

/************************ DATA OUTPUT PART ************************/
   if(show_image){
        cvCvtColor(bin_img, ocv_img, CV_GRAY2RGB);
        if (num_pixels > MIN_FINGER_AREA){
            draw_cp(ocv_img, 12, center, ocv_img->width, ocv_img->height);
             //   cout << "TOUCHED! ";
        }

        // Draw pictures on screen (OPENCV)
        cvShowImage("Original", cam_img);
        cvShowImage("Thresholded", thr_img);
        cvShowImage("Binary", ocv_img);
        cvWaitKey(1);
    }

	// Reset ROI
    cvResetImageROI(cam_img);

//	cout << "Finger position - X: " << robot.x << " mm Y: " << robot.y << " mm Z: " << robot.z << " mm" << endl;
	while(stop_flag);

	}

    v_curr = 0.0;

	pthread_exit(NULL);
}

bool intheregion(){     //area 160x120 px
    //cout << "pos: " << center.x << " " << center.y << endl;
    if (center.x > 15 && center.x < 145) // && center.y > 10 && center.y < 110)
        return true;
    else
        return false;
}


/**************** NETWORK PACKETS THREAD **************/
void* packets_code(void* data){
   timeval start, start_old;
   start_old.tv_sec = start_old.tv_usec = 0;
   uint32_t diff = 0;

    //float k_vib = *(float*)data;
    short int pwm_vib = 0;

    while(connected){
	if (!stop_flag){
		gettimeofday(&start, NULL);
		diff = time_diff(start_old, start);
		start_old.tv_sec = start.tv_sec;
		start_old.tv_usec = start.tv_usec;

		if (SENSATION_CODE_US >= diff)
			usleep(SENSATION_CODE_US - diff);

   //      get_current_force(&force);

        force = force - force_offset;
//cout << "FORCE: " << force << " - INDENT: " << indentation_mm << endl;

        // Read acceleration
        as = get_as(v_curr, file_idx);
   //   printf("AS: %.3f\tV: %.3f\n", as, v_curr);

/************************ COMMAND TO ROBOT PART (X, Y) ************************/

		// Tells the robot offset from center in millimeters
		if (/*force > 0.1 &&*/ center.x && center.y){
 			robot.x = -(PIXELS_TO_MM * (center.x - OPENCV_WIDTH/2));	// Mirror x center
	        	robot.y = PIXELS_TO_MM * (center.y - OPENCV_HEIGHT/2);
		}
		else {
			robot.x = robot.y = 0.0;
		}

/***************** COMMAND TO ROBOT PART (Z) ***************/

        if (/*force > 0.1 &&*/ is_touching) {
            // Update robot.z only if indentation data are valid
            if ((fabs(indentation_mm - old_indentation) < IND_THR) && (indentation_mm < 0))
                    robot.z = 0.8*robot.z + 0.2 * indentation_mm;
        }
        else {
            robot.z = 0;
            indentation_mm = 0;
        }

        old_indentation = indentation_mm;


/******************* VIBRATION PWM GENERATION PART *******************/

		// Sensation rendering algorithm evaluation using as data
        pwm_vib = compute_PWM_from_AS(as);

		// Send PWM to motors through USB using QB libraries
		pwm_vib = (short int)(k_vib*(double)pwm_vib);
		if (!as || !v_curr || !dft321 || !intheregion())
			send_PWM(0);
		else{
			send_PWM(pwm_vib, file_idx/2);
		}

	//	cout << "AS: " << as << " - V: " << v_curr << " - PWM VIB: " << pwm_vib << endl << endl;

	}

#ifdef ENABLE_NETWORK
        // SEND / RECEIVE INFORMATION FROM ROBOT
        if (receive_packet()){
            if (!stop_flag){
//	            cout << "RECEIVED: " << " cmd: " << cmd << ", " << accX << ", " << accY << ", " << accZ << endl;
//              cout << "RECEIVED: " << " button: " << button << ", " << posX << ", " << posY << ", " << posZ << endl << endl;
            }
            if (!connected) break;
        }

        if (button == 2){       // Texture
            dft321 = true;
            send_PWM(0);
            usleep(250000);
            send_PWM(0);
            first_stiffness = true;
            stiff_ref = 1.0; //0.85;       // 1.2
            char* pos = strstr(file[file_idx],".");
            char s[100];
            memset(s, 0, sizeof(s));
            memcpy(s, file[file_idx], pos-file[file_idx]);
            cout << "READY TO REPRODUCE VIRTUAL TEXTURE FOR SPECIMEN: " << s << endl;
        }
        else {
            if (button == 1){       // Softness
                dft321 = false;
                send_PWM(0);
                usleep(250000);
                send_PWM(0);
                first_stiffness = true;
                stiff_ref = st_vector[file_idx]; // + 0.25;
                //float st = (file_idx%2 == 0)?0.88:0.39;
                cout << "READY TO REPRODUCE SOFTNESS (Stiffness: " << st_vector[file_idx] << " N/mm)" << endl;
            }
            else {
                if (button >= 2){
                    if (button != 3){   // button = 3 reset with the same material
                        file_idx = (button - 4);
                    }
                    change_material_prop();
                }
            }
  //          else {      //button = 0
    //            first_stiffness = false;
      //      }
        }

        double rob[4];
        rob[0] = (robot.x*1000.0);
        rob[1] = (robot.y*1000.0);
        rob[2] = (robot.z*1000.0);
        rob[3] = (force*10000.0);

        create_packet(buf, rob);
        send_data(buf);
//      cout << "SENT: " << buf << endl;

        increase_kvib = 0;
//      cout << "K VIB: " << k_vib << endl;

        if (send_info_flag){
            char QB_string[1210];
            char app[10];
            memset(&QB_string, 0, sizeof(QB_string));

            // Build info string
            QB_info(QB_string);

            // Add other info such as k_vib
            unsigned int kvib = int(k_vib*10);
            unsigned short int ref00 = abs(ref0);
            unsigned short int ref11 = abs(ref1);
            sprintf(app, "^%03d^%c'%05d^%c'%05d", kvib, (sign(ref0)), ref00, sign(ref1), ref11);

            send_info(QB_string, app);
            send_info_flag = false;
        }

        // SEND IMAGE
        if (send_img == 1){
        	// Encode image to jpeg format
        	encoded = cvEncodeImage(".jp2", ocv_img, encodeParams);

			// After the call above, the encoded data is in encoded->data.ptr and has a length of encoded->cols bytes
			char imbuf[encoded->rows*encoded->cols];
			memcpy(&imbuf,encoded->data.ptr,sizeof(unsigned char)*encoded->rows*encoded->cols);
			send_image(imbuf, encoded->rows*encoded->cols);
        }

        if (change_motor_ref){
            if(receive_motor_ref()){
                // Adjust motor reference according to stiffness
                set_motor_ref(ref0, ref1);
                change_motor_ref = false;
            }
        //    get_current_force(&force_offset);
        }

#endif

    }

    pthread_exit(NULL);
}

int main(int argc, char** argv){

   int i = 0, c = 0;
   char file_str[20];

   stop_flag = false;

   signal(SIGINT, intHandler);

    if (argc < 2){
        cout << "Usage: sudo ./FYD_PROJECT vibration_gain [show_image[1/0]]" << endl;
        exit(1);
    }

    dft321 = false;

    if (argv[1]){
        K_vib *= atof(argv[1]);        // 5.0 default
    }

    if (argv[2]){
        if (atoi(argv[2])==1){
            st_vector[0] = st_vector[2] = 0.98;
            st_vector[1] = st_vector[3] = 0.76;
        }
        else {
            st_vector[0] = st_vector[2] = 0.94;
            st_vector[1] = st_vector[3] = 0.72;
        }
    }


    if (argv[3])
        show_image = atoi(argv[3]);


#ifdef ENABLE_NETWORK

    connected = false;

    // CONNECTION SETUP
    if (initialize_connection(argv[1])){
        cout << "Error on connection creation" << endl;
        return 1;
    }

    receive_data();             // Initial handshake
    file_idx = atoi(buf);
    cout << "Working on specimen number " << file_idx << endl;

/*
    char QB_string[1210];
    char app[30];
    memset(&QB_string, 0, sizeof(QB_string));
    QB_info(QB_string);
    ref0 = RestOffEnc[0];
    ref1 = RestOffEnc[1];
    unsigned short int ref00 = abs(ref0);
    unsigned short int ref11 = abs(ref1);
    sprintf(app, "^%03d^%c'%05d^%c'%05d", int(K_vib*10), (sign(ref0)), ref00, sign(ref1), ref11);
    send_info(QB_string, app);
*/
#else
    // To have a proper functionality even when no network link is established
//    connected = true;
//    file_idx = 0;

#endif

    // QB MOTORS CONNECTION INITIALIZATION AND SETUP
    init_motor();		// Switch motors on and drive it to rest position
    send_PWM(0);

    // LOAD CELL INITIALIZATION
    init_loadcell(&bridge);

    // CAMERA INITIALIZATION
    if (show_image){
        cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
    }

    // CAMERA AND VIDEO VARIABLES SETUP
    cam_img    = cvCreateImage(cvSize(VIDEO_WIDTH, VIDEO_HEIGHT), IPL_DEPTH_8U, 3);
#ifdef WEBCAM
	cam = cvCaptureFromCAM(0);
	setCameraProperties(cam, FPS, VIDEO_WIDTH, VIDEO_HEIGHT);
#endif

    // SET STIFFNESS AND LOAD INTERPOLATION PARAMETERS
    change_material_prop();


#ifdef ENABLE_NETWORK
    pthread_t finger_thr, motor_thr, packets_thr;
    if (pthread_create(&finger_thr, NULL, finger_code, (void*)0)){
	cout << "Error creating Finger code thread" << endl;
	return -1;
    }
    if (pthread_create(&motor_thr, NULL, motors, (void*)0)){
	cout << "Error creating Motor code thread" << endl;
	return -1;
    }

    if (pthread_create(&packets_thr, NULL, packets_code, (void*)&K_vib)){
	cout << "Error creating Packets code thread" << endl;
	return -1;
    }
#endif

    send_PWM(0);


    // Initialize temperature sensor
    init_temp();
    bool fan_enabled = false;
    bool en_fan = false;
    unsigned int i_fan = 0;
    send_PWM(0);
    send_PWM(0);
    while (connected_main){

        // Check temperature and (in case) activates fan
        double temp = read_temperature();
        // cout << "FYD-pad temperature: " << temp << " °C" << endl;
        if (temp > 34.0){
            if (!fan_enabled && i_fan >= 10){
                en_fan = true;
                i_fan = 0;
            }
            if (en_fan){
                cout << "High temperatures reached (" << temp << " °C) : Enable Fan" << endl;
                enable_fan();
                fan_enabled = true;
                en_fan = false;
            }
            else {
                i_fan++;
            }
        }
        else {
            if (fan_enabled){
                disable_fan();
                fan_enabled = false;
            }
        }
        sleep(5);

    };

    // Deactivate and close motor
    send_PWM(0);
    reset_motor();
    deactivate_motor();
    close_motor();
    disable_fan();
    close_fan();

	// Stop capture
	cvReleaseCapture(&cam);

    // Close load cell
//    close_loadcell(&bridge);

    // Detach and close Accelerometers
    CPhidget_close((CPhidgetHandle)accel);
    CPhidget_delete((CPhidgetHandle)accel);

    cout << "F.Y.D. application terminated successfully" << endl;
    return 0;
}
