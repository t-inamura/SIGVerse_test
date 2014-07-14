#ifndef _PSMOVEDATA_H_
#define _PSMOVEDATA_H_


#include <sstream>
#include <vector>

#define NBDATA 24


template <typename T>
struct Data3D {
  T x;
  T y;
  T z;
};

template <typename T>
struct Data4D : public Data3D<T> {
  T w;
};

struct MoveData {
  Data3D<int> m; // magnetometer
  Data3D<int> a; // accelerometer
  Data3D<int> g; // gyroscope
  Data3D<float> tracker; // tracker data
  int buttonsDown;
  int trigger;
  int battery;
  int celsius_temp;
  float trackerRadius;
  int id;
  Data4D<float> q; // orientation quaternion

  // button events
  unsigned int pressedButtons;
  unsigned int releasedButtons;
};




std::vector<MoveData> buildMoveData (std::string msg) {

  std::string parsedMessage(msg);
  std::vector<MoveData> moves;
  size_t found = parsedMessage.find_first_of("/");
  while (found != std::string::npos) {

    std::string moveString = parsedMessage.substr(0, found);

    MoveData moveData;
    for (int i = 0; i < NBDATA; ++i) {
      int dataFound = moveString.find_first_of(":");
      if (dataFound != std::string::npos) {
	std::string  data = moveString.substr(0, dataFound);
	std::stringstream convert(data);
	switch (i) {
	case 0:
	  convert >> moveData.m.x;
	  break;
	case 1:
	  convert >> moveData.m.y;
	  break;
	case 2:
	  convert >> moveData.m.z;
	  break;
	case 3:
	  convert >> moveData.a.x;
	  break;
	case 4:
	  convert >> moveData.a.y;
	  break;
	case 5:
	  convert >> moveData.a.z;
	  break;
	case 6:
	  convert >> moveData.g.x;
	  break;
	case 7:
	  convert >> moveData.g.y;
	  break;
	case 8:
	  convert >> moveData.g.z;
	  break;
	case 9:
	  convert >> moveData.buttonsDown;
	  break;
	case 10:
	  convert >> moveData.battery;
	  break;
	case 11:
	  convert >> moveData.celsius_temp;
	  break;
	case 12:
	  convert >> moveData.trigger;
	  break;
	case 13:
	  convert >> moveData.tracker.x;
	  break;
	case 14:
	  convert >> moveData.tracker.y;
	  break;
	case 15:
	  convert >> moveData.tracker.z;
	  break;
	case 16:
	  convert >> moveData.trackerRadius;
	  break;
	case 17:
	  convert >> moveData.id;
	  break;
	case 18:
	  convert >> moveData.q.w;
	  break;
	case 19:
	  convert >> moveData.q.x;
	  break;
	case 20:
	  convert >> moveData.q.y;
	  break;
	case 21:
	  convert >> moveData.q.z;
	  break;
	case 22:
	  convert >> moveData.pressedButtons;
	  break;
	case 23:
	  convert >> moveData.releasedButtons;
	  break;
	}
	moveString = moveString.substr(dataFound+1);
      }
    }
    moves.push_back(moveData);

    // next move string
    parsedMessage = parsedMessage.substr(found+1);
    found = parsedMessage.find_first_of("/");

  }

  return moves;
}


struct PSMoveArmCalibration {
  bool shoulderCalibrated;
  bool handExtensionCalibrated;
  Data3D<float> shoulder;
  Data3D<float> handExtension;

  float armLength;
};



#endif /* _PSMOVEDATA_H_ */
