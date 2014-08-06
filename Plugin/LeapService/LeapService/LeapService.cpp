#include "LeapService.h"

using namespace Leap;
using namespace std;

LeapService::LeapService(void){
	listener = MyListener();
	ctrl = Controller(listener);
	setName("LeapService");
}

LeapService::~LeapService(void){
	ctrl.removeListener(listener);
}

double LeapService::onAction(){
	Hand rHand = listener.getHand(R_HAND);
	Hand lHand = listener.getHand(L_HAND);

	sendMsg("rightHand", buildMessageForHand(rHand));

	sendMsg("leftHand", buildMessageForHand(lHand));

	//cout << "LEFT CONF : " << lHand.confidence() << endl;
	//cout << "RIGHT CONF: " << rHand.confidence() << endl;

	return 4 / listener.getCurrentFrameRate();
}

void LeapService::onRecvMsg(sigverse::RecvMsgEvent &evt){
	cout << "SENDER: " << evt.getSender() << endl;
	cout << "MSG: " << evt.getMsg() << endl;
}

MyListener LeapService::getListener(){
	return listener;
}

Matrix LeapService::computeHandRotMatrix(Hand hand){
	float rotX = hand.palmNormal().pitch();
	float rotY = -hand.palmNormal().roll();
	float rotZ = hand.direction().yaw();
	
	/*float rotX = PI/2;
	float rotY = 0;
	float rotZ = 0;*/

	Matrix Rx = Matrix( Vector(1, 0, 0),
						Vector(0, cos(rotX), -sin(rotX)),
						Vector(0, sin(rotX), cos(rotX)));

	Matrix Ry = Matrix( Vector(cos(rotY), 0, sin(rotY)),
						Vector(0, 1, 0),
						Vector(-sin(rotY), 0, cos(rotY)));

	Matrix Rz = Matrix( Vector(cos(rotZ), -sin(rotZ), 0),
						Vector(sin(rotZ), cos(rotZ), 0),
						Vector(0, 0, 1));

	return Rx * Ry * Rz;
}

string LeapService::buildMessageForHand(Hand hand){
	stringstream ss (stringstream::in | stringstream::out);
	string msg = "";
	HANDS idHand = (hand.isLeft())? L_HAND : R_HAND;
	FingerList fingers = listener.getFingerList(idHand);
	
	if(hand.confidence() > 0.5){
		Matrix rotHand = computeHandRotMatrix(hand);
		float* q = listener.matrixToQuaternion(rotHand);
		Vector handPos = hand.palmPosition();
		
		// Hand's rotation
		ss << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "/";
		// Hand's position
		ss << handPos[0] << "," << handPos[1] << "," << handPos[2];
		
		for(FingerList::const_iterator fL = fingers.begin(); fL != fingers.end(); ++fL){
			Finger f = *fL;

			if(f.isValid()){
				ss << ";" << FINGER_NAMES[(int)f.type()] << ":";
				
				// for the thumb one less bone is defined on my hand (to change in future)
				int startingBone = (FINGER_NAMES[(int)f.type()] == "THUMB")? 2 : 1;

				for(int i=startingBone; i < 4; i++){
					Bone b = f.bone((Bone::Type)i);

					q = listener.matrixToQuaternion(b.basis().rigidInverse());

					ss << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "/";
				}

				ss.seekp(ss.str().length()-1);
				ss.clear();
			}
		}

		msg = ss.str().substr(0, ss.str().length() - 1);

		//cout << "MESSSAGE: " << msg << endl;
	}

	return msg;
}

int main(int argc, char** argv) {
	/*LeapService srv = LeapService();
	srv.getListener().matrixToQuaternion(srv.computeHandRotMatrix(Hand()));*/
	
	for(int i = 0; i < argc; i++){
		cout << i << " : " << argv[i] << endl;
	}

	LeapService srv = LeapService();  
	unsigned short port = atoi(argv[2]);

	while(!srv.getListener().getHand(R_HAND).isValid());
	//srv.connect("136.187.35.194", port);
	srv.connect(argv[1], port);
    srv.startLoop();
    
	return 0;
}