#pragma once
#include "Type.h"

class CipherEncrypt: EncryptType
{
protected:
	std::string outputFilePath;
	uchar* fileBuffer;
	uint inputFileSize;
public:
	CipherEncrypt();
	~CipherEncrypt();

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt() = 0;
};

class CipherDecrypt : DecryptType
{
public:
	CipherDecrypt();
	~CipherDecrypt();

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt() = 0;
};