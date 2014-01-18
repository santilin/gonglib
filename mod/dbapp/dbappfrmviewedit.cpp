#include <algorithm>
#include <QListWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <gonggettext.h>
#include "dbappdbapplication.h"
#include "dbappfrmeditrecmaster.h"
#include "dbappfrmviewedit.h"


namespace gong {

/*
 *  Constructs a FrmViewEdit as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FrmViewEdit::FrmViewEdit(DataTable::EditMode editmode,
                         dbRecord *record, dbViewDefinition *viewdef,
                         QWidget* parent, WidgetFlags fl )
    : FrmBase ( parent, "FrmViewEdit", fl ),
      mEditMode( editmode ), pViewDefinition( viewdef ), pRecord( record )
{
    verticalLayout = new QVBoxLayout(this);
    mainLayout = new QVBoxLayout();

    mainLayout_2 = new QVBoxLayout();

    layViewName = new QVBoxLayout();
    lblViewName = new QLabel(this);
    lblViewName->setObjectName(QString::fromUtf8("lblViewName"));
    layViewName->addWidget(lblViewName);
    txtViewName = new LineEdit(this, "txtViewName");
    layViewName->addWidget(txtViewName);

    layViewOrigin = new QVBoxLayout();
    lblViewOrigin = new QLabel(this);
    lblViewOrigin->setObjectName(QString::fromUtf8("lblViewOrigin"));
    layViewOrigin->addWidget(lblViewOrigin);
    txtViewOrigin = new LineEdit(this, "txtViewOrigin");
    txtViewOrigin->setReadOnly( true );
    layViewOrigin->addWidget(txtViewOrigin);

    fieldsLayout = new QHBoxLayout();
    layFields = new QVBoxLayout();
    layFields->setObjectName(QString::fromUtf8("layFields"));
    lblFields = new QLabel(this);
    lblFields->setObjectName(QString::fromUtf8("lblFields"));

    layFields->addWidget(lblFields);

    lstFields = new QListWidget(this);
    lstFields->setObjectName(QString::fromUtf8("lstFields"));
    connect( lstFields, SIGNAL( itemChanged ( QListWidgetItem * ) ),
             this, SLOT( itemChanged ( QListWidgetItem * ) ) );
    connect( lstFields, SIGNAL ( currentItemChanged(QListWidgetItem*,QListWidgetItem*) ),
             this, SLOT ( lstFields_currentItemChanged(QListWidgetItem*,QListWidgetItem*) ) );
    layFields->addWidget(lstFields);
    fieldsLayout->addLayout(layFields);


    layFieldButtons = new QHBoxLayout();
    pushUp = new QPushButton(this);
    layFieldButtons->addWidget(pushUp);
    connect( pushUp, SIGNAL ( clicked() ), this, SLOT ( pushUp_clicked() ) );
    pushUp->setText( toGUI(_("Subir")) );
    pushDown = new QPushButton(this);
    layFieldButtons->addWidget(pushDown);
    connect( pushDown, SIGNAL ( clicked() ), this, SLOT ( pushDown_clicked() ) );
    pushDown->setText( toGUI(_("Bajar")) );
    pushAddField = new QPushButton(this);
    layFieldButtons->addWidget(pushAddField);
    connect( pushAddField, SIGNAL ( clicked() ), this, SLOT ( pushAddField_clicked() ) );
    pushAddField->setText( toGUI(_("Añadir")) );
    layFields->addLayout( layFieldButtons );

    frmFieldProps = new QFrame(this);
    frmFieldProps->setObjectName(QString::fromUtf8("frmFieldProps"));
    frmFieldProps->setFrameShape(QFrame::StyledPanel);
    frmFieldProps->setFrameShadow(QFrame::Raised);
    verticalLayout_3 = new QVBoxLayout(frmFieldProps);
    layProps = new QVBoxLayout();

    layCaption = new QVBoxLayout();
    lblCaption = new QLabel(frmFieldProps);
    lblCaption->setObjectName(QString::fromUtf8("lblCaption"));
    layCaption->addWidget(lblCaption);
    txtCaption = new LineEdit(frmFieldProps, "txtCaption");
    layCaption->addWidget(txtCaption);
    layProps->addLayout(layCaption);
    connect( txtCaption, SIGNAL( validate( QWidget *, bool * ) ),
             this, SLOT( validate( QWidget *, bool * ) ) );

    layWidth = new QVBoxLayout();
    lblWidth = new QLabel(frmFieldProps);
    lblWidth->setObjectName(QString::fromUtf8("lblWidth"));
    layWidth->addWidget(lblWidth);
    txtWidth = new LineEdit(frmFieldProps, "txtWidth", Variant::tInt);
    layWidth->addWidget(txtWidth);
    layProps->addLayout(layWidth);
    connect( txtWidth, SIGNAL( validate( QWidget *, bool * ) ),
             this, SLOT( validate( QWidget *, bool * ) ) );

    layStyle = new QVBoxLayout();
    lblStyle = new QLabel(frmFieldProps);
    layStyle->addWidget(lblStyle);
    txtStyle = new LineEdit( this, "txtStyle" );
    layStyle->addWidget(txtStyle);
    layProps->addLayout(layStyle);
    connect( txtStyle, SIGNAL( validate( QWidget *, bool * ) ),
             this, SLOT( validate( QWidget *, bool * ) ) );

    spProps = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    layProps->addItem(spProps);
    verticalLayout_3->addLayout(layProps);
    fieldsLayout->addWidget(frmFieldProps);
    layViewName->addLayout(fieldsLayout);
    mainLayout_2->addLayout(layViewName);
    mainLayout_2->addLayout(layViewOrigin);
    mainLayout->addLayout(mainLayout_2);

    horizontalLayout = new QHBoxLayout();
    chkIncludeFilter = new QCheckBox(this);
    chkIncludeFilter->setObjectName(QString::fromUtf8("chkIncludeFilter"));
    horizontalLayout->addWidget(chkIncludeFilter);
    connect( chkIncludeFilter, SIGNAL( stateChanged(int)), this, SLOT(viewModified()) );

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    mainLayout->addLayout(horizontalLayout);

    QHBoxLayout *layOrderBy = new QHBoxLayout();
    lblOrderBy = new QLabel(frmFieldProps);
    layOrderBy->addWidget(lblOrderBy);
    txtOrderBy = new LineEdit( this, "txtOrderBy" );
    layOrderBy->addWidget(txtOrderBy);
    mainLayout->addLayout(layOrderBy);
    connect( txtOrderBy, SIGNAL( validate( QWidget *, bool * ) ),
             this, SLOT( validate( QWidget *, bool * ) ) );

    settingsLayout = new QHBoxLayout();
    lblSetting = new QLabel(frmFieldProps);
    settingsLayout->addWidget(lblSetting);
    comboSetting = new QComboBox(this);
    comboSetting->setObjectName(QString::fromUtf8("comboSetting"));
    settingsLayout->addWidget(comboSetting);
    mainLayout->addLayout(settingsLayout);

    buttonsLayout = new QHBoxLayout();
    if( (pFrmEditRecMaster = dynamic_cast<FrmEditRecMaster *>( parent ) ) ) {
        pushApply = new QPushButton(this);
        buttonsLayout->addWidget(pushApply);
        connect ( pushApply, SIGNAL ( clicked() ), this, SLOT ( pushApply_clicked() ) );
        pushApply->setText( _("Aplicar sin grabar") );
    }
    sbButtons = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonsLayout->addItem(sbButtons);
    buttonsLayout->addWidget(pushCancel);
    buttonsLayout->addWidget(pushAccept);
    mainLayout->addLayout(buttonsLayout);
    verticalLayout->addLayout(mainLayout);

    lblViewName->setText( toGUI(_("Nombre de la vista")) );
    lblViewOrigin->setText(toGUI(_("Origen")) );
    lblFields->setText(toGUI(_("Campos")) );
    lblCaption->setText(toGUI(_("Encabezado")) );
    lblWidth->setText(toGUI(_("Ancho")) );
    lblStyle->setText(toGUI(_("Estilo")) );
    lblSetting->setText( toGUI(_("Guardar en") ) );
    lblOrderBy->setText(toGUI(_("Ordenar por")));
    chkIncludeFilter->setText( toGUI( _("Incluir el filtro actual" ) ) );
    comboSetting->insertItem( toGUI( DBAPP->getUserLocalSettings()->getDescription() ) );
    comboSetting->insertItem( toGUI( DBAPP->getGlobalSettings()->getDescription() ) );
	if( mEditMode == DataTable::updating )
		comboSetting->insertItem( toGUI( _("No cambiar" ) ) );
	pushAccept->setText( toGUI( _("Grabar") ) );
    pushCancel->setText( toGUI( _("Cancelar") ) );

    txtViewName->setText( viewdef->getCaption() );
    txtViewOrigin->setText( viewdef->getOrigin() );
    txtOrderBy->setText( viewdef->getOrderBy() );
    Xtring title;

    switch( mEditMode ) {
    case DataTable::inserting:
        pFocusWidget = txtViewName;
        txtOrderBy->setText("2");
        title = _("Añadiendo una nueva vista a la tabla de %2$s");
        break;
    case DataTable::updating:
        title = _("Modificando la vista '%1$s'");
        txtViewName->setEnabled( false );
		comboSetting->setCurrentIndex( 2 );
        break;
    case DataTable::deleting:
        title = _("Eliminando la vista '%1$s'");
        break;
    default:
        throw std::runtime_error("Invalid editmode in FrmViewEdit");
    }
    mSaveSettingIndex = comboSetting->currentIndex();
    setCaption( toGUI(Xtring::printf( title,
                                      viewdef->getOrigin().startsWith("DEFAULT:") || pViewDefinition->getName().endsWith(".DEFAULT")
                                      ? _( "por defecto" ) : pViewDefinition->getCaption().c_str(),
                                      pRecord->getTableDefinition()->getDescPlural().c_str() ) ) );
}

int FrmViewEdit::addFieldToList ( const Xtring &text, ViewEditListItemInfo &iteminfo )
{
    ViewEditListItem *wi = new ViewEditListItem( iteminfo, toGUI( text ), lstFields );
    if( iteminfo.inView && iteminfo.fielddef->isVisible() )
        wi->setCheckState( Qt::Checked );
    else
        wi->setCheckState( Qt::Unchecked );
    if( iteminfo.fielddef->getName() == "ID" )
        wi->setHidden( true );
    return 1;
}

Xtring FrmViewEdit::getFldExpression ( const ViewEditListItemInfo &ii ) const
{
    Xtring exp = ii.tabledef->getName() + "." + ii.fielddef->getName();
    return exp;
}

void FrmViewEdit::pushApply_clicked()
{
    if( pFrmEditRecMaster ) {
        updateView();
        if( txtViewName->toString().isEmpty() )
            txtViewName->setText("Temporal");
        accept();
    }
}

void FrmViewEdit::itemChanged ( QListWidgetItem * anitem )
{
    if( anitem ) {
        ViewEditListItem *item = static_cast<ViewEditListItem *>(anitem);
        ViewEditListItemInfo &ii = item->getItemInfo();
        if( (!ii.fielddef->isVisible() && (item->checkState() != Qt::Unchecked))
                || (ii.fielddef->isVisible() && (item->checkState() == Qt::Unchecked)) ) {
            ii.fielddef->setVisible( item->checkState() != Qt::Unchecked );
            pViewDefinition->setModified( true );
        }
    }
}

void FrmViewEdit::validate( QWidget *sender, bool *is_valid )
{
    ViewEditListItem *previtem = static_cast<ViewEditListItem *>( lstFields->currentItem() );
    if( sender == txtCaption && txtCaption->edited() ) {
        Xtring caption = txtCaption->toString();
        if( caption.find(":") != Xtring::npos
                || caption.find("|") != Xtring::npos ) {
            msgError( this, _("Los encabezados no pueden contener los símbolos \":\" ni \"|\".") );
            txtCaption->setText( caption.replace(":", "_").replace("|","_") );
            setWiseFocus(txtCaption);
        }
        previtem->getItemInfo().fielddef->setCaption( txtCaption->toString() );
        pViewDefinition->setModified( true );
    }
    if( sender == txtStyle && txtStyle->edited() ) {
        previtem->getItemInfo().fielddef->setStyle( txtStyle->toString() );
    }
    if( sender == txtWidth && txtWidth->edited() ) {
        previtem->getItemInfo().fielddef->setDisplayWidth( txtWidth->toInt() );
    }
    pViewDefinition->setModified( true );
}


void FrmViewEdit::lstFields_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if( current ) {
        ViewEditListItem *item = static_cast<ViewEditListItem *>(current);
        ViewEditListItemInfo &ii = item->getItemInfo();
        txtCaption->setText( ii.fielddef->getCaption() );
        txtStyle->setText( ii.fielddef->getStyle() );
        txtWidth->setText( (int) ii.fielddef->getDisplayWidth() );
    }
    updateStatus();
}

void FrmViewEdit::updateStatus()
{
    int currow = lstFields->currentRow();
    pushUp->setEnabled( currow > 1 );
    pushDown->setEnabled( currow < lstFields->count() - 1 );
}

void FrmViewEdit::pushUp_clicked()
{
    QListWidgetItem *current = lstFields->currentItem();
    if( !current ) return;
    int currIndex = lstFields->row(current);

    QListWidgetItem *prev = lstFields->item(lstFields->row(current) - 1);
    if( !prev ) return;
    int prevIndex = lstFields->row(prev);

    QListWidgetItem *temp = lstFields->takeItem(prevIndex);
    lstFields->insertItem(prevIndex, current);
    lstFields->insertItem(currIndex, temp);
    pViewDefinition->setModified( true );
    updateStatus();
}

void FrmViewEdit::pushDown_clicked()
{
    QListWidgetItem *current = lstFields->currentItem();
    if( !current ) return;
    int currIndex = lstFields->row(current);

    QListWidgetItem *next = lstFields->item(lstFields->row(current) + 1);
    if( !next ) return;
    int nextIndex = lstFields->row(next);

    QListWidgetItem *temp = lstFields->takeItem(nextIndex);
    lstFields->insertItem(currIndex, temp);
    lstFields->insertItem(nextIndex, current);
    pViewDefinition->setModified( true );
    updateStatus();
}

void FrmViewEdit::pushAddField_clicked()
{
    Xtring chosenfldname = chooseField( DBAPP->getDatabase(),
                                        pViewDefinition->getFirstFrom(),
                                        this );
    dbFieldDefinition *fielddef = DBAPP->getDatabase()->findFieldDefinition ( chosenfldname );
    if( !fielddef )
        fielddef = DBAPP->getDatabase()->findFieldDefinition ( chosenfldname.mid( chosenfldname.find('.')+1 ));
    if( fielddef ) {
        Xtring desc = fielddef->getCaption().isEmpty() ? fielddef->getDescription().isEmpty() ? fielddef->getName() : fielddef->getDescription() : fielddef->getCaption();
        ViewEditListItemInfo ii;
        ii.tabledef = DBAPP->getDatabase()->findTableDefinition( fielddef->getTableName() );
        ii.fielddef = fielddef;
        ii.inView = true;
        if( fielddef->getName() == "ID" )
            ii.inView = false;
        Xtring descsing;
        if( ii.tabledef )
            descsing = ii.tabledef->getDescSingular();
        else
            descsing = fielddef->getTableName();
        addFieldToList( descsing + ". " + desc, ii );
        lstFields->setCurrentRow( lstFields->count() - 1);
        pViewDefinition->setModified( true );
    }
    updateStatus();
}


int FrmViewEdit::exec()
{
    // Fill in the list of fields with the description of the fields
    for ( unsigned int i = 0; i < pViewDefinition->getFieldCount(); i++ )
    {
        dbFieldDefinition *fielddef = pViewDefinition->getFieldDefinition ( i );
        Xtring desc = fielddef->getCaption().isEmpty() ? fielddef->getDescription().isEmpty() ? fielddef->getName() : fielddef->getDescription() : fielddef->getCaption();
        ViewEditListItemInfo ii;
        ii.tabledef = DBAPP->getDatabase()->findTableDefinition( fielddef->getTableName() );
        ii.fielddef = fielddef;
        ii.inView = true;
        if( fielddef->getName() == "ID" )
            ii.inView = false;
        Xtring descsing;
        if( ii.tabledef )
            descsing = ii.tabledef->getDescSingular();
        else
            descsing = fielddef->getTableName();
        addFieldToList( descsing + ". " + desc, ii );
    }
    lstFields->setCurrentRow(1); // skip ID
    updateStatus();
    showModalFor( getRealParent(), false, true );
    if( !wasCancelled() ) {
        if( pViewDefinition->isModified()
			|| pViewDefinition->getCaption() != txtViewName->toString()
			|| mSaveSettingIndex != comboSetting->currentIndex() ) {
            updateView();
            saveView();
        }
    }
    return !wasCancelled();
}

void FrmViewEdit::addRelatedFields(XtringList &fieldnames, const dbDefinition *db,
                                   const Xtring &tablename, size_t levels,
                                   const Xtring &othertables)
{
    _GONG_DEBUG_PRINT(10, "Adding fields for table " + othertables + "." + tablename );
    // Avoid recursion
    size_t n = std::count(othertables.begin(), othertables.end(), '.');
    if( n >= levels - 1 )
        return;
    if( othertables.find( tablename + "." ) != Xtring::npos )
        return;
    dbTableDefinition *tbldef = db->findTableDefinition( tablename );
    if( tbldef ) {
        for( unsigned int nf = 0; nf < tbldef->getFieldDefinitions().size(); nf++ ) {
            dbFieldDefinition *flddef = tbldef->getFieldDefinition(nf);
            if( flddef->isVisible() )
                fieldnames << Xtring(othertables.isEmpty() ? "" : othertables + ".")
                           + flddef->getFullName();
        }
        for( unsigned int nr = 0; nr < tbldef->getRelationDefinitions().size(); nr++ ) {
            dbRelationDefinition *reldef = tbldef->getRelationDefinitions()[nr];
            Xtring relatedtable = reldef->getRightTable();
            Xtring others = othertables;
            if( !others.isEmpty() )
                others += ".";
            others += tablename;
            addRelatedFields( fieldnames, db, relatedtable, levels, others);
        }
    }
}

bool FrmViewEdit::saveView()
{
    Xtring origin = pViewDefinition->getOrigin();
    Xtring caption = pViewDefinition->getCaption();
    if( caption.upper().startsWith( pViewDefinition->getDescPlural().upper() + " " ) )
        caption = caption.mid( pViewDefinition->getDescPlural().size() + 1 );
    else
        pViewDefinition->setCaption( pViewDefinition->getDescPlural() + " " + caption );
    if( origin.startsWith("DEFAULT:") )
        origin = "CONFIG:DBDEF.VIEW." + origin.mid( strlen("DEFAULT:") ) + ".DEFAULT";
    else if( origin.startsWith("NEW:") )
        origin = "CONFIG:DBDEF.VIEW." + pViewDefinition->getFirstFrom() + "."
                 + caption.replace(" ","_");
    origin = origin.mid( strlen("CONFIG:") );
    if( chkIncludeFilter->isChecked() && pFrmEditRecMaster )
        pViewDefinition->setWhere( pFrmEditRecMaster->getWholeFilter() );
    pViewDefinition->setOrderBy( txtOrderBy->toString() );
    _GONG_DEBUG_PRINT(0, "Must save view " + pViewDefinition->getCaption() +
                      " to origin " + origin + "=" + pViewDefinition->toConfigString() );
	switch( comboSetting->currentIndex() ) {
		case 0:
			DBAPP->getUserLocalSettings()->setValue( origin, pViewDefinition->toConfigString() );
			break;
		case 1:
			DBAPP->getGlobalSettings()->setValue( origin, pViewDefinition->toConfigString() );
			break;
		case 2:
			DBAPP->updateSetting( origin, pViewDefinition->toConfigString() );
			break;
	}
    pViewDefinition->setModified( false );
    DBAPP->setViewsFromConfig();
    return true;
}

Xtring FrmViewEdit::chooseField(const dbDefinition *db,
                                const Xtring &tablename,
                                QWidget *parent,
                                const char *name, WidgetFlags fl)
{
    XtringList fieldnames;
    Xtring othertables;
    addRelatedFields( fieldnames, db, tablename, 2, othertables);
    int choosenindex = msgXtringList( this,
                                      _("Elige un campo para añadir a la vista"),
                                      fieldnames );
    if( choosenindex != -1 )
        return fieldnames[choosenindex];
    return Xtring();
}

void FrmViewEdit::updateView()
{
    pViewDefinition->clearFields();
    pViewDefinition->setCaption( txtViewName->toString() );
    for( int row = 0; row < lstFields->count(); ++row ) {
        ViewEditListItem *item = static_cast<ViewEditListItem *>(lstFields->item(row));
        ViewEditListItemInfo &ii = item->getItemInfo();
        if( ii.fielddef->getName() == "ID" || item->checkState() != Qt::Unchecked ) {
            _GONG_DEBUG_PRINT(10, ii.fielddef->getName() );
            pViewDefinition->addField( ii.fielddef );
        }
    }
    pViewDefinition->createFromFromFields();
}

bool FrmViewEdit::canClose()
{
    if( !wasCancelled() ) {
        if( txtViewName->toString().isEmpty() ) {
            msgError( this, _("El nombre de la vista no puede estar vacío") );
            setWiseFocus(txtViewName);
            return false;
        }
        Xtring origin = pViewDefinition->getOrigin();
        if( origin.startsWith("DEFAULT:") )
            origin = "CONFIG:DBDEF.VIEW." + origin.mid( strlen("DEFAULT:") ) + ".DEFAULT";
        else if( origin.startsWith("NEW:") )
            origin = "CONFIG:DBDEF.VIEW." + pViewDefinition->getFirstFrom() + "."
                     + Xtring(pViewDefinition->getCaption()).replace(" ","_");
        bool isnew = (mEditMode == DataTable::inserting);
        if( isnew && DBAPP->getAppSetting( origin ).isValid() ) {
            msgError(this, Xtring::printf(_("La vista '%s' ya existe. Elige otro nombre"),
                                          txtViewName->toString().c_str() ) );
            return false;
        }
    }
    return true;
}

void FrmViewEdit::viewModified()
{
    pViewDefinition->setModified( true );
}

}

