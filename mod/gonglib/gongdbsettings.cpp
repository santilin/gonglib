#include "gongdbresultset.h"
#include "gongdbsettings.h"

namespace gong {

bool dbSettings::read(const Xtring &scope)
{
    Xtring select = "SELECT * FROM " + mTableName;
    if( !scope.isEmpty() )
        select += " WHERE " + scope;
    dbResultSet *rs = pConnection->select( select );
    while( rs->next() ) {
        // Set the correct value from the type field
        Variant value( Variant::nameToType( rs->toString(3).c_str() ) );
        if( value.isValid() )
            value.copy( rs->toString(1) );
        else
            value = rs->toString(1);
        _GONG_DEBUG_PRINT(5, "Name: " + rs->toString(0) + ", value: " + value.toString() );
        Settings::setValue( rs->toString(0), value );
    }
    return true;
}

bool dbSettings::write(const Xtring &filter)
{
#if 0
    for( unsigned int nv=0; nv<mSettingsDict.size(); nv++ ) {
        Variant v(mSettingsDict[nv]);
        Xtring s_v = v.toString();
        s_v = s_v.replace("\\", "\\\\").replace("\"", "\\\"");
        Xtring sql = "UPDATE " + pConnection->nameToSQL( mTableName )
                     + " SET " + pConnection->nameToSQL( mFldValueName )
                     + " = " + pConnection->toSQL( s_v )
                     + " WHERE " + pConnection->nameToSQL( mFldKeyName )
                     + " = " + pConnection->toSQL( mSettingsDict.getKey(nv) )
                     + " AND " + pConnection->nameToSQL( mFldUserName )
                     + " = " + pConnection->toSQL( mUserID );
        pConnection->exec( sql );
        if( pConnection->getAffectedRows() == 0 ) {
            sql = "INSERT INTO " + pConnection->nameToSQL( mTableName )
                  + "(" + pConnection->nameToSQL( mFldKeyName )
                  + "," + pConnection->nameToSQL( mFldValueName )
                  + "," + pConnection->nameToSQL( mFldTypeName )
                  + "," + pConnection->nameToSQL( mFldUserName )
                  + ")VALUES(" + pConnection->toSQL( mSettingsDict.getKey(nv) )
                  + "," + pConnection->toSQL( s_v )
                  + "," + pConnection->toSQL( Variant::typeToName(v.type()) )
                  + "," + pConnection->toSQL( mUserID )
                  + ")";
            pConnection->exec( sql );
            if( pConnection->getAffectedRows() == 0 ) {
                _GONG_DEBUG_WARNING( "Could not save setting: " + mSettingsDict.getKey(nv) + "=" + s_v );
                return false;
            }
        }
        _GONG_DEBUG_PRINT(0, "Saved setting: " + mSettingsDict.getKey(nv) + "=" + s_v );
    }
#endif
    return true;
}

void dbSettings::removeSetting(const Xtring& settingname)
{
    Xtring sql = "DELETE FROM " + pConnection->nameToSQL( mTableName )
                 + " WHERE " + pConnection->nameToSQL( mFldKeyName )
                 + " = " + pConnection->toSQL( settingname );
// 		+ " AND " + pConnection->nameToSQL( mFldUserName )
// 		+ " = " + pConnection->toSQL( mUserID );
}


void dbSettings::setValue(const Xtring& settingname, const Variant& val)
{
    gong::Settings::setValue(settingname, val);
    Xtring sql = "UPDATE " + pConnection->nameToSQL( mTableName )
                 + " SET " + pConnection->nameToSQL( mFldValueName )
                 + " = " + pConnection->toSQL( val )
                 + " WHERE " + pConnection->nameToSQL( mFldKeyName )
                 + " = " + pConnection->toSQL( settingname );
// 		+ " AND " + pConnection->nameToSQL( mFldUserName )
// 		+ " = " + pConnection->toSQL( mUserID );
    pConnection->exec( sql );
    if( pConnection->getAffectedRows() == 0 ) {
        sql = "INSERT INTO " + pConnection->nameToSQL( mTableName )
              + "(" + pConnection->nameToSQL( mFldKeyName )
              + "," + pConnection->nameToSQL( mFldValueName )
              + "," + pConnection->nameToSQL( mFldTypeName )
              + "," + pConnection->nameToSQL( mFldUserName )
              + ")VALUES(" + pConnection->toSQL( settingname )
              + "," + pConnection->toSQL( val )
              + "," + pConnection->toSQL( Variant::typeToName(val.type()) )
              + "," + pConnection->toSQL( mUserID )
              + ")";
        pConnection->exec( sql );
        if( pConnection->getAffectedRows() == 0 ) {
            _GONG_DEBUG_WARNING( "Could not save setting: " + settingname + "=" + val.toString() );
        }
    }
    _GONG_DEBUG_PRINT(3, "Saved setting: " + settingname + "=" + val.toString() );
}


} // namespace gong

