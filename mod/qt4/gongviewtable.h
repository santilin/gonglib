#ifndef _GONG_VIEWTABLE_H
#define _GONG_VIEWTABLE_H

/** @file gongviewtable.h Qt based table view widget
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QTableView>
class QPainter;

#include <gongformatter.h>
#include <gongdbrecord.h>
#include <gongdbviewdefinition.h>

namespace gong {

class dbDefinition;
class dbFieldDefinition;
class FormatterItemDelegate;
class ViewTableModel;

class ViewTable : public QTableView
{
    Q_OBJECT
public:
    enum TableEditMode {
        None = 0,
        Inline = 1,
        Event = 2
    };

    enum EditMode {
        choosing, browsing,
        selecting, inserting, updating, deleting, duplicating,
        finding, filtering,
        defaulteditmode,
        cancelling,
        accepting
    };

    ViewTable( dbRecord *record, const dbViewDefinitionsList &vlist,
			const Xtring &filter = Xtring(), QWidget* parent=0, const char* name=0 );
    ~ViewTable();

	bool setView( int nview );
    void refresh();

	int currentRow() const;
	void setCurrentRow(int row);
	int rowCount() const;
	int columnCount() const;
    bool isFirstRow() const { return currentRow() == 0; }
    bool isLastRow() const { return currentRow() == rowCount()-1; }

    void setVerticalHeader( bool visible );

    void sync(dbRecordID id);
    virtual QString text ( int row, int col ) const; // from QTable
    Variant value( int row, int col ) const;


    bool firstRow();
    bool lastRow();
    bool nextRow();
    bool prevRow();
    void setTableEditMode(TableEditMode em);
    void scrollToCurrent();
    TableEditMode getTableEditMode() const;
    List<dbRecordID> getSelectedIDs() const;
    void setSelectedIDs( const List<dbRecordID> &selrecords );
    const dbFieldDefinition *getFldInfo(int col);

signals:
    void beginEditSignal(ViewTable *sender, ViewTable::EditMode editMode, const Xtring &fldvalue);
    void focusChanged(ViewTable *sender, QFocusEvent *ev);


protected:
#if 0
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget
    virtual void contentsContextMenuEvent( QContextMenuEvent* e ); // from QScrollView (QTable)
    virtual void contentsMouseDoubleClickEvent( QMouseEvent *e ); // from QScrollView (QTable)
    virtual void removeColumn( unsigned int col ); // from QTable
    virtual void swapColumns( int col1, int col2, bool swapHeaders = false ); // from QTable
    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr,
                             bool selected, const QColorGroup &cg ); // from QTable
    virtual void focusInEvent( QFocusEvent *e); // from QWidget
    virtual void focusOutEvent( QFocusEvent *e); // from QWidget
#endif

private:
    void findInColumn( int column, const Xtring & str, bool caseSensitive, bool backwards );
    void init();

    dbDefinition *pDatabase;
    ViewTableModel *pViewTableModel;
    Formatter mFormatter;
    Xtring searchString;
	int mSortedColumn;
    ViewTable::TableEditMode tbEditMode;
};

}; // namespace
#endif  // _GONG_VIEWTABLE_H
