#pragma once
#include "Type.h"
extern "C" {
#define XMD_H 1
#include <jpeglib.h>    
#include <jerror.h>
#include <transupp.h>
}
#include <vector>
#include <map>

// Encrypt
class DCTEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	FILE* inputFile;

	// JPEGLib variables
	struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	struct jpeg_error_mgr jsrcerr, jdsterr;
	jvirt_barray_ptr* src_coef_arrays;
	jvirt_barray_ptr* dst_coef_arrays;

	/**
	* Encrypts the file in the DCTS of the image.
	* @param j_decompress_ptr The pointer to the info of the source.
	* @param j_compress_ptr The pointer to the info of the destination.
	* @param JDIMENSION The width of the image.
	* @param jvirt_barray_ptr* 2D array of coefficients of the source.
	* @param FILE* The file to encrypt in the DCTS.
	* @param uint The size of the file to encrypt.
	*/
	void EncryptDCTS(j_decompress_ptr a_srcinfo, j_compress_ptr a_dstinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays, FILE* a_encryptFile, uint a_fileSize);
public:
	DCTEncrypt();
	~DCTEncrypt();

	// EncryptType interface
	virtual uint Initialize( std::string a_inputFilePath, std::string a_outputFilePath );
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

// Decrypt
class DCTDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	FILE* inputFile;

	// JPEGLib variables
	struct jpeg_decompress_struct srcinfo, origInfo;
	struct jpeg_error_mgr jsrcerr;
	jvirt_barray_ptr* srcCoefArrays,* origCoefArrays;
	JBLOCKARRAY inputCoefBuffers[MAX_COMPONENTS];

	std::vector<uchar> buffer;

	/**
	* Load the encrypted DCTS of the image.
	* @param j_decompress_ptr The pointer to the info of the source.
	* @param JDIMENSION The width of the image.
	* @param jvirt_barray_ptr* 2D array of coefficients of the source.
	*/
	void LoadEncryptedDCTS(j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays);
	/**
	* Decrypt the encrypted DCTS of the image.
	* @param j_decompress_ptr The pointer to the info of the source.
	* @param JDIMENSION The width of the image.
	* @param jvirt_barray_ptr* 2D array of coefficients of the source.
	*/
	void DecryptDCTS(j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays);
public:
	DCTDecrypt();
	~DCTDecrypt();

	// DecryptType interface
	virtual void Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};