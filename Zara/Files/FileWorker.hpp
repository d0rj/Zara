#pragma once

#include "IFileWorker.hpp"
#include <fstream>


namespace Zara
{
	class FileWorker : virtual public IFileWorker
	{
	private:
		std::ifstream input;
		std::ofstream output;
	public:
		bool IsDir(const fs::path path) override;
		bool CreateDir(const fs::path path) override;
		bool CheckOrCreateDir(const fs::path path) override;
		bool Exists(const fs::path path) const override;
		bool CreateF(const fs::path path, const char* data) override;
		bool CreateF(const fs::path path, const std::string& data) override;
		bool CreateF(const fs::path path, const nlohmann::json& j) override;
		std::string ReadAllFile(const fs::path path) override;
		nlohmann::json ReadJson(const fs::path path) override;
		bool DeleteF(const fs::path path) override;
	};
}
