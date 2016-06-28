#include <gonggettext.h>
#include <gongtextedit.h>
#include <gongfrmbase.h>
#include "dbappdbapplication.h"
#include "dbappfugitimporter.h"

namespace gong {

FugitImporter::FugitImporter(const XtringList &importtables, const Xtring &desc, int pasada,
                             struct ImportOptions &io, QWidget *wm)
    : mDescription(desc), mPasada(pasada), mLevel(0), mImportOptions(io),
      pRecord(0), pDetalle(0), pWidgetMessages(wm)
{
    _GONG_DEBUG_PRINT(3, Xtring::printf("%s(%s,%d)", __FUNCTION__, desc.c_str(), pasada) );
    // Rellenar la información de los ficheros a partir de DBAPP->getImportTablesList
    for( XtringList::const_iterator it=importtables.begin();
            it != importtables.end();
            ++ it ) {
        _GONG_DEBUG_PRINT(4, Xtring::printf("Se importará la tabla %s", (*it).c_str()) );
        dbRecord *r = DBAPP->createRecord( *it );
        ImportTableInfo ifi( r );
#if 0
        dbRecordRelation *rel = r->findRelationByRelatedTable("USUARIAS");
        if( rel )
            rel->setEnabled(false);
#endif
        mFilesInfo.insert(*it, ifi);
    }
}

FugitImporter::~FugitImporter()
{
    for( Dictionary<ImportTableInfo>::const_iterator it = mFilesInfo.begin();
            it != mFilesInfo.end();
            ++it ) {
        struct ImportTableInfo ifi = (*it).second;
        delete ifi.pRecord;
    }
}


void FugitImporter::addMessage(const Xtring &message) const
{
    if( pWidgetMessages ) {
        if( TextEdit *te = dynamic_cast<TextEdit *>(pWidgetMessages) ) {
            /// TODO: rename appendPlainText to append
            te->appendPlainText( toGUI(message) );
            DBAPP->processEvents();
        }
    }
    _GONG_DEBUG_PRINT( 4, message );
}

#ifdef HAVE_POCOLIB
void FugitImporter::characters( const Xtring &data )
#else
void FugitImporter::characters(const xmlChar *text, unsigned int len)
#endif
{
    if( mCurTable.isEmpty() || mFldName.isEmpty() || !pRecord ) {
        _GONG_DEBUG_PRINT(7, Xtring::printf("table=%s,fld=%s: Skipping", mCurTable.c_str(), mFldName.c_str()) );
        return;
    } else {
#ifndef HAVE_POCOLIB
        Xtring data( (const char *)text, len);
        _GONG_DEBUG_PRINT(7, Xtring::printf("[[{%s}]],%d fld=%s", data.c_str(), len, mFldName.c_str()) );
#endif
        mFldData += data;
    }
}


#ifdef HAVE_POCOLIB
void FugitImporter::startElement( const Xtring &name, const Attributes &attributes )
#else
void FugitImporter::startElement( const xmlChar *name, const xmlChar **attributes )
#endif
{
// // //     _GONG_DEBUG_PRINT(4, Xtring::printf("level=%d,%s,%s", mLevel, name,
// // //                                         (attributes)?((const char *)*attributes):"(null attributes)"));
    const Xtring tagname = Xtring(name).upper();
    mFldData.clear();
    dbRecordID id = Xtring(getAttrValue("id", attributes)).toInt();
    if( tagname == "FUGIT" )
    {
        mLevel++;
        if( mPasada == 1 )
            addMessage( _("Inicio de la comprobación") );
        else
            addMessage( _("Inicio de la importación") );

    }
    else if( tagname.startsWith("FICHERO-") ) {
        if( mLevel == 1 ) {
            mLevel++;
            mCurTable = tagname.mid(8);
            _GONG_DEBUG_PRINT(5, "Importando la tabla " + mCurTable );
            pRecord = mFilesInfo[mCurTable].pRecord;
            if( pRecord ) {
                pRecord->setNew();
                pRecord->clear( true ); // set custom default values
                Xtring m;
                if( mPasada == 1 )
                    m = _("\n====== Comprobando ")
                        + DBAPP->getDatabase()->findTableDefinition(mCurTable)->getDescPlural();
                else
                    m = _("\n====== Importando ")
                        + DBAPP->getDatabase()->findTableDefinition(mCurTable)->getDescPlural();
                addMessage( m );
            } else {
                dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition(mCurTable);
                Xtring desc;
                if( tbldef )
                    desc = tbldef->getDescPlural();
                else
                    desc = mCurTable;
                addMessage( Xtring(_("\n>>>>>  Saltando fichero: ")) + desc );
            }
        } else {
            addMessage( Xtring::printf( _("'%s' no puede estar dentro de otro FICHERO-"), tagname.c_str()) );
            stopParser( false );
        }
    } else if( tagname.startsWith("REGISTRO-") && mLevel == 2 ) {
        mLevel++;
        mCurTable = tagname.mid(9);
        pRecord = mFilesInfo[mCurTable].pRecord;
        if( pRecord ) {
            pRecord->setNew();
            pRecord->clear( true ); // set custom default values
            pRecord->setRecordID(id);
        }
    }
    else if( tagname.startsWith("REGISTRO-") && mLevel == 3 )
    {
        mLevel++;
        if( pRecord ) {
            // El fichero maestro toma posesión de este detalle
            pDetalle = DBAPP->createRecord( tagname.mid(9) );
            if( !pDetalle ) { /// \todo {0.3.4} Skip
                addMessage( Xtring::printf( _("%s: La tabla %s no existe"),
                                            tagname.c_str(), tagname.mid(9).c_str() ) );
                stopParser( false );
                return;
            }
            pDetalle->setNew();
            pDetalle->clear( true ); // set custom default values
            pDetalle->setRecordID(id);
        }
    }
    else if( mLevel == 1 ) {
        addMessage( Xtring::printf(
                        _("TAG '%s' debe comenzar con FICHERO- o REGISTRO-"), tagname.c_str() ) );
        stopParser( false );
    } else {
        mFldName = tagname;
    }
}


#ifdef HAVE_POCOLIB
void FugitImporter::endElement( const Xtring &name )
#else
void FugitImporter::endElement( const xmlChar *name )
#endif
{
//     _GONG_DEBUG_PRINT(4, Xtring::printf("level=%d,%s", mLevel, name));
    int  valid = 0; // Ignorar el registro
    const Xtring tagname = Xtring(name).upper();
    if( tagname == "FUGIT" ) {
        if( mLevel == 1 )
            _GONG_DEBUG_PRINT(3, "Finished");
    }
    else if( tagname.startsWith("FICHERO-") && mLevel == 2 )
    {
        mLevel--;
    }
    else if( tagname.startsWith("REGISTRO-") && mLevel == 3 )
    {
        mLevel--;
        if( pRecord )
            importRecord( tagname.mid(9) );
    }
    else if( tagname.startsWith("REGISTRO-") && mLevel == 4 )
    {
        mLevel--;
        if( pRecord )
            importDetalle( tagname.mid(9) );
    }
    else if( tagname == mFldName && (mLevel == 3 || mLevel == 4) )
    {
        if( pRecord ) {
            dbRecord *rec = (mLevel == 3 ? pRecord : pDetalle );
            if( !(mCurTable.isEmpty() || !pRecord) ) {
                _GONG_DEBUG_PRINT(5, Xtring::printf("%s.%s = %s",
                                                    mCurTable.c_str(), tagname.c_str(), mFldData.trim().c_str()) );
                char *lnum_save = setlocale( LC_NUMERIC, "C" );
                char *ltime_save = setlocale( LC_TIME, "C" );
                if( !rec->setValue(tagname, mFldData.trim()) ) {
                    addMessage(Xtring::printf( _("Campo '%s' desconocido para el fichero '%s'"),
                                               tagname.c_str(), rec->getTableName().c_str() ));
                }
                setlocale( LC_NUMERIC, lnum_save );
                setlocale( LC_TIME, ltime_save );
            }
        }
    } else {
        addMessage( Xtring::printf( _("Etiqueta inesperada '%s' a nivel %d"),
                                    tagname.c_str(), mLevel) );
    }
    if( valid == -1 )
        stopParser(false);
    return;
}


// Return value: 1=Valido,  0=Ignorar,  -1=Cancelar la instalacion

int FugitImporter::validate(dbRecord *rec)
{
    _GONG_DEBUG_PRINT(5, rec->toString( TOSTRING_DEBUG_COMPLETE_WITH_RELATIONS ) );
    ValidResult validresult;
    int ret = 1;
#ifdef TODO
    Variant ejv;
    if( rec->getTableDefinition()->findFieldDefinition( "EJERCICIO" ) )
        ejv = rec->getValue("EJERCICIO");
    if( ejv.isValid() )
    {
        int ej = ejv.toInt();
        if(  ej != DBAPP->getEjercicio() )
        {
            if( mImportOptions.mOtroEjercicio == ImportOptions::OTROEJER_IGNORAR )
            {   // Ignorarlos
                mRecError += Xtring::printf(
                                 _("Atención: Se ignora por pertenecer al ejercicio %d"), ej);
                return 0;
            } else if( mImportOptions.mOtroEjercicio == ImportOptions::OTROEJER_IMPORTAR ) {
                // Importar a este ejercicio
                mRecError += Xtring::printf(
                                 _("Atención: Se importa a este ejercicio aunque era del ejercicio %d"), ej);
                rec->setValue("EJERCICIO", DBAPP->getEjercicio() );
            } else if( mImportOptions.mOtroEjercicio == ImportOptions::OTROEJER_ABORTAR ) {
                mRecError += Xtring::printf(
                                 _("Se cancela la importación por pertenecer al ejercicio %d"), ej);
                return -1;
            }
        }
    }
    if( !rec->isValid(ValidResult::invalid, &validresult, true) )
    {
        mRecError += _("Error: ") + validresult.getMessageInfo(0).message;
        ret = -1;
    }
    else
    {
        if( validresult.count() > 0 )
        {
            mRecError += _("Atención: ") + validresult.getMessageInfo(0).message;
        }
    }
#endif
    return ret;
}

int FugitImporter::hayqueActualizar(dbRecord *rec)
{
    if( mImportOptions.mExiste == ImportOptions::EXISTE_IMPORTAR ) { // Actualizar
        return 1;
    } else if( mImportOptions.mExiste == ImportOptions::EXISTE_IGNORAR ) 	{ // Ignorar
        return 0;
    } else if( mImportOptions.mExiste == ImportOptions::EXISTE_PREGUNTAR ) { // Preguntar
        switch( FrmBase::msgYesNoCancel(
                    Xtring::printf( _("Importando %s"), rec->getTableName().c_str()),
                    Xtring::printf( _("El registro:\n%s\n ya existe. Quieres actualizarlo?"),
                                    mRecToString.c_str()), this ) )
        {
        case FrmBase::Yes:
            return 1;
        case FrmBase::No:
            mRecError += _("El registro ya existe. Ignorado por el usuario\n");
            return 0;
        case FrmBase::Cancel:
        default:
            mRecError += _("El registro ya existe. Importación cancelada por el usuario\n");
            return -1;
        }
    }
    else
    {   // mImportOptions.mExiste == ImportOptions::EXISTE_CANCELAR
        mRecError += _("El registro ya existe. Importación cancelada\n");
        return -1;
    }
}

bool FugitImporter::importRecord( const Xtring &tablename )
{
    _GONG_DEBUG_PRINT(4, Xtring::printf("%s(%s)", __FUNCTION__, tablename.c_str() ) );
    _GONG_DEBUG_ASSERT(  tablename == mCurTable  );
    actRelations(pRecord);
    int valid = validate(pRecord);
    // Se hace después del valid, para que los campos se arreglen.
    mRecToString = pRecord->toString(TOSTRING_CODE_AND_DESC);
    // En la primera pasada, suponemos que el id es correcto
    dbRecordID existingid = pRecord->getRecordID(), oldid = existingid;
    if( valid == -1 ) { // Cancelar la importación
        addMessage( mRecToString );
        if( !mRecError.isEmpty() )
            addMessage( mRecError );
        stopParser( false );
    }
    else if( valid == 0 && mPasada == 1 ) { // Ignorar este registro
        addMessage( mRecToString );
        if( !mRecError.isEmpty() )
            addMessage( mRecError );
    } else if( mPasada == 2 && valid == 1 ) { // Importar
        // Ver si existe el registro
        existingid = 0;
#ifdef TODO
        if( tablename == "EMPRESA" )
            pRecord->setValue( "CODIGO", DBAPP->getCodEmpresa() );
#endif
        bool found = pRecord->findMatchingRecord( &existingid );
        try {
            if( !found  )
            {   // No existe
                pRecord->setValue("REC_ORIGEN", mDescription );
                pRecord->setRecordID( 0 );
                if( pRecord->save(false, true) )
                {
                    addMessage( mRecToString );
                    Xtring m = "+++++ " + pRecord->getTableDefinition()->getDescSingular();
                    m += pRecord->getTableDefinition()->isFemenina() ? _(" añadida.") : _(" añadido");
                    addMessage( m );
                    existingid = pRecord->getRecordID();
                }
            }
            else
            {
                int actualiza = hayqueActualizar(pRecord);
                if( actualiza == -1 ) {
                    addMessage( mRecToString );
                    addMessage( mRecError );
                    return valid;
                } else if( actualiza == 1 ) {
                    dbRecord *existingrecord = DBAPP->createRecord( pRecord->getTableName() );
                    existingrecord->read( existingid );
                    char *lnum_save = setlocale( LC_NUMERIC, "C" );
                    char *ltime_save = setlocale( LC_TIME, "C" );
                    for( unsigned int nf=0; nf < pRecord->getFieldCount(); nf++ )
                    {
                        const dbFieldDefinition *flddef = pRecord->getTableDefinition()->getFieldDefinition(nf);
                        if( flddef->getName() != "ID" && !flddef->isCalculated() )
                            existingrecord->setValue(nf, pRecord->getValue(nf));
                    }
                    setlocale( LC_NUMERIC, lnum_save );
                    setlocale( LC_TIME, ltime_save );
                    if( existingrecord->isModified() )
                    {
                        if( existingrecord->save(false, true) ) {
                            addMessage( mRecToString );
                            Xtring m = "+++++ " + pRecord->getTableDefinition()->getDescSingular();
                            m += pRecord->getTableDefinition()->isFemenina() ? _(" modificada.") : _(" modificado");
                            addMessage( m );
                        }
                    }
                }
            }
        } catch( dbError e) {
            addMessage( mRecToString );
            addMessage( Xtring(_("Error al grabar: ")) + e.what() );
        }
    } else {
        addMessage( mRecToString );
        if( !mRecError.isEmpty() )
            addMessage( mRecError );
    }
    _GONG_DEBUG_PRINT(6, Xtring::printf("RecordIDMap_%s[%d]=%d", mCurTable.c_str(), oldid, existingid ) );
    ImportTableInfo iti = mFilesInfo[mCurTable];
    iti.mRecordIDMap[oldid] = existingid;
    mRecToString.clear();
    mRecError.clear();
    return valid;
}

bool FugitImporter::importDetalle( const Xtring &tablename )
{
    _GONG_DEBUG_PRINT(4, Xtring::printf("%s", tablename.c_str() ) );
    bool valid;
    actRelations(pDetalle);
    valid = validate(pDetalle);
    if( mPasada == 2 && valid ) {
        pRecord->addRelatedDetailRecord( tablename, pDetalle );
    }
    return valid;
}

/**
 * Actuliza los campos relacionados que tienen el valor de ID del fichero fugit al valor de ID del registro en la base de datos, que está en el mapa de ids
 * @param rec
 */
void FugitImporter::actRelations(dbRecord *rec)
{
    for( dbFieldDefinitionDict::const_iterator fldit = rec->getTableDefinition()->getFieldDefinitions().begin();
            fldit != rec->getTableDefinition()->getFieldDefinitions().end(); ++fldit ) {
        const dbFieldDefinition *flddef = fldit->second;
        const Xtring &reference = flddef->getReference();
        if( !reference.isEmpty() && !flddef->getName().startsWith("REC_") ) {
            Xtring relTable = dbFieldDefinition::extractTableName( reference );
            // Comprobar que esta relación está activa
            dbRecordRelation *rel = rec->findRelationByRelatedTable(relTable);
            if( rel && rel->isEnabled() ) {
                Xtring relField = dbFieldDefinition::extractFieldName( reference );
                const ImportTableInfo &iti = mFilesInfo[relTable];
                if( iti.pRecord ) {
                    const Xtring &fldname = flddef->getName();
                    _GONG_DEBUG_PRINT(6, Xtring::printf("%s.%s = %d, %s.%s = %d",
                                                        rec->getTableName().c_str(), fldname.c_str(),
                                                        iti.mRecordIDMap.at( rec->getValue(relField).toInt() ),
                                                        relTable.c_str(), relField.c_str(), rec->getValue(fldname).toInt() ) );
                    rec->setValue( fldname, iti.mRecordIDMap.at( rec->getValue(fldname).toInt()) );
                } else {
                    addMessage( Xtring::printf(
                                    _("No se ha importado la tabla %s relacionada con esta tabla: %s"),
                                    relTable.c_str(), rec->getTableName().c_str() ) );
                }
            }
        }
    }
}


#ifdef HAVE_POCOLIB
void FugitImporter::error( const Xtring &message )
{
    addMessage( message );
}
#else
void FugitImporter::error( const char *msg, ... )
{
    /// \todo {bug9} it looks like you can not nest va_list
    va_list args;
    char *buffer = new char[2040];
    va_start(args, msg);
    vsnprintf(buffer, 2039, msg, args);
    va_end( args );
    Xtring ret(buffer);
    delete [] buffer;
    addMessage( ret );
}
#endif

} // namespace gong

