#pragma once

#include "IFileWorker.hpp"
#include "../Crypto/IEncoder.hpp"
#include <fstream>


namespace Zara
{
	class CryptoFileWorker : virtual public IFileWorker
	{
	private:
		std::ofstream output;
		std::ifstream input;
		IEncoder* encoder;
		std::string key;
	public:
		CryptoFileWorker(IEncoder* encoder, std::string key) : encoder(encoder), key(key) {}

		bool IsDir(const fs::path path);
		bool CreateDir(const fs::path path);
		bool CheckOrCreateDir(const fs::path path);
		bool Exists(const fs::path path) const;
		bool CreateF(const fs::path path, const char* data);
		bool CreateF(const fs::path path, const std::string& data);
		bool CreateF(const fs::path path, const nlohmann::json& j);
		std::string ReadAllFile(const fs::path path);
		nlohmann::json ReadJson(const fs::path path);
		bool DeleteF(const fs::path path);
	};
}