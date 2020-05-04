#include "CommandExecutor.hpp"
#include <algorithm>
#include <iostream>


void Zara::CommandExecutor::executeCommand(std::string cmd, std::string arg, SOCKET sock)
{
	if (cmd == "use")
		useCommand(arg, sock);
	else if (cmd == "db")
		dbCommand(arg, sock);
	else if (cmd == "coll")
		collCommand(arg, sock);
	else if (cmd == "insert")
		insertCommand(arg, sock);
}


void Zara::CommandExecutor::useCommand(std::string arg, SOCKET sock)
{
	if (!arg.empty())
	{
		int dotPos = arg.find('.');

		if (dotPos == std::string::npos)
		{
			usedDb = arg;
			EngineResult result = dbEngine->CreateDb(arg);

			if (result == EngineResult::AlreadyExists)
				server->Send(sock, "Switched to db \'" + usedDb + "\'.");
			else
				server->Send(sock, "Created db: \'" + usedDb + "\'.");
		}
		else
		{
			std::string first;
			first = arg.substr(0, dotPos);
			if (first != "db")
			{
				server->Send(sock, "Invalid argument: \'" + arg + "\'. ");
				return;
			}

			usedCollection = arg.substr(dotPos + 1, arg.size() - dotPos - 1);
			EngineResult result = dbEngine->CreateCollection(usedDb, usedCollection);

			if (result == EngineResult::AlreadyExists)
				server->Send(sock, "Switched to collection \'" + usedCollection + "\' in \'" + usedDb + "\'.");
			else
				server->Send(sock, "Created collection: \'" + usedDb + "." + usedCollection + "\'.");
		}
	}
	else
	{
		server->Send(sock, "Invalid command argument: " + arg + ".");
	}
}


void Zara::CommandExecutor::dbCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		server->Send(sock, "Used db: \'" + usedDb + "\'.");
	}
	else if (arg == "*")
	{
		server->Send(sock, dbEngine->FindAllCollections(usedDb));
	}
	else
	{
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
	}
}


void Zara::CommandExecutor::collCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		server->Send(sock, "Used collection: \'" + usedDb + "." + usedCollection + "\'.");
	}
	else if (arg == "*")
	{
		server->Send(sock, dbEngine->AllDocuments(usedDb, usedCollection));
	}
	else
	{
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
	}
}


void Zara::CommandExecutor::insertCommand(std::string arg, SOCKET sock)
{
	if (arg.empty())
	{
		server->Send(sock, "Invalid argument: \'" + arg + "\'.");
		return;
	}
	
	switch (dbEngine->CreateDocument(usedDb, usedCollection, arg))
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
}


Zara::CommandExecutor::CommandExecutor(IDbEngine* dbEngine, IParser* parser, IServer* server)
	: dbEngine(dbEngine), parser(parser), server(server)
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
		server->Send(sock, "Error: no commands.");
		return;
	}

	bool hasFinded = false;
	for (auto parsedCommand = parsedCommands.cbegin(); parsedCommand != parsedCommands.cend(); ++parsedCommand)
	{
		if (auto finded = std::find(commands.begin(), commands.end(), parsedCommand->first); finded != commands.end())
		{
			hasFinded = true;
			executeCommand(parsedCommand->first, parsedCommand->second, sock);
		}
		else
		{
			std::string result = "Unknown command \"" + parsedCommand->first + "\".";
			server->Send(sock, result);
			// TODO: handle errors
			break;
		}
	}

	if (!hasFinded)
	{
		server->Send(sock, "No supported commands finded.");
	}
}
