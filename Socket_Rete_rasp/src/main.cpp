// Standard include
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <eigen3/Eigen/Eigen>

#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/serial_port.hpp> 

// Other files include
#include "wireless_communicator.h"
#include "imuboard_communications.h"
#include "qbmove_communications.h"


// GLOBAL FLAGS
bool connected = true;
bool connected_main = true;
int increase_kvib = 0;
bool stop_flag;
bool send_info_flag = false;

char buf[MAXDATASIZE];


//==================== SERIAL COMMUNICATION FUNCTIONS =========================
 boost::asio::serial_port* serialPort_;
 boost::asio::io_service ioService_;
 uint8_t* dataBuffer_; 
	
//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                   initCOM
//------------------------------------------------------------------------------------------------------------------------------------------
void initCOM() {
	serialPort_ = (new boost::asio::serial_port(ioService_));
	serialPort_->close();
	serialPort_->open("/dev/ttyUSB1");
	serialPort_->set_option(boost::asio::serial_port_base::baud_rate(115200));
	serialPort_->set_option(boost::asio::serial_port_base::character_size(8));
	serialPort_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	serialPort_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	serialPort_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
	
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                   sendCOM
//------------------------------------------------------------------------------------------------------------------------------------------
void sendCOM() {
	// char str[3], str0[3];
	// sprintf(str, "%d", 200);
    // sprintf(str0, "%d", 0);
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(str,3));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(str,3));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(str,3));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(str,3));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(str,3));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char(':'),1));
	// boost::asio::write(*serialPort_,boost::asio::buffer(new char('!'),1));
	
	std::string a;
	a = buf;
	boost::asio::write(*serialPort_,boost::asio::buffer(a,27));
}


//=========================== IMU FUNCTIONS ===================================
comm_settings  comm_settings_t_;
int id_ = 1;
float*   imu_values_;
int      n_imu_;
uint8_t* ids_;
uint8_t* imu_table_;
uint8_t* mag_cal_;

Eigen::MatrixXd acc_;
Eigen::VectorXd acc_norm_start_error_;
Eigen::VectorXd acc_norm_current_error_;

Eigen::MatrixXd gyro_;



//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                              initImuBoard
//------------------------------------------------------------------------------------------------------------------------------------------
void initImuBoard() {
		

	uint8_t aux_string[2000];
	uint8_t PARAM_SLOT_BYTES = 50;
	int num_of_params;
	
	do{
		commGetParamList(&comm_settings_t_ , id_, 0, NULL, 0, 0, aux_string);
		usleep(10000);
		num_of_params = aux_string[5];	
	
		//aux_string[6] <-> packet_data[2] on the firmware
		n_imu_ = aux_string[1*PARAM_SLOT_BYTES + 8];
		printf("Number of connected IMUs: %d\n", n_imu_);
	}while(n_imu_!=REFERENCE_IMU+1);
	
	ids_ = (uint8_t *) calloc(n_imu_, sizeof(uint8_t));
	for (int i=0; i< n_imu_; i++){
		ids_[i] = aux_string[2*PARAM_SLOT_BYTES + 8 + i];
	}
	
	// Retrieve magnetometer calibration parameters
	mag_cal_ = (uint8_t *) calloc(n_imu_, 3*sizeof(uint8_t));
	for (int i=0; i< n_imu_; i++){
		mag_cal_[3*i + 0] = aux_string[3*PARAM_SLOT_BYTES + 8 + 3*i];
		mag_cal_[3*i + 1] = aux_string[3*PARAM_SLOT_BYTES + 9 + 3*i];
		mag_cal_[3*i + 2] = aux_string[3*PARAM_SLOT_BYTES + 10 + 3*i];
		printf("MAG PARAM: %d %d %d\n", mag_cal_[3*i + 0], mag_cal_[3*i + 1], mag_cal_[3*i + 2]);
		
	}
	
	imu_table_ = (uint8_t *) calloc(n_imu_, 5*sizeof(uint8_t));
	for (int i=0; i< n_imu_; i++){
		imu_table_[5*i + 0] = aux_string[4*PARAM_SLOT_BYTES + 8 + 50*i];
		imu_table_[5*i + 1] = aux_string[4*PARAM_SLOT_BYTES + 9 + 50*i];
		imu_table_[5*i + 2] = aux_string[4*PARAM_SLOT_BYTES + 10 + 50*i];
		printf("ID: %d  - %d, %d, %d, %d, %d\n", ids_[i], imu_table_[5*i + 0], imu_table_[5*i + 1], imu_table_[5*i + 2], imu_table_[5*i + 3], imu_table_[5*i + 4]);
		
	}
	
	// Imu values is a 3 sensors x 3 axes x n_imu_ values
	imu_values_ = (float *) calloc(n_imu_, 3*3*sizeof(float));

	acc_.resize(n_imu_,3);
	gyro_.resize(n_imu_,3);
	acc_norm_start_error_.resize(n_imu_);
	acc_norm_current_error_.resize(n_imu_);
}



//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                              readImuBoard
//------------------------------------------------------------------------------------------------------------------------------------------
void readImuBoard()
{
	commGetImuReadings(&comm_settings_t_ , id_, imu_table_, mag_cal_, n_imu_, imu_values_);
			
	for (int i = 0; i < n_imu_; i++) 
	{
		if (imu_table_[5*i + 0]){
			//printf("Accelerometer\n");
			// printf("%d \t %f, %f, %f\n",i, imu_values_[3*3*i], imu_values_[3*3*i+1], imu_values_[3*3*i+2]);
			acc_(i,0) = imu_values_[3*3*i];
			acc_(i,1) = imu_values_[3*3*i+1];
			acc_(i,2) = imu_values_[3*3*i+2];	
		}
		if (imu_table_[5*i + 1]){
			//printf("Gyroscope\n");
			//printf("%f, %f, %f\n", imu_values_[3*3*i+3], imu_values_[3*3*i+4], imu_values_[3*3*i+5]);
			gyro_(i,0) = imu_values_[3*3*i+3];
			gyro_(i,1) = imu_values_[3*3*i+4];
			gyro_(i,2) = imu_values_[3*3*i+5];		
		}
		if (imu_table_[5*i + 2] ){
			//printf("Magnetometer\n");
			//printf("%f, %f, %f\n", imu_values_[3*3*i+6], imu_values_[3*3*i+7], imu_values_[3*3*i+8]);
		}	
		//printf("\n");
	}
}


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                 filterAcc
//------------------------------------------------------------------------------------------------------------------------------------------
void filterAcc(char* str_out)
{
	Eigen::VectorXd diff_norm(n_imu_-1);
	char str_send[MAXDATASIZE]; 
      	double touch[REFERENCE_IMU];
	
	Eigen::VectorXd gain(n_imu_-1);
	gain << 1,1,1,1,1,1;
	
	for(int i=0; i<n_imu_-1; i++)
	{
		diff_norm(i) =  std::abs (acc_.row(REFERENCE_IMU).norm() - acc_.row(i).norm() );
		
		if(acc_.row(REFERENCE_IMU).norm() <0.3 || acc_.row(i).norm()<0.3)
			diff_norm(i)=0;					
		
   		std::cout << "diff norm " << i << "   " << acc_.row(REFERENCE_IMU).norm() << " -  " << acc_.row(i).norm() << " = " << diff_norm(i) << std::endl; 
	}
	
	float sum_touch=0;
	for(int i =0; i<n_imu_-1; i++)
	{
		touch[i] = 1000 * diff_norm(i) * gain(i);
		//std::cout << "touch   " << i << "   " <<  touch[i]  << std::endl;
		if(touch[i]>250)
			touch[i]=250;
	}


	create_packet_imu(str_send,  touch);
	std::cout << "str send: " << str_send << std::endl;

	strcpy(str_out, str_send);
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                 	         initialErrorAcc
//------------------------------------------------------------------------------------------------------------------------------------------
void initialErrorAcc()
{
	float initial_error_sum = 0;
	Eigen::Vector3d acc_tmp, acc_reference;
	
	do 
	{
		initImuBoard();
		usleep(10000);
		readImuBoard(); 
		std::cout << "accelerazione ref  " << acc_(REFERENCE_IMU,0) << "  " << acc_(REFERENCE_IMU,1) << "  " << acc_(REFERENCE_IMU,2) <<  "  " << acc_.row(REFERENCE_IMU).norm() << std::endl;
 	}while( acc_(REFERENCE_IMU,0) == 0 );	

	acc_reference = acc_.row(REFERENCE_IMU);
	acc_reference = acc_reference / acc_reference.norm();  
	// Compute initial errors
	for(int i=0; i<n_imu_-1; i++) 
	{
		acc_tmp = acc_.row(i);
		float norm_tmp = acc_tmp.norm();
		acc_tmp = acc_tmp / norm_tmp; 		
		acc_norm_start_error_(i) = (acc_tmp.cross(acc_reference)).norm();

		std::cout << "Start Error " << i << "   " <<  acc_norm_start_error_(i)  << std::endl;
		initial_error_sum = initial_error_sum + acc_norm_start_error_(i);
	}
	std::cout << "Initial Error Sum" << "   " <<  initial_error_sum  << std::endl;
	
	getchar();
}



//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                      filterAccOrientation
//------------------------------------------------------------------------------------------------------------------------------------------
void filterAccOrientation(char* str_out)
{
	Eigen::VectorXd diff_norm(n_imu_-1);
	Eigen::VectorXd gain(n_imu_-1);
	Eigen::Vector3d acc_tmp, acc_reference;
	double touch[REFERENCE_IMU];
	char str_send[MAXDATASIZE];

	gain << 1,1,1,1.3,0.1,0.1;
	acc_reference = acc_.row(REFERENCE_IMU);
	acc_reference = acc_reference / acc_reference.norm();  
	// Compute current errors	
	for(int i=0; i<n_imu_-1; i++) 
	{
		acc_tmp = acc_.row(i) ;
		float norm_tmp = acc_tmp.norm();
		acc_tmp = acc_tmp / norm_tmp; 		
		acc_norm_current_error_(i) = (acc_tmp.cross(acc_reference)).norm();
		// std::cout << "acc  " << i << "\n " <<  acc_tmp  << std::endl;
	}	

	for(int i=0; i<n_imu_-1; i++)
	{
		diff_norm(i) =  std::abs ( acc_norm_current_error_(i) - acc_norm_start_error_(i) );		
	
		std::cout << "norm  acc " << i << " " <<  acc_norm_current_error_(i)  << std::endl;	 	
		std::cout << "norm  ref  " << i << " " <<  acc_norm_start_error_(i)  << std::endl;	 	
		std::cout << "norm  dif  " << i << " " <<  diff_norm(i)  << std::endl;	 	
		//getchar();	
	}

	for(int i =0; i<n_imu_-1; i++)
	{
		touch[i] = 1000 * diff_norm(i) * gain(i);
		////std::cout << "touch   " << i << "   " <<  touch[i]  << std::endl;
		if(touch[i]>250) 
			touch[i]=250; 
		if(touch[i]<80) 
			touch[i]=0;  
	}

	create_packet_imu(str_send,  touch);
	std::cout << "str send: " << str_send << std::endl;

	strcpy(str_out, str_send);
}




// =============================================================================================
//																				 MadgwickGeneral
// =============================================================================================
Eigen::Vector4d q0_, q1_, q0_off_, q1_off_;
float beta_ = 2.0;
float sampleFreq_ = 150;
float thGyro_ = 10;

Eigen::Vector4d ConjQ(Eigen::Vector4d Q_in) 
{
	Eigen::Vector4d Q_out;
	Q_out(0) =  Q_in(0);
	Q_out(1) = -Q_in(1);
	Q_out(2) = -Q_in(2);
	Q_out(3) = -Q_in(3);
	return Q_out;
}

Eigen::Vector4d QxQ(Eigen::Vector4d Q_1, Eigen::Vector4d Q_2) 
{
    Eigen::Vector4d Q_out;
		Q_out(0) = Q_1(0)*Q_2(0) - (Q_1(1)*Q_2(1) + Q_1(2)*Q_2(2) + Q_1(3)*Q_2(3));
		Q_out(1) = Q_1(0)*Q_2(1) + Q_1(1)*Q_2(0) + (Q_1(2)*Q_2(3) - Q_1(3)*Q_2(2));
		Q_out(2) = Q_1(0)*Q_2(2) + Q_1(2)*Q_2(0) + (Q_1(3)*Q_2(1) - Q_1(1)*Q_2(3));
		Q_out(3) = Q_1(0)*Q_2(3) + Q_1(3)*Q_2(0) + (Q_1(1)*Q_2(2) - Q_1(2)*Q_2(1));
	return Q_out;
}



Eigen::Vector4d MadgwickGeneral(int P, int N, Eigen::Vector4d qL)
{
	//come N vede P;  N è il mio d (wordl), P è il mio s (sensor)
	// ad esempio N=imu1, P=imu0, quindi come la IMU 1 vede la IMU 0 
	float q1, q2 ,q3 ,q4;
    
    float dx, dy, dz;
    float sx, sy, sz;

    Eigen::Vector3d aP, aN;
    Eigen::Vector4d gP, gN, g;

    Eigen::Vector3d fa;
    Eigen::MatrixXd Ja(3,4); 
    Eigen::Vector4d  qdot;

   	Eigen::Vector4d Napla;


	aP(0)  = acc_(P,0);  
	aP(1)  = acc_(P,1);  
	aP(2)  = acc_(P,2);

	aN(0)  = acc_(N,0);  
	aN(1)  = acc_(N,1);  
	aN(2)  = acc_(N,2);
 	
 	aP = aP / aP.norm();
	aN = aN / aN.norm();


	gP(0)  = 0; 
	gP(1)  = gyro_(P,0);  
	gP(2)  = gyro_(P,1);  
	gP(3)  = gyro_(P,2);

    gN(0)  = 0; 
    gN(1)  = gyro_(N,0);  
    gN(2)  = gyro_(N,1);  
    gN(3)  = gyro_(N,2);
	
	gP = gP*(M_PI/180);
	gN = gN*(M_PI/180);

	q1 = qL(0);  
	q2 = qL(1); 
	q3 = qL(2); 
	q4 = qL(3);

	// rotate the angular velocity
	g = QxQ(QxQ(qL, gP), ConjQ(qL)) - gN;	
	

	//accelerometer
	dx = aN(0); 
	dy = aN(1); 
	dz = aN(2); 
	
	sx = aP(0); 
	sy = aP(1); 
	sz = aP(2); 
	
	fa(0) =  2*dx*(0.5 -q3*q3 -q4*q4) + 2*dy*(q1*q4 + q2*q3) + 2*dz*(q2*q4-q1*q3) - sx; 
	fa(1) =  2*dx*(q2*q3 -q1*q4) + 2*dy*(0.5 - q2*q2 - q4*q4) + 2*dz*(q1*q2 + q3*q4) - sy;
	fa(2) =  2*dx*(q1*q3 -q2*q4) + 2*dy*(q3*q4 - q1*q2) + 2*dz*(0.5 - q2*q2 -q3*q3) - sz; 

	// Compute the Jacobian
	Ja << 2*dy*q4-2*dz*q3,    2*dy*q3+2*dz*q4 ,        -4*dx*q3+2*dy*q2-2*dz*q1,  -4*dx*q4+2*dy*q1+2*dz*q2,
		  -2*dx*q4+2*dz*q2,   2*dx*q3-4*dy*q2+2*dz*q1, 2*dx*q2+2*dz*q4,           -2*dx*q1-4*dy*q4+2*dz*q3,
		  2*dx*q3-2*dy*q2,    2*dx*q4-2*dy*q1-4*dz*q2, 2*dx*q1+2*dy*q4-4*dz*q3,   2*dx*q2+2*dy*q3;


	// Compute the Napla
  	Napla = Ja.transpose() * fa;

				    // Aggiunta per il Motion_Detection              
					// switch (P) {
					// case 0: napla_little = Napla.norm();
					// case 3: napla_ring = Napla.norm();
					// case 6: napla_middle = Napla.norm();
					// case 9: napla_index = Napla.norm();
					// case 12: napla_thumb = Napla.norm();
					// }

					//qDot
	// if (Napla.norm() > 1) {
	// 	qdot = 0.5 * QxQ(qL,g) - (beta_*  (Napla/Napla.norm()));
	//  } else {
	// 	qdot = 0.5 * QxQ(qL,g) - (beta_ *  Napla);
	// }

	qdot = 0.5*QxQ( qL,g ) - ( beta_*Napla );					// XXX VERIFICARE QUESTO MEGLIO 


	qL = qL + qdot / sampleFreq_; 

	qL = qL /qL.norm();

	return qL;
} 

Eigen::Vector3d quat2angle(Eigen::Vector4d qL)
{
	Eigen::Vector3d angle;
	angle(0) = atan2(2*qL(1)*qL(2) - 2*qL(0)*qL(3), 2*qL(0)*qL(0) + 2*qL(1)*qL(1)-1); //YAW
	angle(1) = -asin(2*qL(1)*qL(3) + 2*qL(0)*qL(2)); //PITCH
 	angle(2) = atan2(2*qL(2)*qL(3) - 2*qL(0)*qL(1), 2*qL(0)*qL(0) + 2*qL(3)*qL(3)-1); // ROLL


 	// float w = qL(0);
	// float x = qL(1);
	// float y = qL(2);
	// float z = qL(3);
	// pitch singularity
	// IMUs_Angles_(k,0) = atan2(2*Q_joint_(k,1)*Q_joint_(k,2) - 2*Q_joint_(k,0)*Q_joint_(k,3), 2*Q_joint_(k,0)*Q_joint_(k,0) + 2*Q_joint_(k,1)*Q_joint_(k,1)-1); //YAW
	// IMUs_Angles_(k,1) = -asin(2*Q_joint_(k,1)*Q_joint_(k,3) + 2*Q_joint_(k,0)*Q_joint_(k,2)); //PITCH
	// IMUs_Angles_(k,2) = atan2(2*Q_joint_(k,2)*Q_joint_(k,3) - 2*Q_joint_(k,0)*Q_joint_(k,1), 2*Q_joint_(k,0)*Q_joint_(k,0) + 2*Q_joint_(k,3)*Q_joint_(k,3)-1); // ROLL
	// 	IMUs_Angles_(k,0) = atan2(Q_joint_(k,1)*Q_joint_(k,2) + 2*Q_joint_(k,2)*Q_joint_(k,3), 0.5 - Q_joint_(k,2)*Q_joint_(k,2) - 2*Q_joint_(k,3)*Q_joint_(k,3)); //YAW
	//  IMUs_Angles_(k,1) = asin(-2* (Q_joint_(k,1)*Q_joint_(k,3) + Q_joint_(k,0)*Q_joint_(k,2))); //PITCH
	// 	IMUs_Angles_(k,2) = atan2(Q_joint_(k,0)*Q_joint_(k,1) + Q_joint_(k,2)*Q_joint_(k,3), 0.5 - Q_joint_(k,1)*Q_joint_(k,1) - Q_joint_(k,2)*Q_joint_(k,2)); // ROLL
    	
	// yaw singularity
	// angle(0) =  asin(2*x*y + 2*z*w); //YAW
	// angle(2) = - atan2(2*x*w - 2*y*z, 1-2*x*x - 2*z*z); //PITCH
	// angle(1) = - atan2(2*y*w - 2*x*z, 1-2*y*y - 2*z*z); // ROLL

 	return angle;
}



void initialMagdwicFilter()
{
	do 
	{
		initImuBoard();
		usleep(10000);
		readImuBoard(); 
		std::cout << "acc ref  " << acc_(REFERENCE_IMU,0) << "  " << acc_(REFERENCE_IMU,1) << "  " << acc_(REFERENCE_IMU,2) <<  "  " << acc_.row(REFERENCE_IMU).norm() << std::endl;
 	}while( acc_(REFERENCE_IMU,0) == 0 );	

	q0_.setZero();
	q1_.setZero();
	q0_off_.setZero();
	q1_off_.setZero();

	q0_(0) = 1;
	q1_(0) = 1;
	q0_off_(0) = 1;
	q1_off_(0) = 1;

	for(int i=0; i<200; i++)
	{
		readImuBoard();
		q0_off_ = MadgwickGeneral(0,1,q0_off_);
		q1_off_ = MadgwickGeneral(1,2,q1_off_);
		std::cout << "step " << i << std::endl;
	}

	Eigen::Vector3d a0,a1;
	a0   = quat2angle(q0_off_);
	a1   = quat2angle(q1_off_);

	a0   = a0 *(180/M_PI);
	a1   = a1 *(180/M_PI);

	std::cout << "a0:  " << a0(0) << "\t" << a0(1) << "\t" << a0(2) << std::endl;
	std::cout << "a1:  " << a1(0) << "\t" << a1(1) << "\t" << a1(2) << std::endl;
	std::cout << "\n";

	getchar();

}


bool var_ = 1;
Eigen::Vector4d offsetCorrector(Eigen::Vector4d q_off, Eigen::Vector4d qL)
{
	Eigen::Vector4d q_joint;

	q_joint(0) = q_off(0)*qL(0) - (-q_off(1)*qL(1) - q_off(2)*qL(2) - q_off(3)*qL(3));
	q_joint(1) = q_off(0)*qL(1) - q_off(1)*qL(0) - q_off(2)*qL(3) + q_off(3)*qL(2);
	q_joint(2) = q_off(0)*qL(2) - q_off(2)*qL(0) - q_off(3)*qL(1) + q_off(1)*qL(3);
	q_joint(3) = q_off(0)*qL(3) - q_off(3)*qL(0) - q_off(1)*qL(2) + q_off(2)*qL(1);
	
  	return q_joint;
}


void filterAngles(Eigen::Vector3d a0, Eigen::Vector3d a1, char* str_out)
{
	Eigen::VectorXd diff_norm(6);
	diff_norm.setZero();	
	Eigen::VectorXd gain(6);
	double touch[6];
	char str_send[MAXDATASIZE];

	gain << 1,1,1,1,1,1;
	a0(0) = 0;
	a1(0) = 0;
	
	float n0 = a0.norm();
	float n1 = a1.norm();

//	diff_norm(3) = n0+n1;
 	diff_norm(3) = 222;
 
 	//std::cout << "diff_norm " << diff_norm << std::endl;
	
	for(int i =0; i<6; i++)
	{
/*		if(i==3)
		{
			touch[i] = 3 * diff_norm(i) * gain(i);
			std::cout << "touch   " << i << "   " <<  touch[i]  << std::endl;
			if(touch[i]>250) 
				touch[i]=250; 
			if(touch[i]<80) 
				touch[i]=0;  
		}
		else
			touch[i] = 0;
*/	
	touch[i] = 222*var_;
	}

	create_packet_imu(str_send,  touch);
	std::cout << "str send: " << str_send << std::endl;
	strcpy(str_out, str_send);

}















//================================ MAIN ========================================
int main(int argc, char** argv){


	/*** COM SETUP ***/
	//std::cout << "COM START\n\n\n" << std::endl;	
	//initCOM();

	/*** IMU SETUP ***/
	//std::cout << "IMU ACQUISITION START\n\n\n" << std::endl;
	//openRS485(&comm_settings_t_, "/dev/ttyUSB0", 2000000 );
	//initImuBoard();
	//sleep(1);
	// initialErrorAcc();
	//initialMagdwicFilter();
	


	/*** wifi CONNECTION SETUP ***/
 	std::cout << "wifi CONNECTION START\n\n\n" << std::endl;	
	char ip_pc[30] = "192.168.1.50";
	if (initialize_connection(ip_pc) ){
		cout << "Error on connection creation" << endl;
		return 1;
	}
	receive_data();
	std::cout << "Init: " << buf << std::endl;	
	
	Eigen::Vector4d q0_j, q1_j; 
	Eigen::Vector3d a0, a1;

	//strncpy(buf, ":111:111:111:111:111:111:!",27);
	/***  LOOP ***/
	while(1)
	{               
/*		readImuBoard();

		for (int i=0; i<n_imu_; i++)
		{
			if (std::abs(gyro_(i,0))< thGyro_)  gyro_(i,0) = 0;		  
			if (std::abs(gyro_(i,1))< thGyro_)  gyro_(i,1) = 0;
			if (std::abs(gyro_(i,2))< thGyro_)  gyro_(i,2) = 0;
		}
*/
        	//filterAcc(buf);
		// filterAccOrientation(buf);
	
		//if(wifi_event() )
		//	initialErrlo	orAcc();          

		//usleep(2000);
		// sendCOM();


		/*****   Magdwick Filter *****/
/*		q0_  = MadgwickGeneral(0,1, q0_);
		q1_  = MadgwickGeneral(1,2, q1_);

		q0_j = offsetCorrector(q0_off_, q0_);
		q1_j = offsetCorrector(q1_off_, q1_);

		a0   = quat2angle(q0_j);
		a1   = quat2angle(q1_j);

		a0   = a0 *(180/M_PI);
		a1   = a1 *(180/M_PI);

		std::cout << "a0:  " << a0(0) << "\t" << a0(1) << "\t" << a0(2) << std::endl;
		std::cout << "a1:  " << a1(0) << "\t" << a1(1) << "\t" << a1(2) << std::endl;
		std::cout << "\n";
*/
		filterAngles(a0, a1, buf);
		send_data(buf);      
		
		if(receive_wifi_event())
		{
			var_ = !var_;
			std::cout<< "\n\n\n\n\n INIT GLOVE \n\n\n\n\n";		
		}
		
		usleep(2000);
	}	

	
        closeRS485(&comm_settings_t_);
    	
    return 0;
}
