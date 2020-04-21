#pragma once

#include <string>
#include <nlohmann/json.hpp>


namespace Zara
{
	class IFileWorker
	{
	public:
		virtual bool IsDir(const std::string path) = 0;
		virtual bool CreateDir(const std::string path) = 0;
		virtual bool Exists(const std::string path) const = 0;
		virtual bool CreateF(const std::string path, const char* data) = 0;
		virtual bool CreateF(const std::string path, const std::string& data) = 0;
		virtual bool CreateF(const std::string path, const nlohmann::json& j) = 0;
		virtual std::string ReadAllFile(const std::string path) = 0;
		virtual nlohmann::json ReadJson(const std::string path) = 0;
		virtual bool DeleteF(const std::string path) = 0;
	};
}
