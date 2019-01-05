
#ifndef GENCPPCLASS_H
#define GENCPPCLASS_H


#include "capelmodule.h"


/**
 *@author Francisco Santiago Capel Torres
 */

/**
 
	flags:
		0x0001  - Don't generate set
		0x0002  - Set argument is a const reference
		0x0004  - Don't generate get
		0x0008  - Get default value is a const reference
*/
namespace capel {

enum CppFlags {
    NONE					= 0x0000,
    NO_SET					= 0x0001,
	NO_SET_IMPL             = 0x0002,
    NO_GET					= 0x0004,
	NO_GET_IMPL             = 0x0008,
	CONSTRUCTOR_ARG			= 0x0010,
	SET_CONST_REFERENCE     = 0x0020,
    GET_CONST_REFERENCE     = 0x0040,
	CONST_REFERENCE         = SET_CONST_REFERENCE | GET_CONST_REFERENCE,
	NULLABLE                = 0x0080,
	PROPERTY_NONULL 		= 0x0100,
	PROPERTY 				= PROPERTY_NONULL | NULLABLE
};

class GetSetArray {
public:
    int flags;
    Xtring sType;
    Xtring sName;
    Xtring sDefValue;
    Xtring sToFunc;
    Xtring sDoc;
	bool hasGet() { return ((flags & NO_GET) == 0 ); }
	bool hasSet() { return ((flags & NO_SET) == 0 ); }
	bool hasGetImpl() { return ((flags & NO_GET_IMPL) == 0 ); }
	bool hasSetImpl() { return ((flags & NO_SET_IMPL) == 0 ); }
	bool isNullable() { return ((flags & NULLABLE) != 0 ); }
	bool isProperty() { return ((flags & PROPERTY_NONULL) != 0 ); }
	bool isConstructorArg() { return ((flags & CONSTRUCTOR_ARG) != 0 ); }
	
	Xtring getStringToFunc() const {
		if( !sToFunc.size() )
			return Xtring("stringTo") + "<" + sType + ">";
		else
			return sToFunc;
	}
	Xtring getReturnType() const {
        if( (flags & GET_CONST_REFERENCE) || ( (flags & PROPERTY ) && sType=="QString" ) )
			return Xtring("const ") + sType + "&";
        else
            return sType;
    }
	Xtring getTemplateType() const {
		Xtring rtype =  getReturnType() + ", ";
		if( sType == "const char *" )
			return rtype + "null_char_ptr";
		else
			return rtype + sDefValue;
    }
    Xtring getArgumentType() const {
        if( (flags & GET_CONST_REFERENCE) || ( (flags & PROPERTY ) && sType=="QString" ) )
			return Xtring("const ") + sType + "&";
        else
            return sType;
    }
	Xtring getVariantTo(const Xtring &varname = Xtring::null) const;
};

class RTKCppClass {
public:
    RTKCppClass(const Xtring &path = ""): mPath(path) {}
    ;
    virtual ~RTKCppClass();
	/** Crea el constructor */
	void addConstructor(const Xtring &filename, const Xtring &classname,
						GetSetArray *array, int nelem, 	bool implementation = true);
	/** Crea una serie de get/set de clases */
	void addGetSet(const Xtring &filename, const Xtring &classname,
                        GetSetArray *array, int nelem, 	bool implementation = true);
	/** Crea las propiedades */
	virtual void addProperties(const Xtring &filename, const Xtring &classname,
                        GetSetArray *array, int nelem);
						
    virtual void addDerivedConstructor(const Xtring &filename,
                                       const Xtring &basename, GetSetArray *arraybase, int nelembase,
                                       const Xtring &classname, GetSetArray *array, int nelem,
									   bool implementation = true);
	virtual void addDebugPrint(const Xtring &filename, const Xtring &classname,
                               GetSetArray *array, int nelem);

    virtual void addTests(const Xtring &filename,
                          const Xtring &classname, GetSetArray *array, int nelem);
	virtual Xtring addOneTest(const Xtring &/*classname*/, GetSetArray */*array*/) { return ""; }
	
protected:
	Xtring equivType(const Xtring &type);
	Xtring printfType(const Xtring &type);
	Xtring getDefaultLiteral(Xtring ctype, Xtring literal);
	Xtring mPath;
};

#endif

}
