#pragma once
#include "Cipher.h"

// Encrypt
class ROTEncrypt: CipherEncrypt
{
	friend class ROTDecrypt;
protected:
	char modeIndex = -1;
	uint ROTNumber;
public:
	ROTEncrypt();
	~ROTEncrypt();

	// EncryptType interface
	virtual uint Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Encrypt();

	/**
	* Called when encrypting text.
	*/
	void TextEncrypt();
	/**
	* Called when encrypting binary.
	*/
	void ByteEncrypt();
};

// Decrypt
class ROTDecrypt : CipherDecrypt
{
private:
	ROTEncrypt encryption;
public:
	ROTDecrypt();
	~ROTDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};