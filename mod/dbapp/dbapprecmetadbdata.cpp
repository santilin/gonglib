/*<<<<<COPYLEFT*/
/** @file dbapprecmetadbdata.cpp Record of meta data for the database
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
// COPYLEFT Record of meta data for the database
// TYPE dbRecord MetaDBData
/*>>>>>MODULE_INFO*/

/*<<<<<METADBDATA_INCLUDES*/
#include "dbapprecmetadbdata.h"
/*>>>>>METADBDATA_INCLUDES*/

namespace gong {

bool RecMetaDBData::lock(const Xtring &user, const Xtring &password, const Xtring &desc, Date date)
{
    setValue( "LOCKED", true );
    setValue( "LOCKED_PASSWORD", password );
    setValue( "LOCKED_USER", user );
    setValue( "LOCKED_DESC", desc );
    setValue( "LOCKED_DATE", date );
    return save( false );
}

bool RecMetaDBData::unlock(const Xtring &password)
{
    if( password == getValue( "LOCKED_PASSWORD" ).toString() ) {
        setValue( "LOCKED", false);
        setValue( "LOCKED_PASSWORD", Xtring::null );
        setValue( "LOCKED_USER", Xtring::null  );
        setValue( "LOCKED_DESC", Xtring::null  );
        setNullValue( "LOCKED_DATE" );
        return save( false );
    }
    return false;
}

} // namespace gong
