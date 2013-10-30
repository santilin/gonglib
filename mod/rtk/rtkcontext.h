////////////////////////////////////////////////////////////////////////////////
// DScript Scripting Language
// Copyright (C) 2003 Bryan "daerid" Ross
//
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.
////////////////////////////////////////////////////////////////////////////////

#ifndef __DSCRIPT_CONTEXT_H__
#define __DSCRIPT_CONTEXT_H__

////////////////////////////////////////////////////////////////////////////////
// Standard Library Includes
#include <string>
#include <iostream>
#include <map>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DScript Includes
#include "rtkvalue.h"
#include "rtkfunctions.h"
#include "rtkvmachine.h"
#include "rtkcolor.h"
////////////////////////////////////////////////////////////////////////////////
#include <rtkreport.h>

namespace gong {
class Report;
}

namespace dscript
{
/// Encapsulates an entire runtime environment
/// for executing DScript scripts
class context
{
public:
    context(gong::Report *r);
    void enable_logging(std::ostream* out);
    void disable_logging();
    void log_msg(const std::string& message);
    void set_return(const value& val);

    void link_function(const char* name,host_function_t callback);
    void link_function(
        const char* name,
        host_function_t callback,
        int minargs,
        int maxargs,
        const char* usage
    );

    value eval(const std::string& code);
    bool exec(const std::string& file);
    bool exec_compiled(const std::string& file);
    bool compile(const std::string& file);

    value call(const std::string& func);
    value call(const std::string& func,const args_t& args);

    value get_local(const std::string& name);
    void set_local(const std::string& name,const value& val);

    value get_global(const std::string& name);
    void set_global(const std::string& name,const value& val);

    void dump_code(std::ostream& out,const std::string& code);
    void dump_file(std::ostream& out,const std::string& file);
    class gong::Report *getReport() const {
            return report;
        }
private:
    vmachine runtime;
    std::map<std::string,codeblock_t> codeblocks;
    std::ostream* log_out;
    class gong::Report *report;
};
}

#endif//__DSCRIPT_CONTEXT_H__
