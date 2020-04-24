#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>


namespace fs = std::filesystem;


namespace Zara
{
	class IFileWorker
	{
	public:
		virtual bool IsDir(const fs::path path) = 0;
		virtual bool CreateDir(const fs::path path) = 0;
		virtual bool CheckOrCreateDir(const fs::path path) = 0;
		virtual bool Exists(const fs::path path) const = 0;
		virtual bool CreateF(const fs::path path, const char* data) = 0;
		virtual bool CreateF(const fs::path, const std::string& data) = 0;
		virtual bool CreateF(const fs::path, const nlohmann::json& j) = 0;
		virtual std::string ReadAllFile(const fs::path path) = 0;
		virtual nlohmann::json ReadJson(const fs::path path) = 0;
		virtual bool DeleteF(const fs::path path) = 0;
	};
}
