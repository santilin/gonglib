#include <gongfileutils.h>
#include <gongdbtabledefinition.h>
#include <gongdbrecord.h>
#include <dbappdbapplication.h>
#include "sinpapelesmodule.h"
#include "sinpapelesrecordbehavior.h"

namespace gong {
namespace sinpapeles {

bool SinPapelesRecordBehavior::addTo(dbTableDefinition* tbldef)
{
    if( !tbldef->findFieldDefinition( "SINPAPELES_FILENAME" ) )
        tbldef->addFieldString("SINPAPELES_FILENAME", 100)->setCanBeNull( true )->setVisible( true );
    return true;
}

bool SinPapelesRecordBehavior::save(dbRecord* therecord, bool is_pre, const dbRecord* old_record, bool& result)
{
    if( !is_pre ) {
        Xtring fldvalue_fname = therecord->getValue("SINPAPELES_FILENAME").toString();
        // TODO: escribir "borrar" en filename para borrarlo
        if( !fldvalue_fname.isEmpty() ) {
            Xtring shouldbe_value = therecord->getTableName()
                                    + "_" + Xtring::number(therecord->getRecordID()).padL(8,'0')
                                    + "." + FileUtils::extension(fldvalue_fname);
            if( fldvalue_fname != shouldbe_value ) {
                if( copyToAlmacen( fldvalue_fname, shouldbe_value ) ) {
                    therecord->setValue( "SINPAPELES_FILENAME", shouldbe_value );
                    therecord->save(false);
                } else {
                    throw std::runtime_error("Imposible copiar fichero al almacén");
                }
            }
        }
    }
    return true;
}

bool SinPapelesRecordBehavior::copyToAlmacen(const Xtring& source, const Xtring& dest)
{
    _GONG_DEBUG_PRINT(0, "Copying '" + source + "' to '" + dest + "'" );
    Xtring sp_path = ModuleInstance->getModuleSetting("PATH").toString();
    if( sp_path.isEmpty() )
        sp_path = "file://" + sinpapeles::ModuleInstance->getModuleLocalDataDir() + "almacen";
    Xtring protocol, path;
    sp_path.splitIn2( protocol, path, "://" );
    FileUtils::addSeparator(path);
    if( protocol == "file" ) {
        _GONG_DEBUG_PRINT(0, path );
        _GONG_DEBUG_PRINT(0, FileUtils::path(source) );
        if( FileUtils::isDir( path.c_str() ) == false )
            FileUtils::makePath( path );
        if( path == FileUtils::path(source) || (path + "/") == FileUtils::path(source) ) {
            return FileUtils::linkHard( source, path + "/" + dest );
        } else
            return FileUtils::copyFile( source, path + "/" + dest );
    } else if( !protocol.isEmpty() ) {
        _GONG_DEBUG_WARNING( "Unknown protocol " + protocol );
        return false;
    }
    return true;
}


} // namespace sinpapeles
} // namespace gong


