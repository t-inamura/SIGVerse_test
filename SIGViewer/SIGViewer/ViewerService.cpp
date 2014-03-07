#include "ViewerService.h"
#include "binary.h"

namespace Sgv
{

	void ViewerService::recvMsg(sigverse::RecvMsgEvent &evt)
	{
		m_MsgEvt = evt;
		m_isMsg = true;

	}

	void ViewerService::recvCaptureView(sigverse::RecvCptEvent &evt)
	{
		m_requestType = 1;

		m_CptEvt.setCameraID(evt.getCameraID());
		m_CptEvt.setSender(evt.getSender());
	}
	void ViewerService::recvDistanceSensor(sigverse::RecvDstEvent &evt)
	{
		m_requestType = 2;

		m_DstEvt.setType(evt.getType());
		m_DstEvt.setMax(evt.getMax());
		m_DstEvt.setMin(evt.getMin());
		m_DstEvt.setCameraID(evt.getCameraID());
		m_DstEvt.setSender(evt.getSender());
		//char tmp[64];
		//sprintf(tmp, "id = %d, sender = %s", evt.getCameraID(), evt.getSender().c_str());
		//MessageBox( NULL, tmp, "Error", MB_OK);
	}
	void ViewerService::recvDetectEntities(sigverse::RecvDtcEvent &evt)
	{
		m_requestType = 3;

		m_DtcEvt.setCameraID(evt.getCameraID());
		m_DtcEvt.setSender(evt.getSender());

	}

}

