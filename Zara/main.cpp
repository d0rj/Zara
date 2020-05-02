#include <iostream>
#include "Server/Server.hpp"
#include "Files/FileWorker.hpp"
#include "Crypto/EVP.hpp"
#include "Engine/WiredSnake.hpp"
#include "Parser/EagleParser.hpp"


using namespace std;
using namespace Zara;


int main(int argc, char* argv[])
{
	EagleParser parser;

	auto m = parser.Parse("insert {{\"hello\": \"world\"}} play {music} dance {}");

	for (auto it = m.cbegin(); it != m.cend(); ++it)
		std::cout << it->first << " - " << it->second << "\n";

	return 0;
}
