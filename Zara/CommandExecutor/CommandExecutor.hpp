#pragma once

#include "../Engine/IDbEngine.hpp"
#include "../Parser/IParser.hpp"
#include "../Server/IServer.hpp"
#include <string>
#include <mutex>


namespace Zara
{
	class CommandExecutor
	{
	private:
		IDbEngine* dbEngine;
		IParser* parser;
		IServer* server;
		std::string usedDb = "default";
		std::string usedCollection = "default";
		std::vector<std::string> commands = {"use", "db", "coll", "insert", "find"};
		std::mutex engineMutex;
		std::mutex serverMutex;

		void executeCommand(std::string cmd, std::string arg, SOCKET sock);
		void useCommand(std::string arg, SOCKET sock);
		void dbCommand(std::string arg, SOCKET sock);
		void collCommand(std::string arg, SOCKET sock);
		void insertCommand(std::string arg, SOCKET sock);
		void findCommand(std::string arg, SOCKET sock);
	public:
		CommandExecutor(IDbEngine* dbEngine, IServer* server);
		void OnConnect(SOCKET sock);
		void OnDisconnect(SOCKET sock);
		void OnMessage(SOCKET sock, std::string message);
	};
}
