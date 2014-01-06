#ifndef RTKSTYLE_H
#define RTKSTYLE_H

/*<<<<<COPYLEFT*/
/** @file rtkstyle.h RTK style object
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

class Style {

    friend class Object;
    friend class Section;

public:
    Style(const char *name = 0)
        : propName(name) {}
    virtual ~Style() {}

    /*<<<<<STYLE_PROPERTIES*/
public:
	const char * name() const { return propName.get(); }
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
	bool visible() const { return propVisible.get(); }
	void setOrigVisible(const char * visible )		{ propVisible.setOrig(visible); }
	bool supressed() const { return propSupressed.get(); }
	void setOrigSupressed(const char * supressed )		{ propSupressed.setOrig(supressed); }
	bool supressDup() const { return propSupressDup.get(); }
	void setOrigSupressDup(const char * supressdup )		{ propSupressDup.setOrig(supressdup); }
	const char * fontFamily() const { return propFontFamily.get(); }
	void setOrigFontFamily(const char * fontfamily )		{ propFontFamily.setOrig(fontfamily); }
	double fontSize() const { return propFontSize.get(); }
	void setOrigFontSize(const char * fontsize )		{ propFontSize.setOrig(fontsize); }
	int fontWeight() const { return propFontWeight.get(); }
	void setOrigFontWeight(const char * fontweight )		{ propFontWeight.setOrig(fontweight); }
	bool fontItalic() const { return propFontItalic.get(); }
	void setOrigFontItalic(const char * fontitalic )		{ propFontItalic.setOrig(fontitalic); }
	int maxLines() const { return propMaxLines.get(); }
	void setOrigMaxLines(const char * maxlines )		{ propMaxLines.setOrig(maxlines); }
	double minFontSize() const { return propMinFontSize.get(); }
	void setOrigMinFontSize(const char * minfontsize )		{ propMinFontSize.setOrig(minfontsize); }
	const char * format() const { return propFormat.get(); }
	void setOrigFormat(const char * format )		{ propFormat.setOrig(format); }
	const char * mask() const { return propMask.get(); }
	void setOrigMask(const char * mask )		{ propMask.setOrig(mask); }
	const char * locale() const { return propLocale.get(); }
	void setOrigLocale(const char * locale )		{ propLocale.setOrig(locale); }
	Alignment hAlignment() const { return propHAlignment.get(); }
	void setOrigHAlignment(const char * halignment )		{ propHAlignment.setOrig(halignment); }
	Alignment vAlignment() const { return propVAlignment.get(); }
	void setOrigVAlignment(const char * valignment )		{ propVAlignment.setOrig(valignment); }
	Adjustment adjustment() const { return propAdjustment.get(); }
	void setOrigAdjustment(const char * adjustment )		{ propAdjustment.setOrig(adjustment); }
	Color fontColor() const { return propFontColor.get(); }
	void setOrigFontColor(const char * fontcolor )		{ propFontColor.setOrig(fontcolor); }
	Color backColor() const { return propBackColor.get(); }
	void setOrigBackColor(const char * backcolor )		{ propBackColor.setOrig(backcolor); }
	Color borderColor() const { return propBorderColor.get(); }
	void setOrigBorderColor(const char * bordercolor )		{ propBorderColor.setOrig(bordercolor); }
	int borderWidth() const { return propBorderWidth.get(); }
	void setOrigBorderWidth(const char * borderwidth )		{ propBorderWidth.setOrig(borderwidth); }
	BorderStyle borderStyle() const { return propBorderStyle.get(); }
	void setOrigBorderStyle(const char * borderstyle )		{ propBorderStyle.setOrig(borderstyle); }
	Color borderTopColor() const { return propBorderTopColor.get(); }
	void setOrigBorderTopColor(const char * bordertopcolor )		{ propBorderTopColor.setOrig(bordertopcolor); }
	int borderTopWidth() const { return propBorderTopWidth.get(); }
	void setOrigBorderTopWidth(const char * bordertopwidth )		{ propBorderTopWidth.setOrig(bordertopwidth); }
	BorderStyle borderTopStyle() const { return propBorderTopStyle.get(); }
	void setOrigBorderTopStyle(const char * bordertopstyle )		{ propBorderTopStyle.setOrig(bordertopstyle); }
	Color borderBottomColor() const { return propBorderBottomColor.get(); }
	void setOrigBorderBottomColor(const char * borderbottomcolor )		{ propBorderBottomColor.setOrig(borderbottomcolor); }
	int borderBottomWidth() const { return propBorderBottomWidth.get(); }
	void setOrigBorderBottomWidth(const char * borderbottomwidth )		{ propBorderBottomWidth.setOrig(borderbottomwidth); }
	BorderStyle borderBottomStyle() const { return propBorderBottomStyle.get(); }
	void setOrigBorderBottomStyle(const char * borderbottomstyle )		{ propBorderBottomStyle.setOrig(borderbottomstyle); }
	Color borderLeftColor() const { return propBorderLeftColor.get(); }
	void setOrigBorderLeftColor(const char * borderleftcolor )		{ propBorderLeftColor.setOrig(borderleftcolor); }
	int borderLeftWidth() const { return propBorderLeftWidth.get(); }
	void setOrigBorderLeftWidth(const char * borderleftwidth )		{ propBorderLeftWidth.setOrig(borderleftwidth); }
	BorderStyle borderLeftStyle() const { return propBorderLeftStyle.get(); }
	void setOrigBorderLeftStyle(const char * borderleftstyle )		{ propBorderLeftStyle.setOrig(borderleftstyle); }
	Color borderRightColor() const { return propBorderRightColor.get(); }
	void setOrigBorderRightColor(const char * borderrightcolor )		{ propBorderRightColor.setOrig(borderrightcolor); }
	int borderRightWidth() const { return propBorderRightWidth.get(); }
	void setOrigBorderRightWidth(const char * borderrightwidth )		{ propBorderRightWidth.setOrig(borderrightwidth); }
	BorderStyle borderRightStyle() const { return propBorderRightStyle.get(); }
	void setOrigBorderRightStyle(const char * borderrightstyle )		{ propBorderRightStyle.setOrig(borderrightstyle); }
	const char * backgroundImage() const { return propBackgroundImage.get(); }
	void setOrigBackgroundImage(const char * backgroundimage )		{ propBackgroundImage.setOrig(backgroundimage); }
	ImageStyle imageStyle() const { return propImageStyle.get(); }
	void setOrigImageStyle(const char * imagestyle )		{ propImageStyle.setOrig(imagestyle); }
	bool pageBefore() const { return propPageBefore.get(); }
	void setOrigPageBefore(const char * pagebefore )		{ propPageBefore.setOrig(pagebefore); }
	bool pageAfter() const { return propPageAfter.get(); }
	void setOrigPageAfter(const char * pageafter )		{ propPageAfter.setOrig(pageafter); }
	bool resetNPage() const { return propResetNPage.get(); }
	void setOrigResetNPage(const char * resetnpage )		{ propResetNPage.setOrig(resetnpage); }
	bool supIfBlank() const { return propSupIfBlank.get(); }
	void setOrigSupIfBlank(const char * supifblank )		{ propSupIfBlank.setOrig(supifblank); }
	bool repeatHeader() const { return propRepeatHeader.get(); }
	void setOrigRepeatHeader(const char * repeatheader )		{ propRepeatHeader.setOrig(repeatheader); }
	bool printInFirstPage() const { return propPrintInFirstPage.get(); }
	void setOrigPrintInFirstPage(const char * printinfirstpage )		{ propPrintInFirstPage.setOrig(printinfirstpage); }
	virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
	ReadOnlyProperty<const char *> propName;
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
	FixableProperty<int> propMaxLines;
	FixableProperty<double> propMinFontSize;
	FixableProperty<const char *> propFormat;
	FixableProperty<const char *> propMask;
	FixableProperty<const char *> propLocale;
	FixableProperty<Alignment> propHAlignment;
	FixableProperty<Alignment> propVAlignment;
	FixableProperty<Adjustment> propAdjustment;
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
	FixableProperty<bool> propPageBefore;
	FixableProperty<bool> propPageAfter;
	FixableProperty<bool> propResetNPage;
	FixableProperty<bool> propSupIfBlank;
	FixableProperty<bool> propRepeatHeader;
	FixableProperty<bool> propPrintInFirstPage;
/*>>>>>STYLE_PROPERTIES*/
};

} // namespace

#endif
