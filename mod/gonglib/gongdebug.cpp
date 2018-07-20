#include <string>
/**
	Debug levels
	0.- Warnings
	1.- Main program steps
	2.- Main functions
	3.- Main functions phases and other less important functions
	4.- Functions debug output
	5.- Loops
	6.- Others
 */

namespace __gong_debug {

int _gong_debug_level = 0;


}

std::string methodName(const char *pf)
{
	std::string prettyFunction(pf);
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    std::string almost = prettyFunction.substr(begin,end) + "()";
	begin = almost.find(" ");
	if (begin != std::string::npos) {
		return almost.substr(begin+1);
	} else {
		return almost;
	}
}
