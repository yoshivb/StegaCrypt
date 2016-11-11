#include "ROT.h"
#include <iostream>
using namespace std;

//Encrypt
ROTEncrypt::ROTEncrypt()
{
}

ROTEncrypt::~ROTEncrypt()
{
}

uint ROTEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	FILE* inputFile = fopen(a_inputFilePath.c_str(), "rb");
	outputFilePath = a_outputFilePath;

	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);

	fileBuffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	unsigned int result = fread(fileBuffer, 1, inputFileSize, inputFile);
	fclose(inputFile);

	printf("ROT number:\n");
	cin >> ROTNumber;

	string mode;
	bool modeSelected = false;
	uchar interval = 1;
	getline( cin, mode );
	while (!modeSelected)
	{
		printf("[T]ext or [B]ytes:\n");
		getline(cin, mode);
		if (mode[0] == 't' || mode[0] == 'T')
			modeIndex = 0, modeSelected = true;
		else if (mode[0] == 'b' || mode[0] == 'B')
			modeIndex = 1, modeSelected = true;
		else
			printf("Invalid input.\n");
	}

	return inputFileSize;
}

void ROTEncrypt::TextEncrypt()
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);

	for ( uint i = 0; i < inputFileSize; i++ )
	{
		uint output = fileBuffer[i];
		if ( output >= 'a' &&  output <= 'z' )
		{
			output = ( ( ( output - 'a' ) + ROTNumber ) % 26 ) + 'a';
		}
		else if ( output >= 'A' && output <= 'Z' )
		{
			output = ( ( ( output - 'A' ) + ROTNumber ) % 26 ) + 'A';
		}
		buffer[i] = output;
		printf("\rProgress: %i%%", (int)(((float)i / (float)inputFileSize) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), inputFileSize, outputFile);
	fclose(outputFile);
}

void ROTEncrypt::ByteEncrypt()
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);

	for ( uint i = 0; i < inputFileSize; i++ )
	{
		buffer[i] = fileBuffer[i] + ROTNumber;
		printf("\rProgress: %i%%", (int)(((float)i / (float)inputFileSize) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), inputFileSize, outputFile);
	fclose(outputFile);
}

void ROTEncrypt::Encrypt()
{
	if ( modeIndex == 0 )
		TextEncrypt();
	else
		ByteEncrypt();
}

//Decrypt
ROTDecrypt::ROTDecrypt() {};
ROTDecrypt::~ROTDecrypt() {};

void ROTDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	encryption.Initialize( a_inputFilePath, a_outputFilePath );
	if( encryption.modeIndex == 0 ) 
		encryption.ROTNumber = 26 - encryption.ROTNumber;
	else
		encryption.ROTNumber = 256 - encryption.ROTNumber;
}

void ROTDecrypt::Decrypt()
{
	encryption.Encrypt();
}