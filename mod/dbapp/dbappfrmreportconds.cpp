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
    pTabWidget->setTabText( 0, toGUI( _("&Condiciones") ) );
    showTabs( true );

    QWidget *tabOptions = new QWidget( this, "tabOpciones" );
    QVBoxLayout *tabOptionsLayout = new QVBoxLayout(tabOptions);
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
    pEditTitulo = addInput( 0, "Título", titulo, Xtring::null, 0, tabOptionsLayout);
    pComboOutputs = addComboBoxInt(false, tabOptions, _("Destino: "), soutputs, IntList(),
                                   Xtring::null, 0, tabOptionsLayout, true );
    pCheckResumido = addCheckBox( tabOptions, _("Resumido"), false, 0, tabOptionsLayout );
    pCheckApaisado = addCheckBox( tabOptions, _("Apaisado"),
                                  DBAPP->getAppSetting( "RTK.LANDSCAPE" ).toBool(), 0, tabOptionsLayout );
    QSpacerItem *vertSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    tabOptionsLayout->addItem(vertSpacer);
    pTabWidget->insertTab( tabOptions, toGUI( _( "&Opciones" ) ) );
}

Xtring FrmReportConds::readRTK( const Xtring &_template )
{
    Xtring paths = DBAPP->getReportsPath( true );
    Xtring filename = FileUtils::findInPath( paths, _template );
    Xtring rtkstring = FileUtils::readFile( filename );
    if( rtkstring.isEmpty() ) {
        filename = FileUtils::findInPath( paths, _template + ".inc" );
        rtkstring = FileUtils::readFile( filename );
    }
    if( rtkstring.isEmpty() ) {
        FrmBase::msgError(this, Xtring::printf(_("Imposible encontrar el fichero '%s' en\n%s"),
                                               _template.c_str(), paths.c_str() ));
    }
    return rtkstring;
}


} // namespace gong


