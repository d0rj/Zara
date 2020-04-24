#include "FileWorker.hpp"
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


bool Zara::FileWorker::IsDir(const fs::path path)
{
	return fs::is_directory(path);
}


bool Zara::FileWorker::CreateDir(const fs::path path)
{
	return fs::create_directory(path);
}


bool Zara::FileWorker::CheckOrCreateDir(const fs::path path)
{
	if (!Exists(path))
		return CreateDir(path);

	return true;
}


bool Zara::FileWorker::Exists(const fs::path path) const
{
	return fs::exists(path);
}


bool Zara::FileWorker::CreateF(const fs::path path, const char* data)
{
	output.open(path);
	output << data;
	output.close();

	return true;
}


bool Zara::FileWorker::CreateF(const fs::path path, const std::string& data)
{
	return CreateF(path, data.c_str());
}


bool Zara::FileWorker::CreateF(const fs::path path, const nlohmann::json& j)
{
	output.open(path);
	output << j;
	output.close();

	return true;
}


std::string Zara::FileWorker::ReadAllFile(const fs::path path)
{
	std::string result;

	input.open(path);
	result.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
	input.close();

	return result;
}


nlohmann::json Zara::FileWorker::ReadJson(const fs::path path)
{
	nlohmann::json j;

	input.open(path);
	input >> j;
	input.close();

	return j;
}


bool Zara::FileWorker::DeleteF(const fs::path path)
{
	return fs::remove(path);
}
