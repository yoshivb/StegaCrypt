#pragma once
#include <string>

// Useful typedef's.
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

// Creates an EncryptFactory function and a DecryptFactory function with the name as prefix.
#define CryptFactory(name) EncryptType* name##EncryptFactory()	\
{																\
	return (EncryptType*)(new  name##Encrypt());				\
}																\
																\
DecryptType* name##DecryptFactory()								\
{																\
	return (DecryptType*)(new  name##Decrypt());				\
}

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