#pragma once

#include "IFileWorker.hpp"


namespace Zara
{
	class FileWorker : virtual public IFileWorker
	{
	public:
		virtual bool IsDir(const std::string path) override;
		virtual bool CreateDir(const std::string path) override;
		virtual bool Exists(const std::string path) const override;
		virtual bool CreateF(const std::string path, const char* data) override;
		virtual bool CreateF(const std::string path, const std::string& data) override;
		virtual bool CreateF(const std::string path, const nlohmann::json& j) override;
		virtual std::string ReadAllFile(const std::string path) override;
		virtual nlohmann::json ReadJson(const std::string path) override;
		virtual bool DeleteF(const std::string path) override;
	};
}
