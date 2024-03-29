#pragma warning(disable:4996)

#include <aRibeiroCore/common.h> // fix basetsd.h 

#include "JPGHelper.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <jpeglib.h>
#include <vector>

#include <setjmp.h>

#include <aRibeiroCore/StringUtil.h>

namespace aRibeiro {
    
    
    
    /*
     * ERROR HANDLING:
     *
     * The JPEG library's standard error handler (jerror.c) is divided into
     * several "methods" which you can override individually.  This lets you
     * adjust the behavior without duplicating a lot of code, which you might
     * have to update with each future release.
     *
     * Our example here shows how to override the "error_exit" method so that
     * control is returned to the library's caller when a fatal error occurs,
     * rather than calling exit() as the standard error_exit method does.
     *
     * We use C's setjmp/longjmp facility to return control.  This means that the
     * routine which calls the JPEG library must first execute a setjmp() call to
     * establish the return point.  We want the replacement error_exit to do a
     * longjmp().  But we need to make the setjmp buffer accessible to the
     * error_exit routine.  To do this, we make a private extension of the
     * standard JPEG error handler object.  (If we were using C++, we'd say we
     * were making a subclass of the regular error handler.)
     *
     * Here's the extended error handler struct:
     */
    
    struct my_error_mgr {
        struct jpeg_error_mgr pub;    /* "public" fields */
        
        jmp_buf setjmp_buffer;    /* for return to caller */
    };
    
    typedef struct my_error_mgr * my_error_ptr;
    
    
    METHODDEF(void)
    my_error_exit (j_common_ptr cinfo)
    {
        /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
        my_error_ptr myerr = (my_error_ptr) cinfo->err;
        
        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        (*cinfo->err->output_message) (cinfo);
        
        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }
    
    char* JPGHelper::readJPG(const char *filename, int *w, int *h, int *chann, int *pixel_depth, bool invertY, float *gamma) {
        
        char* result = NULL;
        
        /* This struct contains the JPEG decompression parameters and pointers to
         * working space (which is allocated as needed by the JPEG library).
         */
        struct jpeg_decompress_struct cinfo;
        /* We use our private extension JPEG error handler.
         * Note that this struct must live as long as the main JPEG parameter
         * struct, to avoid dangling-pointer problems.
         */
        struct my_error_mgr jerr;
        /* More stuff */
        FILE * infile;        /* source file */
        JSAMPARRAY buffer;        /* Output row buffer */
        int row_stride;        /* physical row width in output buffer */
        
        /* In this example we want to open the input file before doing anything else,
         * so that the setjmp() error recovery below can assume the file is open.
         * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
         * requires it in order to read binary files.
         */
        
        if ((infile = fopen(filename, "rb")) == NULL) {
            fprintf(stderr, "can't open %s\n", filename);
            return 0;
        }
        
        /* Step 1: allocate and initialize JPEG decompression object */
        
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            
            if (result != NULL)
                free_aligned(result);
                //delete[] result;
            
            return NULL;
        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
        
        /* Step 2: specify data source (eg, a file) */
        
        jpeg_stdio_src(&cinfo, infile);
        
        /* Step 3: read file parameters with jpeg_read_header() */
        
        (void) jpeg_read_header(&cinfo, TRUE);
        /* We can ignore the return value from jpeg_read_header since
         *   (a) suspension is not possible with the stdio data source, and
         *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
         * See libjpeg.txt for more info.
         */
        
        /* Step 4: set parameters for decompression */
        
        /* In this example, we don't need to change any of the defaults set by
         * jpeg_read_header(), so we do nothing here.
         */
        
        /* Step 5: Start decompressor */
        
        (void) jpeg_start_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */
        
        /* We may need to do some setup of our own at this point before reading
         * the data.  After jpeg_start_decompress() we have the correct scaled
         * output image dimensions available, as well as the output colormap
         * if we asked for color quantization.
         * In this example, we need to make an output work buffer of the right size.
         */
        /* JSAMPLEs per row in output buffer */
        row_stride = cinfo.output_width * cinfo.output_components;
        /* Make a one-row-high sample array that will go away when done with image */
        buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
        
        
        //result = new char[cinfo.output_width * cinfo.output_height * cinfo.output_components];
        result = (char*)malloc_aligned( cinfo.output_width * cinfo.output_height * cinfo.output_components );
        *w = cinfo.output_width;
        *h = cinfo.output_height;
        *chann = cinfo.output_components;
        *pixel_depth = 8;
        if (gamma != NULL)
            *gamma = (float)cinfo.output_gamma;
        
        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */
        
        /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while (cinfo.output_scanline < cinfo.output_height) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            //put_scanline_someplace(buffer[0], row_stride);
            if (invertY)
                memcpy(&result[(cinfo.output_height - 1 - (cinfo.output_scanline-1) )*row_stride],buffer[0],row_stride);
            else
                memcpy(&result[(cinfo.output_scanline-1)*row_stride],buffer[0],row_stride);
        }
        
        /* Step 7: Finish decompression */
        
        (void) jpeg_finish_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */
        
        /* Step 8: Release JPEG decompression object */
        
        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_decompress(&cinfo);
        
        /* After finish_decompress, we can close the input file.
         * Here we postpone it until after no more JPEG errors are possible,
         * so as to simplify the setjmp error logic above.  (Actually, I don't
         * think that jpeg_destroy can do an error exit, but why assume anything...)
         */
        fclose(infile);
        
        /* At this point you may want to check to see whether any corrupt-data
         * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
         */
        
        /* And we're done! */
        
        return result;
    }
    
    char* JPGHelper::readJPGFromMemory(const char *input_buffer, int input_buffer_size, int *w, int *h, int *chann, int *pixel_depth, bool invertY , float *gamma) {
        
        char* result = NULL;
        
        /* This struct contains the JPEG decompression parameters and pointers to
         * working space (which is allocated as needed by the JPEG library).
         */
        struct jpeg_decompress_struct cinfo;
        /* We use our private extension JPEG error handler.
         * Note that this struct must live as long as the main JPEG parameter
         * struct, to avoid dangling-pointer problems.
         */
        struct my_error_mgr jerr;
        /* More stuff */
        JSAMPARRAY buffer;        /* Output row buffer */
        int row_stride;        /* physical row width in output buffer */
        
        /* In this example we want to open the input file before doing anything else,
         * so that the setjmp() error recovery below can assume the file is open.
         * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
         * requires it in order to read binary files.
         */
        
        //if ((infile = fopen(filename, "rb")) == NULL) {
            //fprintf(stderr, "can't open %s\n", filename);
            //return 0;
        //}
        
        /* Step 1: allocate and initialize JPEG decompression object */
        
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            //fclose(infile);
            
            if (result != NULL)
                free_aligned(result);
                //delete[] result;
            
            return NULL;
        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
        
        /* Step 2: specify data source (eg, a file) */
        jpeg_mem_src(&cinfo, (unsigned char*)input_buffer, input_buffer_size);
        
        /* Step 3: read file parameters with jpeg_read_header() */
        
        (void) jpeg_read_header(&cinfo, TRUE);
        /* We can ignore the return value from jpeg_read_header since
         *   (a) suspension is not possible with the stdio data source, and
         *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
         * See libjpeg.txt for more info.
         */
        
        /* Step 4: set parameters for decompression */
        
        /* In this example, we don't need to change any of the defaults set by
         * jpeg_read_header(), so we do nothing here.
         */
        
        /* Step 5: Start decompressor */
        
        (void) jpeg_start_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */
        
        /* We may need to do some setup of our own at this point before reading
         * the data.  After jpeg_start_decompress() we have the correct scaled
         * output image dimensions available, as well as the output colormap
         * if we asked for color quantization.
         * In this example, we need to make an output work buffer of the right size.
         */
        /* JSAMPLEs per row in output buffer */
        row_stride = cinfo.output_width * cinfo.output_components;
        /* Make a one-row-high sample array that will go away when done with image */
        buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
        
        
        //result = new char[cinfo.output_width * cinfo.output_height * cinfo.output_components];
        result = (char*)malloc_aligned(cinfo.output_width * cinfo.output_height * cinfo.output_components);
        *w = cinfo.output_width;
        *h = cinfo.output_height;
        *chann = cinfo.output_components;
        *pixel_depth = 8;
        if (gamma != NULL)
            *gamma = (float)cinfo.output_gamma;
        
        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */
        
        /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while (cinfo.output_scanline < cinfo.output_height) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            //put_scanline_someplace(buffer[0], row_stride);
            if (invertY)
                memcpy(&result[(cinfo.output_height - 1 - (cinfo.output_scanline-1))*row_stride],buffer[0],row_stride);
            else
                memcpy(&result[(cinfo.output_scanline-1)*row_stride],buffer[0],row_stride);
        }
        
        /* Step 7: Finish decompression */
        
        (void) jpeg_finish_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */
        
        /* Step 8: Release JPEG decompression object */
        
        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_decompress(&cinfo);
        
        /* After finish_decompress, we can close the input file.
         * Here we postpone it until after no more JPEG errors are possible,
         * so as to simplify the setjmp error logic above.  (Actually, I don't
         * think that jpeg_destroy can do an error exit, but why assume anything...)
         */
        //fclose(infile);
        
        /* At this point you may want to check to see whether any corrupt-data
         * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
         */
        
        /* And we're done! */
        
        return result;
    }

    void JPGHelper::closeJPG(char *&buff) {
        if (!buff)
            return;
        free_aligned(buff);
        //delete[]buff;
        buff = NULL;
    }
    //----------------------------------------------------------------------------------
    
    bool JPGHelper::isJPGFilename(const char* filename) {
        std::string file_lower = aRibeiro::StringUtil::toLower(filename);
        return aRibeiro::StringUtil::endsWith(file_lower, ".jpg") ||
                aRibeiro::StringUtil::endsWith(file_lower, ".jpeg") ||
                aRibeiro::StringUtil::endsWith(file_lower, ".jfif");
    }
    
}

