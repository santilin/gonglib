#include <fstream>
#include <errno.h>
#include <memory>

#include <gongfileutils.h>
#include <dbappmainwindow.h>
#include <dbappfrmsqlselect.h>

#include <pagosfrmeditcobro.h>
#include <pagosreccobro.h>
#include <pagosmodule.h>
#include "sociasfrmgenerarrecibos.h"
#include "sociasrecmiembro.h"
#include "sociasmodule.h"

namespace gong {
namespace socias {

FrmGenerarRecibos::FrmGenerarRecibos ( QWidget * parent, WidgetFlags fl )
    : FrmCustom( parent, "FrmGenerarRecibos", fl)
{
    QHBoxLayout *layoutremesa = new QHBoxLayout ( 0, 0, 6, "layoutremesa" );
    searchRemesaCobroNumero = addSearchField( this, "REMESACOBRO", "Numero", "Descripcion", layoutremesa );
    pushRemesaCobroNumero = searchRemesaCobroNumero->getButton();
    connect( pushRemesaCobroNumero, SIGNAL( clicked() ), this, SLOT( pushRemesaCobroNumero_clicked() ) );
    editRemesaCobroNumero = searchRemesaCobroNumero->getEditCode();
    connect( editRemesaCobroNumero, SIGNAL( validate(QWidget *, bool *) ),
             this, SLOT(validate(QWidget*, bool*) ) );
    editRemesaCobroDescripcion = searchRemesaCobroNumero->getEditDesc();
    pControlsLayout->addLayout( layoutremesa );

    QHBoxLayout *layoutrangofechas = new QHBoxLayout ( 0, 0, 6, "layoutrangofechas" );
    QLabel *lblfechaini = new QLabel ( QString::fromUtf8( _("Periodo de los recibos: desde ") ), this, "lblfechaini" );
    layoutrangofechas->addWidget ( lblfechaini );
    editfechaini = new LineEdit ( this, "txtfechaini" );
    editfechaini->setValueType ( Variant::tDate );
    layoutrangofechas->addWidget ( editfechaini );
    QLabel *lblfechafin = new QLabel ( QString::fromUtf8( _( " hasta " ) ), this, "lblfechafin" );
    layoutrangofechas->addWidget ( lblfechafin );
    editfechafin = new LineEdit ( this, "txtfechafin" );
    editfechafin->setValueType ( Variant::tDate );
    layoutrangofechas->addWidget ( editfechafin );
    pControlsLayout->addLayout ( layoutrangofechas );

    QHBoxLayout *layoutfechasrecibos = new QHBoxLayout ( 0, 0, 6, "layoutfechasrecibos" );
    QLabel *lblfechaemi = new QLabel ( QString::fromUtf8( _("Fechas de los recibos: Emisión") ), this, "lblfechaemi" );
    layoutfechasrecibos->addWidget ( lblfechaemi );
    editfechaemi = new LineEdit ( this, "txtfechaemi" );
    editfechaemi->setValueType ( Variant::tDate );
    layoutfechasrecibos->addWidget ( editfechaemi );
    QLabel *lblfechavalor = new QLabel ( QString::fromUtf8( _("Valor") ), this, "lblfechavalor" );
    layoutfechasrecibos->addWidget ( lblfechavalor );
    editfechavalor = new LineEdit ( this, "txtfechavalor" );
    editfechavalor->setValueType ( Variant::tDate );
    layoutfechasrecibos->addWidget ( editfechavalor );
    QLabel *lblfechacargo = new QLabel ( QString::fromUtf8( _("Cargo") ), this, "lblfechacargo" );
    layoutfechasrecibos->addWidget ( lblfechacargo );
    editfechacargo = new LineEdit ( this, "txtfechacargo" );
    editfechacargo->setValueType ( Variant::tDate );
    layoutfechasrecibos->addWidget ( editfechacargo );
    pControlsLayout->addLayout ( layoutfechasrecibos );
#if 0
    QHBoxLayout *layoutinforme = new QHBoxLayout ( 0, 0, 6, "layoutinforme" );
    checkinforme = new QCheckBox ( QString::fromUtf8 ( "Informe" ), this, "checkinforme" );
    layoutinforme->addWidget ( checkinforme );
    pControlsLayout->addLayout ( layoutinforme );
#endif

    QHBoxLayout *layoutgenasiento = new QHBoxLayout ( 0, 0, 6, "layoutgenasiento" );
    checkgenasiento = new QCheckBox ( QString::fromUtf8 ( "Generar el asiento contable de cada recibo" ),
                                      this, "checkgenasiento" );
    checkgenasiento->setChecked( false );
    checkgenasiento->setVisible( false );
    layoutgenasiento->addWidget ( checkgenasiento );
    pControlsLayout->addLayout ( layoutgenasiento );

    QHBoxLayout *layoutsupervisar = new QHBoxLayout ( 0, 0, 6, "layoutsupervisar" );
    checkSupervisar = new QCheckBox ( QString::fromUtf8 ( "Supervisar los recibos generados" ),
                                      this, "checksupervisar" );
    layoutsupervisar->addWidget ( checkSupervisar );
    checkSupervisar->setChecked( true );
    checkSupervisar->setChecked( false );
    checkSupervisar->setVisible( false );
    pControlsLayout->addLayout ( layoutsupervisar );

	searchPartidaCodigo = addSearchField( this, "PARTIDAPROYECTO", "CODIGO", "DESCRIPCION" );
    searchPartidaCodigo = addMultipleSearchField( this, "TIPOSOCIA", "CODIGO", "NOMBRE" );
	
    QHBoxLayout *layoutcondiciones = new QHBoxLayout ( 0, 0, 6, "layoutfechasrecibos" );
    pushCondiciones = new QPushButton( toGUI( _("Condiciones") ), this, "pushCondiciones" );
    connect ( pushCondiciones, SIGNAL ( clicked() ), this, SLOT ( pushCondiciones_clicked() ) );
    layoutcondiciones->addWidget( pushCondiciones );
    editCondiciones = new TextEdit ( this );
    layoutcondiciones->addWidget ( editCondiciones );
    pControlsLayout->addLayout( layoutcondiciones );

//	QPushButton *pushGrabar = new QPushButton ( QString::fromUtf8( _( "&Grabar" ) ), this, "pushGrabar" );
    // signals and slots connections
//	connect ( pushGrabar, SIGNAL ( clicked() ), this, SLOT ( pushGrabar_clicked() ) );
//	layoutAceptarCancelar->addWidget ( pushGrabar );
//	pushGrabar->show();

    editfechaemi->setText( Date::currentDate() );
    editfechavalor->setText( Date::currentDate() );
    editfechacargo->setText( Date::currentDate() );
    editfechaini->setText( Date::currentDate().firstDayOfYear() );
    editfechafin->setText( Date::currentDate().lastDayOfYear() );
    setCaption( toGUI(_("Generación de recibos")) );
    pFocusWidget = editRemesaCobroNumero;
}

// Copiadas de FrmEditRemesa
void FrmGenerarRecibos::specialControlKeyPressed( QWidget *sender, char key )
{
    if( sender == editPartidaCodigo )
        pushPartidaCodigo_clicked();
    if( sender == editRemesaCobroNumero )
        pushRemesaCobroNumero_clicked();
}

void FrmGenerarRecibos::pushPartidaCodigo_clicked()
{
    editPartidaCodigo->setCancelling();
    if( DBAPP->choose(0, searchPartidaCodigo->getRecord(), 0, dbApplication::editNone, this ) ) {
        scatterPartida();
        editPartidaCodigo->setFocus();
    }
}

void FrmGenerarRecibos::scatterPartida()
{
    editPartidaCodigo->setText( searchPartidaCodigo->getRecord()->getValue("CODIGO") );
    editPartidaDescripcion->setText( searchPartidaCodigo->getRecord()->getValue("DESCRIPCION") );
}

void FrmGenerarRecibos::scatterRemesaCobro()
{
    editRemesaCobroNumero->setText( searchRemesaCobroNumero->getRecord()->getValue("NUMERO") );
    editRemesaCobroDescripcion->setText( searchRemesaCobroNumero->getRecord()->getValue("DESCRIPCION") );
    if( !editfechaemi->toDate().isValid() )
        editfechaemi->setText( searchRemesaCobroNumero->getRecord()->getValue("FECHAEMISION") );
    if( !editfechacargo->toDate().isValid() )
        editfechacargo->setText( searchRemesaCobroNumero->getRecord()->getValue("FECHACARGO") );
}

void FrmGenerarRecibos::pushRemesaCobroNumero_clicked()
{
    editRemesaCobroNumero->setCancelling();
    if( DBAPP->choose(0, searchRemesaCobroNumero->getRecord(), 0, dbApplication::editNone, this ) ) {
        scatterRemesaCobro();
        editRemesaCobroNumero->setFocus();
    }
}

void FrmGenerarRecibos::accept()
{

// Comprobar que todos los datos son correctos
    if( editRemesaCobroNumero->toInt() == 0 ) {
        msgError( this, _("No has elegido una remesa.") );
        editRemesaCobroNumero->setFocus();
        return;
    }
    // Comprobar las fechas
    if( editfechaini->toDate() == Date::NullDate ) {
        msgError( this, _("Introduce la fecha inicial del periodo de recibos") );
        editfechaini->setFocus();
        return;
    }
    if( !editfechafin->toDate().isValid() ) {
        msgError( this, _("Introduce la fecha final del periodo de recibos") );
        editfechafin->setFocus();
        return;
    }
    if( !editfechaemi->toDate().isValid() ) {
        msgError( this, _("Introduce la fecha de emisión de los recibos") );
        editfechaemi->setFocus();
        return;
    }
    if( !editfechacargo->toDate().isValid() ) {
        msgError( this, _("Introduce la fecha de cargo de los recibos") );
        editfechacargo->setFocus();
        return;
    }
    if( !editfechavalor->toDate().isValid() ) {
        msgError( this, _("Introduce la fecha de valor de los recibos") );
        editfechavalor->setFocus();
        return;
    }
    if( editfechaini->toDate() > editfechafin->toDate() ) {
        msgError( this, _("La fecha inicial es posterior a la final") );
        editfechaini->setFocus();
        return;
    }
    if( editfechacargo->toDate() < editfechaemi->toDate() ) {
        msgError( this, _("La fecha de cargo es anterior a la de emisión") );
        editfechaemi->setFocus();
        return;
    }

    pagos::RecRemesaCobro *remesa = static_cast<pagos::RecRemesaCobro *>( searchRemesaCobroNumero->getRecord() );
    // Todos los datos son correctos
    // Elegir los miembros
    dbConnection *conn = DBAPP->getConnection();
    Xtring selectstr = "SELECT PROYECTO.ID, MIEMBRO.ID"
                       " FROM PROYECTO INNER JOIN MIEMBRO ON MIEMBRO.PROYECTO_ID=PROYECTO.ID";
    if( !editCondiciones->toString().isEmpty() )
        selectstr += " INNER JOIN CONTACTO ON CONTACTO.ID=MIEMBRO.CONTACTO_ID LEFT JOIN FORMAPAGO ON FORMAPAGO.ID=MIEMBRO.FORMAPAGO_ID LEFT JOIN TIPOSOCIA ON TIPOSOCIA.ID=MIEMBRO.TIPOSOCIA_ID";
    selectstr += " WHERE PROYECTO_ID=" + conn->toSQL( remesa->getRecProyecto()->getRecordID() );
#if 0 // Prefiero no seleccionar tanto, sino comprobarlo después para avisar a la usuaria
    + " AND PM.FECHAALTA<=" + conn->toSQL( editfechafin->toDate() )
    + " AND PM.ESTADO=1"; // activo
#endif
	if( !editCondiciones->toString().isEmpty() ) {
		selectstr.append( " AND " );
		selectstr.append( editCondiciones->toString() );
	}
    selectstr += " ORDER BY PROYECTO.ID, MIEMBRO.NUMEROSOCIA";
    std::auto_ptr<dbResultSet> rs( conn->select( selectstr ) );
    if( rs->getRowCount() == 0 ) {
        msgOk(this, _("No hay miembros que reúnan estas condiciones") );
		return;
    } else {
        if( !msgYesNo(this,
                      Xtring::printf( _("Se han seleccionado %d posibles recibos. ¿Quieres continuar?"), rs->getRowCount() ) ) )
            return;
    }

    // Crear los recibos. En este punto sabemos que el proyecto es válido y el periodo de recibos cae en el intervalo
    // de vigencia del proyecto
    DBAPP->waitCursor(true);
    Xtring informe = Xtring::printf(
                         _("Generando recibos del periodo del %s al %s\n"),
                         editfechaini->toString().c_str(), editfechafin->toString().c_str() );
    if( !editCondiciones->toString().isEmpty() )
        informe += Xtring::printf( _("CONDICIONES: %s\n"), editCondiciones->toString().c_str() );
    bool nocontinuar = false;
    dbRecordID proyecto_id = 0;
    bool saltar_proyecto = false;
    /* Recorrer los miembros, agrupándolos por proyecto y remesa */
    RecProyecto *proyecto = static_cast<RecProyecto *>( DBAPP->createRecord( "PROYECTO" ) );
    RecMiembro *miembro = static_cast<RecMiembro *>( DBAPP->createRecord( "MIEMBRO" ) );
    while( rs->next() ) {
        proyecto->read( rs->toInt(0) );
        if( proyecto->getRecordID() != proyecto_id ) {
            saltar_proyecto = false;
            informe += _("\n\n=== Seleccionando el proyecto ") + proyecto->toString(TOSTRING_CODE_AND_DESC) + "\n";
            proyecto_id = proyecto->getRecordID();
            if( !proyecto->isActivo() ) {
                saltar_proyecto = true;
                informe += _("El proyecto no está activo. Se salta.\n");
            }
            if( proyecto->getValue( "FECHAINICIO" ).toDate() > editfechafin->toDate() ) {
                saltar_proyecto = true;
                informe += _("Este proyecto es posterior al intervalo seleccionado. Se salta.\n");
            }
            if( !proyecto->getValue( "FECHAFIN" ).toDate().isNull() && proyecto->getValue( "FECHAFIN" ).toDate() < editfechaini->toDate() ) {
                saltar_proyecto = true;
                informe += _("Este proyecto es anterior al intervalo seleccionado. Se salta.\n");
            }
        }
        if( saltar_proyecto )
            continue;
#if LATER
        remesa->read( rs->toInt(1) );
        if( remesa->getRecordID() != remesa_id ) {
            saltar_remesa = false;
            informe += _("\n== Seleccionando la remesa ") + remesa->toString(TOSTRING_CODE_AND_DESC) + "\n";
            remesa_id = remesa->getRecordID();
            if( remesa->getValue( "FECHAEMISION" ).toDate() > editfechafin->toDate() ) {
                saltar_remesa = true;
                informe += _("Esta remesa es posterior al intervalo seleccionado. Se salta.\n");
            }
            if( remesa->getValue( "FECHAEMISION" ).toDate() < editfechaini->toDate() ) {
                saltar_remesa = true;
                informe += _("Esta remesa es anterior al intervalo seleccionado. Se salta.\n");
            }
        }
        if( saltar_remesa == true )
            continue;
#endif
        if( miembro->read( rs->toInt(1) ) ) {
            // Calcular importe
            informe += _("= Generando recibo para el miembro ") + miembro->toString(TOSTRING_CODE_AND_DESC) + "\n";
            double porcentajecuota = miembro->getRecTipoSocia()->getValue("PORCENTAJECUOTA").toDouble();
            if( miembro->getValue( "SOCIASESTADO" ).toInt() != SociasModule::activo ) {
                informe += _("Este miembro no está activo. No se genera el recibo\n");
                continue;
            }
            if( miembro->getValue( "FECHAALTA" ).toDate() > editfechafin->toDate() ) { // Activo
                informe += _("Este miembro se dio de alta después del periodo de los recibos. No se genera el recibo\n");
                continue;
            }
            if( miembro->getRecFormaPago()->getValue("NUMPLAZOS").toInt() == 0 ) { // Activo
                informe += Xtring::printf(_("La forma de pago de este miembro, %s, no admite plazos\n"),
                                          miembro->getRecFormaPago()->toString( TOSTRING_CODE_AND_DESC ).c_str() );
                continue;
            }
            if( porcentajecuota == 0.0 ) {
                informe += _("El porcentaje de la cuota de este miembro es cero. No se genera el recibo\n");
                continue;
            } else if( porcentajecuota != 100.0 ) {
                informe += Xtring::printf(
                               _("El porcentaje de la cuota de este miembro es del %f%%.\n"), porcentajecuota );
            }
            double importe = proyecto->getValue("IMPORTE").toDouble() * porcentajecuota / 100;
            // Calcular número de recibos según la forma de pago
            // TODO
#ifdef HAVE_PAGOSMODULE
            uint nrecibos = miembro->getRecFormaPago()->getValue("NUMPLAZOS").toInt();
#else
            uint nrecibos = 1;
#endif

            importe /= nrecibos;
            Date fechacargo = editfechacargo->toDate();
#if HAVE_PAGOSMODULE
            fechacargo = fechacargo + miembro->getRecFormaPago()->getValue("DIASPRIMERPLAZO").toInt();
#endif
            Date fechavalor = editfechavalor->toDate();
            for( uint nrecibo = 0; nrecibo < nrecibos && nocontinuar == false; nrecibo ++ ) {
                // generar el recibo individual
                std::auto_ptr<pagos::RecCobro>
                recibo( static_cast<pagos::RecCobro *>(DBAPP->createRecord( "COBRO" )) );
                recibo->clear( true ); // Set custom default values
                recibo->setValue( "REMESACOBRO_ID", remesa->getRecordID() );
                recibo->setValue( "TABLAFACTURAS", "MIEMBRO" );
                miembro->getRecContacto()->read( miembro->getValue( "CONTACTO_ID" ).toInt() );
                recibo->setValue( "FACTURA_ID", miembro->getRecordID() );
                recibo->setValue( "TABLATERCEROS", "CONTACTO" );
                recibo->setValue( "RAZONSOCIAL", miembro->getRecContacto()->getValue( "RAZONSOCIAL" ) );
                recibo->setValue( "CONTACTO_ID", miembro->getValue( "CONTACTO_ID" ) );
                recibo->setValue( "DESCRIPCION", remesa->getValue("DESCRIPCION") );
                recibo->setValue( "FECHAEMISION", editfechaemi->toDate() );
                recibo->setValue( "FECHAVALOR", fechavalor );
                recibo->setValue( "IMPORTE", importe );
                recibo->setValue( "MONEDA_ID", empresa::ModuleInstance->getRecEmpresa()->getValue( "MONEDA_ID" ) );
                recibo->setValue( "RESTO", importe );
//				recibo->setValue( "CUENTABANCARIA", miembro->getValue( "CUENTABANCARIA" )  );
                recibo->setValue( "ESTADORECIBO", pagos::PagosModule::ReciboPendiente );
                informe += Xtring::printf("\tImporte: %f,\tFecha de valor: %s\n",
                                          importe, fechavalor.toString().c_str() );
                if( !recibo->existsAnother(
                            "TABLAFACTURAS=" + conn->toSQL( "MIEMBRO" )
                            + " AND FACTURA_ID=" + conn->toSQL( miembro->getRecordID() )
                            + " AND REMESACOBRO_ID=" + conn->toSQL( remesa->getRecordID() ) ) ) {
// 					+ " AND FECHAVALOR=" + conn->toSQL( fechavalor )
// 					+ " AND IMPORTE=" + conn->toSQL( importe ) ) ) {
                    Xtring lastrecibo = conn->selectString(
                                            "SELECT MAX(NUMERO) FROM COBRO WHERE REMESACOBRO_ID="
                                            + recibo->getConnection()->toSQL( remesa->getRecordID() ) );
                    Xtring numremesa = remesa->getValue("NUMERO").toString();
                    if( lastrecibo.isEmpty() ) /// \todo {refactor9} Esto se hace en varios sitios, ¿ponerlo en recreciboproyecto?
                        lastrecibo = numremesa.padL(4,'0') + "/000000";
                    lastrecibo = lastrecibo.stringInc( lastrecibo );
                    recibo->setValue( "NUMERO", lastrecibo );
                    try {
                        recibo->save(true);
#if 0
                        if( checkgenasiento->isChecked() ) {
                            // ver cómo se hace en menuTableImport
                            pagos::FrmEditCobro *frmeditrecibo =
                                static_cast<pagos::FrmEditCobro*>(
                                    DBAPP->createEditForm(0, &(*recibo), 0, DataTable::updating,
                                                          dbApplication::simpleEdition, this ) );
                            frmeditrecibo->show();
                            // TODO LATER
// 							frmeditrecibo->setPartidaProyectoID( getRecPartidaProyecto()->getRecordID() );
                            if( checkSupervisar->isChecked() )
                                frmeditrecibo->showModalFor( this, true, true );
                            else {
                                frmeditrecibo->showAndSave();
                            }
                            if( !frmeditrecibo->isSaved() ) {
                                if( msgYesNo( this, _("Has cancelado la edición del recibo. ¿Quieres continuar con el resto?") ) == false ) {
                                    nocontinuar = true;
                                    informe += "Operación cancelada por la usuaria. No se generan más recibos.\n";
                                }
                            }
                            delete frmeditrecibo;
                        }
#endif
                        DBAPP->getMainWindow()->refreshByName( Xtring::null, Xtring::null );
                    } catch( dbError &e ) {
                        informe += Xtring::printf( _("\tError: %s\n"), e.what() );
                    }
                } else {
                    informe += Xtring::printf(
                                   _("Ya existe un recibo con estos datos en esta remesa, por lo que no se generará.\n"));
                }
#ifdef HAVE_PAGOSMODULE
                fechavalor = fechavalor + miembro->getRecFormaPago()->getValue("DIASENTREPLAZOS").toInt();
                fechacargo = fechacargo + miembro->getRecFormaPago()->getValue("DIASENTREPLAZOS").toInt();
#endif
            }
        }
    }
    delete miembro;
    delete proyecto;
    DBAPP->resetCursor();
    msgOkLarge( this, _("Informe de la generación de recibos"), informe );
}

void FrmGenerarRecibos::pushCondiciones_clicked()
{
    FrmSQLSelect *sel = new FrmSQLSelect ( DBAPP->getConnection(), DBAPP->getDatabase(),
                                           0, "filtrar" );
    XtringList joinedtables;
    joinedtables << "MIEMBRO" << "FORMAPAGO" << "TIPOSOCIA" << "CONTACTO";
    if ( sel->exec ( joinedtables, editCondiciones->toString() ) == QDialog::Accepted )
        editCondiciones->setText( toGUI( sel->getSqlExpression() ) );
    delete sel;
}

void FrmGenerarRecibos::validate( QWidget *sender, bool *isvalid )
{
    ValidResult validresult;
    bool v=true;
    if( !isvalid )
        isvalid = &v;
    /*
    	if( sender == editRemesaCobroNumero
    		&& editRemesaCobroNumero->toInt() == 0 ) {
    		validresult.addMessage( INTEG_INVALID, ValidResult::invalid,
    			_("Por favor, elige una remesa"), "" );
    		*isvalid = false;
    	} else if( validSeekCode( sender, validresult, editRemesaCobroNumero,
    		editRemesaCobroDescripcion,
    		remesa, "NUMERO", "DESCRIPCION", Xtring::null) ) {
    		editRemesaCobroNumero->setText( remesa->getValue( "NUMERO" ));
    		editRemesaCobroDescripcion->setText( remesa->getValue( "DESCRIPCION" ));
    	} else {
    		*isvalid = false;
    	}
    */
    // showValidMessages
    Xtring messages;
    if ( !( *isvalid ) ) {
        // Show only the error messages
        for ( uint nm = 0; nm < validresult.count(); nm++ )
            if ( validresult.getMessageInfo( nm ).code == ValidResult::error )
                messages += validresult.getMessageInfo( nm ).message + "\n";
        msgError( this, messages );
    } else {
        // Show the warning messages
        if ( validresult.count() > 0 ) {
            for ( uint nm = 0; nm < validresult.count(); nm++ )
                messages += validresult.getMessageInfo( nm ).message + "\n";
            if( !sender ) {
                messages += _("\n¿Estás segura de grabar este registro?");
                if( !FrmBase::msgYesNo( this, messages ) )
                    *isvalid = false;
            } else
                msgOk( this, messages);
        }
    }
}

} // namespace socias
} // namespace gong



