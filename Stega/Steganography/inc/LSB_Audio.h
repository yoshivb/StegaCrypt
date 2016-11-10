#pragma once
#include "Type.h"
#include <vector>

class LSB_AudioEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	uint dataSize;
	ushort bytesPerSample;
	uint offset;
	uchar* fileBuffer;
	uint inputFileSize;
public:
	LSB_AudioEncrypt();
	~LSB_AudioEncrypt();

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

class LSB_AudioDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	uchar* fileBuffer;
	uint inputFileSize;
	ushort bytesPerSample;
	uint offset;
	uint dataSize;
	std::vector<uchar> outputBuffer;
public:
	LSB_AudioDecrypt();
	~LSB_AudioDecrypt();

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};