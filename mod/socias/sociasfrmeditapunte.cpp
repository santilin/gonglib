/*<<<<<COPYLEFT*/
/** @file sociasfrmeditapunte.cpp Fichero de edición de apuntes
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de apuntes
// FIELD PartidaProyecto_ID Reference(PartidaProyecto,Codigo,Descripcion)
// Inherit contab::Apunte
// TYPE FrmEditRecDetail socias::Apunte
/*>>>>>MODULE_INFO*/

#include "config.h"
#ifdef HAVE_CONTABMODULE
/*<<<<<FRMEDITAPUNTE_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sociasfrmeditapunte.h"
/*>>>>>FRMEDITAPUNTE_INCLUDES*/
#include "contabfrmeditapunte.h"

namespace gong {
namespace socias {

/*<<<<<FRMEDITAPUNTE_CONSTRUCTOR*/
FrmEditApunte::FrmEditApunte(FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm,
	EditMode editmode, dbApplication::EditFlags editflags,
	QWidget* parent, const char* name, WidgetFlags fl )
		: contab::FrmEditApunte( frmmaster, ndetail, detail, dettablename, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditApunte" );
/*>>>>>FRMEDITAPUNTE_CONSTRUCTOR*/
    /*<<<<<FRMEDITAPUNTE_INIT_CONTROLS*/
	QHBoxLayout *partidaproyecto_idLayout = new QHBoxLayout(0, 0, 6, "partidaproyecto_idLayout");

	searchPartidaProyectoCodigo = addSearchField( pControlsFrame, "PARTIDAPROYECTO_ID", "PARTIDAPROYECTO", "CODIGO", "DESCRIPCION", partidaproyecto_idLayout );
	pushPartidaProyectoCodigo = searchPartidaProyectoCodigo->getButton();
	connect( pushPartidaProyectoCodigo, SIGNAL( clicked() ), this, SLOT( pushPartidaProyectoCodigo_clicked() ) );
	editPartidaProyectoCodigo = searchPartidaProyectoCodigo->getEditCode();
	editPartidaProyectoDescripcion = searchPartidaProyectoCodigo->getEditDesc();
	pControlsLayout->addLayout( partidaproyecto_idLayout );
/*>>>>>FRMEDITAPUNTE_INIT_CONTROLS*/
}

void FrmEditApunte::scatterPartidaProyecto()
{
    /*<<<<<FRMEDITAPUNTE_SCATTER_PARTIDAPROYECTO*/
	editPartidaProyectoCodigo->setText( getRecPartidaProyecto()->getValue("CODIGO") );
	editPartidaProyectoDescripcion->setText( getRecPartidaProyecto()->getValue("DESCRIPCION") );
/*>>>>>FRMEDITAPUNTE_SCATTER_PARTIDAPROYECTO*/
}


void FrmEditApunte::scatterFields()
{
    /*<<<<<FRMEDITAPUNTE_SCATTER*/
	contab::FrmEditApunte::scatterFields();
	scatterPartidaProyecto();
/*>>>>>FRMEDITAPUNTE_SCATTER*/
}

void FrmEditApunte::gatherFields()
{
    /*<<<<<FRMEDITAPUNTE_GATHER*/
	contab::FrmEditApunte::gatherFields();
	getRecApunte()->setValue( "PARTIDAPROYECTO_ID", getRecPartidaProyecto()->getRecordID() );
/*>>>>>FRMEDITAPUNTE_GATHER*/
}

void FrmEditApunte::pushPartidaProyectoCodigo_clicked()
{
    /*<<<<<FRMEDITAPUNTE_PUSH_PARTIDAPROYECTO_CODIGO_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchPartidaProyectoCodigo->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editPartidaProyectoCodigo->setJustEdited( false );
			editPartidaProyectoCodigo->setCancelling();
			if( DBAPP->choose(this, getRecPartidaProyecto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterPartidaProyecto();
				editPartidaProyectoCodigo->setJustEdited( true );
				editPartidaProyectoCodigo->setFocus();
			}
			break;
		case 'M':
			{
				if( getRecPartidaProyecto()->getRecordID() ) {
					editPartidaProyectoCodigo->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecPartidaProyecto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editPartidaProyectoCodigo->setJustEdited( true );
						scatterPartidaProyecto();
					}
					editPartidaProyectoCodigo->setFocus();
				}
			}
			break;
		case 'E':
			{
				if( getRecPartidaProyecto()->getRecordID() != 0 ) {
					editPartidaProyectoCodigo->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecPartidaProyecto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				RecPartidaProyecto *tmprec = static_cast<RecPartidaProyecto *>(DBAPP->createRecord( "PartidaProyecto" ));
				editPartidaProyectoCodigo->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editPartidaProyectoCodigo->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editPartidaProyectoCodigo->setJustEdited( true );
					getRecPartidaProyecto()->copyRecord( tmprec );
					scatterPartidaProyecto();
				}
				editPartidaProyectoCodigo->setFocus();
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITAPUNTE_PUSH_PARTIDAPROYECTO_CODIGO_CLICKED*/
}

void FrmEditApunte::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITAPUNTE_SPECIALACTION*/
	mControlKeyPressed = key;
	contab::FrmEditApunte::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editPartidaProyectoCodigo )
		pushPartidaProyectoCodigo_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITAPUNTE_SPECIALACTION*/
}

void FrmEditApunte::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITAPUNTE_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	contab::FrmEditApunte::validateFields(sender, isvalid, validresult);
	if( focusWidget() != pushPartidaProyectoCodigo) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editPartidaProyectoCodigo, editPartidaProyectoDescripcion,
		getRecPartidaProyecto(), "CODIGO", "DESCRIPCION", Xtring::null) )
		scatterPartidaProyecto();
/*>>>>>FRMEDITAPUNTE_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITAPUNTE_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITAPUNTE_FIN*/


#endif
