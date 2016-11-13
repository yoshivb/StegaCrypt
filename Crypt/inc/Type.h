#pragma once
#include <string>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#define CryptFactory(name) EncryptType* name##EncryptFactory() \
{ \
	return (EncryptType*)(new  name##Encrypt()); \
} \
 \
DecryptType* name##DecryptFactory() \
{ \
	return (DecryptType*)(new  name##Decrypt()); \
}

class EncryptType
{
public:
	virtual uint Initialize( std::string a_inputFilePath, std::string a_outputFilePath ) = 0;
	virtual void Encrypt() = 0;
};

class DecryptType
{
public:
	virtual void Initialize( std::string inputFilePath, std::string a_outputFilePath ) = 0;
	virtual void Decrypt() = 0;
};