#include <QClipboard>
#include <QBuffer>
#include <QImage>
#include <QImageWriter>
#include <gongfileutils.h>
#include <gonggettext.h>
#include "gongfrmbase.h"
#include "gongguiapplication.h"
#include "gongguinamespace.h"
#include "gongimagebox.h"
#include "gongfrmimgadvanced.h"

namespace gong {

ImageBox::ImageBox( QWidget *parent, const Xtring &name, const Xtring &caption, bool horizontal )
    : QPushButton( parent, name.c_str() ), mHorizontal(horizontal), mMustBeReadOnly(false),
      mByteCount(0)
{
    setIcon( QIcon::fromTheme("edit-find", QIcon(":/edit-find.png")) );
    if( mHorizontal )
        pLayout = (QBoxLayout *)new QHBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    else
        pLayout = (QBoxLayout *)new QVBoxLayout( 0, 0, 6, ("layout_" + name).c_str() );
    pImageLabel = new QLabel( parent, ("label_" + name).c_str() );
    pImageLabel->setText( toGUI( caption ) );
    pImageLabel->setEnabled( true );
    pImageLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pImageLabel->sizePolicy().hasHeightForWidth() ) );
    pImageLabel->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    pImageLabel->setFrameShape( QFrame::StyledPanel );
    pImageLabel->setFrameShadow( QLabel::Sunken );
    pImageLabel->setLineWidth( 3 );
    pImageLabel->setScaledContents( true );
    pImageLabel->setMinimumWidth( 300 );
    pImageLabel->setMinimumHeight( 300 );
    pImageLabel->setMaximumHeight( 800 );
    pushClear = new QPushButton( parent );
    pImageLabel->setAlignment( int( Qt::AlignTop | Qt::AlignHCenter ) );
    pushClear->setText( toGUI(_("Borrar")) );
    connect( pushClear, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    pushSaveAs = new QPushButton( parent );
    pushSaveAs->setText( toGUI(_("Guardar como..." )) );
    connect( pushSaveAs, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    pushPaste = new QPushButton( parent );
    pushPaste->setText( toGUI(_("Pegar" )) );
    connect( pushPaste, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    pushCopy = new QPushButton( parent );
    pushCopy->setText( toGUI(_("Copiar" )) );
    connect( pushCopy, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    pushAdvanced = new QPushButton( parent );
    pushAdvanced->setText( toGUI(_("Editar..." )) );
    connect( pushAdvanced, SIGNAL( clicked() ), this, SLOT( slot_button_clicked() ) );
    QHBoxLayout *pMiniButtonsLayout = new QHBoxLayout();
    pMiniButtonsLayout->addWidget( this );
    pMiniButtonsLayout->addWidget( pushSaveAs );
    pMiniButtonsLayout->addWidget( pushClear );
    pMiniButtonsLayout->addWidget( pushCopy );
    pMiniButtonsLayout->addWidget( pushPaste );
    pMiniButtonsLayout->addWidget( pushAdvanced );
    pLayout->addLayout( pMiniButtonsLayout );
    pLayout->addWidget( pImageLabel );
    pInfoLabel = new QLabel( this, (Xtring("lblinfo_") + name).c_str() );
    pLayout->setSizeConstraint( QLayout::SetFixedSize );
    pLayout->addWidget( pInfoLabel );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
}

void ImageBox::displayInfo( const Xtring &errmsg )
{
    if( !errmsg.isEmpty() ) {
        pInfoLabel->setText( toGUI( errmsg ) );
        return;
    }
    if( !pImageLabel->pixmap() ) {
        pInfoLabel->clear();
        return;
    }
    Xtring text = Xtring::printf( _("Tamaño %dx%d, %d bytes. Profundidad: %d"),
                                  pImageLabel->pixmap()->width(),
                                  pImageLabel->pixmap()->height(),
                                  mByteCount,
                                  pImageLabel->pixmap()->depth()
                                );
    pInfoLabel->setText( toGUI( text ) );
}

void ImageBox::slot_button_clicked()
{
    theGuiApp->waitCursor( true );
    if( sender() == pushClear ) {
        clear();
    } else if( sender() == pushPaste ) {
        pImageLabel->setPixmap( QPixmap::fromImage(QApplication::clipboard()->image()) );
    } else if( sender() == pushCopy ) {
        QApplication::clipboard()->setImage( getImage()->toImage() );
    } else if( sender() == pushSaveAs ) {
        Xtring fname = theGuiApp->getSaveFileName(
                           _("Elige el fichero donde guardar esta imagen"),
                           mProposedFileName,
                           _("Imágenes (*.png);;Todos (*)"), this );
        if( !fname.isEmpty() ) {
            getImage()->save( toGUI(fname) );
        }
    } else if( sender() == pushAdvanced ) {
        FrmImgAdvanced *advform = new FrmImgAdvanced(
            this, "convert", parentWidget(), "FrmImgAdvanced");
        advform->showModalFor( parentWidget(), false, true );
    }
    theGuiApp->resetCursor();
}

bool ImageBox::loadFromFile( const Xtring &filename )
{
    theGuiApp->waitCursor( true );
    QPixmap *pixmap = new QPixmap();
    bool ret = false;
    if( pixmap->load( toGUI(filename) ) ) {
        pImageLabel->setPixmap( *pixmap );
        delete pixmap;
        pImageLabel->update();
        ret = true;
        mByteCount = FileUtils( filename ).size();
        displayInfo();
    } else
        displayInfo( Xtring::printf(_("Fichero %s no encontrado o erróneo"), filename.c_str() ) );
    theGuiApp->resetCursor();
    return ret;
}

bool ImageBox::setImage(QPixmap* image)
{
	pImageLabel->setPixmap( *image);
    pImageLabel->update();
    displayInfo();
    setEdited( true );
	return true;
}

bool ImageBox::setImageData( const Variant &data )
{
    mByteCount = data.toString().size();
    if( mByteCount == 0 ) {
        pImageLabel->clear();
        displayInfo();
        return true;
    }
    QImage imagefromdata;
    QByteArray a( data.toString().data(), mByteCount );
    if( !imagefromdata.loadFromData( a, "PNG") ) {
        _GONG_DEBUG_WARNING( "Could not read PNG image" );
        displayInfo( _("Imposible leer el fichero PNG") );
        return false;
    }
    QPixmap *pixmap = new QPixmap();
    if( !pixmap->convertFromImage( imagefromdata ) ) {
        _GONG_DEBUG_WARNING( "Could not convert PNG Image" );
        displayInfo( _("Imposible convertir imagen a PNG") );
        delete pixmap;
        return false;
    } else
        pImageLabel->setPixmap( *pixmap );
    delete pixmap;
    pImageLabel->update();
    displayInfo();
    setEdited( true );
    return true;
}

Variant ImageBox::toData() const
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

} // namespace
