#ifndef _GONG_FILEUTILS_H
#define _GONG_FILEUTILS_H

/** @file gongfileutils.h File system utilities
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <sys/stat.h>
#include "gongxtring.h"

namespace gong {

class FileUtils
{
public:
    FileUtils(const Xtring &afilename): mFileName(afilename) {
        mStat.st_dev = 0;
    }
    bool exists(bool reread=false) {
        return read(reread);
    }
    unsigned long size(bool reread=false);
    static bool exists(const char *filename);
    static bool isDir(const char *filename);
    static Xtring basename(const Xtring &filespec);
    static Xtring filename(const Xtring &filespec);
    static Xtring extension(const Xtring &filespec);
    static Xtring path(const Xtring &filespec);
    static bool isSeparator(int c) {
        return (c=='/');
    }
    static void addSeparator(Xtring &filespec);
    static void removeLastSeparator(Xtring &filespec);
    static XtringList getDirEntries(const Xtring &filespec = ".");
    static Xtring findInPath(const Xtring &path, const Xtring &file, const Xtring &subpath = Xtring::null);
    static int execProcess(const Xtring &command,
                           Xtring &messages = Xtring::null, Xtring &errors = Xtring::null);
    static Xtring getcwd();
    static Xtring readFile(const Xtring &filename);
    static bool writeFile(const Xtring &filename, const Xtring &text);
    static int copyFile(const Xtring &source, const Xtring &dest);
    static int linkHard(const Xtring &source, const Xtring &dest);
    static int linkSoft(const Xtring &source, const Xtring &dest);
    static int makePath(const Xtring &path );
    static void removeExtension(Xtring &fullname);
 	static bool chmod(const Xtring &filename, mode_t mode);
 	static bool chmod(const Xtring &filename, const Xtring &mode);
	static Xtring canonicalPath(const Xtring &path, const Xtring &cwd = Xtring::null);
	static bool samePath(const Xtring &p1, const Xtring &p2);

private:
    bool read(bool reread=false);
    Xtring mFileName;
    struct stat mStat;
};

} // namespace gong

#endif // _GONG_FILEUTILS_H
