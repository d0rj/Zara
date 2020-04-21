#pragma once

#include "IEncoder.hpp"


namespace Zara
{
	class EVP : virtual public IEncoder
	{
	private:
		unsigned char* iv = (unsigned char*)"0123456789012345";

		int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key,
			unsigned char* iv, unsigned char* ciphertext);
		int decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key,
			unsigned char* iv, unsigned char* plaintext);
		void handleErrors();
	public:
		const static uint32_t BuffSize = 256;

		std::string Encode(std::string data, std::string key) override;
		std::string Decode(std::string data, std::string key) override;
	};
}
