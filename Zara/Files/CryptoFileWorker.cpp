#include "CryptoFileWorker.hpp"
#include <iostream>


bool Zara::CryptoFileWorker::IsDir(const fs::path path)
{
	return fs::is_directory(path);
}


bool Zara::CryptoFileWorker::CreateDir(const fs::path path)
{
	return fs::create_directory(path);
}


bool Zara::CryptoFileWorker::CheckOrCreateDir(const fs::path path)
{
	if (!Exists(path))
		return CreateDir(path);

	return true;
}


bool Zara::CryptoFileWorker::Exists(const fs::path path) const
{
	return fs::exists(path);
}


bool Zara::CryptoFileWorker::CreateF(const fs::path path, const char* data)
{
	output.open(path, std::ios::out | std::fstream::trunc);
	output << encoder->Encode(data, key);
	output.close();

	return true;
}


bool Zara::CryptoFileWorker::CreateF(const fs::path path, const std::string& data)
{
	return CreateF(path, data.c_str());
}


bool Zara::CryptoFileWorker::CreateF(const fs::path path, const nlohmann::json& j)
{
	std::string data = encoder->Encode(j.dump(), key);

	output.open(path, std::ios::out | std::fstream::trunc | std::ios::binary);
	output << data;
	output.close();

	return true;
}


std::string Zara::CryptoFileWorker::ReadAllFile(const fs::path path)
{
	input.open(path, std::ios::binary);
	std::string readed;
	readed.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());

	std::string result = encoder->Decode(readed, key);

	input.close();
	return result;
}


nlohmann::json Zara::CryptoFileWorker::ReadJson(const fs::path path)
{
	return nlohmann::json::parse(ReadAllFile(path));
}


bool Zara::CryptoFileWorker::DeleteF(const fs::path path)
{
	return fs::remove(path);
}
