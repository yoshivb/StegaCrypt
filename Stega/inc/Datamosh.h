#pragma once
#include "Type.h"
#include <vector>

struct EditableChunk
{
	uint beginIndex;
	uint length;
};

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

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

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

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};