/*<<<<<MODULE_INFO*/
// COPYLEFT Fichero de edición de Cuentas
// FIELD Debe Money tabExtracto saldoLayout
// FIELD Haber Money tabExtracto saldoLayout
// FIELD Saldo Money tabExtracto saldoLayout
// FIELD Contacto_ID Reference(contactos::Contacto,CIF,Nombre,dbApplication::InsertIfNotFound) - contactoLayout
// FIELD Cuenta String - cuentaLayout
// FIELD Descripcion String - cuentaLayout
// FIELD Manual bool - tipoLayout
// FIELD Notas text - notasLayout
// TYPE FrmEditRecMaster contab::Cuenta
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITCUENTA_INCLUDES*/
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "contabfrmeditcuenta.h"
/*>>>>>FRMEDITCUENTA_INCLUDES*/
#include "contabmodule.h"
#include "contabcuenta.h"
#include "contabrecasiento.h"
#include "contabfrmeditasiento.h"

namespace gong {
namespace contab {

/*<<<<<FRMEDITCUENTA_CONSTRUCTOR*/
FrmEditCuenta::FrmEditCuenta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
	                               EditMode editmode, dbApplication::EditFlags editflags,
	                               QWidget *parent, const char* name, WidgetFlags fl )
	    : FrmEditRecMaster( parentfrm, master, dm, editmode, editflags, parent, name, fl )
{
	if ( !name )
	    setName( "FrmEditCuenta" );
/*>>>>>FRMEDITCUENTA_CONSTRUCTOR*/

    /*<<<<<FRMEDITCUENTA_INIT_CONTROLS*/
	showTabs(true);
	QWidget *tabExtracto = new QWidget( pTabWidget, "tabExtracto" );
	QVBoxLayout *tabExtractoLayout = new QVBoxLayout(tabExtracto, 11, 6, "tabExtractoLayout");
	QHBoxLayout *saldoLayout = new QHBoxLayout(0, 0, 6, "saldoLayout");
	QHBoxLayout *contactoLayout = new QHBoxLayout(0, 0, 6, "contactoLayout");
	QHBoxLayout *cuentaLayout = new QHBoxLayout(0, 0, 6, "cuentaLayout");
	QHBoxLayout *tipoLayout = new QHBoxLayout(0, 0, 6, "tipoLayout");
	QHBoxLayout *notasLayout = new QHBoxLayout(0, 0, 6, "notasLayout");
	editDebe = addEditField( tabExtracto, "CUENTA", "DEBE", saldoLayout );
	editHaber = addEditField( tabExtracto, "CUENTA", "HABER", saldoLayout );
	editSaldo = addEditField( tabExtracto, "CUENTA", "SALDO", saldoLayout );

	searchContactoCIF = addSearchField( pControlsFrame, "CONTACTO_ID", "CONTACTO", "CIF", "NOMBRE", contactoLayout );
	pushContactoCIF = searchContactoCIF->getButton();
	connect( pushContactoCIF, SIGNAL( clicked() ), this, SLOT( pushContactoCIF_clicked() ) );
	editContactoCIF = searchContactoCIF->getEditCode();
	editContactoNombre = searchContactoCIF->getEditDesc();
	editCuenta = addEditField( pControlsFrame, "CUENTA", "CUENTA", cuentaLayout );
	editDescripcion = addEditField( pControlsFrame, "CUENTA", "DESCRIPCION", cuentaLayout );
	checkManual = addCheckField( pControlsFrame, "CUENTA", "MANUAL", tipoLayout );
	editNotas = addTextField( pControlsFrame, "CUENTA", "NOTAS", notasLayout );
	tabExtractoLayout->addLayout( saldoLayout );
	pControlsLayout->addLayout( contactoLayout );
	pControlsLayout->addLayout( cuentaLayout );
	pControlsLayout->addLayout( tipoLayout );
	pControlsLayout->addLayout( notasLayout );
/*>>>>>FRMEDITCUENTA_INIT_CONTROLS*/
    // No se genera automáticamente porque usa getViewsByName
    RecAsiento *asiento = static_cast<RecAsiento*>(ModuleInstance->createRecord("ASIENTO"));
    dbViewDefinitionDict asientoviews;
    ModuleInstance->getDatabase()->getViewsByName( "ASIENTO._EXTRACTO_", asientoviews );
    dbRecordDataModel *asientodm = new dbRecordDataModel(asiento, asientoviews, "1=0");
    pFrmAsiento = static_cast<FrmEditAsiento *>(
                      ModuleInstance->createEditForm(this, asiento, asientodm, DataTable::browsing,
                              static_cast<dbApplication::EditFlags>(dbApplication::embedded + dbApplication::tableEditEvent ),
                              tabExtracto) );
    pFrmAsiento->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding);
    tabExtractoLayout->addWidget(pFrmAsiento);
    tabExtracto->show();

#if 0
    // Eliminar la cuenta de las vistas
    for( dbViewDefinitionDict::iterator itview = asientoviews.begin();
            itview != asientoviews.end();
            ++ itview ) {
        dbViewDefinition *view = (*itview).second;
        for( dbFieldDefinitionDict::const_iterator itfld = view->getFieldDefinitions().begin();
                itfld != view->getFieldDefinitions().end();
                ++itfld ) {
            dbFieldDefinition *flddef = (*itfld).second;
            if( flddef->getTableName().upper() == "CUENTA" )
                flddef->setVisible( false );
        }
    }
#endif
    pTabWidget->addTab( tabExtracto, "&Extracto" );
}

void FrmEditCuenta::scatterFields()
{
    if( isDuplicating() )
        getRecCuenta()->resetSaldos();
    // Quitar el valor por defecto del cif del contacto
    if( getRecContacto()->isNew() )
        getRecContacto()->setValue("CIF", "");
    /*<<<<<FRMEDITCUENTA_SCATTER*/
	editDebe->setText(getRecCuenta()->getValue("DEBE").toMoney());
	if( isEditing() && (pFocusWidget == 0) )
		pFocusWidget = editDebe;
	editHaber->setText(getRecCuenta()->getValue("HABER").toMoney());
	editSaldo->setText(getRecCuenta()->getValue("SALDO").toMoney());
	editCuenta->setText(getRecCuenta()->getValue("CUENTA").toString());
	editDescripcion->setText(getRecCuenta()->getValue("DESCRIPCION").toString());
	checkManual->setChecked(getRecCuenta()->getValue("MANUAL").toBool());
	editNotas->setText(getRecCuenta()->getValue("NOTAS").toString());
	scatterContacto();
/*>>>>>FRMEDITCUENTA_SCATTER*/
    if( isInserting() ) {
        getRecord()->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
        if( isDuplicating() ) {
            // Calcular la siguiente cuenta del grupo
            Cuenta cuenta(editCuenta->toString(), ModuleInstance->getDigitosTrabajo());
            if( cuenta.isGrupo() ) {
                cuenta += ".";
                cuenta.expandir();
            }
            while( getRecord()->getConnection()->selectInt("SELECT COUNT(*) FROM CUENTA"
                    + getRecCuenta()->getFilter( "WHERE",
                                                 "cuenta=" + getRecord()->getConnection()->toSQL(cuenta) ) ) != 0 )
                cuenta = cuenta.sigCuentaDiario();
            editCuenta->setText(cuenta);
        }
    } else if( !isDeleting() ) {
        pFrmAsiento->setFormFilter( "APUNTE.CUENTA_ID="
                                    + getRecord()->getConnection()->toSQL(getRecCuenta()->getRecordID()) );
        // Si no, no se llama al showEvent que es el que hace el setdatamodel
        pTabWidget->setCurrentPage( 1 );
    }
    if( !pFrmAsiento->getDataTable()->numRows() ) {
        pTabWidget->setCurrentPage( 0 );
        pFocusWidget = editCuenta;
    } else {
        pTabWidget->setCurrentPage( 1 );
        pFocusWidget = pFrmAsiento;
    }
}

void FrmEditCuenta::gatherFields()
{
    /*<<<<<FRMEDITCUENTA_GATHER*/
	getRecCuenta()->setValue( "DEBE", editDebe->toMoney());
	getRecCuenta()->setValue( "HABER", editHaber->toMoney());
	getRecCuenta()->setValue( "SALDO", editSaldo->toMoney());
	getRecCuenta()->setValue( "CONTACTO_ID", getRecContacto()->getRecordID() );
	getRecCuenta()->setValue( "CUENTA", editCuenta->toString());
	getRecCuenta()->setValue( "DESCRIPCION", editDescripcion->toString());
	getRecCuenta()->setValue( "MANUAL", checkManual->isChecked());
	getRecCuenta()->setValue( "NOTAS", editNotas->toString());
/*>>>>>FRMEDITCUENTA_GATHER*/
}

bool FrmEditCuenta::remove()
{
    Cuenta cuenta( editCuenta->toString(), contab::ModuleInstance->getDigitosTrabajo() );
    cuenta.expandir();
    if( cuenta.isGrupo() && !cuenta.isEmpty() ) { // No borrar si tiene cuentas hijas
        if( getRecord()->getConnection()->selectInt("SELECT COUNT(*) FROM CUENTA"
                " WHERE ejercicio=" + getRecord()->getConnection()->toSQL(empresa::ModuleInstance->getEjercicio())
                + " AND " + getRecord()->getConnection()->toSQLStartLike("CUENTA", cuenta)
                + " AND CUENTA <>" + getRecord()->getConnection()->toSQL(cuenta) ) != 0 ) {
            msgError(this, Xtring::printf("La cuenta %s tiene cuentas hijas", cuenta.c_str()));
            return false;
        }
    }
    return FrmEditRec::remove();
}

void FrmEditCuenta::validateFields(QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITCUENTA_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
	if( !sender && !pRecord->isValid( ValidResult::editing, validresult ) )
			*isvalid = false;
	if( focusWidget() != pushContactoCIF) // To avoid triggering the validating if the button is pressed
	if( validSeekCode( sender, isvalid, *validresult, editContactoCIF, editContactoNombre,
		getRecContacto(), "CIF", "NOMBRE", Xtring::null, dbApplication::SeekCodeFlags( dbApplication::InsertIfNotFound )) )
		scatterContacto();
/*>>>>>FRMEDITCUENTA_VALIDATE*/
    if ( sender == editContactoCIF && editContactoCIF->isJustEdited()
            && editDescripcion->toString().isEmpty() ) {
        editDescripcion->setText( editContactoNombre->toString() );
        editDescripcion->setJustEdited( true );
    }
    Cuenta cuenta( editCuenta->toString(), contab::ModuleInstance->getDigitosTrabajo() );
    if( sender == editCuenta ) {
        cuenta.expandir();
        editCuenta->setText(cuenta);
        if( getRecord()->existsAnother("cuenta", cuenta) ) {
            validresult->addWarning( Xtring::printf("El campo '%s' no puede estar duplicado",
                                                    ModuleInstance->getDatabase()->findFieldDefinition("CUENTA", "cuenta")->getCaption().c_str() ), "CUENTA_ID");
            *isvalid = false;
        }
    }

    if( sender == editDescripcion ) {
        Cuenta padre = cuenta.getPadre();
        Xtring cond = "descripcion=" + getRecord()->getConnection()->toSQL( editDescripcion->toString() );
        if( !padre.isEmpty() )
            cond = "(" + cond + " AND " + getRecord()->getConnection()->toSQLLike("cuenta", padre)
                   + " AND cuenta!=" + getRecord()->getConnection()->toSQL(padre) + ")";
        if( getRecord()->existsAnother(cond) ) {
            validresult->addWarning( Xtring::printf("El campo '%s' no debería estar duplicado dentro del grupo %s.",
                                                    ModuleInstance->getDatabase()->findFieldDefinition("CUENTA", "descripcion")->getCaption().c_str(), padre.c_str() ),
                                     "CUENTA_ID");
        }
    }
// 	if( !comboTipo->toString().isEmpty() && comboTipo->isNewItem() && !sender ) {
// 		if( msgYesNo( this, _("Atención"),
// 			Xtring::printf(_("El valor '%s' del desplegable es nuevo, ¿quieres continuar?"),
// 				comboTipo->toString().c_str() ) ) ) {
// 			ModuleInstance->getListaTiposCuenta() << comboTipo->toString();
// 			comboTipo->insertItem( comboTipo->toString(), comboTipo->toString() );
// 		} else {
// 			validresult->addMessage(INTEG_INVALID, ValidResult::invalid,
// 				_("Grabación cancelada"), "TIPO");
// 			*isvalid = false;
// 		}
// 	}
    if (!ir) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

void FrmEditCuenta::scatterContacto()
{
    /*<<<<<FRMEDITCUENTA_SCATTER_CONTACTO*/
	editContactoCIF->setText( getRecContacto()->getValue("CIF") );
	editContactoNombre->setText( getRecContacto()->getValue("NOMBRE") );
/*>>>>>FRMEDITCUENTA_SCATTER_CONTACTO*/
}
void FrmEditCuenta::pushContactoCIF_clicked()
{
    /*<<<<<FRMEDITCUENTA_PUSH_CONTACTO_CIF_CLICKED*/
	char action = mControlKeyPressed;
	if( !isEditing() || searchContactoCIF->mustBeReadOnly() )
		action = 'E';
	switch( action ) {
		case 'F':
		case '\0':
			editContactoCIF->setJustEdited( false );
			editContactoCIF->setCancelling();
			if( DBAPP->choose(this, getRecContacto(), 0, dbApplication::editNone, this ) ) {
				setEdited(true);
				scatterContacto();
				editContactoCIF->setJustEdited( true );
				setWiseFocus(editContactoCIF);
			}
			break;
		case 'M':
			{
				if( getRecContacto()->getRecordID() ) {
					editContactoCIF->setJustEdited( false );
					if( DBAPP->editRecord(this,
							getRecContacto(), 0, DataTable::updating,
							dbApplication::simpleEdition, this ) ) {
						editContactoCIF->setJustEdited( true );
						scatterContacto();
					}
				setWiseFocus(editContactoCIF);
				}
			}
			break;
		case 'E':
			{
				if( getRecContacto()->getRecordID() != 0 ) {
					editContactoCIF->setJustEdited( false );
					DBAPP->getMainWindow()->createClient( DBAPP->createEditForm(this, getRecContacto(),
						0, DataTable::selecting, dbApplication::simpleEdition, this ) );
				}
			}
			break;
		case 'A':
			{
				contactos::RecContacto *tmprec = static_cast<contactos::RecContacto *>(DBAPP->createRecord( "Contacto" ));
				editContactoCIF->setJustEdited( false );
				tmprec->clear( true ); // set default values
				DBAPP->setCodeNotFound( editContactoCIF->toString() );
				if( DBAPP->editRecord(this, tmprec, 0, DataTable::inserting,
					dbApplication::simpleEdition, this ) ) {
					editContactoCIF->setJustEdited( true );
					getRecContacto()->copyRecord( tmprec );
					scatterContacto();
				}
				setWiseFocus(editContactoCIF);
				DBAPP->setCodeNotFound( Xtring() );
			}
			break;
	}
/*>>>>>FRMEDITCUENTA_PUSH_CONTACTO_CIF_CLICKED*/
}
void FrmEditCuenta::specialControlKeyPressed(QWidget *sender, char key)
{
    /*<<<<<FRMEDITCUENTA_SPECIALACTION*/
	mControlKeyPressed = key;
	FrmEditRecMaster::specialControlKeyPressed(sender,key); // calls the behaviors
	if( sender == editContactoCIF )
		pushContactoCIF_clicked();
	mControlKeyPressed = '\0';
/*>>>>>FRMEDITCUENTA_SPECIALACTION*/
}


/*<<<<<FRMEDITCUENTA_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITCUENTA_FIN*/
