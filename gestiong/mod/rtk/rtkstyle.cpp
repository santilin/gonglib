#include "rtkstyle.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
  @class Style
  @brief Styles define a common set of properties that can be applied to any object, section or the report itself.

The definition of styles allow the customization of the appearance of the report. They are assigned to
objects via the "style" property:

\code
Style aStyle = new Style("heading");
aStyle->setFontItalic(true);
aStyle->setFontFamily("Courier");
aStyle->setAlignment(Qt::AlignCenter);

Object aText = new Object(aReport, "atext");
aText->setStyle("heading");
\endcode

When an object has a style assigned, it takes the values of the properties of that style.
However, if an object defines the value for a property, it takes precedence over the style.

\code
aText->setFontWeight(50);
aText->setFontItalic(false); // Overrides the value of the style FontItalic value
\endcode

When an object has not defined a property, RTK looks for that property in the style of the object.
If the object has not defined a style or the style has not defined that property, RTK looks for that
property in its parent object, which follows the same rule. If there is no parent, RTK either looks
for that property in the Output asigned to the report, or provides a default value.

This means that the objects inherit the properties from their parents. So,
if you assign some properties to a section, they will be the default properties for all the objects
in that section.


 */



void Style::fixParameters(const ParametersList &parameters, const char *delim)
{
    /*<<<<<STYLE_FIXPARAMETERS*/
    propVisible.fix( parameters, delim, true );
    propSupressed.fix( parameters, delim, false );
    propSupressDup.fix( parameters, delim, false );
    propFontFamily.fix( parameters, delim, "Helvetica" );
    propFontSize.fix( parameters, delim, 10 );
    propFontWeight.fix( parameters, delim, 0 );
    propFontItalic.fix( parameters, delim, false );
    propMaxLines.fix( parameters, delim, 0 );
    propMinFontSize.fix( parameters, delim, 8 );
    propFormat.fix( parameters, delim );
    propMask.fix( parameters, delim );
    propLocale.fix( parameters, delim );
    propHAlignment.fix( parameters, delim, AlignAuto );
    propVAlignment.fix( parameters, delim, AlignAuto );
    propAdjustment.fix( parameters, delim, AdjustTrim );
    propFontColor.fix( parameters, delim, Color::Black );
    propBackColor.fix( parameters, delim, Color::White );
    propBorderColor.fix( parameters, delim, Color::Black );
    propBorderWidth.fix( parameters, delim, 0 );
    propBorderStyle.fix( parameters, delim, BorderNone );
    propBorderTopColor.fix( parameters, delim, Color::Black );
    propBorderTopWidth.fix( parameters, delim, 0 );
    propBorderTopStyle.fix( parameters, delim, BorderNone );
    propBorderBottomColor.fix( parameters, delim, Color::Black );
    propBorderBottomWidth.fix( parameters, delim, 0 );
    propBorderBottomStyle.fix( parameters, delim, BorderNone );
    propBorderLeftColor.fix( parameters, delim, Color::Black );
    propBorderLeftWidth.fix( parameters, delim, 0 );
    propBorderLeftStyle.fix( parameters, delim, BorderNone );
    propBorderRightColor.fix( parameters, delim, Color::Black );
    propBorderRightWidth.fix( parameters, delim, 0 );
    propBorderRightStyle.fix( parameters, delim, BorderNone );
    propBackgroundImage.fix( parameters, delim );
    propImageStyle.fix( parameters, delim, RTK::ImageNone );
    propPageBefore.fix( parameters, delim, false );
    propPageAfter.fix( parameters, delim, false );
    propResetNPage.fix( parameters, delim, false );
    propSupIfBlank.fix( parameters, delim, false );
    propRepeatHeader.fix( parameters, delim, false );
    propPrintInFirstPage.fix( parameters, delim, true );
    propPadding.fix( parameters, delim, 0 );
    propPaddingLeft.fix( parameters, delim, 0 );
    propPaddingTop.fix( parameters, delim, 0 );
    /*>>>>>STYLE_FIXPARAMETERS*/
}

} // namespace RTK
