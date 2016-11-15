#pragma once
#include "DCT.h"
#include "Datamosh.h"
#include "LSB_Image.h"
#include "LSB_Audio.h"

// DCT
CryptFactory(DCT)

bool DCTValidateFunction( std::string& extension )
{
	if (extension == "jpeg" || extension == "jpg")
	{
		return true;
	}
	return false;
}

// Datamosh
CryptFactory(Datamosh)

bool DatamoshValidateFunction(std::string& extension)
{
	if (extension == "jpeg" || extension == "jpg")
	{
		return true;
	}
	return false;
}

// LSB Image
CryptFactory(LSB_Image)

bool LSB_ImageValidateFunction(std::string& extension)
{
	if (extension == "png")
	{
		return true;
	}
	return false;
}

// LSB Audio
CryptFactory(LSB_Audio)

bool LSB_AudioValidateFunction(std::string& extension)
{
	if (extension == "wav")
	{
		return true;
	}
	return false;
}