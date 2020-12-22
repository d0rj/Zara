#include "CommandExecutor.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include "../Parser/EagleParser.hpp"


void Zara::CommandExecutor::executeCommand(std::string cmd, std::string arg, SOCKET sock)
{
	handlers[cmd](arg, sock);
}


void Zara::CommandExecutor::useCommand(std::string arg, SOCKET sock)
{
	if (!arg.empty())
	{
		int dotPos = arg.find('.');

		if (dotPos == std::string::npos)
		{
			usedDb = arg;
			engineMutex.lock();
			EngineResult result = dbEngine->CreateDb(arg);
			engineMutex.unlock();

			serverMutex.lock();
			if (result == EngineResult::AlreadyExists)
				server->Send(sock, "Switched to db \'" + usedDb + "\'.");
			else
				server->Send(sock, "Created db: \'" + usedDb + "\'.");
			serverMutex.unlock();
		}
		else
		{
			std::string first;
			first = arg.substr(0, dotPos);
			if (first != "db")
			{
				serverMutex.lock();
				server->Send(sock, "Invalid argument: \'" + arg + "\'. ");
				serverMutex.unlock();

				return;
			}

			usedCollection = arg.substr(dotPos + 1, arg.size() - dotPos - 1);

			engineMutex.lock();
			EngineResult result = dbEngine->CreateCollection(usedDb, usedCollection);
			engineMutex.unlock();

			serverMutex.lock();
			if (result == EngineResult::AlreadyExists)
				server->Send(sock, "Switched to collection \'" + usedCollection + "\' in \'" + usedDb + "\'.");
			else
				server->Send(sock, "Created collection: \'" + usedDb + "." + usedCollection + "\'.");
			serverMutex.unlock();
		}
	}
	else
	{
		serverMutex.lock();
		server->Send(sock, "Invalid command argument: " + arg + ".");
		serverMutex.unlock();
	}
}


void Zara::CommandExecutor::dbCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Used db: \'" + usedDb + "\'.");
		serverMutex.unlock();
	}
	else if (arg == "*")
	{
		engineMutex.lock();
		std::string collections = dbEngine->FindAllCollections(usedDb);
		engineMutex.unlock();

		serverMutex.lock();
		server->Send(sock, collections);
		serverMutex.unlock();
	}
	else
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();
	}
}


void Zara::CommandExecutor::collCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Used collection: \'" + usedDb + "." + usedCollection + "\'.");
		serverMutex.unlock();
	}
	else if (arg == "*")
	{
		engineMutex.lock();
		std::string documents = dbEngine->AllDocuments(usedDb, usedCollection);
		engineMutex.unlock();

		serverMutex.lock();
		server->Send(sock, documents);
		serverMutex.unlock();
	}
	else
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();
	}
}


void Zara::CommandExecutor::insertCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();

		return;
	}
	
	engineMutex.lock();
	EngineResult result = dbEngine->CreateDocument(usedDb, usedCollection, arg);
	engineMutex.unlock();

	serverMutex.lock();
	switch (result)
	{
	case EngineResult::Error:
	case EngineResult::ErrorOnCreating:
	case EngineResult::ErrorInEngine:
		server->Send(sock, "Error in inserting document: \'" + arg + "\'.");
		break;
	case EngineResult::OK:
		server->Send(sock, "Document created.");
		break;
	case EngineResult::AlreadyExists:
		server->Send(sock, "Document already exists.");
		break;
	default:
		break;
	}
	serverMutex.unlock();
}


void Zara::CommandExecutor::findCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();

		return;
	}
	
	nlohmann::json query;
	try
	{
		query = parser->ParseJson(arg);
	}
	catch (nlohmann::json::parse_error& _)
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();

		return;
	}

	engineMutex.lock();
	std::string documents = dbEngine->FindAllDocuments(usedDb, usedCollection, query);
	engineMutex.unlock();

	serverMutex.lock();
	server->Send(sock, documents);
	serverMutex.unlock();
}


void Zara::CommandExecutor::removeCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();

		return;
	}

	nlohmann::json query;
	try
	{
		query = nlohmann::json::parse(arg);
	}
	catch (nlohmann::json::parse_error& _)
	{
		serverMutex.lock();
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		serverMutex.unlock();

		return;
	}

	engineMutex.lock();
	auto result = dbEngine->DeleteDocuments(usedDb, usedCollection, query);
	engineMutex.unlock();

	serverMutex.lock();
	switch (result)
	{
	case Zara::EngineResult::Error:
	case Zara::EngineResult::NotFound:
		server->Send(sock, "No found documents to remove.");
		break;
	case Zara::EngineResult::OK:
		server->Send(sock, "Removed.");
		break;
	default:
		break;
	}
	serverMutex.unlock();
}


Zara::CommandExecutor::CommandExecutor(IDbEngine* dbEngine, IServer* server)
	: dbEngine(dbEngine), parser(new EagleParser()), server(server)
{
}


void Zara::CommandExecutor::OnConnect(SOCKET sock)
{
}


void Zara::CommandExecutor::OnDisconnect(SOCKET sock)
{
}


void Zara::CommandExecutor::OnMessage(SOCKET sock, std::string message)
{
	std::unordered_map<std::string, std::string> parsedCommands = parser->Parse(message);
	if (parsedCommands.empty())
	{
		serverMutex.lock();
		server->Send(sock, "Error: no commands.");
		serverMutex.unlock();

		return;
	}

	bool hasFinded = false;
	for (auto parsedCommand = parsedCommands.cbegin(); parsedCommand != parsedCommands.cend(); ++parsedCommand)
	{
		if (handlers.contains(parsedCommand->first))
		{
			hasFinded = true;
			executeCommand(parsedCommand->first, parsedCommand->second, sock);
		}
		else
		{
			std::string result = "Unknown command \"" + parsedCommand->first + "\".";

			serverMutex.lock();
			server->Send(sock, result);
			serverMutex.unlock();
			// TODO: handle errors
			break;
		}
	}

	if (!hasFinded)
	{
		serverMutex.lock();
		server->Send(sock, "No supported commands finded.");
		serverMutex.unlock();
	}
}
