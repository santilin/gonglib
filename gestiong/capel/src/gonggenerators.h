
#include "gonglib_inc.h"
#include "capelcppmodule.h"

namespace capel {
void genMakefileCvs();
void genMakefileAm(const Xtring &filename, const gong::XtringList &modules, const Xtring &args);
void genSrc_MakefileAm(const Xtring &type, const Xtring &args);
void genModuleCpp(const Xtring &args, const gong::XtringList &modules);
void genModuleConfigure_ac(CapelModule *cpm, const gong::XtringList &modules, const Xtring &args);
void getConditionalCode( const Xtring &moreprops, Xtring &csh, Xtring &cscpp, Xtring &ceh, Xtring &cecpp );
};
