#include "Cipher.h"
#include <iostream>
using namespace std;

//Encrypt
CipherEncrypt::CipherEncrypt()
{
}

CipherEncrypt::~CipherEncrypt()
{
}

uint CipherEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	FILE* inputFile = fopen(a_inputFilePath.c_str(), "rb");
	outputFilePath = a_outputFilePath;

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	fileBuffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	unsigned int result = fread(fileBuffer, 1, inputFileSize, inputFile);
	fclose(inputFile);

	return inputFileSize;
}

//Decrypt
CipherDecrypt::CipherDecrypt() {};
CipherDecrypt::~CipherDecrypt() {};

void CipherDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
}