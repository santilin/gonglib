/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes
// MEMBER isValid
// MEMBER toString
// MEMBER isEmpty
// MEMBER beforeSaveOrDeleteRelated
// RELATION Cuenta
// TYPE dbRecord contab::Apunte
/*>>>>>MODULE_INFO*/

/*<<<<<APUNTE_INCLUDES*/
#include "contabrecapunte.h"
/*>>>>>APUNTE_INCLUDES*/
#include "contabmodule.h"
#include "contabcuenta.h"

namespace gong {
namespace contab {

/*<<<<<APUNTE_RELATIONS*/
RecCuenta *RecApunte::getRecCuenta() const
{
	return static_cast<RecCuenta*>(findRelatedRecord("CUENTA_ID"));
}

/*>>>>>APUNTE_RELATIONS*/

bool RecApunte::actSaldosCuenta( bool sumar, const dbRecordID cuenta_id,
                                 int ejercicio, Money debeorig, Money haberorig ) const
{
    bool ret = false;
    Money debe = debeorig, haber = haberorig;
    if ( !sumar )
    {
        debe = debe * -1L;
        haber = haber * -1L;
    }
    RecCuenta cuenta(getConnection());
    if ( cuenta.read( cuenta_id ) )
    {
        cuenta.setValue( "DEBE", cuenta.getValue( "DEBE" ).toMoney() + debe );
        cuenta.setValue( "HABER", cuenta.getValue( "HABER" ).toMoney() + haber );
        cuenta.setValue( "SALDO", cuenta.getValue( "SALDO" ).toMoney() + debe - haber );
        ret = cuenta.save(false);
        // Actualización recursiva de la cuenta padre
        Cuenta padre( cuenta.getValue( "CUENTA" ).toString(), contab::ModuleInstance->getDigitosTrabajo() );
        dbRecordID padre_id = 0;
        do
        {
            // Puede haber cuentas que no tengan todas las cuentas padres creadas
            padre = padre.getPadre();
            if ( !padre.isEmpty() )
            {
                padre_id = getConnection()->selectInt( "SELECT id FROM CUENTA"
                                                       + cuenta.getFilter("WHERE", "cuenta=" + getConnection()->toSQL( padre ) ) );
            }
        }
        while ( !padre.isEmpty() && padre_id == 0 );
        if ( padre_id != 0 )
            ret += actSaldosCuenta( sumar, padre_id, ejercicio, debeorig, haberorig );
    }
    return true; // No se puede cancelar la grabación de un apunte porque haya cuentas sin padre
}


/**
 * Redefinida porque se considera vacío un apunte aunque tenga el número de apunte, de modo que no quedan apuntes vacíos cuando se guarda el asiento
 * @return
 */
/*<<<<<APUNTE_ISEMPTY*/
bool RecApunte::isEmpty( const Xtring &nocheck_fields ) const
{
/*>>>>>APUNTE_ISEMPTY*/
    if ( getValue( "CUENTA_ID" ).toInt() != 0 )
        return false;
    if ( getValue( "DEBE" ).toMoney() != 0.0 )
        return false;
    if ( getValue( "HABER" ).toMoney() != 0.0 )
        return false;
    if ( !getValue( "CONCEPTO" ).toString().isEmpty() )
        return false;
    if ( !getValue( "NOTAS" ).toString().isEmpty() )
        return false;
    return true;
}

/*<<<<<APUNTE_ISVALID*/
bool RecApunte::isValid(ValidResult::Context context, ValidResult *result )
{
	bool ret = dbRecord::isValid(context, result);
/*>>>>>APUNTE_ISVALID*/
    return ret;
}

/*<<<<<APUNTE_TOSTRING*/
Xtring RecApunte::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>APUNTE_TOSTRING*/
    result = dbRecord::toString(format, includedFields);
    return result;
}


/*<<<<<APUNTE_BEFORESAVEORDELETERELATED*/
void RecApunte::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>APUNTE_BEFORESAVEORDELETERELATED*/
    actSaldosCuenta( saving,
                     getValue( "CUENTA_ID" ).toInt(),
                     master->getValue( "EJERCICIO" ).toInt(),
                     getValue( "DEBE" ).toMoney(),
                     getValue( "HABER" ).toMoney() );
}

#if 0
bool RecApunteBase::isValid( GIntegResult::severity sev, GIntegResult *result, bool fix, GSqlQuery *q )
{
    bool ret = GSqlRecord::isValid( sev, result, fix, q );
    if ( q )
    {
        RecCuenta cuenta( getValue( "CUENTA", _ID() ) );
        if ( cuenta.read( q ) )
        {
            if ( !cuenta.getValue( "CUENTA", ().isDiario() ) )
            {
                result->addMessage( INTEG_INVALID, GIntegResult::inconsistent,
                                    ("La cuenta '%1' del apunte %2 no puede ser una cuenta de grupo" )
                                    .arg( cuenta.getValue( "CUENTA", () ).arg( getNUMAPUNTE() ),
                                          "Value("CUENTA", _ID" ) );
                ret = false;
            }
        }
        else
        {
            result->addMessage( INTEG_INVALID, GIntegResult::incomplete,
                                ("La %1 no existe" )
                                .arg( GongApp::pGongApp->getDataDictionary() ->getFieldCaption( "cuenta.cuenta" ) ),
                                "Value("CUENTA", _ID" );
            ret = false;
        }
    }

    Cuenta contra = getCONTRAPARTIDA();
    if ( !contra.isEmpty() )
    {
        QString msgvalid;
        if ( !contra.isValid( msgvalid ) )
        {
            bool fixed = false;
            if ( fix )
            {
                contra.contraer();
                contra.expandir();
                fixed = contra.isValid( msgvalid )
                        ;
                if ( fixed )
                    setCONTRAPARTIDA( contra );
            }
            if ( !fixed )
            {
                if ( result )
                    result->addMessage( 100, GIntegResult::invalid, msgvalid, "CONTRAPARTIDA" );
                ret = false;
            }
        }
    }
    return ret;
}
#endif

void RecApunte::quickValues(Cuenta cuenta,const Xtring &desc_cuenta, Money debe, Money haber,
                            const Xtring &concepto, const Xtring &contrapartida )
{
    cuenta.expandir();
    RecCuenta *reccuenta = static_cast<RecCuenta *>(DBAPP->createRecord("CUENTA") );
    dbRecordID cuenta_id = reccuenta->readWithFilter("CUENTA=" + reccuenta->getConnection()->toSQL( cuenta ) );
    if( cuenta_id == 0 ) {
        reccuenta->clear(true);
        reccuenta->setValue( "CUENTA", cuenta );
        if( desc_cuenta.isEmpty() )
            reccuenta->setValue( "DESCRIPCION", "Generada automáticamente" );
        else
            reccuenta->setValue( "DESCRIPCION", desc_cuenta );
        reccuenta->save(false);
    }
    setValue( "CUENTA_ID", reccuenta->getRecordID() );
    getRecCuenta()->copyRecord( reccuenta );
    setValue( "CONTRAPARTIDA", contrapartida );
    setValue( "DEBE", debe );
    setValue( "HABER", haber );
    setValue( "CONCEPTO", concepto );
}

} // namespace contab
} // namespace gong

