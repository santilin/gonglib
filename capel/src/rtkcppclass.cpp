
#include "capelmodule.h"
#include "rtkcppclass.h"

namespace capel {

RTKCppClass::~RTKCppClass() {}

Xtring RTKCppClass::equivType(const Xtring &type) {
    if( type == "Variant" || type == "GVariant" )
        return "variant";
    else if( type == "Double" || type == "Measure" )
        return "double";
	else if( type == "Xtring" || type == "QString" || type == "QString&" || type == "Xtring&")
        return "string";
    else if( type == "Bool" )
        return "bool";
    else
        return type;
}

Xtring RTKCppClass::printfType(const Xtring &type) {
    Xtring eqtype = equivType(type);
    if( eqtype == "int" || eqtype == "bool" )
        return "%d";
    else if( eqtype == "double" || eqtype == "float" )
        return "%f";
    else if( eqtype == "string" || eqtype == "char *" )
        return "%s";
    else
        return "%d";
}


Xtring RTKCppClass::getDefaultLiteral(Xtring type, Xtring literal) {
    Xtring CType = equivType(type).lower();
    if( CType == "string" )
        return "\"" + literal + "\"";
    else if( CType == "numeric" || CType == "money" || CType == "bool" )
        return literal;
    else if( CType == "date" )
        return "QDateTime()";
    else if( CType == "int" )
        return literal;
    else
        return "getDefaultLiteral::Desconocido: " + type;
}


/** Crea el constructor de la clase */
void RTKCppClass::addConstructor(const Xtring &filename, const Xtring &classname,
                                 GetSetArray *array, int nelem, bool implementation)
{
	CapelModule *cm = new CapelModule(mPath + filename + ".h");
    Xtring s = "public:\n";
    s+="\t" + classname + "(";
    for( int i=0; i<nelem; i++) {
        if( array[i].isConstructorArg() ) {
            if( i>0 )
                s+=", ";
            if( (i%3 == 2) && (i<nelem-1))
                s+="\n\t\t";
            s+= array[i].getReturnType() + " " + array[i].sName.lower();
            if( array[i].sDefValue != "" )
                s += " = " + array[i].sDefValue;
        }
    }
    s+=")\n";
    if( implementation ) {
        s += "\t\t: ";
        for( int i=0; i<nelem; i++) {
            if( array[i].isNullable() || array[i].isConstructorArg() ) {
                if( i>0 )
                    s+=", ";
                if( (i%2 == 1) && (i<nelem-1))
                    s+="\n\t\t";
                if( array[i].isNullable() )
                    s += "mNull" + array[i].sName + "(true)";
                if( array[i].isConstructorArg() )
                    s+= "m" + array[i].sName + "(" + array[i].sName.lower() + ")";
            }
        }
        cm->insert_extrusion(classname.upper() + "_CONSTRUCTOR", s, "\n\t{}\n");
    } else {
        s += ";\n";
        cm->insert_extrusion(classname.upper() + "_CONSTRUCTOR", s);
    }
    cm->writeIfModified();
}

/** Crea el constructor de la clase con referencia a otra clase base */
void RTKCppClass::addDerivedConstructor(const Xtring &filename,
                                        const Xtring &basename, GetSetArray *arraybase, int nelembase,
                                        const Xtring &classname, GetSetArray *array, int nelem,
                                        bool implementation) {
    int i;
	bool hasArg = false, hasBaseArg = false;
	CapelModule *cm = new CapelModule(mPath + filename + ".h");
    Xtring s;
    s = "public:\n\t" + classname + "(";
    // parametros contructor clase derivada
    for( i=0; i<nelem; i++) {
        if( array[i].isConstructorArg() ) {
			hasArg = true;
            if( i>0 )
                s+=", ";
            if( (i%3 == 2) && (i<nelem-1))
                s+="\n\t\t";
            s+= array[i].getReturnType() + " " + array[i].sName.lower();
            if( array[i].sDefValue != "" )
                s += " = " + array[i].sDefValue;
        }
    }
    // parametros contructor clase base
    for( i=0; i<nelembase; i++) {
        if( arraybase[i].isConstructorArg() ) {
			hasBaseArg = true;
			if( hasArg or i>0)
	            s+=", ";
            if( (i%3 == 2) && (i<nelembase-1))
                s+="\n\t\t";
            s+= arraybase[i].getReturnType() + " " + arraybase[i].sName.lower();
            if( arraybase[i].sDefValue != "" )
                s += " = " + arraybase[i].sDefValue;
        }
    }
    s+=")\n";
    if( implementation ) {
        s += "\t:" + basename + "(";
        // constructor base
        for( i=0; i<nelembase; i++) {
            if( arraybase[i].isConstructorArg() ) {
				if( i>0)
					s+=", ";
				if( (i%3 == 2) && (i<nelembase-1))
                    s+="\n\t\t";
                s+= arraybase[i].sName.lower();
            }
        }
        s += ")\n\t";
		// derived class members initializacion
        for( i=0; i<nelem; i++) {
            if( array[i].isNullable() || array[i].isConstructorArg() ) {
				if( hasBaseArg )
                	s+=", ";
                if( (i%3 == 2) && (i<nelem-1))
                    s+="\n\t\t";
				if( array[i].isNullable() ) {
                    s += "mNull" + array[i].sName + "(true)";
					if( array[i].isConstructorArg() )
						s += ", ";
				}
                if( array[i].isConstructorArg() )
                    s+= "m" + array[i].sName + "(" + array[i].sName.lower() + ")";
            }
        }
        cm->insert_extrusion(classname.upper() + "_CONSTRUCTOR", s, "\n\t{};\n");
    } else {
        s += ";\n";
        cm->insert_extrusion(classname.upper() + "_CONSTRUCTOR", s);
    }

    cm->writeIfModified();

}



/** Crea una serie de get/set de clases */
void RTKCppClass::addGetSet(const Xtring &filename, const Xtring &classname,
                            GetSetArray *array, int nelem, bool )
{
	CapelModule *cm = new CapelModule(mPath + filename + ".h");
    Xtring s = "public:\n";
    // Funciones get/set publicas
    for( int i=0; i<nelem; i++) {
        // Get
        if( array[i].hasGet() ) {
            if( array[i].sDoc.size() )
                s+="\t/** Gets " + array[i].sDoc + " */\n";
			s+="\t" + array[i].getReturnType() + " " + array[i].sName.unProper() + "() const";
            if( array[i].hasGetImpl() )  {
                s += " { return m" + array[i].sName + "; }\n";
            } else {
                s += ";\n";
            }
        }
        // Set
        if( array[i].hasSet() ) {
            if( array[i].sDoc.size() )
                s+="\t/** Sets " + array[i].sDoc + " */\n";
            s+= "\tvoid set" + array[i].sName + "(" + array[i].getArgumentType()
                +  " " + array[i].sName.lower() + ")";
            if( array[i].hasSetImpl() ) {
                s += " { ";
                if( array[i].isNullable() )
                    s += "\n\t\tmNull" + array[i].sName + " = false; ";
                s+= "m" + array[i].sName + " = " + array[i].sName.lower() + "; }\n";
            } else {
                s += ";\n";
            }
            if( array[i].isNullable() )
                s += "\tbool isNull" + array[i].sName + "() const { return mNull" + array[i].sName + "; }\n";
        }
    }
    // Protected
    s += "protected:\n";
    for( int i=0; i<nelem; i++) {
        if( array[i].isNullable() )
            s += "\tbool mNull" + array[i].sName + ";";
        s+="\t" + array[i].sType + " m" + array[i].sName + ";\n";
    }
    cm->insert_extrusion(classname.upper() + "_GETSET", s);
    cm->writeIfModified();
    delete cm;
}



/** Gens debug info */
void RTKCppClass::addDebugPrint(const Xtring &filename, const Xtring &classname,
                                GetSetArray *array, int nelem)
{
	CapelModule *cm = new CapelModule(mPath + filename);
    Xtring extrusion = classname.upper() + "_DEBUGPRINT";
    // printf
    Xtring sparam, svalues = "\n\t\")\",\n";
    for( int i=0; i<nelem; i++) {
        if( i>0 ) {
            sparam += "\",\"\n";
            svalues += ",\n";
        }
		sparam += "\t\"" + array[i].sName.proper() + "='" + printfType(array[i].sType)
                  + "'\"";
        if( equivType(array[i].sType) == "string" )
            svalues += "\t(const char *)";
        else
            svalues += "\t";
        svalues += "m" + array[i].sName;
    }
    cm->insert_extrusion(extrusion, sparam + svalues + "\t);\n");
    cm->write();

}



/** Crea una serie de get/set de clases */
void RTKCppClass::addTests(const Xtring &filename,
                           const Xtring &classname, GetSetArray *array, int nelem)
{
	CapelModule *cm = new CapelModule(mPath + filename + ".h");
    Xtring s =
        "/**"
        "\nTest the " + classname + " object"
        "\n"
        "\t@author Francisco Santiago Capel Torres\n"
        "\t*/\n"
        "class Test" + classname + ": public Test\n"
        "{\n"
        "public:\n"
        "\tTest" + classname + "();\n"
        "\t~Test" + classname + "();\n"
        "\tint run();\n";

    for( int i=0; i<nelem; i++) {
        if( array[i].flags & PROPERTY )
            s+= "\tvoid test" + array[i].sName + "();\n";
    }

    s += "private:\n"
         "\t" + classname + " *p" + classname + ";\n";
    cm->insert_extrusion(classname.upper() + "_TESTS", s, "};\n");
    cm->writeIfModified();


	CapelModule *cmcpp = new CapelModule(mPath + filename + ".cpp");
    s =
        "Test" + classname + "::Test" + classname + "()\n"
        "\t:Test()\n"
        "{\n";
    cmcpp->insert_extrusion(classname.upper() + "_TEST_CONSTRUCTOR", s, "}\n");
    s =
        "Test" + classname + "::~Test" + classname + "()\n"
        "{\n";
    cmcpp->insert_extrusion(classname.upper() + "_TEST_DESTRUCTOR", s, "}\n");

    Xtring sstart = "int Test"  + classname + "::run()\n"
                     "{\n"
                     "\tqDebug(\"Test" + classname + " started\");\n";
    s = "";
    for( int i=0; i<nelem; i++) {
        if( array[i].flags & PROPERTY )
            s+= "\ttest" + array[i].sName + "();\n";
    }
    Xtring send =
        "\tqDebug(\"Test" + classname + " finished\");\n"
        "\treturn 0;\n"
        "}\n";
    cmcpp->insert_extrusion(classname.upper() + "_RUN", s, send, sstart);

    for( int i=0; i<nelem; i++) {
        s = "";
        if( array[i].flags & PROPERTY ) {
            s= addOneTest(classname, &array[i]);
            cmcpp->insert_extrusion(classname.upper() + "_TEST_" + array[i].sName.upper(), s,
			"}\n\n",
            "void Test" + classname + "::test" + array[i].sName + "()\n""{\n" );
        }
    }
    cmcpp->writeIfModified();
}


void RTKCppClass::addProperties(const Xtring &filename, const Xtring &classname,
                        GetSetArray *array, int nelem)
{
	CapelModule *cm = new CapelModule(mPath + filename + ".h");
	Xtring s = "public:\n";
	for( int i=0; i<nelem; i++) {
        // Property
		if( array[i].isProperty() ) {
			s+="\tProperty<" + array[i].getReturnType() + "> prop" + array[i].sName.proper() + ";\n";
		}
	}
	cm->insert_extrusion(classname.upper() + "_PROPERTIES", s);
	cm->writeIfModified();
	delete cm;
}

Xtring GetSetArray::getVariantTo(const Xtring &varname) const
{
	if( sType == "int" || sType == "uint" )
		return varname + ".toInt()";
	else if( sType == "UnitsType" || sType == "AggregateType"
		|| sType == "Adjustment" || sType == "BorderStyle" || sType == "Alignment"
		|| sType == "Color" || sType == "PageOrientation" || sType == "ImageStyle" )
		return "static_cast<" + sType + ">( " + varname + ".toInt() )";
	else if( sType == "const char *" )
		return varname + ".toString().c_str()";
	else if( sType == "bool" )
		return varname + ".toBool()";
	else if( sType == "Date" )
		return varname + ".toDate()";
	else if( sType == "Time" )
		return varname + ".toTime()";
	else if( sType == "DateTime" )
		return varname + ".toDateTime()";
	else if( sType == "double" || sType == "Measure" )
		return varname + ".toDouble()";
	else
		return varname + ".Indefinido";
}

}

