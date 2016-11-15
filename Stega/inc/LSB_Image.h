#pragma once
#include "Type.h"

// Encrypt
class LSB_ImageEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	struct FIBITMAP* inputFile;

public:
	LSB_ImageEncrypt();
	~LSB_ImageEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

// DecryptType
class LSB_ImageDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	struct FIBITMAP* inputFile;

	/**
	* Called if you have the original image.
	*/
	void WithOriginal();
	/**
	* Called if you don't have the original image.
	*/
	void WithoutOriginal();
public:
	LSB_ImageDecrypt();
	~LSB_ImageDecrypt();

	// EncryptType Deterface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};