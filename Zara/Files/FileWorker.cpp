#include "FileWorker.hpp"
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


bool Zara::FileWorker::IsDir(const std::filesystem::path path)
{
	return fs::is_directory(path);
}


bool Zara::FileWorker::CreateDir(const std::filesystem::path path)
{
	return fs::create_directory(path);
}


bool Zara::FileWorker::Exists(const std::filesystem::path path) const
{
	return fs::exists(path);
}


bool Zara::FileWorker::CreateF(const std::filesystem::path path, const char* data)
{
	std::ofstream file(path);
	file << data;
	file.close();

	return true;
}


bool Zara::FileWorker::CreateF(const std::filesystem::path path, const std::string& data)
{
	return CreateF(path, data.c_str());
}


bool Zara::FileWorker::CreateF(const std::filesystem::path path, const nlohmann::json& j)
{
	std::ofstream file(path);
	file << j;

	return true;
}


std::string Zara::FileWorker::ReadAllFile(const std::filesystem::path path)
{
	std::ifstream file(path);
	std::string result;
	result.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	return result;
}


nlohmann::json Zara::FileWorker::ReadJson(const std::filesystem::path path)
{
	std::ifstream file(path);
	nlohmann::json j;
	file >> j;

	return j;
}


bool Zara::FileWorker::DeleteF(const std::filesystem::path path)
{
	return fs::remove(path);
}
