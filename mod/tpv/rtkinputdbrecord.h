#ifndef _RTK_INPUTDBRECORD_H
#define _RTK_INPUTDBRECORD_H

#include <rtkinput.h>

namespace gong {

class dbRecord;
class dbRecordList;

class InputdbRecord: public Input {
public:
    InputdbRecord( dbRecord *record, dbRecordList *reclist, class Report &report, const char *name )
		: Input( report, name, "mysql" ), pRecord( record ), pRecordList( reclist ) {};
    /* Override to init the query and set the source field of every input field */
    virtual bool init();
    /* Override to read the next record from the input */
    virtual bool next();
    /* Override to get the value of the nth source field from the input */
    virtual Variant getValue(uint sourcepos, Variant::Type type) const;

protected:
	dbRecord *pRecord, *pDetailRecord;
	dbRecordList *pRecordList;
	uint mRecordNumber;
};

} // namespace gong

#endif // _RTK_INPUTDBRECORD_H
