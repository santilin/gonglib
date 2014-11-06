#include <gongdatatable.h>
#include <gongdbrecorddatamodel.h>
#include "reccolordatatable.h"

namespace gong {
namespace reccolor {

DataTable::DataTable(dbDefinition* database, QWidget* parent, const char* name)
    : gong::DataTable( database, parent, name ), mColorColumn( -1 ), pColorField( 0 )
{
}

void DataTable::paintCell(QPainter* p, int row, int col, const QRect& cr, bool selected, const QColorGroup& cg)
{
    // The first 16 colors are standard ones
    static int scolors[16] = {
        0x000080, // Navy blue
        0x0000ff, // Blue
        0x008000, // green
        0xffff00, // yellow
        0xff0000, // red
        0x808080, // gray
        0xffa500, // Orange
        0x800000, // Maroon
        0x800080, // purple
        0x808000, // olive
        0x9966cc, // Amethyst
        0x00ffff, // Aqua
        0xC0C0C0, // silver
        0xff00ff, // fuchsia
        0x008080, // teal
        0x00ff00 // lime
    };
    if( mColorColumn != -1 ) {
        int ncolor = getDataModel()->getValue( row, mColorColumn ).toInt();
        if( ncolor != 0 ) {
            if( ncolor <= 16 )
                ncolor = scolors[ncolor-1];
            QColorGroup new_cg( cg );
            if( selected ) {
                new_cg.setColor( QPalette::WindowText, QColor( ncolor ) );
            } else {
                new_cg.setColor( QPalette::Base, QColor( ncolor ) );
            }
            gong::DataTable::paintCell(p, row, col, cr, selected, new_cg);
            return;
        }
    }
    gong::DataTable::paintCell(p, row, col, cr, selected, cg);
}

bool DataTable::setView(int newview)
{
    mColorColumn = -1;
    if( newview >= (int)getDataModel()->getViewCount() )
        newview = 0;
    dbViewDefinition *orig_view_def = getDataModel()->getView( newview );
    dbRecordDataModel *rdm = static_cast<dbRecordDataModel *>( getDataModel() );
    Xtring fldname = rdm->getRecord()->getTableName() + ".REC_COLOR";
    pColorField = orig_view_def->findFieldDefinition( fldname );
    if( !pColorField )
        pColorField = orig_view_def->findFieldDefinition( "REC_COLOR" );
    if( !pColorField ) {
        dbFieldDefinition *new_fld_def = new dbFieldDefinition(
            rdm->getRecord()->getTableName(), "REC_COLOR", SQLINTEGER, 12, 0,
            dbFieldDefinition::INVISIBLE);
        mColorColumn = orig_view_def->getFieldCount();
        orig_view_def->addField( new_fld_def );
        bool ret = gong::DataTable::setView( newview );
        /// \todo ????
//		orig_view_def->deleteField( fldname.c_str() );
//		delete new_fld_def;
        return ret;
    } else {
        mColorColumn = orig_view_def->getFieldPosition( "REC_COLOR" );
        return gong::DataTable::setView( newview );
    }
}

} // namespace reccolor
} // namespace gong
