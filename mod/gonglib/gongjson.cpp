#include <fstream>
#include "gongdebug.h"
#include "gongxtring.h"
#include "gongjson.h"

namespace gong {

/*
 * Por si alguna vez quiero mostrar el número de línea
 * https://stackoverflow.com/questions/4813129/how-to-get-the-line-number-from-a-file-in-c
 */	
	
JsonTree readJson(const std::string &jsonfile)
{
    std::ifstream f(jsonfile);
	if ( !f.is_open() || !f.good() ) {
        _GONG_DEBUG_WARNING( Xtring::printf("%s: %s", strerror( errno ), jsonfile.c_str()) );
        return JsonTree();
    }
	try {
		return json::parse(f);
	} catch( nlohmann::detail::parse_error &e ) {
		Xtring err = e.what();
		std::cout << err << std::endl;
		if (e.byte > 1 )
			f.seekg(e.byte - 2);
		else 
			f.seekg(0);
		char text[121];
		f.read(text, 120);
		std::cout << text << std::endl;
		exit(1);
	}
}

} // namespace gong
     

