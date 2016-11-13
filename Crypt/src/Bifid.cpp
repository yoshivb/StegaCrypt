#include "Bifid.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

//Encrypt
BifidEncrypt::BifidEncrypt()
{
}

BifidEncrypt::~BifidEncrypt()
{
}

bool BifidEncrypt::ParseSquareFile( string& a_squareFilePath )
{
	std::ifstream squareFile(a_squareFilePath.c_str());

	uint size = -1;
	Coords coords;
	coords.y = 0;

	string line;
	while (getline(squareFile, line))
	{
		std::istringstream iss(line);
		
		if ( size == -1 )
			size = line.size();
		else if ( line.size() != size )
			return false;

		for ( coords.x = 0; coords.x < line.size(); coords.x++ )
			polybiusSquare.insert( PolybiusSquarePair( line[coords.x], coords) );

		coords.y++;
	}

	return ( coords.y == size );
}

uint BifidEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;
	inputFile = std::ifstream( a_inputFilePath.c_str(), ios::binary );

	std::streampos fsize = inputFile.tellg();
	inputFile.seekg( 0, std::ios::end );
	inputFileSize = inputFile.tellg() - fsize;
	inputFile.seekg( 0, std::ios::beg );

	string squareFilePath;

	bool validFile = false;
	while ( !validFile )
	{
		printf( "Polybius Square file:\n" );
		getline( cin, squareFilePath );
		validFile = ParseSquareFile( squareFilePath );
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
		for ( i = 0; i < line.size() - 1; i+=2 )
		{
			Coords firstCoords = polybiusSquare.find( line[i] )->second;
			Coords secondCoords = polybiusSquare.find( line[i+1] )->second;
			result.y = firstCoords.y;
			result.x = secondCoords.y;
			coordList.push_back( result );
		}
		if ( i == line.size() - 1 )
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
		for ( ; i < line.size() - 1; i += 2 )
		{
			Coords firstCoords = polybiusSquare.find( line[i] )->second;
			Coords secondCoords = polybiusSquare.find( line[i+1] )->second;
			result.y = firstCoords.x;
			result.x = secondCoords.x;
			coordList.push_back( result );
		}
		for ( uint k = 0; k < coordList.size(); k++, j++ )
		{
			auto findResult = find_if(begin(polybiusSquare), end(polybiusSquare), [&](const PolybiusSquarePair &pair)
			{
				return pair.second.x == coordList[k].x && pair.second.y == coordList[k].y;
			});
			buffer[j] = findResult->first;
		}
		if ( j < inputFileSize )
			buffer[j++] = '\n';
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
	encryption.Encrypt();
}