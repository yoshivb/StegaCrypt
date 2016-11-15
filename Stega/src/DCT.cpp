#include "DCT.h"
#include "Shlwapi.h"
#include <string.h>
#include <iostream>
using namespace std;

//Encrypt
DCTEncrypt::DCTEncrypt()
{
}

DCTEncrypt::~DCTEncrypt()
{
}

void DCTEncrypt::EncryptDCTS(j_decompress_ptr a_srcinfo, j_compress_ptr a_dstinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays, FILE* a_encryptFile, uint a_fileSize)
{
	size_t block_row_size;
	JBLOCKARRAY coef_buffers[MAX_COMPONENTS];
	JBLOCKARRAY row_ptrs[MAX_COMPONENTS];

	uchar* buffer = (uchar*)malloc(sizeof(uchar) * a_fileSize);
	unsigned int result = fread(buffer, 1, a_fileSize, a_encryptFile);

	//Allocate DCT array buffers
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		coef_buffers[compnum] = (a_dstinfo->mem->alloc_barray)((j_common_ptr)a_dstinfo, JPOOL_IMAGE, a_srcinfo->comp_info[compnum].width_in_blocks,
			a_srcinfo->comp_info[compnum].height_in_blocks);
	}

	uchar j = 8;
	uchar mask = 0xC0;
	uint l = 0;

	//For each component,
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		block_row_size = (size_t) sizeof(JCOEF) * DCTSIZE2 * a_srcinfo->comp_info[compnum].width_in_blocks;
		//...iterate over rows,
		for (JDIMENSION rownum = 0; rownum < a_srcinfo->comp_info[compnum].height_in_blocks; rownum++)
		{
			row_ptrs[compnum] = ((a_dstinfo)->mem->access_virt_barray)((j_common_ptr)&dstinfo, src_coef_arrays[compnum], rownum, (JDIMENSION)1, FALSE);
			//...and for each block in a row,
			for (JDIMENSION blocknum = 0; blocknum < a_srcinfo->comp_info[compnum].width_in_blocks; blocknum++)
				//...iterate over DCT coefficients
				for (JDIMENSION i = 0; i < DCTSIZE2; i++)
				{
					//Altering the DCT coefficients.
					short currentCoef = row_ptrs[compnum][0][blocknum][i];

					if (l < a_fileSize && (currentCoef > 5 || currentCoef < -5) )
					{
						ushort bitsMasked = (buffer[l] & mask) >> (j - 2);
						short sBitsMasked = 0;
						memcpy(&sBitsMasked, &bitsMasked, sizeof(short));
						short maskedCoef = ((currentCoef >> 2) << 2) | sBitsMasked;
						if (maskedCoef != currentCoef)
						{
							j -= 2, mask >>= 2;
							if (j == 0)
							{
								l++;
								printf("Progress: %i%%\r", (int)(((float)l / (float)a_fileSize) * 100.0f));
								j = 8;
								mask = 0xC0;
							}
						}
						currentCoef = maskedCoef;
					}

					coef_buffers[compnum][rownum][blocknum][i] = currentCoef;
				}
		}
	}
	printf("\n");

	//Save the changes
	//For each component,
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		block_row_size = (size_t) sizeof(JCOEF)*DCTSIZE2 * a_srcinfo->comp_info[compnum].width_in_blocks;
		//...iterate over rows
		for (JDIMENSION rownum = 0; rownum < a_srcinfo->comp_info[compnum].height_in_blocks; rownum++)
		{
			// Copy the whole rows.
			row_ptrs[compnum] = (a_dstinfo->mem->access_virt_barray)((j_common_ptr)a_dstinfo, src_coef_arrays[compnum], rownum, (JDIMENSION)1, TRUE);
			memcpy(row_ptrs[compnum][0][0], coef_buffers[compnum][rownum][0], block_row_size);
		}
	}
}

uint DCTEncrypt::Initialize( string a_inputFilePath, string a_outputFilePath )
{
	outputFilePath = a_outputFilePath;

	inputFile = fopen( a_inputFilePath.c_str(), "rb" );

	static jpeg_transform_info transformoption;
	transformoption.transform = JXFORM_NONE;
	transformoption.trim = FALSE;
	transformoption.force_grayscale = FALSE;

	// Initialize the JPEG decompression object with default error handling.
	srcinfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&srcinfo);

	// Initialize the JPEG compression object with default error handling.
	dstinfo.err = jpeg_std_error(&jdsterr);
	jpeg_create_compress(&dstinfo);

	// Specify data source for decompression.
	jpeg_stdio_src(&srcinfo, inputFile);

	// Enable saving of extra markers that we want to copy.
	jcopy_markers_setup(&srcinfo, JCOPYOPT_ALL);

	// Read file header.
	(void)jpeg_read_header(&srcinfo, TRUE);

	jtransform_request_workspace(&srcinfo, &transformoption);
	src_coef_arrays = jpeg_read_coefficients(&srcinfo);
	jpeg_copy_critical_parameters(&srcinfo, &dstinfo);

	uint fileSize = 0;
	for (JDIMENSION compnum = 0; compnum < (uint)srcinfo.num_components; compnum++)
	{
		fileSize += (srcinfo.comp_info[compnum].height_in_blocks * srcinfo.comp_info[compnum].width_in_blocks * DCTSIZE2)/8;
	}
	return fileSize;
}

void DCTEncrypt::Encrypt( FILE* a_encryptFile, uint a_fileSize )
{
	static jpeg_transform_info transformoption;
	transformoption.transform = JXFORM_NONE;
	transformoption.trim = FALSE;
	transformoption.force_grayscale = FALSE;

	jtransform_request_workspace(&srcinfo, &transformoption);

	EncryptDCTS(&srcinfo, &dstinfo, 0, src_coef_arrays, a_encryptFile, a_fileSize);

	dst_coef_arrays = jtransform_adjust_parameters(&srcinfo, &dstinfo, src_coef_arrays, &transformoption);
	fclose(inputFile);

	//And write everything back
	FILE* fp = fopen(outputFilePath.c_str(), "wb");

	/* Specify data destination for compression */
	jpeg_stdio_dest(&dstinfo, fp);

	/* Start compressor (note no image data is actually written here) */
	jpeg_write_coefficients(&dstinfo, dst_coef_arrays);

	/* Copy to the output file any extra markers that we want to preserve */
	jcopy_markers_execute(&srcinfo, &dstinfo, JCOPYOPT_ALL);

	jpeg_finish_compress(&dstinfo);
	jpeg_destroy_compress(&dstinfo);
	(void)jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);

	fclose(fp);
}

//Decrypt
DCTDecrypt::DCTDecrypt()
{
}

DCTDecrypt::~DCTDecrypt()
{
}

void DCTDecrypt::LoadEncryptedDCTS( j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays)
{
	size_t block_row_size;
	JBLOCKARRAY row_ptrs[MAX_COMPONENTS];

	//Allocate DCT array buffers
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		inputCoefBuffers[compnum] = (a_srcinfo->mem->alloc_barray)((j_common_ptr)a_srcinfo, JPOOL_IMAGE, a_srcinfo->comp_info[compnum].width_in_blocks,
			a_srcinfo->comp_info[compnum].height_in_blocks);
	}

	//For each component,
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		block_row_size = (size_t) sizeof(JCOEF) * DCTSIZE2 * a_srcinfo->comp_info[compnum].width_in_blocks;
		//...iterate over rows,
		for (JDIMENSION rownum = 0; rownum < a_srcinfo->comp_info[compnum].height_in_blocks; rownum++)
		{
			row_ptrs[compnum] = ((a_srcinfo)->mem->access_virt_barray)((j_common_ptr)&a_srcinfo, srcCoefArrays[compnum], rownum, (JDIMENSION)1, FALSE);
			//...and for each block in a row,
			for (JDIMENSION blocknum = 0; blocknum < a_srcinfo->comp_info[compnum].width_in_blocks; blocknum++)
				//...iterate over DCT coefficients
				for (JDIMENSION i = 0; i < DCTSIZE2; i++)
				{
					inputCoefBuffers[compnum][rownum][blocknum][i] = row_ptrs[compnum][0][blocknum][i];
				}
		}
	}
}

void DCTDecrypt::Initialize(string a_inputFilePath, string a_outputFilePath)
{
	outputFilePath = a_outputFilePath;

	inputFile = fopen(a_inputFilePath.c_str(), "rb");

	static jpeg_transform_info transformoption;
	transformoption.transform = JXFORM_NONE;
	transformoption.trim = FALSE;
	transformoption.force_grayscale = FALSE;

	/* Initialize the JPEG decompression object with default error handling. */
	srcinfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&srcinfo);

	/* Specify data source for decompression */
	jpeg_stdio_src(&srcinfo, inputFile);

	/* Enable saving of extra markers that we want to copy */
	jcopy_markers_setup(&srcinfo, JCOPYOPT_ALL);

	/* Read file header */
	(void)jpeg_read_header(&srcinfo, TRUE);

	jtransform_request_workspace(&srcinfo, &transformoption);
	srcCoefArrays = jpeg_read_coefficients(&srcinfo);

	LoadEncryptedDCTS(&srcinfo, 0, srcCoefArrays);
}

void DCTDecrypt::DecryptDCTS(j_decompress_ptr a_srcinfo, JDIMENSION a_x_crop_offset, jvirt_barray_ptr* a_src_coef_arrays)
{
	size_t block_row_size;
	JBLOCKARRAY row_ptrs[MAX_COMPONENTS];

	uchar currentChar = 0;
	uchar mask = 3;
	uchar j = 8;

	//For each component,
	for (JDIMENSION compnum = 0; compnum < (uint)a_srcinfo->num_components; compnum++)
	{
		block_row_size = (size_t) sizeof(JCOEF) * DCTSIZE2 * a_srcinfo->comp_info[compnum].width_in_blocks;
		//...iterate over rows,
		for (JDIMENSION rownum = 0; rownum < a_srcinfo->comp_info[compnum].height_in_blocks; rownum++)
		{
			row_ptrs[compnum] = ((a_srcinfo)->mem->access_virt_barray)((j_common_ptr)&a_srcinfo, origCoefArrays[compnum], rownum, (JDIMENSION)1, FALSE);
			//...and for each block in a row,
			for (JDIMENSION blocknum = 0; blocknum < a_srcinfo->comp_info[compnum].width_in_blocks; blocknum++)
				//...iterate over DCT coefficients
				for (JDIMENSION i = 0; i < DCTSIZE2; i++)
				{
					short origCoeff = row_ptrs[compnum][0][blocknum][i];
					short encryptedCoeff = inputCoefBuffers[compnum][rownum][blocknum][i];
					if (origCoeff != encryptedCoeff)
					{
						uchar encryptedByte;
						memcpy(&encryptedByte, &encryptedCoeff, 1);
						uchar maskedBits = encryptedByte & mask;
						currentChar += (maskedBits << (j - 2));
						j -= 2;
						if (j == 0)
						{
							buffer.push_back(currentChar);
							currentChar = 0;
							j = 8;
						}
					}
				}
		}
	}
}

void DCTDecrypt::Decrypt()
{
	string originalFilePath;
	{
		int validPath = 0;
		string inputPath;
		wstring w_inputPath;
		while (!validPath)
		{
			printf("Original file path:\n");
			getline(cin, inputPath);
			if (*inputPath.begin() == '"' && *inputPath.rbegin() == '"')
			{
				inputPath = inputPath.substr(1, inputPath.length() - 2);
			}
			w_inputPath = wstring(inputPath.begin(), inputPath.end());
			validPath = PathFileExists(w_inputPath.c_str());
			if ( validPath == 0 )
			{
				printf("Invalid path.\n");
			}
		}
		originalFilePath = inputPath;
	}

	FILE* originalFileHandle = fopen(originalFilePath.c_str(), "rb");

	static jpeg_transform_info transformoption;
	transformoption.transform = JXFORM_NONE;
	transformoption.trim = FALSE;
	transformoption.force_grayscale = FALSE;

	/* Initialize the JPEG decompression object with default error handling. */
	origInfo.err = jpeg_std_error(&jsrcerr);
	jpeg_create_decompress(&origInfo);

	/* Specify data source for decompression */
	jpeg_stdio_src(&origInfo, originalFileHandle);

	/* Enable saving of extra markers that we want to copy */
	jcopy_markers_setup(&origInfo, JCOPYOPT_ALL);

	/* Read file header */
	(void)jpeg_read_header(&origInfo, TRUE);

	jtransform_request_workspace(&origInfo, &transformoption);
	origCoefArrays = jpeg_read_coefficients(&origInfo);

	DecryptDCTS(&origInfo, 0, origCoefArrays);

	printf("Progress: 100%%\r");
	printf("\n");

	FILE* outputFile = fopen(outputFilePath.c_str(), "wb");
	fwrite(&buffer[0], sizeof(uchar), buffer.size(), outputFile);
	fclose(outputFile);

	(void)jpeg_finish_decompress(&origInfo);
	jpeg_destroy_decompress(&origInfo);
	(void)jpeg_finish_decompress(&srcinfo);
	jpeg_destroy_decompress(&srcinfo);
}