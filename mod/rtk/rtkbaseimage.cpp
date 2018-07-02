#include <gongdebug.h>
#include <rtkconfig.h>
#include "rtkbaseimage.h"

#ifdef RTK_JPEG_SUPPORT

#ifndef RTK_PNG_SUPPORT
#include "setjmp.h"
#endif // RTK_PNG_SUPPORT

#include <cstdio>
extern "C" {
#include <jpeglib.h>
}

#endif // RTK_JPEG_SUPPORT

#ifdef RTK_PNG_SUPPORT
#include <png.h>

#include <algorithm>
#include <cmath>

using namespace std;


// Define png_jmpbuf() in case we are using a pre-1.0.6 version of libpng
#ifndef png_jmpbuf
#define png_jmpbuf(png_ptr) png_ptr->jmpbuf
#endif // png_jmpbuf

// Define various expansion transformations for old versions of libpng
#if PNG_LIBPNG_VER < 10004
#define png_set_palette_to_rgb(p)  png_set_expand(p)
#define png_set_gray_1_2_4_to_8(p) png_set_expand(p)
#define png_set_tRNS_to_alpha(p)   png_set_expand(p)
#endif // PNG_LIBPNG_VER < 10004

#endif // RTK_PNG_SUPPORT

using namespace std;

namespace gong {}
namespace RTK = gong;

namespace gong {

bool BaseImage::read(const string& filename)
{
    std::string:: size_type extPos = filename.rfind('.');
    if (extPos == std::string::npos)
        return false;
    string ext = string(filename, extPos + 1, filename.length() - extPos - 1 );

    _GONG_DEBUG_PRINT(10, "Loading image from file " + filename + ", type " + ext);

    if( ext == "jpeg" || ext == "JPEG" || ext == "JPG" || ext == "jpg" )
        return readJPEG(filename);
    else if( ext == "bmp" || ext == "BMP" )
        return readBMP(filename);
    else if( ext == "png" || ext == "PNG" )
        return readPNG(filename);

    _GONG_DEBUG_WARNING( filename + ": unrecognized or unsupported image file type");
    return false;
}


#ifdef RTK_JPEG_SUPPORT

struct my_error_mgr
{
    struct jpeg_error_mgr pub;	// "public" fields
    jmp_buf setjmp_buffer;      // for return to caller
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    // cinfo->err really points to a my_error_mgr struct, so coerce pointer
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    // Always display the message.
    // We could postpone this until after returning, if we chose.
    (*cinfo->err->output_message) (cinfo);

    // Return control to the setjmp point
    longjmp(myerr->setjmp_buffer, 1);
}
#endif // RTK_JPEG_SUPPORT


bool BaseImage::readJPEG(const string& filename, int )
{
#ifdef RTK_JPEG_SUPPORT

    // This struct contains the JPEG decompression parameters and pointers to
    // working space (which is allocated as needed by the JPEG library).
    struct jpeg_decompress_struct cinfo;

    // We use our private extension JPEG error handler.
    // Note that this struct must live as long as the main JPEG parameter
    // struct, to avoid dangling-pointer problems.
    struct my_error_mgr jerr;
    // More stuff
    JSAMPARRAY buffer;		// Output row buffer
    int row_stride;		// physical row width in output buffer
    long cont;

    // In this example we want to open the input file before doing anything else,
    // so that the setjmp() error recovery below can assume the file is open.
    // VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    // requires it in order to read binary files.

    FILE *in;
    in = fopen(filename.c_str(), "rb");
    if (in == NULL)
        return false;

    // Step 1: allocate and initialize JPEG decompression object
    // We set up the normal JPEG error routines, then override error_exit.
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    // Establish the setjmp return context for my_error_exit to use.
    if (setjmp(jerr.setjmp_buffer))
    {
        // If we get here, the JPEG code has signaled an error.
        // We need to clean up the JPEG object, close the input file, and return.
        jpeg_destroy_decompress(&cinfo);
        fclose(in);

        return false;
    }

    // Now we can initialize the JPEG decompression object.
    jpeg_create_decompress(&cinfo);

    // Step 2: specify data source (eg, a file)
    jpeg_stdio_src(&cinfo, in);

    // Step 3: read file parameters with jpeg_read_header()
    (void) jpeg_read_header(&cinfo, TRUE);

    // We can ignore the return value from jpeg_read_header since
    //  (a) suspension is not possible with the stdio data source, and
    //  (b) we passed TRUE to reject a tables-only JPEG file as an error.

    // Step 4: set parameters for decompression

    // In this example, we don't need to change any of the defaults set by
    // jpeg_read_header(), so we do nothing here.

    // Step 5: Start decompressor

    (void) jpeg_start_decompress(&cinfo);
    // We can ignore the return value since suspension is not possible
    // with the stdio data source.

    // We may need to do some setup of our own at this point before reading
    // the data.  After jpeg_start_decompress() we have the correct scaled
    // output image dimensions available, as well as the output colormap
    // if we asked for color quantization.
    // In this example, we need to make an output work buffer of the right size.
    // JSAMPLEs per row in output buffer
    row_stride = cinfo.output_width * cinfo.output_components;
    // Make a one-row-high sample array that will go away when done with image
    buffer = (*cinfo.mem->alloc_sarray)
             ((j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);

    // Step 6: while (scan lines remain to be read)
    //             jpeg_read_scanlines(...);

    // Here we use the library's state variable cinfo.output_scanline as the
    // loop counter, so that we don't have to keep track ourselves.

    mWidth = cinfo.image_width;
    mHeight = cinfo.image_height;
    mSize = mWidth * mHeight * cinfo.output_components;
    mDepth = 8;
    pPixels = new unsigned char[mSize];

    // cont = cinfo.output_height - 1;
    cont = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        // jpeg_read_scanlines expects an array of pointers to scanlines.
        // Here the array is only one element long, but you could ask for
        // more than one scanline at a time if that's more convenient.
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);

        // Assume put_scanline_someplace wants a pointer and sample count.
        // put_scanline_someplace(buffer[0], row_stride);
        memcpy( pPixels + (cont * mHeight * cinfo.output_components), buffer[0], row_stride);
        cont++;
    }

    // Step 7: Finish decompression

    (void) jpeg_finish_decompress(&cinfo);
    // We can ignore the return value since suspension is not possible
    // with the stdio data source.

    // Step 8: Release JPEG decompression object

    // This is an important step since it will release a good deal of memory.
    jpeg_destroy_decompress(&cinfo);

    // After finish_decompress, we can close the input file.
    // Here we postpone it until after no more JPEG errors are possible,
    // so as to simplify the setjmp error logic above.  (Actually, I don't
    // think that jpeg_destroy can do an error exit, but why assume anything...

    fclose(in);

    // At this point you may want to check to see whether any corrupt-data
    // warnings occurred (test whether jerr.pub.num_warnings is nonzero).

    return true;

#elif MACOSX

    Image* img = NULL;
    CGBuffer* cgJpegImage;
    size_t img_w, img_h, img_d;

    cgJpegImage = new CGBuffer(filename.c_str());
    if (cgJpegImage == NULL)
    {
        char tempcwd[2048];
        getcwd(tempcwd, sizeof(tempcwd));
        DPRINTF(0, "CGBuffer :: Error opening JPEG image file %s/%s\n", tempcwd, filename.c_str());
        delete cgJpegImage;
        return NULL;
    }

    if (!cgJpegImage->LoadJPEG())
    {
        char tempcwd[2048];
        getcwd(tempcwd, sizeof(tempcwd));
        DPRINTF(0, "CGBuffer :: Error loading JPEG image file %s/%s\n", tempcwd, filename.c_str());
        delete cgJpegImage;
        return NULL;
    }

    cgJpegImage->Render();

    img_w = static_cast<size_t> cgJpegImage->image_size.width;
    img_h = static_cast<size_t> cgJpegImage->image_size.height;
    img_d = static_cast<size_t> ((cgJpegImage->image_depth == 8) ? 1 : 4);

    // DPRINTF(0,"cgJpegImage :: %d x %d x %d [%d] bpp\n", img_w, img_h, static_cast<size_t>cgJpegImage->image_depth, img_d);

#ifdef MACOSX_ALPHA_JPEGS

    int format = (img_d == 1) ? GL_LUMINANCE : GL_RGBA;
#else

    int format = (img_d == 1) ? GL_LUMINANCE : GL_RGB;
#endif

    img = new Image(format, img_w, img_h);
    if (img == NULL || img->getPixels() == NULL)
    {
        DPRINTF(0, "Could not create image\n");
        delete cgJpegImage;
        return NULL;
    }
    // following code flips image and skips alpha byte if no alpha support
    unsigned char* bout = (unsigned char*) img->getPixels();
    unsigned char* bin  = (unsigned char*) cgJpegImage->buffer->data;
    uint bcount = img_w * img_h * img_d;
    uint i = 0;
    bin += bcount+(img_w*img_d); // start one row past end
    for (i=0; i<bcount; ++i)
    {
        // at end of row, move back two rows
        if ( (i % (img_w * img_d)) == 0 )
            bin -= 2*(img_w * img_d);

#ifndef MACOSX_ALPHA_JPEGS

        if (( (img_d != 1) && !((i&3)^3) )) // skip extra byte
        {
            ++bin;
        } else
#endif // !MACOSX_ALPHA_JPEGS

            *bout++ = *bin++;
    }
    delete cgJpegImage;
    return img;

#else

    return false;
#endif // RTK_JPEG_SUPPORT
}


#ifdef RTK_PNG_SUPPORT
void PNGReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
    FILE* fp = (FILE*) png_get_io_ptr(png_ptr);
    fread((void*) data, 1, length, fp);
}
#endif


bool BaseImage::readPNG(const string& filename)
{
#ifndef RTK_PNG_SUPPORT
    return false;
#else

    char header[8];
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    FILE* fp = NULL;
    png_bytep* row_pointers = NULL;

    fp = fopen(filename.c_str(), "rb");
    if (fp == NULL)
    {
        clog << "Error opening image file " << filename << '\n';
        return false;
    }

    if( !fread(header, 1, sizeof(header), fp) )
        return false;
    if (png_sig_cmp((unsigned char*) header, 0, sizeof(header)))
    {
        clog << "Error: " << filename << " is not a PNG file.\n";
        fclose(fp);
        return false;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        fclose(fp);
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        clog << "Error reading PNG image file " << filename << '\n';
        return false;
    }

    // png_init_io(png_ptr, fp);
    png_set_read_fn(png_ptr, (void*) fp, PNGReadData);
    png_set_sig_bytes(png_ptr, sizeof(header));

    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr,
                 &width, &height, &bit_depth,
                 &color_type, &interlace_type,
                 NULL, NULL);

    mWidth = width;
    mHeight = height;
    mDepth = bit_depth;
    mSize = mWidth * mHeight * (bit_depth / 8);
    pPixels = new unsigned char[mSize];

    // TODO: consider using paletted textures if they're available
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // TODO: consider passing images with < 8 bits/component to
    // GL without expanding
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    else if (bit_depth < 8)
        png_set_packing(png_ptr);

    row_pointers = new png_bytep[height];
    for (uint i = 0; i < height; i++)
        row_pointers[i] = (png_bytep) (pPixels + ( i * width));

    png_read_image(png_ptr, row_pointers);

    delete[] row_pointers;

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    return true;
#endif
}


// BMP file definitions--can't use windows.h because we might not be
// built on Windows!
typedef struct
{
    unsigned char b;
    unsigned char m;
    uint size;
    uint reserved;
    uint offset;
}
BMPFileHeader;

typedef struct
{
    uint size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bpp;
    uint compression;
    uint imageSize;
    int widthPPM;
    int heightPPM;
    uint colorsUsed;
    uint colorsImportant;
}
BMPImageHeader;


static int readInt(ifstream& in)
{
    unsigned char b[4];
    in.read(reinterpret_cast<char*>(b), 4);
    return ((int) b[3] << 24) + ((int) b[2] << 16)
           + ((int) b[1] << 8) + (int) b[0];
}

static short readShort(ifstream& in)
{
    unsigned char b[2];
    in.read(reinterpret_cast<char*>(b), 2);
    return ((short) b[1] << 8) + (short) b[0];
}


bool BaseImage::readBMP(ifstream& in)
{
    BMPFileHeader fileHeader;
    BMPImageHeader imageHeader;

    in >> fileHeader.b;
    in >> fileHeader.m;
    fileHeader.size = readInt(in);
    fileHeader.reserved = readInt(in);
    fileHeader.offset = readInt(in);

    if (fileHeader.b != 'B' || fileHeader.m != 'M')
        return false;

    imageHeader.size = readInt(in);
    imageHeader.width = readInt(in);
    imageHeader.height = readInt(in);
    imageHeader.planes = readShort(in);
    imageHeader.bpp = readShort(in);
    imageHeader.compression = readInt(in);
    imageHeader.imageSize = readInt(in);
    imageHeader.widthPPM = readInt(in);
    imageHeader.heightPPM = readInt(in);
    imageHeader.colorsUsed = readInt(in);
    imageHeader.colorsImportant = readInt(in);

    if (imageHeader.width <= 0 || imageHeader.height <= 0)
        return false;

    // We currently don't support compressed BMPs
    if (imageHeader.compression != 0)
        return false;
    // We don't handle 1-, 2-, or 4-bpp images
    if (imageHeader.bpp != 8 && imageHeader.bpp != 24 && imageHeader.bpp != 32)
        return false;

    unsigned char* palette = NULL;
    if (imageHeader.bpp == 8)
    {
        printf("Reading %d color palette\n", imageHeader.colorsUsed);
        palette = new unsigned char[imageHeader.colorsUsed * 4];
        in.read(reinterpret_cast<char*>(palette), imageHeader.colorsUsed * 4);
    }

    in.seekg(fileHeader.offset, ios::beg);

    uint bytesPerRow =
        (imageHeader.width * imageHeader.bpp / 8 + 1) & ~1;
    uint imageBytes = bytesPerRow * imageHeader.height;

    // slurp the image data
    pPixels = new unsigned char[imageBytes];
    in.read(reinterpret_cast<char*>(pPixels), imageBytes);

    // check for truncated file

    mWidth = imageHeader.width;
    mHeight = imageHeader.height;
    mDepth = imageHeader.bpp;

    // Copy the image and perform any necessary conversions
    for (int y = 0; y < imageHeader.height; y++)
    {
        unsigned char* src = &pPixels[y * bytesPerRow];
        unsigned char* dst = &pPixels[y * bytesPerRow];

        switch (imageHeader.bpp)
        {
        case 8:
        {
            for (int x = 0; x < imageHeader.width; x++)
            {
                unsigned char* color = palette + (*src << 2);
                dst[0] = color[2];
                dst[1] = color[1];
                dst[2] = color[0];
                src++;
                dst += 3;
            }
        }
        break;

        case 24:
        {
            for (int x = 0; x < imageHeader.width; x++)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                src += 3;
                dst += 3;
            }
        }
        break;

        case 32:
        {
            for (int x = 0; x < imageHeader.width; x++)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                src += 4;
                dst += 3;
            }
        }
        break;
        }
    }

    return true;
}


bool BaseImage::readBMP(const string& filename)
{
    bool ret = false;
    ifstream bmpFile(filename.c_str(), ios::in | ios::binary);

    if (bmpFile.good())
    {
        ret = readBMP(bmpFile);
        bmpFile.close();
    }
    return ret;
}

} // namespace RTK
