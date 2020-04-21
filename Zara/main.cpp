#include <iostream>
#include "Server/Server.hpp"
#include "Files/FileWorker.hpp"
#include "Crypto/EVP.hpp"


using namespace std;
using namespace Zara;


int main(int argc, char* argv[])
{
	IFileWorker* files = new FileWorker();
	IEncoder* coder = new EVP();

	files->CreateF("C:/test.txt", coder->Encode("Hello, world!", "it's_a_me_mario"));

	cout << coder->Decode(files->ReadAllFile("C:/test.txt"), "it's_a_me_mario") << endl;

	return 0;
}
