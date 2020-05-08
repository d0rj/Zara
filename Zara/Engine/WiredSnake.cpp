#include "WiredSnake.hpp"
#include <sstream>
#include <iostream>


const std::string Zara::WiredSnake::EngineName = "WiredSnake";
const std::string Zara::WiredSnake::FilesName = ".ws";


inline fs::path Zara::WiredSnake::configFilePath() const noexcept
{
	return (dbPath / (EngineName + ".turtle"));
}


inline fs::path Zara::WiredSnake::lockFilePath() const noexcept
{
	return (dbPath / (EngineName + ".lock"));
}


inline fs::path Zara::WiredSnake::manifestFilePath() const noexcept
{
	return (dbPath / (EngineName + FilesName));
}


fs::path Zara::WiredSnake::collectionFilePath(std::string dbName, std::string name) const noexcept
{
	fs::path to_db = directoryPrerDb ? (dbPath / dbName) : dbPath;

	return (to_db / ("collection-" + std::to_string(hash(dbName + name)) + FilesName));
}


void Zara::WiredSnake::createConfigFile() const noexcept
{
	nlohmann::json config;
	config["directoryPrerDb"] = directoryPrerDb;
	config["crypto"] = crypto;

	files->CreateF(configFilePath(), config);
}


void Zara::WiredSnake::createManifestFile() const noexcept
{
	nlohmann::json manifest;
	manifest["dbs"] = nlohmann::json::array();
	files->CreateF(manifestFilePath(), manifest);
}


void Zara::WiredSnake::handleConfigFile()
{
	nlohmann::json config = files->ReadJson(configFilePath());

	configFromJson(&config);
}


// TODO: async 
bool Zara::WiredSnake::locked() const noexcept
{
	return false;
}


void Zara::WiredSnake::configFromJson(nlohmann::json* config) noexcept
{
	directoryPrerDb = (*config)["directoryPrerDb"].get<bool>();
	crypto = (*config)["crypto"].get<bool>();
}


bool Zara::WiredSnake::dbExist(std::string name) const noexcept
{
	for (auto& db : manifest["dbs"])
		if (db["name"].get<std::string>() == name)
		{
			return true;
		}

	return false;
}


void Zara::WiredSnake::createEmptyDb(std::string name) noexcept
{
	manifest["dbs"].push_back(
		{ 
			{"_id", hash(name)}, 
			{"name", name}, 
			{"collections", nlohmann::json::array()} 
		});
}


nlohmann::json Zara::WiredSnake::createEmptyCollection(std::string name) noexcept
{
	nlohmann::json collection = 
	{
		{"_id", hash(name)},
		{"name", name},
		{"documents", nlohmann::json::array()}
	};

	return collection;
}


int Zara::WiredSnake::getDbIndex(std::string name)
{
	auto dbs = manifest["dbs"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < dbs.size(); ++i)
		if (dbs[i]["name"].get<std::string>() == name)
		{
			return i;
		}

	return -1;
}


int Zara::WiredSnake::getCollectionIndex(std::string dbName, std::string name)
{
	return getCollectionIndex(getDbIndex(dbName), name);
}


int Zara::WiredSnake::getCollectionIndex(int dbIndex, std::string name)
{
	auto collections = manifest["dbs"][dbIndex]["collections"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < collections.size(); ++i)
		if (collections[i] == name)
		{
			return i;
		}

	return -1;
}


int Zara::WiredSnake::getDocumentIndex(std::string dbName, std::string collName, std::vector<std::string> query)
{
	int dbIndex = getDbIndex(dbName);
	int collIndex = getCollectionIndex(dbIndex, collName);

	fs::path collectionPath = collectionFilePath(dbName, collName);
	nlohmann::json collection = files->ReadJson(collectionPath);

	auto documents = collection["documents"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < documents.size(); ++i)
	{
		auto temp = documents[i].find(query[0]);
		if (temp != documents[i].end())
			if ((*temp) == query[1])
			{
				return i;
			}
	}

	return -1;
}


Zara::WiredSnake::WiredSnake(IFileWorker* fileWorker, fs::path path) 
	: files(fileWorker)
{
	files->CheckOrCreateDir(path);
	path /= "db";
	files->CheckOrCreateDir(path);

	dbPath = path;

	if (!files->Exists(configFilePath()))
	{
		createConfigFile();
	}
	handleConfigFile();

	if (!files->Exists(manifestFilePath()))
	{
		createManifestFile();
	}
	manifest = files->ReadJson(manifestFilePath());
}


bool Zara::WiredSnake::DbExists(std::string dbName)
{
	return dbExist(dbName);
}


bool Zara::WiredSnake::CollectionExists(std::string dbName, std::string name)
{
	return files->Exists(collectionFilePath(dbName, name));
}


bool Zara::WiredSnake::DocumentExists(std::string dbName, std::string collectionName, std::vector<std::string> query)
{
	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return false;

	int collIndex = getCollectionIndex(dbIndex, collectionName);
	if (collIndex < 0)
		return false;

	int docIndex = getDocumentIndex(dbName, collectionName, query);
	if (docIndex < 0)
		return false;

	return true;
}


Zara::EngineResult Zara::WiredSnake::CreateDb(std::string name)
{
	if (directoryPrerDb ? files->Exists(dbPath / name) : dbExist(name))
	{
		return EngineResult::AlreadyExists;
	}
	else
	{
		if (directoryPrerDb)
			files->CreateDir(dbPath / name);

		createEmptyDb(name);
		files->CreateF(manifestFilePath(), manifest);

		return EngineResult::OK;
	}
}


Zara::EngineResult Zara::WiredSnake::CreateCollection(std::string dbName, std::string name)
{
	if (!dbExist(dbName))
		CreateDb(dbName);

	fs::path filePath = collectionFilePath(dbName, name);

	if (!files->Exists(filePath))
	{
		manifest["dbs"][getDbIndex(dbName)]["collections"].push_back(name);
		files->CreateF(manifestFilePath(), manifest);

		nlohmann::json collection = createEmptyCollection(name);

		return files->CreateF(filePath, collection) ? EngineResult::OK : EngineResult::Error;
	}
	else
	{
		return EngineResult::AlreadyExists;
	}
}


Zara::EngineResult Zara::WiredSnake::CreateDocument(std::string dbName, std::string collectionName, std::string document)
{
	if (!CollectionExists(dbName, collectionName))
		CreateCollection(dbName, collectionName);

	fs::path filePath = collectionFilePath(dbName, collectionName);
	nlohmann::json collection = files->ReadJson(filePath);

	nlohmann::json docJson;
	try
	{
		docJson = nlohmann::json::parse(document);
	}
	catch (nlohmann::json::parse_error& e)
	{
		return EngineResult::Error;
	}

	collection["documents"].push_back(docJson);
	files->CreateF(filePath, collection);

	return EngineResult::OK;
}


Zara::EngineResult Zara::WiredSnake::DeleteCollection(std::string dbName, std::string name)
{
	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return EngineResult::NotFound;

	int collIndex = getCollectionIndex(dbIndex, name);
	if (collIndex < 0)
		return EngineResult::NotFound;

	files->DeleteF(collectionFilePath(dbName, name));

	manifest["dbs"][dbIndex]["collections"].erase(collIndex);

	return EngineResult::OK;
}


Zara::EngineResult Zara::WiredSnake::DeleteDocument(std::string dbName, std::string collectionName, std::vector<std::string> query)
{
	if (!DocumentExists(dbName, collectionName, query))
		return EngineResult::NotFound;
	else
	{
		fs::path filePath = collectionFilePath(dbName, collectionName);

		auto dbIndex = getDbIndex(dbName);
		auto collIndex = getCollectionIndex(dbIndex, collectionName);
		auto docIndex = getDocumentIndex(dbName, collectionName, query);

		nlohmann::json collection = files->ReadJson(filePath);
		collection["documents"].erase(docIndex);
		files->CreateF(filePath, collection);

		manifest["dbs"][dbIndex]["collections"][collIndex]["documents"].erase(docIndex);
		files->CreateF(manifestFilePath(), manifest);

		return EngineResult::OK;
	}
}


std::string Zara::WiredSnake::FindAllCollections(std::string dbName)
{
	std::ostringstream ss;
	ss << std::string();

	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return ss.str();

	auto collections = manifest["dbs"][dbIndex]["collections"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < collections.size(); ++i)
	{
		ss << collections[i]["name"].get<std::string>() << "\n";
	}

	return ss.str();
}


std::string Zara::WiredSnake::FindDocument(std::string dbName, std::string collectionName, std::vector<std::string> query)
{
	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return std::string();
	int collIndex = getCollectionIndex(dbIndex, collectionName);
	if (collIndex < 0)
		return std::string();

	fs::path path = collectionFilePath(dbName, collectionName);
	nlohmann::json collection = files->ReadJson(path);

	auto documents = collection["documents"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < documents.size(); ++i)
	{
		auto temp = documents[i].find(query[0]);

		if (temp != documents[i].end())
			if ((*temp) == query[1])
			{
				return documents[i];
			}
	}

	return std::string();
}


std::string Zara::WiredSnake::FindAllDocuments(std::string dbName, std::string collectionName, std::vector<std::string> query)
{
	std::ostringstream ss;
	ss << std::string();

	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return ss.str();
	int collIndex = getCollectionIndex(dbIndex, collectionName);
	if (collIndex < 0)
		return ss.str();

	fs::path path = collectionFilePath(dbName, collectionName);
	nlohmann::json collection = files->ReadJson(path);

	auto documents = collection["documents"].get<std::vector<nlohmann::json>>();
	for (size_t i = 0; i < documents.size(); ++i)
	{
		auto temp = documents[i].find(query[0]);

		if (temp != documents[i].end())
			if ((*temp) == query[1])
			{
				ss << documents[i] << "\n";
			}
	}

	return ss.str();
}


std::string Zara::WiredSnake::FindAllDocuments(std::string dbName, std::string collectionName, nlohmann::json query)
{
	std::ostringstream ss;
	ss << std::string();

	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return ss.str();
	int collIndex = getCollectionIndex(dbIndex, collectionName);
	if (collIndex < 0)
		return ss.str();

	fs::path path = collectionFilePath(dbName, collectionName);
	nlohmann::json collection = files->ReadJson(path);

	auto documents = collection["documents"].get<std::vector<nlohmann::json>>();
	bool thisChecked;
	for (auto document : documents)
	{
		thisChecked = true;
		for (auto& subQuery : query.items())
		{
			auto temp = document.find(subQuery.key());

			if (temp != document.end())
			{
				if ((*temp) != subQuery.value())
				{
					thisChecked = false;
					break;
				}
			}
			else
			{
				thisChecked = false;
				break;
			}
		}

		if (thisChecked)
			ss << document << "\n";
	}

	return ss.str();
}


std::string Zara::WiredSnake::AllDocuments(std::string dbName, std::string collectionName)
{
	std::ostringstream ss;
	ss << std::string();

	int dbIndex = getDbIndex(dbName);
	if (dbIndex < 0)
		return ss.str();
	int collIndex = getCollectionIndex(dbIndex, collectionName);
	if (collIndex < 0)
		return ss.str();

	fs::path path = collectionFilePath(dbName, collectionName);
	nlohmann::json collection = files->ReadJson(path);

	for (auto& document : collection["documents"])
		ss << document << "\n";

	return ss.str();
}
