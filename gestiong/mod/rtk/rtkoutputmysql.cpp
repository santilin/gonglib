#include "rtkobject.h"
#include "rtksection.h"
#include "rtkreport.h"
#include "rtkoutputmysql.h"


#ifdef HAVE_MYSQL

namespace gong {}
namespace RTK = gong;

namespace gong {

OutputMySql::~OutputMySql()
{
    ::mysql_close( &mMySql );
}


int OutputMySql::startReport()
{
    if( ::mysql_init(&mMySql) ) {
        ::mysql_options(&mMySql, MYSQL_OPT_COMPRESS, NULL);
        ::mysql_real_connect(&mMySql, host(), user(), password(),
                             database(), port(), 0, 0);
    }
    if( ::mysql_errno(&mMySql) ) {
        mReport.addError(Error::SqlConnect, "OutputMySql", 0, ::mysql_error(&mMySql));
        return 1;
    }
    // Create tables if they not exists
    for( uint ns = 0; ns<mReport.sectionsCount(); ns++ ) {
        Section *s = mReport.getSection(ns);
        Xtring sql, sname = s->name();
        if( createTables() ) {
            sql = "DROP TABLE IF EXISTS " + sname;
            if( ::mysql_real_query(&mMySql, sql.c_str() , sql.size() ) ) {
                mReport.addError(Error::SqlQuery, "OutputMysql:", 0, ::mysql_error(&mMySql), sql.c_str());
                return 1;
            }
        }
        sql = "CREATE TABLE " + sname + "(";
        for( uint no = 0; no < s->objectsCount(); no++ ) {
            Object *o = s->getObject(no);
            if( no != 0 )
                sql += ",";
            sql += o->name();
            sql += " VARCHAR(255)";
        }
        sql += ")";
        if( ::mysql_real_query(&mMySql, sql.c_str() , sql.size() ) ) {
            mReport.addError(Error::SqlQuery, "OutputMysql:", 0, ::mysql_error(&mMySql), sql.c_str());
            return 1;
        }
    }
    return 0;
}

Measure OutputMySql::endSection(const Section &section)
{
    Xtring sql = Xtring("INSERT INTO ") + section.name() + " VALUES(";
    for( uint no = 0; no < section.objectsCount(); no++ ) {
        Object *o = section.getObject(no);
        if( no != 0 )
            sql += ",";
        sql += '\'' + o->formattedText() + '\'';
    }
    sql += ")";
    if( ::mysql_real_query(&mMySql, sql.c_str() , sql.size() )  ) {
        mReport.addError(Error::SqlQuery, "OutputMysql:", 0, ::mysql_error(&mMySql), sql.c_str());
        return false;
    }
    return 0.0;
}

} // namespace RTK

#endif // RTK_MYSQL

