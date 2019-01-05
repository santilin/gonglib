#include "capelcppmodule.h"

namespace capel {

void addModuleDefinition(CppModule *cpcpp, CppModule *cph,
		const Xtring &pluginname, const gong::XtringList &records,
        const gong::XtringList &requires, const gong::XtringList &optional,
		const gong::XtringList &namslisttables);
};
