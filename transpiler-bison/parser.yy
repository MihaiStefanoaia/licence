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
  DOLLAR  "$"
  SEMIC   ";"
  COMMA   ","
  COLON   ":"
  WIRE    "wire"
  ARRAY   "array"
  LT      "<"
  GT      ">"
  AUTOGEN "__autogen"
  LAYOUT "layout"
;

%token <std::string> IDENTIFIER
%token <int> NUMBER
%nterm <nlohmann::json> stmt
%nterm <nlohmann::json> sys_cmd
%nterm <nlohmann::json> wire_decl
%nterm <nlohmann::json> array_decl
%nterm <nlohmann::json> module_decl
%nterm <nlohmann::json> layout_decl
%nterm <nlohmann::json> layout_args
%nterm <nlohmann::json> l_arg
%nterm <nlohmann::json> args
%nterm <nlohmann::json> access

%printer { yyo << $$; } <*>;

%%
%start document;

document:
  document stmt SEMIC {trp.ret += $2;}
| %empty {trp.ret = nlohmann::json::array();}
;

stmt: 
  sys_cmd {$$ = $1;};
| module_decl {$$ = $1;}
| wire_decl {$$ = $1;}
| array_decl {$$ = $1;};
| layout_decl {$$ = $1;};
;

sys_cmd:
  DOLLAR IDENTIFIER ARGS_B NUMBER ARGS_E{
    $$["stmt_type"] = "sys_cmd";
    $$["type"] = $2;
    $$["value"] = $4;
  };
;

wire_decl:
  WIRE IDENTIFIER {
    $$["stmt_type"] = "wire_decl";
    $$["name"] = $2;
  };

array_decl:
  ARRAY LT NUMBER GT IDENTIFIER ARGS_B args ARGS_E{
    $$["stmt_type"] = "array_decl";
    $$["name"] = $5;
    $$["size"] = $3;
    $$["args"] = $7;
  }
| ARRAY LT NUMBER GT IDENTIFIER ARGS_B AUTOGEN ARGS_E {
  $$["stmt_type"] = "autogen_decl";
  $$["name"] = $5;
  $$["size"] = $3;
};
;

module_decl:
  IDENTIFIER IDENTIFIER ARGS_B args ARGS_E{
    $$["stmt_type"] = "module_decl";
    $$["type"] = $1;
    $$["name"] = $2;
    $$["args"] = $4;
  };

layout_decl:
  LAYOUT IDENTIFIER ACC_B layout_args ACC_E{
    $$["stmt_type"] = "layout_decl";
    $$["name"] = $2;
    $$["args"] = $4;
  }
;

layout_args:
  layout_args COMMA l_arg{
    $$ = $1;
    $$ += $3;
  }
| l_arg{
  $$ = {$1};
  }
;

l_arg:
  ARGS_B NUMBER COMMA NUMBER ARGS_E COLON IDENTIFIER{
    $$["x"] = $2;
    $$["y"] = $4;
    $$["content"] = $7;
  }
;

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
    $$["type"] = "array_access";
    $$["name"] = $1;
    $$["index"] = $3;
  };
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
