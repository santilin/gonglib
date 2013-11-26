/*<<<<<COPYLEFT*/
/** @file sociasfrmeditcobrobehavior.cpp Behavior para el formulario de remesas del módulo socias
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
// COPYLEFT Behavior para el formulario de remesas del módulo socias
// TYPE FrmEditRecBehavior socias::Cobro
/*>>>>>MODULE_INFO*/
/*<<<<<FRMEDITCOBROBEHAVIOR_INCLUDES*/
#include <dbappmainwindow.h>
#include "sociasfrmeditcobrobehavior.h"
/*>>>>>FRMEDITCOBROBEHAVIOR_INCLUDES*/
#include "pagosfrmeditcobro.h"
#include "sociasrecmiembro.h"
#include "sociasmodule.h"

namespace gong {
namespace socias {

void FrmEditCobroBehavior::initGUI()
{
    /*<<<<<FRMEDITCOBROBEHAVIOR_INITGUI*/
/*>>>>>FRMEDITCOBROBEHAVIOR_INITGUI*/
}

void FrmEditCobroBehavior::scatterFields( bool is_pre )
{
    if( !is_pre) return;
    /*<<<<<FRMEDITCOBROBEHAVIOR_SCATTER*/

/*>>>>>FRMEDITCOBROBEHAVIOR_SCATTER*/
}

void FrmEditCobroBehavior::gatherFields()
{
    /*<<<<<FRMEDITCOBROBEHAVIOR_GATHER*/

/*>>>>>FRMEDITCOBROBEHAVIOR_GATHER*/
}

void FrmEditCobroBehavior::validateFields(bool is_pre,
        QWidget *sender, bool *isvalid, ValidResult *ir)
{
    if( !is_pre) return;
    /*<<<<<FRMEDITCOBROBEHAVIOR_VALIDATE*/
	bool v=true;
	if( !isvalid )
		isvalid = &v;
	ValidResult *validresult = ( ir ? ir : new ValidResult() );
/*>>>>>FRMEDITCOBROBEHAVIOR_VALIDATE*/
    pagos::FrmEditCobro *frmcobro = static_cast<pagos::FrmEditCobro *>(pTheForm);
    if( sender == frmcobro->getSearchFacturaNumero() ) {
        LineEdit *editFacturaNumero = frmcobro->getSearchFacturaNumero()->getEditCode();
        if( editFacturaNumero->isJustEdited() ) {
            LineEdit *editTerceroCodigo = frmcobro->getSearchTerceroCodigo()->getEditCode();
            if( editTerceroCodigo->toString().isEmpty() ) {
                RecMiembro *miembro = static_cast< RecMiembro *> (
                                          frmcobro->getSearchFacturaNumero()->getRecord() );
                frmcobro->getSearchFacturaNumero()->setValue( miembro->getRecContacto()->getValue( "CODIGO" ).toInt() );
            }
        }
    }

#if 0
    /// \todo {0.3.4} Hacer un validSeekCode por nombre del contacto
    if( validSeekCode( sender, isvalid, *validresult, editMiembroNumeroSocia, editMiembroContacto_Nombre,
                       getRecMiembro(), "NUMEROSOCIA", "", "MIEMBRO.PROYECTO_ID="
                       + getRecord()->getConnection()->toSQL( getRecRemesaProyecto()->getRecProyecto()->getRecordID() ) ) ) {
        if( getRecMiembro()->getValue( "PROYECTO_ID").toInt() != getRecRemesaProyecto()->getValue( "PROYECTO_ID" ).toInt() ) {
            validresult->addValidError(
                Xtring::printf("Este miembro no pertenece al proyecto %d, '%s' de esta remesa.",
                               getRecRemesaProyecto()->getRecProyecto()->getValue( "CODIGO" ).toInt(),
                               getRecRemesaProyecto()->getRecProyecto()->getValue( "DESCRIPCION" ).toString().c_str() ),
                "NUMEROSOCIA" );
            *isvalid = false;
        } else
            scatterMiembro();
    }
    if( sender == editRemesaProyectoNumero && editNumero->toString().isEmpty() ) {
        editFechaEmision->setText( getRecRemesaProyecto()->getValue("FECHAEMISION") );
        editFechaValor->setText( getRecRemesaProyecto()->getValue("FECHAEMISION") );
        editFechaCargo->setText( getRecRemesaProyecto()->getValue("FECHACARGO") );
        editDescripcion->setText( getRecRemesaProyecto()->getValue("DESCRIPCION") );
        Xtring cond_recibo =
            Xtring("SELECT MAX(NUMERO) FROM RECIBOPROYECTO" );
        cond_recibo += " WHERE REMESAPROYECTO_ID="
                       + getRecord()->getConnection()->toSQL( getRecRemesaProyecto()->getRecordID() );
        Xtring numero = getRecord()->getConnection()->selectString( cond_recibo );
        if( numero.isEmpty() ) {
            numero = getRecRemesaProyecto()->getValue("NUMERO").toString();
            numero.padL(4,'0');
            editNumero->setText( numero + "/000001" );
        } else
            editNumero->setText( Xtring::stringInc(numero) );
    }
    if( sender == editMiembroNumeroSocia && isInserting() && editImporte->toDouble() == 0.0 ) {
        double porcentajecuota = getRecMiembro()->getRecTipoSocia()->getValue("PORCENTAJECUOTA").toDouble();
        double importe = getRecRemesaProyecto()->getRecProyecto()->getValue("IMPORTE").toDouble() * porcentajecuota / 100;
        // Calcular número de recibos según la forma de pago
        /// TODO
#ifdef HAVE_PAGOSMODULE
        uint nrecibos = getRecMiembro()->getRecFormaPago()->getValue("NUMPAGOS").toInt();
#else
        uint nrecibos = 1;
#endif
        importe /= nrecibos;
        editImporte->setText( importe );
        editResto->setText( importe );
    }
    if( !sender || sender == editNumero ) {
        if( getRecord()->existsAnother(
                    "REMESAPROYECTO_ID=" + getRecord()->getConnection()->toSQL( getRecRemesaProyecto()->getRecordID() )
                    + " AND NUMERO=" + getRecord()->getConnection()->toSQL( editNumero->toString() ) ) ) {
            validresult->addValidError(
                "Ya existe un recibo en esta remesa con este número.",
                "FECHAEMISION");
            *isvalid = false;
        }
    }
#if 0
    // Comprobar que no existe un recibo en la misma fecha para la misma socia
    if( !sender ) {
        if( getRecord()->existsAnother(
                    "REMESAPROYECTO_ID=" + getRecord()->getConnection()->toSQL( getRecRemesaProyecto()->getRecordID() )
                    + " AND MIEMBRO_ID=" + getRecord()->getConnection()->toSQL( getRecMiembro()->getRecordID() )
                    + " AND FECHAEMISION=" + getRecord()->getConnection()->toSQL( editFechaEmision->toDate() ) ) ) {
            validresult->addWarning(
                "Ya existe un recibo en esta remesa para este miembro con esta fecha de emisión.",
                "FECHAEMISION");
        }
    }
#endif
    /// \todo {0.3.4} No llegar tan lejos, avisar antes, por ejemplo al elegir el proyecto o antes del formulario
    if( !sender && !getRecRemesaProyecto()->getRecProyecto()->isActivo() && (isInserting() || isDuplicating()) ) {
        validresult->addValidError( Xtring::printf("El proyecto %s de esta remesa no está activo. No se pueden añadir recibos",
                                    getRecRemesaProyecto()->getRecProyecto()->toString().c_str() ), "REMESAPROYECTO_ID");
        *isvalid = false;
    }
    // Lógica del pago
    if( !sender ) {
        if( editImporte->toMoney() < editResto->toMoney() ) {
            validresult->addValidError( "El resto no puede ser mayor que el importe", "RESTO" );
            *isvalid = false;
        }
        if( comboEstado->currentItem() == SociasModule::pagado ) {
            if( editResto->toMoney() != 0.0 ) {
                validresult->addValidError( "El resto del recibo tiene que estar a cero", "RESTO" );
                *isvalid = false;
            }
            if( editFechaPago->toDate() == Date::NullDate ) {
                validresult->addValidError( "La fecha de pago no puede estar vacía", "FECHAPAGO" );
                *isvalid = false;
            }
            if( editDocumentoPago->toString().isEmpty() ) {
                validresult->addValidError( "El documento de pago no debería estar vacío", "DOCUMENTOPAGO" );
            }
            /// \todo {0.3.4} SociasModule::devuelto
        } else if( comboEstado->currentItem() == SociasModule::pendiente ) {
            if( editResto->toMoney() == 0.0 && editImporte->toMoney() > 0.0 ) {
                validresult->addValidError( "El resto del recibo no puede estar a cero", "RESTO" );
                *isvalid = false;
            }
            if( !editFechaPago->toDate().isValid() ) {
                validresult->addValidError( "La fecha de pago tiene tiene que estar vacía", "FECHAPAGO" );
                *isvalid = false;
            }
            if( !editDocumentoPago->toString().isEmpty() ) {
                validresult->addValidError( "El documento de pago tiene que estar vacío", "DOCUMENTOPAGO" );
                *isvalid = false;
            }
        }
    }
#endif
    if( !ir ) {
        showValidMessages(isvalid, *validresult, sender);
        delete validresult;
    }
}

bool FrmEditCobroBehavior::canBeginEdit(DataTable::EditMode newmode)
{
    _GONG_DEBUG_TRACE(0);
    if( newmode == DataTable::inserting ) {
#ifdef HAVE_FACTUMODULE
        if( ModuleInstance->getFactuModule() )
            pTheForm->getRecord()->setValue( "TABLATERCEROS", "CLIENTE" );
        else
#endif
            pTheForm->getRecord()->setValue( "TABLATERCEROS", "CONTACTO" );
        pTheForm->getRecord()->setValue( "TABLAFACTURAS", "MIEMBRO" );
    }
    return true;
}

/*<<<<<FRMEDITCOBROBEHAVIOR_FIN*/
} // namespace socias
} // namespace gong
/*>>>>>FRMEDITCOBROBEHAVIOR_FIN*/

