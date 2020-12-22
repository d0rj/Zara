#pragma once

#include "../Engine/IDbEngine.hpp"
#include "../Parser/IParser.hpp"
#include "../Server/IServer.hpp"
#include <string>
#include <mutex>
#include <functional>


using namespace std::placeholders;


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
		std::map<std::string, std::function<void(std::string, SOCKET)>> handlers = {
			{"use", std::bind(&CommandExecutor::useCommand, this, std::placeholders::_1, std::placeholders::_2)},
			{"db", std::bind(&CommandExecutor::dbCommand, this, std::placeholders::_1, std::placeholders::_2)},
			{"coll", std::bind(&CommandExecutor::collCommand, this, std::placeholders::_1, std::placeholders::_2)},
			{"insert", std::bind(&CommandExecutor::insertCommand, this, std::placeholders::_1, std::placeholders::_2)},
			{"find", std::bind(&CommandExecutor::findCommand, this, std::placeholders::_1, std::placeholders::_2)},
			{"remove", std::bind(&CommandExecutor::removeCommand, this, std::placeholders::_1, std::placeholders::_2)}
		};
		std::mutex engineMutex;
		std::mutex serverMutex;

		void executeCommand(std::string cmd, std::string arg, SOCKET sock);
		void useCommand(std::string arg, SOCKET sock);
		void dbCommand(std::string arg, SOCKET sock);
		void collCommand(std::string arg, SOCKET sock);
		void insertCommand(std::string arg, SOCKET sock);
		void findCommand(std::string arg, SOCKET sock);
		void removeCommand(std::string arg, SOCKET sock);
	public:
		CommandExecutor(IDbEngine* dbEngine, IServer* server);
		void OnConnect(SOCKET sock);
		void OnDisconnect(SOCKET sock);
		void OnMessage(SOCKET sock, std::string message);
	};
}
