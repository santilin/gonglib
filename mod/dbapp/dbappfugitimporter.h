#ifndef _GONG_DBAPPFUGITIMPORTER_H
#define _GONG_DBAPPFUGITIMPORTER_H

/** @file dbappdbappfugitimporter.h A parser for FUGIT files
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongxmlparser.h>
#include <qwidget.h>

namespace gong {

class FugitImporter: public XmlParser
{
public:
    typedef std::map<dbRecordID, dbRecordID> RecordIDsMap;
    class ImportTableInfo {
    public:
        ImportTableInfo(dbRecord *rec=0, RecordIDsMap recidmap = RecordIDsMap())
            : pRecord(rec), mRecordIDMap(recidmap) {};
        dbRecord *pRecord;
        RecordIDsMap mRecordIDMap;
    };
    struct ImportOptions {
        enum { OTROEJER_ABORTAR, OTROEJER_IMPORTAR, OTROEJER_IGNORAR, OTROEJER_PREGUNTAR } mOtroEjercicio;
        enum { EXISTE_ABORTAR, EXISTE_IGNORAR, EXISTE_IMPORTAR, EXISTE_PREGUNTAR} mExiste;
        enum { ERROR_IGNORAR, ERROR_ABORTAR } mError;
    };

public:
    FugitImporter(const XtringList &importtables, const Xtring &desc,
                  int pasada, ImportOptions &io, QWidget *messageswidget = 0);
    ~FugitImporter();
    void addMessage(const Xtring &message) const;

protected:
    virtual bool importRecord( const Xtring &tablename);
    virtual bool importDetalle( const Xtring &tablename);

private:
    virtual void startElement( const xmlChar *name, const xmlChar **attributes ); // From XmlParser
    virtual void endElement( const xmlChar *name ); // From XmlParser
    virtual void characters( const xmlChar *text, unsigned int len ); // From XmlParser
    virtual void error ( const char *msg, ... ); // From XmlParser

    int validate(dbRecord *rec);
    int hayqueActualizar(dbRecord *rec);
    void actRelations(dbRecord *rec);

    Dictionary<ImportTableInfo> mFilesInfo;
    Xtring mDescription;
    int mPasada, mLevel;
    ImportOptions &mImportOptions;
    Xtring mFldData;
    Xtring mFldName;
    Xtring mCurTable;
    dbRecord *pRecord, *pDetalle;
    QWidget *pWidgetMessages;
    Xtring mRecToString, mRecError;
};

} // namespace gong
#endif // _GONG_DBAPPPFUGITIMPORTER_H

