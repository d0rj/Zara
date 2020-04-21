#include "EVP.hpp"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>


int Zara::EVP::encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext)
{
	EVP_CIPHER_CTX* ctx;
	int len;
	int ciphertext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		handleErrors();
	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}


int Zara::EVP::decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext)
{
	EVP_CIPHER_CTX* ctx;
	int len;
	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		handleErrors();
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}


// TODO: 
void Zara::EVP::handleErrors()
{
}


std::string Zara::EVP::Encode(std::string data, std::string key)
{
	unsigned char* plainText = (unsigned char*)data.c_str();
	unsigned char cipherText[BuffSize];

	int cipher_len = encrypt(plainText, strlen((const char*)plainText), (unsigned char*)key.c_str(),
		iv,
		cipherText);

	std::string result;
	for (int i = 0; i < cipher_len; ++i)
		result.push_back(cipherText[i]);

	return result;
}


std::string Zara::EVP::Decode(std::string data, std::string key)
{
	unsigned char decryptedtext[BuffSize];
	int decripted_len = decrypt((unsigned char*)data.c_str(), data.length(), (unsigned char*)key.c_str(),
		iv,
		decryptedtext);

	std::string result;	
	for (int i = 0; i < decripted_len; ++i)
		result.push_back(decryptedtext[i]);

	return result;
}
