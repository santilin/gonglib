#ifndef RTKOBJECT_H
#define RTKOBJECT_H

/*<<<<<COPYLEFT*/
/** @file rtkobject.h Base RTK object
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
#include <gongvariant.h>
#include "rtkparameter.h"
#include "rtkproperty.h"
#include "rtkstyle.h"
#include "rtkimage.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class Object
{
public:
    friend class TestObject;
    friend class Report;
    friend class Section;

    Object( Object *parent, const char *name = 0);
    virtual ~Object();
    class Report *report() const;
    class Object *parent() const {
            return pParent;
        }
    bool isSection() const;
    bool isReport() const;
    bool isObject() const;
    bool isConst() const {
        return mIsConst;
    }

    Xtring text() const;
    Xtring formattedText() const {
        return mFormattedText;
    }
    Variant realValue() const;

    void setName(const char *newname) {
        propName.set(newname);
    }
    void setFontColor(const Color &color) {
        propFontColor.set(color);
    }
    void setBackColor(const Color &color) {
        propBackColor.set(color);
    }
    /* Gets an object's property value by name */
    virtual Variant getPropValue(const Xtring &propname) const;
    /* Sets an object's property working value by name */
    virtual bool setPropValue(const Xtring &propname, const Variant &value);
    /* Sets an object's property original value by name */
    bool setPropOrigValue(const Xtring &propname, const Xtring &value);
    Variant getAggregateValue(uint level);

	double realFontSize() const;
private: // use realFontSize instead
    double fontSize() const;
//{capel} remove fontSize()
/*<<<<<OBJECT_PROPERTIES*/
public:
	const char * name() const { return propName.get(); }
	const char * value() const { return propValue.get(); }
	void setOrigValue(const char * value )		{ propValue.setOrig(value); }
	const char * formulaBefore() const { return propFormulaBefore.get(); }
	void setOrigFormulaBefore(const char * formulabefore )		{ propFormulaBefore.setOrig(formulabefore); }
	const char * formulaAfter() const { return propFormulaAfter.get(); }
	void setOrigFormulaAfter(const char * formulaafter )		{ propFormulaAfter.setOrig(formulaafter); }
	AggregateType aggregate() const { return propAggregate.get(); }
	void setOrigAggregate(const char * aggregate )		{ propAggregate.setOrig(aggregate); }
	Measure posX() const { return propPosX.get(); }
	void setOrigPosX(const char * posx )		{ propPosX.setOrig(posx); }
	Measure posY() const { return propPosY.get(); }
	void setOrigPosY(const char * posy )		{ propPosY.setOrig(posy); }
	Measure sizeX() const { return propSizeX.get(); }
	void setOrigSizeX(const char * sizex )		{ propSizeX.setOrig(sizex); }
	Measure sizeY() const { return propSizeY.get(); }
	void setOrigSizeY(const char * sizey )		{ propSizeY.setOrig(sizey); }
	const char * style() const { return propStyle.get(); }
	void setOrigStyle(const char * style )		{ propStyle.setOrig(style); }
	const char * url() const { return propUrl.get(); }
	void setOrigUrl(const char * url )		{ propUrl.setOrig(url); }
	const char * tag() const { return propTag.get(); }
	void setOrigTag(const char * tag )		{ propTag.setOrig(tag); }
	Measure marginLeft() const { return propMarginLeft.get(); }
	void setOrigMarginLeft(const char * marginleft )		{ propMarginLeft.setOrig(marginleft); }
	Measure marginRight() const { return propMarginRight.get(); }
	void setOrigMarginRight(const char * marginright )		{ propMarginRight.setOrig(marginright); }
	Measure marginTop() const { return propMarginTop.get(); }
	void setOrigMarginTop(const char * margintop )		{ propMarginTop.setOrig(margintop); }
	Measure marginBottom() const { return propMarginBottom.get(); }
	void setOrigMarginBottom(const char * marginbottom )		{ propMarginBottom.setOrig(marginbottom); }
	Measure padding() const { return propPadding.get(); }
	void setOrigPadding(const char * padding )		{ propPadding.setOrig(padding); }
	Measure paddingLeft() const { return propPaddingLeft.get(); }
	void setOrigPaddingLeft(const char * paddingleft )		{ propPaddingLeft.setOrig(paddingleft); }
	Measure paddingTop() const { return propPaddingTop.get(); }
	void setOrigPaddingTop(const char * paddingtop )		{ propPaddingTop.setOrig(paddingtop); }
	bool visible() const;
	void setOrigVisible(const char * visible )		{ propVisible.setOrig(visible); }
	bool supressed() const;
	void setOrigSupressed(const char * supressed )		{ propSupressed.setOrig(supressed); }
	bool supressDup() const;
	void setOrigSupressDup(const char * supressdup )		{ propSupressDup.setOrig(supressdup); }
	const char * fontFamily() const;
	void setOrigFontFamily(const char * fontfamily )		{ propFontFamily.setOrig(fontfamily); }
	void setOrigFontSize(const char * fontsize )		{ propFontSize.setOrig(fontsize); }
	int fontWeight() const;
	void setOrigFontWeight(const char * fontweight )		{ propFontWeight.setOrig(fontweight); }
	bool fontItalic() const;
	void setOrigFontItalic(const char * fontitalic )		{ propFontItalic.setOrig(fontitalic); }
	double minFontSize() const;
	void setOrigMinFontSize(const char * minfontsize )		{ propMinFontSize.setOrig(minfontsize); }
	Adjustment adjustment() const;
	void setOrigAdjustment(const char * adjustment )		{ propAdjustment.setOrig(adjustment); }
	int maxLines() const;
	void setOrigMaxLines(const char * maxlines )		{ propMaxLines.setOrig(maxlines); }
	const char * format() const;
	void setOrigFormat(const char * format )		{ propFormat.setOrig(format); }
	const char * mask() const;
	void setOrigMask(const char * mask )		{ propMask.setOrig(mask); }
	const char * locale() const;
	void setOrigLocale(const char * locale )		{ propLocale.setOrig(locale); }
	Alignment hAlignment() const;
	void setOrigHAlignment(const char * halignment )		{ propHAlignment.setOrig(halignment); }
	Alignment vAlignment() const;
	void setOrigVAlignment(const char * valignment )		{ propVAlignment.setOrig(valignment); }
	Color fontColor() const;
	void setOrigFontColor(const char * fontcolor )		{ propFontColor.setOrig(fontcolor); }
	Color backColor() const;
	void setOrigBackColor(const char * backcolor )		{ propBackColor.setOrig(backcolor); }
	Color borderColor() const;
	void setOrigBorderColor(const char * bordercolor )		{ propBorderColor.setOrig(bordercolor); }
	int borderWidth() const;
	void setOrigBorderWidth(const char * borderwidth )		{ propBorderWidth.setOrig(borderwidth); }
	BorderStyle borderStyle() const;
	void setOrigBorderStyle(const char * borderstyle )		{ propBorderStyle.setOrig(borderstyle); }
	Color borderTopColor() const;
	void setOrigBorderTopColor(const char * bordertopcolor )		{ propBorderTopColor.setOrig(bordertopcolor); }
	int borderTopWidth() const;
	void setOrigBorderTopWidth(const char * bordertopwidth )		{ propBorderTopWidth.setOrig(bordertopwidth); }
	BorderStyle borderTopStyle() const;
	void setOrigBorderTopStyle(const char * bordertopstyle )		{ propBorderTopStyle.setOrig(bordertopstyle); }
	Color borderBottomColor() const;
	void setOrigBorderBottomColor(const char * borderbottomcolor )		{ propBorderBottomColor.setOrig(borderbottomcolor); }
	int borderBottomWidth() const;
	void setOrigBorderBottomWidth(const char * borderbottomwidth )		{ propBorderBottomWidth.setOrig(borderbottomwidth); }
	BorderStyle borderBottomStyle() const;
	void setOrigBorderBottomStyle(const char * borderbottomstyle )		{ propBorderBottomStyle.setOrig(borderbottomstyle); }
	Color borderLeftColor() const;
	void setOrigBorderLeftColor(const char * borderleftcolor )		{ propBorderLeftColor.setOrig(borderleftcolor); }
	int borderLeftWidth() const;
	void setOrigBorderLeftWidth(const char * borderleftwidth )		{ propBorderLeftWidth.setOrig(borderleftwidth); }
	BorderStyle borderLeftStyle() const;
	void setOrigBorderLeftStyle(const char * borderleftstyle )		{ propBorderLeftStyle.setOrig(borderleftstyle); }
	Color borderRightColor() const;
	void setOrigBorderRightColor(const char * borderrightcolor )		{ propBorderRightColor.setOrig(borderrightcolor); }
	int borderRightWidth() const;
	void setOrigBorderRightWidth(const char * borderrightwidth )		{ propBorderRightWidth.setOrig(borderrightwidth); }
	BorderStyle borderRightStyle() const;
	void setOrigBorderRightStyle(const char * borderrightstyle )		{ propBorderRightStyle.setOrig(borderrightstyle); }
	const char * backgroundImage() const;
	void setOrigBackgroundImage(const char * backgroundimage )		{ propBackgroundImage.setOrig(backgroundimage); }
	ImageStyle imageStyle() const;
	void setOrigImageStyle(const char * imagestyle )		{ propImageStyle.setOrig(imagestyle); }
	virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
	Property<const char *> propName;
	FixableProperty<const char *> propValue;
	FixableProperty<const char *> propFormulaBefore;
	FixableProperty<const char *> propFormulaAfter;
	FixableProperty<AggregateType> propAggregate;
	FixableProperty<Measure> propPosX;
	FixableProperty<Measure> propPosY;
	FixableProperty<Measure> propSizeX;
	FixableProperty<Measure> propSizeY;
	FixableProperty<const char *> propStyle;
	FixableProperty<const char *> propUrl;
	FixableProperty<const char *> propTag;
	FixableProperty<Measure> propMarginLeft;
	FixableProperty<Measure> propMarginRight;
	FixableProperty<Measure> propMarginTop;
	FixableProperty<Measure> propMarginBottom;
	FixableProperty<Measure> propPadding;
	FixableProperty<Measure> propPaddingLeft;
	FixableProperty<Measure> propPaddingTop;
	FixableProperty<bool> propVisible;
	FixableProperty<bool> propSupressed;
	FixableProperty<bool> propSupressDup;
	FixableProperty<const char *> propFontFamily;
	FixableProperty<double> propFontSize;
	FixableProperty<int> propFontWeight;
	FixableProperty<bool> propFontItalic;
	FixableProperty<double> propMinFontSize;
	FixableProperty<Adjustment> propAdjustment;
	FixableProperty<int> propMaxLines;
	FixableProperty<const char *> propFormat;
	FixableProperty<const char *> propMask;
	FixableProperty<const char *> propLocale;
	FixableProperty<Alignment> propHAlignment;
	FixableProperty<Alignment> propVAlignment;
	FixableProperty<Color> propFontColor;
	FixableProperty<Color> propBackColor;
	FixableProperty<Color> propBorderColor;
	FixableProperty<int> propBorderWidth;
	FixableProperty<BorderStyle> propBorderStyle;
	FixableProperty<Color> propBorderTopColor;
	FixableProperty<int> propBorderTopWidth;
	FixableProperty<BorderStyle> propBorderTopStyle;
	FixableProperty<Color> propBorderBottomColor;
	FixableProperty<int> propBorderBottomWidth;
	FixableProperty<BorderStyle> propBorderBottomStyle;
	FixableProperty<Color> propBorderLeftColor;
	FixableProperty<int> propBorderLeftWidth;
	FixableProperty<BorderStyle> propBorderLeftStyle;
	FixableProperty<Color> propBorderRightColor;
	FixableProperty<int> propBorderRightWidth;
	FixableProperty<BorderStyle> propBorderRightStyle;
	FixableProperty<const char *> propBackgroundImage;
	FixableProperty<ImageStyle> propImageStyle;
/*>>>>>OBJECT_PROPERTIES*/

private:
    void setRealValue(Variant value) {
        mPrevRealValue = mRealValue;
        mRealValue = value;
    }
    void swapNonConstValues();
    void swapAggregateValues( uint level );
    void fixProperties(const class Output *out);
    void fixMeasures(Measure outsizex, Measure outsizey,
                     UnitsType repunit, Measure repsizex, Measure repsizey,
                     Object *prevobj=0 );
    bool setConst(bool isconst) {
        return (mIsConst = isconst);
    }
    int inputFieldNumber() const {
        return mInputFieldNumber;
    }
    void setInputSourcePos(int sourcepos) {
        mInputFieldNumber = sourcepos;
    }
    void formatText();
    bool formattedValueChanged() const;
    bool realValueChanged() const {
        return ( mPrevRealValue != mRealValue );
    }
    bool valueChanged() const {
        // If the previous value is invalid, this is the first value assigned, so no changed.
        return ( mPrevRealValue.isValid() && mPrevRealValue != mRealValue );
    }
public: // for tests only
    Variant execFormula(const Xtring &formula);
private:

    class Object *pParent;
    const char *pName;
    Variant mRealValue, mPrevRealValue;
    // One aggregate value for each group level. The index is level-1, as there is no group level 0
    std::vector<Variant> mAggregateValues, mPrevAggregateValues;
    bool mIsConst;         // Whether the value is const
    int mInputFieldNumber; // If the value is an input field
    Xtring mFormattedText;
    Xtring mPrevFormattedText;
	double mFontSizeIncrement;

#ifdef _GONG_DEBUG
protected:
    virtual Xtring debugPrint(const Xtring &file, const Xtring &function, int nline) const;
#endif
};

} // namespace

#endif
