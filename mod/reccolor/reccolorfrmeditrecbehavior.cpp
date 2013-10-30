
#include <QMenu>
#include <QMenuBar>
#include <dbappfrmeditrecmaster.h>
#include "reccolormodule.h"
#include "reccolordatatable.h"
#include "reccolorfrmeditrecbehavior.h"


namespace gong {
namespace reccolor {


void ColorFrmEditRecBehavior::initGUI()
{
// 	pTheForm->addEditField( getControlsFrame(pTheForm),
// 									   pTheForm->getRecord()->getTableName(), "REC_COLOR");
    DataTable *dt = new DataTable( DBAPP->getDatabase(), pTheForm, "pDataTable" );
    changeDataTable(dt);
    pMenuRecColor = new QMenu( pTheForm );
    pTheForm->getMenuBar()->insertItem( QString::fromUtf8(_( "&Colores" )), pMenuRecColor );

    Xtring scolor = _("&0.- Borrar color");
    QAction *action = new QAction( toGUI( scolor), QKeySequence( toGUI("SHIFT+CTRL+0" ) ), pTheForm, 0 );
    connect( action, SIGNAL( activated() ), this, SLOT( menuRecColor_clicked() ) );
    pMenuRecColor->addAction( action );
    for( uint nc = 1; nc < 15; ++nc ) {
        scolor = ModuleInstance->getModuleSetting( "COLORNAME_" + Xtring::number(nc) ).toString();
        if( scolor.isEmpty() )
            scolor = _("Color " ) + Xtring::number( nc );
        if( nc < 10 ) {
            scolor = "&" + Xtring::number( nc ) + ".- " + scolor;
            action = new QAction( toGUI( scolor), QKeySequence( toGUI("SHIFT+CTRL+" + Xtring::number(nc)) ), pTheForm, 0 );
        } else {
            scolor = Xtring::number( nc ) + ".- " + scolor;
            action = new QAction( toGUI( scolor), pTheForm );
        }
        connect( action, SIGNAL( activated() ), this, SLOT( menuRecColor_clicked() ) );
        pMenuRecColor->addAction( action );
    }
    action = new QAction( toGUI( _("&20.- Otro color") ), pTheForm );
    connect( action, SIGNAL( activated() ), this, SLOT( menuRecColor_clicked() ) );
    pMenuRecColor->addAction( action );
}

void ColorFrmEditRecBehavior::menuRecColor_clicked()
{
    QAction *action = static_cast<QAction *>(sender());
    Xtring text = fromGUI(action->text());
    int ncolor;
    if( text.startsWith("&") )
        ncolor = text.mid(1).toInt(); // Skip &
    else
        ncolor = text.toInt();
    if( ncolor == 20 ) {
        if( !FrmBase::inputBoxInt( ncolor, _("Introduce un código de color"), Xtring::null, 0 ) )
            return;
    }
    FrmEditRecMaster *theform = static_cast<FrmEditRecMaster *>( action->parent() );
    dbRecord *r = theform->getRecord();
    if( theform->read() ) {
        Xtring sql = "UPDATE " + r->getTableName()
                     + " SET REC_COLOR=" + r->getConnection()->toSQL( ncolor )
                     + " WHERE ID=" + r->getConnection()->toSQL( r->getRecordID() );
        r->getConnection()->exec( sql );
        theform->refresh();
    } else {
        _GONG_DEBUG_WARNING( "No se pudo leer el registro" );
    }
}

void ColorFrmEditRecBehavior::scatterFields( bool is_pre )
{
    if( is_pre ) return;
    LineEdit *le_color = 0;
    for ( EditControlsList::const_iterator it = getEditControlsList().begin();
            it != getEditControlsList().end();
            ++ it ) {
        if( Xtring((*it)->name()).endsWith( pTheForm->getRecord()->getTableName() + "_REC_COLOR" ) ) {
            le_color = static_cast<LineEdit *>(*it);
            le_color->setText( pTheForm->getRecord()->getValue( "REC_COLOR").toString() );
            break;
        }
    }
}

void ColorFrmEditRecBehavior::gatherFields( bool is_pre )
{
    if( is_pre ) return;
    LineEdit *le_color = 0;
    for ( EditControlsList::const_iterator it = getEditControlsList().begin();
            it != getEditControlsList().end();
            ++ it ) {
        if( Xtring((*it)->name()).endsWith( pTheForm->getRecord()->getTableName() + "_REC_COLOR" ) ) {
            le_color = static_cast<LineEdit *>(*it);
            pTheForm->getRecord()->setValue( "REC_COLOR", le_color->toInt() );
            break;
        }
    }
}

} // namespace reccolor
} // namespace gong
