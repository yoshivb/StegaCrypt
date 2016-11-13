#pragma once
#include "Type.h"
#include "ROT.h"
#include "Bifid.h"

// ROT
CryptFactory(ROT)

bool ROTValidateFunction( std::string& extension )
{
	return true;
}

// Bifid
CryptFactory(Bifid)

bool BifidValidateFunction( std::string& extension )
{
	return true;
}