#include <QBuffer>
#include <QImage>
#include <QImageWriter>
#include <unistd.h>
#include <gongfileutils.h>
#include <gonggettext.h>
#include "gongfrmimgadvanced.h"
#include "gongimagebox.h"

namespace gong {

enum effects { 	NONE, BUBBLE, ROUNDED_CORNERS, BLURRED_ROUNDED_CORNERS, FORWEB, FORSCREEN,  };

FrmImgAdvanced::FrmImgAdvanced( ImageBox *imgbox, const Xtring &convert_path,
                                QWidget *parent, const char *name )
    : FrmBase( parent, name ), pImageBox( imgbox ), mConvertPath( convert_path )
{
//     theGuiApp->waitCursor( true );
    QVBoxLayout *mainLayout = new QVBoxLayout( this );
    pImageLabel = new ResizableImageLabel( this, (Xtring("lblimg_") + name).c_str() );
    pImageLabel->setText( imgbox->text() );
    pImageLabel->setEnabled( true );
    //pImageLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pImageLabel->sizePolicy().hasHeightForWidth() ) );
    pImageLabel->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    pImageLabel->setFrameShape( QFrame::StyledPanel );
    pImageLabel->setFrameShadow( QLabel::Sunken );
    pImageLabel->setLineWidth( 3 );
    pImageLabel->setScaledContents( true );
    pImageLabel->setMinimumWidth( 300 );
    pImageLabel->setMinimumHeight( 300 );
    pImageLabel->setAlignment( int( Qt::AlignTop | Qt::AlignHCenter ) );
    setImageData( imgbox->toData() );
    mainLayout->addWidget( pImageLabel );
    pInfoLabel = new QLabel( this, (Xtring("lblinfo_") + name).c_str() );
    mainLayout->addWidget( pInfoLabel );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    pushReset = new QPushButton( this );
    pushReset->setText( toGUI(_("Reiniciar")) );
    connect( pushReset, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    buttonsLayout->addWidget( pushReset );
    XtringList effects;
    IntList values;
    effects << _("Elige un efecto...") << _("Burbuja") << _("Redondear esquinas")
            << _("Difuminar y redondear esquinas");
    comboEffects = new ComboBoxInt( effects, values, this, Xtring::null, Xtring::null );
    connect( comboEffects, SIGNAL( activated( int ) ), this, SLOT( slot_combo_activated( int ) ) );
    buttonsLayout->addWidget( comboEffects );
    pushCrop = new QPushButton( this );
    pushCrop->setText( toGUI(_("Recortar")) );
    connect( pushCrop, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    buttonsLayout->addWidget( pushCrop );
    pushResize = new QPushButton( this );
    pushResize->setText( toGUI(_("Tamaño")) );
    buttonsLayout->addWidget( pushResize );
    connect( pushResize, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    buttonsLayout->addWidget( pushCancel );
    buttonsLayout->addWidget( pushAccept );
    mainLayout->addLayout( buttonsLayout );
    displayInfo();
    setMaximumHeight( 800 );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    setTitle( _("Edición avanzada de imágenes" ) );
//     theGuiApp->resetCursor();
}

void FrmImgAdvanced::accept()
{
    pImageBox->setImageData( getImageData() );
    close();
}


Variant FrmImgAdvanced::getImageData() const
{
    Variant ret;
    if( pImageLabel->pixmap() ) {
        QByteArray bytes;
        QBuffer buffer(&bytes);
        QImageWriter writer(&buffer, "PNG");
        QImage testImage = pImageLabel->pixmap()->toImage();
        writer.write(testImage);
        QByteArray data = buffer.data();
        ret = Variant( (const char *)data, data.length() );
    }
    return ret;
}

void FrmImgAdvanced::displayInfo()
{
    if( !pImageLabel->pixmap() ) {
        pInfoLabel->clear();
        return;
    }
    Xtring text = Xtring::printf( _("Tamaño %dx%d, %d bytes\nNúmero de colores: %d"),
                                  pImageLabel->pixmap()->width(),
                                  pImageLabel->pixmap()->height(),
                                  mByteCount,
                                  pImageLabel->pixmap()->depth()
                                );
    pInfoLabel->setText( toGUI( text ) );
}

void FrmImgAdvanced::setImageData( const Variant &data )
{
    mByteCount = data.toString().size();
    if( mByteCount == 0 ) {
        pImageLabel->clear();
        return;
    }
    QImage imagefromdata;
    QByteArray a( data.toString().data(), data.toString().size() );
    if( ! imagefromdata.loadFromData( a, "PNG") ) {
        _GONG_DEBUG_WARNING( "Could not read PNG image" );
        return;
    }
    QPixmap *pixmap = new QPixmap();
    if( !pixmap->convertFromImage( imagefromdata ) ) {
        _GONG_DEBUG_WARNING( "Could not convert PNG Image" );
        delete pixmap;
        return;
    } else
        pImageLabel->setPixmap( *pixmap );
    delete pixmap;
    pImageLabel->update();
}

void FrmImgAdvanced::slot_button_clicked()
{
	QPushButton *button = static_cast<QPushButton *>(sender());
    if( button == pushReset ) {
        setImageData( pImageBox->toData() );
        pImageLabel->update();
        displayInfo();
    } else if( button == pushCrop ) {
        if( pImageLabel->getRubberBand() && pImageLabel->getRubberBand()->isVisible() ) {
            QSize origsize( pImageLabel->pixmap()->size() );
            QPixmap copied( pImageLabel->pixmap()->copy( pImageLabel->getRubberBand()->geometry() ) );
            QPixmap scaled( copied.scaled( origsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation) );
            pImageLabel->setPixmap( scaled );
            displayInfo();
            pImageLabel->getRubberBand()->hide();
        }
    } else if( button == pushResize ) {
        QSize origsize( pImageLabel->pixmap()->size() );
        FrmResizeImage *fri = new FrmResizeImage( origsize.width(), origsize.height(), this );
        fri->showModalFor( this, true, true );
        if( !fri->wasCancelled() ) {
            int newsizex = fri->getSizeX();
            int newsizey = fri->getSizeY();
            int bytes = fri->getByteCount();
            Xtring command = mConvertPath;
            if( newsizex != 0 || newsizey != 0 ) {
				command += " -resize ";
                if( newsizex != 0 )
                    command += Xtring::number( newsizex );
                if( newsizey != 0 )
                    command += "x" + Xtring::number( newsizey );
            } else if( bytes != 0 ) {
                command += " -resize " + Xtring::number( bytes ) + "@";
            } else {
                msgError( this, _("No se ha introducido ningún tamaño") );
                return;
            }
            const char *_imagename = "/tmp/tmpimage.png";
            pImageLabel->pixmap()->save( _imagename );
            command += " " + Xtring(_imagename) ;
            execCommandAndShow( command );
            unlink( _imagename );
        }
        delete fri;
    }
}
/* http://www.imagemagick.org/Usage/thumbnails/*/
void FrmImgAdvanced::slot_combo_activated(int index)
{
    Xtring command;
    const char *_imagename = "/tmp/tmpimage.png";
    pImageLabel->pixmap()->save( _imagename );
    switch( index ) {
    case ROUNDED_CORNERS: {
        int radio = 64;
        Xtring color = "White";
        if( !FrmBase::inputBoxInt( radio, _("Radio") ) )
            return;
        if( !FrmBase::inputBoxString( color, _("Color") ) )
            return;
        command = Xtring::printf( "convert %s \\( +clone -crop %dx%d+0+0 -fill %s -colorize 100%% -draw 'fill black circle %d,%d %d,0' -background %s -alpha shape \\( +clone -flip \\) \\( +clone -flop \\) \\( +clone -flip \\) \\) -flatten ",
                                  _imagename, radio, radio, color.c_str(), radio-1, radio-1, radio-1, color.c_str() );
        execCommandAndShow( command );
    }
    break;
    case BUBBLE: {
        int size = 8;
        Xtring color = "gray50";
        if( !FrmBase::inputBoxInt( size, _("Tamaño del borde") ) )
            return;
        if( !FrmBase::inputBoxString( color, _("Color") ) )
            return;
        command = Xtring::printf( "convert %s -fill %s -colorize 100%% -raise %d -normalize -blur 0x%d  /tmp/bubble_overlay.png;"
                                  "convert %s /tmp/bubble_overlay.png -compose hardlight -composite",
                                  _imagename, color.c_str(), size, size, _imagename );
        execCommandAndShow( command );
        unlink( "/tmp/bubble_overlay.png" );
    }
    break;
    case BLURRED_ROUNDED_CORNERS: {
        int size = 8;
        if( !FrmBase::inputBoxInt( size, _("Tamaño del borde") ) )
            return;
        command = Xtring::printf( "convert %s -alpha set -virtual-pixel transparent -channel A -blur 0x%d -level 50%%,100%% +channel",
                                  _imagename, size );
        execCommandAndShow( command );
    }
    case FORWEB:
        break;
    case FORSCREEN:
        break;
    }
    unlink( _imagename );
}



/**
 * @brief Execs a image manipulation command. Add a final parameter that is the resulting image
 *
 * @param command ...
 * @return bool
 **/
int FrmImgAdvanced::execCommandAndShow(const Xtring& command)
{
    const char *result_img = "/tmp/tmpimage_resized.png";
    theGuiApp->waitCursor( true );
    Xtring messages, errors;
    int ret = FileUtils::execProcess( command + " " + result_img, messages, errors );
    if( errors.isEmpty() && messages.isEmpty() ) {
        QPixmap *pixmap = new QPixmap();
        if( pixmap->load( result_img ) ) {
            pImageLabel->setPixmap( *pixmap );
            delete pixmap;
            pImageLabel->update();
            mByteCount = FileUtils( result_img ).size();
            displayInfo();
            unlink( result_img );
        }
    } else {
        msgError( this, messages + "\n" + errors );
    }
    theGuiApp->resetCursor();
    return ret;
}


// RESIZABLE IMAGE LABEL

void ResizableImageLabel::mousePressEvent(QMouseEvent *event)
{
    mOrigin = event->pos();
    if( !pRubberBand )
        pRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    else
        pRubberBand->hide();
    pRubberBand->setGeometry(QRect(mOrigin, QSize()));
    pRubberBand->show();
}

void ResizableImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    pRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
}

void ResizableImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
// 	pRubberBand->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}

FrmResizeImage::FrmResizeImage(int sizex, int sizey, QWidget* parent)
    : FrmBase(parent)
{
    QVBoxLayout *formlayout = new QVBoxLayout( this );
    QHBoxLayout *sizexLayout = new QHBoxLayout();
    QLabel *lblsizex = new QLabel( this, toGUI(_("Ancho")) );
    sizexLayout->addWidget( lblsizex );
    pEditSizeX = new LineEdit( this, "sizex", Variant::tInt );
    pEditSizeX->setText( sizex );
    sizexLayout->addWidget( pEditSizeX );
    QHBoxLayout *sizeyLayout = new QHBoxLayout();
    QLabel *lblsizey = new QLabel( this, toGUI(_("Alto")) );
    sizeyLayout->addWidget( lblsizey );
    pEditSizeY = new LineEdit( this, "sizey", Variant::tInt );
    pEditSizeY->setText( sizey );
    sizeyLayout->addWidget( pEditSizeY );
    QHBoxLayout *aspectLayout = new QHBoxLayout();
    pCheckAspect = new QCheckBox( this );
    pCheckAspect->setText( _("Mantener aspecto") );
    aspectLayout->addWidget( pCheckAspect );
    pCheckAspect->setChecked( true );
    QHBoxLayout *bytesLayout = new QHBoxLayout();
    QLabel *lblbytes = new QLabel( this, toGUI(_("Tamaño")) );
    bytesLayout->addWidget( lblbytes );
    pEditByteCount = new LineEdit( this, "bytecount", Variant::tInt);
    bytesLayout->addWidget( pEditByteCount );
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( pushCancel );
    buttonsLayout->addWidget( pushAccept );
    formlayout->addLayout( sizexLayout );
    formlayout->addLayout( sizeyLayout );
    formlayout->addLayout( aspectLayout );
    formlayout->addLayout( bytesLayout );
    formlayout->addLayout( buttonsLayout );
    connect( pEditSizeX, SIGNAL( validate(QWidget *, bool*) ),
             this, SLOT(validateFields(QWidget*, bool*) ) );
    connect( pEditSizeY, SIGNAL( validate(QWidget *, bool*) ),
             this, SLOT(validateFields(QWidget*, bool*) ) );
    if( sizey != 0 )
        mAspectRatio = (double)sizex/(double)sizey;
    else
        mAspectRatio = 0.0;
}

void FrmResizeImage::validateFields(QWidget *sender , bool *isvalid )
{
    _GONG_DEBUG_PRINT(0, mAspectRatio );
    _GONG_DEBUG_PRINT(0, pEditSizeX->toInt());
    _GONG_DEBUG_PRINT(0, pEditSizeY->toInt());
    if( pCheckAspect->isOn() && mAspectRatio != 0.0 ) {
        if( sender == pEditSizeX ) {
            pEditSizeY->setText( int(pEditSizeX->toInt() / mAspectRatio) );
        } else if( sender == pEditSizeY ) {
            pEditSizeX->setText( int(pEditSizeY->toInt() * mAspectRatio) );
        }
    }
}

} // namespace gong
