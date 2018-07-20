#include <gongdebug.h>
#include <gonglist.h>
#include <fstream>
#include "capelmodule.h"

#define DEBUG_CAPELMODULE 3


namespace capel {

CapelModule::CapelModule(const Xtring &lfilename, const Xtring &lbegin_keyword,
						 const Xtring &lend_keyword,
						 const Xtring &lbegin_comment, const Xtring &lend_comment)
{
    init(lfilename, lbegin_keyword, lend_keyword, lbegin_comment, lend_comment);
}

void CapelModule::init(const Xtring &lfilename, const Xtring &lbegin_keyword,
					   const Xtring &lend_keyword,
					   const Xtring &lbegin_comment, const Xtring &lend_comment)
{
    mBeginKeyword = lbegin_keyword;
    mBeginComment = lbegin_comment;
    mEndKeyword = lend_keyword;
    mEndComment = lend_comment;
	mFilename = lfilename;
	std::ifstream pFile(mFilename.c_str());
    if( !pFile ) {
		_GONG_DEBUG_PRINT(1, strerror(errno) );
		mBuffer.clear();
	} else {
		_GONG_DEBUG_PRINT(3, Xtring("capel: opening ") + mFilename );
		pFile.seekg(0, std::ios::end);
		size_t filelen = pFile.tellg();
		pFile.seekg(0, std::ios::beg);
		char *buf = new char[filelen+1];
		pFile.read(buf, filelen);
        buf[filelen]='\0';
        mBuffer = Xtring(buf);
        delete buf;
		pFile.close();
	}
    mCurrentExtrusion="DEFAULT";
}

size_t CapelModule::write()
{
    size_t ret = 0;

	std::ofstream pFile(mFilename.c_str());
    if( !pFile ) {
		_GONG_DEBUG_PRINT(1, strerror(errno) );
	} else {
		_GONG_DEBUG_PRINT(2, Xtring::printf( "capel: %s,  %d bytes written",
			mFilename.c_str(), mBuffer.length() ) );
        pFile.write(mBuffer.c_str(), mBuffer.length());
		if( pFile.bad()  ) {
			_GONG_DEBUG_WARNING(Xtring::printf( "%s: %s",
				mFilename.c_str(), strerror(errno)) );
			ret = 0;
		} else {
			ret = pFile.tellp();
		}
		pFile.close();
	}
    return ret;
}

size_t CapelModule::writeIfModified()
{
    size_t ret = 0;

	std::fstream pFile(mFilename.c_str());
	if( !pFile ) {
		// Maybe the file does not exists
		ret = write();
	} else {
		bool modified = true;
		_GONG_DEBUG_PRINT(3, "reopening " + mFilename );
		pFile.seekg(0, std::ios::end);
		size_t filelen = pFile.tellg();
		pFile.seekg(0, std::ios::beg);
		char *buf = new char[filelen+1];
		pFile.read(buf, filelen);
        buf[filelen]='\0';
		modified = (filelen != mBuffer.size()) || (mBuffer.compare(buf) != 0);
#if DEBUG_CAPELMODULE > 5
		fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>OLD>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		fprintf(stderr, buf);
		fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>NEW>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		fprintf(stderr, mBuffer.c_str());
#endif
        delete buf;
		pFile.close();
		if( modified ) {
			ret = write(); // Write reopens the file for writing
		} else {
			_GONG_DEBUG_PRINT(3, "capel: Not modified: " + mFilename );
			ret = 0;
		}
    }
    return ret;
}


Xtring CapelModule::fill_begin_extrusion(const Xtring &extrusion_name) const
{
    return mBeginComment + mBeginKeyword + extrusion_name;
}

Xtring CapelModule::fill_end_extrusion(const Xtring &extrusion_name) const
{
    return mBeginComment + mEndKeyword + extrusion_name;
}

Xtring CapelModule::fill_begin_extrusion_with_space(const Xtring &extrusion_name) const
{
    return mBeginComment + " " + mBeginKeyword + extrusion_name;
}

Xtring CapelModule::fill_end_extrusion_with_space(const Xtring &extrusion_name) const
{
    return mBeginComment + " " + mEndKeyword + extrusion_name;
}

/**
   Firstly, look up for the extrusion + end comment, and if not found
   look for the extrusion name only.
   This avoids substring errors in extrusion names like:
   {<<<<FORM_EDIT}
   and
   {<<<<FORM_EDITED}
*/
Xtring::size_type CapelModule::lookup_extrusion_begin(const Xtring &extrusion_name) const
{
	Xtring fullname = fill_begin_extrusion(extrusion_name);
	Xtring::size_type pos = mBuffer.find(fullname + mEndComment);
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + mParameterDelimiterBegin );
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + " ");
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + "\n");
	if( pos == Xtring::npos ) {
		fullname = fill_begin_extrusion_with_space(extrusion_name);
		pos = mBuffer.find(fullname + mEndComment);
	}
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + " " + mEndComment);
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + mParameterDelimiterBegin );
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + " ");
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + "\n");
	return pos;
}

Xtring::size_type CapelModule::lookup_extrusion_end(const Xtring &extrusion_name)
{
	Xtring fullname = fill_end_extrusion(extrusion_name);
	Xtring::size_type pos = mBuffer.find(fullname + mEndComment);
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + " ");
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + "\n");
	if( pos == Xtring::npos ) {
		fullname = fill_end_extrusion_with_space(extrusion_name);
		pos = mBuffer.find(fullname + mEndComment);
	}
	if( pos == Xtring::npos ) {
		pos = mBuffer.find(fullname + mEndComment);
	}
	if( pos == Xtring::npos ) {
		pos = mBuffer.find(fullname + " " + mEndComment);
	}
	if( pos == Xtring::npos ) { // EOF?
		pos = mBuffer.find(fullname);
		if( pos != Xtring::npos ) {
			if( pos + fullname.length() != mBuffer.length() )
				pos = Xtring::npos;
			else
				mBuffer += "\n";
		}
	}
	return pos;
}

Xtring::size_type CapelModule::lookup_extrusion_end(const Xtring &extrusion_name) const
{
	Xtring fullname = fill_end_extrusion(extrusion_name);
	Xtring::size_type pos = mBuffer.find(fullname + mEndComment);
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + " ");
	if( pos == Xtring::npos )
		pos = mBuffer.find(fullname + "\n");
	if( pos == Xtring::npos ) {
		fullname = fill_end_extrusion_with_space(extrusion_name);
		pos = mBuffer.find(fullname + mEndComment);
	}
	if( pos == Xtring::npos ) {
		pos = mBuffer.find(fullname + mEndComment);
	}
	if( pos == Xtring::npos ) {
		pos = mBuffer.find(fullname + " " + mEndComment);
	}
	if( pos == Xtring::npos ) { // EOF?
		pos = mBuffer.find(fullname);
		if( pos != Xtring::npos ) {
			if( pos + fullname.length() != mBuffer.length() )
				pos = Xtring::npos;
		}
	}
	return pos;
}


Xtring CapelModule::get_parameters(const Xtring &extrusion_name) const
{
	Xtring fullname = fill_begin_extrusion(extrusion_name);
	Xtring::size_type pos = mBuffer.find(fullname + mParameterDelimiterBegin );
	if( pos == Xtring::npos ) {
		fullname = fill_begin_extrusion_with_space(extrusion_name);
		pos = mBuffer.find(fullname + mParameterDelimiterBegin );
	}
	if( pos != Xtring::npos ) {
		pos += fullname.size() + 1;
		Xtring::size_type posend = mBuffer.find( mParameterDelimiterEnd, pos );
		if( posend != Xtring::npos ) {
			return mBuffer.mid( pos, posend-pos );
		}
	}
	return Xtring::null;
}


Xtring::size_type CapelModule::lookup_extrusion_code(const Xtring &extrusion_name) const
{
	int begin;
	if( (begin=lookup_extrusion_begin(extrusion_name)) == -1 )
		return -1;
	else {
		while( begin<(int)mBuffer.length() && mBuffer[begin] != '\n' )
			begin++;
	}
	return begin;
}

Xtring CapelModule::get_extrusion_text(const Xtring &extrusion_name)
{
	Xtring result;

	Xtring::size_type begin = lookup_extrusion_code(extrusion_name);
	if( begin != Xtring::npos ) {
		int end = lookup_extrusion_end(extrusion_name);
		if( end != Xtring::npos ) {
			result = mBuffer.substr(begin+1, end-begin-1);
		}
	}
	return result;
}

int CapelModule::remove_extrusion(const Xtring &extrusion_name, Xtring *parameters)
{
	int begin, end;
	begin = lookup_extrusion_begin(extrusion_name);
	end = lookup_extrusion_end(extrusion_name);
	if( (end==-1) && (begin==-1) )
		return -1;
	else if( (begin!=-1) && (end==-1) ) {
		/* If there is no end, assume end to be the line following begin */
		end = begin;
	} else if( (begin==-1) && (end!=-1) ) {
		/* If there is no begin, assume begin=end and end=the line following begin */
		begin = end;
	}
	if( end < begin ) {
		_GONG_DEBUG_WARNING( "Start of section " + extrusion_name + " is after end" );
	}
	if (parameters != 0) {
		*parameters = get_parameters( extrusion_name );
	}
	while( end<(int)mBuffer.length() && mBuffer[end] != '\n' )
		end++;
	if( end != (int)mBuffer.length() )
		end++;
	mBuffer.erase(begin, end-begin);
	return begin;
}

Xtring::size_type CapelModule::insert_text(int begin, const Xtring &text, bool addnewline)
{
    int textlen = text.length();

    if( addnewline && !text.endsWith('\n') ) {
        mBuffer.insert(begin, text + "\n");
        return begin + textlen + 1;
    } else {
        mBuffer.insert(begin, text);
        return begin + textlen;
    }
}

int CapelModule::insert_extrusion_at(int begin,
									 const Xtring &extrusion_name, const Xtring &text,
									 const Xtring &default_following_text,
									 const Xtring &default_preceding_text, bool addnewline)
{
    bool exists = true;
    int begextr;
    mCurrentExtrusion=extrusion_name;
	Xtring parameters;
    begextr = remove_extrusion(extrusion_name, &parameters);
    // If the extrusion exists, ignore begin
    if( begextr != -1 )
        begin = begextr;
    else
        exists = false;
	if( begin == -1 )
		begin = 0; // Insert at the begining
    if( begin == -2 ) {
      	/* The extrusion doesn't exist and begin not supplied */
      	begin = mBuffer.length();
		if( begin > 0 && mBuffer[begin - 1] != '\n' )
			mBuffer.insert(begin++, "\n");
      	exists = false;
    }
    if( !exists && default_preceding_text.size() ) {
    	begin = insert_text(begin, default_preceding_text);
    }
	if( parameters.isEmpty() ) {
		begin = insert_text(begin, fill_begin_extrusion(extrusion_name) + mEndComment );
		begin = insert_text(begin, text, addnewline);
	} else {
		begin = insert_text(begin, fill_begin_extrusion(extrusion_name)
			+ mParameterDelimiterBegin + parameters + mParameterDelimiterEnd + mEndComment );
		// Process parameters on text
		begin = insert_text(begin, process_parameters( text, parameters ) );
	}
    begin = insert_text(begin, fill_end_extrusion(extrusion_name) + mEndComment);
    if( !exists && default_following_text.size() ) {
    	begin = insert_text(begin, default_following_text);
    }
    return begin;
}


int CapelModule::append_text_to_extrusion(const Xtring &extrusion_name, const Xtring &text, bool addnewline)
{
	int begin;

	begin = lookup_extrusion_end(extrusion_name);
	if( begin == -1 ) {
		/* The extrusion doesn't exist */
		begin = insert_extrusion(extrusion_name, text, Xtring(), Xtring(), addnewline);
	} else {
		begin = insert_text(begin, text, addnewline);
		mCurrentExtrusion=extrusion_name;
	}
	return begin;
}

/* El parametro newline lo he tenido que anadir despues para que sucesivas inserciones con << no
   anadieran finales de linea. Por defecto para las otras funciones es true */
CapelModule &CapelModule::operator<<(const Xtring &text)
{
    append_text_to_extrusion(mCurrentExtrusion, text, /*addnewline=*/false);
    return *this;
}


CapelModule &CapelModule::operator<<(int val)
{
	append_text_to_extrusion(mCurrentExtrusion, Xtring::number(val), /*addnewline=*/false);
    return *this;
}

int CapelModule::clear_extrusion(const Xtring &extrusion_name)
{
	int begin, end;

	mCurrentExtrusion=extrusion_name;
	begin = lookup_extrusion_begin(extrusion_name);
	end = lookup_extrusion_end(extrusion_name);
	if( (end==-1) || (begin==-1) )
		return -1;
	while( begin<(int)mBuffer.length() && mBuffer[begin] != '\n' )
		begin++;
	begin++;
	mBuffer.erase(begin, end-begin);
	return begin;
}


int CapelModule::position_before_extrusion(const Xtring &extrusion_name)
{
	int begin = lookup_extrusion_begin(extrusion_name);
	if( begin != - 1 )
		begin--;
	return begin;
}

int CapelModule::position_after_extrusion(const Xtring &extrusion_name)
{
	size_t end = lookup_extrusion_end(extrusion_name);
	if( end != Xtring::npos ) {
		// Try to find two \n 
		size_t first_newline = Xtring::npos;
		while( end+1<mBuffer.length() ) {
			if ( mBuffer[end] == '\n' ) {
				if (first_newline == Xtring::npos) {
					first_newline = end;
				}
				if (mBuffer[end+1] == '\n') {
					break;
				}
			}
			end++;
		}
		if( end == mBuffer.length() ) {
			if( first_newline != Xtring::npos) {
				end = first_newline;
			}
		} else if (end+1 == mBuffer.length() ) {
			if( first_newline != Xtring::npos) {
				end = first_newline;
			}
		}
		++end;
	}
	return end;
}

int CapelModule::position_before_text(const Xtring &text)
{
	int begin=mBuffer.find(text);
	if( begin != - 1 )
		begin--;
	return begin;
}


int CapelModule::position_after_text(const Xtring &text)
{
	int begin;

	if( (begin=mBuffer.find(text)) == -1 )
		return -1;
	else {
		while( begin<(int)mBuffer.length() && mBuffer[begin] != '\n' )
			begin++;
		if( begin != (int)mBuffer.length() )
			begin++;
	}
	return begin;
}

Xtring &CapelModule::replace_global(const Xtring &search, const Xtring &repl)
{
	int searchlen = search.length();
	int repllen = repl.length();
	int match = mBuffer.find(search);
	while( match != -1 ) {
		mBuffer.erase(match, searchlen);
		mBuffer.insert(match, repl);
		match = mBuffer.find(search, match + repllen);
	}
	return mBuffer;
}

Xtring &CapelModule::replace_in_extrusion(const Xtring &extrusion_name,
										   const Xtring &search,
										   const Xtring &repl )
{
	int begin = lookup_extrusion_begin(extrusion_name);
	int end = lookup_extrusion_end(extrusion_name);
	if( begin == -1 || end == -1 )
		return mBuffer;
	int searchlen = search.length();
	int repllen = repl.length();
	int match = mBuffer.find(search, begin);
	while( match != -1 && match < end ) {
		mBuffer.erase(match, searchlen);
		mBuffer.insert(match, repl);
		match = mBuffer.find(search, match + repllen);
		end += repllen - searchlen;
	}
	return mBuffer;
}


Xtring CapelModule::process_parameters(const Xtring& text, const Xtring& parameters)
{
	if( parameters[0] == 's' ) { // replace
		char delimiter = parameters[1];
		gong::XtringList repl_strings;
		parameters.mid(2).tokenize( repl_strings, Xtring(1,delimiter) );
		return Xtring(text).replace( repl_strings[0], repl_strings[1] );
	}
	return text;
}


Xtring::size_type CapelModule::find_global(const Xtring &search) const
{
	return mBuffer.find(search);
}

static const char *upper_table[][2] =
{
	// Utf8
	{ "Á", "á" },
	{ "É", "é" },
	{ "Í", "í" },
	{ "Ó", "ó" },
	{ "Ú", "ú" },
	{ "Ñ", "ñ" },
	{ "Ç", "ç" },
	{ "Ä", "ä" },
	{ "Ë", "ë" },
	{ "Ï", "ï" },
	{ "Ö", "ö" },
	{ "Ü", "ü" },
	{ "À", "à" },
	{ "È", "è" },
	{ "Ì", "ì" },
	{ "Ò", "ò" },
	{ "Ù", "ù" },
	{ "Â", "â" },
	{ "Ê", "ê" },
	{ "Î", "î" },
	{ "Ô", "ô" },
	{ "Û", "û" },
	{ "Ŕ", "ŕ" }
};


Xtring CapelModule::capitalize( const Xtring &str )
{
	const char *pos = str.c_str();
	Xtring ret;
	// First letter, upper
	if( *pos ) {
		uint i;
		for ( i = 0; i < sizeof ( upper_table ) /sizeof ( upper_table[0] ); i++ )
		{
			if( !strncmp( upper_table[i][1], pos, strlen( upper_table[i][1]  ) ) )
			{
				ret += upper_table[i][0];
				pos += strlen( upper_table[i][1] );
				break;
			}
		}
		if ( i == sizeof ( upper_table ) /sizeof ( upper_table[0] ) ) {
			ret += ::toupper ( *pos );
			++pos;
		}
	}
	// Next letters, no touch
	while( *pos ) {
		ret += *pos;
		++pos;
	}
	return ret;
}


int CapelModule::remove_empty_extrusions(const XtringList &extrusions)
{
	for( const Xtring &s: extrusions) {
		if( is_empty_extrusion(s) ) {
			remove_extrusion(s);
		}
	}
}

bool CapelModule::is_empty_extrusion(const gong::Xtring& extrusion_name) const 
{
	Xtring::size_type begin = lookup_extrusion_code(extrusion_name);
	if( begin != Xtring::npos ) {
		int end = lookup_extrusion_end(extrusion_name);
		if( end != Xtring::npos ) {
			--end;
			while( begin < end ) {
				if (!isspace(mBuffer[++begin]) ) {
					return false;
				}
			}
		}
	}
	return true;
}



}
