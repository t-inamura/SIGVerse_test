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
#include <psmoveapi/psmove_fusion.h>

//SIGVerse
#include "SIGService.h"



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
  PSMoveFusion* fusionTracker;

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
    fusionTracker = psmove_fusion_new(tracker, 1., 1000.);


    std::cout << "Tracker ... OK" << std::endl;
  }


  for (int i = 0; i < count; ++i) {
    moves.push_back(psmove_connect_by_id(i));
    assert((moves[i]) != NULL);

    // get orientation data
    psmove_enable_orientation(moves[i], PSMove_True);
    assert(psmove_has_orientation(moves[i]));


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


    // Enable rate limiting for LED updates
    psmove_set_rate_limiting(*move, PSMove_True);

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
  psmove_fusion_free(fusionTracker);
}



void PSMoveAPIService::onRecvMsg(sigverse::RecvMsgEvent &evt)
{
  std::cout << "incomming message" << std::endl;
  // std::string msg = evt.getMsg();
  // std::cout << "PSMove incomming message : " << msg << std::endl;
  // int idPos = msg.find_first_of(":");
  // std::string msgID = msg.substr(0, idPos);
  // if (msgID == "PSMOVE") {
  //   int callPos = msg.find(":", idPos+1);
  //   std::string remoteCall = msg.substr(idPos+1, callPos);
  //   if (remoteCall == "psmove_reset_orientation") {
  //     std::stringstream s_moveID(msg.substr(callPos+1));
  //     int moveID;
  //     s_moveID >> moveID;
  //     psmove_reset_orientation(moves[moveID]);
  //   }
  // }
}



double PSMoveAPIService::onAction()
{

  std::vector<PSMove*>::iterator move;
  std::stringstream msgStream;

  // get data from all connected move controllers
  for (move = moves.begin(); move != moves.end(); ++move) {


    int pressed_buttons;

    if (ctype != Conn_USB && !((pressed_buttons = psmove_get_buttons(*move)) & Btn_PS)) {
      // wait for new data
      while(psmove_poll(*move));

      // 3D data values
      int x, y, z;

      // Magnetometer
      psmove_get_magnetometer(*move, &x, &y, &z);
      msgStream << x << ":" << y << ":" << z << ":";
      // std::cout << "Magnetometer : " << x << ":" << y << ":" << z << ":" << std::endl;

      // Accelerometer
      psmove_get_accelerometer(*move, &x, &y, &z);
      msgStream << x << ":" << y << ":" << z << ":";
      // std::cout << "Accelerometer : " << x << ":" << y << ":" << z << ":" << std::endl;

      // Gyroscope
      psmove_get_gyroscope(*move, &x, &y, &z);
      msgStream << x << ":" << y << ":" << z << ":";
      // std::cout << "Gyroscope : " << x << ":" << y << ":" << z << ":" << std::endl;

      // Buttons
      unsigned int buttons = psmove_get_buttons(*move);
      msgStream << buttons << ":";
      // std::cout << "buttons : " << buttons << std::endl;

      int battery = psmove_get_battery(*move);
      msgStream << battery << ":";
      // std::cout << "battery : " << battery << std::endl;


      float celsius_temp = psmove_get_temperature_in_celsius(*move);
      msgStream << celsius_temp << ":";
      // std::cout << "celsius_temp : " << celsius_temp << std::endl;

      int trigger = psmove_get_trigger(*move);
      msgStream << trigger << ":";
      // std::cout << "trigger : " << trigger << std::endl;



      // Tracker data

      if (trackerEnabled) {
	float tX, tY, tZ, r = 0;
	psmove_tracker_get_position(tracker, *move, &tX, &tY, &r);
	psmove_fusion_get_position(fusionTracker, *move, &tX, &tY, &tZ);

	// need to call update functions on tracker after collecting data
	psmove_tracker_update_image(tracker);
	psmove_tracker_update(tracker, NULL);
	psmove_tracker_annotate(tracker);

	frame = psmove_tracker_get_frame(tracker);
	if (frame) {
	  cvShowImage("live camera feed", frame);
	}

	// z = psmove_tracker_distance_from_radius(tracker, r);

	msgStream << tX << ":" << tY << ":" << tZ << ":" << r << ":";
	// std::cout << "Tracker : " << tX << ":" << tY << ":" << tZ << ":" << r << ":" << std::endl;

      } // if(trackerEnabled)

      else {
	msgStream << 0 << ":" << 0 << ":" << 0 << ":" << 0 << ":";
	// std::cout << "Tracker disabled !!! ------------------" << std::endl;
      }

      // get move ID with iterator position
      msgStream << move - moves.begin() << ":";
      // std::cout << "Move ID : " << move - moves.begin() << std::endl;


      // get move orientation
      float qw, qx, qy, qz = 0;
      psmove_get_orientation(*move, &qw, &qx, &qy, &qz);
      msgStream << qw << ":" << qx << ":" << qy << ":" << qz << ":";
      // std::cout << "Quaternion : " << qw << ":" << qx << ":" << qy << ":" << qz << ":" << std::endl;


      // button events
      unsigned int pressed, released;
      psmove_get_button_events(*move, &pressed, &released);
      msgStream << pressed << ":" << released << ":";

      // reset orientation of the PSMove when pressing CROSS
      if (pressed & Btn_CROSS) {
	psmove_reset_orientation(*move);
      }


      // add move separator
      msgStream << "/";


      psmove_update_leds(*move);


    } // connection type


  } // move iterator




  // send msg to each target
  std::vector<std::string>::iterator target;
  std::string msg = msgStream.str();
  if (! msg.empty()) {
    // std::cout << "[" << msgStream.str() << "]" << std::endl;
    for (target = targets.begin(); target != targets.end(); ++target) {
      sendMsg(*target, msgStream.str());
    }
  }


  // std::cout << "return statement reached" << std::endl;
  // refresh faster if trackerEnabled
  if (trackerEnabled) {
    return 0.0015*moves.size();
  }

  return 0.001*moves.size();

}



int main(int argc, char** argv)
{
  // Create an instance of the service class with the specified service name
  if (argc < 4) {
    std::cerr << "Usage : $ PSMoveAPIService.sig host port [message targets]" << std::endl;
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
  if (srv.connect(host, port)) {
    srv.startLoop();
  }
  else {
    std::cerr << "Impossible to connect to server" << std::endl;
    return -1;
  }
  return 0;
}



