#include <gonggettext.h>
#include <gongdbdefinition.h>
#include <gongcsvutils.h>
#include <dbappdbapplication.h>
#include "contactossmtpmailsender.h"
#include "contactosmodule.h"
#include "contactosfrmmailing.h"
#include "../gonglib/gongdbfieldemail.h"

namespace gong {
namespace contactos {

FrmMailing::FrmMailing( QWidget* parent, WidgetFlags fl )
    : FrmCustom( parent, "Contactos::FrmMailing", fl )
{
    Xtring titulo = _("Envío de correos electrónicos masivos");
    setTitle( titulo );

    tabFrameEdit = new QTabWidget( pFrameEdit );
    pInputsLayout->addWidget( tabFrameEdit );
    tabSeleccion = new QWidget( tabFrameEdit, "tabSeleccion" );
    QVBoxLayout *selLayout = new QVBoxLayout( tabSeleccion );
    mSearchBoxes << addMultipleSearchField( tabSeleccion, "CONTACTO", "CIF", "NOMBRE", selLayout );
    // Buscar todos los ficheros que tengan relación con contactos
    for( dbTableDefinitionsList::const_iterator it = DBAPP->getDatabase()->getTables().begin();
            it != DBAPP->getDatabase()->getTables().end();
            ++ it ) {
        dbTableDefinition *tbldef = it->second;
        if( tbldef->findFieldDefinition("CONTACTO_ID") ) {
            Xtring code = tbldef->findFieldByFlags(dbFieldDefinition::CODE) ?
                          tbldef->findFieldByFlags(dbFieldDefinition::CODE)->getName() : "CODIGO";
            Xtring desc = tbldef->findFieldByFlags(dbFieldDefinition::DESCRIPTION) ?
                          tbldef->findFieldByFlags(dbFieldDefinition::DESCRIPTION)->getName() : "DESCRIPCION";
            mSearchBoxes << addMultipleSearchField( tabSeleccion, tbldef->getName(), code, desc, selLayout );
        }
    }
	pushShowEMails = addButton( tabFrameEdit, _("Mostrar emails" ), 0, selLayout );
    pushShowEMailsAndNames = addButton( tabFrameEdit, _("Mostrar nombres y emails" ), 0, selLayout );
    tabFrameEdit->addTab( tabSeleccion, _("&Destinatarias") );

    tabContenido = new QWidget( tabFrameEdit, "tabContenido" );
    QVBoxLayout *contLayout = new QVBoxLayout( tabContenido );
	pFrom = addInput(tabContenido, _("Remitente"), Variant(), "STRING", 0, contLayout );
    pSubject = addInput(tabContenido, _("Asunto"), Variant(), "STRING", 0, contLayout );
	pBody = addTextEditBox( tabContenido, _("Cuerpo"), Xtring::null, 0, contLayout );
    pHTMLBody = addRichTextBox(tabContenido, _("Cuerpo HTML"), 0, contLayout );
    tabFrameEdit->addTab( tabContenido, _("&Contenido") );

    tabConfiguracion = new QWidget( tabFrameEdit, "tabConfiguracion" );
    QVBoxLayout *confLayout = new QVBoxLayout( tabConfiguracion );
    pHost = addInput(tabConfiguracion, _("Servidor SMTP"),
					 ModuleInstance->getModuleSetting( "SMTP_HOST"), "STRING", 0, confLayout );
    pUser = addInput(tabConfiguracion, _("Usuaria"),
					 ModuleInstance->getModuleSetting( "SMTP_USER"), "STRING", 0, confLayout );
    pPassword = addInput(tabConfiguracion, _("Contraseña"), Variant(), "PASSWORD", 0, confLayout );
    pPort = addInput(tabConfiguracion, _("Puerto"),
					 ModuleInstance->getModuleSetting( "SMTP_PORT").toInt(), "INTEGER", 0, confLayout );
	pCheckSaveSettings = addCheckBox( this, _("Guardar datos de conexión"), true, 0, confLayout );
    tabFrameEdit->addTab( tabConfiguracion, _("&Servidor SMTP") );

	tabResultado = new QWidget( tabFrameEdit, "tabResultado" );
    QVBoxLayout *resLayout = new QVBoxLayout( tabResultado );
	pResultado = addTextEditBox( tabResultado, _("Resultado"), Xtring::null, 0, resLayout );
	pErrors = addTextEditBox( tabResultado, _("Errores"), Xtring::null, 0, resLayout );
	tabFrameEdit->addTab( tabResultado, _("Resultado") );
}

void FrmMailing::validate_input(QWidget* sender, bool* isvalid)
{
	pushAccept->setEnabled( true );
    if( sender == pushShowEMails ) {
        XtringList emails;
        getEmailsList( emails, false );
        msgOkLarge( this, Xtring::printf( _("%d emails seleccionados"), emails.size() ),
                    emails.join("\n") );
    } else if( sender == pushShowEMailsAndNames ) {
        XtringList emails;
        getEmailsList( emails, true );
        msgOkLarge( this, Xtring::printf( _("%d emails seleccionados"), emails.size() ),
                    emails.join("\n") );
    }
    FrmCustom::validate_input( sender, isvalid );
}

int FrmMailing::getEmailsList( XtringList &list, bool include_names ) const
{
	DBAPP->waitCursor( true );
    dbConnection *conn = ModuleInstance->getConnection();
    for( List<SearchBox *>::const_iterator sbit = mSearchBoxes.begin();
            sbit != mSearchBoxes.end(); ++sbit ) {
        Xtring tablename = (*sbit)->getTableName();
        List<dbRecordID> &ids = (*sbit)->getRecordIDs();
        if( ids.size() ) {
            Xtring sql = "SELECT DISTINCT CONTACTO.EMAIL, CONTACTO.NOMBRE FROM "
                         +  conn->nameToSQL( tablename );
            if( tablename != "CONTACTO" ) {
                sql += " INNER JOIN CONTACTO ON " + conn->nameToSQL( tablename ) + ".CONTACTO_ID=CONTACTO.ID";
            }
            sql += " WHERE CONTACTO.EMAIL IS NOT NULL"
                   " AND CONTACTO.EMAIL <> '' AND " + conn->nameToSQL( tablename ) + ".ID IN(" + ids.join(",") + ")";
            dbResultSet *rs = conn->select (sql );
            while( rs->next() ) {
                Xtring email = rs->toString(0);
                Xtring nombre = rs->toString(1);
                addEmailToList(list, email, nombre, include_names);
            }
            delete rs;
        }
    }
	DBAPP->resetCursor();
    return list.size();
}

void FrmMailing::addEmailToList(XtringList& list, const Xtring &email,
                                const Xtring &nombre, bool include_names) const
{
    if( email.find( ";" ) != Xtring::npos ) {
        XtringList emails;
        email.tokenize( emails, ";" );
        for( XtringList::const_iterator it = emails.begin(); it != emails.end(); ++ it ) {
            if( include_names )
                list << "\"" + CsvUtils::dupQuotes( nombre, '"') + "\",\"" + CsvUtils::dupQuotes( (*it).trim(), '"' );
            else
                list << (*it).trim();
        }
    } else {
        if( include_names ) {
            list << "\"" + CsvUtils::dupQuotes( nombre, '"' ) + "\",\"" + CsvUtils::dupQuotes( email, '"' );
        } else
            list << email.trim();
    }
}


void FrmMailing::accept()
{
	if( pFrom->toString().isEmpty() ) {
		msgError( this, _("El remitente está vacío. Introduce una dirección de correo.") );
		return;
	}
	EMail e(pFrom->toString() );
	if( !e.isValid() ) {
		msgError( this, _("Introduce una dirección de correo válida en el remitente.") );
		return;
	}
	if( pSubject->toString().isEmpty() ) {
		msgError( this, _("El asunto está vacío.") );
		return;
	}
	if( pBody->toString().isEmpty() && pHTMLBody->toString().isEmpty() ) {
		msgError( this, _("El cuerpo está vacío") );
		return;
	}
	if( pHost->toString().isEmpty() || pUser->toString().isEmpty() || pPassword->toString().isEmpty() ) {
		msgError( this, _("Falta rellenar la configuración del servidor SMTP") );
		return;
	}
	tabFrameEdit->setCurrentIndex( 3 );
	DBAPP->processEvents();
	DBAPP->waitCursor( true );
	pushAccept->setEnabled( true );
	XtringList emails;
	SMTPMailSender s( pHost->toString(), pPort->toInt(), pUser->toString(), pPassword->toString() );
	if( !s.open() ) {
		addMessage( pErrors, _("Error: " + s.getError()) );
	} else {
		getEmailsList( emails, false );
		for( XtringList::const_iterator it = emails.begin(); it != emails.end(); ++ it ) {
			int ret = 0;
			if( pHTMLBody->toString().isEmpty() )
				ret = s.send( pFrom->toString(), *it, pSubject->toString(), pBody->toString() );
			else
				ret = s.sendHTML( pFrom->toString(), *it, pSubject->toString(), pBody->toString() );
			if( ret ) {
				addMessage( pResultado, Xtring::printf( _("%s: Ok\n"), (*it).c_str() ) );
			} else {
				addMessage( pErrors, Xtring::printf( _("%s: Error: %s\n"), (*it).c_str(), s.getError().c_str() ) );
			}
		}
		s.close();
		if( pCheckSaveSettings->isChecked() ) {
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_HOST", pHost->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_USER", pUser->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_PORT", pPort->toInt() );
		} else {
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_HOST", pHost->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_USER", pUser->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_PORT", pPort->toInt() );
		}
		msgOk( this, _("La operación se ha completado con éxito") );
	}
	DBAPP->resetCursor();
}

void FrmMailing::addMessage(TextEdit* dest, const Xtring& message)
{
	dest->moveCursor(QTextCursor::End);
	dest->insertPlainText( toGUI( message ) );
}


} // namespace contactos
} // namespace gong

