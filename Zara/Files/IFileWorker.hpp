#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>


namespace Zara
{
	class IFileWorker
	{
	public:
		virtual bool IsDir(const std::filesystem::path path) = 0;
		virtual bool CreateDir(const std::filesystem::path path) = 0;
		virtual bool Exists(const std::filesystem::path path) const = 0;
		virtual bool CreateF(const std::filesystem::path path, const char* data) = 0;
		virtual bool CreateF(const std::filesystem::path, const std::string& data) = 0;
		virtual bool CreateF(const std::filesystem::path, const nlohmann::json& j) = 0;
		virtual std::string ReadAllFile(const std::filesystem::path path) = 0;
		virtual nlohmann::json ReadJson(const std::filesystem::path path) = 0;
		virtual bool DeleteF(const std::filesystem::path path) = 0;
	};
}
