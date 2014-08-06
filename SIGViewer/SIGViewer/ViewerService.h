
#ifndef __VIEWER_SERVICE__
#define __VIEWER_SERVICE__

#include "SIGService.h"

namespace Sgv
{
	class ViewerService : public sigverse::SIGService
	{
	public:
		ViewerService(std::string name): sigverse::SIGService(name),
			m_isMsg(false), m_requestType(NULL){}
		~ViewerService()
		{
			//sigverse::SgvSocket *sgvSock= getSgvSocket();
			//delete sgvSock;
		}


		void recvMsg(sigverse::RecvMsgEvent &evt);

		void recvCaptureView(sigverse::RecvCptEvent &evt);

		void recvDistanceSensor(sigverse::RecvDstEvent &evt);

		void recvDetectEntities(sigverse::RecvDtcEvent &evt);

		bool isMsg(){return m_isMsg;}

		sigverse::RecvMsgEvent *getRecvMsgEvent(){ 
			if(!m_isMsg) return NULL;
			else { 
				m_isMsg = false;
				return &m_MsgEvt;
			}
		}
		int getRequestType(){return m_requestType;}

		void setRequestType(int type){ m_requestType = type;}

		sigverse::RecvCptEvent getCptEvent(){return m_CptEvt;}

		sigverse::RecvDstEvent getDstEvent(){return m_DstEvt;}

		sigverse::RecvDtcEvent getDtcEvent(){return m_DtcEvt;}

	private:

		bool m_isMsg;
		sigverse::RecvMsgEvent m_MsgEvt;
		sigverse::RecvCptEvent m_CptEvt;
		sigverse::RecvDstEvent m_DstEvt;
		sigverse::RecvDtcEvent m_DtcEvt;

		int m_requestType;

	};
}

#endif

