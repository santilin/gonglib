
#ifndef CAPELRTKMODULE_H_
#define CAPELRTKMODULE_H_

#include <capelmodule.h>


namespace capel {

class RtkModule: public CapelModule
{
public:
	RtkModule( const Xtring &filename,
	                const Xtring &begin_keyword = "<<<<<", const Xtring &end_keyword = ">>>>>",
	                const Xtring &begin_comment = "//", const Xtring &end_comment = "\n" )
		: CapelModule(filename, begin_keyword, end_keyword, begin_comment, end_comment)
		{}
		
	virtual void addCopyLeft(const Xtring &project, const Xtring &file,
							  const Xtring &license = Xtring());
	virtual void addDefinition(const Xtring &_type, const Xtring &_repname, 
		const Xtring &reptitle, const gong::XtringList &fields, const Xtring &args);
};

}

#endif
