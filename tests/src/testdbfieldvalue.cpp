
#include <gongdbfieldvalue.h>
#include <gongdebug.h>
#include <iostream>
#include "testdbfieldvalue.h"

using namespace gong;


TestdbFieldValue::TestdbFieldValue()

{
}


TestdbFieldValue::~TestdbFieldValue()
{
}



void TestdbFieldValue::run()
{
	test_normal();
	test_create();
	std::cout << "TestdbFieldValue finished" << std::endl;
}

int TestdbFieldValue::test_normal()
{
	int ret = 0;

	dbFieldValue fint(0);
	_GONG_DEBUG_ASSERT ( fint.isNull() == true );
	_GONG_DEBUG_ASSERT ( fint.isModified() == false );
	_GONG_DEBUG_ASSERT ( fint == 0 );
	fint.setValue(10);
	_GONG_DEBUG_ASSERT ( fint.isNull() == false );
	_GONG_DEBUG_ASSERT ( fint.isModified() == true );
	_GONG_DEBUG_ASSERT ( fint == 10 );
	_GONG_DEBUG_ASSERT ( fint.toString() == "10" );
	fint.setNull();
	_GONG_DEBUG_ASSERT ( fint.isNull() == true );
	_GONG_DEBUG_ASSERT ( fint.isModified() == true );
	_GONG_DEBUG_ASSERT ( fint.toInt() == 0 );
	fint.clear();
	_GONG_DEBUG_ASSERT ( fint.isNull() == true );
	_GONG_DEBUG_ASSERT ( fint.isModified() == false );
	_GONG_DEBUG_ASSERT ( fint.toInt() == 0 );

	dbFieldValue flong(0LL);
	_GONG_DEBUG_ASSERT ( flong.isNull() == true );
	_GONG_DEBUG_ASSERT ( flong.isModified() == false );
	_GONG_DEBUG_ASSERT ( flong == 0 );
	flong.setValue(10LL);
	_GONG_DEBUG_ASSERT ( flong.isNull() == false );
	_GONG_DEBUG_ASSERT ( flong.isModified() == true );
	_GONG_DEBUG_ASSERT ( flong == 10LL );
	_GONG_DEBUG_ASSERT ( flong.toString() == "10" );
	flong.setNull();
	_GONG_DEBUG_ASSERT ( flong.isNull() == true );
	_GONG_DEBUG_ASSERT ( flong.isModified() == true );
	_GONG_DEBUG_ASSERT ( flong.toLong() == 0 );
	flong.clear();
	_GONG_DEBUG_ASSERT ( flong.isNull() == true );
	_GONG_DEBUG_ASSERT ( flong.isModified() == false );
	_GONG_DEBUG_ASSERT ( flong.toLong() == 0 );

	dbFieldValue fdouble(0.0);
	_GONG_DEBUG_ASSERT ( fdouble.isNull() == true );
	_GONG_DEBUG_ASSERT ( fdouble.isModified() == false );
	_GONG_DEBUG_ASSERT ( fdouble == 0 );
	fdouble.setValue(10.2);
	_GONG_DEBUG_ASSERT ( fdouble.isNull() == false );
	_GONG_DEBUG_ASSERT ( fdouble.isModified() == true );
	_GONG_DEBUG_ASSERT ( fdouble == 10.2 );
	_GONG_DEBUG_ASSERT ( fdouble.toString() == "10.2" );
	fdouble.setNull();
	_GONG_DEBUG_ASSERT ( fdouble.isNull() == true );
	_GONG_DEBUG_ASSERT ( fdouble.isModified() == true );
	_GONG_DEBUG_ASSERT ( fdouble.toDouble() == 0.0 );
	fdouble.clear();
	_GONG_DEBUG_ASSERT ( fdouble.isNull() == true );
	_GONG_DEBUG_ASSERT ( fdouble.isModified() == false );
	_GONG_DEBUG_ASSERT ( fdouble.toDouble()  == 0.0 );

	dbFieldValue fcharptr("");
	_GONG_DEBUG_ASSERT ( fcharptr.isNull() == true );
	_GONG_DEBUG_ASSERT ( fcharptr.isModified() == false );
	_GONG_DEBUG_ASSERT ( fcharptr == "" );
	fcharptr.setValue("10.2");
	_GONG_DEBUG_ASSERT ( fcharptr.isNull() == false );
	_GONG_DEBUG_ASSERT ( fcharptr.isModified() == true );
	_GONG_DEBUG_ASSERT ( fcharptr == "10.2" );
	_GONG_DEBUG_ASSERT ( fcharptr.toString() == "10.2" );
	fcharptr.setNull();
	_GONG_DEBUG_ASSERT ( fcharptr.isNull() == true );
	_GONG_DEBUG_ASSERT ( fcharptr.isModified() == true );
	_GONG_DEBUG_ASSERT ( fcharptr.toString() == "" );
	fcharptr.clear();
	_GONG_DEBUG_ASSERT ( fcharptr.isNull() == true );
	_GONG_DEBUG_ASSERT ( fcharptr.isModified() == false );
	_GONG_DEBUG_ASSERT ( fcharptr.toString() == "" );

	dbFieldValue fbinary(Variant( (void *)"hola", 4) );
	_GONG_DEBUG_ASSERT ( fbinary.type() == Variant::tBinary );
	_GONG_DEBUG_ASSERT ( fbinary.isNull() == true );
	_GONG_DEBUG_ASSERT ( fbinary.isModified() == false );
	_GONG_DEBUG_ASSERT ( fbinary.toString() == "hola" );
	fbinary.setValue("10.2");
	_GONG_DEBUG_ASSERT ( fbinary.isNull() == false );
	_GONG_DEBUG_ASSERT ( fbinary.isModified() == true );
	_GONG_DEBUG_ASSERT ( fbinary == "10.2" );
	_GONG_DEBUG_ASSERT ( fbinary.toString() == "10.2" );
	fbinary.setNull();
	_GONG_DEBUG_ASSERT ( fbinary.isNull() == true );
	_GONG_DEBUG_ASSERT ( fbinary.isModified() == true );
	_GONG_DEBUG_ASSERT ( fbinary.toString() == "" );
	fbinary.clear();
	_GONG_DEBUG_ASSERT ( fbinary.isNull() == true );
	_GONG_DEBUG_ASSERT ( fbinary.isModified() == false );
	_GONG_DEBUG_ASSERT ( fbinary.toString() == "" );

	dbFieldValue fmoney(Money(0.0, 2 ) );
	_GONG_DEBUG_ASSERT ( fmoney.isNull() == true );
	_GONG_DEBUG_ASSERT ( fmoney.isModified() == false );
	_GONG_DEBUG_ASSERT ( fmoney == 0 );
	fmoney.setValue(10.2);
	_GONG_DEBUG_ASSERT ( fmoney.isNull() == false );
	_GONG_DEBUG_ASSERT ( fmoney.isModified() == true );
	_GONG_DEBUG_ASSERT ( fmoney == 10.2 );
	_GONG_DEBUG_ASSERT ( fmoney.toString() == "10.20" );
	fmoney.setNull();
	_GONG_DEBUG_ASSERT ( fmoney.isNull() == true );
	_GONG_DEBUG_ASSERT ( fmoney.isModified() == true );
	_GONG_DEBUG_ASSERT ( fmoney.toDouble() == 0.0 );
	fmoney.clear();
	_GONG_DEBUG_ASSERT ( fmoney.isNull() == true );
	_GONG_DEBUG_ASSERT ( fmoney.isModified() == false );
	_GONG_DEBUG_ASSERT ( fmoney.toDouble()  == 0.0 );
	fmoney.setValue ( Money ( Xtring("0.0").toDouble(), 4 ) );

	Money m1( 0.0, 4);
	Money m2( 0.0, 4);
	_GONG_DEBUG_ASSERT( m1 == m2 );
	Variant v1( Money(0.0, 4) );
	Variant v2( Money(0.0, 4) );
	_GONG_DEBUG_ASSERT( v1 == v2 );
	Money m3( 0.0, 4 );
	Money m4 = m3;
	_GONG_DEBUG_ASSERT( m3 == m4 );
	Variant v3( Money(0.0, 4) );
	Variant v4;
	v4.copy( v3 );
	_GONG_DEBUG_ASSERT( v3 == v4 );

	return ret;
}


int TestdbFieldValue::test_create()
{
	int ret = 0;

	return ret;

}

