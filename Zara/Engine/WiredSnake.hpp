#pragma once 

#include "IDbEngine.hpp"
#include <filesystem>
#include "../Files/IFileWorker.hpp"


namespace fs = std::filesystem;


namespace Zara
{
	class WiredSnake : virtual public IDbEngine
	{
		const static std::string EngineName;
		const static std::string FilesName;
	private:
		fs::path dbPath;
		bool directoryPrerDb = false;
		bool crypto = false;
		std::hash<std::string> hash;
		IFileWorker* files;


		fs::path configFilePath() const noexcept;
		fs::path lockFilePath() const noexcept;
		fs::path manifestFilePath() const noexcept;
		fs::path collectionFilePath(std::string dbName, std::string name) const noexcept;

		void createConfigFile() const noexcept;
		void createManifestFile() const noexcept;
		void handleConfigFile();
		void configFromJson(nlohmann::json* config) noexcept;
		bool dbExist(std::string name) const noexcept;
		void createEmptyDb(std::string name) noexcept;
		nlohmann::json createEmptyCollection(std::string name) noexcept;
		int getDbIndex(std::string name);
		int getCollectionIndex(std::string dbName, std::string name);
		int getCollectionIndex(int dbIndex, std::string name);
		int getDocumentIndex(std::string dbName, std::string collName, std::vector<std::string> query);
	public:
		WiredSnake(IFileWorker* fileWorker, fs::path path);

		bool DbExists(std::string dbName);
		bool CollectionExists(std::string dbName, std::string name);
		bool DocumentExists(std::string dbName, std::string collectionName, std::vector<std::string> query);

		EngineResult CreateDb(std::string name);
		EngineResult CreateCollection(std::string dbName, std::string name);
		EngineResult CreateDocument(std::string dbName, std::string collectionName, std::string document);

		EngineResult DeleteCollection(std::string dbName, std::string name);
		EngineResult DeleteDocument(std::string dbName, std::string collectionName, std::vector<std::string> query);
		EngineResult DeleteDocuments(std::string dbName, std::string collectionName, nlohmann::json query);

		std::string FindAllCollections(std::string dbName);
		std::string FindDocument(std::string dbName, std::string collectionName, std::vector<std::string> query);
		std::string FindAllDocuments(std::string dbName, std::string collectionName, std::vector<std::string> query);
		std::string FindAllDocuments(std::string dbName, std::string collectionName, nlohmann::json query);
		std::string AllDocuments(std::string dbName, std::string collectionName);
	};
}
