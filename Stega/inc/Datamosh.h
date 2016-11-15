#pragma once
#include "Type.h"
#include <vector>

struct EditableChunk
{
	uint beginIndex;
	uint length;
};

// Encrypt
class DatamoshEncrypt : EncryptType
{
private:
	std::string outputFilePath;
	uint inputFileSize;
	FILE* inputFile;
	std::vector<EditableChunk> editableChunks;
	uchar* fileBuffer;
public:
	DatamoshEncrypt();
	~DatamoshEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

// Decrypt
class DatamoshDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	FILE* encryptedFile;
	uint encryptedFileSize;
	uchar* encryptedBytes;
public:
	DatamoshDecrypt();
	~DatamoshDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};