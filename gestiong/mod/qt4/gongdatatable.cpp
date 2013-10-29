#include <vector>
#include <qapplication.h>
#include <qlayout.h>
#include <qpainter.h>
#include <q3popupmenu.h>
#include <q3valuelist.h>
#include <qdatetime.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QPointer>

#include <gonggettext.h>
#include <gongdbdefinition.h>
#include <gongdbfielddefinition.h>
#include <gongdbfieldlistofvalues.h>
#include <gongtabledatamodel.h>
#include "gongguiapplication.h"
#include "gongdatatable.h"

namespace gong {

class DataTablePrivate
{
public:
    DataTablePrivate() :
        sortedColumn( -1 ), pDataModel( 0 ),
        tbEditMode( DataTable::None ) {}
    ~DataTablePrivate() {}
    void clear();

    typedef std::vector< unsigned int > ColIndex;
    ColIndex colIndex;
    int sortedColumn;
    std::vector<const dbFieldDefinition *> fldInfo;
    std::vector< QIcon > fldIcon;
    TableDataModel *pDataModel;
    Xtring searchString;
    DataTable::TableEditMode tbEditMode;
};



// Clears all the lists in DataTablePrivate
void DataTablePrivate::clear()
{
    fldInfo.clear();
    fldIcon.clear();
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

DataTable::DataTable ( dbDefinition *database,
                       QWidget * parent, const char * name )
    : Q3Table( parent, name ), pDatabase( database),
      mFormatter(*GongLibraryInstance->getRegConfig())
{
    init();
}

/*! \internal
*/


void DataTable::init()
{
    d = new DataTablePrivate();
    setFocusStyle( FollowStyle );
    setColumnMovingEnabled(false);
    setSorting(true);
    setVerticalHeader(false);
    reset();

}

/*!
    Destroys the object and frees any allocated resources.
*/

DataTable::~DataTable()
{
    delete d;
}


void DataTable::setVerticalHeader( bool visible )
{
    verticalHeader()->setVisible( visible );
    if( visible )
        setLeftMargin(20);
    else
        setLeftMargin(0);
}

/*!
  Hace que el Modelo de datos sea el origen de datos de la tabla.
  En realidad, aquí solamente define la lista de columnas,
  ni siquiera las crea en la tabla. Cuando se llama a refresh,
  se crean las columnas realmente en la tabla.
*/
void DataTable::setDataModel( TableDataModel *dm )
{
    _GONG_DEBUG_ASSERT( dm );
    d->pDataModel = dm;
//    setView( 0 );
}

bool DataTable::setView( int newview )
{
    if( newview >= (int)d->pDataModel->getViewCount() )
        newview = 0;
    d->pDataModel->setViewIndex(newview);
    setNumRows( d->pDataModel->getRowCount() );
    d->clear();
    d->sortedColumn = 1; // Para saltarse la columna ID
    d->searchString.clear();
    const dbViewDefinition *curView = d->pDataModel->getCurrentView();
    if( curView )
    {
        for ( unsigned int i = 0; i < curView->getFieldCount(); ++i )
        {
            _GONG_DEBUG_PRINT(10, Xtring::printf("Añadiendo columna: %s",
                                                 curView->getFieldDefinition(i)->getName().c_str() ) );
            /// \todo {refactor} esto realmente tiene que ser const
            addColumn( curView->getFieldDefinition(i), QIcon() );
        }
        refresh( RefreshColumns );
        return true;
    } else
        return false;
}


/*!
    \overload void DataTable::refresh( Refresh mode )

    Refreshes the table. If there is no currently defined query (see
    setTableDataModel()), nothing happens. The \a mode parameter determines
    which type of refresh will take place.

    \sa Refresh setTableDataModel() addColumn()
*/

void DataTable::refresh( DataTable::Refresh mode )
{
    _GONG_DEBUG_ASSERT(  d->pDataModel  );
    bool refreshData = ( mode & RefreshData );
    bool refreshCol = ( mode & RefreshColumns || ((int)d->fldInfo.size() != numCols() ) );
    bool refreshNumRows = ( mode & RefreshNumRows );
    if( !refreshData && !refreshCol && !refreshNumRows )
        return;
    viewport()->setUpdatesEnabled( false );
    if( refreshData ) {
        setView( d->pDataModel->getCurrentViewIndex() );
    }
    if( refreshNumRows ) {
        setNumRows( d->pDataModel->getRowCount() );
    }
    if ( refreshCol )
    {
        setNumCols( 0 );
        d->colIndex.clear();
        unsigned int fpos = (unsigned int)-1;
        // Loop through all the columns added to fldInfo with addColumn to match them with the
        // datamodel fields
        for( unsigned int i = 0; i < d->fldInfo.size(); ++i )
        {
            Xtring colname = d->pDataModel->getColName(i).upper();
            Xtring fullfldname;
            if ( colname.find(".") == Xtring::npos )
                fullfldname = d->fldInfo[i]->getName().upper();
            else
                fullfldname = d->fldInfo[i]->getFullName().upper();
            if( colname == fullfldname )
            {   // If the field in the table column definition and in the datamodel are in the same position
                fpos = i;
            }
            else
            {
                // otherwise we take the first field that matches the desired name
                for( fpos = 0; fpos < d->pDataModel->getColCount(); fpos++)
                {
                    if( fullfldname == d->pDataModel->getColName(fpos).upper() )
                        break;
                }
                if( fpos == d->pDataModel->getColCount() )
                    fpos = (unsigned int)-1;
            }
            if ( fpos!=(unsigned int)-1 )
            {
                setNumCols( numCols() + 1 );
                d->colIndex.push_back( fpos );
                setColumnReadOnly( numCols()-1, true );
                Q3Header* h = horizontalHeader();
                QIcon icons = d->fldIcon[ i ];
                h->setLabel( numCols()-1, icons, toGUI(d->fldInfo[i]->getCaption().c_str()) );
                if ( !d->fldInfo[i]->isVisible() )
                    Q3Table::hideColumn(i);
                else
                {
                    // Here, sqlwidth can be the real sqlwidth defined for the field or the width defined in the attribute w: of the view
                    int w = d->fldInfo[i]->getDisplayWidth();
                    if( w == 0 && pDatabase ) {
                        dbFieldStyle *styledef = pDatabase->findFieldStyle(d->fldInfo[i]->getStyle());
                        w = styledef?styledef->getWidth():0;
                    }
                    if( w == 0 )
                        w = d->fldInfo[i]->getCaption().length();
                    Q3Table::setColumnWidth( i, int(w * fontMetrics().width("m")) );
                }
                _GONG_DEBUG_PRINT(10, Xtring::printf("Añadida columna[%d] con caption='%s' y nombre='%s' en la posición %d",
                                                     i, d->fldInfo[i]->getCaption().c_str(),
                                                     d->pDataModel->getColName(i).c_str(), fpos));
            }
            else
            {
                _GONG_DEBUG_WARNING( Xtring::printf("Columna '%s' no encontrada", d->fldInfo[i]->getName().c_str()) );
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
    Equivalent to calling refresh( DataTable::RefreshData ).
*/

void DataTable::refresh()
{
    refresh( RefreshData );
}


/*!
    Resets the table so that it displays no data.

    \sa setTableDataModel()
*/

void DataTable::reset()
{
    clearCellWidget( currentRow(), currentColumn() );
    verticalScrollBar()->setValue(0);
    setNumRows(0);
    ensureCellVisible( 0, 0 );
    d->clear();
    if ( sorting() )
        horizontalHeader()->setSortIndicator( -1 );
}


/*!
    Adds the next column to be displayed using the field \a fieldName,
    column label \a label, width \a width and iconset \a iconset.

    If \a label is specified, it is used as the column's header label,
    otherwise the field's display label is used when setTableDataModel() is
    called. The \a iconset is used to set the icon used by the column
    header; by default there is no icon.

    \sa setTableDataModel() refresh()
*/

void DataTable::addColumn(const dbFieldDefinition *fldinfo, const QIcon& iconset)
{
    _GONG_DEBUG_PRINT(10, Xtring::printf("Añadiendo campo %p,%s a la tabla con caption=%s y width=%d",
                                         fldinfo, fldinfo->getFullName().c_str(), fldinfo->getCaption().c_str(), fldinfo->getDisplayWidth() ) );
    d->fldInfo.push_back( fldinfo );
    d->fldIcon.push_back( iconset );
}

/*!
    Removes column \a col from the list of columns to be displayed. If
    \a col does not exist, nothing happens.

    \sa GSqlField
*/

void DataTable::removeColumn( unsigned int col )
{
    if ( col < d->fldInfo.size() )
    {
        d->fldInfo.erase( d->fldInfo.begin() + col );
        d->fldIcon.erase ( d->fldIcon.begin() + col );
    }
}

/**
 * @brief If the form is not heigh enough, the last row may still get hiden
 *
 * @return bool
 **/
void DataTable::scrollToCurrent()
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

void DataTable::setTableEditMode(TableEditMode em)
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

DataTable::TableEditMode DataTable::getTableEditMode() const
{
    return d->tbEditMode;
}

/*!
    Returns the data model used by the data table.
*/

TableDataModel* DataTable::getDataModel() const
{
    return d->pDataModel;
}


/*! \reimp */
/*
  QDataTable usa eventFilter para procesar keyPressEvent.
  Sin embargo, eso hace que cuando se pulsa Ctrl+X, el eventFilter
  obtenga la pulsación del Ctrl sólamente, y luego la de X, así que
  Ctrl no hace nada y luego se usa la tecla X para buscar.
  Usando keyPressEvent, se obtiene la X y el Control a la vez, asi
  que aquí no se usa y se pasa directamente al padre.
 */
void DataTable::keyPressEvent( QKeyEvent *ke )
{
    if( !d->pDataModel || d->pDataModel->getRowCount() == 0 )
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
void DataTable::contentsContextMenuEvent( QContextMenuEvent* e )
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
    QPointer<Q3PopupMenu> popup = new Q3PopupMenu( this, "DataTableContextMenu" );
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

int DataTable::indexOf( unsigned int i ) const
{
    if( (int)i == -1 ) // what a hack!
        return d->fldInfo.size();
    else
        return d->colIndex.at(i);
}


/*!
    Returns the text in cell \a row, \a col, or an empty string if the
    cell is empty. If the cell's value is NULL then nullText() will be
    returned. If the cell does not exist then QString::null is
    returned.
*/

QString DataTable::text ( int row, int col ) const
{
    _GONG_DEBUG_ASSERT(  d->pDataModel  );

    QString s;
    if ( (unsigned int)row < d->pDataModel->getRowCount() )
        s = toGUI(d->pDataModel->getValue(row, indexOf( col ) ).toString());
    return s;
}

/*!
    Returns the value in cell \a row, \a col, or an invalid value if
    the cell does not exist or has no value.
*/

Variant DataTable::value ( int row, int col ) const
{
    _GONG_DEBUG_ASSERT(  d->pDataModel  );

    Variant v;
    if ( (unsigned int)row < d->pDataModel->getRowCount() )
        v = d->pDataModel->getValue(row, indexOf( col ) );
    return v;
}


/*!
    Sorts column \a col in ascending order if \a ascending is TRUE
    (the default); otherwise sorts in descending order.

    The \a wholeRows parameter is ignored; DataTable always sorts
    whole rows by the specified column.
*/

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

void DataTable::paintCell( QPainter * p, int row, int col, const QRect & cr,
                           bool selected, const QColorGroup &cg )
{
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Datatable::paintCell(%d,%d, total:%d, selected=%d)", row, col, d->pDataModel->getRowCount(), selected ));
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Row(%d) selected:%d, Col(%d) selected:%d", currentRow(), isRowSelected(row), currentColumn(), isColumnSelected(col)));
    _GONG_DEBUG_ASSERT( d->pDataModel );
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
    if( (unsigned int)row < d->pDataModel->getRowCount() )
    {
        p->setPen( selected ? cg.foreground() : cg.text() );
        const dbFieldDefinition *flddef = d->fldInfo[indexOf(col)];
        dbFieldStyle *fldstyle = 0;
        QString text;
        dbFieldStyle::Alignment al = dbFieldStyle::AlignAuto;
        Variant value(d->pDataModel->getValue(row, indexOf( col )));
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

void DataTable::swapColumns( int col1, int col2, bool )
{
    const dbFieldDefinition *fld = d->fldInfo[ col1 ];
    QIcon fldIcon = d->fldIcon[ col1 ];

    d->fldInfo[ col1 ] = d->fldInfo[ col2 ];
    d->fldIcon[ col1 ] = d->fldIcon[ col2 ];

    d->fldInfo[ col2 ] = fld;
    d->fldIcon[ col2 ] = fldIcon;

    int colIndex = d->colIndex[ col1 ];
    d->colIndex[ col1 ] = d->colIndex[ col2 ];
    d->colIndex[ col2 ] = colIndex;
}


void DataTable::sync(dbRecordID id)
{
    if( id==0 )
        return;
    unsigned int row=0;
    theGuiApp->waitCursor();
    clearSelection();
    while( row < d->pDataModel->getRowCount() )
    {
        if( static_cast<dbRecordID>(d->pDataModel->
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

void DataTable::findInColumn( int column, const Xtring & str,
                              bool caseSensitive, bool backwards )
{
    _GONG_DEBUG_ASSERT(  d->pDataModel  );
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
        while( !found && row < d->pDataModel->getRowCount() )
        {
#if 0
            const dbFieldDefinition *flddef = d->fldInfo[indexOf(column)];
            _GONG_DEBUG_ASSERT(  flddef  );
            dbFieldStyle *fldstyle = theGuiApp->getDatabase()->findFieldStyle(flddef->getStyle());
            if( fldstyle )
            {
                text = mFormatter.format(d->pDataModel->getValue(row, indexOf( column )),
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


void DataTable::contentsMouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() != Qt::LeftButton )
        return;
    if( rowAt( e->y() ) != -1 )
        emit beginEditSignal(this, defaulteditmode, Xtring::null );
    else
        emit beginEditSignal(this, inserting, Xtring::null );
}

void DataTable::sortColumn( int col, bool /*ascending*/, bool /*wholeRows*/)
{
    if( d->pDataModel->getColCount() <= (unsigned int)col )
        return;
    d->sortedColumn = col;
    /// \todo {0.3.4} q->setOrderBy(d->fldInfo[col]->getName() + (ascending?" ASC":" DESC"));
    refresh();
}

const dbFieldDefinition *DataTable::getFldInfo(int col)
{
    return d->fldInfo[col];
}

bool DataTable::nextRow()
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

bool DataTable::prevRow()
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

bool DataTable::firstRow()
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

bool DataTable::lastRow()
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

void DataTable::focusInEvent( QFocusEvent *e)
{
    Q3Table::focusInEvent( e );
    if( e->reason() != Qt::ActiveWindowFocusReason && e->reason() != Qt::OtherFocusReason ) {
        theGuiApp->sendKeyEvent(this, Qt::Key_Right, 0, 0 );
        theGuiApp->sendKeyEvent(this, Qt::Key_Left, 0, 0 );
    }
    emit focusChanged(this, e);
}

void DataTable::focusOutEvent(QFocusEvent* e)
{
    Q3Table::focusOutEvent(e);
    emit focusChanged(this, e);
}


List<dbRecordID> DataTable::getSelectedIDs() const
{
    List<dbRecordID> sel;
    for( int i=0; i<numRows(); i++ ) {
        if( isRowSelected(i) ) {
            _GONG_DEBUG_PRINT(10, Xtring::printf("Row %d is selected: ID=%d", i,
                                                 d->pDataModel->getRowID(i) ) );
            sel.push_back(d->pDataModel->getRowID(i));
        }
    }
    return sel;
}

void DataTable::setSelectedIDs( const List<dbRecordID> &selrecords )
{
    if( selrecords.size() ) {
        for( int i=0; i<numRows(); i++ ) {
            if( selrecords.contains( d->pDataModel->getRowID(i) ) )
                selectRow(i);
        }
    }
}

}; // namespace gong
