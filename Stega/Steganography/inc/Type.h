#pragma once
#include <string>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

class EncryptType
{
public:
	virtual uint Initialize( std::string a_inputFilePath, std::string a_outputFilePath ) = 0;
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize) = 0;
};

class DecryptType
{
public:
	virtual void Initialize( std::string inputFilePath, std::string a_outputFilePath ) = 0;
	virtual void Decrypt() = 0;
};