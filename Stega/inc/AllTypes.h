#pragma once
#include "DCT.h"
#include "Datamosh.h"
#include "LSB_Image.h"
#include "LSB_Audio.h"

// DCT
EncryptType* DCTEncryptFactory()
{
	return (EncryptType*)(new DCTEncrypt());
}

bool DCTValidateFunction( std::string& extension )
{
	if (extension == "jpeg" || extension == "jpg")
	{
		return true;
	}
	return false;
}

DecryptType* DCTDecryptFactory()
{
	return (DecryptType*)(new DCTDecrypt());
}

// Datamosh
EncryptType* DatamoshEncryptFactory()
{
	return (EncryptType*)(new DatamoshEncrypt());
}

bool DatamoshValidateFunction(std::string& extension)
{
	if (extension == "jpeg" || extension == "jpg")
	{
		return true;
	}
	return false;
}

DecryptType* DatamoshDecryptFactory()
{
	return (DecryptType*)(new DatamoshDecrypt());
}

// LSB Image
EncryptType* LSB_ImageEncryptFactory()
{
	return (EncryptType*)(new LSB_ImageEncrypt());
}

bool LSB_ImageValidateFunction(std::string& extension)
{
	if (extension == "png")
	{
		return true;
	}
	return false;
}

DecryptType* LSB_ImageDecryptFactory()
{
	return (DecryptType*)(new LSB_ImageDecrypt());
}

// LSB Audio
EncryptType* LSB_AudioEncryptFactory()
{
	return (EncryptType*)(new LSB_AudioEncrypt());
}

bool LSB_AudioValidateFunction(std::string& extension)
{
	if (extension == "wav")
	{
		return true;
	}
	return false;
}

DecryptType* LSB_AudioDecryptFactory()
{
	return (DecryptType*)(new LSB_AudioDecrypt());
}