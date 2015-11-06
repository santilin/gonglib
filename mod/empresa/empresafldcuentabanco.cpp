/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa una cuenta bancaria
// MEMBER isValid
// TYPE dbFieldDefinition empresa::CuentaBanco NoConstr
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<DBFIELD_CUENTABANCO_INCLUDES*/
#include "empresafldcuentabanco.h"
/*>>>>>DBFIELD_CUENTABANCO_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<DBFIELD_CUENTABANCO_ISVALID*/
bool FldCuentaBanco::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_CUENTABANCO_ISVALID*/
    if ( !dbFieldDefinition::isValid( r, value, context, integres ) )
        return false;
    Xtring cuenta = value->toString().replace(" ", "");
	if( cuenta.isEmpty() )
		return true;
	bool esiban = false;
	if( cuenta.length() > 2 && isalpha(cuenta[0]) && isalpha(cuenta[1]) ) {
		esiban = true;
	} else if( cuenta.length() == 20 ) { // Cuenta sin IBAN
		esiban = false;
		for ( unsigned int i = 0; i < cuenta.length(); i++ ) {
			if ( !isdigit( cuenta[i] ) ) {
				if( integres )
					integres->addError( "Las cuentas bancarias sólo pueden contener dígitos.",
										getName() );
				return false;
			}
		}
	} else {
		if( integres )
			integres->addError( _("Las cuentas bancarias deben tener 20 dígitos (españolas y sin IBAN) o 24 dígitos (IBAN)"),
								getName() );
		return false;
	}	
	if( (esiban && cuenta.startsWith("ES")) || !esiban ) {
		Xtring dc = cuenta.mid( (esiban ? 12 : 8), 2 );
		if ( dc.toInt() != calcDigitosControl( cuenta ) ) {
			if( integres )
				integres->addError(
					Xtring::printf( _("El dígito de control del CCC: %s es erróneo (debería ser %d)"),
									dc.c_str(), calcDigitosControl( cuenta ) ), getName() );
			return false;
		}
	} 
	if( esiban ) {
		Xtring dc_calculado, error;
		if( !checkCodigoIBAN( cuenta, dc_calculado, error ) ) {
			if( integres )
				integres->addError( error, getName() );
			return false;
		}
	}
    return true;
}

/*
	http://www.bes.es/pdf/aeb/aeb_19.pdf
	Ejemplo correcto: 00720101930000122351
*/
int FldCuentaBanco::modulo11( const Xtring & digitos )
{
    static int pesos[] = {6, 3, 7, 9, 10, 5, 8, 4, 2, 1 };
    if ( digitos.length() != 8 && digitos.length() != 10 )
        return -10000;
    int peso = 0;
    for ( int i = digitos.length() - 1, p = 0; i >= 0; --i, ++p )
        peso += int( digitos[i] - '0' ) * pesos[p];
    peso = 11 - ( peso % 11 );
    if ( peso == 10 )
        peso = 1;
    else if ( peso == 11 )
        peso = 0;
    return peso;
}

int FldCuentaBanco::calcDigitosControl( const Xtring & cuenta )
{
	int dig1, dig2;
	if( cuenta.length() == 20 ) {
		dig1 = modulo11( cuenta.left( 8 ) );
		dig2 = modulo11( cuenta.mid( 10 ) );
	} else {
		dig1 = modulo11( cuenta.mid( 4, 8 ) );
		dig2 = modulo11( cuenta.mid( 14 ) );
	}
    return dig1 * 10 + dig2;
}

/* Calculate the mod 97 of an arbitrarily large number (as a string). */
int FldCuentaBanco::modulo97(const Xtring &ibanstring)
{
   //implements a stepwise check for mod 97 in chunks of 9 at the first time
   // , then in chunks of seven prepended by the last mod 97 operation converted
   //to a string
   int segstart = 0 ;
   int step = 9 ;
   Xtring prepended ;
   long number = 0 ;
   while ( segstart  < ibanstring.length( ) - step ) {
      number = Xtring(prepended + ibanstring.mid( segstart , step )).toLong() ;
      int remainder = number % 97 ;
      prepended =  Xtring::number( remainder ) ;
      if ( remainder < 10 ) 
	 prepended = "0" + prepended ;
      segstart = segstart + step ;
      step = 7 ;
   }
   number = Xtring(prepended + ibanstring.substr( segstart )).toLong() ;
   return (number % 97) ;
}


bool FldCuentaBanco::checkCodigoIBAN( const Xtring &cuenta, Xtring &dc, Xtring &error_message )
{
   typedef struct { const char *country; int length; } country_info;
   static country_info ci[] =
			{ {"AL" , 28} , {"AD" , 24} , {"AT" , 20} , {"AZ" , 28 } ,
			   {"BE" , 16} , {"BH" , 22} , {"BA" , 20} , {"BR" , 29 } ,
			   {"BG" , 22} , {"CR" , 21} , {"HR" , 21} , {"CY" , 28 } ,
			   {"CZ" , 24} , {"DK" , 18} , {"DO" , 28} , {"EE" , 20 } ,
			   {"FO" , 18} , {"FI" , 18} , {"FR" , 27} , {"GE" , 22 } ,
				{"DE" , 22} , {"GI" , 23} , {"GR" , 27} , {"GL" , 18 } ,
				{"GT" , 28} , {"HU" , 28} , {"IS" , 26} , {"IE" , 22 } , 
			   {"IL" , 23} , {"IT" , 27} , {"KZ" , 20} , {"KW" , 30 } ,
			   {"LV" , 21} , {"LB" , 28} , {"LI" , 21} , {"LT" , 20 } , 
			   {"LU" , 20} , {"MK" , 19} , {"MT" , 31} , {"MR" , 27 } , 
			   {"MU" , 30} , {"MC" , 27} , {"MD" , 24} , {"ME" , 22 } , 
			   {"NL" , 18} , {"NO" , 15} , {"PK" , 24} , {"PS" , 29 } , 
			   {"PL" , 28} , {"PT" , 25} , {"RO" , 24} , {"SM" , 27 } , 
			   {"SA" , 24} , {"RS" , 22} , {"SK" , 24} , {"SI" , 19 } , 
			   {"ES" , 24} , {"SE" , 24} , {"CH" , 21} , {"TN" , 24 } ,
			   {"TR" , 26} , {"AE" , 23} , {"GB" , 22} , {"VG" , 24 } } ;
   Xtring teststring( cuenta.upper() ) ;
   teststring.remove_white_space();
   // buscar el país
   Xtring pais = teststring.substr(0,2);
   uint i;
   for( i=0; i<sizeof(ci); ++i) {
	   if( strcmp(pais.c_str(), ci[i].country) == 0 ) {
		   break;
	   }
   }
   if( i == sizeof(ci) ) {
	   if( &error_message != &Xtring::null ) 
		   error_message = Xtring::printf(_("Código de país erróneo: %s"), pais.c_str());
	   return false;
   }
   if ( teststring.length( ) != ci[ i ].length ) {
	   if( &error_message != &Xtring::null ) 
		   error_message = Xtring::printf(_("Longitud de la cuenta errónea, debe de ser: %d"), ci[ i ].length);
	   return false;
   }
   teststring = teststring.mid(4) + teststring.mid(0,2) + "00";
 
   Xtring numberstring ;//will contain the letter substitutions
   for (int c=0; c<teststring.length(); ++c) {
	   char ch = teststring[c];
      if (std::isdigit(ch)) 
		numberstring += ch;
      if (std::isupper(ch)) 
		numberstring += Xtring::number(static_cast<int>(ch) - 55);
   }
   int idc = 98 - modulo97(numberstring);
   Xtring sdc(Xtring::number(idc));
   if( (idc) < 10 )
	   sdc = "0" + sdc;
   if (sdc != cuenta.mid(2,2)) {
	   if( &error_message != &Xtring::null ) 
		   error_message = Xtring::printf(_("Dígitos de control erróneos, deberían ser %s"), sdc.c_str());
	   return false;
   } else {
		return true;
   }
}

} // Namespace empresa
} // Namespace gong
