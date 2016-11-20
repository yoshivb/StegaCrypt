#pragma once

// Creates an EncryptFactory function and a DecryptFactory function with the name as prefix.
#define CryptFactory(name)										\
EncryptType* name##EncryptFactory()								\
{																\
	return (EncryptType*)(new name##Encrypt());					\
}																\
																\
DecryptType* name##DecryptFactory()								\
{																\
	return (DecryptType*)(new name##Decrypt());					\
}

// Create an ValidateFunction with the name as prefix.
#define CryptValidate(name)										\
bool name##ValidateFunction( std::string& extension )			\
{																\
	return true;												\
}

#define CryptValidate_1(name,ext)								\
bool name##ValidateFunction( std::string& extension )			\
{																\
	return extension == #ext;									\
}

#define CryptValidate_2(name, ext1, ext2)						\
bool name##ValidateFunction( std::string& extension )			\
{																\
	return (extension == #ext1 || extension == #ext2);			\
}

#define CryptValidate_3(name, ext1, ext2, ext3)					\
bool name##ValidateFunction( std::string& extension )			\
{																\
	return (extension == #ext1 || extension == #ext2);			\
}
