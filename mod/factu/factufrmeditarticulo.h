/*<<<<<COPYLEFT*/
/** @file factufrmeditarticulo.h Fichero de edición de artículos
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITARTICULO_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITARTICULO_H
#define _FACTU_FACTUFRMEDITARTICULO_H
/*>>>>>FRMEDITARTICULO_PREAMBLE*/

/*<<<<<FRMEDITARTICULO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecarticulo.h"
/*>>>>>FRMEDITARTICULO_INCLUDES*/
#include "empresarectipoiva.h"
#include "facturecproveedora.h"

namespace gong {
namespace factu {

/*<<<<<FRMEDITARTICULO_CLASS*/
class FrmEditArticulo: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditArticulo(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecArticulo *getRecArticulo() const { return (RecArticulo *)getRecord(); }
/*>>>>>FRMEDITARTICULO_CLASS*/
    virtual ~FrmEditArticulo();
    void fixPrecios( const Xtring &fldchanged );

protected:
    void setStockControls();
    void scatterArticuloBase();

    /*<<<<<FRMEDITARTICULO_SCATTERS_AND_SLOTS*/
protected:
	void scatterFamilia();
	void scatterProveedora();
	void scatterTipoIVA();

private slots:
	void pushFamiliaCodigo_clicked();
	void pushProveedoraCodigo_clicked();
	void pushTipoIVACodigo_clicked();

public:
	RecFamilia* getRecFamilia() const
		{ return static_cast<RecArticulo*>(getRecord())->getRecFamilia(); }
	RecProveedora* getRecProveedora() const
		{ return static_cast<RecArticulo*>(getRecord())->getRecProveedora(); }
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecArticulo*>(getRecord())->getRecTipoIVA(); }
/*>>>>>FRMEDITARTICULO_SCATTERS_AND_SLOTS*/

    RecArticulo* getRecArticuloBase() const
    {
        return pRecArticuloBase;
    }

    /*<<<<<FRMEDITARTICULO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITARTICULO_VIRTUALS_FROM_FRMEDITREC*/
protected slots:
    void pushArticuloBaseCodigo_clicked();
    void editArticuloImagen_clicked();
    void pushGenCodArt_clicked();

    /*<<<<<FRMEDITARTICULO_CONTROLS*/
protected:
	gong::SearchBox *searchFamiliaCodigo;
	QPushButton *pushFamiliaCodigo;
	gong::LineEdit *editFamiliaCodigo;
	gong::LineEdit *editFamiliaNombre;
	gong::SearchBox *searchProveedoraCodigo;
	QPushButton *pushProveedoraCodigo;
	gong::LineEdit *editProveedoraCodigo;
	gong::LineEdit *editProveedoraRazonSocial;
	gong::EditBox *editNombre;
	gong::EditBox *editCodigoInterno;
	gong::EditBox *editCodigo;
	gong::SearchBox *searchTipoIVACodigo;
	QPushButton *pushTipoIVACodigo;
	gong::LineEdit *editTipoIVACodigo;
	gong::LineEdit *editTipoIVANombre;
	gong::EditBox *editFabricante;
	gong::EditBox *editCosteSinIVA;
	gong::EditBox *editCoste;
	gong::EditBox *editMargenComercial;
	gong::EditBox *editPVPSinIVA;
	gong::EditBox *editPVP;
	gong::EditBox *editDtoComercial;
	gong::EditBox *editPVP1;
	gong::EditBox *editPVP2;
	gong::EditBox *editPVP3;
	gong::CheckBox *checkUsarStocks;
	gong::EditBox *editStock;
	gong::EditBox *editStockInicial;
	gong::EditBox *editStockMaximo;
	gong::EditBox *editStockMinimo;
	gong::CheckBox *checkEnUso;
	gong::CheckBox *checkNovedad;
	gong::CheckBox *checkOferta;
	gong::TextBox *editNotas;
	RichTextBox *editDescripcion;
	gong::EditBox *editMultiploBase;
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuenta;
#endif
	gong::ImageBox *editArticuloImagen_Imagen;
/*>>>>>FRMEDITARTICULO_CONTROLS*/
    QVBoxLayout *pExtraLayout;
    gong::SearchBox *searchArticuloBaseCodigo;
    QPushButton *pushArticuloBaseCodigo;
    gong::LineEdit *editArticuloBaseCodigo;
    gong::LineEdit *editArticuloBaseNombre;
    QPushButton *pushGenCodArt;

    RecArticulo *pRecArticuloBase;
};

/*<<<<<FRMEDITARTICULO_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITARTICULO_H
/*>>>>>FRMEDITARTICULO_POSTAMBLE*/
