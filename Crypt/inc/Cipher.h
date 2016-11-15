#pragma once
#include "Type.h"

// CipherEncrypt base class
class CipherEncrypt: EncryptType
{
protected:
	std::string outputFilePath;
	uchar* fileBuffer;
	uint inputFileSize;
public:
	CipherEncrypt();
	~CipherEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt() = 0;
};

// CipherDecrypt  base class
class CipherDecrypt : DecryptType
{
public:
	CipherDecrypt();
	~CipherDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt() = 0;
};