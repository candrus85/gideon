#ifndef RT_AST_NODE_HPP
#define RT_AST_NODE_HPP

#include "compiler/symboltable.hpp"
#include "compiler/gen_state.hpp"

namespace raytrace {

  namespace ast {
    
    /* Stores the state of the language parser. */
    struct parser_state {
      variable_symbol_table variables;
      function_symbol_table functions;
      control_state control;
    };
    
    class ast_node { 
    public:

      ast_node(parser_state *st) : state(st) { }
      virtual ~ast_node() { }
      
    protected:

      parser_state *state;

    };

  };

};

#endif