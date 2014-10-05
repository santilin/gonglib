#include <cmath>  // Debian, round()
#include <cerrno>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <qimage.h>
#include <qpixmap.h>

#include <gongdebug.h>
#include <gongxtring.h>
#include <gongfileutils.h>
#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkimage.h"
#include "dbappreportqtoutput.h"
#include "dbappdbapplication.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

void ReportQtOutput::initReportQtOutput()
{
    QPaintDevice *dev = mPainter.device();
    setUnitsPerInchX(dev->x11AppDpiX());
    setUnitsPerInchY(dev->x11AppDpiY());
}


int ReportQtOutput::startReport()
{
    Output::startReport();
    mPageCollection.clear();
    return 0;
}

int ReportQtOutput::endReport()
{
    Output::endReport();
    if( mPainter.isActive() )
        mPainter.end();
    return 0;
}

Measure ReportQtOutput::startPage()
{
    Output::startPage();
    if( mCurrentPageInFolio <= 1 ) {
        QPicture *aPicture = new QPicture();
#ifdef _GONG_DEBUG
        _GONG_DEBUG_PRINT(0, Xtring::printf("Page: %f x %f, margins: %f, %f, %f, %f",
                                            sizeX(), sizeY(), marginLeft(), marginTop(), marginRight(), marginBottom() ) );
        if( __gong_debug::_gong_debug_level == 3 ) {
            std::cout << "DeviceMetrics:w:"
                      << aPicture->width() << ",h:" << aPicture->height() << ",wMM:" << aPicture->widthMM() << ",hMM:" << aPicture->heightMM()
                      << ",dpix:" << aPicture->logicalDpiX() << ",dpiy:" << aPicture->logicalDpiY() << std::endl;
            QFontMetrics fm( QFont( "", (int)mReport.realFontSize() ) );
            std::cout << "FontMetrics:w:" << fm.width( 'W' ) << ",h:" << fm.height() << std::endl;
        }
#endif
        if ( mPainter.begin( aPicture ) == 0 ) {
            delete aPicture;
            return 0;
        }
        mPageCollection.push_back( aPicture );
        mSavedBrush = mPainter.brush();
    }
    drawGraphics( mReport, int( marginLeft() + mReport.posX() + mReport.marginLeft()),
                  int(marginTop() + mReport.posY() + (sizeY() + mReport.marginTop()) * (mCurrentPageInFolio -1 ) ),
                  int(mReport.sizeX()), int(mReport.sizeY()), true ); // beforetext
    return 0;
}

Measure ReportQtOutput::endPage()
{
    Output::endPage();
    drawGraphics( mReport, int( marginLeft() + mReport.posX() + mReport.marginLeft()),
                  int( marginTop() + mReport.posY() + (sizeY() + mReport.marginTop()) * (mCurrentPageInFolio -1 ) ),
                  int(mReport.sizeX()), int(mReport.sizeY()), false ); // aftertext
    if( mCurrentPageInFolio <= 1 ) {
        mPainter.end();
    }
    return 0;
}

Measure ReportQtOutput::startSection( const Section &section )
{
    int x0, y0, width, height;
    Output::startSection(section);
    clipMeasures( section, &x0, &y0, &width, &height);
    _GONG_DEBUG_PRINT(4, Xtring::printf("Section:%s, After clipping: x=%d, y=%d, w=%d, h=%d",
                                        section.name(), x0, y0, width, height ) );
    drawGraphics( section, x0, y0, width, height, true ); // beforetext
    mGrowthY = 0;
    return 0;
}


Measure ReportQtOutput::endSection( const Section &section )
{
    int x0, y0, width, height;
    clipMeasures( section, &x0, &y0, &width, &height);
    drawGraphics( section, x0, y0, width, height+int(round(mGrowthY,0)), false ); // aftertext
    Output::endSection(section);
    mCurrY = ceil(mCurrY);
    return 0;
}


inline bool operator==(const Color &c1, const QColor &c2)
{
    return (c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() == c2.blue() );
}
inline bool operator!=(const Color &c1, const QColor &c2)
{
    return !(c1==c2);
}
inline QColor toQColor(const Color &c)
{
    return QColor(c.red(), c.green(), c.blue());
}

/*
	The images and the background should be printed before the text
	The borders of the object should be printed after the text
*/

void ReportQtOutput::drawGraphics( const Object &object, int x0, int y0, int width, int height, bool beforetext )
{
// 	y0 += sizeY() * (mCurrentPageInFolio - 1);
    // Draw the border and the background color
    QColor bc = toQColor(object.backColor());
#ifdef _GONG_DEBUG
    QColor painterbc = mPainter.brush().color();
    _GONG_DEBUG_PRINT(10, Xtring::printf("Color de fondo del objeto: %s", bc.name().latin1() ) );
    _GONG_DEBUG_PRINT(10, Xtring::printf("Color de fondo del painter: %s", painterbc.name().latin1() ) );
#endif
    if( bc != mPainter.brush().color() && beforetext ) { // Pintar el fondo del objeto
        QPen linePen( bc, 0, Qt::NoPen);
        mPainter.setPen( linePen );
        QBrush backBrush( bc );
        mPainter.setBrush( backBrush );
        mPainter.drawRect( x0, y0, width, height );
    }
    // If the general border is defined, use it
    if( beforetext == false ) {
        mPainter.setBrush( Qt::NoBrush );
        if ( object.borderStyle() != BorderNone ) {
            QPen linePen( toQColor(object.borderColor()), object.borderWidth(),
                          ( Qt::PenStyle ) object.borderStyle() );
            mPainter.setPen( linePen );
            mPainter.drawRect( x0, y0, width - object.borderWidth(), height- object.borderWidth() );
        } else { // Use the other borders instead
            if ( object.borderTopStyle() != BorderNone ) {
                QPen linePen( toQColor(object.borderTopColor()), object.borderTopWidth(),
                              ( Qt::PenStyle ) object.borderTopStyle() );
                mPainter.setPen( linePen );
                mPainter.drawLine( x0, y0, x0 + width, y0 );
            }
            if ( object.borderBottomStyle() != BorderNone ) {
                QPen linePen( toQColor(object.borderBottomColor()),
                              object.borderBottomWidth(),
                              ( Qt::PenStyle ) object.borderBottomStyle() );
                mPainter.setPen( linePen );
                mPainter.drawLine( x0, y0 + height - object.borderBottomWidth(), x0 + width, y0 + height -object.borderBottomWidth() );
            }
            if ( object.borderLeftStyle() != BorderNone ) {
                QPen linePen( toQColor(object.borderLeftColor()), object.borderLeftWidth(),
                              ( Qt::PenStyle ) object.borderLeftStyle() );
                mPainter.setPen( linePen );
                mPainter.drawLine( x0, y0, x0, y0 + height );
            }
            if ( object.borderRightStyle() != BorderNone ) {
                QPen linePen( toQColor(object.borderRightColor()), object.borderRightWidth(),
                              ( Qt::PenStyle ) object.borderRightStyle() );
                mPainter.setPen( linePen );
                mPainter.drawLine( x0 + width - object.borderRightWidth(), y0, x0 + width - object.borderRightWidth(), y0 + height );
            }
        }
    }
    if( beforetext ) {
        if( !strempty(object.backgroundImage()) ) {
            RTK::Image *rtkimage = object.report()->findImage(object.backgroundImage());
            if( rtkimage ) {
                _GONG_DEBUG_PRINT(4, object.report()->imagesPath() ); /// BUG
                Xtring realpath = FileUtils::findInPath(object.report()->getCWD() + ":" + object.report()->imagesPath(), rtkimage->url() );
                QPixmap *i = new QPixmap();
                if( i->load(toGUI(realpath)) ) {
                    switch( rtkimage->imageStyle() ) {
                    case ImageNone:
                        mPainter.drawPixmap( x0, y0, *i, 0, 0, width, height);
                        break;
                    case ImageCenter:
                    {
                        int imw = i->width();
                        int imh = i->height();
                        if( imw > width )
                            imw = width;
                        if( imh > height )
                            imh = height;
                        int imx0 = ( x0 + (width - imw) / 2 );
                        int imy0 = ( y0 + (height - imh) / 2 );
                        mPainter.drawPixmap( imx0, imy0, *i, 0, 0, imw, imh);
                    }
                    break;
                    case ImageTile:
                        mPainter.drawTiledPixmap( x0, y0, width, height, *i);
                        break;
                    case ImageExpand:
                        mPainter.drawPixmap( QRect(x0, y0, width, height), *i);
                        break;
                    }
                } else {
                    mReport.addWarning( Error::OpenFile, __FUNCTION__, 0,
                                        realpath.c_str(), strerror(errno) );
                }
            }
        }
    }
}


/** prints the object using the specificed painter & x/y-offsets */
Measure ReportQtOutput::printObject( const Object &object )
{
    int x0, y0, width, height;
    if( object.isSection() ) // If this is a section, do not print the value, which is the groupvalue
        return y0;

    clipMeasures( object, &x0, &y0, &width, &height);
    _GONG_DEBUG_PRINT(5, Xtring::printf("Printing object %s(%s) at x=%d, y=%d, w=%d, h=%d)",
                                        object.name(), Variant::typeToName(object.realValue().type()),
                                        x0, y0,width,height ) );

    drawGraphics( object, x0, y0, width, height, true ); // beforetext
    Section *parent_section = static_cast<Section *>(object.parent());
    if( parent_section->padding() != 0.0 ) {
        x0 += parent_section->padding();
        y0 += parent_section->padding();
    } else {
        x0 += parent_section->paddingLeft();
        y0 += parent_section->paddingTop();
    }

    // TODO:Optimization: getRealValue is called twice, the other in object.text()
    Variant value = object.realValue();
    if( value.type() == Variant::tBinary ) {
        // Draw the Image
        QPixmap image;
        if( image.loadFromData( (const uchar *)value.toString().c_str(), value.toString().size(), 0, 0 ) ) {
            switch( object.imageStyle() ) {
            case ImageNone:
                _GONG_DEBUG_PRINT( 4, "Drawing image without style" );
                mPainter.drawPixmap( x0, y0, image.width(), image.height(), image);
                break;
            case ImageCenter:
            {
                _GONG_DEBUG_PRINT( 4, "Drawing centered image" );
                int imw = image.width();
                int imh = image.height();
                if( imw > width )
                    imw = width;
                if( imh > height )
                    imh = height;
                int imx0 = ( x0 + (width - imw) / 2 );
                int imy0 = ( y0 + (height - imh) / 2 );
                mPainter.drawPixmap( imx0, imy0, image, 0, 0, imw, imh );
            }
            break;
            case ImageTile:
                _GONG_DEBUG_PRINT( 4, "Drawing tiled image" );
                mPainter.drawTiledPixmap( x0, y0, width, height, image);
                break;
            case ImageExpand:
                _GONG_DEBUG_PRINT( 4, "Drawing expanded image" );
                mPainter.drawPixmap( x0, y0, width, height, image);
                break;
            }
        } else {
            mReport.addWarning( Error::ImageFormat, __FILE__ , 0, object.name(), mReport.recordCount() );
        }
    } else {
        Xtring text( object.formattedText() );
        if ( text.size() ) {
            // Set the font
            double fs = object.realFontSize();
            QFont font( object.fontFamily(), fs == 0.0 ? 10 : int(round(fs, 0)),
                        object.fontWeight(), object.fontItalic() );
            mPainter.setFont( font );
            QFontMetrics fm( font );
            QPen textPen( toQColor(object.fontColor()), 0, Qt::SolidLine );
            mPainter.setPen( textPen );

            // Set the text alignment flags
            // Horizontal
            int tf = Qt::AlignLeft;
            switch ( object.hAlignment() ) {
            case AlignAuto:
                if ( Variant::isNumeric( value.type() ) )
                    tf = Qt::AlignRight;
                else
                    tf = Qt::AlignLeft;
                break;
            case AlignLeft:
                tf = Qt::AlignLeft;
                break;
            case AlignTop:
            case AlignJustify:  // TODO
                tf = Qt::AlignLeft;
                break;
            case AlignHCenter:
                tf = Qt::AlignHCenter;
                break;
            case AlignRight:
            case AlignBottom:
                tf = Qt::AlignRight;
            default:
                break;
            }

            // Vertical
            switch ( object.vAlignment() ) {
            case AlignJustify:  // ???
            case AlignTop:
                tf = tf | Qt::AlignTop;
                break;
            case AlignBottom:
            case AlignRight:
                tf = tf | Qt::AlignBottom;
                break;
            case AlignCenter:
            case AlignVCenter:
            case AlignLeft:
                tf = tf | Qt::AlignVCenter;
            default:
                break;
            }

            switch ( object.adjustment() ) {
            case RTK::AdjustGrow: {
                tf = tf | Qt::TextWordWrap;
                QRect outrect = mPainter.boundingRect( x0, y0, width, -1, tf,
                                                       toGUI(text) );
                mPainter.drawText( x0, y0 - fm.descent(), width, outrect.height(),
                                   tf, toGUI(text) );
                Measure excess = outrect.height() - height;
                if( excess > mGrowthY )
                    mGrowthY = excess;
                /** \todo {add} redraw section's background and borders */
            }
            break;
            case RTK::AdjustReduceFont: {
                QFont *smallerfont = 0;
                QRect outrect = mPainter.boundingRect( x0, y0, width, -1, tf, toGUI(text) );
                while( (outrect.height() -2 - height > 0)
                        || (outrect.width() -1 - width > 0 ) ) {
                    if( !smallerfont )
                        smallerfont = new QFont( font );
                    smallerfont->setPointSize( smallerfont->pointSize() - 0.5 );
                    _GONG_DEBUG_PRINT( 4, smallerfont->pointSize() );
                    if( smallerfont->pointSize() < 7 )
                        break;
                    mPainter.setFont( *smallerfont );
                    outrect = mPainter.boundingRect( x0, y0, width, -1, tf, toGUI(text) );
                }
                if( smallerfont != 0 ) {
                    QFontMetrics fm( *smallerfont );
                    mPainter.drawText( x0, y0 - fm.descent(), width, height, tf, toGUI(text) );
                    delete smallerfont;
                    mPainter.setFont( font );
                } else {
                    mPainter.drawText( x0, y0 - fm.descent(), width, height, tf, toGUI(text) );
                }
            }
            break;
            case RTK::AdjustTrim:
                _GONG_DEBUG_PRINT(10, Xtring::printf("drawText(x=%d, y=%d, w=%d, h=%d, \"%s\")",
                                                     x0, y0-fm.descent(),width,height+fm.descent(), text.c_str() ) );
                mPainter.drawText( x0, y0 - fm.descent(), width, height + fm.descent(),
                                   tf, toGUI(text) );
                break;
            case RTK::AdjustNone:
                mPainter.drawText( x0, y0 - fm.descent(), int(round(sizeX() - marginRight() - x0, 0)), height + fm.descent(),
                                   tf, toGUI(text) );
                break;
            default:
                break;
            }
        }
    }
    drawGraphics( object, x0, y0, width, height, false ); // aftertext
    return y0;
}

} // namespace gong

