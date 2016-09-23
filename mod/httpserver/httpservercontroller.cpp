#include <sstream>
#include "httpservercontroller.h"

namespace gong {
namespace httpserver {

Controller *Controller::addRoutes()
{
	return this;
}

bool Controller::url_decode(const Xtring& in, Xtring& out)
{
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}
	
	
void Controller::parsePlainJSON(const Xtring& JSON, XtringList names_values)
{

}

	

} // namespace httpserver 
} // namespace gong 
