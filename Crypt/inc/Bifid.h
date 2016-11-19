#pragma once
#include "Cipher.h"
using namespace std;

// Encrypt
class BifidEncrypt: CipherEncrypt
{
	friend class BifidDecrypt;
protected:
	PolybiusSquare polybiusSquare;
	std::ifstream inputFile;
public:
	BifidEncrypt();
	~BifidEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();
};

// Decrypt
class BifidDecrypt : CipherDecrypt
{
private:
	BifidEncrypt encryption;
public:
	BifidDecrypt();
	~BifidDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};