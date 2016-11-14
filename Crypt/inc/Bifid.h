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

class BifidEncrypt: CipherEncrypt
{
	friend class BifidDecrypt;
protected:
	PolybiusSquare polybiusSquare;
	std::ifstream inputFile;
public:
	BifidEncrypt();
	~BifidEncrypt();

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();

	bool ParseSquareFile( string& a_squareFilePath );
};

class BifidDecrypt : CipherDecrypt
{
private:
	BifidEncrypt encryption;
public:
	BifidDecrypt();
	~BifidDecrypt();

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};