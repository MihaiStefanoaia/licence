#include "transpiler.h"
namespace sim{
    transpiler::transpiler() : trace_parsing(false), trace_scanning(false){
    }

    int transpiler::parse(const std::string& str){
        file = str;
        location.initialize (&file);
        scan_begin ();
        yy::parser parse (*this);
        parse.set_debug_level (trace_parsing);
        int res = parse ();
        scan_end ();
        return res;
    }


    nlohmann::json transpiler::transpile(const std::string &path) {
        parse(path);
        return ret;
    }

}