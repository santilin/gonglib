#ifndef _GONG_DBRECORD_H
#define _GONG_DBRECORD_H

/** @file gongdbrecord.h An in-memory table record
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"
#include "gongregexp.h"
#include "gongdbtabledefinition.h"
#include "gongdbrecordrelation.h"
#include "gongdbfieldvalue.h"
#include "gongdbresultset.h"

namespace gong {

class dbUser;

const int TOSTRING_DEBUG = 0;
const int TOSTRING_DEBUG_COMPLETE = 1;
const int TOSTRING_DEBUG_ONLY_MODIFIED = 2;
const int TOSTRING_DEBUG_COMPLETE_WITH_RELATIONS = 3;
const int TOSTRING_DESC = 4;
const int TOSTRING_CODE_AND_DESC = 5;
const int TOSTRING_CODE_AND_DESC_WITH_TABLENAME = 6;
const int TOSTRING_FUGIT = 10;
const int TOSTRING_CSV = 11;
const int TOSTRING_SQL_INSERT = 12;
const int TOSTRING_SQL_UPDATE = 13;
const int TOSTRING_USER = 100;

class dbRecord
{
protected:
    /** Constructors */
    dbRecord(dbConnection *conn, const dbTableDefinition *tbldef, dbRecordID recid=0, dbUser *user=0);
    dbRecord(dbConnection *conn, dbTableDefinition *tbldef, dbRecordID recid=0, dbUser *user=0);
    /** Virtual constructors */
    virtual dbRecord *create(dbConnection *conn, const dbTableDefinition *tbldef, dbRecordID recid=0, dbUser *user =0);

public:
    dbRecord *duplicate();
    virtual ~dbRecord();
    virtual void init() {}

    bool copyRecord(dbRecord *other, bool deep = true, const Xtring &inc_fields = Xtring::null,
                    const Xtring &noinc_fields = Xtring::null);
    dbConnection *getConnection() const {
        return pConn;
    }
    bool isRead() const {
        return mIsRead;
    }
    bool isDeleted() const {
        return mIsDeleted;
    }
    /** returns whether the record is empty */
    virtual bool isEmpty( const Xtring &nocheck_fields = Xtring::null) const;
    /** @return whether this record is new (not in the database) */
    bool isNew() const {
        return mIsNew;
    }
    /** @return whether any field of this record has changed */
    bool isModified() const;

    virtual bool read(const Xtring &where);
    bool readWithFilter(const Xtring &where);
    bool read(dbRecordID recid);
    /** Read the related records */
    bool readRelated( bool force );
    /** Saves this record. If it is new, executes an INSERT else an UPDATE */
    virtual bool save( bool saverelated );
    /** Save the related records */
    bool saveRelated( bool updating );
    /** Removes the current record. Executes a DELETE */
    virtual bool remove();
    /** Remove the related records */
    bool removeRelated( bool updating );

    /** Gets the value of a field. If the field doesn't exists, returns a Variant() */
    dbFieldValue getValue(unsigned int nfield) const;
    bool isNullValue(unsigned int nfield) const;
    dbFieldValue getValue(const Xtring &fldname) const;
    bool isNullValue(const Xtring &fldname) const;
    virtual dbFieldValue calcValue(const Xtring &fldname) const;

    /** Gets the original value of a field. If the field doesn't exists, returns a Variant() */
    dbFieldValue getOrigValue(unsigned int nfield) const;
    bool isNullOrigValue(unsigned int nfield) const;
    dbFieldValue getOrigValue(const Xtring &fldname) const;
    bool isNullOrigValue(const Xtring &fldname) const;

    /** Sets the value of a field by position. If the field doesn't exists, returns false */
    bool setValue(unsigned int nfield, const Variant &value);
    /** Sets the value of a field by name. If the field doesn't exists, returns false */
    bool setValue(const Xtring &fldname, const Variant &value);
    bool setValuesFromRow( dbResultSet *rs, dbResultSet::size_type row );

    /** Sets the value of a field to null by position. If the field doesn't exists, returns false */
    bool setNullValue(unsigned int nfield);
    /** Sets the value of a field to null by name. If the field doesn't exists, returns false */
    bool setNullValue(const Xtring &fldname);

    /** Controls the existence of the record in the database
    	@param isnew 	true- it has been newly created in memory or just deleted
    			false- it has been read from the database
    */
    void setNew(bool isnew = true);
    /** Clear the values of the fields but setModified(false) */
    virtual void clear( bool setcustomvalues );

    /// \todo {add} look for primary keys in the table definition
    /** @return the ID of the record. This is usually the first field in the table definition, of type INTEGER */
    dbRecordID getRecordID() const;
    /** Sets de ID of the record */
    void setRecordID(dbRecordID recid);
    Xtring getIDSqlCond( dbRecordID recid = 0) const;
    Xtring getIDSqlCond( List<dbRecordID> recids ) const;

    /** @return the user of this record (who created it in memory) */
    dbUser *getUser() const {
        return pUser;
    }
    /** Sets the user of this record (who created it) */
    void setUser(dbUser *puser) {
        pUser = puser;
    }
    enum SeekCodeFlags { SeekCodeNone = 0x00,  InsertIfNotFound = 0x01,
                         SeekCodeInDesc = 0x02, SeekCodeMultiple = 0x04, DontShowBrowse = 0x08,
                         AllowNotFound = 0x10, SeekCodeReadonly = 0x20, AskIfFoundOne = 0x40,
						 SeekSecondaryCodes = 0x80
                       };
    dbRecordID seekCode( int &nvalues, const Xtring &fldcod, const Variant &foundcod,
                         const Xtring &flddesc = Xtring::null, const Variant &founddesc = Variant(),
                         const Xtring &cond = Xtring::null, SeekCodeFlags flags = SeekCodeNone,
                         Xtring &matchingcond = Xtring::null );

    /** Helper function to see if a given record exists with field=strvalue */
    dbRecordID existsAnother(const Xtring &cond) const;
    dbRecordID existsAnother(const Xtring &field, const Variant &value) const {
        return existsAnother(field + "=" + pConn->toSQL(value));
    }
    virtual bool findMatchingRecord(dbRecordID *matchingid) const;
    int selectNextInt(const Xtring &fldname, const Xtring &conds = Xtring::null,
                      bool usegaps = true) const;

    /** Adds a relation to the record */
    int addRelation(dbRecordRelation *rel);
    /** Removes a static relation from the record */
    int removeRelation(unsigned int index);
    void removeRelations();
    /** Gets the index of the relation to a given table */
    int getRelationIndex(const Xtring &relname) const;
    /** Gets a relation for two tables  */
    dbRecordRelation *getRelation(unsigned int index) const;
    dbRecordRelation *findRelation(const Xtring &relname) const;
    dbRecordRelation *findRelationByRelatedTable(const Xtring &tablename) const;
    dbRecordRelationDict findRelationsBySemanticProperties(const XtringList &properties) const;
    /** Gets a pointer to a related record */
    dbRecord *getRelatedRecord(unsigned int index) const;
    dbRecord *findRelatedRecord(const Xtring &relname) const;
    /** Gets the list of relations */
    const dbRecordRelationDict &getRelationsList() const {
        return mRecordRelations;
    }
    dbRecordRelationDict &getRelationsList() {
        return mRecordRelations;
    }
    /** Returns whether the record has enabled relations */
    bool hasEnabledRelations() const;
    /** Adds a detail record to a one2many relation */
    bool addRelatedDetailRecord(const Xtring &reltable, const dbRecord *relrecord);
    /** Gets the complete FROM clause, including relations */
    Xtring getJoinedFrom(bool inner=true) const;

    /** Gets the filter to apply in browse, choose and edit */
    Xtring getFilter(const Xtring &where, const Xtring &addTo = Xtring::null,
                     const Xtring &condsSep= "AND") const;
    /** Sets the filter to apply in browse, choose and edit */
    void addFilter(const Xtring &filter);
    void addStructuralFilter(const Xtring &filter);
    bool isStructuralFilter(const Xtring &filter);
    bool removeFilter(const Xtring &filter = Xtring::null);
    XtringList &getFilters() {
        return mFilters;
    }

    /** Creates a string representation of the record */
    virtual Xtring toString(int format = TOSTRING_DEBUG,
                            const Xtring &includedFields = Xtring::null) const;
    /** Fills in the values of the fields from a string */
    virtual bool fromString(const Xtring &source, int format,
                            const Xtring &includedFields = Xtring::null);

    /** Validate the record */
    virtual bool validate(ValidResult::Context context = ValidResult::saving );
	const ValidResult &getErrors() const { return mErrors; }

    /** Meta-information about the table, based upon the Data Dictionary */
    const dbTableDefinition* getTableDefinition() const {
        return pTableDef;
    }
    const dbFieldDefinition *getFieldDefinition(unsigned int nfield) const {
        return pTableDef->getFieldDefinition(nfield);
    }
    const dbFieldDefinition *findFieldDefinition(const Xtring &fldname) const {
        return pTableDef->findFieldDefinition(fldname);
    }
// 	int getFieldPos(const Xtring &fldname) const;
    /** @return the name of the database table */
    const Xtring &getTableName() const {
        return pTableDef->getName();
    }
    /** @return the number of fields in the table definition */
    unsigned int getFieldCount() const {
        return pTableDef->getFieldCount();
    }

    const XtringList &getSemanticProperties() const {
        return mSemanticProperties;
    }
    void addSemanticProperty( const Xtring &property ) {
        mSemanticProperties << property;
    }
    bool hasSemanticProperty( const Xtring &property ) {
        return mSemanticProperties.contains( property );
    }

protected:
    /** Executes a SELECT statement to read the record from the database */
    bool SELECT(const Xtring &/*where*/);
    bool SELECT() {
        return SELECT(getRecordID());
    }
    bool SELECT(dbRecordID recid) {
        return SELECT(Xtring("id=") + pConn->toSQL(recid) );
    }
    /** Executes an INSERT statement to insert the record to the database */
    bool INSERT();
    /** Executes an UPDATE statement to update the record in the database */
    bool UPDATE();
    /** Executes a DELETE statement to delete the record from the database */
    bool DELETE();
    /** Sets the changed flag of all the fields of the record */
    void setModified(bool changed);
    /** Chance to update a related record before saving or deleting the master */
    virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving) {}
    void beforeSaveRelated(dbRecord *master) {
        beforeSaveOrDeleteRelated( master, true );
    }
    void beforeDeleteRelated(dbRecord *master) {
        beforeSaveOrDeleteRelated( master, false);
    }
    void setRelatedID( int nfield, const Variant &id );

protected:
    const dbTableDefinition *pTableDef;
    dbUser *pUser;

private:
    void init_record();
    void clearRelations();
    dbConnection *pConn;
    struct {
        unsigned short int mIsNew:1;
        unsigned short int mIsRead;
        unsigned short int mIsDeleted;
    };
    XtringList mFilters;
    dbRecordRelationDict mRecordRelations;
    /* This dictionary is indexed only by the name of the fields, without the table name */
    Dictionary<dbFieldValue> mFieldValues, mOrigFieldValues;
    XtringList mSemanticProperties;
	ValidResult mErrors;

    /* Avoid copy. Use copyRecord instead */
    dbRecord(const dbRecord &other);
    dbRecord &operator=(const dbRecord &other);
};

}; // namespace gong

#endif // GONGDBRECORD_H
