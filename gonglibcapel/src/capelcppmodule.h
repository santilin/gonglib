
#ifndef CAPELCAPELCPPMODULE_H_
#define CAPELCAPELCPPMODULE_H_

#include <capelmodule.h>


namespace capel {


class CppModule: public CapelModule
{
public:
	CppModule( const Xtring &filename,
	                const Xtring &begin_keyword = "<<<<<", const Xtring &end_keyword = ">>>>>",
	                const Xtring &begin_comment = "/*", const Xtring &end_comment = "*/",
					const Xtring &params_delimiter_begin = "¿", const Xtring &params_delimiter_end = "?" )
		: CapelModule(filename, begin_keyword, end_keyword, begin_comment, end_comment)
		{
			mParameterDelimiterBegin = params_delimiter_begin;
			mParameterDelimiterEnd = params_delimiter_end;
		}

	virtual void addCopyLeft(const Xtring &project, const Xtring &file,
							  const Xtring &license = Xtring());
	virtual void addClassDefinition(const Xtring &fullclassname,
		const gong::XtringList &members, const Xtring &classprops);
	virtual void addHeaderPreamble(const Xtring &fullclassname);
	virtual void addHeaderPostamble(const Xtring &fullclassname);
	void addHeaderProtect(const Xtring &fullclassname);
	Xtring getHeaderProtect(const Xtring &fullclassname) const;

	static Xtring extractClassname(const Xtring &fullclassname);
	static Xtring extractNamespace(const Xtring &fullclassname);
protected:
	Xtring functionParameter(const Xtring &type);
	Xtring functionReturn(const Xtring &type, bool isreturnconst);
	Xtring getDefault(const Xtring &type, const Xtring &defaultvalue);
	Xtring unpackType(const Xtring &type);
	Xtring memberName(const Xtring &type, const Xtring &membername);
	Xtring methodGetName(const Xtring &type, const Xtring &membername);
};

}

#endif
