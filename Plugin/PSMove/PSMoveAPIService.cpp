// cstd
#include <stdlib.h>
#include <unistd.h>

// std
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// OpenCV
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"

// PSMoveAPI
#include <psmoveapi/psmove.h>
#include <psmoveapi/psmove_tracker.h>

//SIGVerse
#include "SIGService.h"


//template <typename T> string tostr(const T& t) { ostringstream os; os << t; return os.str(); } // template to convert double variables to string



class PSMoveAPIService : public sigverse::SIGService
{
public:
  PSMoveAPIService(std::string name, std::vector<std::string> messageTargets, bool enableTracker = false);
  ~PSMoveAPIService();
  double onAction();
  void onRecvMsg(sigverse::RecvMsgEvent &evt);

private:
  std::vector<PSMove *> moves;
  enum PSMove_Connection_Type ctype;
  std::vector<std::string> targets;

  // tracker
  bool trackerEnabled;
  PSMoveTracker* tracker;

  // opencv frame
  void* frame;
};


PSMoveAPIService::PSMoveAPIService(std::string name,
				   std::vector<std::string> messageTargets, 
				   bool enableTracker) : 
  SIGService(name), 
  targets(messageTargets),
  trackerEnabled(enableTracker)

{


  if (!psmove_init(PSMOVE_CURRENT_VERSION)) {
    std::cerr << "PS Move API init failed (wrong version?)" << std::endl;
    exit(1);
  }

  int count = psmove_count_connected();
  std::cout << "Connected controllers: " << count << std::endl;


  // init psmove tracker
  if (trackerEnabled) {
    
    tracker = psmove_tracker_new();
    psmove_tracker_set_mirror(tracker, PSMove_True);

    std::cout << "Tracker ... OK" << std::endl;
  }


  for (int i = 0; i < count; ++i) {
    moves.push_back(psmove_connect_by_id(i));
    assert((moves[i]) != NULL);



    while (trackerEnabled) {
      std::cout << "Calibrating controller " << i << "..." << std::endl;
      int result = psmove_tracker_enable(tracker, moves[i]);
      
      if (result == Tracker_CALIBRATED) {
	enum PSMove_Bool auto_update_leds =
	  psmove_tracker_get_auto_update_leds(tracker,
					      moves[i]);
	std::cout << "OK, auto_update_leds is " 
		  << ((auto_update_leds == PSMove_True)?"enabled":"disabled")
		  << std::endl;
	break;
      } else {
	std::cout << "ERROR - retrying" << std::endl;
      }
    }
    
  }




  if ((moves.size() == 0) || (moves[0] == NULL)) {
    std::cout << "Could not connect to default Move controller.\n"
	      << "Please connect one via USB or Bluetooth." << std::endl;

    exit(1);
  }



  std::vector<PSMove*>::iterator move;

  for (move = moves.begin(); move != moves.end(); ++move) {
      
    char *serial = psmove_get_serial(*move);
    std::cout << "Serial : " << serial << std::endl;
    free(serial);

    ctype = psmove_connection_type(*move);
    switch (ctype) {
    case Conn_USB:
      std::cout << "Connected via USB." << std::endl;
      break;
    case Conn_Bluetooth:
      std::cout << "Connected via Bluetooth." << std::endl;
      break;
    case Conn_Unknown:
      std::cout << "Unknown connection type." << std::endl;
      break;
    }

    // for (int i = 0; i<10; i++) {
    //   psmove_set_leds(*move, 0, 255 * (i % 3 == 0), 0);
    //   psmove_set_rumble(*move, 255 * (i % 2));
    //   psmove_update_leds(*move);
    //   usleep(10 * (i % 10));
    // }

    // for (int i = 250; i >= 0; i -= 5) {
    //   psmove_set_leds(*move, i, i, 0);
    //   psmove_set_rumble(*move, 0);
    //   psmove_update_leds(*move);
    //   usleep(100);
    // }

    // psmove_set_leds(*move, 0, 0, 255);
    // psmove_update_leds(*move);
    // usleep(10000);

    // Enable rate limiting for LED updates
    psmove_set_rate_limiting(*move, PSMove_True);

    // psmove_set_leds(*move, 0, 0, 0);
    // psmove_set_rumble(*move, 0);
    // psmove_update_leds(*move);
  }
  

  std::cout << "End init PSMove" << std::endl;


};


PSMoveAPIService::~PSMoveAPIService()
{
  std::vector<PSMove*>::iterator move;
  for (move = moves.begin(); move != moves.end(); ++move) {
    psmove_disconnect(*move);      
  }
  disconnect();
  psmove_tracker_free(tracker);
}



void PSMoveAPIService::onRecvMsg(sigverse::RecvMsgEvent &evt)
{
}



double PSMoveAPIService::onAction()
{

  std::vector<PSMove*>::iterator move;
  std::stringstream msgStream;

  // std::cout << "moves.size() : " << moves.size() << std::endl;
  for (move = moves.begin(); move != moves.end(); ++move) {
  

    int pressed_buttons;

    if (ctype != Conn_USB && !((pressed_buttons = psmove_get_buttons(*move)) & Btn_PS)) {
      int res = psmove_poll(*move);
      // std::cout << "psmove_poll : " << res << std::endl;
      if (res) {


	/*
	  if (pressed_buttons & Btn_SQUARE) {
	  printf("Btn_SQUARE pressed, with trigger value: %d\n",
	  psmove_get_trigger(*move));
	  psmove_set_rumble(*move, psmove_get_trigger(*move));
	  }
	  else {
	  psmove_set_rumble(*move, 0x00);
	  }


	  //psmove_set_leds(*move, 0, 0, psmove_get_trigger(*move));
	  if (pressed_buttons & Btn_CIRCLE) {
	  psmove_set_leds(*move, 100, 0, 0);
	  }
	  else if (pressed_buttons & Btn_TRIANGLE) {
	  psmove_set_leds(*move, 0, 100, 0);
	  }
	  else if (pressed_buttons & Btn_CROSS) {
	  psmove_set_leds(*move, 0, 0, 100);
	  }
	*/

	int x, y, z;


	// Magnetometer
	psmove_get_magnetometer(*move, &x, &y, &z);
	// root["magnetX"] = x;
	// root["magnetY"] = y;
	// root["magnetZ"] = z;
	//printf("magnetometer: %5d %5d %5d\n", x, y, z);
	msgStream << x << ":" << y << ":" << z << ":";
	std::cout << "Magnetometer : " << x << ":" << y << ":" << z << ":" << std::endl;
			
	// Accelerometer
	psmove_get_accelerometer(*move, &x, &y, &z);
	// root["accelX"] = x;
	// root["accelY"] = y;
	// root["accelZ"] = z;
	//printf("accel: %5d %5d %5d\n", x, y, z);
	msgStream << x << ":" << y << ":" << z << ":";
	std::cout << "Accelerometer : " << x << ":" << y << ":" << z << ":" << std::endl;
			
	// Gyroscope
	psmove_get_gyroscope(*move, &x, &y, &z);
	// root["gyroX"] = x;
	// root["gyroY"] = y;
	// root["gyroZ"] = z;
	//printf("gyro: %5d %5d %5d\n", x, y, z);
	msgStream << x << ":" << y << ":" << z << ":";
	std::cout << "Gyroscope : " << x << ":" << y << ":" << z << ":" << std::endl; 
						
	// Buttons
	// root["buttons"] = psmove_get_buttons(*move);
	//printf("buttons: %x\n", psmove_get_buttons(*move));
	unsigned int buttons = psmove_get_buttons(*move); 
	msgStream << buttons << ":";
	std::cout << "buttons : " << buttons << std::endl;

	int battery = psmove_get_battery(*move);
	// root["battery"] = battery;
	msgStream << battery << ":";
	std::cout << "battery : " << battery << std::endl;

	/*
	  if (battery == Batt_CHARGING) {
	  printf("battery charging\n");
	  }
	  else if (battery == Batt_CHARGING_DONE) {
	  printf("battery fully charged (on charger)\n");
	  }
	  else if (battery >= Batt_MIN && battery <= Batt_MAX) {
	  printf("battery level: %d / %d\n", battery, Batt_MAX);
	  }
	  else {
	  printf("battery level: unknown (%x)\n", battery);
	  }
	*/

	// root["temp"] = psmove_get_temperature_in_celsius(*move);
	float celsius_temp = psmove_get_temperature_in_celsius(*move);
	msgStream << celsius_temp << ":";
	std::cout << "celsius_temp : " << celsius_temp << std::endl;

	int trigger = psmove_get_trigger(*move);
	msgStream << trigger << ":";
	std::cout << "trigger : " << trigger << std::endl;

	// psmove_update_leds(*move);


	// Tracker data

	if (trackerEnabled) {
	  psmove_tracker_update_image(tracker);
	  psmove_tracker_update(tracker, NULL);
	  psmove_tracker_annotate(tracker);

	  frame = psmove_tracker_get_frame(tracker);
	  if (frame) {
            cvShowImage("live camera feed", frame);
	  }

            /* Optional and not required by default (see auto_update_leds above)
	       unsigned char r, g, b;
	       psmove_tracker_get_color(tracker, controllers[i], &r, &g, &b);
	       psmove_set_leds(controllers[i], r, g, b);
	       psmove_update_leds(controllers[i]);
            */

	  float x, y, r, z;
	  psmove_tracker_get_position(tracker, *move, &x, &y, &r);
	  z = psmove_tracker_distance_from_radius(tracker, r);

	  msgStream << x << ":" << y << ":" << z << ":" << r << ":";
	  std::cout << "Tracker : " << x << ":" << y << ":" << z << ":" << r << ":" << std::endl;
	  // }
	} // trackerEnabled
	else {
	  msgStream << 0 << ":" << 0 << ":" << 0 << ":" << 0 << ":";
	  std::cout << "Tracker disabled !!! ------------------" << std::endl;
	  // close service
	}

	// add move separator
	msgStream << "/";


	// usleep(100000);


	psmove_update_leds(*move);
      
      } // psmove_poll

    
    } // connection type


  } // move iterator




  std::vector<std::string>::iterator target;
  std::string msg = msgStream.str();
  if (! msg.empty()) {
    std::cout << "[" << msgStream.str() << "]" << std::endl;
    for (target = targets.begin(); target != targets.end(); ++target) {
      sendMsg(*target, msgStream.str()); 
    }
  }
  else {
    // std::cout << "Empty message !" << std::endl;
  }

  return 0.00001;

}



int main(int argc, char** argv)
{
  // Create an instance of the service class with the specified service name
  if (argc < 4) {
    std::cerr << "Usage : \"PSMoveAPIService.sig host port [message targets]" << std::endl;
    return -1;
  }

  bool enableTracker = false;

  std::vector<std::string> targets;
  for (int i = 3; i < argc; ++i) {
    // options 
    if (std::string(argv[i]) == "--enable-tracker") {
      enableTracker = true;
      std::cout << "Tracker enabled." << std::cout;
    }
    // targets of the messages
    else {
      targets.push_back(argv[i]);
      std::cout << "the messages will be sent to |" << argv[i] << "|" << std::endl;
    }
  }

  PSMoveAPIService srv("PSMoveAPIService", targets, enableTracker);
  std::string host = argv[1];
  unsigned short port = (unsigned short)(atoi(argv[2]));
  srv.connect(host, port);
  srv.startLoop();

  return 0;
}



