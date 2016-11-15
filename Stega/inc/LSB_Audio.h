#pragma once
#include "Type.h"
#include <vector>

class LSB_AudioEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	uchar* fileBuffer;
	uint inputFileSize;

	// Audio variables
	ushort bytesPerSample;
	uint offset;
	uint dataSize;
public:
	LSB_AudioEncrypt();
	~LSB_AudioEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

class LSB_AudioDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	uchar* fileBuffer;
	uint inputFileSize;
	std::vector<uchar> outputBuffer;

	// Audio variables
	ushort bytesPerSample;
	uint offset;
	uint dataSize;
public:
	LSB_AudioDecrypt();
	~LSB_AudioDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};