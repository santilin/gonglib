#ifndef TESTDBCALCULATOR_H
#define TESTDBCALCULATOR_H

#include <gongdbrecord.h>
using namespace gong;

class TestdbCalculator
{
public:
  TestdbCalculator();
  void testRecord( dbRecord *r, char *expr );
  dbRecord *createRecord();
  void run( char * expr );
};

#endif
