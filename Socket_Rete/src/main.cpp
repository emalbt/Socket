#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/serial_port.hpp> 

#include "Network.h"
#include "Network_B.h"



using namespace std;

//NETWORK VARIABLES
const char* hostname;
char buf[MAXDATASIZE];
short int cmd;
bool connected = true;



const char* hostname_B;
char buf_B[MAXDATASIZE_B];
short int cmd_B;
bool connected_B = true;


//==================== SERIAL COMMUNICATION FUNCTIONS =========================
 boost::asio::serial_port* serialPort_;
 boost::asio::io_service ioService_;
 uint8_t* dataBuffer_; 
    
//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                                              initCOM
//------------------------------------------------------------------------------------------------------------------------------------------
void initCOM() 
{
    serialPort_ = (new boost::asio::serial_port(ioService_));
    serialPort_->close();
    serialPort_->open("/dev/ttyUSB0");
    serialPort_->set_option(boost::asio::serial_port_base::baud_rate(115200));
    serialPort_->set_option(boost::asio::serial_port_base::character_size(8));
    serialPort_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serialPort_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serialPort_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                                            sendCOM
//------------------------------------------------------------------------------------------------------------------------------------------
void sendCOM() 
{
    // std::string a = ":200:200:200:200:200:200:!";
    std::string a = buf;
    boost::asio::write(*serialPort_,boost::asio::buffer(a,26));
    // char str[3], str0[3];
    // sprintf(str, "%d", 200);
    //     sprintf(str0, "%d", 0);
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
    
}

void close_application()
{
      /* Close connection with F.Y.D. */
      close_connection();
      exit (0);
}

char x_;

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}



void key_control(){
    char buf_init[5];

    switch(x_)
    {
        case 'a': 
            std::cout << "\n\nInit hand A:\n\n" << std::endl;
            strncpy(buf_init, "init", 5);     // Initial handshake
            send_data(buf_init);
            break;
        case 'b': 
            std::cout << "\n\nInit hand B:\n\n" << std::endl;
            strncpy(buf_init, "init", 5);     // Initial handshake
            send_data_B(buf_init);
            break;       
    }
}



int main(int argc, char** argv) 
{
    

    hostname   = "192.168.1.42";
    hostname_B = "192.168.1.43";
           
    
    // Retrieve server address
    if (initialize_connection())
    {
        cout << "Error creating connection with " << hostname << endl;
        return 1;
    }

    if (initialize_connection_B())
    {
        cout << "Error creating connection with " << hostname_B << endl;
        return 1;
    }

    // initCOM();

    //Send material information to FYD-pad device
    // strncpy(buf, ":250:000:00:000:000:000:!",30);

    strncpy(buf, "init", 4);     // Initial handshake
    send_data(buf);
    set_recv_timeout();
    sleep(1);

    strncpy(buf_B, "init", 4);     // Initial handshake
    send_data_B(buf_B);
    set_recv_timeout_B();
    sleep(1);

    while(1)
    {    

        if (kbhit())
        {         
            x_ = getchar();
            key_control();
        }

        // std::cout <<"send" << std::endl;
        // send_data("init");
        receive_data();
        receive_data_B();
        // strncpy(buf, ":200:200:200:200:200:200:!",30);
        // sendCOM();
        
        std::cout << "A: " << buf << std::endl;
        std::cout << "B: " << buf_B << std::endl;
        
    }



    return 0;
}
