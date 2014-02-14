#include "config.h"
#include <gonggettext.h>
#include <gongdbdefinition.h>
#include <gongdbfieldemail.h>
#include <gongcsvutils.h>
#include <dbappdbapplication.h>
#include <dbappsmtpmailsender.h>
#include "contactosmodule.h"
#include "contactosfrmmailing.h"

using Poco::Net::MailRecipient;

namespace gong {
namespace contactos {

FrmMailing::FrmMailing( QWidget* parent, WidgetFlags fl )
    : FrmCustom( parent, "Contactos::FrmMailing", fl )
{
    Xtring titulo = _("Envío de correos electrónicos masivos");
    setTitle( titulo );
	pushAccept->setText( _("Enviar") );

    tabFrameEdit = new QTabWidget( pControlsFrame );
    pControlsLayout->addWidget( tabFrameEdit );
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

	tabMailsExtra = new QWidget( tabFrameEdit, "tabMailsExtra" );
	QVBoxLayout *extraLayout = new QVBoxLayout( tabMailsExtra );
	pMailsExtra = addTextEditBox( tabMailsExtra, _("Mails extra"), Xtring::null, 0, extraLayout );
	QLabel *me_instrucciones = new QLabel( tabMailsExtra );
	me_instrucciones->setText( toGUI( _("Escribe o pega aquí otras direcciones de correo, una por línea") ) );
	extraLayout->addWidget( me_instrucciones );
    tabFrameEdit->addTab( tabMailsExtra, toGUI( _("&Más emails") ) );

    tabContenido = new QWidget( tabFrameEdit, "tabContenido" );
    QVBoxLayout *contLayout = new QVBoxLayout( tabContenido );
	pFrom = addInput(tabContenido, _("Remitente"),
					 ModuleInstance->getModuleSetting( "SMTP_FROM" ), "STRING", 0, contLayout );
    pSubject = addInput(tabContenido, _("Asunto"), Variant(), "STRING", 0, contLayout );
	pBody = addTextEditBox( tabContenido, _("Cuerpo"), Xtring::null, 0, contLayout );
    pHTMLBody = addRichTextBox(tabContenido, _("Cuerpo HTML"), 0, contLayout );
	pAttachment = addFileNameBox( tabContenido, _("Adjunto"), 0, contLayout );
    tabFrameEdit->addTab( tabContenido, toGUI( _("&Contenido") ) );

    tabConfiguracion = new QWidget( tabFrameEdit, "tabConfiguracion" );
    QVBoxLayout *confLayout = new QVBoxLayout( tabConfiguracion );
    pHost = addInput(tabConfiguracion, _("Servidor SMTP"),
					 ModuleInstance->getModuleSetting( "SMTP_HOST"), "STRING", 0, confLayout );
    pUser = addInput(tabConfiguracion, _("Usuaria"),
					 ModuleInstance->getModuleSetting( "SMTP_USER"), "STRING", 0, confLayout );
    pPassword = addInput(tabConfiguracion, _("Contraseña"), Variant(), "PASSWORD", 0, confLayout );
    pPort = addInput(tabConfiguracion, _("Puerto"),
					 ModuleInstance->getModuleSetting( "SMTP_PORT").toInt(), "INTEGER", 0, confLayout );
    pGrouping = addInput(tabConfiguracion, _("Número de destinatarios a agregar en cada email"),
					 0, "INTEGER", 0, confLayout );
	pCheckSaveSettings = addCheckBox( this, _("Guardar datos de conexión"), true, 0, confLayout );
    tabFrameEdit->addTab( tabConfiguracion, toGUI( _("&Servidor SMTP") ) );

	tabResultado = new QWidget( tabFrameEdit, "tabResultado" );
    QVBoxLayout *resLayout = new QVBoxLayout( tabResultado );
	pResultado = addTextEditBox( tabResultado, _("Resultado"), Xtring::null, 0, resLayout );
	pOks = addTextEditBox( tabResultado, _("Correctos"), Xtring::null, 0, resLayout );
	pErrors = addTextEditBox( tabResultado, _("Errores"), Xtring::null, 0, resLayout );
	lblProgreso = new QLabel( tabResultado );
	resLayout->addWidget( lblProgreso );
	tabFrameEdit->addTab( tabResultado, toGUI( _("Resultado") ) );
	if( mSearchBoxes.size() )
		pFocusWidget = pFrom; // mSearchBoxes[0]->getEditCode();
#if !HAVE_POCOLIB
	msgError( this, _("No se pueden enviar emails, la biblioteca POCO no está instalada") );
	pushAccept->setEnabled( false );
#endif
}

void FrmMailing::validate_input(QWidget* sender, bool* isvalid)
{
	_GONG_DEBUG_TRACE(0);
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
    // Extra emails
    XtringList extraemails;
	pMailsExtra->toString().tokenize( extraemails, ",;\n" );
	for( XtringList::const_iterator it = extraemails.begin(); it != extraemails.end(); ++ it ) {
		Xtring email, contact;
		Xtring(*it).trim().splitIn2( contact, email, "<" );
		if( email.isEmpty() ) {
			addEmailToList( list, contact, Xtring::null, false );
		} else {
			if( email.endsWith(">") )
				email = email.mid( 0, contact.size() - 1 );
			addEmailToList( list, email, contact, include_names );
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
#if !HAVE_POCOLIB
	msgError( this, _("No se pueden enviar emails, la biblioteca POCO no está instalada") );
#else
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
	uint grouping = static_cast<uint>(pGrouping->toInt());
    if(  grouping < 1 || grouping > 100 ) {
		msgError( this, _("El número de direcciones por email tiene que estar entre 1 y 100") );
		return;
	}
	tabFrameEdit->setCurrentIndex( 4 );
	DBAPP->processEvents();
	DBAPP->waitCursor( true );
	XtringList emails;
	pErrors->clear();
	pResultado->clear();
	pOks->clear();
	DBAPP->processEvents();
	SMTPMailSender s( pHost->toString(), pPort->toInt(), pUser->toString(), pPassword->toString() );
	if( !s.open() ) {
		addMessage( pErrors, _("Error: " + s.getError()) );
	} else {
		getEmailsList( emails, false );
		uint groupcount = 0, totalcount = 0, okcount = 0, errorcount = 0;
		bool isHTML = !pHTMLBody->toString().isEmpty();
		Poco::Net::MailMessage *message = 0;
		bool do_send = false;
		Xtring recipients;
		for( XtringList::const_iterator it = emails.begin(); it != emails.end(); ++ it ) {
			++totalcount;
			bool sent = false;
			if( grouping <= 1 ) {
				message = s.createMessage( pFrom->toString(), *it, pSubject->toString(),
					isHTML ? pHTMLBody->toString() : pBody->toString(), isHTML );
				recipients = *it;
				do_send = true;
			} else {
				if( message == 0 ) {
					message = s.createMessage( pFrom->toString(), Xtring::null, pSubject->toString(),
						isHTML ? pHTMLBody->toString() : pBody->toString(), isHTML );
				}
				message->addRecipient( MailRecipient(MailRecipient::BCC_RECIPIENT, *it ) );
				if( !recipients.isEmpty() )
					recipients.append( ";" );
				recipients.append( *it );
				if( ++groupcount == grouping || totalcount == emails.size() ) {
					groupcount = 0;
					do_send = true;
				}
			}
			if( do_send ) {
				sent = s.sendMessage( message );
				delete message;
				message = 0;
				do_send = false;
				if( sent ) {
					okcount++;
					addMessage( pResultado, Xtring::printf( _("%s: Ok\n"), recipients.c_str() ) );
					addMessage( pOks, Xtring::printf( _("%s\n"), Xtring(recipients).replace(";","\n").c_str() ) );
				} else {
					errorcount++;
					addMessage( pResultado, Xtring::printf( _("%s: Error: %s\n"), recipients.c_str(), s.getError().c_str() ) );
					addMessage( pErrors, Xtring::printf( _("%s\n"), Xtring(recipients).replace(";","\n").c_str() ) );
				}
				recipients.clear();
				lblProgreso->setText( toGUI( Xtring::printf( _("Enviando %d correos a %d destinatarios. %d correos correctos, %d correos con errores"),
														  emails.size(), emails.size()/grouping, okcount, errorcount ) ) );
			}
		}
		s.close();
		if( pCheckSaveSettings->isChecked() ) {
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_FROM", pFrom->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_HOST", pHost->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_USER", pUser->toString() );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_PORT", pPort->toInt() );
		} else {
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_FROM", Xtring::null );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_HOST", Xtring::null );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_USER", Xtring::null );
			DBAPP->setUserLocalSetting( "MODULE.CONTACTOS.SMTP_PORT", 0 );
		}
		if( errorcount == 0) {
			msgOk( this, _("La operación se ha completado con éxito") );
			pushAccept->setEnabled( false );
		} else {
			pushAccept->setEnabled( true );
		}
	}
	DBAPP->resetCursor();
#endif
}

void FrmMailing::addMessage(TextEdit* dest, const Xtring& message)
{
	dest->moveCursor(QTextCursor::End);
	dest->insertPlainText( toGUI( message ) );
	DBAPP->processEvents();
}


} // namespace contactos
} // namespace gong

