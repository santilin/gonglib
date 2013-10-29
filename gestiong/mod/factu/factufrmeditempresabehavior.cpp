/*<<<<<COPYLEFT*/
/** @file factufrmeditempresabehavior.cpp Behavior para añadir el campo recargo de equivalencia
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Behavior para añadir el campo recargo de equivalencia
// FIELD RecargoEquivalencia bool tabFactu
// TYPE FrmEditRecBehavior factu::Empresa
/*>>>>>MODULE_INFO*/

#include <gonggettext.h>
/*<<<<<FRMEDITEMPRESABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "factufrmeditempresabehavior.h"
/*>>>>>FRMEDITEMPRESABEHAVIOR_INCLUDES*/

namespace gong {
namespace factu {

void FrmEditEmpresaBehavior::initGUI()
{
    /*<<<<<FRMEDITEMPRESABEHAVIOR_INITGUI*/
    QWidget *pControlsFrame = getControlsFrame();
    QVBoxLayout* pControlsLayout = getControlsLayout();
    QWidget *tabFactu = getOrCreateTab( "tabFactu" );
    QVBoxLayout *tabFactuLayout = static_cast<QVBoxLayout *>( tabFactu->layout() );
    QHBoxLayout *recargoequivalenciaLayout = new QHBoxLayout(0, 0, 6, "recargoequivalenciaLayout");
    checkRecargoEquivalencia = pTheForm->addCheckField( tabFactu, "EMPRESA", "RECARGOEQUIVALENCIA", recargoequivalenciaLayout );
    tabFactuLayout->addLayout( recargoequivalenciaLayout );
    /*>>>>>FRMEDITEMPRESABEHAVIOR_INITGUI*/
    pTheForm->setTabTitle( tabFactu, _("Facturación") );
}

void FrmEditEmpresaBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITEMPRESABEHAVIOR_SCATTER*/
    checkRecargoEquivalencia->setChecked( pTheForm->getRecord()->getValue("RECARGOEQUIVALENCIA").toBool());
    /*>>>>>FRMEDITEMPRESABEHAVIOR_SCATTER*/
}

void FrmEditEmpresaBehavior::gatherFields()
{
    /*<<<<<FRMEDITEMPRESABEHAVIOR_GATHER*/
    pTheForm->getRecord()->setValue( "RECARGOEQUIVALENCIA", checkRecargoEquivalencia->isChecked());
    /*>>>>>FRMEDITEMPRESABEHAVIOR_GATHER*/
}

void FrmEditEmpresaBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITEMPRESABEHAVIOR_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    /*>>>>>FRMEDITEMPRESABEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}


/*<<<<<FRMEDITEMPRESABEHAVIOR_FIN*/
} // namespace factu
} // namespace gong
/*>>>>>FRMEDITEMPRESABEHAVIOR_FIN*/


