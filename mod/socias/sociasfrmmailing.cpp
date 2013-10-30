#include <sociasfrmmailing.h>
#include <sociasmodule.h>

namespace gong {
namespace socias {

FrmMailing::FrmMailing( QWidget* parent, WidgetFlags fl )
    : contactos::FrmMailing( parent, fl )
{
    QWidget *tabSeleccion = static_cast<QWidget *>(child("tabSeleccion"));
    pSearchMiembro = addMultipleSearchField( tabSeleccion, "MIEMBRO", "NUMEROSOCIA",
                     "CLIENTE.RAZONSOCIAL",
                     dynamic_cast<QHBoxLayout *>(tabSeleccion->layout()) );
}

int FrmMailing::getEmailsList( XtringList &emails, bool include_names ) const
{
    dbConnection *conn = ModuleInstance->getConnection();
    contactos::FrmMailing::getEmailsList( emails, include_names);
    List<dbRecordID> &ids = pSearchMiembro->getRecordIDs();
    if( ids.size() ) {
        Xtring sql = "SELECT DISTINCT CONTACTO.EMAIL, CONTACTO.NOMBRE FROM MIEMBRO"
#ifdef HAVE_FACTUMODULE
                     " INNER JOIN CLIENTE ON MIEMBRO.CLIENTE_ID=CLIENTE.ID"
                     " INNER JOIN CONTACTO ON CLIENTE.CONTACTO_ID=CONTACTO.ID";
#else
                     " INNER JOIN CONTACTO ON MIEMBRO.CONTACTO_ID=CONTACTO.ID";
#endif
        sql += " WHERE CONTACTO.EMAIL IS NOT NULL"
               " AND CONTACTO.EMAIL <> '' AND MIEMBRO.ID IN(" + ids.join(",") + ")";
        dbResultSet *rs = conn->select (sql );
        while( rs->next() ) {
            Xtring email = rs->toString(0);
            Xtring nombre = rs->toString(1);
            addEmailToList( emails, email, nombre, include_names );
        }
        delete rs;
    }
    return emails.size();
}



} // namespace contactos
} // namespace gong

