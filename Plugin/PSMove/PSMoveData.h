#include <sstream>
#include <vector>

#define NBDATA 17


template <typename T>
struct Data3D {
  T x;
  T y;
  T z;
};

struct MoveData {
  Data3D<int> m; // magnetometer
  Data3D<int> a; // accelerometer
  Data3D<int> g; // gyroscope
  Data3D<float> tracker; // tracker data
  int buttons;
  int trigger;
  int battery;
  int celsius_temp;
  float trackerRadius;
};




std::vector<MoveData> buildMoveData (std::string msg) {

  std::string parsedMessage(msg);
  std::vector<MoveData> moves;
  size_t found = parsedMessage.find_first_of("/");
  while (found != std::string::npos) {
    // std::cout << "parsedMessage : " << parsedMessage << std::endl;
    // std::cout << "found : " << found << std::endl;
    std::string moveString = parsedMessage.substr(0, found);
    // std::cout << "moveString : " << moveString << std::endl;
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
	  convert >> moveData.buttons;
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
	}
	moveString = moveString.substr(dataFound+1);
      }
    }
    moves.push_back(moveData);

    // next move string
    parsedMessage = parsedMessage.substr(found+1);
    found = parsedMessage.find_first_of("/");
    // sleep(1);
  }

  // std::cout << "Finally parsed !" << std::endl;
  return moves;
}
