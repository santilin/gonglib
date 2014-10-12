/*<<<<<MODULE_INFO*/
// MEMBER findMatchingRecord
// MEMBER toString
// MEMBER save
// MEMBER remove
// RELATION Proyecto
// RELATION pagos::FormaPago MODULE_INCLUDED(Pagos)
// RELATION contactos::Contacto
// RELATION factu::Cliente MODULE_INCLUDED(Factu)
// INTERFACE public IPagableMiembro #ifdef|HAVE_PAGOSMODULE
// RELATION TipoSocia
// TYPE dbRecord socias::Miembro
/*>>>>>MODULE_INFO*/

/*<<<<<MIEMBRO_INCLUDES*/
#include "sociasrecmiembro.h"
/*>>>>>MIEMBRO_INCLUDES*/
#include "sociasmodule.h"


namespace gong {
namespace socias {

int RecMiembro::getLastNumeroSocia(int proyecto_id) const
{
    Xtring where;
    if( proyecto_id )
        where = "WHERE PROYECTO_ID=" + getConnection()->toSQL( proyecto_id );
    return getConnection()->selectInt("SELECT MAX(NUMEROSOCIA) FROM MIEMBRO " + where );
}

/*<<<<<MIEMBRO_RELATIONS*/
RecProyecto *RecMiembro::getRecProyecto() const
{
	return static_cast<RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

#ifdef HAVE_PAGOSMODULE
pagos::RecFormaPago *RecMiembro::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}
#endif

#ifdef HAVE_FACTUMODULE
factu::RecCliente *RecMiembro::getRecCliente() const
{
	return static_cast<factu::RecCliente*>(findRelatedRecord("CLIENTE_ID"));
}
#endif

RecTipoSocia *RecMiembro::getRecTipoSocia() const
{
	return static_cast<RecTipoSocia*>(findRelatedRecord("TIPOSOCIA_ID"));
}

/*>>>>>MIEMBRO_RELATIONS*/
// {capel} eliminar getRecContacto() y los ifs fuera de las funciones

contactos::RecContacto *RecMiembro::getRecContacto() const
{
    return static_cast<contactos::RecContacto*>(findRelatedRecord("CONTACTO_ID"));
}

/// \todo {0.3.4} Asegurarse de que no machacamos a otro miembro. Comprobar el contacto y todo lo que se pueda
/*<<<<<MIEMBRO_FINDMATCHINGRECORD*/
bool RecMiembro::findMatchingRecord(dbRecordID *matchingid) const
{
	bool found;
/*>>>>>MIEMBRO_FINDMATCHINGRECORD*/
    found = dbRecord::findMatchingRecord(matchingid);
    return found;
}

/*<<<<<MIEMBRO_TOSTRING*/
Xtring RecMiembro::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>MIEMBRO_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC ) {
        if( !getRecContacto()->getValue("CIF").toString().isEmpty() )
            result += getRecContacto()->getValue( "CIF" ).toString() + ",";
        result += getRecContacto()->getValue("NOMBRE").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<MIEMBRO_SAVE*/
bool RecMiembro::save(bool saverelated) throw( dbError )
{
/*>>>>>MIEMBRO_SAVE*/
#ifdef HAVE_PAGOSMODULE
    actRestoFactura();
#endif
    bool ret = dbRecord::save(saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
        delCobros(true);
        genCobros();
#endif
    }
    return ret;
}

/*<<<<<MIEMBRO_REMOVE*/
bool RecMiembro::remove() throw( dbError )
{
/*>>>>>MIEMBRO_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        delCobros(true);
#endif
    }
    return ret;
}

bool RecMiembro::fromString(const Xtring& source, int format, const Xtring& includedFields)
{
	return dbRecord::fromString(source, format, includedFields);
	/*
	contactos::RecContacto *contacto = getRecContacto();
	dbRecordID contacto_id = 0;
	if( contacto->fromString(source, format, includedFields ) ) {
		dbRecordID existing_id = 0;
		contacto->findMatchingRecord(&existing_id);
		if( existing_id ) {
			contacto->read( existing_id );
			contacto->fromString( source, format, includedFields);
		}
	}
	if( contacto_id == 0 ) {
		getRecContacto()->setNew( true );
	}
	*/
}

} // namespace socias
} // namespace gong


#if 0
/**
 * Redefinida de FrmEditRec para crear o modificar el asiento asociado al recibo
 * @return
 */
bool FrmEditReciboProyecto::save()
{
    bool ret = FrmEditRecMaster::save();
#ifdef HAVE_CONTABMODULE
    if( ModuleInstance->hasContabilidad() ) {
        if( ret ) {
            dbRecordID asientoid = getRecord()->getValue( "ASIENTO_CARGO_ID" ).toInt();
            // http://www.solucionesong.org/asesoria/consultas/consulta/?id=3545
            Xtring documento = getRecord()->getValue("NUMERO").toString();
            Money importe = getRecRemesaProyecto()->getRecProyecto()->getValue( "IMPORTE" ).toMoney();
            Xtring concepto = Xtring::printf( _("Recibo %s de la remesa %s del proyecto %s"), documento.c_str(),
                                              getRecRemesaProyecto()->getValue( "DESCRIPCION" ).toString().c_str(),
                                              getRecRemesaProyecto()->getRecProyecto()->getValue( "DESCRIPCION" ).toString().c_str() );
            dbRecordID tipodocumentoid = getRecord()->getConnection()->selectInt(
                                             "SELECT ID FROM TIPODOC WHERE CODIGO=" + getRecord()->getConnection()->toSQL(
                                                 ModuleInstance->getModuleSetting( "CODIGO_TIPODOC_RECIBO" ).toInt() ) );
            if( (asientoid = creaAsiento( asientoid,
                                          "RECIBOPROYECTO.ID=" + Xtring::number(getRecord()->getRecordID()), tipodocumentoid, documento,
                                          creaCuentaSocia(), importe, "721.1", importe, concepto, mPartidaProyectoID ) ) != 0 ) {
                getRecord()->setValue( "ASIENTO_CARGO_ID", asientoid );
                getRecord()->save(false); // no guardar las relaciones
            }
        }
    }
#endif
    return ret;
}

#ifdef HAVE_CONTABMODULE
dbRecordID FrmEditReciboProyecto::creaAsiento(dbRecordID asientoid,
        const Xtring &origen, dbRecordID tipodocumentoid, const Xtring &documento,
        const Xtring &cuentadebe, Money debe, const Xtring &cuentahaber, Money haber,
        const Xtring &concepto, dbRecordID partidaid)
{
    contab::RecAsiento *asiento = static_cast<contab::RecAsiento *>(DBAPP->createRecord( "ASIENTO" ));
    if( asiento->read( asientoid) ) { /// \todo 0.4 Generar el asiento y compararlo con el que ya existe.
        asiento->remove();
    }
    asiento->setValue( "EJERCICIO", empresa::ModuleInstance->getEjercicio() );
    asiento->setValue( "FECHA", getRecMiembro()->getValue( "FECHAALTA" ).toDate() );
    asiento->setValue( "TIPO", 0 );
    asiento->setValue( "TIPODOC_ID", tipodocumentoid );
// 	asiento->getRecTipoDoc()->read( tipodocumentoid );
    asiento->setValue( "REC_ORIGEN", origen);
    asiento->setValue( "NUMDOCUMENTO", documento);
    RecApunte *apuntedebe = static_cast<RecApunte *>(DBAPP->createRecord( "APUNTE" ));
// 	apuntedebe->quickValues( contab::Cuenta(cuentadebe, contab::ModuleInstance->getDigitosTrabajo()),
// 									concepto, debe, 0, contab::Cuenta(cuentahaber, contab::ModuleInstance->getDigitosTrabajo()) );
    apuntedebe->setValue( "PARTIDAPROYECTO_ID", partidaid);
    apuntedebe->getRecPartidaProyecto()->read( partidaid );
    asiento->addApunte( apuntedebe );
    RecApunte *apuntehaber = static_cast<RecApunte *>(DBAPP->createRecord( "APUNTE" ));
// 	apuntehaber->quickValues( contab::Cuenta(cuentahaber, contab::ModuleInstance->getDigitosTrabajo()),
// 							  concepto, 0, haber, contab::Cuenta(cuentadebe, contab::ModuleInstance->getDigitosTrabajo()) );
    asiento->addApunte( apuntehaber );

    FrmEditRec *frmeditasiento = DBAPP->createEditForm(this,
                                 asiento, 0, DataTable::inserting, dbApplication::simpleEdition, this );
    if( mSupervisando )
        frmeditasiento->showModalFor( this, true, true );
    else {
        frmeditasiento->show();
        frmeditasiento->accept();
    }
    if( frmeditasiento->isSaved() ) {
        DBAPP->getMainWindow()->refreshByName( Xtring::null, Xtring::null );
        asientoid = asiento->getRecordID();
    }
    delete frmeditasiento;
    delete asiento;
    return asientoid;
}

Xtring FrmEditReciboProyecto::creaCuentaSocia()
{
    contab::Cuenta socia( "447." + getRecMiembro()->getRecContacto()->getValue( "CODIGO" ).toString(),
                          contab::ModuleInstance->getDigitosTrabajo() );
    socia.expandir();
    dbRecordID cuentaid = getRecord()->getConnection()->selectInt(
                              "SELECT ID FROM CUENTA WHERE CUENTA=" + getRecord()->getConnection()->toSQL( socia )
                              + " AND EJERCICIO=" + getRecord()->getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    if( cuentaid == 0 ) {
        if( mSupervisando )
            msgOk( this, Xtring::printf( _("La cuenta %s del miembro %s no existe."), socia.c_str(),
                                         getRecMiembro()->getRecContacto()->getValue( "NOMBRE" ).toString().c_str() ) );
        contab::RecCuenta *reccuenta = static_cast<contab::RecCuenta *>(DBAPP->createRecord( "CUENTA" ));
        reccuenta->setValue( "CUENTA", socia );
        reccuenta->setValue( "DESCRIPCION",
                             getRecMiembro()->getRecContacto()->getValue( "NOMBRE" ).toString() );
        reccuenta->setValue( "CONTACTO_ID", getRecMiembro()->getValue( "CONTACTO_ID" ) );
        /// \todo readRelated() requiere que el registro no sea nuevo, por lo tanto no lo puedo usar aquí.
        reccuenta->getRecContacto()->read ( reccuenta->getValue( "CONTACTO_ID" ).toInt() );
        FrmEditRec *frmeditcuenta = DBAPP->createEditForm(this,
                                    reccuenta, 0, DataTable::inserting, dbApplication::simpleEdition, this );
        if( mSupervisando )
            frmeditcuenta->showModalFor( this, true, true );
        else {
            frmeditcuenta->show();
            frmeditcuenta->accept();
        }
        delete frmeditcuenta;
        delete reccuenta;
    }
    return socia;
}
#endif

/**
 * Redefinida de FrmEditRec para eliminar el asiento asociado al recibo
 * @return
 */
bool FrmEditReciboProyecto::remove()
{
    if( getRecord()->getValue( "ESTADO" ).toInt() == SociasModule::pagado ) {
        msgOk( this, _("Este recibo está pagado. Anula el pago antes de eliminarlo.") );
        return false;
    }
    bool removemiembro = true;
#ifdef HAVE_CONTABMODULE
    if( ModuleInstance->hasContabilidad() ) {
        dbRecordID asientoid = getRecord()->getValue( "ASIENTO_CARGO_ID" ).toInt();
        if( asientoid ) {
            contab::RecAsiento *asiento = static_cast<contab::RecAsiento *>(DBAPP->createRecord( "ASIENTO" ));
            if( asiento->read( asientoid ) ) {
                switch( msgYesNoCancel( this,
                                        Xtring::printf( _( "Este recibo está contabilizado en el asiento %d de fecha %s.\n"
                                                        "¿Quieres borrar el asiento asociado a este recibo?"),
                                                        asiento->getValue( "NUMASIENTO" ).toInt(),
                                                        asiento->getValue( "FECHA" ).toString().c_str() ) ) ) {
                case FrmEditRec::Yes: {
                    try {
                        if( !asiento->remove() )
                            removemiembro = false;
                        else
                            DBAPP->getMainWindow()->refreshByName( Xtring::null, Xtring::null );
                    } catch( dbError &e ) {
                        msgError( this,
                                  Xtring(e.what()) + _(". Borra el asiento a mano y vuelve a intentarlo."));
                        removemiembro = false;
                    }
                    break;
                    case FrmEditRec::Cancel:
                        return false;
                    }
                }
            }
        }
    }
#endif
    if( removemiembro )
        return FrmEditRecMaster::remove();
    else
        return false;
}
#endif
