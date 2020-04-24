#pragma once

#include "IFileWorker.hpp"


namespace Zara
{
	class FileWorker : virtual public IFileWorker
	{
	public:
		virtual bool IsDir(const std::filesystem::path path) override;
		virtual bool CreateDir(const std::filesystem::path path) override;
		virtual bool Exists(const std::filesystem::path path) const override;
		virtual bool CreateF(const std::filesystem::path path, const char* data) override;
		virtual bool CreateF(const std::filesystem::path path, const std::string& data) override;
		virtual bool CreateF(const std::filesystem::path path, const nlohmann::json& j) override;
		virtual std::string ReadAllFile(const std::filesystem::path path) override;
		virtual nlohmann::json ReadJson(const std::filesystem::path path) override;
		virtual bool DeleteF(const std::filesystem::path path) override;
	};
}
