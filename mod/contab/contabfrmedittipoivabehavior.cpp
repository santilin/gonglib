/*<<<<<COPYLEFT*/
/** @file contabfrmedittipoivabehavior.cpp Fichero de edición de tipos de IVA
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
// COPYLEFT Fichero de edición de tipos de IVA
// FIELD SubcuentaSoportado string
// FIELD SubcuentaRepercutido string
// TYPE FrmEditRecBehavior contab::TipoIVA
/*>>>>>MODULE_INFO*/

/*<<<<<FRMEDITTIPOIVABEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "contabfrmedittipoivabehavior.h"
/*>>>>>FRMEDITTIPOIVABEHAVIOR_INCLUDES*/

namespace gong {
namespace contab {

void FrmEditTipoIVABehavior::initGUI()
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_INITGUI*/
    QWidget *pControlsFrame = getControlsFrame();
    QVBoxLayout* pControlsLayout = getControlsLayout();
    QHBoxLayout *subcuentasoportadoLayout = new QHBoxLayout(0, 0, 6, "subcuentasoportadoLayout");
    QHBoxLayout *subcuentarepercutidoLayout = new QHBoxLayout(0, 0, 6, "subcuentarepercutidoLayout");
    editSubcuentaSoportado = pTheForm->addEditField( pControlsFrame, "TIPOIVA", "SUBCUENTASOPORTADO", subcuentasoportadoLayout );
    editSubcuentaRepercutido = pTheForm->addEditField( pControlsFrame, "TIPOIVA", "SUBCUENTAREPERCUTIDO", subcuentarepercutidoLayout );
    pControlsLayout->addLayout( subcuentasoportadoLayout );
    pControlsLayout->addLayout( subcuentarepercutidoLayout );
    /*>>>>>FRMEDITTIPOIVABEHAVIOR_INITGUI*/
}

void FrmEditTipoIVABehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_SCATTER*/
    editSubcuentaSoportado->setText( pTheForm->getRecord()->getValue("SUBCUENTASOPORTADO").toString());
    editSubcuentaRepercutido->setText( pTheForm->getRecord()->getValue("SUBCUENTAREPERCUTIDO").toString());
    /*>>>>>FRMEDITTIPOIVABEHAVIOR_SCATTER*/
}

void FrmEditTipoIVABehavior::gatherFields()
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_GATHER*/
    pTheForm->getRecord()->setValue( "SUBCUENTASOPORTADO", editSubcuentaSoportado->toString());
    pTheForm->getRecord()->setValue( "SUBCUENTAREPERCUTIDO", editSubcuentaRepercutido->toString());
    /*>>>>>FRMEDITTIPOIVABEHAVIOR_GATHER*/
}

void FrmEditTipoIVABehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    /*<<<<<FRMEDITTIPOIVABEHAVIOR_VALIDATE*/
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    ValidResult *validresult = ( ir ? ir : new ValidResult() );
    /*>>>>>FRMEDITTIPOIVABEHAVIOR_VALIDATE*/
    if( !ir ) {
        showValidMessages( isvalid, *validresult, sender);
        delete validresult;
    }
}

/*<<<<<FRMEDITTIPOIVABEHAVIOR_FIN*/
} // namespace contab
} // namespace gong
/*>>>>>FRMEDITTIPOIVABEHAVIOR_FIN*/

