#pragma once
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
class jpegHelper
{
public:
	jpegHelper() = default;
	static void ReadFromFile(
		const char * filename,
		unsigned char ** img_buff,//output_img_bytes
		unsigned int & output_width,
		unsigned int & output_height,
		unsigned int & output_components);
	static void ReadFromBytes(
		const unsigned char bytes[],//input_bytes
		const unsigned int length,//input_bytes_length
		unsigned char ** img_buff,//output_img_bytes
		unsigned int & output_width,
		unsigned int & output_height,
		unsigned int & output_components);
	static void SaveTo(
		const char * filename,
		const unsigned char *img_buff,
		const unsigned int width,
		const unsigned int height,
		const unsigned int quality = 50
	);
	~jpegHelper() = default;
private:
	struct my_error_mgr {
		struct jpeg_error_mgr pub;

		jmp_buf setjmp_buffer;
	};

	typedef struct my_error_mgr * my_error_ptr;

	METHODDEF(void) my_error_exit(j_common_ptr cinfo)
	{
		/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
		my_error_ptr myerr = (my_error_ptr)cinfo->err;

		/* Always display the message. */
		/* We could postpone this until after returning, if we chose. */
		(*cinfo->err->output_message) (cinfo);

		/* Return control to the setjmp point */
		longjmp(myerr->setjmp_buffer, 1);
	}
};

