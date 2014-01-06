#include <ctype.h>
#include <gongdebug.h>
#include <gongformatter.h>
#include "rtkdscript.h"
#include "rtkcompiler.h"
#include "rtkstdlib.h"
#include "rtkobject.h"
#include "rtkreport.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
  @class Object
  @brief The Object class is the base class of all the objects that form a report definition.

  A report definition is a Report object which contains several Section objects   which in turn contain several Object objects.

*/

Object::Object( Object *parent, const char *name ) :
    propVisible(true), propSupressed(false),
    pParent(parent), mIsConst(false), mInputFieldNumber(-1), mFontSizeIncrement(0.0)
{
    // If propName is initialized on the constructor above,
    // propName.isNull() would be true as all the properties must be 'set' to avoid being null
    if( !strempty(name) )
        propName.set(name);
}


Object::~Object()
{
}

bool Object::isReport() const {
    return dynamic_cast<const Report *>(this);
}
bool Object::isSection() const {
    return dynamic_cast<const Section *>(this);
}
bool Object::isObject() const {
    return dynamic_cast<const Object *>(this);
}
Report *Object::report() const {
    if( isReport() )
        return static_cast<Report *>(const_cast<Object *>(this));
    else if( isSection() )
        return static_cast<Report *>(parent());
    else {
        Object *_parent = static_cast<Report *>(parent());
        if( _parent )
            return static_cast<Report *>(_parent->parent());
        else
            return 0;
    }
}

bool Object::formattedValueChanged() const
{
    return ( mPrevFormattedText != mFormattedText );
}


/**
 * Gets the real value of an object which has been evaluated on evalConstValues at the
 * beginning, or on evalNonConstValues and calcAggregateValues on each record.
 * @return the real value
 */
Variant Object::realValue() const
{
    if ( aggregate() != AggNone && !isSection() )
        return mAggregateValues[static_cast<Section *>(parent())->groupLevel()];
    else
        return mRealValue;
}

Xtring Object::text() const
{
    return realValue().toString();
}

void Object::formatText()
{
    mPrevFormattedText = mFormattedText;
    Variant v = realValue();
    mFormattedText = report()->formatter().format(v, format(), mask(), v.type() );
}

Variant Object::getAggregateValue(uint level)
{
    return mAggregateValues[level];
}

Variant Object::getPropValue(const Xtring & propname) const
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        return mRealValue;
    /*<<<<<OBJECT_GETPROPVALUE*/
	else if( strcasecmp(name, "Value") == 0 )
		return propValue.get();
	else if( strcasecmp(name, "FormulaBefore") == 0 )
		return propFormulaBefore.get();
	else if( strcasecmp(name, "FormulaAfter") == 0 )
		return propFormulaAfter.get();
	else if( strcasecmp(name, "Aggregate") == 0 )
		return propAggregate.get();
	else if( strcasecmp(name, "PosX") == 0 )
		return propPosX.get();
	else if( strcasecmp(name, "PosY") == 0 )
		return propPosY.get();
	else if( strcasecmp(name, "SizeX") == 0 )
		return propSizeX.get();
	else if( strcasecmp(name, "SizeY") == 0 )
		return propSizeY.get();
	else if( strcasecmp(name, "Style") == 0 )
		return propStyle.get();
	else if( strcasecmp(name, "Url") == 0 )
		return propUrl.get();
	else if( strcasecmp(name, "Tag") == 0 )
		return propTag.get();
	else if( strcasecmp(name, "MarginLeft") == 0 )
		return propMarginLeft.get();
	else if( strcasecmp(name, "MarginRight") == 0 )
		return propMarginRight.get();
	else if( strcasecmp(name, "MarginTop") == 0 )
		return propMarginTop.get();
	else if( strcasecmp(name, "MarginBottom") == 0 )
		return propMarginBottom.get();
	else if( strcasecmp(name, "Padding") == 0 )
		return propPadding.get();
	else if( strcasecmp(name, "PaddingLeft") == 0 )
		return propPaddingLeft.get();
	else if( strcasecmp(name, "PaddingTop") == 0 )
		return propPaddingTop.get();
	else if( strcasecmp(name, "Visible") == 0 )
		return propVisible.get();
	else if( strcasecmp(name, "Supressed") == 0 )
		return propSupressed.get();
	else if( strcasecmp(name, "SupressDup") == 0 )
		return propSupressDup.get();
	else if( strcasecmp(name, "FontFamily") == 0 )
		return propFontFamily.get();
	else if( strcasecmp(name, "FontSize") == 0 )
		return propFontSize.get();
	else if( strcasecmp(name, "FontWeight") == 0 )
		return propFontWeight.get();
	else if( strcasecmp(name, "FontItalic") == 0 )
		return propFontItalic.get();
	else if( strcasecmp(name, "MinFontSize") == 0 )
		return propMinFontSize.get();
	else if( strcasecmp(name, "Adjustment") == 0 )
		return propAdjustment.get();
	else if( strcasecmp(name, "MaxLines") == 0 )
		return propMaxLines.get();
	else if( strcasecmp(name, "Format") == 0 )
		return propFormat.get();
	else if( strcasecmp(name, "Mask") == 0 )
		return propMask.get();
	else if( strcasecmp(name, "Locale") == 0 )
		return propLocale.get();
	else if( strcasecmp(name, "HAlignment") == 0 )
		return propHAlignment.get();
	else if( strcasecmp(name, "VAlignment") == 0 )
		return propVAlignment.get();
	else if( strcasecmp(name, "FontColor") == 0 )
		return propFontColor.get().toUInt();
	else if( strcasecmp(name, "BackColor") == 0 )
		return propBackColor.get().toUInt();
	else if( strcasecmp(name, "BorderColor") == 0 )
		return propBorderColor.get().toUInt();
	else if( strcasecmp(name, "BorderWidth") == 0 )
		return propBorderWidth.get();
	else if( strcasecmp(name, "BorderStyle") == 0 )
		return propBorderStyle.get();
	else if( strcasecmp(name, "BorderTopColor") == 0 )
		return propBorderTopColor.get().toUInt();
	else if( strcasecmp(name, "BorderTopWidth") == 0 )
		return propBorderTopWidth.get();
	else if( strcasecmp(name, "BorderTopStyle") == 0 )
		return propBorderTopStyle.get();
	else if( strcasecmp(name, "BorderBottomColor") == 0 )
		return propBorderBottomColor.get().toUInt();
	else if( strcasecmp(name, "BorderBottomWidth") == 0 )
		return propBorderBottomWidth.get();
	else if( strcasecmp(name, "BorderBottomStyle") == 0 )
		return propBorderBottomStyle.get();
	else if( strcasecmp(name, "BorderLeftColor") == 0 )
		return propBorderLeftColor.get().toUInt();
	else if( strcasecmp(name, "BorderLeftWidth") == 0 )
		return propBorderLeftWidth.get();
	else if( strcasecmp(name, "BorderLeftStyle") == 0 )
		return propBorderLeftStyle.get();
	else if( strcasecmp(name, "BorderRightColor") == 0 )
		return propBorderRightColor.get().toUInt();
	else if( strcasecmp(name, "BorderRightWidth") == 0 )
		return propBorderRightWidth.get();
	else if( strcasecmp(name, "BorderRightStyle") == 0 )
		return propBorderRightStyle.get();
	else if( strcasecmp(name, "BackgroundImage") == 0 )
		return propBackgroundImage.get();
	else if( strcasecmp(name, "ImageStyle") == 0 )
		return propImageStyle.get();
/*>>>>>OBJECT_GETPROPVALUE*/
    return Variant();
}


bool Object::setPropValue(const Xtring &propname, const Variant &value)
{
    const char *name = propname.c_str();
    if( strcasecmp(name, "VALUE" ) == 0 )
        mRealValue = value;
    /*<<<<<OBJECT_SETPROPVALUE*/
	else if( strcasecmp(name, "Value") == 0 )
		propValue.set( value.toString().c_str());
	else if( strcasecmp(name, "FormulaBefore") == 0 )
		propFormulaBefore.set( value.toString().c_str());
	else if( strcasecmp(name, "FormulaAfter") == 0 )
		propFormulaAfter.set( value.toString().c_str());
	else if( strcasecmp(name, "Aggregate") == 0 )
		propAggregate.set( static_cast<AggregateType>( value.toInt() ));
	else if( strcasecmp(name, "PosX") == 0 )
		propPosX.set( value.toDouble());
	else if( strcasecmp(name, "PosY") == 0 )
		propPosY.set( value.toDouble());
	else if( strcasecmp(name, "SizeX") == 0 )
		propSizeX.set( value.toDouble());
	else if( strcasecmp(name, "SizeY") == 0 )
		propSizeY.set( value.toDouble());
	else if( strcasecmp(name, "Style") == 0 )
		propStyle.set( value.toString().c_str());
	else if( strcasecmp(name, "Url") == 0 )
		propUrl.set( value.toString().c_str());
	else if( strcasecmp(name, "Tag") == 0 )
		propTag.set( value.toString().c_str());
	else if( strcasecmp(name, "MarginLeft") == 0 )
		propMarginLeft.set( value.toDouble());
	else if( strcasecmp(name, "MarginRight") == 0 )
		propMarginRight.set( value.toDouble());
	else if( strcasecmp(name, "MarginTop") == 0 )
		propMarginTop.set( value.toDouble());
	else if( strcasecmp(name, "MarginBottom") == 0 )
		propMarginBottom.set( value.toDouble());
	else if( strcasecmp(name, "Padding") == 0 )
		propPadding.set( value.toDouble());
	else if( strcasecmp(name, "PaddingLeft") == 0 )
		propPaddingLeft.set( value.toDouble());
	else if( strcasecmp(name, "PaddingTop") == 0 )
		propPaddingTop.set( value.toDouble());
	else if( strcasecmp(name, "Visible") == 0 )
		propVisible.set( value.toBool());
	else if( strcasecmp(name, "Supressed") == 0 )
		propSupressed.set( value.toBool());
	else if( strcasecmp(name, "SupressDup") == 0 )
		propSupressDup.set( value.toBool());
	else if( strcasecmp(name, "FontFamily") == 0 )
		propFontFamily.set( value.toString().c_str());
	else if( strcasecmp(name, "FontSize") == 0 )
		propFontSize.set( value.toDouble());
	else if( strcasecmp(name, "FontWeight") == 0 )
		propFontWeight.set( value.toInt());
	else if( strcasecmp(name, "FontItalic") == 0 )
		propFontItalic.set( value.toBool());
	else if( strcasecmp(name, "MinFontSize") == 0 )
		propMinFontSize.set( value.toDouble());
	else if( strcasecmp(name, "Adjustment") == 0 )
		propAdjustment.set( static_cast<Adjustment>( value.toInt() ));
	else if( strcasecmp(name, "MaxLines") == 0 )
		propMaxLines.set( value.toInt());
	else if( strcasecmp(name, "Format") == 0 )
		propFormat.set( value.toString().c_str());
	else if( strcasecmp(name, "Mask") == 0 )
		propMask.set( value.toString().c_str());
	else if( strcasecmp(name, "Locale") == 0 )
		propLocale.set( value.toString().c_str());
	else if( strcasecmp(name, "HAlignment") == 0 )
		propHAlignment.set( static_cast<Alignment>( value.toInt() ));
	else if( strcasecmp(name, "VAlignment") == 0 )
		propVAlignment.set( static_cast<Alignment>( value.toInt() ));
	else if( strcasecmp(name, "FontColor") == 0 )
		propFontColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BackColor") == 0 )
		propBackColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderColor") == 0 )
		propBorderColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderWidth") == 0 )
		propBorderWidth.set( value.toInt());
	else if( strcasecmp(name, "BorderStyle") == 0 )
		propBorderStyle.set( static_cast<BorderStyle>( value.toInt() ));
	else if( strcasecmp(name, "BorderTopColor") == 0 )
		propBorderTopColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderTopWidth") == 0 )
		propBorderTopWidth.set( value.toInt());
	else if( strcasecmp(name, "BorderTopStyle") == 0 )
		propBorderTopStyle.set( static_cast<BorderStyle>( value.toInt() ));
	else if( strcasecmp(name, "BorderBottomColor") == 0 )
		propBorderBottomColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderBottomWidth") == 0 )
		propBorderBottomWidth.set( value.toInt());
	else if( strcasecmp(name, "BorderBottomStyle") == 0 )
		propBorderBottomStyle.set( static_cast<BorderStyle>( value.toInt() ));
	else if( strcasecmp(name, "BorderLeftColor") == 0 )
		propBorderLeftColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderLeftWidth") == 0 )
		propBorderLeftWidth.set( value.toInt());
	else if( strcasecmp(name, "BorderLeftStyle") == 0 )
		propBorderLeftStyle.set( static_cast<BorderStyle>( value.toInt() ));
	else if( strcasecmp(name, "BorderRightColor") == 0 )
		propBorderRightColor.set( static_cast<Color>( value.toInt() ));
	else if( strcasecmp(name, "BorderRightWidth") == 0 )
		propBorderRightWidth.set( value.toInt());
	else if( strcasecmp(name, "BorderRightStyle") == 0 )
		propBorderRightStyle.set( static_cast<BorderStyle>( value.toInt() ));
	else if( strcasecmp(name, "BackgroundImage") == 0 )
		propBackgroundImage.set( value.toString().c_str());
	else if( strcasecmp(name, "ImageStyle") == 0 )
		propImageStyle.set( static_cast<ImageStyle>( value.toInt() ));
/*>>>>>OBJECT_SETPROPVALUE*/
    return false;
}

bool Object::setPropOrigValue(const Xtring &propname, const Xtring &value)
{
    const char *name = propname.c_str();
    if( false ) {}
    /*<<<<<OBJECT_SETORIGPROPVALUE*/
	else if( strcasecmp(name, "Value") == 0 )
		propValue.setOrig( value.c_str() );
	else if( strcasecmp(name, "FormulaBefore") == 0 )
		propFormulaBefore.setOrig( value.c_str() );
	else if( strcasecmp(name, "FormulaAfter") == 0 )
		propFormulaAfter.setOrig( value.c_str() );
	else if( strcasecmp(name, "Aggregate") == 0 )
		propAggregate.setOrig( value.c_str() );
	else if( strcasecmp(name, "PosX") == 0 )
		propPosX.setOrig( value.c_str() );
	else if( strcasecmp(name, "PosY") == 0 )
		propPosY.setOrig( value.c_str() );
	else if( strcasecmp(name, "SizeX") == 0 )
		propSizeX.setOrig( value.c_str() );
	else if( strcasecmp(name, "SizeY") == 0 )
		propSizeY.setOrig( value.c_str() );
	else if( strcasecmp(name, "Style") == 0 )
		propStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "Url") == 0 )
		propUrl.setOrig( value.c_str() );
	else if( strcasecmp(name, "Tag") == 0 )
		propTag.setOrig( value.c_str() );
	else if( strcasecmp(name, "MarginLeft") == 0 )
		propMarginLeft.setOrig( value.c_str() );
	else if( strcasecmp(name, "MarginRight") == 0 )
		propMarginRight.setOrig( value.c_str() );
	else if( strcasecmp(name, "MarginTop") == 0 )
		propMarginTop.setOrig( value.c_str() );
	else if( strcasecmp(name, "MarginBottom") == 0 )
		propMarginBottom.setOrig( value.c_str() );
	else if( strcasecmp(name, "Padding") == 0 )
		propPadding.setOrig( value.c_str() );
	else if( strcasecmp(name, "PaddingLeft") == 0 )
		propPaddingLeft.setOrig( value.c_str() );
	else if( strcasecmp(name, "PaddingTop") == 0 )
		propPaddingTop.setOrig( value.c_str() );
	else if( strcasecmp(name, "Visible") == 0 )
		propVisible.setOrig( value.c_str() );
	else if( strcasecmp(name, "Supressed") == 0 )
		propSupressed.setOrig( value.c_str() );
	else if( strcasecmp(name, "SupressDup") == 0 )
		propSupressDup.setOrig( value.c_str() );
	else if( strcasecmp(name, "FontFamily") == 0 )
		propFontFamily.setOrig( value.c_str() );
	else if( strcasecmp(name, "FontSize") == 0 )
		propFontSize.setOrig( value.c_str() );
	else if( strcasecmp(name, "FontWeight") == 0 )
		propFontWeight.setOrig( value.c_str() );
	else if( strcasecmp(name, "FontItalic") == 0 )
		propFontItalic.setOrig( value.c_str() );
	else if( strcasecmp(name, "MinFontSize") == 0 )
		propMinFontSize.setOrig( value.c_str() );
	else if( strcasecmp(name, "Adjustment") == 0 )
		propAdjustment.setOrig( value.c_str() );
	else if( strcasecmp(name, "MaxLines") == 0 )
		propMaxLines.setOrig( value.c_str() );
	else if( strcasecmp(name, "Format") == 0 )
		propFormat.setOrig( value.c_str() );
	else if( strcasecmp(name, "Mask") == 0 )
		propMask.setOrig( value.c_str() );
	else if( strcasecmp(name, "Locale") == 0 )
		propLocale.setOrig( value.c_str() );
	else if( strcasecmp(name, "HAlignment") == 0 )
		propHAlignment.setOrig( value.c_str() );
	else if( strcasecmp(name, "VAlignment") == 0 )
		propVAlignment.setOrig( value.c_str() );
	else if( strcasecmp(name, "FontColor") == 0 )
		propFontColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BackColor") == 0 )
		propBackColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderColor") == 0 )
		propBorderColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderWidth") == 0 )
		propBorderWidth.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderStyle") == 0 )
		propBorderStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderTopColor") == 0 )
		propBorderTopColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderTopWidth") == 0 )
		propBorderTopWidth.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderTopStyle") == 0 )
		propBorderTopStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderBottomColor") == 0 )
		propBorderBottomColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderBottomWidth") == 0 )
		propBorderBottomWidth.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderBottomStyle") == 0 )
		propBorderBottomStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderLeftColor") == 0 )
		propBorderLeftColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderLeftWidth") == 0 )
		propBorderLeftWidth.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderLeftStyle") == 0 )
		propBorderLeftStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderRightColor") == 0 )
		propBorderRightColor.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderRightWidth") == 0 )
		propBorderRightWidth.setOrig( value.c_str() );
	else if( strcasecmp(name, "BorderRightStyle") == 0 )
		propBorderRightStyle.setOrig( value.c_str() );
	else if( strcasecmp(name, "BackgroundImage") == 0 )
		propBackgroundImage.setOrig( value.c_str() );
	else if( strcasecmp(name, "ImageStyle") == 0 )
		propImageStyle.setOrig( value.c_str() );
/*>>>>>OBJECT_SETORIGPROPVALUE*/
    else
        return false;
    return true;
}

double scalesArray[ 5 ][ 5 ] =
{
    { 1, 1 / dotsperchar, charsperinch, charsperinch / cmperinch, charsperinch / mmperinch },
    { dotsperchar, 1, dotsperinch, dotsperinch / cmperinch, dotsperinch / mmperinch },
    { 1 / charsperinch, 1 / dotsperinch, 1, 1 / cmperinch, 1 / mmperinch },
    { cmperinch / charsperinch, cmperinch / dotsperinch, cmperinch, 1, 10 },
    { mmperinch / charsperinch, mmperinch / dotsperinch, mmperinch, 0.1, 1 }
};


/**
	* @brief Fix all this object measures
	*  sizex == 0, the size of the parent
	*  posx == -1, following the last one
	*  posx < -1, the parent right margin - abs(posx)
	*  sizey == 0, the size of the parent - posy
	*
	* @param outsizex ...
	* @param outsizey ...
	* @param repunit ...
	* @param repsizex ...
	* @param repsizey ...
	* @param prevobj ...
	* @return void
	**/
void Object::fixMeasures( Measure outsizex, Measure outsizey,
                          UnitsType repunit, Measure repsizex, Measure repsizey,
                          Object *prevobj )
{
    UnitsType u = extractUnits(propPosX.getOrig(), repunit );
    Measure m = stringTo<Measure>(propPosX.getOrig() );
    bool posxwasminus1 = ( m == Measure(-1) );
    propPosX = fixOneMeasure( u, m, repunit, repsizex, outsizex );
    u = extractUnits(propPosY.getOrig(), repunit );
    m = stringTo<Measure>(propPosY.getOrig() );
    bool posywasminus1 = ( m == Measure(-1) );
    propPosY = fixOneMeasure( u, m, repunit, repsizey, outsizey );
    // If the sizes are not defined, we take the parent's sizes
    m = stringTo<Measure>(propSizeX.getOrig() );
    if( m == Measure(0) ) {
        if( parent() )
            propSizeX = static_cast<Object *>(parent())->sizeX() - posX();
        else
            propSizeX = outsizex - posX();
    } else {
        u = extractUnits(propSizeX.getOrig(), repunit );
        m = stringTo<Measure>(propSizeX.getOrig() );
        propSizeX = fixOneMeasure( u, m, repunit, repsizex, outsizex );
    }
    _GONG_DEBUG_PRINT(10, Xtring::printf( "%s: posx=%f, sizex=%f", name(), posX(), sizeX() ) );
    m = stringTo<Measure>(propSizeY.getOrig() );
    if( m == Measure(0) ) {
        if( parent() )
            propSizeY = static_cast<Object *>(parent())->sizeY() - posY();
        else
            propSizeY = outsizey - posY();
    } else {
        u = extractUnits(propSizeY.getOrig(), repunit );
        m = stringTo<Measure>(propSizeY.getOrig() );
        propSizeY = fixOneMeasure( u, m, repunit, repsizey, outsizey);
    }
    u = extractUnits(propMarginLeft.getOrig(), repunit );
    m = stringTo<Measure>(propMarginLeft.getOrig() );
    propMarginLeft = fixOneMeasure( u, m, repunit, repsizex, outsizex );
    u = extractUnits(propMarginRight.getOrig(), repunit );
    m = stringTo<Measure>(propMarginRight.getOrig() );
    propMarginRight = fixOneMeasure( u, m, repunit, repsizex, outsizex );
    u = extractUnits(propMarginTop.getOrig(), repunit );
    m = stringTo<Measure>(propMarginTop.getOrig() );
    propMarginTop = fixOneMeasure( u, m, repunit, repsizey, outsizey );
    u = extractUnits(propMarginBottom.getOrig(), repunit );
    m = stringTo<Measure>(propMarginBottom.getOrig() );
    propMarginBottom = fixOneMeasure( u, m, repunit, repsizey, outsizey );
    if( isSection() ) {
        Section *sect = static_cast<Section *>(this);
        u = extractUnits(sect->propPadding.getOrig(), repunit );
        m = stringTo<Measure>(sect->propPadding.getOrig() );
        sect->propPadding = fixOneMeasure( u, m, repunit, repsizey, outsizey );
        u = extractUnits(sect->propPaddingLeft.getOrig(), repunit );
        m = stringTo<Measure>(sect->propPaddingLeft.getOrig() );
        sect->propPaddingLeft = fixOneMeasure( u, m, repunit, repsizey, outsizey );
        u = extractUnits(sect->propPaddingTop.getOrig(), repunit );
        m = stringTo<Measure>(sect->propPaddingTop.getOrig() );
        sect->propPaddingTop = fixOneMeasure( u, m, repunit, repsizey, outsizey );
    }
    if( posywasminus1 ) {
        if( prevobj )
            propPosY = prevobj->posY() + prevobj->sizeY() - posY();
        else
            propPosY = 0;
    }
    if( posxwasminus1 ) {
        if( prevobj )
            propPosX = prevobj->posX() + prevobj->sizeX() - posX();
        else
            propPosX = 0;
    } else if( posX() < Measure(0) ) {
        if( parent() ) {
            propPosX = static_cast<Object *>(parent())->sizeX() + posX();
        } else
            propPosX = outsizex - posX();
    }

}


void Object::fixParameters(const ParametersList &parameters, const char *delim)
{
	bool isvalid;
/*<<<<<OBJECT_FIXPARAMETERS*/
	if( !propValue.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propValue.getOrig(), "Value");
	if( !propFormulaBefore.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFormulaBefore.getOrig(), "FormulaBefore");
	if( !propFormulaAfter.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFormulaAfter.getOrig(), "FormulaAfter");
	if( !propAggregate.fix( parameters, delim, AggNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propAggregate.getOrig(), "Aggregate");
	if( !propPosX.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propPosX.getOrig(), "PosX");
	if( !propPosY.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propPosY.getOrig(), "PosY");
	if( !propSizeX.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propSizeX.getOrig(), "SizeX");
	if( !propSizeY.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propSizeY.getOrig(), "SizeY");
	if( !propStyle.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propStyle.getOrig(), "Style");
	if( !propUrl.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propUrl.getOrig(), "Url");
	if( !propTag.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propTag.getOrig(), "Tag");
	if( !propMarginLeft.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMarginLeft.getOrig(), "MarginLeft");
	if( !propMarginRight.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMarginRight.getOrig(), "MarginRight");
	if( !propMarginTop.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMarginTop.getOrig(), "MarginTop");
	if( !propMarginBottom.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMarginBottom.getOrig(), "MarginBottom");
	if( !propPadding.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propPadding.getOrig(), "Padding");
	if( !propPaddingLeft.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propPaddingLeft.getOrig(), "PaddingLeft");
	if( !propPaddingTop.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propPaddingTop.getOrig(), "PaddingTop");
	if( !propVisible.fix( parameters, delim, true ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propVisible.getOrig(), "Visible");
	if( !propSupressed.fix( parameters, delim, false ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propSupressed.getOrig(), "Supressed");
	if( !propSupressDup.fix( parameters, delim, false ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propSupressDup.getOrig(), "SupressDup");
	if( !propFontFamily.fix( parameters, delim, "Helvetica" ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFontFamily.getOrig(), "FontFamily");
	if( !propFontSize.fix( parameters, delim, 10 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFontSize.getOrig(), "FontSize");
	if( !propFontWeight.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFontWeight.getOrig(), "FontWeight");
	if( !propFontItalic.fix( parameters, delim, false ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFontItalic.getOrig(), "FontItalic");
	if( !propMinFontSize.fix( parameters, delim, 8 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMinFontSize.getOrig(), "MinFontSize");
	if( !propAdjustment.fix( parameters, delim, AdjustTrim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propAdjustment.getOrig(), "Adjustment");
	if( !propMaxLines.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMaxLines.getOrig(), "MaxLines");
	if( !propFormat.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFormat.getOrig(), "Format");
	if( !propMask.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propMask.getOrig(), "Mask");
	if( !propLocale.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propLocale.getOrig(), "Locale");
	if( !propHAlignment.fix( parameters, delim, AlignAuto ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propHAlignment.getOrig(), "HAlignment");
	if( !propVAlignment.fix( parameters, delim, AlignAuto ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propVAlignment.getOrig(), "VAlignment");
	if( !propFontColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propFontColor.getOrig(), "FontColor");
	if( !propBackColor.fix( parameters, delim, Color::White ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBackColor.getOrig(), "BackColor");
	if( !propBorderColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderColor.getOrig(), "BorderColor");
	if( !propBorderWidth.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderWidth.getOrig(), "BorderWidth");
	if( !propBorderStyle.fix( parameters, delim, BorderNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderStyle.getOrig(), "BorderStyle");
	if( !propBorderTopColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderTopColor.getOrig(), "BorderTopColor");
	if( !propBorderTopWidth.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderTopWidth.getOrig(), "BorderTopWidth");
	if( !propBorderTopStyle.fix( parameters, delim, BorderNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderTopStyle.getOrig(), "BorderTopStyle");
	if( !propBorderBottomColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderBottomColor.getOrig(), "BorderBottomColor");
	if( !propBorderBottomWidth.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderBottomWidth.getOrig(), "BorderBottomWidth");
	if( !propBorderBottomStyle.fix( parameters, delim, BorderNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderBottomStyle.getOrig(), "BorderBottomStyle");
	if( !propBorderLeftColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderLeftColor.getOrig(), "BorderLeftColor");
	if( !propBorderLeftWidth.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderLeftWidth.getOrig(), "BorderLeftWidth");
	if( !propBorderLeftStyle.fix( parameters, delim, BorderNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderLeftStyle.getOrig(), "BorderLeftStyle");
	if( !propBorderRightColor.fix( parameters, delim, Color::Black ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderRightColor.getOrig(), "BorderRightColor");
	if( !propBorderRightWidth.fix( parameters, delim, 0 ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderRightWidth.getOrig(), "BorderRightWidth");
	if( !propBorderRightStyle.fix( parameters, delim, BorderNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBorderRightStyle.getOrig(), "BorderRightStyle");
	if( !propBackgroundImage.fix( parameters, delim ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propBackgroundImage.getOrig(), "BackgroundImage");
	if( !propImageStyle.fix( parameters, delim, RTK::ImageNone ) )
		report()->addWarning( Error::InvalidArgument, 0, 0, propImageStyle.getOrig(), "ImageStyle");
/*>>>>>OBJECT_FIXPARAMETERS*/
	if( propFontSize.getOrig() && (*propFontSize.getOrig() == '+' || *propFontSize.getOrig() == '-' )) {
		Xtring errmsg;
		mFontSizeIncrement = stringTo<double>(propFontSize.getOrig() );
		propFontSize.clear();
	}
}

/**
 * Executes a formula for this object.
 * @param formula A string with the formula code
 * @return The value of the calculated formula
 */
Variant Object::execFormula(const Xtring &formula)
{
    Variant ret;
    if( formula.size() ) {
#ifdef HAVE_BOOST_SPIRIT
        /*
        	Syntax:
        		=return value;
        */
        Xtring code;
        if( formula[0] == '=' )
            code = formula.mid(1);
        else
            code = formula;
        code = code.trim();
        if( code.startsWith( "const " ) ) {
            code = code.mid(6);
            mIsConst = true;
        }
        if( code.find("return") == Xtring::npos ) {
            if( code.startsWith("@") )
                code = "return " + code;
            else {
                _GONG_DEBUG_WARNING("Formula " + code + " does not have a return value.");
            }
        }
        code.replace("@this", Xtring("@") + name() );
        if( !code.endsWith(";") )
            code += ";";
        _GONG_DEBUG_PRINT(4, Xtring::printf("Eval formula '%s' for object '%s': ", code.c_str(), name() ));
        dscript::value v;
        try {
            v = report()->getFormulaContext()->eval( code.c_str() );
        } catch( dscript::compiler_error &e ) {
            _GONG_DEBUG_WARNING( e.what() );
        }
        switch( v.type ) {
        case dscript::value::type_str:
            ret = Xtring(v.to_str());
            break;
        case dscript::value::type_int:
            ret = v.to_int();
            break;
        case dscript::value::type_flt:
            ret = v.to_flt();
            break;
        case dscript::value::type_date:
            ret = v.to_datetime();
            break;
        case dscript::value::type_binary:
            ret = Variant(static_cast<const void *>(v.to_str().c_str()), v.to_str().size());
            break;
        }
#endif

#if USE_CXX_FORMULA == 1
        // Function must have the following syntax = functionname ( parameter )
        int pos = 0;
        Variant value;
        Xtring funct_name;
        Xtring funct_parameter;
        bool syntax_ok = false;
        FormulaToken ft = getNextToken(code, &value, &pos);
        if( ft == FT_NAME ) {
            funct_name = value.toString();
            ft = getNextToken(code, &value, &pos);
            if( ft == FT_BRACKET ) {
                ft = getNextToken(code, &value, &pos);
                if( ft == FT_NAME || ft == FT_STRING || ft == FT_NUMBER ) {
                    funct_parameter = value.toString();
                    syntax_ok = true;
                    ft = getNextToken(code, &value, &pos);
                    if( ft != FT_BRACKET )
                        syntax_ok = false;
                } else 	if( ft == FT_BRACKET ) {
                    funct_parameter.clear();
                    syntax_ok = true;
                }
                ft = getNextToken(code, &value, &pos);
                if( ft != FT_END ) {
                    syntax_ok = false;
                }
            }
        }
        if( syntax_ok ) {
            Report::cxx_formula *function;
            if( (function = report()->findDLSymbol(funct_name.c_str())) ) {
                ret = function(this, funct_parameter.c_str() );
            }
        } else {
            report()->addWarning(Error::FormulaSyntaxError, __FUNCTION__, 0, code);
        }
#endif
        _GONG_DEBUG_PRINT(4, Xtring::printf("Result: %s (%s)",
                                            ret.toString().c_str(), ret.typeToName(ret.type()) ) );
    }
    return ret;
}

void Object::swapNonConstValues()
{
    Variant tmp(mPrevRealValue);
    mPrevRealValue = mRealValue;
    mRealValue = tmp;
    _GONG_DEBUG_PRINT(5, Xtring::printf("Swapped value for object %s, curr: %s, old: %s",
                                        name(), mRealValue.toString().c_str(), mPrevRealValue.toString().c_str() ) );
}

void Object::swapAggregateValues( uint level )
{
    _GONG_DEBUG_ASSERT( level >= 0 );
    for ( uint i = 0; i <= level; i++ ) {
        Variant tmp(mPrevAggregateValues[i]);
        mPrevAggregateValues[i] = mAggregateValues[i];
        mAggregateValues[i] = tmp;
        _GONG_DEBUG_PRINT(5, Xtring::printf("Swapped aggvalue for object %s, level %d, curr: %s, old: %s",
                                            name(), i, mAggregateValues[i].toString().c_str(), mPrevAggregateValues[i].toString().c_str() ) );
    }
}


#ifdef _GONG_DEBUG
Xtring Object::debugPrint(const Xtring &file, const Xtring &function, int line) const
{
    return file + ":" + Xtring::number(line) + ':' + function + ": ";
}
#endif


double Object::realFontSize() const
{
	double fontsize = fontSize();
	if( mFontSizeIncrement != 0.0 )
		fontsize += mFontSizeIncrement;
	if( fontsize < minFontSize() )
		fontsize = minFontSize();
	return fontsize;
}


/*<<<<<OBJECT_Visible_GET*/
bool Object::visible() const
{
	if( propVisible.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propVisible.isNull()) {
			return styledef->propVisible.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->visible();
		}
	}
	return propVisible.get();
}
/*>>>>>OBJECT_Visible_GET*/

/*<<<<<OBJECT_Supressed_GET*/
bool Object::supressed() const
{
	if( propSupressed.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propSupressed.isNull()) {
			return styledef->propSupressed.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->supressed();
		}
	}
	return propSupressed.get();
}
/*>>>>>OBJECT_Supressed_GET*/
/*<<<<<OBJECT_FontFamily_GET*/
const char * Object::fontFamily() const
{
	if( propFontFamily.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFontFamily.isNull()) {
			return styledef->propFontFamily.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->fontFamily();
		}
	}
	return propFontFamily.get();
}
/*>>>>>OBJECT_FontFamily_GET*/
/*<<<<<OBJECT_FontSize_GET*/
double Object::fontSize() const
{
	if( propFontSize.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFontSize.isNull()) {
			return styledef->propFontSize.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->fontSize();
		}
	}
	return propFontSize.get();
}
/*>>>>>OBJECT_FontSize_GET*/
/*<<<<<OBJECT_FontWeight_GET*/
int Object::fontWeight() const
{
	if( propFontWeight.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFontWeight.isNull()) {
			return styledef->propFontWeight.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->fontWeight();
		}
	}
	return propFontWeight.get();
}
/*>>>>>OBJECT_FontWeight_GET*/
/*<<<<<OBJECT_FontItalic_GET*/
bool Object::fontItalic() const
{
	if( propFontItalic.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFontItalic.isNull()) {
			return styledef->propFontItalic.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->fontItalic();
		}
	}
	return propFontItalic.get();
}
/*>>>>>OBJECT_FontItalic_GET*/
/*<<<<<OBJECT_MinFontSize_GET*/
double Object::minFontSize() const
{
	if( propMinFontSize.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propMinFontSize.isNull()) {
			return styledef->propMinFontSize.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->minFontSize();
		}
	}
	return propMinFontSize.get();
}
/*>>>>>OBJECT_MinFontSize_GET*/
/*<<<<<OBJECT_Adjustment_GET*/
Adjustment Object::adjustment() const
{
	if( propAdjustment.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propAdjustment.isNull()) {
			return styledef->propAdjustment.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->adjustment();
		}
	}
	return propAdjustment.get();
}
/*>>>>>OBJECT_Adjustment_GET*/
/*<<<<<OBJECT_MaxLines_GET*/
int Object::maxLines() const
{
	if( propMaxLines.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propMaxLines.isNull()) {
			return styledef->propMaxLines.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->maxLines();
		}
	}
	return propMaxLines.get();
}
/*>>>>>OBJECT_MaxLines_GET*/
/*<<<<<OBJECT_HAlignment_GET*/
Alignment Object::hAlignment() const
{
	if( propHAlignment.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propHAlignment.isNull()) {
			return styledef->propHAlignment.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->hAlignment();
		}
	}
	return propHAlignment.get();
}
/*>>>>>OBJECT_HAlignment_GET*/
/*<<<<<OBJECT_VAlignment_GET*/
Alignment Object::vAlignment() const
{
	if( propVAlignment.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propVAlignment.isNull()) {
			return styledef->propVAlignment.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->vAlignment();
		}
	}
	return propVAlignment.get();
}
/*>>>>>OBJECT_VAlignment_GET*/
/*<<<<<OBJECT_FontColor_GET*/
Color Object::fontColor() const
{
	if( propFontColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFontColor.isNull()) {
			return styledef->propFontColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->fontColor();
		}
	}
	return propFontColor.get();
}
/*>>>>>OBJECT_FontColor_GET*/
/*<<<<<OBJECT_BackColor_GET*/
Color Object::backColor() const
{
	if( propBackColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBackColor.isNull()) {
			return styledef->propBackColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->backColor();
		}
	}
	return propBackColor.get();
}
/*>>>>>OBJECT_BackColor_GET*/
/*<<<<<OBJECT_BorderColor_GET*/
Color Object::borderColor() const
{
	if( propBorderColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderColor.isNull()) {
			return styledef->propBorderColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderColor();
		}
	}
	return propBorderColor.get();
}
/*>>>>>OBJECT_BorderColor_GET*/
/*<<<<<OBJECT_BorderWidth_GET*/
int Object::borderWidth() const
{
	if( propBorderWidth.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderWidth.isNull()) {
			return styledef->propBorderWidth.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderWidth();
		}
	}
	return propBorderWidth.get();
}
/*>>>>>OBJECT_BorderWidth_GET*/
/*<<<<<OBJECT_BorderTopColor_GET*/
Color Object::borderTopColor() const
{
	if( propBorderTopColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderTopColor.isNull()) {
			return styledef->propBorderTopColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderTopColor();
		}
	}
	return propBorderTopColor.get();
}
/*>>>>>OBJECT_BorderTopColor_GET*/
/*<<<<<OBJECT_BorderTopWidth_GET*/
int Object::borderTopWidth() const
{
	if( propBorderTopWidth.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderTopWidth.isNull()) {
			return styledef->propBorderTopWidth.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderTopWidth();
		}
	}
	return propBorderTopWidth.get();
}
/*>>>>>OBJECT_BorderTopWidth_GET*/
/*<<<<<OBJECT_BorderBottomColor_GET*/
Color Object::borderBottomColor() const
{
	if( propBorderBottomColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderBottomColor.isNull()) {
			return styledef->propBorderBottomColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderBottomColor();
		}
	}
	return propBorderBottomColor.get();
}
/*>>>>>OBJECT_BorderBottomColor_GET*/
/*<<<<<OBJECT_BorderBottomWidth_GET*/
int Object::borderBottomWidth() const
{
	if( propBorderBottomWidth.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderBottomWidth.isNull()) {
			return styledef->propBorderBottomWidth.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderBottomWidth();
		}
	}
	return propBorderBottomWidth.get();
}
/*>>>>>OBJECT_BorderBottomWidth_GET*/
/*<<<<<OBJECT_BorderLeftColor_GET*/
Color Object::borderLeftColor() const
{
	if( propBorderLeftColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderLeftColor.isNull()) {
			return styledef->propBorderLeftColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderLeftColor();
		}
	}
	return propBorderLeftColor.get();
}
/*>>>>>OBJECT_BorderLeftColor_GET*/
/*<<<<<OBJECT_BorderLeftWidth_GET*/
int Object::borderLeftWidth() const
{
	if( propBorderLeftWidth.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderLeftWidth.isNull()) {
			return styledef->propBorderLeftWidth.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderLeftWidth();
		}
	}
	return propBorderLeftWidth.get();
}
/*>>>>>OBJECT_BorderLeftWidth_GET*/
/*<<<<<OBJECT_BorderRightColor_GET*/
Color Object::borderRightColor() const
{
	if( propBorderRightColor.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderRightColor.isNull()) {
			return styledef->propBorderRightColor.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderRightColor();
		}
	}
	return propBorderRightColor.get();
}
/*>>>>>OBJECT_BorderRightColor_GET*/
/*<<<<<OBJECT_BorderRightWidth_GET*/
int Object::borderRightWidth() const
{
	if( propBorderRightWidth.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderRightWidth.isNull()) {
			return styledef->propBorderRightWidth.get();
		}
		if( parent() ) {
			Object *obj = static_cast<Object *>(parent());
			return obj->borderRightWidth();
		}
	}
	return propBorderRightWidth.get();
}
/*>>>>>OBJECT_BorderRightWidth_GET*/


/*<<<<<OBJECT_Format_GET*/
const char * Object::format() const
{
	if( propFormat.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propFormat.isNull()) {
			return styledef->propFormat.get();
		}
	}
	return propFormat.get();
}
/*>>>>>OBJECT_Format_GET*/
/*<<<<<OBJECT_Locale_GET*/
const char * Object::locale() const
{
	if( propLocale.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propLocale.isNull()) {
			return styledef->propLocale.get();
		}
	}
	return propLocale.get();
}
/*>>>>>OBJECT_Locale_GET*/
/*<<<<<OBJECT_BorderStyle_GET*/
BorderStyle Object::borderStyle() const
{
	if( propBorderStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderStyle.isNull()) {
			return styledef->propBorderStyle.get();
		}
	}
	return propBorderStyle.get();
}
/*>>>>>OBJECT_BorderStyle_GET*/
/*<<<<<OBJECT_BorderTopStyle_GET*/
BorderStyle Object::borderTopStyle() const
{
	if( propBorderTopStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderTopStyle.isNull()) {
			return styledef->propBorderTopStyle.get();
		}
	}
	return propBorderTopStyle.get();
}
/*>>>>>OBJECT_BorderTopStyle_GET*/
/*<<<<<OBJECT_BorderBottomStyle_GET*/
BorderStyle Object::borderBottomStyle() const
{
	if( propBorderBottomStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderBottomStyle.isNull()) {
			return styledef->propBorderBottomStyle.get();
		}
	}
	return propBorderBottomStyle.get();
}
/*>>>>>OBJECT_BorderBottomStyle_GET*/
/*<<<<<OBJECT_BorderLeftStyle_GET*/
BorderStyle Object::borderLeftStyle() const
{
	if( propBorderLeftStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderLeftStyle.isNull()) {
			return styledef->propBorderLeftStyle.get();
		}
	}
	return propBorderLeftStyle.get();
}
/*>>>>>OBJECT_BorderLeftStyle_GET*/
/*<<<<<OBJECT_BorderRightStyle_GET*/
BorderStyle Object::borderRightStyle() const
{
	if( propBorderRightStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBorderRightStyle.isNull()) {
			return styledef->propBorderRightStyle.get();
		}
	}
	return propBorderRightStyle.get();
}
/*>>>>>OBJECT_BorderRightStyle_GET*/
/*<<<<<OBJECT_BackgroundImage_GET*/
const char * Object::backgroundImage() const
{
	if( propBackgroundImage.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propBackgroundImage.isNull()) {
			return styledef->propBackgroundImage.get();
		}
	}
	return propBackgroundImage.get();
}
/*>>>>>OBJECT_BackgroundImage_GET*/

/*<<<<<OBJECT_SupressDup_GET*/
bool Object::supressDup() const
{
	if( propSupressDup.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propSupressDup.isNull()) {
			return styledef->propSupressDup.get();
		}
	}
	return propSupressDup.get();
}
/*>>>>>OBJECT_SupressDup_GET*/

/*<<<<<OBJECT_ImageStyle_GET*/
ImageStyle Object::imageStyle() const
{
	if( propImageStyle.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propImageStyle.isNull()) {
			return styledef->propImageStyle.get();
		}
	}
	return propImageStyle.get();
}
/*>>>>>OBJECT_ImageStyle_GET*/

/*<<<<<OBJECT_Mask_GET*/
const char * Object::mask() const
{
	if( propMask.isNull() ) {
		Style *styledef = report()->findStyle(style());
		if( styledef && !styledef->propMask.isNull()) {
			return styledef->propMask.get();
		}
	}
	return propMask.get();
}
/*>>>>>OBJECT_Mask_GET*/

} // namespace RTK


