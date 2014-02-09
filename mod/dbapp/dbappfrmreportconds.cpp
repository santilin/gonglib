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


	QFrame *frameOptions = new QFrame( this );

    XtringList soutputs;
    soutputs << _("Pantalla")
		<< _("Impresora")
		<< _("PDF")
		<< _("LibreOffice Calc")
		<< _("CSV (OpenOffice, Excel, ...)" )
		<< _("PostScript")
		<< _("Fichero de texto")
		<< _("GNUmeric")
		<< _("HTML")
		<< _("XML");
    pComboOutputs = addComboBoxInt(false, frameOptions, _("Destino: "), soutputs, IntList() );
    pCheckResumido = addCheckBox( frameOptions, _("Resumido"), false );
    pCheckCSV = addCheckBox( frameOptions, _("Salida a CSV, OpenOffice Calc, etc."), false );
	pInputsLayout->addWidget( frameOptions );
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


