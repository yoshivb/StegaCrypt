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

class DCTEncrypt: EncryptType
{
private:
	std::string outputFilePath;
	FILE* inputFile;

	struct jpeg_decompress_struct srcinfo;
	struct jpeg_compress_struct dstinfo;
	struct jpeg_error_mgr jsrcerr, jdsterr;
	jvirt_barray_ptr* src_coef_arrays;
	jvirt_barray_ptr* dst_coef_arrays;

	void EncryptDCTS(j_decompress_ptr a_srcinfo, j_compress_ptr a_dstinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays, FILE* a_encryptFile, uint a_fileSize);
public:
	DCTEncrypt();
	~DCTEncrypt();

	virtual uint Initialize( std::string a_inputFilePath, std::string a_outputFilePath );
	virtual void Encrypt(FILE* a_encryptFile, uint a_fileSize);
};

class DCTDecrypt : DecryptType
{
private:
	std::string outputFilePath;
	FILE* inputFile;

	struct jpeg_decompress_struct srcinfo, origInfo;
	struct jpeg_error_mgr jsrcerr;
	jvirt_barray_ptr* srcCoefArrays,* origCoefArrays;
	JBLOCKARRAY inputCoefBuffers[MAX_COMPONENTS];
	std::vector<uchar> buffer;

	void LoadEncryptedDCTS(j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays);
	void DecryptDCTS(j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays);
public:
	DCTDecrypt();
	~DCTDecrypt();

	virtual void Initialize(std::string a_inputFilePath, std::string a_outputFilePath);
	virtual void Decrypt();
};