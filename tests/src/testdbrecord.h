#ifndef TESTDBRECORD_H
#define TESTDBRECORD_H

#include <gongdbrecord.h>

class TestdbRecord
{
public:
    TestdbRecord();
    ~TestdbRecord();

	void run();
	int test_normal();
	int test_imagefield();
	int test_create();
	int test_tostringfugit();
};

#endif // TESTDBRECORD_H
