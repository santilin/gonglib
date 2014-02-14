#include <fstream>
#include <gongfileutils.h>
#include <dbappreport.h>
#include "dbappfrmreportconds.h"

namespace gong {

FrmReportConds::FrmReportConds(const Xtring &titulo,
							   QWidget * parent, WidgetFlags fl)
    : FrmCustom(parent, "FrmReportConds", fl)
{
	setTitle( titulo );
    pEditTitulo = addInput( 0, "Título", titulo );
	showTabs( true );

	QWidget *tabOptions = new QWidget( this, "tabOpciones" );
	/*QVBoxLayout *tabOptionsLayout = */new QVBoxLayout(tabOptions);
    XtringList soutputs;
    soutputs << _("Pantalla")
		<< _("Impresora (directo)")
		<< _("Impresora (con diálogo)")
		<< _("PDF")
		<< _("LibreOffice Calc")
		<< _("CSV (OpenOffice, Excel, ...)" )
		<< _("PostScript")
		<< _("Fichero de texto")
		<< _("GNUmeric");
    pComboOutputs = addComboBoxInt(false, tabOptions, _("Destino: "), soutputs, IntList() );
    pCheckResumido = addCheckBox( tabOptions, _("Resumido"), false );
    pCheckApaisado = addCheckBox( tabOptions, _("Apaisado"), false );
    pTabWidget->insertTab( tabOptions, toGUI( _( "&Destino" ) ) );
}

Xtring FrmReportConds::readRTK( const Xtring &_template )
{
    Xtring paths = DBAPP->getReportsPath( true );
    Xtring filename = FileUtils::findInPath( paths, _template );
    Xtring rtkstring = FileUtils::readFile( filename );
    if( rtkstring.isEmpty() ) {
        FrmBase::msgError(this, Xtring::printf(_("Imposible encontrar el fichero %s en\n%s"),
                                               _template.c_str(), paths.c_str() ));
        return Xtring();
    }
    return rtkstring;
}


} // namespace gong


