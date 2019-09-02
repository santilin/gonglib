#include "ofertasfrmeditcobrooferta.h"

namespace gong {
namespace ofertas {

FrmEditCobroOferta::FrmEditCobroOferta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                                       EditMode editmode, dbApplication::EditFlags editflags,
                                       QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditCobro( parentfrm, master, dm, editmode, editflags, parent, "FrmEditCobroOferta", fl )
{
}

void FrmEditCobroOferta::cambiaFields()
{
	Xtring tablafacturas = "OFERTAVENTA";
	Xtring fldfactcodigo, fldfactdesc;
	Xtring tablaterceros = pRecord->getValue( "TABLATERCEROS" ).toString();
	Xtring fldterccodigo, fldtercdesc, descsingular, descplural;

	if( !searchFacturaNumero || tablafacturas != searchFacturaNumero->getTableName()
            || !searchTerceroCodigo || tablaterceros != searchTerceroCodigo->getTableName() ) {

		if( tablafacturas != Xtring::null ) {
			removeControl( searchFacturaNumero );
			mFldFactCodigo = fldfactcodigo;
			mFldFactDesc = fldfactdesc;
			if( fldfactcodigo == Xtring::null ) {
				dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablafacturas );
				if( tbldef ) {
					mFldFactCodigo = tbldef->getCodeField();
					mFldFactDesc = tbldef->getDescField();
				}
			}
			searchFacturaNumero = addSearchField( pControlsFrame, "FACTURA_ID", tablafacturas,
												mFldFactCodigo, mFldFactDesc, pFacturasLayout );
			alignLayout( pFacturasLayout, true );
			pushFacturaNumero = searchFacturaNumero->getButton();
			connect( pushFacturaNumero, SIGNAL( clicked() ), this, SLOT( pushFacturaNumero_clicked() ), Qt::UniqueConnection );
			editFacturaNumero = searchFacturaNumero->getEditCode();
			connect( editFacturaNumero, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
					this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
			editFacturaDesc = searchFacturaNumero->getEditDesc();
			editFacturaNumero->setWidthInChars(15);
			if( !isInserting() )
				searchFacturaNumero->setMustBeReadOnly(true);
			setTabOrder( editFacturaNumero, editNumero );
		} else {
			searchFacturaNumero = 0;
			pushFacturaNumero = 0;
		}

        removeControl( searchTerceroCodigo );
		if( tablaterceros != Xtring::null) {
			mFldTercCodigo = fldterccodigo;
			mFldTercDesc = fldtercdesc;
			if( fldterccodigo == Xtring::null ) {
				dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablaterceros );
				if( tbldef ) {
					dbFieldDefinition *flddeffc = tbldef->findFieldByFlags( dbFieldDefinition::CODE );
					if( flddeffc )
						mFldTercCodigo = flddeffc->getName();
					dbFieldDefinition *flddeffd = tbldef->findFieldByFlags( dbFieldDefinition::DESCRIPTION );
					if( flddeffd )
						mFldTercDesc = flddeffd->getName();
				}
			}
			searchTerceroCodigo = addSearchField( pControlsFrame, "TERCERO_ID", tablaterceros,
												mFldTercCodigo, mFldTercDesc, pTercerosLayout );
			alignLayout( pTercerosLayout, true );
			pushTerceroCodigo = searchTerceroCodigo->getButton();
			connect( pushTerceroCodigo, SIGNAL( clicked() ), this, SLOT( pushTerceroCodigo_clicked() ), Qt::UniqueConnection );
			editTerceroCodigo = searchTerceroCodigo->getEditCode();
			connect( editTerceroCodigo, SIGNAL( specialControlKeyPressed( QWidget *, char ) ),
				this, SLOT( specialControlKeyPressed( QWidget *, char ) ), Qt::UniqueConnection );
			editTerceroDesc = searchTerceroCodigo->getEditDesc();
			if( !isInserting() ) 
				searchTerceroCodigo->setMustBeReadOnly(true);
			setTabOrder( editFacturaNumero, editTerceroCodigo );
			setTabOrder( editTerceroCodigo, editNumero );
			editFacturaNumero->setWidthInChars(15);
		} else {
			searchTerceroCodigo = 0;
			pushTerceroCodigo = 0;
		}
    }

}


/*<<<<<FRMEDITCOBROOFERTA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITCOBROOFERTA_FIN*/
