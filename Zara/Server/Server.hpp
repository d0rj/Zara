#pragma once

#include "IServer.hpp"


namespace Zara
{
	class Server : virtual public IServer
	{
	private:
		int port;
		bool running;
		char buffer[Zara::MAX_BUFF_SIZE];
		SOCKET listeningSock;
		fd_set master;

		void handleNewConnection(SOCKET sock);
		void handleDisconnect(SOCKET sock);
		void handleNewMessage(SOCKET sock, std::string message);
	public:
		Server(int port);
		virtual void Listen(
			std::function<void(SOCKET)> onConnect,
			std::function<void(SOCKET)> onDisconnect,
			std::function<void(SOCKET, std::string)> onMessage
		) override;
		virtual void Send(SOCKET to, std::string message, int flags = 0) const noexcept override;
		virtual void Close() noexcept override;
	};
}
