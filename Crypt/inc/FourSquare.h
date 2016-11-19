#pragma once
#include "Cipher.h"
using namespace std;

// Encrypt
class FourSquareEncrypt: CipherEncrypt
{
	friend class FourSquareDecrypt;
protected:
	PolybiusSquare plaintextSquare;
	PolybiusSquare topCipherSquare;
	PolybiusSquare bottomCipherSquare;
	std::ifstream inputFile;

	/**
	* Removes all the duplicate letter from the keyword.
	* @param string* The keyword.
	*/
	void ParseKeyword( string& a_keyword );
public:
	FourSquareEncrypt();
	~FourSquareEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();
};

// Decrypt
class FourSquareDecrypt : CipherDecrypt
{
private:
	FourSquareEncrypt encryption;
public:
	FourSquareDecrypt();
	~FourSquareDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};