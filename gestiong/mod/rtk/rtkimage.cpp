/*
[16:41:41] <santi> para usar las imagenes
[16:41:53] <santi> tienes que anadir a los rtk una nueva tag
[16:41:57] <santi> <IMAGE>
[16:42:03] <santi> por ejemplo,
[16:42:41] <santi> <IMAGE name="logo" url="/home/gong/cositas/phone.png" mimetype="image/gif"></IMAGE>
[16:42:55] <santi> esto define una imagen con nombre 'logo'
[16:43:13] <santi> la url es el fichero que la contiene, por ahora solo ficheros locales
[16:43:31] <santi> el mimetype no lo uso por ahora
[16:43:45] <santi> pero en el futuro dira que tipo de imagen (o cualquier otra cosa es)
[16:44:00] <santi> luego, en cualquier objeto del reports
[16:44:11] <santi> puedes poner su propiedad backgroundimage="logo"
[16:44:25] <santi> y ya esta
[16:44:40] <santi> cuando digo cualquier objeto, me refiero a objetos, secciones y al propio informe
[16:44:57] <santi> otras propiedades que voy a a;adir a las imagenes son
[16:45:20] <santi> cosas como si hay que escalarlas, rotarlas, aplicarle efectos, duplicarlas, etc.
[16:45:54] <santi> al definir las imagenes independientemente de los objetos, se pueden reutilizar
[16:45:58] <santi> en distintos objetos
[16:46:13] <santi> y ademas, se pueden cambiar a voluntad usando formulas para mostrar imagenes
[16:46:21] <santi> diferentes segun el valor de algun campo
[16:46:40] <santi> tambien se podran definir las imagenes directamente en la plantilla rtk
[16:47:00] <santi> dentro del tag <IMAGE>[[CDATA]]</IMAGE>
[16:47:14] <santi> asi nos ahorramos abrir ficheros y todo va mas rapido
[16:47:35] <santi> por supuesto, se pueden usar imagenes en los estilos, etc....
*/

#include <gongfileutils.h>
#include "rtkimage.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

Image::~Image()
{
    if( pBaseImage ) delete pBaseImage;
}

// Do not read the image until really needed
const BaseImage *Image::findBaseImage(const Xtring &paths, const Xtring &cwd)
{
    if( !pBaseImage ) {
        if( !strempty(url()) ) {
            Xtring realpath = FileUtils::findInPath(paths, url(), cwd);
            pBaseImage = new BaseImage();
            if( !pBaseImage->read(realpath) ) {
                delete pBaseImage;
                pBaseImage = 0;
            }
        }
    }
    return pBaseImage;
}

void Image::fixParameters(const ParametersList &parameters, const char *delim)
{
    /*<<<<<IMAGE_FIXPARAMETERS*/
    propUrl.fix( parameters, delim );
    propMimeType.fix( parameters, delim );
    propImageStyle.fix( parameters, delim, RTK::ImageNone );
    /*>>>>>IMAGE_FIXPARAMETERS*/
}

} // Namespace RTK
