#pragma once
#include "Type.h"
#include "ROT.h"

// ROT
EncryptType* ROTEncryptFactory()
{
	return (EncryptType*)(new ROTEncrypt());
}

bool ROTValidateFunction( std::string& extension )
{
	return true;
}

DecryptType* ROTDecryptFactory()
{
	return (DecryptType*)(new ROTDecrypt());
}