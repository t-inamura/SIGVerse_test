#pragma once
#include "Leap.h"
#include "Constants.h"
#include <cstdlib>
#include <cmath>

using namespace Leap;
using namespace std;

class MyListener : public Listener
{
	public:
		void onConnect(const Controller &controller);
		void onDisconnect(const Controller &controller);
		void onFrame(const Controller &controller);
		float* matrixToQuaternion(Matrix mat);
		Hand getHand(HANDS idHand);
		FingerList getFingerList(HANDS idHand);
		float getCurrentFrameRate();
	
	private:
		Hand rightHand;
		FingerList rightHandFingers;
		Hand leftHand;
		FingerList leftHandFingers;
		float currentFrameRate;
};
