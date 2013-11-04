#include <gonggettext.h>
#include <empresamodule.h>
#include <dbappreport.h>
#include "factumodule.h"
#include "factufrmmod303.h"
#include "factufldivadetallado.h"

namespace gong {
namespace factu {


FrmMod303::FrmMod303(QWidget * parent, const char * name,
                     WidgetFlags fl)
    : FrmCustom(parent, name, fl)
{
    if ( !name )
        setName( "FrmMod303" );
    Xtring titulo = _("Modelo 303. Impuesto sobre el valor añadido");
    setTitle( titulo );
    XtringList opciones;
    opciones << "Iva devengado (repercutido)" << "IVA Deducible (soportado)";
    pComboOpciones = addComboBoxXtring( 0, "Opción", opciones );
    XtringList trimestres;
    trimestres << "1er. Trimestre" << "2º Trimestre" << "3er Trimestre" << "4º Trimestre";
    pComboTrimestre = addComboBoxXtring( 0, "Trimestre", trimestres );
    pSearchTipoDoc = addMultipleSearchField( 0, "TIPODOC", "CODIGO", "NOMBRE" );
    pCheckSepararIvas = addCheckBox( 0, "Separar los tipos de IVA", true );
}


void FrmMod303::accept()
{
    bool devengado = (pComboOpciones->currentIndex() == 0);
    bool separarivas = (pCheckSepararIvas->isOn() );
    List<dbRecordID> &tipodoc_ids = pSearchTipoDoc->getRecordIDs();
    Xtring where;
    int ejercicio = empresa::ModuleInstance->getEjercicio();
    switch( pComboTrimestre->currentIndex() ) {
    case 0:
        where = "(F.FECHAIVA>=" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 1, 1 ) )
                + ")AND(F.FECHAIVA<" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 4, 1 ) ) + ")";
        break;
    case 1:
        where = "(F.FECHAIVA>=" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 4, 1 ) )
                + ")AND(F.FECHAIVA<" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 7, 1 ) ) + ")";
        break;
    case 2:
        where = "(F.FECHAIVA>=" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 7, 1 ) )
                + ")AND(F.FECHAIVA<" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 10, 1 ) ) + ")";
        break;
    case 3:
        where = "(F.FECHAIVA>=" + ModuleInstance->getConnection()->toSQL( Date( ejercicio, 10, 1 ) )
                + ")AND(F.FECHAIVA<" + ModuleInstance->getConnection()->toSQL( Date( ejercicio+1, 1, 1 ) ) + ")";
        break;
    }
    where += "AND(F.IVADETALLADO<>2)AND(F.FECHAIVA IS NOT NULL)AND(F.EMPRESA_ID=" +
             ModuleInstance->getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) + ")";
    if( tipodoc_ids.size() )
        where += " AND(TIPODOC_ID IN(" + tipodoc_ids.join(",") + "))";
    if( separarivas )
        where += "AND(T.IVA<>0)";
    Xtring from;
    if( devengado ) {
        if( separarivas ) {
            from = " TIPOIVA T INNER JOIN FACTURAVENTADET FD ON FD.TIPOIVA_ID=T.ID"
                   " INNER JOIN FACTURAVENTA F ON FD.FACTURAVENTA_ID=F.ID"
                   " INNER JOIN CLIENTE RS ON RS.ID = F.CLIENTE_ID";
        } else {
            from = "FACTURAVENTA F INNER JOIN CLIENTE RS ON RS.ID = F.CLIENTE_ID";
        }
    } else {
        if( separarivas ) {
            from = " TIPOIVA T INNER JOIN FACTURACOMPRADET FD ON FD.TIPOIVA_ID=T.ID"
                   " INNER JOIN FACTURACOMPRA F ON FD.FACTURACOMPRA_ID=F.ID"
                   " INNER JOIN PROVEEDORA RS ON RS.ID = F.PROVEEDORA_ID";
        } else {
            from = "FACTURACOMPRA F INNER JOIN PROVEEDORA RS ON RS.ID = F.PROVEEDORA_ID";
        }
    }
    Xtring rtkstring = readRTK( "mod303.inc" );
    if( separarivas )
        rtkstring = "#define SEPARARIVAS\n" + rtkstring;
    if( !rtkstring.isEmpty() ) {
        rtkstring += "\n\tINPUT default {\n"
                     "\t\tfrom = \"" + from + "\";\n"
                     "\t\twhere = \"" + where + "\";\n";
        if( separarivas )
            rtkstring += "\t\tgroupby = \"F.ID,T.ID\";\n";
        rtkstring += "\t}" // Si esto no se añade, peta el report->fixInputParameters
                     "}\n"; // y si no se añade esto, da error de syntaxis en la última línea

        AppReport *report = new AppReport(*DBAPP);
        report->readString( rtkstring.c_str() );
        report->setParameterValue( "EMPRESA", empresa::ModuleInstance->getNombreEmpresa() );
        report->setParameterValue( "IVADETALLADO_RECARGO", Xtring::number( FldIVADetallado::con_recargo ) );
        Dictionary<Xtring> properties;
        properties.insert( "TITLE", getTitle() );
        report->print(RTK_Screen, properties, Xtring(), Xtring(),
                      DBAPP->getAppSetting( "RTK.LANDSCAPE" ).toBool() ? Landscape : DefaultOrientation, false);
    }
}

} // namespace cats
} // namespace gong

