#include "MyListener.h"

void MyListener::onConnect(const Controller &controller){
	currentFrameRate = 25;
	rightHand = Hand();
	rightHandFingers = FingerList();
	leftHand = Hand();
	leftHandFingers  = FingerList();
	cout << "Leap motion connected!" << endl;
}

void MyListener::onDisconnect(const Controller &controller){
	cout << "Leap motion disconnected!" << endl;
}

void MyListener::onFrame(const Controller &controller){
	Frame curFrame = controller.frame();
	
	//in order to synchronize onFrame function and onAction function
	currentFrameRate = curFrame.currentFramesPerSecond();

	//detecting if the current frame detects hands
	if (!curFrame.hands().isEmpty() && curFrame.hands().count() == 2) {
		Hand hand1 = curFrame.hands()[0];
		Hand hand2 = curFrame.hands()[1];

		if(hand1.isLeft()){
			leftHand = hand1;
			rightHand = hand2;
		}

		else{
			leftHand = hand2;
			rightHand = hand1;
		}

		FingerList rightFingers = rightHand.fingers();
		FingerList leftFingers = leftHand.fingers();

		if (!rightFingers.isEmpty() && rightFingers.count() == 5){
			rightHandFingers = rightFingers;
		}

		if (!leftFingers.isEmpty() && leftFingers.count() == 5){
			leftHandFingers = leftFingers;
		}
	}

	else{
		rightHand = Hand::invalid();
		leftHand = Hand::invalid();
	}
}

float* MyListener::matrixToQuaternion(Matrix mat){
	float* res = (float*) calloc(4, sizeof(float));
	float* rot = mat.toArray3x3();
	
	//qw calculation
	res[0] = sqrt(1 + rot[0] + rot[4] + rot[8]) / 2;
	float w4 = 4 * res[0];

	//qx calculation
	res[1] = (rot[7] - rot[5]) / w4;
	
	//qy calculation
	res[2] = (rot[2] - rot[6]) / w4;
	
	//qz calculation
	res[3] = (rot[1] - rot[3]) / w4;

	/*cout << "qw = " << res[0] << endl;
	cout << "qx = " << res[1] << endl;
	cout << "qy = " << res[2] << endl;
	cout << "qz = " << res[3] << endl;*/

	return res;
}

Hand MyListener::getHand(HANDS idHand){
	if(idHand == R_HAND){
		return rightHand;
	}

	return leftHand;
}

FingerList MyListener::getFingerList(HANDS idHand){
	if(idHand == R_HAND){
		return rightHandFingers;
	}

	return leftHandFingers;
}

float MyListener::getCurrentFrameRate(){
	return currentFrameRate;
}
	