#include "gonglib_inc.h"
#include "capelcppmodule.h"

namespace capel {

void addRecordDefinition(CppModule *cpcpp, CppModule *cph, const Xtring &recname,
	const Xtring &classprops, const gong::XtringList &members,
    const gong::XtringList &relations, const gong::Xtring &inherit,
	const gong::XtringList &interfaces);
void adddbFieldDefinition(CppModule *cpcpp, CppModule *cph, const Xtring &recname,
	const gong::XtringList &members, const Xtring &inherit, const Xtring &args);

}