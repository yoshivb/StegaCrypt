#include "Bifid.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//Encrypt
BifidEncrypt::BifidEncrypt()
{
}

BifidEncrypt::~BifidEncrypt()
{
}

uint BifidEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;
	inputFile = std::ifstream( a_inputFilePath.c_str(), ios::binary );

	std::streampos fsize = inputFile.tellg();
	inputFile.seekg( 0, std::ios::end );
	inputFileSize = (uint)(inputFile.tellg() - fsize);
	inputFile.seekg( 0, std::ios::beg );

	string squareFilePath;

	bool validFile = false;
	while ( !validFile )
	{
		printf( "Polybius Square file:\n" );
		getline( cin, squareFilePath );
		validFile = (ParseSquareFile( squareFilePath, polybiusSquare ) != -1);
		if( !validFile ) printf( "Invalid file!\n" );
	}

	return inputFileSize;
}

void BifidEncrypt::Encrypt()
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * inputFileSize);
	string line;
	uint j = 0;
	while (getline(inputFile, line))
	{
		vector<Coords> coordList;
		uint i;
		Coords result;
		bool returnCarriage = (line[line.size() - 1] == '\r');
		uint lineSize = ( returnCarriage ) ? line.size() - 1 : line.size();

		// Re-pair coords.
		for ( i = 0; i < lineSize - 1; i+=2 )
		{
			Coords firstCoords = polybiusSquare.find( line[i] )->second;
			Coords secondCoords = polybiusSquare.find( line[i+1] )->second;
			result.y = firstCoords.y;
			result.x = secondCoords.y;
			coordList.push_back( result );
		}
		if ( i == lineSize - 1 )
		{
			Coords firstCoords = polybiusSquare.find( line[i] )->second;
			Coords secondCoords = polybiusSquare.find( line[0] )->second;
			result.y = firstCoords.y;
			result.x = secondCoords.x;
			coordList.push_back( result );
			i = 1;
		}
		else
			i = 0;
		for ( ; i < lineSize - 1; i += 2 )
		{
			Coords firstCoords = polybiusSquare.find( line[i] )->second;
			Coords secondCoords = polybiusSquare.find( line[i+1] )->second;
			result.y = firstCoords.x;
			result.x = secondCoords.x;
			coordList.push_back( result );
		}
		
		// Find letters to the new coords.
		for ( uint k = 0; k < coordList.size(); k++, j++ )
		{
			PolybiusSquare::iterator findResult = FindByCoords( polybiusSquare, coordList[k] );
			buffer[j] = findResult->first;
		}
		if ( j < inputFileSize )
		{
			if ( returnCarriage ) buffer[j++] = '\r';
			buffer[j++] = '\n';
		}
		printf("\rProgress: %i%%", (int)(((float)j / (float)inputFileSize) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), inputFileSize, outputFile);
	fclose(outputFile);
}

//Decrypt
BifidDecrypt::BifidDecrypt() {};
BifidDecrypt::~BifidDecrypt() {};

void BifidDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	encryption.Initialize( a_inputFilePath, a_outputFilePath );
}

void BifidDecrypt::Decrypt()
{
	uchar* buffer = (uchar*)malloc(sizeof(uchar) * encryption.inputFileSize);
	string line;
	uint j = 0;
	while (getline(encryption.inputFile, line))
	{
		vector<Coords> coordList;
		vector<uchar> coords;
		uint i;
		Coords result;
		bool returnCarriage = (line[line.size() - 1] == '\r');
		uint lineSize = ( returnCarriage ) ? line.size() - 1 : line.size();

		// Get all numbers.
		for ( i = 0; i < lineSize; i++ )
		{
			Coords curCoords = encryption.polybiusSquare.find( line[i] )->second;
			coords.push_back( curCoords.y );
			coords.push_back( curCoords.x );
		}

		// Re-pair coords.
		for ( i = 0; i < lineSize; i++ )
		{
			result.y = coords[i];
			result.x = coords[i+lineSize];
			coordList.push_back( result );
		}

		// Find letters to the coords.
		for ( i = 0; i < coordList.size(); i++ )
		{
			auto findResult = find_if(begin(encryption.polybiusSquare), end(encryption.polybiusSquare), [&](const PolybiusSquarePair &pair)
			{
				return pair.second.x == coordList[i].x && pair.second.y == coordList[i].y;
			});
			buffer[j++] = findResult->first;
		}
		if ( j < encryption.inputFileSize )
		{
			if ( returnCarriage ) buffer[j++] = '\r';
			buffer[j++] = '\n';
		}
		printf("\rProgress: %i%%", (int)(((float)j / (float)encryption.inputFileSize) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(encryption.outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), encryption.inputFileSize, outputFile);
	fclose(outputFile);
}