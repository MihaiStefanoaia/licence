%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.5.1"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  #include "json.hpp"
  namespace sim{
    class transpiler;
  }
}

// The parsing context.
%param { sim::transpiler& trp }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "transpiler.h"
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  ARGS_B  "("
  ARGS_E  ")"
  ACC_B   "["
  ACC_E   "]"
  SYS_C   "$"
  SEMIC   ";"
  COMMA   ","
;

%token <std::string> IDENTIFIER
%token <int> NUMBER
%nterm <nlohmann::json> stmt
%nterm <nlohmann::json> sys_cmd
%nterm <nlohmann::json> wire_decl
%nterm <nlohmann::json> module_decl
%nterm <nlohmann::json> args
%nterm <nlohmann::json> access

%printer { yyo << $$; } <*>;

%%
%start document;

document:
  document stmt SEMIC {trp.ret += $2;}
| %empty {trp.ret = nlohmann::json::array();};

stmt: 
  sys_cmd {$$ = $1;};
| module_decl {$$ = $1;};
|  wire_decl {$$ = $1;}

sys_cmd:
  SYS_C IDENTIFIER ARGS_B NUMBER ARGS_E{
    $$["stmt_type"] = "sys_cmd";
    $$["type"] = $2;
    $$["value"] = $4;
  };
;

wire_decl:
  IDENTIFIER IDENTIFIER {
    $$["stmt_type"] = "wire_decl";
    $$["type"] = $1;
    $$["name"] = $2;
  };

module_decl:
  IDENTIFIER IDENTIFIER ARGS_B args ARGS_E{
    $$["stmt_type"] = "module_decl";
    $$["type"] = $1;
    $$["name"] = $2;
    $$["args"] = $4;
  };

args:
  args COMMA access{
    $$ = $1;
    $$ += $3;
  }
| access {
    $$ = {$1};
  };

access:
  IDENTIFIER{
    $$["type"] = "basic";
    $$["name"] = $1;
  }
| IDENTIFIER ACC_B NUMBER ACC_E{
    $$["type"] = "array";
    $$["name"] = $1;
    $$["value"] = $3;
  };
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
