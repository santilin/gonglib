#ifndef RTKPARAMETER_H
#define RTKPARAMETER_H

/*<<<<<COPYLEFT*/
/** @file rtkparameter.h Report parameters
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
#include <gongvariant.h>
#include "rtkproperty.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class Parameter
{
public:
    Parameter(const char *name, const char *value, Variant::Type type = Variant::tString,
              const char *caption = 0 )
        : propName(name), propValue(value), propType(type), propCaption(caption)
    {}
    void setValue(const char *value) {
        propValue.set(value);
    }
    /*<<<<<PARAMETER_PROPERTIES*/
public:
	const char * name() const { return propName.get(); }
	const char * value() const { return propValue.get(); }
	Variant::Type type() const { return propType.get(); }
	const char * caption() const { return propCaption.get(); }

public:
	ReadOnlyProperty<const char *> propName;
	Property<const char *> propValue;
	Property<Variant::Type> propType;
	Property<const char *> propCaption;
/*>>>>>PARAMETER_PROPERTIES*/
};

typedef std::vector<Parameter *> ParametersList;

} // namespace

#endif
