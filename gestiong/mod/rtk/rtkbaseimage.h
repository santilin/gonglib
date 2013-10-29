#ifndef RTKBASEIMAGE_H
#define RTKBASEIMAGE_H

/*<<<<<COPYLEFT*/
/** @file rtkbaseimage.h Low level image handling
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

#include <string>
#include <iostream>
#include <fstream>

namespace gong {}
namespace RTK = gong;

namespace gong {

class BaseImage
{
public:
    BaseImage() : mWidth(0), mHeight(0), mDepth(0), mSize(0), pPixels(0) {}
    ~BaseImage() {
        if( pPixels ) delete pPixels;
    }

    bool readJPEG(const std::string& filename, int channels = BaseImage::ColorChannel);
    bool readBMP(const std::string& filename);
    bool readPNG(const std::string& filename);
    bool read(const std::string& filename);

    int width() const {
        return mWidth;
    }
    int height() const {
        return mHeight;
    }
    int depth() const {
        return mDepth;
    }
    unsigned char* pixels() const {
        return pPixels;
    }
    int size() const {
        return mSize;
    }

    enum {
        ColorChannel = 1,
        AlphaChannel = 2
    };

private:
    int mWidth;
    int mHeight;
    int mDepth;
    int mSize;
    unsigned char* pPixels;
    bool readBMP(std::ifstream& in);
    BaseImage(const BaseImage &);
    BaseImage &operator =(const BaseImage &);
};


} // namespace RTK

#endif // RTKBASEIMAGE_H
