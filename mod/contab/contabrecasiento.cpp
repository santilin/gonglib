/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de Asientos
// MEMBER isValid
// MEMBER toString
// RELATION empresa::Proyecto
// RELATION Apunte Detalles
// TYPE dbRecord contab::Asiento
/*>>>>>MODULE_INFO*/

/*<<<<<ASIENTO_INCLUDES*/
#include "contabrecasiento.h"
/*>>>>>ASIENTO_INCLUDES*/
#include "contabrecapunte.h"
#include "contabmodule.h"

namespace gong {
namespace contab {

void RecAsiento::init()
{
    addStructuralFilter( "ASIENTO.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
    addStructuralFilter( "ASIENTO.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<ASIENTO_RELATIONS*/
empresa::RecProyecto *RecAsiento::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecApunte *RecAsiento::getRecApunte( int apunte ) const
{
	return static_cast<RecApunte*>(findRelationByRelatedTable("APUNTE" )->getRelatedRecord( apunte));
}

dbRecordList *RecAsiento::getListApunte() const
{
	return findRelationByRelatedTable( "APUNTE" )->getRelatedRecordList();
}
/*>>>>>ASIENTO_RELATIONS*/

void RecAsiento::borrarDebeYHaberApuntes()
{
    for( unsigned int i=0; i< getApuntes()->size(); i++ ) {
        RecApunte *apunte = static_cast<RecApunte *>(getApuntes()->at(i));
        apunte->setValue("DEBE", 0.0);
        apunte->setValue("HABER", 0.0);
        apunte->setValue("DESCRIPCION", Xtring());
    }
    setValue("DEBE", 0.0);
    setValue("HABER", 0.0);
    setValue("SALDO", 0.0);
}

Money RecAsiento::updateSaldo()
{
    Money debe = 0.0, haber = 0.0;
    for( unsigned int i=0; i< getApuntes()->size(); i++ ) {
        RecApunte *apunte = static_cast<RecApunte *>(getApuntes()->at(i));
        debe = debe + apunte->getValue("DEBE").toMoney();
        haber = haber + apunte->getValue("HABER").toMoney();
    }
    setValue("DEBE", debe);
    setValue("HABER", haber);
    setValue("SALDO", debe-haber);
    return debe-haber;
}

unsigned int RecAsiento::numeraApuntes()
{
    unsigned int i;
    for( i=0; i< getApuntes()->size(); i++ ) {
        RecApunte *apunte = static_cast<RecApunte *>(getApuntes()->at(i));
        apunte->setValue("NUMAPUNTE", i+1 );
    }
    return i;
}

unsigned int RecAsiento::fixConceptos()
{
    unsigned int i = 0;
    for( i=0; i< getApuntes()->size(); i++ ) {
//         RecApunte *apunte = static_cast<RecApunte *>(getApuntes()->at(i));
    }
    return i;
}


Money RecAsiento::cuadraUltimoApunte()
{
    Money debe = 0.0, haber = 0.0, saldo = 0.0;
    unsigned int i;
    RecApunte *apunte = 0;
    for( i=0; i< getApuntes()->size(); i++ ) {
        apunte = static_cast<RecApunte *>(getApuntes()->at(i));
        debe = debe + apunte->getValue("DEBE").toMoney();
        haber = haber + apunte->getValue("HABER").toMoney();
    }
    if( apunte ) {
        saldo = debe-haber;
        if( saldo <= 0 ) {
            apunte->setValue( "DEBE", -saldo );
            apunte->setValue( "HABER", 0 );
        } else {
            apunte->setValue( "DEBE", 0 );
            apunte->setValue( "HABER", saldo );
        }
    }
    return saldo;
}

void RecAsiento::addApunte(const RecApunte *apunte, int pos)
{
    getApuntes()->addRecord(apunte, pos);
    numeraApuntes();
}

/*<<<<<ASIENTO_ISVALID*/
bool RecAsiento::isValid(ValidResult::Context context)
{
	bool ret = dbRecord::isValid(context, result);
/*>>>>>ASIENTO_ISVALID*/
    if( getValue("FECHA").toDate().getYear() != empresa::ModuleInstance->getEjercicio() ) {
        result->addError( Xtring::printf("La fecha no es del ejercicio actual: %d",
                                         empresa::ModuleInstance->getEjercicio() ), "FECHA" );
        ret = false;
    }
    if( uint(getValue("TIPOASIENTO").toInt()) == 0 ) {
        result->addError( "El asiento no tiene un tipo definido", "TIPOASIENTO" );
        if( context == ValidResult::fixing )
            setValue("TIPOASIENTO", 0);
    }
    if( context == ValidResult::fixing ) {
        if( getValue( "CONTADOR" ).toInt() == 0 )
            setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
        updateSaldo();
    }
    return ret;
}

/*<<<<<ASIENTO_TOSTRING*/
Xtring RecAsiento::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>ASIENTO_TOSTRING*/
    if( format == TOSTRING_USER )
        result = Xtring::printf( "%d, %s", getValue("NUMASIENTO").toInt(), getValue("FECHA").toString().c_str() );
    else
        result = dbRecord::toString(format, includedFields);
    return result;
}

void RecAsiento::rescateValues(RecAsiento* other)
{
    if( other ) {
        setValue("CONTADOR", other->getValue("CONTADOR"));
        if( getValue("NUMASIENTO").toInt() == 0 )
            setValue("NUMASIENTO", other->getValue("NUMASIENTO") );
        if( getValue("TIPOASIENTO").toInt() == 0 )
            setValue("TIPOASIENTO", other->getValue("TIPOASIENTO") );
        if( getValue("NOTAS").isEmpty() )
            setValue("NOTAS", other->getValue("NOTAS") );
        if( getValue("CONTADORNUMDOCUMENTO").isEmpty() )
            setValue("CONTADORNUMDOCUMENTO", other->getValue("CONTADORNUMDOCUMENTO") );
    }
}

// int RecAsiento::getLastNumDocumento(int proyecto_id) const
// {
// 	return selectNextInt("CONTADORNUMDOCUMENTO", "PROYECTO_ID=" + getConnection()->toSQL( proyecto_id ) );
// }


} // namespace contab
} // namespace gong


