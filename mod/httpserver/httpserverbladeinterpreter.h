#ifndef HTTPSERVER_BLADEINTERPRETER_H
#define HTTPSERVER_BLADEINTERPRETER_H

#include <map>
#include <gongxtring.h>

namespace gong {
namespace httpserver {

  
class BladeInterpreter
{
public:
	static Xtring interpret(const Xtring &source, const std::map<Xtring, Xtring> &variables);
};
  
} // namespace httpserver 
} // namespace gong 

#endif // HTTPSERVER_BLADEINTERPRETER_H



