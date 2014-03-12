
#include "dbappfrmcsvparams.h"

namespace gong {
	
	
FrmCsvParams::FrmCsvParams(FrmCsvParams::Modes mode, QWidget* parent, const Xtring& title, 
						   const Xtring& separator, const Xtring& delimiter, const Xtring& encoding)
	: FrmCustom(parent, "FrmCsvParams" ), pEditExportFilename(0), pEditImportFilename(0)
{
	setTitle( title );
	if( mode == exporting ) {
		pEditExportFilename = addFileNameBox( parent, _("Fichero de destino") );
	}
	if( mode == importing ) {
		pEditImportFilename = addFileNameBox( parent, _("Fichero de origen") );
	}
	pEditSeparator = addInput( parent, _("Separador de campos"), "," );
	pEditDelimiter = addInput( parent, _("Delimitador de campos"), "\"" );
	pEditEncoding = addInput( parent, _("Codificación de caracteres"), "UTF-8" );
	pEditEncoding->setEnabled(false);
}
	
bool FrmCsvParams::validate() 
{
	if( pEditImportFilename && pEditImportFilename->getFileName().isEmpty() ) {
		msgError( this, _("Elige un fichero desde el que importar") );
		return false;
	} else if( pEditExportFilename && pEditExportFilename->getFileName().isEmpty() ) {
		msgError( this, _("Elige un fichero al que importar") );
		return false;
	}
	return true;
}
	
	
} // namespace gong
