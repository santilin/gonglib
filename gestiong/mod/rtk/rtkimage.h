#ifndef RTKIMAGE_H
#define RTKIMAGE_H

/*<<<<<COPYLEFT*/
/** @file rtkimage.h Upper level Image object
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

#include <gongxtring.h>
#include "rtkproperty.h"
#include "rtkbaseimage.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
  @brief This class represents an Image or other multimedia file

  */
class Image
{
public:
    Image(const char * name)
        : propName(name), pBaseImage(0) {}
    virtual ~Image();
    const BaseImage *findBaseImage(const Xtring &paths, const Xtring &cwd = Xtring::null);

    /*<<<<<IMAGE_PROPERTIES*/
public:
    const char * name() const {
        return propName.get();
    }
    const char * url() const {
        return propUrl.get();
    }
    void setOrigUrl(const char * url )		{
        propUrl.setOrig(url);
    }
    const char * mimeType() const {
        return propMimeType.get();
    }
    void setOrigMimeType(const char * mimetype )		{
        propMimeType.setOrig(mimetype);
    }
    ImageStyle imageStyle() const {
        return propImageStyle.get();
    }
    void setOrigImageStyle(const char * imagestyle )		{
        propImageStyle.setOrig(imagestyle);
    }
    virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    ReadOnlyProperty<const char *> propName;
    FixableProperty<const char *> propUrl;
    FixableProperty<const char *> propMimeType;
    FixableProperty<ImageStyle> propImageStyle;
    /*>>>>>IMAGE_PROPERTIES*/
private:
    BaseImage *pBaseImage;
};

} // namespace RTK

#endif
