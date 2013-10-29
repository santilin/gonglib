
#ifndef CAPELCAPELPHPMODULE_H_
#define CAPELCAPELPHPMODULE_H_

#include <capelmodule.h>


namespace capel {


class PhpModule: public CapelModule
{
public:
	PhpModule( const Xtring &filename,
	                const Xtring &begin_keyword = "<<<<<", const Xtring &end_keyword = ">>>>>",
	                const Xtring &begin_comment = "/*", const Xtring &end_comment = "*/" )
		: CapelModule(filename, begin_keyword, end_keyword, begin_comment, end_comment)
		{}
		
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
