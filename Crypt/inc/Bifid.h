#pragma once
#include "Cipher.h"
#include <map>
#include <fstream>
using namespace std;

struct Coords
{
	uchar x, y;
};
typedef pair<uchar, Coords> PolybiusSquarePair;
typedef map<uchar, Coords> PolybiusSquare;

// Encrypt
class BifidEncrypt: CipherEncrypt
{
	friend class BifidDecrypt;
protected:
	PolybiusSquare polybiusSquare;
	std::ifstream inputFile;
public:
	BifidEncrypt();
	~BifidEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();

	/**
	* Parse a square file into polybiusSquare.
	* @param string The file path.
	* @return bool Whether the file was valid.
	*/
	bool ParseSquareFile( string& a_squareFilePath );
};

// Decrypt
class BifidDecrypt : CipherDecrypt
{
private:
	BifidEncrypt encryption;
public:
	BifidDecrypt();
	~BifidDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};