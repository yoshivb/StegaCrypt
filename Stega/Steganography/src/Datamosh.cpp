#include "Datamosh.h"
#include "Shlwapi.h"
#include <string.h>
#include "FreeImage.h"
#include <iostream>
#include <assert.h>
using namespace std;

//Encrypt
DatamoshEncrypt::DatamoshEncrypt()
{
}

DatamoshEncrypt::~DatamoshEncrypt()
{
}

uint DatamoshEncrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;

	inputFile = fopen(a_inputFilePath.c_str(), "rb");

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	fileBuffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	unsigned int result = fread(fileBuffer, 1, inputFileSize, inputFile);
	fclose(inputFile);

	bool dataFound = false;
	EditableChunk currentChunk;
	for (int i = 0; i < inputFileSize; i++)
	{
		if (fileBuffer[i] == 0xFF && fileBuffer[i + 1] == 0xD9)
		{
			currentChunk.length = (i - 1) - currentChunk.beginIndex;
			editableChunks.push_back(currentChunk);
			currentChunk = EditableChunk();
			dataFound = false;
		}
		if (fileBuffer[i] == 0xFF && fileBuffer[i + 1] == 0xDA)
		{
			currentChunk.beginIndex = i+2;
			dataFound = true;
			i++;
			continue;
		}
	}
	return inputFileSize/3;
}

void DatamoshEncrypt::Encrypt(FILE* a_encryptFile, uint a_fileSize)
{
	string mode;
	char modeIndex = -1;
	bool modeSelected = false;
	uchar interval = 1;
	while (!modeSelected)
	{
		printf("[F]ull file, [B]ody only, [H]alf body:\n");
		getline(cin, mode);
		if (mode[0] == 'f' || mode[0] == 'F')
			modeIndex = 0, modeSelected = true;
		else if (mode[0] == 'b' || mode[0] == 'B')
			modeIndex = 1, modeSelected = true;
		else if (mode[0] == 'h' || mode[0] == 'H')
			modeIndex = 2, modeSelected = true;
		else
			printf("Invalid input.\n");
	}
	{
		printf("Interval count:\n");
		std::string intervalString;
		getline(cin, intervalString);
		int result = 0;
		string reverseInput(intervalString.rbegin(), intervalString.rend());
		int j = 1;
		for (unsigned int i = 0; i < reverseInput.length(); i++, j *= 10)
		{
			result += (reverseInput[i] - '0') * j;
		}
		interval = result;
	}
	
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * a_fileSize);
	unsigned int result = fread(buffer, 1, a_fileSize, a_encryptFile);

	FreeImage_Initialise();

	uint j = 0;
	uint chunkIndex = editableChunks.size() - 1;
	uint startIndex = 4;
	if (modeIndex == 1)
		startIndex = editableChunks[chunkIndex].beginIndex;
	else if (modeIndex == 2)
		startIndex = editableChunks[chunkIndex].beginIndex + (editableChunks[chunkIndex].length/2);
	for (uint i = startIndex; i < inputFileSize && j < a_fileSize; i += interval)
	{
		if (fileBuffer[i] == buffer[j]) continue;
		FIMEMORY *fiMemory = FreeImage_OpenMemory(fileBuffer, inputFileSize);
		uchar backupChar = fileBuffer[i];
		fileBuffer[i] = buffer[j];

		FIBITMAP *dib = FreeImage_LoadFromMemory(FIF_JPEG, fiMemory);

		if (dib != NULL)
		{
			j++;
			printf("Progress: %i%%\r", (int)(((float)j / (float)a_fileSize) * 100.0f));
		}
		else
			fileBuffer[i] = backupChar;

		// don't forget to free the dib !
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(fiMemory); 
	}
	printf("\n");
	if (j < a_fileSize)
	{
		printf("Could not encrypt file.\n");
		return;
	}
	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(fileBuffer, sizeof(uchar), inputFileSize, outputFile);
	fclose(outputFile);
}

//Decrypt
DatamoshDecrypt::DatamoshDecrypt()
{
}

DatamoshDecrypt::~DatamoshDecrypt()
{
}

void DatamoshDecrypt::Initialize(std::string inputFilePath, std::string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;

	encryptedFile = fopen(inputFilePath.c_str(), "rb");
	fseek(encryptedFile, 0, SEEK_END);
	encryptedFileSize = ftell(encryptedFile);
	rewind(encryptedFile);

	encryptedBytes = (uchar*)malloc(sizeof(uchar) * encryptedFileSize);
	unsigned int result = fread(encryptedBytes, 1, encryptedFileSize, encryptedFile);
}

void DatamoshDecrypt::Decrypt()
{
	string originalFilePath;
	{
		bool validPath = false;
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
			if (!validPath)
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

	uchar* origBuffer = (uchar*)malloc(sizeof(uchar) * origFileSize);
	unsigned int result = fread(origBuffer, 1, origFileSize, originalFileHandle);

	vector<uchar> diffBytes;
	for (uint i = 0; i < encryptedFileSize; i++)
	{
		if (origBuffer[i] != encryptedBytes[i])
			diffBytes.push_back( encryptedBytes[i] );
		printf("Progress: %i%%\r", (int)(((float)i / (float)encryptedFileSize) * 100.0f));
	}
	printf("Progress: 100%%\r");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(&diffBytes[0], sizeof(uchar), diffBytes.size(), outputFile);
	fclose(outputFile);
}