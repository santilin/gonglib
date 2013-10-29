#include "dbappfrmeditrecmaster.h"
#include "dbappfrmeditrecbehavior.h"

namespace gong {

/**
	* @brief This is a pure virtual class. It has no members, so you can reuse the same
	* instance in different forms.
	* If you want to store private information, you can do so in derived classes.
	*
	**/

QWidget* FrmEditRecBehavior::getOrCreateTab( const char *tabname)
{
    QWidget *tab = static_cast<QWidget *>(pTheForm->child( tabname ));
    if( !tab ) {
        tab = pTheForm->insertTab( 0, tabname );
        /*QVBoxLayout *vl = */
        new QVBoxLayout( tab );
        pTheForm->showTabs(true);
    }
    return tab;
}

QWidget* FrmEditRecBehavior::getControlsFrame()
{
    return pTheForm->pControlsFrame;
}

QVBoxLayout* FrmEditRecBehavior::getControlsLayout()
{
    return pTheForm->pControlsLayout;
}

QHBoxLayout* FrmEditRecBehavior::getButtonsLayout()
{
    return pTheForm->pButtonsLayout;
}

EditControlsList& FrmEditRecBehavior::getEditControlsList() const
{
    return pTheForm->mEditControls;
}

char FrmEditRecBehavior::getControlKeyPressed()
{
    return pTheForm->mControlKeyPressed;
}

DataTable* FrmEditRecBehavior::changeDataTable(DataTable *new_dt)
{
    FrmEditRecMaster *theMaster = static_cast<FrmEditRecMaster *>(pTheForm);
    delete theMaster->pDataTable;
    new_dt->hide();
    if( theMaster->mEditFlags & dbApplication::tableEditEvent )
        new_dt->setTableEditMode( DataTable::Inline );
    else
        new_dt->setTableEditMode( DataTable::None );
    theMaster->pMainLayout->insertWidget( 1, new_dt );
    theMaster->connect( new_dt, SIGNAL( currentChanged( int, int ) ), theMaster, SLOT( tableCurrentChanged( int, int ) ) );
    theMaster->connect( new_dt, SIGNAL( beginEditSignal( DataTable *, DataTable::EditMode, const Xtring& ) ),
                        theMaster, SLOT( slotBeginEdit( DataTable *, DataTable::EditMode, const Xtring& ) ) );
    theMaster->pDataTable = new_dt;
    return new_dt;
}

// FrmEditRec* FrmEditRecBehavior::findFormOf(QWidget* w) const
// {
// 	FrmEditRec *ret = 0;
// 	while( w ) {
// 		if( (ret = dynamic_cast<FrmEditRec *>(w) ) != 0 ) {
// 			return ret;
// 		} else {
// 			w = static_cast<QWidget *>(w->parent());
// 		}
// 	}
// 	return ret;
// }

void FrmEditRecBehavior::showValidMessages(bool* isvalid, const ValidResult &validresult, QWidget* sender)
{
    pTheForm->showValidMessages( isvalid, validresult, sender );
}

} // namespace

