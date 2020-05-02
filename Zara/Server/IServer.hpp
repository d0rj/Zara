#pragma once

#include <string>
#include <WS2tcpip.h>
#include <functional>
#pragma comment (lib, "ws2_32.lib")


namespace Zara
{
	const int MAX_BUFF_SIZE = 4096;

	class IServer
	{
	public:
		IServer(int port) {};
		virtual void Listen(
			std::function<void(SOCKET)> onConnect,
			std::function<void(SOCKET)> onDisconnect,
			std::function<void(SOCKET, std::string)> onMessage
			) = 0;
		virtual void Send(SOCKET to, std::string message, int flags = 0) const noexcept = 0;
		virtual void Close() noexcept = 0;
	};
}
