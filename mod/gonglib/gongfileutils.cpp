
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/sendfile.h>  // sendfile
#include <unistd.h>
#include <fcntl.h>         // open
#include <dirent.h>
#include <cerrno>
#include <fstream>

#include "gongdebug.h"
#include "gonggettext.h"
#include "gongfileutils.h"
#include "gonglibrary.h"

namespace gong {

// TODO: isReadable(), isWritable()

bool FileUtils::read(bool reread)
{
    if( !reread && mStat.st_dev == 0 )
        return true;
    int ret = stat(mFileName.c_str(), &mStat );
    if( ret && (errno==EFAULT || errno==ENOMEM) ) {
        _GONG_DEBUG_PERROR(mFileName );
        mStat.st_dev = 0;
        return false;
    } else
        return true;
}


unsigned long FileUtils::size(bool reread)
{
    if( read(reread) )
        return mStat.st_size;
    else
        return 0;
}


bool FileUtils::exists(const char *filename)
{
    struct stat buff;
    int ret = stat(filename, &buff);
    if( ret && (errno==EFAULT || errno==ENOMEM) )
        _GONG_DEBUG_PERROR(filename);
    return !ret;
}

bool FileUtils::isDir(const char *filename)
{
    struct stat buff;
    int ret = stat(filename, &buff);
    if( ret && (errno==EFAULT || errno==ENOMEM) ) {
        _GONG_DEBUG_WARNING( Xtring("Error stating file: ") + filename );
        exit(ret);
    }
    return (buff.st_mode & S_IFMT) == S_IFDIR;
}

/* Copied from stlplus.sourceforge.net */
Xtring FileUtils::basename (const Xtring &spec)
{
    Xtring fname = filename(spec);
    // scan back through filename until a '.' is found and remove suffix
    // the whole filename is the basename if there is no '.'
    Xtring::size_type i = fname.find_last_of('.');
    // observe Unix convention that a dot at the start of a filename is part of the basename, not the extension
    if (i != 0 && i != Xtring::npos)
        fname.erase(i, fname.size()-i);
    return fname;
}

Xtring FileUtils::filename(const Xtring &spec)
{
    // scan back through filename until a preferred_separator is found and remove prefix;
    // if there is no preferred_separator then remove nothing, i.e. the whole filespec is filename
    unsigned i = spec.size();
    while (i--)
    {
        if (isSeparator(spec[i]))
            return spec.substr(i+1,spec.size()-i-1);
    }
    return spec;
}

Xtring FileUtils::extension (const Xtring &spec)
{
    Xtring fname = filename(spec);
    // scan back through filename until a '.' is found and remove prefix;
    Xtring::size_type i = fname.find_last_of('.');
    // observe Unix convention that a dot at the start of a filename is part of the name, not the extension;
    if (i != 0 && i != Xtring::npos)
        fname.erase(0, i+1);
    else
        fname.erase();
    return fname;
}

Xtring FileUtils::path (const Xtring &spec)
{
    // scan back through filename until a separator is found and remove prefix
    // if there is no separator, remove the whole
    unsigned i = spec.size()-1;
	if( isSeparator(spec[i]) )
		--i;
    while (i--)
    {
        if (isSeparator(spec[i]))
            return spec.substr(0,i+1);
    }
    return Xtring::null;
}

XtringList FileUtils::getDirEntries(const Xtring &filespec)
{
    struct dirent **namelist;
    XtringList entries;
    int n = scandir(filespec.c_str(), &namelist, 0, alphasort);
    if (n < 0)
    {
        _GONG_DEBUG_PERROR("scandir: " + filespec);
    }
    else
    {
        for(int i=0; i<n; i++)
        {
            entries << namelist[i]->d_name;
            free(namelist[i]);
        }
        free(namelist);
    }
    return entries;
}

/**
 * Looks for a file
 * in every path or path/subpath in the colon-delimited parameter path
 * @param path
 * @param file
 * @param subpath
 * @return
 */
Xtring FileUtils::findInPath(const Xtring &path, const Xtring &file, const Xtring &subpath)
{
    if( file.startsWith("/") && FileUtils::exists(file.c_str()) )
        return file;
    Xtring foundfile = file;
    // Look for file in PATH
    XtringList paths;
    Xtring spath = subpath;
    if( !spath.isEmpty() && !spath.endsWith( "/" ) )
        spath += "/";
    if( path.tokenize(paths, ":") != 0 ) {
        for( XtringList::iterator it = paths.begin(); it!=paths.end(); ++it ) {
            Xtring onepath = *it;
            if( it->isEmpty() )
                continue;
            if( !onepath.endsWith( "/" ) )
                onepath += "/";
            foundfile = onepath + file;
            if( FileUtils::exists(foundfile.c_str()) )
                return foundfile;
            if( !spath.isEmpty() ) {
                foundfile = onepath + spath + file;
                if( FileUtils::exists(foundfile.c_str()) )
                    return foundfile;
            }
        }
    }
    _GONG_DEBUG_WARNING( Xtring::printf( _("'%s' not found in path: %s"),
                                         file.c_str(), path.c_str() ) );
    return file;
}

/// \todo {bug5} popen reads the standard output, but should read the standard error
// http://jineshkj.wordpress.com/2006/12/22/how-to-capture-stdin-stdout-and-stderr-of-child-program/
int FileUtils::execProcess(const Xtring& _command, Xtring& messages, Xtring& errors)
{
    Xtring command = _command;
    Xtring tmpfilename = Xtring("/tmp/") + GongLibraryInstance->getPackageName() + ".err";
    int ret = 0; // Normal exit
    if( &errors != &Xtring::null )
        command += Xtring(" 2>") + tmpfilename;
    if( &messages != &Xtring::null ) {
        FILE *f = NULL;
        if( (f = popen(command.c_str(), "r")) == NULL) {
            ret = -1;
        } else {
            if( errno != 0 )
                ret = errno;
            int c = 0;
            char buffer[1024] = { '\0' };
            while( (c = fgetc(f)) != EOF)
            {
                ungetc(c, f);
                if( fgets(buffer, sizeof(buffer) - 1, f) ) {
                    messages += buffer;
                }
            }
        }
        if( !ret )
            ret = pclose(f);
        else
            pclose(f);
    } else {
        ret = system( command.c_str() );
    }
    if( &errors != &Xtring::null ) {
        errors = FileUtils::readFile( tmpfilename );
        unlink( tmpfilename.c_str() );
        if( !errors.isEmpty() )
            _GONG_DEBUG_WARNING( errors );
    }
    return ret;
}

Xtring FileUtils::getcwd()
{
    char tempcwd[2048];
    if( ::getcwd(tempcwd, sizeof(tempcwd)) )
        return Xtring(tempcwd);
    else
        return Xtring::null;
}

Xtring FileUtils::readFile(const Xtring &filename)
{
    std::ifstream f;
    f.open( filename.c_str(), std::ios_base::binary | std::ios_base::in );
    if ( !f.is_open() || !f.good() ) {
        _GONG_DEBUG_WARNING( Xtring::printf("%s: %s", strerror( errno ), filename.c_str()) );
        return Xtring::null;
    }
    f.seekg( 0, std::ios_base::end );
    long end_pos = f.tellg();
    f.seekg( 0, std::ios_base::beg );
    char *buffer = new char[ end_pos + 1 ];
    f.read( buffer, end_pos );
    Xtring ret = Xtring( buffer, end_pos );
    delete [] buffer;
    return ret;
}

bool FileUtils::writeFile(const Xtring& filename, const Xtring& text)
{
    bool ret = false;

    std::ofstream pFile(filename.c_str());
    if( !pFile ) {
        perror(filename.c_str());
    } else {
        pFile.write(text.c_str(), text.length());
        if( pFile.bad()  ) {
            _GONG_DEBUG_WARNING( Xtring::printf("%s: %s", strerror( errno ), filename.c_str()) );
        } else
            ret = true;
        pFile.close();
    }
    return ret;
}


int FileUtils::copyFile(const Xtring& fnsource, const Xtring& fndest)
{
    int source = open( fnsource.c_str(), O_RDONLY, 0);
    int dest = open( fndest.c_str(), O_WRONLY | O_CREAT /*| O_TRUNC*/, 0644);

    // struct required, rationale: function stat() exists also
    struct stat stat_source;
    fstat(source, &stat_source);

    ssize_t ret = sendfile(dest, source, 0, stat_source.st_size);

    close(source);
    close(dest);
    return (ret == stat_source.st_size);
}

int FileUtils::linkHard(const Xtring& source, const Xtring& dest)
{
    int ret = link( source.c_str(), dest.c_str() );
    if( ret ) {
        _GONG_DEBUG_PERROR(dest);
    }
    return ret;
}

int FileUtils::linkSoft(const Xtring& source, const Xtring& dest)
{
    int ret = symlink( source.c_str(), dest.c_str() );
    if( ret ) {
        _GONG_DEBUG_PERROR(dest);
    }
    return ret;
}

void FileUtils::addSeparator(Xtring& filespec)
{
    unsigned i = filespec.size();
    if( i == 0 || !isSeparator(filespec[i-1]) )
        filespec += "/";
}

int FileUtils::makePath(const Xtring& path)
{
    int ret = system( ("mkdir -p " + path).c_str() );
    if( ret == -1 )
        _GONG_DEBUG_WARNING( strerror(errno) );
    return ret;
}


} // namespace gong


