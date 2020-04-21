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
			void (*onConnect)(SOCKET sock),
			void (*onDisconnect)(SOCKET sock),
			void (*onMessage)(SOCKET sock, std::string message)) override;
		virtual void Send(SOCKET to, std::string message, int flags = 0) const noexcept override;
		virtual void Close() noexcept override;
	};
}
