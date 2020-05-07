#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <vector>


namespace Zara
{
	enum class EngineResult
	{
		Error = 0,
		OK,
		AlreadyExists,
		ErrorOnCreating,
		ErrorInEngine,
		NotFound,
	};


	class IDbEngine
	{
	protected:
		nlohmann::json manifest;
	public:
		virtual bool DbExists(std::string dbName) = 0;
		virtual bool CollectionExists(std::string dbName, std::string name) = 0;
		virtual bool DocumentExists(std::string dbName, std::string collectionName, std::vector<std::string> query) = 0;

		virtual EngineResult CreateDb(std::string name) = 0;
		virtual EngineResult CreateCollection(std::string dbName, std::string name) = 0;
		virtual EngineResult CreateDocument(std::string dbName, std::string collectionName, std::string document) = 0;

		virtual EngineResult DeleteCollection(std::string dbName, std::string name) = 0;
		virtual EngineResult DeleteDocument(std::string dbName, std::string collectionName, std::vector<std::string> query) = 0;

		virtual std::string FindAllCollections(std::string dbName) = 0;
		virtual std::string FindDocument(std::string dbName, std::string collectionName, std::vector<std::string> query) = 0;
		virtual std::string FindAllDocuments(std::string dbName, std::string collectionName, std::vector<std::string> query) = 0;
		virtual std::string FindAllDocuments(std::string dbName, std::string collectionName, nlohmann::json query) = 0;
		virtual std::string AllDocuments(std::string dbName, std::string collectionName) = 0;
	};
}
