#include "LSB_Audio.h"
#include "Shlwapi.h"
#include <iostream>
using namespace std;

//Encrypt
LSB_AudioEncrypt::LSB_AudioEncrypt()
{
}

LSB_AudioEncrypt::~LSB_AudioEncrypt()
{
}

uint LSB_AudioEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	FILE* inputFile = fopen(a_inputFilePath.c_str(), "rb");
	outputFilePath = a_outputFilePath;

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	fileBuffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	unsigned int result = fread(fileBuffer, 1, inputFileSize, inputFile);
	fclose(inputFile);

	offset = 0;
	// Find the beginning of the data.
	for (uint i = 0; i < inputFileSize; i++)
	{
		if (fileBuffer[i] == 'd' && fileBuffer[i+1] == 'a' &&fileBuffer[i+2] == 't' &&fileBuffer[i+3] == 'a' )
		{
			offset = i + 4;
			break;
		}
	}
	dataSize = 0;
	memcpy(&dataSize, fileBuffer + offset, 4);
	bytesPerSample = 0;
	memcpy(&bytesPerSample, fileBuffer + 34, 2);
	bytesPerSample = bytesPerSample / 8;

	return dataSize / 8;
}

void LSB_AudioEncrypt::Encrypt(FILE* a_encryptFile, uint a_fileSize)
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * a_fileSize);
	unsigned int result = fread(buffer, 1, a_fileSize, a_encryptFile);

	for (uint i = 0, k = offset + 4; i < a_fileSize; i++)
	{
		for (uchar j = 8, mask = 0xC0; j > 0; k+=bytesPerSample)
		{
			uchar bitsMasked = (buffer[i] & mask) >> (j - 2);
			uchar tmpChar = fileBuffer[k];
			fileBuffer[k] = ((fileBuffer[k] >> 2) << 2) + bitsMasked;
			if (bitsMasked > 3 || (tmpChar - fileBuffer[k]) > 3)
				printf("Something is wrong!\n");
			if (tmpChar != fileBuffer[k])
				j -= 2, mask >>= 2;
		}
		printf("\rProgress: %i%%", (int)(((float)i / (float)a_fileSize) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(fileBuffer, sizeof(uchar), inputFileSize, outputFile);
	fclose(outputFile);
}

//Decrypt
LSB_AudioDecrypt::LSB_AudioDecrypt() {};
LSB_AudioDecrypt::~LSB_AudioDecrypt() {};

void LSB_AudioDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	FILE* inputFile = fopen(a_inputFilePath.c_str(), "rb");
	outputFilePath = a_outputFilePath;

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	fileBuffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	unsigned int result = fread(fileBuffer, 1, inputFileSize, inputFile);
	fclose(inputFile);

	offset = 0;
	// Find the beginning of the data.
	for (uint i = 0; i < inputFileSize; i++)
	{
		if (fileBuffer[i] == 'd' && fileBuffer[i + 1] == 'a' &&fileBuffer[i + 2] == 't' &&fileBuffer[i + 3] == 'a')
		{
			offset = i + 4;
			break;
		}
	}
	dataSize = 0;
	memcpy(&dataSize, fileBuffer + offset, 4);
	bytesPerSample = 0;
	memcpy(&bytesPerSample, fileBuffer + 34, 2);
	bytesPerSample = bytesPerSample / 8;
}

void LSB_AudioDecrypt::Decrypt()
{
	string originalFilePath;
	{
		int validPath = 0;
		string inputPath;
		wstring w_inputPath;
		while (!validPath)
		{
			printf("Original file path:\n");
			getline(cin, inputPath);
			if (*inputPath.begin() == '"' && *inputPath.rbegin() == '"')
			{
				inputPath = inputPath.substr(1, inputPath.length() - 2);
			}
			w_inputPath = wstring(inputPath.begin(), inputPath.end());
			validPath = PathFileExists(w_inputPath.c_str());
			if (validPath == 0)
			{
				printf("Invalid path.\n");
			}
		}
		originalFilePath = inputPath;
	}

	FILE* originalFileHandle = fopen(originalFilePath.c_str(), "rb");

	fseek(originalFileHandle, 0, SEEK_END);
	uint origFileSize = ftell(originalFileHandle);
	rewind(originalFileHandle);

	uchar* buffer = (uchar*)malloc(sizeof(uchar) * origFileSize);
	unsigned int result = fread(buffer, 1, origFileSize, originalFileHandle);
	fclose(originalFileHandle);

	uchar currentChar = 0;
	uchar mask = 3;
	uchar j = 8;
	for (uint i = 0; i < dataSize; i+=bytesPerSample)
	{
		uint k = i + offset + 4;
		if (fileBuffer[k] != buffer[k])
		{
			uchar maskedBits = fileBuffer[k] & mask;
			if (maskedBits > 3 || (fileBuffer[k] - buffer[k]) > 3)
				printf("Something is wrong!\n");
			currentChar += (maskedBits << (j - 2));
			j -= 2;
			if (j == 0)
			{
				outputBuffer.push_back(currentChar);
				currentChar = 0;
				j = 8;
			}
		}
	}

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(&outputBuffer[0], sizeof(uchar), outputBuffer.size(), outputFile);
	fclose(outputFile);
}