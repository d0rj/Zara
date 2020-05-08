#include <iostream>
#include "Server/Server.hpp"
#include "Files/CryptoFileWorker.hpp"
#include "Crypto/EVP.hpp"
#include "Engine/WiredSnake.hpp"
#include "CommandExecutor/CommandExecutor.hpp"


using namespace std;
using namespace Zara;


int main(int argc, char* argv[])
{
	string key = "0123456789012345";
	IEncoder* encoder = new EVP((unsigned char*)key.c_str());
	IFileWorker* files = new CryptoFileWorker(encoder, key);
	IDbEngine* engine = new WiredSnake(files, "C:/zara");
	IServer* server = new Server(3228);
	CommandExecutor executor(engine, server);
	
	server->Listen(
		[&executor](SOCKET sock) mutable {
			std::cout << "Connected." << std::endl;
			executor.OnConnect(sock);
		},
		[&executor](SOCKET sock) mutable {
			std::cout << "Disconnected." << std::endl;
			executor.OnDisconnect(sock);
		},
		[&executor](SOCKET sock, std::string message) mutable {
			std::cout << "Message: " << message << "."<< std::endl;
			executor.OnMessage(sock, message);
		}
	);

	return 0;
}
