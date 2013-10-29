
#include "gonglib_inc.h"
#include "capelcppmodule.h"

namespace capel {

void addFrmEditBehavior(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const gong::Xtring &recname, const gong::XtringList &fields,
	const gong::Xtring &props, const gong::Xtring &inherit);
void addFrmEditBehaviorFields(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const Xtring &recname, const gong::XtringList &fields,
	const gong::XtringList &members, const gong::Xtring &props, const Xtring &inherit);

}