#pragma once
#include "Type.h"

class LSB_ImageEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	struct FIBITMAP* inputFile;
public:
	LSB_ImageEncrypt();
	~LSB_ImageEncrypt();

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

class LSB_ImageDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	struct FIBITMAP* inputFile;

	void WithOriginal();
	void WithoutOriginal();
public:
	LSB_ImageDecrypt();
	~LSB_ImageDecrypt();

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};