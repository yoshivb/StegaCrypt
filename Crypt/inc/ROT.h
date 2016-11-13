#pragma once
#include "Cipher.h"

class ROTEncrypt: CipherEncrypt
{
public:
	char modeIndex = -1;
	uint ROTNumber;

	ROTEncrypt();
	~ROTEncrypt();

	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();

	void TextEncrypt();
	void ByteEncrypt();
};

class ROTDecrypt : CipherDecrypt
{
private:
	ROTEncrypt encryption;
public:
	ROTDecrypt();
	~ROTDecrypt();

	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};