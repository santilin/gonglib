#include <QBuffer>
#include <QImageWriter>
#include <gongguiapplication.h>
#include <gongfileutils.h>
#include <gonggettext.h>
#include "factumodule.h"
#include "facturecarticulo.h"
#include "factufrmcambiosmasivosarticulos.h"

namespace gong {
namespace factu {

static const int OP_CAMBIAR_PRECIOS = 1;
static const int OP_GRABAR_IMAGENES = 2;
static const int OP_CARGAR_IMAGENES = 3;

static const int OP_PRECIOS_FIJO = 1;
static const int OP_PRECIOS_SUMAR_PORCENTAJE = 2;
static const int OP_PRECIOS_RESTAR_PORCENTAJE = 3;

FrmCambiosMasivosArticulos::FrmCambiosMasivosArticulos(QWidget * parent, const char * name,
        WidgetFlags fl)
    : FrmCustom(parent, name, fl)
{
    if ( !name )
        setName( "FrmCambiosMasivosArticulos" );
    Xtring titulo = _("Cambios masivos en artículos");
    setTitle( titulo );
    pComboOperacion = addComboBoxInt( false, 0, _("Operación"), mOperaciones, mCodigosOperaciones );
    pComboOperacion->insertItem( _("Cambiar precios"), OP_CAMBIAR_PRECIOS );
    pComboOperacion->insertItem( _("Grabar imágenes en una carpeta"), OP_GRABAR_IMAGENES );
    pComboOperacion->insertItem( _("Cargar imágenes desde una carpeta"), OP_CARGAR_IMAGENES );
    XtringList campos = getPVPFields();
    XtringList campos_captions;
    dbTableDefinition *art_def = DBAPP->getDatabase()->findTableDefinition( "ARTICULO" );
    _GONG_DEBUG_ASSERT( art_def );
    for( XtringList::const_iterator it = campos.begin(); it != campos.end(); ++ it )
        campos_captions << art_def->findFieldDefinition( *it )->getDescription();
    QFrame *frameCambioPrecios = new QFrame( this );
    pControlsLayout->addWidget( frameCambioPrecios );
    QHBoxLayout *frameCambioPreciosLayout = new QHBoxLayout( frameCambioPrecios );
    pComboCampoPrecioCambio = addComboBoxXtring( false, frameCambioPrecios, _("Cambiar:"),
                              campos_captions, campos, Xtring::null,
                              0, frameCambioPreciosLayout );
    pComboOperacionPrecio = addComboBoxInt( false, frameCambioPrecios, _("Aplicar"),
                                            mOperacionesPrecios, mCodigosOperacionesPrecios,
                                            Xtring::null, 0, frameCambioPreciosLayout );
    pComboOperacionPrecio->insertItem( _("Precio fijo"), OP_PRECIOS_FIJO );
    pComboOperacionPrecio->insertItem( _("Sumar porcentaje"), OP_PRECIOS_SUMAR_PORCENTAJE );
    pComboOperacionPrecio->insertItem( _("Restar porcentaje"), OP_PRECIOS_RESTAR_PORCENTAJE );
    pNuevoPrecio = addInput( frameCambioPrecios, _("Nuevo valor"), 0.0, "double", 0, frameCambioPreciosLayout );
    pComboCampoPrecioBase = addComboBoxXtring( false, frameCambioPrecios, _("Sobre:"),
                            campos_captions, campos, _("Valor introducido"), 0, frameCambioPreciosLayout );
    pSearchFamilia = addMultipleSearchField( 0, "FAMILIA", "CODIGO", "NOMBRE" );
    pSearchProveedora = addMultipleSearchField( 0, "PROVEEDORA", "CODIGO", "RAZONSOCIAL" );
    pSearchArticulo = addMultipleSearchField( 0, "ARTICULO", "CODIGO", "NOMBRE" );
}

XtringList FrmCambiosMasivosArticulos::getPVPFields() const
{
    XtringList pvpfields;
    pvpfields << "PVP" << "PVP1" << "PVP2" << "PVP3" << "COSTE" << "PVPSINIVA" << "COSTESINIVA";
    return pvpfields;
}


Xtring FrmCambiosMasivosArticulos::getGlobalWhere() const
{
    List<dbRecordID> &familia_ids = pSearchFamilia->getRecordIDs();
    List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
    List<dbRecordID> &articulo_ids = pSearchArticulo->getRecordIDs();
    Xtring where;
    if( familia_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "F.ID IN(" + familia_ids.join(",") + ")";
    }
    if( proveedora_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "P.ID IN(" + proveedora_ids.join(",") + ")";
    }
    if( articulo_ids.size() ) {
        if( !where.isEmpty() )
            where += " AND ";
        where += "A.ID IN(" + articulo_ids.join(",") + ")";
    }
    if( !where.isEmpty() )
        return " WHERE " + where;
    else
        return where;
}

bool FrmCambiosMasivosArticulos::validate()
{
    List<dbRecordID> &familia_ids = pSearchFamilia->getRecordIDs();
    List<dbRecordID> &proveedora_ids = pSearchProveedora->getRecordIDs();
    List<dbRecordID> &articulo_ids = pSearchArticulo->getRecordIDs();
    if( (familia_ids.size() || proveedora_ids.size() )
            && articulo_ids.size() ) {
        msgError(this, _("Si seleccionas familias o proveedoras, no puedes seleccionar artículos") );
        return false;
    }
    if( pComboOperacionPrecio->getCurrentItemValue() != OP_PRECIOS_FIJO
            && pComboCampoPrecioBase->getCurrentItemValue().isEmpty() ) {
        msgError(this, _("Elige un campo sobre el que aplicar los cambios") );
        return false;
    }
    return true;
}

void FrmCambiosMasivosArticulos::accept()
{
    if( validate() ) {
        Xtring message = _("%d artículos cambiados");
        unsigned long int count;
        switch( pComboOperacion->getCurrentItemValue() ) {
        case OP_CAMBIAR_PRECIOS:
            count = cambiar_precios( pComboOperacionPrecio->getCurrentItemValue(),
                                     pNuevoPrecio->toDouble(),
                                     pComboCampoPrecioCambio->getCurrentItemValue(),
                                     pComboCampoPrecioBase->getCurrentItemValue() );
            break;
        case OP_CARGAR_IMAGENES:
            message = "%d imágenes de artículos cargadas satisfactoriamente";
            count = cargar_imagenes();
            break;
        case OP_GRABAR_IMAGENES:
            message = "%d imágenes de artículos grabadas satisfactoriamente";
            count = grabar_imagenes();
            break;
        }
        msgOk(this, Xtring::printf( message, count ) );
    }
}

long unsigned int FrmCambiosMasivosArticulos::cambiar_precios(
    int operacion, double nuevoprecio, const Xtring& fldcambio, const Xtring &fldbase)
{
    Xtring where = getGlobalWhere();
    Xtring select = "SELECT A.ID "
                    " FROM ARTICULO A INNER JOIN PROVEEDORA P ON P.ID=A.PROVEEDORA_ID"
                    " INNER JOIN FAMILIA F ON F.ID=A.FAMILIA_ID";
    select += where;
    dbResultSet *rs = DBAPP->getConnection()->select( select );
    RecArticulo *articulo = static_cast<RecArticulo *>( DBAPP->createRecord("ARTICULO") );
    _GONG_DEBUG_ASSERT( articulo );
    unsigned long int count = 0;
    DBAPP->showOSD( getTitle(), Xtring::null, 0 );
    DBAPP->waitCursor(true);
    while( rs->next() ) {
        DBAPP->processEvents();
        if( articulo->read( rs->toInt((uint)0) ) ) {
            switch( operacion ) {
            case OP_PRECIOS_FIJO:
                if( fldbase.isEmpty() )
                    articulo->setValue( fldcambio, Money( nuevoprecio ) );
                else
                    articulo->setValue( fldcambio, articulo->getValue( fldbase ).toMoney() );
                break;
            case OP_PRECIOS_SUMAR_PORCENTAJE:
                if( !fldbase.isEmpty() )
                    articulo->setValue( fldcambio, Money( articulo->getValue( fldbase ).toDouble() * ( 1 + nuevoprecio / 100 ) ) );
                break;
            case OP_PRECIOS_RESTAR_PORCENTAJE:
                if( !fldbase.isEmpty() )
                    articulo->setValue( fldcambio, Money( articulo->getValue( fldbase ).toDouble() * ( 1 - nuevoprecio / 100 ) ) );
                break;
            }
            articulo->fixPrecios( fldcambio );
            if( articulo->save(true, false ) )
                count ++;
            if( count % 10 == 0 )
                DBAPP->showOSD( getTitle(), Xtring::printf(_("%d artículos cambiados"), count), 0 );
        }
    }
    DBAPP->resetCursor();
    DBAPP->hideOSD();
    delete articulo;
    return count;
}

unsigned long int FrmCambiosMasivosArticulos::grabar_imagenes()
{
    int count = 0;
    Xtring dirname = GuiApplication::getExistingDirName( _("Carpeta de destino de las imágenes"),
                     DBAPP->getLocalDataDir(), this );
    if( !dirname.isEmpty() ) {
        DBAPP->waitCursor( true );
        Xtring where = getGlobalWhere();
        Xtring select = "SELECT A.ID, A.CODIGO, AI.IMAGEN "
                        " FROM ARTICULO A LEFT JOIN ARTICULOIMAGEN AI ON A.ARTICULOIMAGEN_ID = AI.ID INNER JOIN PROVEEDORA P ON P.ID=A.PROVEEDORA_ID"
                        " INNER JOIN FAMILIA F ON F.ID=A.FAMILIA_ID";
        select += " " + where;
        dbResultSet *rs = DBAPP->getConnection()->select( select );
        while( rs->next() ) {
            Xtring imagename = Xtring::printf(
                                   ModuleInstance->getModuleSetting( "IMGARTFILENAME" ).toString(),
                                   rs->toString(1).c_str() );
            Xtring imagen = rs->toString(2);
            if( !imagen.isEmpty() ) {
                count++;
                QImage imagefromdata;
                QByteArray a( imagen.c_str(), imagen.size() );
                if( ! imagefromdata.loadFromData( a, "PNG" ) ) {
                    _GONG_DEBUG_WARNING( "Could not read image " + imagename );
                } else {
                    QPixmap *pixmap = new QPixmap();
                    if( !pixmap->convertFromImage( imagefromdata ) ) {
                        _GONG_DEBUG_WARNING( "Could not convert image " + imagename );
                    } else if( !pixmap->save( Xtring(dirname + "/" + imagename).c_str() ) ) {
                        _GONG_DEBUG_WARNING( "Could not save image " + imagename );
                    }
                    delete pixmap;
                }
            }
        }
        DBAPP->resetCursor();
    }
    return count;
}

unsigned long int FrmCambiosMasivosArticulos::cargar_imagenes()
{
    int count = 0;
    Xtring dirname = GuiApplication::getExistingDirName(_("Carpeta que contiene las imágenes"),
                     DBAPP->getLocalDataDir(), this );
    if( !dirname.isEmpty() ) {
        DBAPP->waitCursor( true );
        Xtring where = getGlobalWhere();
        Xtring select = "SELECT A.ID, A.CODIGO, A.ARTICULOIMAGEN_ID "
                        " FROM ARTICULO A INNER JOIN PROVEEDORA P ON P.ID=A.PROVEEDORA_ID"
                        " INNER JOIN FAMILIA F ON F.ID=A.FAMILIA_ID";
        select += " " + where;
        QPixmap *pixmap = new QPixmap();
        dbResultSet *rs = DBAPP->getConnection()->select( select );
        while( rs->next() ) {
            Xtring imagename = dirname + "/" + Xtring::printf(
                                   ModuleInstance->getModuleSetting( "IMGARTFILENAME" ).toString(),
                                   rs->toString(1).c_str() );
            dbRecordID a_id = rs->toInt((uint)0);
            dbRecordID ai_id = rs->toInt(2);
            if( FileUtils::exists( imagename.c_str() ) ) {
                if( pixmap->load( imagename.c_str() ) ) {
                    QImage imagetosave = pixmap->toImage();
                    QByteArray bytes;
                    QBuffer buffer(&bytes);
                    QImageWriter writer(&buffer, "PNG");
                    writer.write(imagetosave);
                    QByteArray data = buffer.data();
                    Variant value = Variant( (const char *)data, data.length() );
                    Xtring update;
                    if( ai_id == 0 ) {
                        update = "INSERT INTO ARTICULOIMAGEN (IMAGEN) VALUES( "
                                 + DBAPP->getConnection()->toSQL( value ) + ")";
                        DBAPP->getConnection()->exec( update );
                        update = "UPDATE ARTICULO SET ARTICULOIMAGEN_ID="
                                 + DBAPP->getConnection()->toSQL( DBAPP->getConnection()->getLastInsertID( Xtring::null, Xtring::null ) )
                                 + " WHERE ID=" + DBAPP->getConnection()->toSQL( a_id );
                        DBAPP->getConnection()->exec( update );
                    } else {
                        update = "UPDATE ARTICULOIMAGEN SET IMAGEN=" + DBAPP->getConnection()->toSQL( value )
                                 + " WHERE ID=" + DBAPP->getConnection()->toSQL( ai_id );
                        DBAPP->getConnection()->exec( update );
                    }
                    count++;
                } else {
                    _GONG_DEBUG_WARNING( "Could not load image " + imagename );
                }
            } else {
                _GONG_DEBUG_WARNING( "There is no image " + imagename );
            }
        }
        delete pixmap;
        DBAPP->resetCursor();
    }
    return count;
}


} // namespace factu
} // namespace gong

