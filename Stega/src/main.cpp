#include <Windows.h>
#include "Shlwapi.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "AllTypes.h"

using namespace std;

struct EncryptFunctionsPair
{
	string functionName;
	EncryptType*( *function )();
	bool(*validateFunction)(string&);

	// Constructor
	EncryptFunctionsPair( string a_functionName, EncryptType*( *a_function )(), bool(*a_validateFunction)(string&) )
	{
		functionName = a_functionName;
		function = a_function;
		validateFunction = a_validateFunction;
	}
};

struct DecryptFunctionsPair
{
	string functionName;
	DecryptType*( *function )();
	bool(*validateFunction)(string&);

	// Constructor
	DecryptFunctionsPair( string a_functionName, DecryptType*( *a_function )(), bool(*a_validateFunction)(string&) )
	{
		functionName = a_functionName;
		function = a_function;
		validateFunction = a_validateFunction;
	}
};

/**
* Strips the quotes from the beginning and of a string.
* @param string The original string.
* @return string The string without quotes.
*/
string StripQuotes( string& input )
{
	if ( *input.begin() == '"' && *input.rbegin() == '"' )
	{
		return input.substr( 1, input.length() - 2 );
	}
	return input;
}

/**
* Requests the user to input a path and check if it's valid, will only return when a valid path is input.
* @param string The question it should ask when asking for the string.
* @return string The valid path.
*/
string GetValidPath( string question )
{
	int validPath = 0;
	string inputPath;
	wstring w_inputPath;
	while ( !validPath )
	{
		printf( question.c_str() );
		getline( cin, inputPath );
		inputPath = StripQuotes( inputPath );
		w_inputPath = wstring( inputPath.begin(), inputPath.end() );
		validPath = PathFileExists( w_inputPath.c_str() );
		if ( validPath == 0 )
		{
			printf( "Invalid path.\n" );
		}
	}
	return inputPath;
}

/**
* Get the folder path from the file path.
* @param string The file path.
* @return string The folder path.
*/
string GetFolderFromPath(string filePath)
{
	uint index = filePath.find_last_of('\\');
	if (index == string::npos) return string();
	return string(filePath.begin(), filePath.begin() + index + 1);
}

/**
* Stringfy the amount of bytes.
* @param uint The amount of bytes.
* @return string The resulting string.
*/
string StringfyBytecount(uint bytes)
{
	string result;
	if (bytes > 1000000)
		result = std::to_string(bytes / 1000000) + string(" MB");
	else if (bytes > 1000)
		result = std::to_string(bytes / 1000) + string(" kB");
	else
		result = std::to_string(bytes) + string(" bytes");
	return result;
}

/**
* Gets the index from a string.
* @param string The input string.
* @return int The index, is -1 when invalid.
*/
int GetIndexFromString( string& input, int maxVal )
{
	if (input.size() == 0) return -1;
	int result = 0;
	for ( unsigned int i = 0; i < input.length(); i++ )
	{
		if ( input[i] < '0' || input[i] > '9' ) return -1;
	}
	string reverseInput( input.rbegin(), input.rend() );
	int j = 1;
	for ( unsigned int i = 0; i < reverseInput.length(); i++, j*=10 )
	{
		result += (reverseInput[i] - '0') * j;
	}
	if ( result > maxVal ) return -1;
	return result;
}

/**
* Requests the user to input an index and check if it's valid, will only return when a valid path is input.
* @param int The max index.
* @return int The valid index.
*/
int GetValidIndex( int maxVal )
{
	bool validIndex = false;
	string inputIndex;
	int index = 0;
	while ( !validIndex )
	{
		getline( cin, inputIndex );
		index = GetIndexFromString( inputIndex, maxVal );
		validIndex = ( index != -1 );
		if ( !validIndex )
		{
			printf( "Invalid index.\n" );
		}
	}
	return index;
}

/**
* Gets the extension from a string.
* @param string The input string.
* @return string The extension.
*/
string GetExtension( string& filePath )
{
	uint index = filePath.find_last_of('.');
	if (index == string::npos) return string();
	return string(filePath.begin() + index + 1, filePath.end());
}

#define EncryptMethodCount 4
EncryptFunctionsPair EncryptFunctions[EncryptMethodCount] {
	EncryptFunctionsPair("LSB", &LSB_ImageEncryptFactory, &LSB_ImageValidateFunction),
	EncryptFunctionsPair("LSB", &LSB_AudioEncryptFactory, &LSB_AudioValidateFunction),
	EncryptFunctionsPair("Datamosh", &DatamoshEncryptFactory, &DatamoshValidateFunction ),
	EncryptFunctionsPair("DCT", &DCTEncryptFactory, &DCTValidateFunction )
};

#define DecryptMethodCount 4
DecryptFunctionsPair DecryptFunctions[DecryptMethodCount]{
	DecryptFunctionsPair("LSB", &LSB_ImageDecryptFactory, &LSB_ImageValidateFunction),
	DecryptFunctionsPair("LSB", &LSB_AudioDecryptFactory, &LSB_AudioValidateFunction),
	DecryptFunctionsPair("Datamosh", &DatamoshDecryptFactory, &DatamoshValidateFunction),
	DecryptFunctionsPair("DCT", &DCTDecryptFactory, &DCTValidateFunction)
};

/**
* Main loop for an encryption method.
*/
void EncryptMode()
{
	string inputFilePath;
	string folderPath;
	string outputFileName;
	string encryptFilePath;

	inputFilePath = GetValidPath( "File path:\n" );
	folderPath = GetFolderFromPath(inputFilePath);
	printf( "Output file name:\n" );
	getline( cin, outputFileName );
	printf( "Pick an encryption type:\n" );
	string inputFileExtension = GetExtension(inputFilePath);
	for ( int i = 0; i < EncryptMethodCount; i++ )
	{
		if (!EncryptFunctions[i].validateFunction(inputFileExtension)) continue;
		char currentLine[256];
		sprintf_s( currentLine, "[%i] %s\n", i, EncryptFunctions[i].functionName.c_str() );
		printf( currentLine );
	}
	int encryptIndex = GetValidIndex( EncryptMethodCount - 1 );
	EncryptType* encryptType = EncryptFunctions[encryptIndex].function();
	uint maxFileSize = encryptType->Initialize( inputFilePath, folderPath + outputFileName );
	printf("Max encrypt file size: %s\n", StringfyBytecount(maxFileSize).c_str());
	uint fileSize = maxFileSize;
	FILE* encryptFile = NULL;
	while (fileSize >= maxFileSize)
	{
		encryptFilePath = GetValidPath("File to encrypt:\n");
		encryptFile = fopen(encryptFilePath.c_str(), "rb");
		fseek(encryptFile, 0, SEEK_END);
		fileSize = ftell(encryptFile);
		rewind(encryptFile);
		if (fileSize >= maxFileSize)
		{
			printf("File is too big.\n");
			fclose(encryptFile);
		}
	}
	encryptType->Encrypt(encryptFile, fileSize);
}

/**
* Main loop for a decryption method.
*/
void DecryptMode()
{
	string inputFilePath;
	string outputFileName;
	string folderPath;

	inputFilePath = GetValidPath("File path:\n");
	folderPath = GetFolderFromPath(inputFilePath);
	printf("Output file name:\n");
	getline(cin, outputFileName);
	printf("Pick an decryption type:\n");
	string inputFileExtension = GetExtension(inputFilePath);
	for (int i = 0; i < DecryptMethodCount; i++)
	{
		if (!DecryptFunctions[i].validateFunction(inputFileExtension)) continue;
		char currentLine[256];
		sprintf_s(currentLine, "[%i] %s\n", i, DecryptFunctions[i].functionName.c_str());
		printf(currentLine);
	}
	int decryptIndex = GetValidIndex(DecryptMethodCount - 1);
	DecryptType* decryptType = DecryptFunctions[decryptIndex].function();
	decryptType->Initialize(inputFilePath, folderPath + outputFileName);
	decryptType->Decrypt();
}

int main()
{
	string mode;
	bool modeSelected = false;

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xA );
	printf( "========[Steganography]========\n" );
	printf( "=  Author: Yoshivb            =\n" );
	printf( "=  Version: 0.1               =\n" );
	printf( "===============================\n" );
	printf( "\n\n\n" );
	while ( !modeSelected )
	{
		printf( "[E]ncrypt or [D]ecrypt?\n" );
		getline( cin, mode );
		if ( mode[0] == 'E' || mode[0] == 'e' )
		{
			modeSelected = true;
			EncryptMode();
		}
		else if ( mode[0] == 'D' || mode[0] == 'd' )
		{
			modeSelected = true;
			DecryptMode();
		}
		else
		{
			printf( "Invalid input.\n" );
		}
	}
	printf( "Press enter to exit.\n" );
	getchar();
	return 0;
};