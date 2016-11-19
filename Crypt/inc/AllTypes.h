#pragma once
#include "Type.h"

// Function structs.
struct EncryptFunctionsPair
{
	std::string functionName;
	EncryptType*( *function )();
	bool(*validateFunction)(std::string&);

	// Constructor
	EncryptFunctionsPair( std::string a_functionName, EncryptType*( *a_function )(), bool(*a_validateFunction)(std::string&) )
	{
		functionName = a_functionName;
		function = a_function;
		validateFunction = a_validateFunction;
	}
};

struct DecryptFunctionsPair
{
	std::string functionName;
	DecryptType*( *function )();
	bool(*validateFunction)(std::string&);

	// Constructor
	DecryptFunctionsPair( std::string a_functionName, DecryptType*( *a_function )(), bool(*a_validateFunction)(std::string&) )
	{
		functionName = a_functionName;
		function = a_function;
		validateFunction = a_validateFunction;
	}
};


// ROT
#include "ROT.h"
CryptFactory(ROT)

CryptValidate(ROT)

// Bifid
#include "Bifid.h"
CryptFactory(Bifid)

CryptValidate(Bifid)

// Four Square
#include "FourSquare.h"
CryptFactory(FourSquare)

CryptValidate_1(FourSquare, txt)


// Functions
#define EncryptMethodCount 3
EncryptFunctionsPair EncryptFunctions[EncryptMethodCount] {
	EncryptFunctionsPair("ROT", &ROTEncryptFactory, &ROTValidateFunction),
	EncryptFunctionsPair("Bifid", &BifidEncryptFactory, &BifidValidateFunction),
	EncryptFunctionsPair("FourSquare", &FourSquareEncryptFactory, &FourSquareValidateFunction)
};

#define DecryptMethodCount 3
DecryptFunctionsPair DecryptFunctions[DecryptMethodCount]{
	DecryptFunctionsPair("ROT", &ROTDecryptFactory, &ROTValidateFunction),
	DecryptFunctionsPair("Bifid", &BifidDecryptFactory, &BifidValidateFunction),
	DecryptFunctionsPair("FourSquare", &FourSquareDecryptFactory, &FourSquareValidateFunction)
};