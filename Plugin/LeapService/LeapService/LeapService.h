#pragma once
#include "SIGService.h"
#include "MyListener.h"
#include "LeapMath.h"
#include <sstream>

using namespace Leap;
using namespace std;

class LeapService : public sigverse::SIGService
{
	public:
		LeapService(void);
		~LeapService(void);
		double onAction();
		void onRecvMsg(sigverse::RecvMsgEvent &evt);
		MyListener getListener();
		Matrix computeHandRotMatrix(Hand hand);
		string buildMessageForHand(Hand hand);

	private:
		MyListener listener;
		Controller ctrl;
		string name;
};
