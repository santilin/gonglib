#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <q3listbox.h>
#include <QLayout>
#include <QToolTip>
#include "gonglineedit.h"
#include "gongtextedit.h"

#include <gonggettext.h>
#include <gongdbdefinition.h>
#include "dbappfrmeditrec.h"
#include "dbappfrmsqlselect.h"
#include "dbappdbapplication.h"


namespace gong {

#define OP_IGUALQUE 0
#define OP_MENORQUE 1
#define OP_MAYORQUE 2
#define OP_MENORIGUALQUE 3
#define OP_MAYORIGUALQUE 4
#define OP_DISTINTOA 5
#define OP_CONTIENE 6
#define OP_NOCONTIENE 7
#define OP_COMIENZAPOR 8
#define OP_NOCOMIENZAPOR 9
#define OP_ESTAVACIO 10
#define OP_NOESTAVACIO 11
#define OP_ESCIERTO 12
#define OP_ESFALSO 13

/*
 *  Constructs a FrmSQLSelect as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmSQLSelect::FrmSQLSelect ( dbConnection *conn, dbDefinition *db,
                             QWidget* parent, const char* name, WidgetFlags fl )
    : FrmBase ( parent, name, fl ), pDatabase( db ), pConnection( conn )
{
    if ( !name )
        setName ( "FrmSQLSelect" );
    FrmSQLSelectLayout = new Q3VBoxLayout ( this, 11, 6, "FrmSQLSelectLayout" );

    layout12 = new Q3VBoxLayout ( 0, 0, 6, "layout12" );

    layout11 = new Q3VBoxLayout ( 0, 0, 6, "layout11" );

    layout9 = new Q3HBoxLayout ( 0, 0, 6, "layout9" );

    layout6 = new Q3VBoxLayout ( 0, 0, 6, "layout6" );

    labelCampoDecha = new QLabel ( this, "labelCampoDecha" );
    layout6->addWidget ( labelCampoDecha );

    lbCamposIzda = new Q3ListBox ( this, "lbCamposIzda" );
    layout6->addWidget ( lbCamposIzda );
    layout9->addLayout ( layout6 );

    layout8 = new Q3VBoxLayout ( 0, 0, 6, "layout8" );

    labelOperador = new QLabel ( this, "labelOperador" );
    layout8->addWidget ( labelOperador );

    lbOperador = new Q3ListBox ( this, "lbOperador" );
    lbOperador->setSizePolicy ( QSizePolicy ( ( QSizePolicy::SizeType ) 1, ( QSizePolicy::SizeType ) 7, 0, 0, lbOperador->sizePolicy().hasHeightForWidth() ) );
    layout8->addWidget ( lbOperador );
    layout9->addLayout ( layout8 );

    layout7 = new Q3VBoxLayout ( 0, 0, 6, "layout7" );

    labelValorComparado = new QLabel ( this, "labelValorComparado" );
    layout7->addWidget ( labelValorComparado );

    textValorComparado = new LineEdit ( this, "textValorComparado" );
    layout7->addWidget ( textValorComparado );

    labelCampoComparado = new QLabel ( this, "labelCampoComparado" );
    layout7->addWidget ( labelCampoComparado );

    lbCamposDecha = new Q3ListBox ( this, "lbCamposDecha" );
    layout7->addWidget ( lbCamposDecha );
    layout9->addLayout ( layout7 );
    layout11->addLayout ( layout9 );

    layout11_2 = new Q3VBoxLayout ( 0, 0, 6, "layout11_2" );

    labelSQLText = new QLabel ( this, "labelSQLText" );
    layout11_2->addWidget ( labelSQLText );

    textSQL = new TextEdit ( this );
    textSQL->setObjectName( "textSQL" );
    textSQL->setSizePolicy ( QSizePolicy ( ( QSizePolicy::SizeType ) 7, ( QSizePolicy::SizeType ) 1, 0, 0, textSQL->sizePolicy().hasHeightForWidth() ) );
    layout11_2->addWidget ( textSQL );
    layout11->addLayout ( layout11_2 );

    layout9_2 = new Q3HBoxLayout ( 0, 0, 6, "layout9_2" );

    pushAnadirY = new QPushButton ( this, "pushAnadirY" );
    layout9_2->addWidget ( pushAnadirY );
    pushAnadirO = new QPushButton ( this, "pushAnadirO" );
    layout9_2->addWidget ( pushAnadirO );
    pushEliminarFiltro = new QPushButton ( this, "pushEliminarFiltro" );
    layout9_2->addWidget ( pushEliminarFiltro );
    pushRellenarCampo = new QPushButton ( this, "rellenarCampo" );
    layout9_2->addWidget ( pushRellenarCampo );

    layout11->addLayout ( layout9_2 );
    layout12->addLayout ( layout11 );

    layoutAceptarCancelar = new Q3HBoxLayout ( 0, 0, 6, "layoutAceptarCancelar" );
    layout12->addLayout ( layoutAceptarCancelar );
    FrmSQLSelectLayout->addLayout ( layout12 );
    languageChange();

    // signals and slots connections
    connect ( pushAnadirO, SIGNAL ( clicked() ), this, SLOT ( pushAnadirO_clicked() ) );
    connect ( pushAnadirY, SIGNAL ( clicked() ), this, SLOT ( pushAnadirY_clicked() ) );
    connect ( pushRellenarCampo, SIGNAL ( clicked() ), this, SLOT ( pushRellenarCampo_clicked() ) );
    connect ( lbCamposIzda, SIGNAL ( highlighted ( int ) ), this, SLOT ( lbCamposIzda_highlighted ( int ) ) );
    connect ( lbCamposIzda, SIGNAL ( doubleClicked ( Q3ListBoxItem * ) ), this, SLOT ( pushRellenarCampo_clicked() ) );
    connect ( lbCamposDecha, SIGNAL ( highlighted ( int ) ), this, SLOT ( lbCamposDecha_highlighted ( int ) ) );
    connect ( pushEliminarFiltro, SIGNAL ( clicked() ), this, SLOT ( pushEliminarFiltro_clicked() ) );
    connect ( textValorComparado, SIGNAL ( validate ( QWidget *, bool * ) ), this, SLOT ( validateValorComparado ( QWidget *, bool * ) ) );

    layoutAceptarCancelar->addWidget ( pushAccept );
    layoutAceptarCancelar->addWidget ( pushCancel );
}

FrmSQLSelect::~FrmSQLSelect()
{
    _GONG_DEBUG_TRACE(1);
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FrmSQLSelect::languageChange()
{
    setCaption ( toGUI ( _ ( "Selección de filtros" ) ) );
    labelCampoDecha->setText ( toGUI ( _ ( "Campo de la condición" ) ) );
    labelOperador->setText ( toGUI ( _ ( "Comparación" ) ) );
    lbOperador->clear();
    lbOperador->insertItem ( toGUI ( _ ( "= \"es igual que\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "< \"es menor que\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "> \"es mayor que\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "<= \"es menor o igual que\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( ">= \"es mayor o igual que\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "!= \"es distinto a\"" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "Contiene" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "No Contiene" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "Comienza por" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "No comienza por" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "Está vacío" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "No está vacío" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "es cierto" ) ) );
    lbOperador->insertItem ( toGUI ( _ ( "es falso" ) ) );
    lbOperador->setCurrentItem ( 6 );
    labelValorComparado->setText ( toGUI ( _ ( "Valor con el que se compara" ) ) );
    labelCampoComparado->setText ( toGUI ( _ ( "Campo con el que se compara" ) ) );
    labelSQLText->setText ( toGUI ( _ ( "Texto completo de la condición" ) ) );
    pushEliminarFiltro->setText ( toGUI ( _ ( "Eliminar filtro" ) ) );
    pushAnadirY->setText ( toGUI ( _ ( "Añadir condición" ) ) );
    pushAnadirO->setText ( toGUI ( _ ( "Condición alternativa" ) ) );
    pushRellenarCampo->setText( toGUI (_ ("Elegir valores") ) );
}

/*>>>>>UIC_REST*/


int FrmSQLSelect::addFieldToListBoxes ( const Xtring &text, item_info &iteminfo )
{
    lbCamposIzda->insertItem ( QString::fromLocal8Bit ( text.c_str() ) );
    lbCamposDecha->insertItem ( QString::fromLocal8Bit ( text.c_str() ) );
    mItemsInfo.push_back ( iteminfo );
    return 1;
}

Xtring FrmSQLSelect::getFldExpression ( const item_info &ii ) const
{
    Xtring exp = ii.tabledef->getName() + "." + ii.fielddef->getName();
    switch ( ii.operation )
    {
    case sumof:
        exp = "SUM(" + exp + ")";
        break;
    case countof:
        exp = "COUNT(" + exp + ")";
        break;
    case maxof:
        exp = "MAX(" + exp + ")";
        break;
    case minof:
        exp = "MIN(" + exp + ")";
        break;
    default:
        break;
    }
    return exp;
}

Xtring FrmSQLSelect::getCondition()
{
    int leftindex = lbCamposIzda->currentItem();
    if ( leftindex == -1 )
    {
        msgError( this, _( "Elige un campo para la condición" ) );
        return Xtring::null;
    }
    item_info &iileft = mItemsInfo[leftindex];
    int operindex = lbOperador->currentItem();
    if ( operindex == -1 )
    {
        msgError( this, _( "Elige un operador de comparación" ) );
        return Xtring::null;
    }
    Xtring cond;
    if ( iileft.fielddef == 0 )
    {
        for ( std::vector<item_info>::const_iterator it = mItemsInfo.begin() + 1;
                it != mItemsInfo.end();
                ++ it )
        {
            bool aplicable = true;
            if ( ( *it ).operation != none || ! ( *it ).fielddef->isVisible() )
                aplicable = false;
            if ( aplicable )
            {
                if ( ( *it ).fielddef->getSqlColumnType() == SQLDECIMAL ||
                        ( *it ).fielddef->getSqlColumnType() == SQLINTEGER ||
                        ( *it ).fielddef->getSqlColumnType() == SQLFLOAT )
                    Variant v = textValorComparado->toVariant().toDouble ( &aplicable );
            }
            if ( aplicable )
            {
                if ( ( *it ).fielddef->getSqlColumnType() == SQLDATE ||
                        ( *it ).fielddef->getSqlColumnType() == SQLTIME ||
                        ( *it ).fielddef->getSqlColumnType() == SQLDATETIME )
                    aplicable = textValorComparado->toDateTime().isValid();
            }
            if ( aplicable )
            {
                if ( ( *it ).fielddef->getSqlColumnType() == SQLBOOL )
                {
                    const Xtring boolvalue = textValorComparado->toString().upper();
                    aplicable = ( boolvalue == "S" || boolvalue == "Sí" || boolvalue == "Si" || boolvalue == "Y" || boolvalue == "1" )
                                || ( boolvalue == "N" || boolvalue == "No" || boolvalue == "N" || boolvalue == "0" );
                }
            }
            if ( aplicable )
            {
                if ( !cond.isEmpty() )
                    cond += "OR";
                cond += getSubCondition ( *it, operindex );
                if ( cond == "-1" )
                {
                    msgError( this, _( "Elige un campo o escriba un texto para la comparación" ) );
                    return Xtring::null;
                }
            }
        }
    }
    else
    {
        cond += getSubCondition ( iileft, operindex );
    }
    _GONG_DEBUG_PRINT ( 2, Xtring::printf ( "SubCondición: %s", cond.c_str() ) );
    return cond;
}

Xtring FrmSQLSelect::getSubCondition ( item_info iileft, int operindex )
{
    bool needsright = true;
    Xtring cond = "(";
    if ( operindex == OP_CONTIENE ) { // like
        cond += pConnection->toSQLLike ( getFldExpression ( iileft ), textValorComparado->toString() );
        needsright = false;
    } else if( operindex== OP_NOCONTIENE ) {
        cond += pConnection->toSQLLike ( getFldExpression ( iileft ), textValorComparado->toString(), true );
        needsright = false;
    } else if ( operindex == OP_COMIENZAPOR ) {
        cond += pConnection->toSQLStartLike ( getFldExpression ( iileft ), textValorComparado->toString() );
        needsright = false;
    } else if ( operindex == OP_NOCOMIENZAPOR ) {
        cond += pConnection->toSQLStartLike ( getFldExpression ( iileft ), textValorComparado->toString(), true );
        needsright = false;
    } else {
        cond += getFldExpression ( iileft );
        switch ( operindex )
        {
        case OP_IGUALQUE:
            cond += "=";
            break;
        case OP_MENORQUE:
            cond += "<";
            break;
        case OP_MAYORQUE:
            cond += ">";
            break;
        case OP_MENORIGUALQUE:
            cond += "<=";
            break;
        case OP_MAYORIGUALQUE:
            cond += ">=";
            break;
        case OP_DISTINTOA:
            cond += "<>";
            break;
        case OP_ESTAVACIO:
            if ( iileft.fielddef->canBeNull() )
                cond += " IS NULL ";
            else
                cond += "=\"\"";
            needsright=false;
            break;
        case OP_NOESTAVACIO:
            if ( iileft.fielddef->canBeNull() )
                cond += " IS NOT NULL ";
            else
                cond += "<>\"\"";
            needsright=false;
            break;
        }
    }
    int rightindex = lbCamposDecha->currentItem();
    if ( needsright )
    {
        if ( textValorComparado->text().isEmpty() && rightindex != -1 )
        {
            item_info &iiright = mItemsInfo[rightindex + 1];
            cond += getFldExpression ( iiright );
        }
        else
        {
            switch ( iileft.fielddef->getSqlColumnType() )
            {
            case SQLINTEGER:
                cond += pConnection->toSQL (
                            textValorComparado->toInt() ).trim();
                break;
            case SQLBOOL:
                cond += pConnection->toSQL ( textValorComparado->toBool() );
                break;
            case SQLDECIMAL:
            case SQLFLOAT:
                cond += pConnection->toSQL (
                            textValorComparado->toDouble() ).trim();
                break;
            case SQLDATE:
            case SQLTIME:
            case SQLDATETIME:
            case SQLTIMESTAMP:
                cond += pConnection->toSQL ( textValorComparado->toDateTime() );
                break;
            default:
                cond += pConnection->toSQL ( textValorComparado->toString() );
                break;
            }
        }
    }
    cond += ")";
    return cond;
}


void FrmSQLSelect::pushAnadirO_clicked()
{
    const Xtring cond = getCondition();
    if ( !cond.isEmpty() )
    {
        Xtring text = textSQL->toString();
        if ( text.isEmpty() )
            textSQL->setText ( toGUI(cond) );
        else
            textSQL->setText ( Xtring ( "(" + text + ") or " + cond ) );
    }
}

void FrmSQLSelect::pushAnadirY_clicked()
{
    Xtring cond = getCondition();
    if ( !cond.isEmpty() )
    {
        Xtring text = textSQL->toString();
        if ( text.isEmpty() )
            textSQL->setText ( toGUI(cond) );
        else
            textSQL->setText ( Xtring ( "(" + text + ") and " + cond ) );
    }
}



/**
 * Muestra una ventana con los posibles valores del campo seleccionado
 */
void FrmSQLSelect::pushRellenarCampo_clicked()
{
    int leftindex = lbCamposIzda->currentItem();
    if ( leftindex < 1 )
    {
        msgError( this, _( "Elige un campo para la condición" ) );
        lbCamposIzda->setFocus();
        return;
    }
    item_info &iileft = mItemsInfo[leftindex];
    Variant v = chooseValue( pConnection,
                             iileft.tabledef, iileft.fielddef, this );
    if( v.isValid() ) {
        textValorComparado->setText( v );
        lbOperador->setCurrentItem(0);
        pushAnadirY_clicked();
    }
}


void FrmSQLSelect::lbCamposDecha_highlighted ( int )
{
    textValorComparado->setText ( Xtring() );
}

void FrmSQLSelect::lbCamposIzda_highlighted ( int index )
{
    item_info &ii = mItemsInfo[index];
    textValorComparado->setValueType ( Variant::tString );
    textValorComparado->setMask ( Xtring::null );
    textValorComparado->setFormat ( Xtring::null );
    if ( ii.fielddef != 0 )
        FrmEditRec::applyFieldStyle ( this->textValorComparado, ii.fielddef );
    textValorComparado->setVisible( true ); // Override visibility of the field definition
    textValorComparado->setReadOnly( false ); // Override readonlyness of the field definition
    textValorComparado->setText ( Xtring() );
}

void FrmSQLSelect::pushEliminarFiltro_clicked()
{
    if ( !textSQL->toPlainText().isEmpty() )
        textSQL->setPlainText ( QString::null );
}

int FrmSQLSelect::exec ( const dbRecord *record, const Xtring &currentfilter )
{
    textSQL->setText ( currentfilter );
    List<dbTableDefinition *> tabledefs;
    if ( record ) {
        dbTableDefinition *tbldef =
            pDatabase->findTableDefinition ( record->getTableName() );
        if ( tbldef ) {
            tabledefs.push_back ( tbldef );
            for ( unsigned int i=0; i<record->getRelationsList().size(); i++ ) {
                dbRecordRelation *rel = record->getRelationsList()[i];
                if ( rel->isEnabled() ) {
                    tbldef = pDatabase->findTableDefinition( rel->getRelationDefinition()->getRightTable() );
                    if ( tbldef && !tabledefs.contains ( tbldef ) )
                        tabledefs.push_back ( tbldef );
                }
            }
        }
    }
    else
    {
        for ( unsigned int i=0; i < pDatabase->getTables().size(); i++ )
        {
            dbTableDefinition *tbldef = pDatabase->getTableDefinition ( i );
            tabledefs.push_back ( tbldef );
        }
    }
    return exec ( tabledefs, currentfilter );
}

int FrmSQLSelect::exec ( const XtringList & tables, const Xtring & currentfilter )
{
    List<dbTableDefinition *> tabledefs;
    for ( XtringList::const_iterator it = tables.begin();
            it != tables.end();
            ++ it )
    {
        dbTableDefinition *tbldef =
            pDatabase->findTableDefinition ( *it );
        if ( tbldef )
        {
            tabledefs.push_back ( tbldef );
        }
    }
    return exec ( tabledefs, currentfilter );
}

int FrmSQLSelect::exec ( const List< dbTableDefinition * > & tabledefs, const Xtring & currentfilter )
{
    item_info i0;
    i0.fielddef = 0;
    i0.tabledef = 0;
    lbCamposIzda->insertItem ( toGUI ( _ ( "Cualquier campo" ) ) );
    mItemsInfo.push_back ( i0 );
    // Fill in the listboxes with the description of the fields
    for ( List<dbTableDefinition *>::const_iterator ittable = tabledefs.begin();
            ittable != tabledefs.end();
            ++ ittable )
    {
        dbTableDefinition *tbldef = *ittable;
        for ( unsigned int i = 0; i < tbldef->getFieldCount(); i++ )
        {
            dbFieldDefinition *fielddef = tbldef->getFieldDefinition ( i );
            Xtring desc = fielddef->getCaption().isEmpty() ? fielddef->getDescription().isEmpty() ? fielddef->getName() : fielddef->getDescription() : fielddef->getCaption();
            item_info ii;
            ii.tabledef = tbldef;
            ii.fielddef = fielddef;
            switch ( fielddef->getVariantType() )
            {
            case Variant::tInt:
            case Variant::tLong:
            case Variant::tDouble:
            case Variant::tMoney:
                ii.operation = none;
                addFieldToListBoxes ( tbldef->getDescSingular() + ". " + desc, ii );
                /*
                			ii.operation = sumof;
                			addFieldToListBoxes(tabledef->getDescription() + ".Suma de: " + fielddef->getDescription(), ii);
                			ii.operation = maxof;
                			addFieldToListBoxes(tabledef->getDescription() + ".Máximo de: " + fielddef->getDescription(), ii);
                			ii.operation = minof;
                			addFieldToListBoxes(tabledef->getDescription() + ".Mínimo de: " + fielddef->getDescription(), ii);
                */
                break;
            case Variant::tDate:
                ii.operation = none;
                addFieldToListBoxes ( tbldef->getDescSingular() + ". " + desc, ii );
                ii.operation = yearof;
                addFieldToListBoxes ( tbldef->getDescSingular() + ".Año de: " + fielddef->getDescription(), ii );
                ii.operation = monthof;
                addFieldToListBoxes ( tbldef->getDescSingular() + ".Mes de: " + fielddef->getDescription(), ii );
                ii.operation = dayof;
                addFieldToListBoxes ( tbldef->getDescSingular() + ".Día de: " + fielddef->getDescription(), ii );
                /*
                			ii.operation = maxof;
                			addFieldToListBoxes(tbldef->getDescSingular() + ".Máximo de: " + fielddef->getDescription(), ii);
                			ii.operation = minof;
                			addFieldToListBoxes(tbldef->getDescSingular() + ".Mínimo de: " + fielddef->getDescription(), ii);
                */
                break;
            default:
                ii.operation = none;
                addFieldToListBoxes ( tbldef->getDescSingular() + ". " + desc, ii );
                break;
            }
        }
    }
    textSQL->setText ( QString::fromUtf8 ( currentfilter.c_str() ) );
    lbCamposIzda->setCurrentItem ( 0 );
    pFocusWidget = textValorComparado;
    showModalFor( getRealParent(), true, true );
    return !wasCancelled();
}


Xtring FrmSQLSelect::getSqlExpression()
{
    return textSQL->toString();
}

/**
 * Se comporta como una búsqueda normal de caracteres si se elige la condicion por defecto,
   "cualquier campo contiene valor"
 * @param
 * @param
 */
void FrmSQLSelect::validateValorComparado ( QWidget *, bool * )
{
    if ( focusWidget() && strcmp ( focusWidget()->name(), "lbCamposDecha" ) == 0 )
    {
        // Presumiblemente se ha pulsado INTRO.
        if ( !textValorComparado->toString().isEmpty() &&
                lbCamposIzda->currentItem() == 0 &&
//		        textSQL->toString().isEmpty() &&
                lbOperador->currentItem() == OP_CONTIENE )
        {
            pushAnadirY_clicked();
            accept();
        }
    }
}

Variant FrmSQLSelect::chooseValue(dbConnection *conn,
                                  const dbTableDefinition *tbldef, const dbFieldDefinition *flddef,
                                  QWidget *parent, const char *name, WidgetFlags fl)
{
    Xtring fldname = flddef->getName().upper();
    Xtring tablename = tbldef->getName().upper();
    if( fldname.endsWith("_ID") ) { // Foreign key
        tablename = fldname.left( fldname.find("_ID") );
        tbldef = pDatabase->findTableDefinition( tablename );
        _GONG_DEBUG_ASSERT( tbldef );
        fldname = "ID";
    }
    if( fldname == "ID" ) {
        // Buscar el código y la descripción en la tabla.
        for( uint nf = 0; nf < tbldef->getFieldCount(); nf ++ ) {
            if( tbldef->getFieldDefinition(nf)->isCode() )
                fldname += "," + tbldef->getFieldDefinition(nf)->getName();
            if( tbldef->getFieldDefinition(nf)->isDescription() )
                fldname += "," + tbldef->getFieldDefinition(nf)->getName();
        }
    }
    Xtring sql = "SELECT DISTINCT " + fldname + " FROM " + tablename + " ORDER BY 1";
    dbResultSet *rs = conn->select(sql);
    bool habia = false;
    XtringList strings;
    List<Variant> variants;
    while( rs->next() ) {
        habia = true;
        variants.push_back( rs->getValue(0) );
        _GONG_DEBUG_PRINT( 4, "Adding " + rs->toString(0) + " to the " + Xtring::number(variants.size()) + " variant[]" );
        Xtring astring;
        for( uint nf = 0; nf < rs->getColumnCount(); nf ++ ) {
            if( nf != 0 )
                astring += ", ";
            astring += rs->toString( nf );
        }
        strings << astring;
    }
    delete rs;
    if( !habia ) {
        msgWarning(this,
                   Xtring::printf("El campo %s del fichero de %s no contiene valores",
                                  flddef->getDescription().c_str(), tbldef->getDescPlural().c_str() ) );
    } else {
        int choosenindex = msgXtringList( this,
                                          Xtring::printf( _("Elige un valor para el campo %s del fichero de %s"),
                                                  flddef->getDescription().c_str(), tbldef->getDescPlural().c_str() ),
                                          strings );
        if( choosenindex != -1 )
            return variants[choosenindex];
    }
    return Variant();
}

}

