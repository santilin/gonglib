/*<<<<<FRMEDITAPUNTEOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITAPUNTEOFERTA_H
#define _OFERTAS_OFERTASFRMEDITAPUNTEOFERTA_H
/*>>>>>FRMEDITAPUNTEOFERTA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditapunteoferta.h Fichero de edición de
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

/*<<<<<FRMEDITAPUNTEOFERTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ofertasrecapunteoferta.h"
/*>>>>>FRMEDITAPUNTEOFERTA_INCLUDES*/
#include <empresarecproyecto.h>

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITAPUNTEOFERTA_CLASS*/
class FrmEditApunteOferta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditApunteOferta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecApunteOferta *getRecApunteOferta() const { return (RecApunteOferta *)getRecord(); }
/*>>>>>FRMEDITAPUNTEOFERTA_CLASS*/
    /*<<<<<FRMEDITAPUNTEOFERTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITAPUNTEOFERTA_VIRTUALS_FROM_FRMEDITREC*/
    void changeTipoApunte();
    virtual bool canBeginEdit( EditMode newmode ); // from FrmEditRec

/*<<<<<FRMEDITAPUNTEOFERTA_SCATTERS_AND_SLOTS*/
protected:
	void scatterCuentaOferta();
	void scatterProyecto();

private slots:
	void pushCuentaOfertaCodigo_clicked();
	void pushProyectoCodigo_clicked();

public:
	RecCuentaOferta* getRecCuentaOferta() const
		{ return static_cast<RecApunteOferta*>(getRecord())->getRecCuentaOferta(); }
	empresa::RecProyecto* getRecProyecto() const
		{ return static_cast<RecApunteOferta*>(getRecord())->getRecProyecto(); }
/*>>>>>FRMEDITAPUNTEOFERTA_SCATTERS_AND_SLOTS*/
    RecTercero* getRecTercero() const
    {
        return static_cast<RecApunteOferta*>(getRecord())->getRecTercero();
    }
    RecConcepto* getRecConcepto() const
    {
        return static_cast<RecApunteOferta*>(getRecord())->getRecConcepto();
    }
protected:
    void scatterTercero();
    void scatterConcepto();
private slots:
    void pushTerceroCodigo_clicked();
    void pushConceptoCodigo_clicked();


/*<<<<<FRMEDITAPUNTEOFERTA_CONTROLS*/
protected:
	gong::EditBox *editNumero;
	gong::CheckBox *checkAutomatico;
	gong::EditBox *editFecha;
	gong::EditBox *editImporte;
	gong::SearchBox *searchCuentaOfertaCodigo;
	QPushButton *pushCuentaOfertaCodigo;
	gong::LineEdit *editCuentaOfertaCodigo;
	gong::LineEdit *editCuentaOfertaNombre;
	gong::ComboBoxXtring *comboTablaTerceros;
	gong::EditBox *editTercero;
	gong::ComboBoxXtring *comboTablaConceptos;
	gong::EditBox *editConcepto;
	gong::EditBox *editReferencia;
	gong::SearchBox *searchProyectoCodigo;
	QPushButton *pushProyectoCodigo;
	gong::LineEdit *editProyectoCodigo;
	gong::LineEdit *editProyectoNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITAPUNTEOFERTA_CONTROLS*/
    QBoxLayout *pTercerosLayout, *pConceptosLayout;
    gong::SearchBox *searchTerceroCodigo;
    QPushButton *pushTerceroCodigo;
    gong::LineEdit *editTerceroCodigo;
    gong::LineEdit *editTerceroNombre;
    gong::SearchBox *searchConceptoCodigo;
    QPushButton *pushConceptoCodigo;
    gong::LineEdit *editConceptoCodigo;
    gong::LineEdit *editConceptoNombre;
    Xtring mFldConcCodigo, mFldConcDesc, mFldTercCodigo, mFldTercDesc;
};

/*<<<<<FRMEDITAPUNTEOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITAPUNTEOFERTA_H
/*>>>>>FRMEDITAPUNTEOFERTA_POSTAMBLE*/

