#ifndef __SGVSOCKET__
#define __SGVSOCKET__

#include <winsock2.h>
//#include "SgvLog.h"

namespace sigverse
{
	class SgvSocket
	{
	public:

		SgvSocket();
		SgvSocket(SOCKET sock);

		~SgvSocket();

		bool initWinsock();

		void finalWinsock(){WSACleanup();}

		bool connectTo(const char *hostname, int port);

		bool sendData( const char *msg, int size);

		bool recvData( char *msg, int size);

		SOCKET getSocket() { return m_sock; }


	private:
		SOCKET m_sock;

		//Sgv::LogPtr mLog;

	};
}

#endif

