#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "Defines.h"

// Useful typedef's.
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

// EncryptType base class.
class EncryptType
{
public:
	/**
	* Intializes the class.
	* @param string The filepath of the file to hide a file in.
	* @param string The filepath for the output file.
	* @return uint The maximum size of the file to encrypt.
	*/
	virtual uint Initialize( std::string a_inputFilePath, std::string a_outputFilePath ) = 0;
	/**
	* Decrypts the file.
	*/
	virtual void Encrypt() = 0;
};

// DecryptType base class.
class DecryptType
{
public:
	/**
	* Intializes the class.
	* @param string The filepath of the file to decrypt.
	* @param string The filepath for the output file.
	* @return uint The maximum size of the file to encrypt.
	*/
	virtual void Initialize( std::string inputFilePath, std::string a_outputFilePath ) = 0;
	/**
	* Decrypts the file.
	*/
	virtual void Decrypt() = 0;
};

// Polybius Square types.
struct Coords
{
	uchar x, y;
};
typedef std::pair<uchar, Coords> PolybiusSquarePair;
typedef std::map<uchar, Coords> PolybiusSquare;

/**
* Parse a square file into PolybiusSquare.
* @param string The file path.
* @param PolybiusSquare& The output PolybiusSquare.
* @return int Is -1 if file is not valid otherwise square size.
*/
int ParseSquareFile( std::string& a_squareFilePath, PolybiusSquare& a_squareOut );

/**
* Find the item using coords.
* @param PolybiusSquare& The PolybiusSquare.
* @param Coords The coords.
* @return iterator The item.
*/
PolybiusSquare::iterator FindByCoords( PolybiusSquare& a_square, Coords a_coords );