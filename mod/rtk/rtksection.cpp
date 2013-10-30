#include "rtksection.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

Section::Section(SectionType type, Object *parent, const char *name )
    : Object(parent, name), propType(type),
      mRecordNumber(0), mRecordCount(0)
{
}

Section::~Section()
{
    for( uint i=0; i<mObjects.size(); i++ )
        delete mObjects[i];
}

Object *Section::findObject(const Xtring &name) const
{
    for( uint i=0; i<mObjects.size(); i++ )
        if( mObjects[i]->name() == name )
            return mObjects[i];
    return 0;
}

Object *Section::getObject(int i) const
{
    if( i<0 || i>(int)mObjects.size() )
        return 0;
    else
        return mObjects[i];
}


int Section::insertObject(Object *object)
{
    mObjects.push_back(object);
    return mObjects.size();
}

Variant Section::getPropValue(const Xtring & propname) const
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        return mRealValue;
    /*<<<<<SECTION_GETPROPVALUE*/
    else if( strcasecmp(name, "Columns") == 0 )
        return propColumns.get();
    else if( strcasecmp(name, "PageBefore") == 0 )
        return propPageBefore.get();
    else if( strcasecmp(name, "PageAfter") == 0 )
        return propPageAfter.get();
    else if( strcasecmp(name, "ResetNPage") == 0 )
        return propResetNPage.get();
    else if( strcasecmp(name, "SupIfBlank") == 0 )
        return propSupIfBlank.get();
    else if( strcasecmp(name, "RepeatHeader") == 0 )
        return propRepeatHeader.get();
    else if( strcasecmp(name, "PrintInFirstPage") == 0 )
        return propPrintInFirstPage.get();
    else if( strcasecmp(name, "Padding") == 0 )
        return propPadding.get();
    else if( strcasecmp(name, "PaddingLeft") == 0 )
        return propPaddingLeft.get();
    else if( strcasecmp(name, "PaddingTop") == 0 )
        return propPaddingTop.get();
    /*>>>>>SECTION_GETPROPVALUE*/
    else if( Object::getPropValue(propname) == Variant() ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring objname, pname;
            propname.splitIn2( objname, pname, "." );
            for( uint nobj=0; nobj<mObjects.size(); nobj++ ) {
                Object *obj = mObjects[nobj];
                if( !strcaseequal( obj->name(), objname.c_str() ) )
                    return obj->getPropValue( pname );
            }
        }
    }
    return Variant();
}

bool Section::setPropValue(const Xtring &propname, const Variant &value)
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        mRealValue = value;
    /*<<<<<SECTION_SETPROPVALUE*/
    else if( strcasecmp(name, "Columns") == 0 )
        propColumns.set( value.toInt());
    else if( strcasecmp(name, "PageBefore") == 0 )
        propPageBefore.set( value.toBool());
    else if( strcasecmp(name, "PageAfter") == 0 )
        propPageAfter.set( value.toBool());
    else if( strcasecmp(name, "ResetNPage") == 0 )
        propResetNPage.set( value.toBool());
    else if( strcasecmp(name, "SupIfBlank") == 0 )
        propSupIfBlank.set( value.toBool());
    else if( strcasecmp(name, "RepeatHeader") == 0 )
        propRepeatHeader.set( value.toBool());
    else if( strcasecmp(name, "PrintInFirstPage") == 0 )
        propPrintInFirstPage.set( value.toBool());
    else if( strcasecmp(name, "Padding") == 0 )
        propPadding.set( value.toDouble());
    else if( strcasecmp(name, "PaddingLeft") == 0 )
        propPaddingLeft.set( value.toDouble());
    else if( strcasecmp(name, "PaddingTop") == 0 )
        propPaddingTop.set( value.toDouble());
    /*>>>>>SECTION_SETPROPVALUE*/
    else if( !Object::setPropValue(propname, value) ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring objname, pname;
            propname.splitIn2( objname, pname, "." );
            for( uint nobj=0; nobj<mObjects.size(); nobj++ ) {
                Object *obj = mObjects[nobj];
                if( !strcaseequal( obj->name(), objname.c_str() ) )
                    return obj->setPropValue( pname, value );
            }
        }
        return false;
    }
    return true;
}

bool Section::setPropOrigValue(const Xtring &propname, const Xtring &value)
{
    const char *name = propname.c_str();
    if( false ) {}
    /*<<<<<SECTION_SETORIGPROPVALUE*/
    else if( strcasecmp(name, "Columns") == 0 )
        propColumns.setOrig( value.c_str() );
    else if( strcasecmp(name, "PageBefore") == 0 )
        propPageBefore.setOrig( value.c_str() );
    else if( strcasecmp(name, "PageAfter") == 0 )
        propPageAfter.setOrig( value.c_str() );
    else if( strcasecmp(name, "ResetNPage") == 0 )
        propResetNPage.setOrig( value.c_str() );
    else if( strcasecmp(name, "SupIfBlank") == 0 )
        propSupIfBlank.setOrig( value.c_str() );
    else if( strcasecmp(name, "RepeatHeader") == 0 )
        propRepeatHeader.setOrig( value.c_str() );
    else if( strcasecmp(name, "PrintInFirstPage") == 0 )
        propPrintInFirstPage.setOrig( value.c_str() );
    else if( strcasecmp(name, "Padding") == 0 )
        propPadding.setOrig( value.c_str() );
    else if( strcasecmp(name, "PaddingLeft") == 0 )
        propPaddingLeft.setOrig( value.c_str() );
    else if( strcasecmp(name, "PaddingTop") == 0 )
        propPaddingTop.setOrig( value.c_str() );
    /*>>>>>SECTION_SETORIGPROPVALUE*/
    else if( !Object::setPropOrigValue(propname, value) ) {
        if( propname.find(".") != Xtring::npos ) {
            Xtring objname, pname;
            propname.splitIn2( objname, pname, "." );
            for( uint nobj=0; nobj<mObjects.size(); nobj++ ) {
                Object *obj = mObjects[nobj];
                if( !strcaseequal( obj->name(), objname.c_str() ) )
                    return obj->setPropOrigValue( pname, value );
            }
        }
        return false;
    }
    return true;
}

void Section::fixParameters(const ParametersList &parameters, const char *delim)
{
    Object::fixParameters(parameters, delim);
    /*<<<<<SECTION_FIXPARAMETERS*/
    propColumns.fix( parameters, delim, 1 );
    propPageBefore.fix( parameters, delim, false );
    propPageAfter.fix( parameters, delim, false );
    propResetNPage.fix( parameters, delim, false );
    propSupIfBlank.fix( parameters, delim, false );
    propRepeatHeader.fix( parameters, delim, false );
    propPrintInFirstPage.fix( parameters, delim, true );
    propPadding.fix( parameters, delim, 0 );
    propPaddingLeft.fix( parameters, delim, 0 );
    propPaddingTop.fix( parameters, delim, 0 );
    /*>>>>>SECTION_FIXPARAMETERS*/
    if( isDetails() )
        propGroupLevel = 0;
    for( uint nobj=0; nobj<mObjects.size(); nobj++ )
        mObjects[nobj]->fixParameters(parameters, delim);
}


/*<<<<<SECTION_PageBefore_GET*/
bool Section::pageBefore() const
{
    if( propPageBefore.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPageBefore.isNull()) {
            return styledef->propPageBefore.get();
        }
    }
    return propPageBefore.get();
}
/*>>>>>SECTION_PageBefore_GET*/
/*<<<<<SECTION_PageAfter_GET*/
bool Section::pageAfter() const
{
    if( propPageAfter.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPageAfter.isNull()) {
            return styledef->propPageAfter.get();
        }
    }
    return propPageAfter.get();
}
/*>>>>>SECTION_PageAfter_GET*/
/*<<<<<SECTION_ResetNPage_GET*/
bool Section::resetNPage() const
{
    if( propResetNPage.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propResetNPage.isNull()) {
            return styledef->propResetNPage.get();
        }
    }
    return propResetNPage.get();
}
/*>>>>>SECTION_ResetNPage_GET*/
/*<<<<<SECTION_SupIfBlank_GET*/
bool Section::supIfBlank() const
{
    if( propSupIfBlank.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propSupIfBlank.isNull()) {
            return styledef->propSupIfBlank.get();
        }
    }
    return propSupIfBlank.get();
}
/*>>>>>SECTION_SupIfBlank_GET*/
/*<<<<<SECTION_RepeatHeader_GET*/
bool Section::repeatHeader() const
{
    if( propRepeatHeader.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propRepeatHeader.isNull()) {
            return styledef->propRepeatHeader.get();
        }
    }
    return propRepeatHeader.get();
}
/*>>>>>SECTION_RepeatHeader_GET*/
/*<<<<<SECTION_PrintInFirstPage_GET*/
bool Section::printInFirstPage() const
{
    if( propPrintInFirstPage.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPrintInFirstPage.isNull()) {
            return styledef->propPrintInFirstPage.get();
        }
    }
    return propPrintInFirstPage.get();
}
/*>>>>>SECTION_PrintInFirstPage_GET*/

/*<<<<<SECTION_Padding_GET*/
Measure Section::padding() const
{
    if( propPadding.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPadding.isNull()) {
            return styledef->propPadding.get();
        }
    }
    return propPadding.get();
}
/*>>>>>SECTION_Padding_GET*/
/*<<<<<SECTION_PaddingLeft_GET*/
Measure Section::paddingLeft() const
{
    if( propPaddingLeft.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPaddingLeft.isNull()) {
            return styledef->propPaddingLeft.get();
        }
    }
    return propPaddingLeft.get();
}
/*>>>>>SECTION_PaddingLeft_GET*/
/*<<<<<SECTION_PaddingTop_GET*/
Measure Section::paddingTop() const
{
    if( propPaddingTop.isNull() ) {
        Style *styledef = report()->findStyle(style());
        if( styledef && !styledef->propPaddingTop.isNull()) {
            return styledef->propPaddingTop.get();
        }
    }
    return propPaddingTop.get();
}
/*>>>>>SECTION_PaddingTop_GET*/

} // namespace RTK

