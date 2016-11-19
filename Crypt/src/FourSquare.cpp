#include "FourSquare.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//Encrypt
FourSquareEncrypt::FourSquareEncrypt()
{
}

FourSquareEncrypt::~FourSquareEncrypt()
{
}

uint FourSquareEncrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;
	inputFile = std::ifstream( a_inputFilePath.c_str(), ios::binary );

	std::streampos fsize = inputFile.tellg();
	inputFile.seekg( 0, std::ios::end );
	inputFileSize = (uint)(inputFile.tellg() - fsize);
	inputFile.seekg( 0, std::ios::beg );

	string squareFilePath;

	int squareSize = -1;
	while ( squareSize == -1 )
	{
		printf( "Polybius Square file:\n" );
		getline( cin, squareFilePath );
		squareSize = ParseSquareFile( squareFilePath, plaintextSquare );
		if( squareSize == -1 ) printf( "Invalid file!\n" );
	}

	string keyword1;
	string keyword2;
	printf( "Keyword 1:\n" );
	getline( cin, keyword1 );
	ParseKeyword( keyword1 );
	printf( "Keyword 2:\n" );
	getline( cin, keyword2 );
	ParseKeyword( keyword2 );

	uint i;
	for ( i = 0; i < keyword1.length(); i++ )
	{
		Coords coords;
		coords.x = i % squareSize;
		coords.y = i / squareSize;
		topCipherSquare.insert( PolybiusSquarePair( keyword1[i], coords ) );
	}
	for ( auto const& item : plaintextSquare )
	{
		if ( keyword1.find( item.first ) != string::npos )
			continue;
		Coords coords;
		coords.x = i % squareSize;
		coords.y = i / squareSize;
		topCipherSquare.insert( PolybiusSquarePair( item.first, coords ) );
		i++;
	}

	for ( i = 0; i < keyword2.length(); i++ )
	{
		Coords coords;
		coords.x = i % squareSize;
		coords.y = i / squareSize;
		bottomCipherSquare.insert( PolybiusSquarePair( keyword2[i], coords ) );
	}
	for ( auto const& item : plaintextSquare )
	{
		if ( keyword2.find( item.first ) != string::npos )
			continue;
		Coords coords;
		coords.x = i % squareSize;
		coords.y = i / squareSize;
		bottomCipherSquare.insert( PolybiusSquarePair( item.first, coords ) );
		i++;
	}
		
	return inputFileSize;
}

void FourSquareEncrypt::ParseKeyword( string& a_keyword )
{
	auto last = a_keyword.end();

	for ( auto first = a_keyword.begin(); first != last; first++ )
    {
        last = std::remove( std::next( first ), last, *first );
    }

	a_keyword.erase( last, a_keyword.end() );
}

void FourSquareEncrypt::Encrypt()
{
	string data;

	inputFile.seekg(0, std::ios::end);   
	data.reserve(inputFile.tellg());
	inputFile.seekg(0, std::ios::beg);

	data.assign( ( std::istreambuf_iterator<char>(inputFile) ), std::istreambuf_iterator<char>() );

	// Remove all whitespaces.
	data.erase( remove_if( data.begin(), data.end(), isspace ), data.end() );
	if ( data.size() & 1 )
		data.push_back( 'x' );

	uchar* buffer = (uchar*)malloc(sizeof(uchar) * data.length());
	for ( uint i = 0; i < data.size(); i += 2 )
	{
		Coords first = plaintextSquare[data[i]];
		Coords second = plaintextSquare[data[i+1]];

		Coords firstCipherCoords;
		firstCipherCoords.x = second.x;
		firstCipherCoords.y = first.y;
		Coords secondCipherCoords;
		secondCipherCoords.x = first.x;
		secondCipherCoords.y = second.y;

		PolybiusSquare::iterator firstCiphertext = FindByCoords( topCipherSquare, firstCipherCoords );
		PolybiusSquare::iterator secondCiphertext = FindByCoords( bottomCipherSquare, secondCipherCoords );
		buffer[i] = firstCiphertext->first;
		buffer[i+1] = secondCiphertext->first;
		printf("\rProgress: %i%%", (int)(((float)i / (float)data.length()) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), data.length(), outputFile);
	fclose(outputFile);
}

//Decrypt
FourSquareDecrypt::FourSquareDecrypt() {};
FourSquareDecrypt::~FourSquareDecrypt() {};

void FourSquareDecrypt::Initialize(std::string a_inputFilePath, std::string a_outputFilePath)
{
	encryption.Initialize( a_inputFilePath, a_outputFilePath );
}

void FourSquareDecrypt::Decrypt()
{
	string data;

	encryption.inputFile.seekg(0, std::ios::end);   
	data.reserve(encryption.inputFile.tellg());
	encryption.inputFile.seekg(0, std::ios::beg);

	data.assign( ( std::istreambuf_iterator<char>(encryption.inputFile) ), std::istreambuf_iterator<char>() );

	uchar* buffer = (uchar*)malloc(sizeof(uchar) * data.length());
	for ( uint i = 0; i < data.size(); i += 2 )
	{
		Coords first = encryption.topCipherSquare[data[i]];
		Coords second = encryption.bottomCipherSquare[data[i+1]];

		Coords firstCipherCoords;
		firstCipherCoords.x = second.x;
		firstCipherCoords.y = first.y;
		Coords secondCipherCoords;
		secondCipherCoords.x = first.x;
		secondCipherCoords.y = second.y;

		PolybiusSquare::iterator firstCiphertext = FindByCoords( encryption.plaintextSquare, firstCipherCoords );
		PolybiusSquare::iterator secondCiphertext = FindByCoords( encryption.plaintextSquare, secondCipherCoords );
		buffer[i] = firstCiphertext->first;
		buffer[i+1] = secondCiphertext->first;
		printf("\rProgress: %i%%", (int)(((float)i / (float)data.length()) * 100.0f));
	}
	printf("\rProgress: 100%%");
	printf("\n");

	FILE* outputFile = fopen(encryption.outputFilePath.c_str(), "wb");
	fwrite(buffer, sizeof(uchar), data.length(), outputFile);
	fclose(outputFile);
}