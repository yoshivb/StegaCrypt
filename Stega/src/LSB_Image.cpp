#include "LSB_Image.h"
#include "Shlwapi.h"
#include <string.h>
#include "FreeImage.h"
#include <iostream>
#include <vector>
using namespace std;

unsigned DLL_CALLCONV
_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return fread(buffer, size, count, (FILE *)handle);
}

unsigned DLL_CALLCONV
_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return fwrite(buffer, size, count, (FILE *)handle);
}

int DLL_CALLCONV
_SeekProc(fi_handle handle, long offset, int origin) {
	return fseek((FILE *)handle, offset, origin);
}

long DLL_CALLCONV
_TellProc(fi_handle handle) {
	return ftell((FILE *)handle);
}

FreeImageIO io;

//Encrypt
LSB_ImageEncrypt::LSB_ImageEncrypt()
{
}

LSB_ImageEncrypt::~LSB_ImageEncrypt()
{
}

uint LSB_ImageEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;
	FreeImage_Initialise();

	io.read_proc = _ReadProc;
	io.write_proc = _WriteProc;
	io.seek_proc = _SeekProc;
	io.tell_proc = _TellProc;

	FILE *file = fopen( a_inputFilePath.c_str(), "rb" );
	inputFile = FreeImage_LoadFromHandle( FIF_PNG, &io, (fi_handle)file, 0 );

	return (FreeImage_GetWidth(inputFile) * FreeImage_GetHeight(inputFile) * FreeImage_GetBPP(inputFile) ) / 32;
}

void LSB_ImageEncrypt::Encrypt(FILE* a_encryptFile, uint a_fileSize)
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * a_fileSize);
	unsigned int result = fread(buffer, 1, a_fileSize, a_encryptFile);

	uchar* inputImageBuffer = FreeImage_GetBits(inputFile);
	for (uint i = 0, k = 0; i < a_fileSize; i++)
	{
		for (uchar j = 8, mask = 0xC0; j > 0; k++)
		{
			uchar bitsMasked = (buffer[i] & mask) >> (j-2);
			uchar tmpChar = inputImageBuffer[k];
			inputImageBuffer[k] = ( (inputImageBuffer[k] >> 2) << 2) + bitsMasked;
			if (bitsMasked > 3 || (tmpChar - inputImageBuffer[k]) > 3)
				printf("Something is wrong!\n");
			if (tmpChar != inputImageBuffer[k])
				j-=2, mask >>= 2;
		}
		printf("Progress: %i%%\r", (int)(((float)i / (float)a_fileSize) * 100.0f));
	}
	printf("Progress: 100%%\r");
	printf("\n");
	FreeImage_Save( FIF_PNG, inputFile, outputFilePath.c_str() );

	FreeImage_DeInitialise();
}

//Decrypt
LSB_ImageDecrypt::LSB_ImageDecrypt() {};
LSB_ImageDecrypt::~LSB_ImageDecrypt() {};

void LSB_ImageDecrypt::WithOriginal()
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
	FIBITMAP* originalFile = FreeImage_LoadFromHandle(FIF_PNG, &io, (fi_handle)originalFileHandle, 0);

	vector<uchar> buffer;
	uint fileSize = (FreeImage_GetWidth(inputFile) * FreeImage_GetHeight(inputFile) * FreeImage_GetBPP(inputFile)) / 8;
	uchar* origBuffer = FreeImage_GetBits(originalFile);
	uchar* inputBuffer = FreeImage_GetBits(inputFile);
	uchar currentChar = 0;
	uchar mask = 3;
	uchar j = 8;
	for (uint i = 0; i < fileSize; i++)
	{
		if (origBuffer[i] != inputBuffer[i])
		{
			uchar maskedBits = inputBuffer[i] & mask;
			if (maskedBits > 3 || (inputBuffer[i]-origBuffer[i]) > 3)
				printf("Something is wrong!\n");
			currentChar += (maskedBits << (j-2));
			j -= 2;
			if (j == 0)
			{
				buffer.push_back(currentChar);
				currentChar = 0;
				j = 8;
			}
		}
		printf("Progress: %i%%\r", (int)(((float)i / (float)fileSize) * 100.0f));
	}
	printf("Progress: 100%%\r");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(&buffer[0], sizeof(uchar), buffer.size(), outputFile);
	fclose(outputFile);
}

void LSB_ImageDecrypt::WithoutOriginal()
{
	uint fileSizeGuess;
	printf("Estimate resulting file size:\n");
	{
		string input;
		getline(cin, input);
		int result = 0;
		string reverseInput(input.rbegin(), input.rend());
		int j = 1;
		for (unsigned int i = 0; i < reverseInput.length(); i++, j *= 10)
		{
			result += (reverseInput[i] - '0') * j;
		}
		fileSizeGuess = result;
	}
}

void LSB_ImageDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;

	FreeImage_Initialise();

	io.read_proc = _ReadProc;
	io.write_proc = _WriteProc;
	io.seek_proc = _SeekProc;
	io.tell_proc = _TellProc;

	FILE *file = fopen(a_inputFilePath.c_str(), "rb");
	inputFile = FreeImage_LoadFromHandle(FIF_PNG, &io, (fi_handle)file, 0);
}

void LSB_ImageDecrypt::Decrypt()
{
	printf("Original file available? [Y] or [N]\n");
	{
		string input;
		getline(cin, input);
		if (input[0] == 'y' || input[0] == 'Y')
		{
			WithOriginal();
		}
		else
		{
			WithoutOriginal();
		}
	}
}