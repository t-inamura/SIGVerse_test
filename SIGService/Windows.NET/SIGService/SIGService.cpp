
#include "stdafx.h"

#include "SIGService.h"

#include "binary.h"

#include <time.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>

#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;

namespace sigverse
{
	char *StringToChar(String^ str)
	{
		char *p = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
		return p;
	}

	bool RecvMsgEvent::setData(String^ data, int size)
	{

		cli::array<String^>^ SepString = {",",";"};
		cli::array<String^>^ all = data->Split(SepString, System::StringSplitOptions::None);
		this->m_from = all[0];

		int msgSize = Convert::ToInt32(all[1]);
		m_msg = gcnew String(all[2]->ToCharArray(0, msgSize));
		return true;
	}

	SIGService::SIGService(String^ name)
	{
		m_sgvsock = gcnew SgvSocket();
		m_sgvsock->initWinsock();

		m_name = name;
		m_connectedView = false;
		m_viewsock = nullptr;

		m_autoExitLoop = false;
		m_autoExitProc = false;
		m_onLoop = false;
	}

	SIGService::SIGService()
	{
		m_sgvsock = gcnew SgvSocket();
		m_sgvsock->initWinsock();

		m_connectedView = false;
		m_viewsock = nullptr;

		m_autoExitLoop = false;
		m_autoExitProc = false;
		m_onLoop = false;
	}

	SIGService::~SIGService()
	{
		delete m_sgvsock;

		if(m_viewsock != nullptr)
			delete m_viewsock;
	}

	bool SIGService::connect(String^ host, int port)
	{
		char *host_p = StringToChar(host);
		if(m_sgvsock->connectTo(host_p, port)) {

			String^ tmp = gcnew String("SIGMESSAGE," + m_name + ",");
			char tmp2[128];
			sprintf_s(tmp2, 128, "%s", tmp);
			m_sgvsock->sendData(tmp2, tmp->Length);
		}
		else{
			MessageBox::Show("Could not connect to simserver", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return false;
		}
		char tmp[8];
		memset(tmp, 0, sizeof(tmp));

		if(recv(m_sgvsock->getSocket(), tmp, sizeof(tmp), 0) < 0) {
			MessageBox::Show("Service: Failed to connect server", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
		}

		if(strcmp(tmp,"SUCC") == 0) {
			m_host = host;
			return true;
		}
		else if(strcmp(tmp,"FAIL") == 0){

			String^ tmp2 = gcnew String("");
			tmp2 = "Servicename " + m_name + " is already exist.";
			MessageBox::Show("Hello, World!", tmp2, MessageBoxButtons::OK,  MessageBoxIcon::Information);
			return false;
		}

		return true;
	}

	bool SIGService::disconnect()
	{
		if(!m_sgvsock->sendData("00004", 5)) return false;
		return true;
	}

	bool SIGService::sendMsgToCtr(String^ to, String^ msg)
	{
		cliext::map<String^, SgvSocket^>::iterator it;
		it = m_consocks.find(to);

		if(it != m_consocks.end()) {
			int msgSize = msg->Length;
			String^ ssize = msgSize.ToString();
			SgvSocket^ sock = (*it)->second;
			String^ sendMsg = m_name + "," + ssize + "," + msg;
			int tmpsize = sizeof(unsigned short) * 2;
			int sendSize = sendMsg->Length + tmpsize;

			char *sendBuff = new char[sendSize];
			char *p = sendBuff;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0002);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
			char *sendMsg_c = StringToChar(sendMsg);
			memcpy(p, sendMsg_c, sendMsg->Length);
			if(!sock->sendData(sendBuff, sendSize)) {
				delete [] sendBuff;			
				return false; 
			}

			delete [] sendBuff;	 
			return true;
		}
		else {
			return false;
		}
	}

	bool SIGService::sendMsg(String^ to, String^ msg)
	{
		char msize[6];
		int ms = (int)msg->Length;
		sprintf_s(msize, 6,"%.5d", ms);
		String^ msg_size = gcnew String(msize);
		//msg_size = std::string(msize);

		String^ tmp_data = msg_size + "," + msg + "," + "-1.0," + "1," + to + ",";
		char bsize[6];
		int nbyte = (int)tmp_data->Length + 5;
		sprintf_s(bsize, 6,"%.5d", nbyte);
		String^ tmp_size = gcnew String(bsize);
		tmp_data = tmp_size + tmp_data;
		char* tmp_data_c = StringToChar(tmp_data);
		if(!m_sgvsock->sendData(tmp_data_c, tmp_data->Length)) return false;

		return true;
	}

	bool SIGService::sendMsg(List<String^>^ to, String^ msg)
	{
		char msize[6];
		int ms = (int)msg->Length;
		sprintf_s(msize, 6,"%.5d", ms);
		String^ msg_size = gcnew String(msize);


		String^ tmp_data = msg_size + "," + msg + "," + "-1.0,";

		int size = to->Count;

		char tmp[6];
		sprintf_s(tmp, 6, "%d", size);
		String^ num = gcnew String(tmp);

		tmp_data += num + ","; // + to + "," + msg;
		for(int i = 0; i < size; i++) {
			tmp_data += to[i] + ",";
		}

		char bsize[6];
		int nbyte = (int)tmp_data->Length + 5;
		sprintf_s(bsize, 6, "%.5d", nbyte);
		String^ tmp_size = gcnew String(bsize);
		tmp_data = tmp_size + tmp_data;

		char* tmp_data_c = StringToChar(tmp_data);
		if(!m_sgvsock->sendData(tmp_data_c, tmp_data->Length)) return false;
		return true;
	}

	void SIGService::startLoop(double ltime)
	{
		m_start = true;
		static clock_t start;
		clock_t now;
		start = clock();

		m_onLoop = true;
		while(1){

			if(m_onLoop){
				if(ltime > 0){
					now = clock();
					double tmp =(double)(now - start)/CLOCKS_PER_SEC;
					if(tmp > ltime) {
						m_onLoop = false;
						break;
					}
				}
				if(!checkRecvData(100)) {
					m_onLoop = false;
					break;
				}
			}
			else break;
		}
		m_start = false;
	}

	bool SIGService::checkRecvData(int usec)
	{

		static bool init;
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = usec;

		SOCKET mainSock = m_sgvsock->getSocket();
		if(mainSock == NULL) return false;

		static double timewidth;
		static clock_t start;
		clock_t now;

		if(m_start && !init) {
			onInit();
			timewidth = onAction();
			start = clock();
			init = true;
		}
		else{
			now = clock();
			double tmp =(double)(now - start)/CLOCKS_PER_SEC;

			if(tmp > timewidth) {
				timewidth = onAction();
				start = clock();
			}
		}
		fd_set fds, readfds;

		int n;

		cliext::vector<sigverse::SgvSocket^> tmp_socks;
		tmp_socks.push_back(m_sgvsock);

		FD_ZERO(&readfds);

		FD_SET(mainSock,&readfds);

		if(m_connectedView){
			SOCKET viewSock = m_viewsock->getSocket();
			tmp_socks.push_back(m_viewsock);
			FD_SET(viewSock,&readfds);
		}

		cliext::map<String^, SgvSocket^>::iterator it;
		it = m_consocks.begin();

		while(it != m_consocks.end()) {
			SOCKET sock = (*it)->second->getSocket();
			FD_SET(sock,&readfds);
			tmp_socks.push_back((*it)->second);
			it++;
		}

		memcpy(&fds, &readfds, sizeof(fd_set));

		n = select(0, &fds, NULL, NULL, &tv);

		if (n == 0) {
			return true;
		}
		else if(n < 0) {
			return false;
		}

		int connectSize = tmp_socks.size();
		for(int i = 0; i < connectSize; i++) {
			SOCKET nowsock = tmp_socks[i]->getSocket();

			if (FD_ISSET(nowsock, &fds)) {

				char bsize[4];
				if(!tmp_socks[i]->recvData(bsize,4)) return true;

				char *p = bsize;
				unsigned short header = BINARY_GET_DATA_S_INCR(p, unsigned short);
				unsigned short size = BINARY_GET_DATA_S_INCR(p, unsigned short);

				size -= 4;

				char *recvBuff = new char[size];
				if(size > 0)
					if(!tmp_socks[i]->recvData(recvBuff, size)) return true;

				char *nexttoken;

				switch(header) {
					case 0x0001:
						{
							RecvMsgEvent^ msg = gcnew RecvMsgEvent();

							//msg.setData(recvBuff, size);
							//String^ tmp = recvBuff;
							String^ tmp = gcnew String(recvBuff);
							msg->setData(tmp, size);
 
							onRecvMsg(msg);
							break;
						}
					case 0x0002:
						{
							String^  name = gcnew String(recvBuff);
							//name[size] = '\0';
							m_elseServices.Add(name);

							break;
						}
					case 0x0003:
						{
							char *pp = recvBuff;
							unsigned short port = BINARY_GET_DATA_S_INCR(pp, unsigned short);


							std::string name = strtok_s(pp, ",", &nexttoken);
							String^ name_s = gcnew String(name.c_str());
							SgvSocket^ sock = gcnew SgvSocket();
							sock->initWinsock();

							char *host_p = StringToChar(m_host);

							if(sock->connectTo(host_p, port)) {
								m_consocks.insert(cliext::map<String^, SgvSocket^>::make_value(name_s, sock));
								m_entitiesName.Add(name_s);
							}

							char tmp[sizeof(unsigned short)*2];
							char *p = tmp;

							BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0001);
							BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

							sock->sendData(tmp, sizeof(unsigned short)*2);

							break;
						}
					case 0x0004:
						{
							char *pp = recvBuff;
							std::string ename_tmp = strtok_s(pp, ",", &nexttoken);

							String^ ename = gcnew String(ename_tmp.c_str()); 

							disconnectFromController(ename);

							cliext::map<String^, SgvSocket^>::iterator sockit;
							sockit = m_consocks.find(ename);
							if(sockit != m_consocks.end()) {
								m_consocks.erase(sockit);
							}
							return false;
						}
					case 0x0005:
						{
							delete m_viewsock;
							m_viewsock = nullptr;
							m_connectedView = false;

							if(m_autoExitProc){
								exit(0);
							}
							else if(m_autoExitLoop){
								m_onLoop = false;
							}

							break;
						}
				}
				delete [] recvBuff;
			} //if (FD_ISSET(mainSock, &fds)) {
		} // for(int i = 0; i < connectSize; i++){
		return true;
	}

	bool SIGService::connectToViewer()
	{
		m_viewsock = gcnew SgvSocket();
		//m_viewsock->initWinsock();

		if(!m_viewsock->connectTo("localhost", 11000)) {
			MessageBox::Show("Could not connect to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return false;
		}
		else {
			int nsize = m_name->Length;
			int sendSize = nsize + sizeof(unsigned short)*2;
			char *sendBuff = new char[sendSize];
			char *p = sendBuff;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0000);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);

			char* name= StringToChar(m_name);

			memcpy(p, name, m_name->Length);
			if(!m_viewsock->sendData(sendBuff, sendSize)) {
				delete [] sendBuff;
			}
			delete [] sendBuff;

			m_connectedView = true;
			return true;
		}
	}
	bool SIGService::disconnectFromController(String^ entityName)
	{
		cliext::map<String^, SgvSocket^>::iterator it;
		it = m_consocks.find(entityName);

		if(it != m_consocks.end()) {
			SgvSocket^ sock = (*it)->second;

			char tmp[sizeof(unsigned short)*2];
			char *p = tmp;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

			sock->sendData(tmp, sizeof(unsigned short)*2);

			m_consocks.erase(it);
			delete sock;

			return true;
		}
		else
			return false;
	}

	void SIGService::disconnectFromAllController()
	{
		cliext::map<String^, SgvSocket^>::iterator it;
		it = m_consocks.begin();

		while(it != m_consocks.end()) {
			SgvSocket^ sock = (*it)->second;

			char tmp[sizeof(unsigned short)*2];
			char *p = tmp;

			BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
			BINARY_SET_DATA_S_INCR(p, unsigned short, sizeof(unsigned short));

			sock->sendData(tmp, sizeof(unsigned short)*2);

			delete sock;
			it++;
		}
		m_consocks.clear();
	}

	void SIGService::disconnectFromViewer()
	{
		if(!m_connectedView || m_viewsock == nullptr) return;

		char buf[4];
		char *p = buf;
		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0003);
		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);

		m_viewsock->sendData(buf, 4);

		delete m_viewsock;
		m_viewsock = nullptr;
	}

	ViewImage^ SIGService::captureView(String^ entityName, int camID, ColorBitType ctype,ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox::Show("captureView: Service is not connected to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);

			return nullptr;
		}
		String^ cameraID = camID.ToString();
		int isize = size;
		String^ camSize = isize.ToString();
		String^ sendMsg = entityName + "," + cameraID + "," + camSize + ",";
		int tmpsize = sizeof(unsigned short) * 2;
		int sendSize = sendMsg->Length + tmpsize;

		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0001);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);

		char* sendMsg_c = StringToChar(sendMsg);
		memcpy(p, sendMsg_c, sendMsg->Length);

		if(!m_viewsock->sendData(sendBuff, sendSize)) {
			delete [] sendBuff;			
			return nullptr; 
		}

		ViewImageInfo^ info = gcnew ViewImageInfo(IMAGE_DATA_WINDOWS_BMP, COLORBIT_24, IMAGE_320X240);
		ViewImage^ view = gcnew ViewImage(info);

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		int imageSize = 320*240*3;

		char *headerBuff = new char[headerSize];

		if(!m_viewsock->recvData(headerBuff, headerSize)) {
			delete [] headerBuff;
			return nullptr; 
		}

		p = headerBuff;
		double fov;
		double ar;
		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);
		if(result == 2){
			String^ tmp = "captureView: cannot find entity " + entityName;
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] headerBuff;
			return nullptr;
		}
		else if (result == 3){
			String^ tmp = "captureView: " + entityName + " doesn't have camera";
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] headerBuff;
			return nullptr;
		}
		else{
			fov = BINARY_GET_DOUBLE_INCR(p);
			ar = BINARY_GET_DOUBLE_INCR(p);
			view->setFOVy(fov);
			view->setAspectRatio(ar);
		}

		delete [] headerBuff;

		char *imageBuff = new char[imageSize];
		if(!m_viewsock->recvData(imageBuff, imageSize)) {
			delete [] imageBuff;
			return nullptr; 
		}

		view->setBuffer(imageBuff);
		return view;
	}

	unsigned char SIGService::distanceSensor(String^ entityName, double offset, double range, int camID)
	{
		if(!m_connectedView){
			MessageBox::Show("distanceSensor: Service is not connected to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return 255;
		}

		String^ cameraID = camID.ToString();
		String^ sendMsg = entityName + "," + cameraID + ",";

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;
		int sendSize = sendMsg->Length + tmpsize;

		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0002);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);
		char* sendMsg_c = StringToChar(sendMsg);
		memcpy(p, sendMsg_c, sendMsg->Length);

		if(!m_viewsock->sendData(sendBuff, sendSize)) {
			delete [] sendBuff;			
			return 255; 
		}

		delete [] sendBuff;
		int recvSize = sizeof(unsigned short) + 1;

		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return 255; 
		}
		p = recvBuff;

		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);
		if(result == 2){
			String^ tmp = "distanceSensor: cannot find entity " + entityName;
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] recvBuff;
			return 255;
		}
		else if (result == 3){
			String^ tmp = "distanceSensor: " + entityName + "doesn't have camera id: " + camID.ToString();
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] recvBuff;
			return 255;
		}
		unsigned char distance = p[0];
		delete [] recvBuff;
		return distance;
	}

	ViewImage^ SIGService::distanceSensor1D(String^ entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox::Show("distanceSensor1D: Service is not connected to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return nullptr;
		}
		return getDistanceImage(entityName, offset, range, camID, 1, ctype, size);
	}

	ViewImage^ SIGService::distanceSensor2D(String^ entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox::Show("distanceSensor2D: Service is not connected to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return nullptr;
		}
		return getDistanceImage(entityName, offset, range, camID, 2, ctype, size);
	}

	ViewImage^ SIGService::getDistanceImage(String^ entityName, double offset, double range, int camID, int dimension, ColorBitType ctype, ImageDataSize size)
	{
		String^ dim = dimension.ToString();
		String^ cameraID = camID.ToString();
		int tmp_size = size;
		String^ camSize = tmp_size.ToString();
		String^ sendMsg = entityName + "," + dim + "," + cameraID + "," + camSize + ",";

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;

		int sendSize = sendMsg->Length + tmpsize;
		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0004);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);

		char* sendMsg_c = StringToChar(sendMsg);
		memcpy(p, sendMsg_c, sendMsg->Length);

		if(!m_viewsock->sendData(sendBuff, sendSize)){
			delete [] sendBuff;
			return nullptr;
		}

		delete [] sendBuff;

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		char *header = new char[headerSize];
		if(!m_viewsock->recvData(header, headerSize)){
			return nullptr;
		}
		p = header;
		unsigned short result = BINARY_GET_DATA_S_INCR(p, unsigned short);

		if(result == 2){
			String^ tmp;
			if(dimension == 1)
				tmp = "distanceSensor1D: cannot find entity " + entityName;
			else
				tmp = "distanceSensor2D: cannot find entity " + entityName;
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] header;
			return nullptr;
		}
		else if (result == 3){
			String^ tmp;
			if(dimension == 1)
				tmp = "distanceSensor1D: " + entityName + " doesn't have camera id: " + camID.ToString();
			else
				tmp = "distanceSensor2D: " + entityName + " doesn't have camera id: " + camID.ToString();
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] header;
			return nullptr;
		}

		double fov = BINARY_GET_DOUBLE_INCR(p);
		double ar = BINARY_GET_DOUBLE_INCR(p);

		delete [] header;

		int recvSize;
		if(dimension == 1){
			recvSize = 320;
		}
		else if(dimension == 2){
			recvSize = 320*240;
		}
		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return nullptr; 
		}

		ViewImage^ img;
		if(dimension == 1){
			ViewImageInfo^ info = gcnew ViewImageInfo(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X1);
			img = gcnew ViewImage(info);
		}

		else if(dimension == 2){
			ViewImageInfo^ info = gcnew ViewImageInfo(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X240);
			img = gcnew ViewImage(info);
		}
		img->setFOVy(fov);
		img->setAspectRatio(ar);
		img->setBuffer(recvBuff);
		//unsigned char *recvData = new unsigne
		//ViewImage *img;
		return img;
	}

	ViewImage^ SIGService::getDepthImage(String^ entityName, double offset, double range, int camID, ColorBitType ctype, ImageDataSize size)
	{
		if(!m_connectedView){
			MessageBox::Show("getDepthImage: Service is not connected to viewer", "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			return nullptr;
		}

		String^ cameraID = camID.ToString();
		int tmp_size = size;
		String^ camSize = tmp_size.ToString();
		String^ sendMsg = entityName + "," + cameraID + "," + camSize + ",";

		int tmpsize = sizeof(double) * 2 + sizeof(unsigned short)*2;

		int sendSize = sendMsg->Length + tmpsize;
		char *sendBuff = new char[sendSize];
		char *p = sendBuff;

		BINARY_SET_DATA_S_INCR(p, unsigned short, 0x0005);
		BINARY_SET_DATA_S_INCR(p, unsigned short, sendSize);
		BINARY_SET_DOUBLE_INCR(p, offset);
		BINARY_SET_DOUBLE_INCR(p, range);

		char* sendMsg_c = StringToChar(sendMsg);
		memcpy(p, sendMsg_c, sendMsg->Length);

		if(!m_viewsock->sendData(sendBuff, sendSize)){
			delete [] sendBuff;
			return nullptr;
		}

		delete [] sendBuff;

		int headerSize = sizeof(unsigned short) + sizeof(double)*2;
		char *header = new char[headerSize];
		if(!m_viewsock->recvData(header, headerSize)){
			return nullptr;
		}
		p = header;
		unsigned short result = BINARY_GET_DATA_S(p, unsigned short);

		if(result == 2){
			String^ tmp = "getDepthImage: cannot find entity " + entityName;
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] header;
			return nullptr;
		}
		else if (result == 3){

			String^ tmp = "getDepthImage: " + entityName + "doesn't have camera id: " + camID.ToString();
			MessageBox::Show(tmp, "Error", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			delete [] header;
			return nullptr;
		}

		double fov = BINARY_GET_DOUBLE_INCR(p);
		double ar = BINARY_GET_DOUBLE_INCR(p);

		delete [] header;

		int recvSize = 320*240;

		char *recvBuff = new char[recvSize];
		if(!m_viewsock->recvData(recvBuff, recvSize)) {
			delete [] recvBuff;
			return nullptr; 
		}

		ViewImage^ img;
		ViewImageInfo^ info = gcnew ViewImageInfo(IMAGE_DATA_WINDOWS_BMP, DEPTHBIT_8, IMAGE_320X240);
		img = gcnew ViewImage(info);

		img->setFOVy(fov);
		img->setAspectRatio(ar);
		img->setBuffer(recvBuff);
		//unsigned char *recvData = new unsigne
		//ViewImage *img;
		return img;
	}

};

