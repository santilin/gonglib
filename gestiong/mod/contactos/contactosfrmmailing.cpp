#include <gonggettext.h>
#include <gongdbdefinition.h>
#include <gongcsvutils.h>
#include <dbappdbapplication.h>
#include "contactosmodule.h"
#include "contactosfrmmailing.h"

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
    tabFrameEdit->addTab( tabSeleccion, _("&Destinatarias") );

    tabContenido = new QWidget( tabFrameEdit, "tabContenido" );
    QVBoxLayout *contLayout = new QVBoxLayout( tabContenido );
    pSubject = addInput(tabContenido, _("Asunto"), Variant(), "STRING", 0, contLayout );
    pBody = addRichTextBox(tabContenido, _("Cuerpo"), 0, contLayout );
    tabFrameEdit->addTab( tabContenido, _("&Contenido") );

    tabConfiguracion = new QWidget( tabFrameEdit, "tabConfiguracion" );
    QVBoxLayout *confLayout = new QVBoxLayout( tabConfiguracion );
    pHost = addInput(tabConfiguracion, _("Servidor SMTP"), Variant(), "STRING", 0, confLayout );
    pUser = addInput(tabConfiguracion, _("Usuaria"), Variant(), "STRING", 0, confLayout );
    pPassword = addInput(tabConfiguracion, _("Contraseña"), Variant(), "PASSWORD", 0, confLayout );
    pPort = addInput(tabConfiguracion, _("Puerto"), Variant(), "INTEGER", 0, confLayout );
    tabFrameEdit->addTab( tabConfiguracion, _("&Servidor SMTP") );
    pushShowEMails = addButton( tabFrameEdit, _("Mostrar emails" ), 0, selLayout );
    pushShowEMailsAndNames = addButton( tabFrameEdit, _("Mostrar nombres y emails" ), 0, selLayout );
}

void FrmMailing::validate_input(QWidget* sender, bool* isvalid)
{
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


} // namespace contactos
} // namespace gong

