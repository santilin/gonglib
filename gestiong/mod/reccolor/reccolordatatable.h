#ifndef RECCOLOR_DATATABLE_H
#define RECCOLOR_DATATABLE_H

#include <gongdatatable.h>

namespace gong {
namespace reccolor {

class DataTable: public gong::DataTable
{
public:
    DataTable( dbDefinition *database, QWidget* parent=0, const char* name=0 );
    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr,
                             bool selected, const QColorGroup &cg ); // from gong::DataTable
    virtual bool setView( int view );
private:
    int mColorColumn;
    dbFieldDefinition *pColorField;
};

} // namespace reccolor
} // namespace gong

#endif // RECCOLOR_DATATABLE_H
