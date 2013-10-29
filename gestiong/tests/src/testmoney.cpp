#include <gongdebug.h>
#include <gongmoney.h>
#include <gongvariant.h>
#include "testmoney.h"

using namespace gong;

TestMoney::TestMoney()

{
}


TestMoney::~TestMoney()
{
}


int TestMoney::testToString()
{
	Money m(1234.3456, 4 );
	_GONG_DEBUG_ASSERT_EQUALS( m.toString(), "1234.3456" );
	Money m1(1234.34, 4 );
	_GONG_DEBUG_ASSERT_EQUALS( m1.toString(), "1234.3400" );
	return 1;
}

int TestMoney::run()
{
	testToString();
	Money m0;
	_GONG_DEBUG_ASSERT( m0.toDouble() == 0.0 );
	Money m102(102, 2);
	_GONG_DEBUG_ASSERT( m102.toDouble() == 102 );
	Money m1_23(1.23, 2);
	_GONG_DEBUG_ASSERT( (float) m1_23.toDouble() == (float) 1.23 );
	Money m13_43(13.43);
	_GONG_DEBUG_ASSERT( (float) m13_43.toDouble() == (float) 13.43 );
	Money m123_34 = 123.34;
	_GONG_DEBUG_ASSERT( (float) m123_34.toDouble() == (float) 123.34 );
	Money m1000 = Money(1000, 0);
	_GONG_DEBUG_ASSERT( m1000.toInt() == 1000 );
	_GONG_DEBUG_ASSERT( m1000.toDouble() == 1000.0 );

	Money m_sum = m0 + m102;
	_GONG_DEBUG_ASSERT( m_sum == m0 + m102 );
	_GONG_DEBUG_ASSERT( m_sum == m102 );
	_GONG_DEBUG_ASSERT( m_sum == 102 );
	m_sum = m1_23;
	m_sum += 1;
	_GONG_DEBUG_ASSERT( m_sum == m1_23 + 1 );
	_GONG_DEBUG_ASSERT( m_sum.toString() == "2.23");
	_GONG_DEBUG_ASSERT( m_sum == 2.23 );
	m_sum += 1.34;
	_GONG_DEBUG_ASSERT( m_sum == m1_23 + 1 + 1.34);
	_GONG_DEBUG_ASSERT( m_sum == 1.23 + 1 + 1.34 );
	m_sum -= Money(.36);
	_GONG_DEBUG_ASSERT( m_sum == Money(1.23 + 1 + 1.34 - .36) );
	Money m_sub = m_sum - 36;
	_GONG_DEBUG_ASSERT( m_sub == (m_sum - 36) );
	_GONG_DEBUG_ASSERT( m_sub == Money(1.23 + 1 + 1.34 - .36 - 36 ));

	m_sum = 1000;
	_GONG_DEBUG_ASSERT( m_sum.toDouble() == 1000.0 );

	_GONG_DEBUG_ASSERT( Money(0) < Money(1) );
	_GONG_DEBUG_ASSERT( Money(1.3) < Money(1.4) );
	_GONG_DEBUG_ASSERT( Money(1.3333333) == Money(1.3333334) );
	_GONG_DEBUG_ASSERT( Money(1.3333333, 7) < Money(1.3333334, 7) );
	_GONG_DEBUG_ASSERT( Money(324324324,2) < Money(324324325,2) );
	_GONG_DEBUG_ASSERT( Money(1.34,2) == Money(1.34,3) );
	_GONG_DEBUG_ASSERT( Money(1.344,2) != Money(1.344,3) );
	_GONG_DEBUG_ASSERT( Money(1.344,2) < Money(1.344,3) );
	_GONG_DEBUG_ASSERT( Money(1.344,2) <= Money(1.344,3) );

	_GONG_DEBUG_ASSERT( Money(0) > Money(-1) );
	_GONG_DEBUG_ASSERT( Money(-1.3) > Money(-1.4) );
	_GONG_DEBUG_ASSERT( Money(-1.3333333) == Money(-1.3333334) );
	_GONG_DEBUG_ASSERT( Money(-1.3333333, 7) > Money(-1.3333334, 7) );
	_GONG_DEBUG_ASSERT( Money(-324324324,2) < Money(324324325,2) );
	_GONG_DEBUG_ASSERT( Money(-1.34,2) == Money(-1.34,3) );
	_GONG_DEBUG_ASSERT( Money(-1.344,2) != Money(-1.344,3) );
	_GONG_DEBUG_ASSERT( Money(-1.344,2) > Money(-1.344,3) );
	_GONG_DEBUG_ASSERT( Money(-1.344,2) >= Money(-1.344,3) );

	long long p = Money(0,2).packed();
	_GONG_DEBUG_ASSERT( Money::unpack(p) == Money(0,2) );
	p = Money(-34.34,4).packed();
	_GONG_DEBUG_ASSERT( Money::unpack(p) == Money(-34.34,4) );
	p = Money(34.34,4).packed();
	_GONG_DEBUG_ASSERT( Money::unpack(p) == Money(34.34,4) );

	_GONG_DEBUG_ASSERT( Money(125.25) * 1.5 == Money(125.25 * 1.5 ) );

	return 1;
}


