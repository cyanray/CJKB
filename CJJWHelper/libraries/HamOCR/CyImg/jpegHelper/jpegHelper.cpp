#include "jpegHelper.h"
#include <exception>
#include <cstring>
using std::runtime_error;
void jpegHelper::ReadFromFile(
	const char * filename,
	unsigned char **img_buff,
	unsigned int &output_width,
	unsigned int &output_height,
	unsigned int &output_components)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride;
	if ((infile = fopen(filename, "rb")) == NULL)
	{
		throw runtime_error("Can't open file.");
	}
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		throw runtime_error("An error occurrence.");
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	(void)jpeg_read_header(&cinfo, TRUE);
	(void)jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;

	output_width = cinfo.output_width;
	output_height = cinfo.output_height;
	output_components = cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	*img_buff = new unsigned char[cinfo.output_width * cinfo.output_height * cinfo.output_components]{ 0 };
	unsigned char *p = *img_buff;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void)jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(p, *buffer, row_stride);
		p += row_stride;
	}
	(void)jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
}

void jpegHelper::ReadFromBytes(
	const unsigned char bytes[],
	const unsigned int length, 
	unsigned char ** img_buff,
	unsigned int & output_width, 
	unsigned int & output_height, 
	unsigned int & output_components)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;
	int row_stride;
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		throw runtime_error("An error occurrence.");
	}
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, bytes, length);
	(void)jpeg_read_header(&cinfo, TRUE);
	(void)jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;

	output_width = cinfo.output_width;
	output_height = cinfo.output_height;
	output_components = cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	*img_buff = new unsigned char[cinfo.output_width * cinfo.output_height * cinfo.output_components]{ 0 };
	unsigned char *p = *img_buff;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void)jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(p, *buffer, row_stride);
		p += row_stride;
	}
	(void)jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

void jpegHelper::SaveTo(
	const char * filename, 
	const unsigned char * img_buff,
	const unsigned int width,
	const unsigned int height,
	const unsigned int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	if ((outfile = fopen(filename, "wb")) == NULL)
	{
		throw runtime_error("Can't open file.");
	}
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = width; 	/* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);
	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */

	unsigned char *imgbuf = const_cast<unsigned char *>(img_buff);
	while (cinfo.next_scanline < cinfo.image_height) {
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could pass
		 * more than one scanline at a time if that's more convenient.
		 */
		row_pointer[0] = &imgbuf[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);

}
