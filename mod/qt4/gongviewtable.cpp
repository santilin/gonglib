#include <QHeaderView>
#include <QStyledItemDelegate>
#include <gongdbrecorddatamodel.h>
#include <gongtabledatamodel.h>
#include <gongguiapplication.h>
#include "gongviewtable.h"

namespace gong {

class ViewTableModel: public QAbstractTableModel, public dbRecordDataModel
{
public:
	ViewTableModel( QObject *parent, dbRecord *record, const dbViewDefinitionDict &vlist,
					const Xtring &filter = Xtring() )
		: QAbstractTableModel( parent ), dbRecordDataModel( record, vlist, filter ) {};
    void addColumn(const dbFieldDefinition *fldinfo, const QIcon& iconset);
	bool setView( int view );
	void refresh() { reset(); }
protected:
	int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
private:
    std::vector<const dbFieldDefinition *> mFields;
    std::vector<QIcon> mIcons;
};

int ViewTableModel::columnCount(const QModelIndex& parent) const
{
	_GONG_DEBUG_PRINT(0, mFields.size());
	return mFields.size();
}

int ViewTableModel::rowCount(const QModelIndex& parent) const
{
	_GONG_DEBUG_PRINT(0, getRowCount() );
	return getRowCount();
}

QVariant ViewTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	else if( role == Qt::TextAlignmentRole ) {
		switch( index.column() ) {
		default:
			return Qt::AlignRight;
		}
	} else if( role == Qt::DisplayRole ) {
		int column = index.column();
		int row = index.row();
		return QVariant( toGUI(getValue(row, column).toString()) );
	}
	return QVariant();
}

QVariant ViewTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if( orientation == Qt::Horizontal ) {
		if( role == Qt::DisplayRole ) {
			return QVariant(toGUI( mFields[section]->getCaption()));
		}
	}
	return QVariant();
}


void ViewTableModel::addColumn(const dbFieldDefinition *fldinfo, const QIcon& iconset)
{
    _GONG_DEBUG_PRINT(0, Xtring::printf("Añadiendo campo %p,%s a la tabla con caption=%s y width=%d",
                                         fldinfo, fldinfo->getFullName().c_str(), fldinfo->getCaption().c_str(), fldinfo->getDisplayWidth() ) );
    mFields.push_back( fldinfo );
    mIcons.push_back( iconset );
}


bool ViewTableModel::setView( int newview )
{
	mFields.clear();
	mIcons.clear();
    if( newview >= (int)getViewCount() )
        newview = 0;
    setViewIndex(newview);
    const dbViewDefinition *curView = getCurrentView();
    if( curView )
    {
        for ( unsigned int i = 0; i < curView->getFieldCount(); ++i )
        {
            _GONG_DEBUG_PRINT(0, Xtring::printf("Añadiendo columna: %s",
					curView->getFieldDefinition(i)->getName().c_str() ) );
            addColumn( curView->getFieldDefinition(i), QIcon() );
        }
        reset();
        return true;
    } else
        return false;
}


/*!
    Constructs a data table which is a child of \a parent, called name
    \a name using the tdm \a tdm.

    Columns are automatically created based upon the fields in the \a tdm
    record.

    If the \a tdm is read-only, the table also becomes read-only.
    In addition, the table adopts the tdm's driver's definition for
    representing NULL values as strings.
*/

ViewTable::ViewTable( dbRecord *record, const dbViewDefinitionDict &vlist,
					const Xtring &filter, QWidget * parent, const char * name )
    : QTableView( parent ), pDatabase( &record->getTableDefinition()->getdbDefinition() ),
      mFormatter( *GongLibraryInstance->getRegConfig() )
{
	setObjectName( name );
	pViewTableModel = new ViewTableModel( this, record, vlist, filter );
	setModel( pViewTableModel );
// 	setSelectionMode( QAbstractItemView::SingleSelection );
// 	setSelectionBehavior( QAbstractItemView::SelectRows );
// 	setAlternatingRowColors( true );
// 	verticalHeader()->setDefaultSectionSize( 16 );
// 	verticalHeader()->hide();
// 	horizontalHeader()->setStretchLastSection(true);
}

ViewTable::~ViewTable()
{
 	delete pViewTableModel;
}

bool ViewTable::setView(int nview)
{
    mSortedColumn = 1; // Para saltarse la columna ID
    searchString.clear();
	return pViewTableModel->setView( nview );
}

void ViewTable::refresh()
{
	pViewTableModel->refresh();
}


/*!
    Returns the text in cell \a row, \a col, or an empty string if the
    cell is empty. If the cell's value is NULL then nullText() will be
    returned. If the cell does not exist then QString::null is
    returned.
*/

QString ViewTable::text ( int row, int col ) const
{
    QString s;
    if ( (unsigned int)row < pViewTableModel->getRowCount() )
        s = toGUI(pViewTableModel->getValue(row, col ).toString());
    return s;
}


int ViewTable::currentRow() const
{
	int row = -1;
	const QModelIndexList &selecteds = selectionModel()->selection().indexes();
	if( selecteds.count() != 0 ) {
		row = selecteds.at(0).row();
	}
	return row;
}


void ViewTable::setCurrentRow(int row)
{
	if( (int)row < model()->rowCount() && (int) row > 0)
		selectionModel()->setCurrentIndex(model()->index( row, 0),
			QItemSelectionModel::Select | QItemSelectionModel::Rows );
}

int ViewTable::columnCount() const
{
	return horizontalHeader()->count();
}


void ViewTable::setVerticalHeader( bool visible )
{
    verticalHeader()->setVisible( visible );
// 	if( visible )
//         setLeftMargin(20);
//     else
//         setLeftMargin(0);
}




/*!
    \overload void ViewTable::refresh( Refresh mode )

    Refreshes the table. If there is no currently defined query (see
    setTableDataModel()), nothing happens. The \a mode parameter determines
    which type of refresh will take place.

    \sa Refresh setTableDataModel() addColumn()
*/

#if 0
void ViewTable::refresh( ViewTable::Refresh mode )
{
    _GONG_DEBUG_ASSERT(  pViewTableModel  );
    bool refreshData = ( mode & RefreshData );
    bool refreshCol = ( mode & RefreshColumns || ((int)mFields.size() != columnCount() ) );
    bool refreshNumRows = ( mode & RefreshNumRows );
    if( !refreshData && !refreshCol && !refreshNumRows )
        return;
    viewport()->setUpdatesEnabled( false );
    if( refreshData ) {
        setView( pViewTableModel->getCurrentViewIndex() );
    }
    if( refreshNumRows ) {
        setNumRows( pViewTableModel->getRowCount() );
    }
    if ( refreshCol )
    {
        setNumCols( 0 );
        d->colIndex.clear();
        unsigned int fpos = (unsigned int)-1;
        // Loop through all the columns added to mFields with addColumn to match them with the
        // datamodel fields
        for( unsigned int i = 0; i < d->mFields.size(); ++i )
        {
            Xtring colname = pViewTableModel->getColName(i).upper();
            Xtring fullfldname;
            if ( colname.find(".") == Xtring::npos )
                fullfldname = d->mFields[i]->getName().upper();
            else
                fullfldname = d->mFields[i]->getFullName().upper();
            if( colname == fullfldname )
            {   // If the field in the table column definition and in the datamodel are in the same position
                fpos = i;
            }
            else
            {
                // otherwise we take the first field that matches the desired name
                for( fpos = 0; fpos < pViewTableModel->getColCount(); fpos++)
                {
                    if( fullfldname == pViewTableModel->getColName(fpos).upper() )
                        break;
                }
                if( fpos == pViewTableModel->getColCount() )
                    fpos = (unsigned int)-1;
            }
            if ( fpos!=(unsigned int)-1 )
            {
                setNumCols( numCols() + 1 );
                d->colIndex.push_back( fpos );
                setColumnReadOnly( numCols()-1, true );
                Q3Header* h = horizontalHeader();
                QIcon icons = d->mIcons[ i ];
                h->setLabel( numCols()-1, icons, toGUI(d->mFields[i]->getCaption().c_str()) );
                if ( !d->mFields[i]->isVisible() )
                    Q3Table::hideColumn(i);
                else
                {
                    // Here, sqlwidth can be the real sqlwidth defined for the field or the width defined in the attribute w: of the view
                    int w = d->mFields[i]->getDisplayWidth();
                    if( w == 0 && pDatabase ) {
                        dbFieldStyle *styledef = pDatabase->findFieldStyle(d->mFields[i]->getStyle());
                        w = styledef?styledef->getWidth():0;
                    }
                    if( w == 0 )
                        w = d->mFields[i]->getCaption().length();
                    Q3Table::setColumnWidth( i, int(w * fontMetrics().width("m")) );
                }
                _GONG_DEBUG_PRINT(10, Xtring::printf("Añadida columna[%d] con caption='%s' y nombre='%s' en la posición %d",
                                                     i, d->mFields[i]->getCaption().c_str(),
                                                     pViewTableModel->getColName(i).c_str(), fpos));
            }
            else
            {
                _GONG_DEBUG_WARNING( Xtring::printf("Columna '%s' no encontrada", d->mFields[i]->getName().c_str()) );
            }
        }
// 		for( int i = 0; i < verticalHeader()->count(); ++i ) {
// 			verticalHeader()->resizeSection( i, fontMetrics().height() + 2 );
// 		}
    }
    viewport()->setUpdatesEnabled( true );
    viewport()->update();
    horizontalHeader()->update();
    if( numRows() > 0 ) {
        setCurrentCell( 0, 1 );
        selectRow(0);
    }
}

/*!
    Refreshes the table. The query is refreshed using the current
    filter, the current sort, and the currently defined columns.
    Equivalent to calling refresh( ViewTable::RefreshData ).
*/

void ViewTable::refresh()
{
    refresh( RefreshData );
}


/*!
    Removes column \a col from the list of columns to be displayed. If
    \a col does not exist, nothing happens.

    \sa GSqlField
*/

void ViewTable::removeColumn( unsigned int col )
{
    if ( col < d->mFields.size() )
    {
        d->mFields.erase( d->mFields.begin() + col );
        d->mIcons.erase ( d->mIcons.begin() + col );
    }
}

/**
 * @brief If the form is not heigh enough, the last row may still get hiden
 *
 * @return bool
 **/
void ViewTable::scrollToCurrent()
{
    if( currentRow() + 1 >= numRows() ) {
        ensureCellVisible( currentRow(), currentColumn() );
        verticalScrollBar()->triggerAction( QAbstractSlider::SliderToMaximum );
    } else if( currentRow() + 2 >= numRows() )
        ensureCellVisible( currentRow() + 1, currentColumn() );
    else if( currentRow() + 3 >= numRows() )
        ensureCellVisible( currentRow() + 2, currentColumn() );
    else if( currentRow() + 4 >= numRows() )
        ensureCellVisible( currentRow() + 3, currentColumn() );
    else
        ensureCellVisible( currentRow() + 4, currentColumn() );
}

void ViewTable::setTableEditMode(TableEditMode em)
{
    d->tbEditMode = em;
    switch( em ) {
    case None:
        setFocusStyle( Q3Table::FollowStyle );
        setSelectionMode( Q3Table::MultiRow );
        break;
    case Inline:
        setFocusStyle( Q3Table::SpreadSheet );
        setSelectionMode( Q3Table::SingleRow );
        break;
    case Event:
        setFocusStyle( Q3Table::FollowStyle );
        setSelectionMode( Q3Table::Multi );
        break;
    };
}

ViewTable::TableEditMode ViewTable::getTableEditMode() const
{
    return d->tbEditMode;
}

/*!
    Returns the data model used by the data table.
*/

TableDataModel* ViewTable::getDataModel() const
{
    return pViewTableModel;
}


/*! \reimp */
/*
  QViewTable usa eventFilter para procesar keyPressEvent.
  Sin embargo, eso hace que cuando se pulsa Ctrl+X, el eventFilter
  obtenga la pulsación del Ctrl sólamente, y luego la de X, así que
  Ctrl no hace nada y luego se usa la tecla X para buscar.
  Usando keyPressEvent, se obtiene la X y el Control a la vez, asi
  que aquí no se usa y se pasa directamente al padre.
 */
void ViewTable::keyPressEvent( QKeyEvent *ke )
{
    if( !pViewTableModel || pViewTableModel->getRowCount() == 0 )
    {
        ke->ignore();
    }
    else if( (ke->key() == Qt::Key_Enter) || (ke->key() == Qt::Key_Return) )
    {
        d->searchString.clear();
        theGuiApp->hideOSD();
        emit beginEditSignal(this, defaulteditmode, Xtring::null);
        return;
    }
    else if( ke->text()[ 0 ].isPrint() )
    {
        switch( d->tbEditMode )
        {
        case None:
            d->searchString += ke->text().latin1();
            theGuiApp->hideOSD();
            theGuiApp->showOSD( _("Buscando"), d->searchString );
            findInColumn(d->sortedColumn, d->searchString, false, false);
            break;
        case Inline:
        case Event:
            d->searchString.clear();
            theGuiApp->hideOSD();
            emit beginEditSignal(this, defaulteditmode, Xtring(ke->text().latin1()));
            return;
        }
    }
    else if( ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete )
    {
        if( d->searchString.length() > 0 )
        {
            d->searchString = d->searchString.mid(0, d->searchString.length()-1);
            theGuiApp->hideOSD();
            theGuiApp->showOSD( _("Buscando"), d->searchString );
            findInColumn(d->sortedColumn, d->searchString, false, true);
        }
    }
    else if( ke->key() == Qt::Key_Escape )
    {
        ke->ignore();
    }
    else if( ke->state() & (Qt::ControlModifier | Qt::ShiftModifier | Qt::MetaModifier | Qt::AltModifier ) )
    {
        Q3Table::keyPressEvent(ke);
    }
    else
    {
        // QTable no ignora los eventos de teclado que no reconoce, por lo tanto,
        // cuando la tabla tiene el foco, las unicas teclas que funcionan son las
        // aceleradoras, porque se interceptan antes de que lleguen a la tabla.
        // Por ejemplo, las teclas de funcion no funcionan cuando QTable tiene el foco.
        if( ke->key() == Qt::Key_Up || ke->key() == Qt::Key_PageUp ||
                ke->key() == Qt::Key_Home || ke->key() == Qt::Key_Down ||
                ke->key() == Qt::Key_PageDown || ke->key() == Qt::Key_End ||
                ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right ) {
            d->searchString.clear();
//			theGuiApp->hideOSD();
            Q3Table::keyPressEvent(ke);
        }
        else
            ke->ignore();
    }
}

/*! \reimp */
void ViewTable::contentsContextMenuEvent( QContextMenuEvent* e )
{
    /// \todo {refactor} filtering, searching, etc. Es necesario o se puede usar el menu de frmeditrec?
    /// \todo {0.3.4} Añadir resto de funciones sobre el registro
    Q3Table::contentsContextMenuEvent( e );
    if ( !getDataModel() )
        return;
    if ( isReadOnly() )
        return;
    enum {
        IdView,
        IdInsert,
        IdUpdate,
        IdDelete
    };
    QPointer<Q3PopupMenu> popup = new Q3PopupMenu( this, "ViewTableContextMenu" );
    int id[ 4 ];
    id[ IdView ] = popup->insertItem( QString::fromUtf8( _( "&Examinar" ) ) );
    id[ IdInsert ] = popup->insertItem( QString::fromUtf8( _( "&Añadir" ) ) );
    id[ IdUpdate ] = popup->insertItem( QString::fromUtf8( _( "&Modificar" ) ) );
    id[ IdDelete ] = popup->insertItem( QString::fromUtf8( _( "&Borrar" ) ) );
    popup->setItemEnabled( id[ IdView ], true ); /// \todo {0.3.4} Habilitar/deshabilitar menús
    popup->setItemEnabled( id[ IdInsert ], true );
    popup->setItemEnabled( id[ IdUpdate ], true );
    popup->setItemEnabled( id[ IdDelete ], true );
    int r = popup->exec( e->globalPos() );
    delete (Q3PopupMenu*) popup;
    if ( r == id[ IdView ] )
    {
        emit beginEditSignal(this, selecting, Xtring(text(currentRow(), currentColumn()).latin1()) );
    }
    else if ( r == id[ IdInsert ] )
    {
        emit beginEditSignal(this, inserting, Xtring(text(currentRow(), currentColumn()).latin1()) );
    }
    else if ( r == id[ IdUpdate ] )
    {
        emit beginEditSignal(this, updating, Xtring(text(currentRow(), currentColumn()).latin1()) );
    }
    else if ( r == id[ IdDelete ] )
    {
        emit beginEditSignal(this, deleting, Xtring(text(currentRow(), currentColumn()).latin1()) );
    }
    e->accept();
}


/*!
    Returns the index of the field within the current SQL query that
    is displayed in column \a i.
*/

int ViewTable::indexOf( unsigned int i ) const
{
    if( (int)i == -1 ) // what a hack!
        return d->mFields.size();
    else
        return d->colIndex.at(i);
}



/*!
    Returns the value in cell \a row, \a col, or an invalid value if
    the cell does not exist or has no value.
*/

Variant ViewTable::value ( int row, int col ) const
{
    _GONG_DEBUG_ASSERT(  pViewTableModel  );

    Variant v;
    if ( (unsigned int)row < pViewTableModel->getRowCount() )
        v = pViewTableModel->getValue(row, indexOf( col ) );
    return v;
}


/*!
    \reimp

    This function renders the cell at \a row, \a col with the value of
    the corresponding query field on the painter \a p. Depending on
    the table's current edit mode, paintField() is called for the
    appropriate query field. \a cr describes the cell coordinates in
    the content coordinate system. If \a selected is TRUE the cell has
    been selected and would normally be rendered differently than an
    unselected cell.

    \sa QSql::isNull()
*/

void ViewTable::paintCell( QPainter * p, int row, int col, const QRect & cr,
                           bool selected, const QColorGroup &cg )
{
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Datatable::paintCell(%d,%d, total:%d, selected=%d)", row, col, pViewTableModel->getRowCount(), selected ));
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Row(%d) selected:%d, Col(%d) selected:%d", currentRow(), isRowSelected(row), currentColumn(), isColumnSelected(col)));
    _GONG_DEBUG_ASSERT( pViewTableModel );
    _GONG_DEBUG_ASSERT( row >= 0 );
#if 0
    if( verticalHeader()->sectionSize( row ) != fontMetrics().height() + 2 )
        verticalHeader()->resizeSection( row, fontMetrics().height() + 2 );
#endif
    if( currentColumn() == col && currentRow() == row && hasFocus() ) {
        QColorGroup cg2 = cg;
        cg2.setBrush( QColorGroup::Base, QBrush(QColor(0xf9, 0xff, 0xce)) );
        Q3Table::paintCell( p, row, col, cr, true, cg2 );
    }
    else
        Q3Table::paintCell( p, row, col, cr, selected/*rowselected*/, cg );
    if( (unsigned int)row < pViewTableModel->getRowCount() )
    {
        p->setPen( selected ? cg.foreground() : cg.text() );
        const dbFieldDefinition *flddef = d->mFields[indexOf(col)];
        dbFieldStyle *fldstyle = 0;
        QString text;
        dbFieldStyle::Alignment al = dbFieldStyle::AlignAuto;
        Variant value(pViewTableModel->getValue(row, indexOf( col )));
        if( flddef ) {
// 			_GONG_DEBUG_PRINT(0, Xtring::printf("Field: %s, style:%s",
// 								flddef->getName().c_str(),
// 								flddef->getStyle().c_str() ) );
            if( pDatabase )
                fldstyle = pDatabase->findFieldStyle(flddef->getStyle());
        }
        if( flddef && fldstyle ) {
            Xtring fldtext = flddef->getDisplayValue(value);
            if( value.toString() == fldtext ) {
                text = toGUI(mFormatter.format( fldtext, fldstyle->getFormat().c_str(),
                                                fldstyle->getMask().c_str(), flddef->getVariantType() ).c_str() );
                al = fldstyle->getHAlignment();
            } else {
                text = toGUI( fldtext );
                al = dbFieldStyle::AlignLeft;
            }
        } else if( flddef ) { // No style
            Xtring fldtext = flddef->getDisplayValue(value);
            if( value.toString() == fldtext ) {
                text = toGUI(mFormatter.format( fldtext, 0, 0, flddef->getVariantType() ) );
                if( flddef->getSqlColumnType() == SQLINTEGER
                        || flddef->getSqlColumnType() == SQLDECIMAL
                        || flddef->getSqlColumnType() == SQLFLOAT )
                    al = dbFieldStyle::AlignRight;
            } else {
                text = toGUI( fldtext );
                al = dbFieldStyle::AlignLeft;
            }
        } else {
            text = toGUI( value.toString().c_str() );
            if( value.isNumeric(value.type() ) )
                al = dbFieldStyle::AlignRight;
        }
// 		_GONG_DEBUG_PRINT(0, Xtring::printf("Formatted Text:%s", (const char *)text ));
        // Atencion, si no se dibujan las celdas de la tabla, es por el alignment del campo
        p->drawText( 2,2, cr.width()-4, cr.height()-4, toGUI(al), text );

    }
}

/*!
    \reimp
*/

void ViewTable::swapColumns( int col1, int col2, bool )
{
    const dbFieldDefinition *fld = d->mFields[ col1 ];
    QIcon mIcons = d->mIcons[ col1 ];

    d->mFields[ col1 ] = d->mFields[ col2 ];
    d->mIcons[ col1 ] = d->mIcons[ col2 ];

    d->mFields[ col2 ] = fld;
    d->mIcons[ col2 ] = mIcons;

    int colIndex = d->colIndex[ col1 ];
    d->colIndex[ col1 ] = d->colIndex[ col2 ];
    d->colIndex[ col2 ] = colIndex;
}


void ViewTable::sync(dbRecordID id)
{
    if( id==0 )
        return;
    unsigned int row=0;
    theGuiApp->waitCursor();
    clearSelection();
    while( row < pViewTableModel->getRowCount() )
    {
        if( static_cast<dbRecordID>(pViewTableModel->
                                    getValue(row, 0).toInt()) == id )
        {
            break;
        }
        row++;
    }
    // QT4: table->scrollToItem( table->item( item->tableIndex, 0 ), QAbstractItemView::EnsureVisible );
    setCurrentCell( row, 0 );
    selectRow( row );
    scrollToCurrent();
    theGuiApp->resetCursor();
}

/*
    Look for a substring in a column.
    Used to find a substring typed from the keyboard.
*/

void ViewTable::findInColumn( int column, const Xtring & str,
                              bool caseSensitive, bool backwards )
{
    _GONG_DEBUG_ASSERT(  pViewTableModel  );
    _GONG_DEBUG_PRINT(10, Xtring::printf("Buscando: %s en la columna %d", str.c_str(), column));
    Xtring tmp, text;
    unsigned int  row = currentRow(), startRow = row;
    bool  wrap = true, found = false;

    if( str.isEmpty() )
        return;

    if( !caseSensitive )
        tmp = str.lower();
    else
        tmp = str;
    theGuiApp->waitCursor(true);
    while( wrap )
    {
        while( !found && row < pViewTableModel->getRowCount() )
        {
#if 0
            const dbFieldDefinition *flddef = d->mFields[indexOf(column)];
            _GONG_DEBUG_ASSERT(  flddef  );
            dbFieldStyle *fldstyle = theGuiApp->getDatabase()->findFieldStyle(flddef->getStyle());
            if( fldstyle )
            {
                text = mFormatter.format(pViewTableModel->getValue(row, indexOf( column )),
                                         fldstyle->getFormat().c_str(), fldstyle->getMask().c_str(), flddef->getVariantType() );
            } else {
                text = this->text(row, indexOf(column) ).latin1();
            }
#endif
            text = this->text(row, indexOf(column) ).latin1();
            if( !caseSensitive )
            {
                text = text.lower();
            }
            if( text.startsWith( tmp ) )
            {
                clearSelection();
                setCurrentCell( row, column );
                found = true;
            }
            if( !backwards )
            {
                row++;
            }
            else
            {
                row--;
            }
        }
        if( !backwards )
        {
            if( startRow != 0 )
            {
                startRow = 0;
            }
            else
            {
                wrap = false;
            }
            row = 0;
        }
        else
        {
            if( startRow != (unsigned int) (numRows() - 1) )
            {
                startRow = numRows() - 1;
            }
            else
            {
                wrap = false;
            }
            row = numRows() - 1;
        }
    }
    theGuiApp->resetCursor();
}


void ViewTable::contentsMouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() != Qt::LeftButton )
        return;
    if( rowAt( e->y() ) != -1 )
        emit beginEditSignal(this, defaulteditmode, Xtring::null );
    else
        emit beginEditSignal(this, inserting, Xtring::null );
}

void ViewTable::sortColumn( int col, bool /*ascending*/, bool /*wholeRows*/)
{
    if( pViewTableModel->getColCount() <= (unsigned int)col )
        return;
    d->sortedColumn = col;
    /// \todo {0.3.4} q->setOrderBy(d->mFields[col]->getName() + (ascending?" ASC":" DESC"));
    refresh();
}

const dbFieldDefinition *ViewTable::getFldInfo(int col)
{
    return d->mFields[col];
}

bool ViewTable::nextRow()
{
    if( currentRow() < numRows() )
    {
        setCurrentCell( currentRow() + 1, 0 );
        return true;
    }
    else
    {
        return false;
    }
}

bool ViewTable::prevRow()
{
    if( currentRow() > 0 )
    {
        setCurrentCell( currentRow() - 1, 0 );
        return true;
    }
    else
    {
        return false;
    }
}

bool ViewTable::firstRow()
{
    if( numRows() > 0 )
    {
        setCurrentCell( 0, 0 );
        return true;
    }
    else
    {
        return false;
    }
}

bool ViewTable::lastRow()
{
    if( numRows() > 0 )
    {
        setCurrentCell( numRows()-1, 0 );
        return true;
    }
    else
    {
        return false;
    }
}

void ViewTable::focusInEvent( QFocusEvent *e)
{
    Q3Table::focusInEvent( e );
    if( e->reason() != Qt::ActiveWindowFocusReason && e->reason() != Qt::OtherFocusReason ) {
        theGuiApp->sendKeyEvent(this, Qt::Key_Right, 0, 0 );
        theGuiApp->sendKeyEvent(this, Qt::Key_Left, 0, 0 );
    }
    emit focusChanged(this, e);
}

void ViewTable::focusOutEvent(QFocusEvent* e)
{
    Q3Table::focusOutEvent(e);
    emit focusChanged(this, e);
}


List<dbRecordID> ViewTable::getSelectedIDs() const
{
    List<dbRecordID> sel;
    for( int i=0; i<numRows(); i++ ) {
        if( isRowSelected(i) ) {
            _GONG_DEBUG_PRINT(10, Xtring::printf("Row %d is selected: ID=%d", i,
                                                 pViewTableModel->getRowID(i) ) );
            sel.push_back(pViewTableModel->getRowID(i));
        }
    }
    return sel;
}

void ViewTable::setSelectedIDs( const List<dbRecordID> &selrecords )
{
    if( selrecords.size() ) {
        for( int i=0; i<numRows(); i++ ) {
            if( selrecords.contains( pViewTableModel->getRowID(i) ) )
                selectRow(i);
        }
    }
}

#endif

}; // namespace gong
