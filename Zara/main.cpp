#include <iostream>
#include "Server/Server.hpp"


using namespace std;
using namespace Zara;


int main(int argc, char* argv[])
{
	IServer* server = new Server(8888);
	server->Listen(
		[](SOCKET sock) 
		{
			cout << "Connected" << endl;
		}, 
		[](SOCKET sock) 
		{
			cout << "Disconnected" << endl;
		}, 
		[](SOCKET sock, string message) 
		{
			cout << "New message: " << message << endl;
		}
	);

	return 0;
}