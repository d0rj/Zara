#include "Server.hpp"
#include <functional>
#include <thread>


void Zara::Server::handleNewConnection(SOCKET sock)
{
	SOCKET client = accept(sock, nullptr, nullptr);
	FD_SET(client, &master);

	Send(client, "Connected to ZaraDB.");
}


void Zara::Server::handleDisconnect(SOCKET sock)
{
	closesocket(sock);
	FD_CLR(sock, &master);
}


void Zara::Server::handleNewMessage(SOCKET sock, std::string message)
{
}


Zara::Server::Server(int port) : Zara::IServer(port), port(port)
{
	ZeroMemory(buffer, Zara::MAX_BUFF_SIZE);

	WSADATA wsaData;
	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsOk)
	{
		return;
	}

	listeningSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSock == INVALID_SOCKET)
	{
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listeningSock, (sockaddr*)&hint, sizeof(hint));
}


void Zara::Server::Listen(
	std::function<void(SOCKET)> onConnect,
	std::function<void(SOCKET)> onDisconnect,
	std::function<void(SOCKET, std::string)> onMessage
)
{
	listen(listeningSock, SOMAXCONN);

	FD_ZERO(&master);
	FD_SET(listeningSock, &master);

	running = true;
	while (running)
	{
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];

			if (sock == listeningSock)
			{
				handleNewConnection(sock);

				std::thread clientThread(onConnect, sock);
				clientThread.join();
			}
			else
			{
				ZeroMemory(buffer, MAX_BUFF_SIZE);

				int bytesIn = recv(sock, buffer, MAX_BUFF_SIZE, 0);
				if (bytesIn <= 0)
				{
					handleDisconnect(sock);
					
					std::thread clientThread(onDisconnect, sock);
					clientThread.join();
				}
				else
				{
					auto message = std::string(buffer, bytesIn);

					handleNewMessage(sock, message);
					
					std::thread clientThread(onMessage, sock, message);
					clientThread.join();
				}
			}
		}
	}

	FD_CLR(listeningSock, &master);

	while (master.fd_count > 0)
	{
		SOCKET sock = master.fd_array[0];

		Send(sock, "Server is shutting down.");

		FD_CLR(sock, &master);
		closesocket(sock);
	}
}


void Zara::Server::Send(SOCKET to, std::string message, int flags) const noexcept
{
	send(to, (message + "\r\n").c_str(), message.length() + 3, flags);
}


void Zara::Server::Close() noexcept
{
	closesocket(listeningSock);
	WSACleanup();
}
