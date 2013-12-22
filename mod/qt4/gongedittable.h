#ifndef _GONG_EDITTABLE_H
#define _GONG_EDITTABLE_H

/** @file gongedittable.h Qt based table view widget
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
namespace gong {

class dbDefinition;
class TableDataModel;
class dbFieldDefinition;
class EditTablePrivate;

class EditTable : public QTableView
{
    Q_OBJECT
public:
    enum Refresh {
        RefreshNone = 0,
        RefreshData = 1,
        RefreshColumns = 2,
        RefreshNumRows = 4,
        RefreshAll = RefreshData | RefreshColumns | RefreshNumRows
    };

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

    EditTable( dbDefinition *database, QWidget* parent=0, const char* name=0 );
    ~EditTable();

	int currentRow() const;
	void setCurrentRow(int row);
	int numRows() const;
    bool isFirstRow() const { return currentRow() == 0; }
    bool isLastRow() const { return currentRow() == numRows()-1; }

    void setDataModel( TableDataModel *dm );
    TableDataModel* getDataModel() const;
    virtual bool setView( int view );
    virtual QString text ( int row, int col ) const; // from QTable
    void sync(dbRecordID id);
    void refresh();
    virtual void refresh( Refresh mode );
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
    void setVerticalHeader( bool visible );
    int  indexOf( unsigned int i ) const;
    const dbFieldDefinition *getFldInfo(int col);

signals:
    void beginEditSignal(EditTable *sender, EditTable::EditMode editMode, const Xtring &fldvalue);
    void focusChanged(EditTable *sender, QFocusEvent *ev);


#if 0
protected:
    virtual void keyPressEvent( QKeyEvent *e ); // from QWidget
    virtual void contentsContextMenuEvent( QContextMenuEvent* e ); // from QScrollView (QTable)
    virtual void contentsMouseDoubleClickEvent( QMouseEvent *e ); // from QScrollView (QTable)
    virtual void addColumn(const dbFieldDefinition *fldinfo, const QIcon& iconset); // from QTable
    virtual void removeColumn( unsigned int col ); // from QTable
    virtual void swapColumns( int col1, int col2, bool swapHeaders = false ); // from QTable
    virtual void paintCell ( QPainter * p, int row, int col, const QRect & cr,
                             bool selected, const QColorGroup &cg ); // from QTable
    virtual void focusInEvent( QFocusEvent *e); // from QWidget
    virtual void focusOutEvent( QFocusEvent *e); // from QWidget
#endif

private:
    void findInColumn( int column, const Xtring & str, bool caseSensitive, bool backwards );
    void reset();
    void init();

    dbDefinition *pDatabase;
    EditTablePrivate* d;
    Formatter mFormatter;
};


}; // namespace
#endif  // _GONG_EDITTABLE_H
