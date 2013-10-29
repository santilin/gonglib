
#include "gonglib_inc.h"
#include "capelcppmodule.h"

namespace capel {

void insert_cabeceras(const gong::XtringList &props, CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const gong::Xtring &recname, const Xtring &ext_prefix);
void insert_detalles(const gong::XtringList &props, CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const gong::Xtring &recname, const Xtring &ext_prefix);
void addFrmEditRecMaster(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const gong::Xtring &recname, const gong::XtringList &fields,
	const gong::Xtring &props, const gong::Xtring &inherit, const gong::XtringList &interfaces);
void addFrmEditRecMasterFields(CppModule *cpcpp, CppModule *cph, const Xtring &frmtype,
	const Xtring &recname, const gong::XtringList &fields,
	const gong::XtringList &members, const gong::Xtring &props, const Xtring &inherit);

}