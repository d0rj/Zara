#pragma once

#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")


namespace Zara
{
	const int MAX_BUFF_SIZE = 4096;

	class IServer
	{
	public:
		IServer(int port) {};
		virtual void Listen(
			void (*onConnect)(SOCKET sock), 
			void (*onDisconnect)(SOCKET sock), 
			void (*onMessage)(SOCKET sock, std::string message)) = 0;
		virtual void Send(SOCKET to, std::string message, int flags = 0) const noexcept = 0;
		virtual void Close() noexcept = 0;
	};
}
