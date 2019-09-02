#include <gonggettext.h>
#include "ventaswebmodule.h"
#include "ventaswebfrmcambiosmasivosarticulos.h"

namespace gong {
namespace ventasweb {

static const int OP_PUBLICAR_ARTICULO = 100;
static const int OP_DESPUBLICAR_ARTICULO = 101;
static const int OP_SUBIR_IMAGENES_FTP = 102;

FrmCambiosMasivosArticulos::FrmCambiosMasivosArticulos(QWidget * parent,
        const char * name,
        WidgetFlags fl)
    : factu::FrmCambiosMasivosArticulos(parent, name, fl)
{
    pComboOperacion->insertItem( "Publicar artículos en la web", OP_PUBLICAR_ARTICULO );
    pComboOperacion->insertItem( "Despublicar artículos en la web", OP_DESPUBLICAR_ARTICULO );
    pComboOperacion->insertItem( "Subir imágenes por FTP a la web", OP_SUBIR_IMAGENES_FTP );
}

void FrmCambiosMasivosArticulos::accept()
{
    if( !validate() )
        return;
    Xtring message = _("%d artículos cambiados");
    int count = 0;
    switch( pComboOperacion->getCurrentItemValue() ) {
    case OP_PUBLICAR_ARTICULO:
        count = publicar_articulos(true);
        break;
    case OP_DESPUBLICAR_ARTICULO:
        count = publicar_articulos(true);
        break;
    case OP_SUBIR_IMAGENES_FTP:
        count = subir_imagenes();
        break;
    default:
        factu::FrmCambiosMasivosArticulos::accept();
        return;
    }
    msgOk(this, Xtring::printf( message, count ) );
}

int FrmCambiosMasivosArticulos::publicar_articulos(bool publicar)
{
    Xtring where = getGlobalWhere();
    Xtring update = "UPDATE ARTICULO A INNER JOIN PROVEEDORA P ON P.ID=A.PROVEEDORA_ID"
                    " INNER JOIN FAMILIA F ON F.ID=A.FAMILIA_ID";
    update += " SET A.PUBLICARWEB=";
    if( publicar )
        update += "1";
    else
        update += "0";
    update += " " + where;
    DBAPP->getConnection()->exec( update );
    return DBAPP->getConnection()->getAffectedRows();
}


int FrmCambiosMasivosArticulos::subir_imagenes()
{
    return 0;
}


} // namespace ventasweb
} // namespace gong

