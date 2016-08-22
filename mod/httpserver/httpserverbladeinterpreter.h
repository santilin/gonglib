#ifndef HTTPSERVER_BLADEINTERPRETER_H
#define HTTPSERVER_BLADEINTERPRETER_H

#include <gongxtring.h>
#include <gongdictionary.h>

namespace gong {
namespace httpserver {

  
class BladeInterpreter
{
public:
	static Xtring interpret(const Xtring &source, const Dictionary<Xtring> &variables);
};
  
} // namespace httpserver 
} // namespace gong 

#endif // HTTPSERVER_BLADEINTERPRETER_H



