#include <fstream>
#include "gongdbuser.h"

namespace gong {

dbUser::~dbUser()
{
    if( pLogFile )
        delete pLogFile;
}


int dbUser::doLog(const Xtring &message, int rowsaffected )
{
    switch( mLogType )
    {
    case none:
        return true;
    case file:
        if( !pLogFile )
        {
            pLogFile = new std::ofstream(mLogFilename.c_str(), std::ios::app);
            if( !pLogFile )
            {
                delete pLogFile;
                pLogFile = 0;
            }
        }
        if( pLogFile )
        {
            Xtring text = DateTime().toString() + ":user(" + Xtring::number(getUserCode()) + "):" + message ;
            if( rowsaffected != -1 )
                text += ":rowsaffected(" + Xtring::number(rowsaffected) + ")";
            pLogFile->write(text.c_str(), text.length());
            pLogFile->flush();
            return 1;
        }
        break;
    case sql:
        if( mLogDatabase && pConnection )
        {
            pConnection->exec("INSERT INTO " + mLogFilename +
                              "(user, time, message, rowsaffected) VALUES ("
                              + pConnection->toSQL(getUserCode())
                              + "," + pConnection->toSQL(DateTime())
                              + "," + pConnection->toSQL(message)
                              + "," + pConnection->toSQL(rowsaffected) + ")");
            return 1;
        }
    }
    return 0;
}

}
