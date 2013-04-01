#ifndef RT_AST_FUNCTION_HPP
#define RT_AST_FUNCTION_HPP

#include "compiler/ast/statement.hpp"
#include "compiler/ast/expression.hpp"
#include "compiler/ast/global.hpp"
#include "compiler/symboltable.hpp"
#include "compiler/gen_state.hpp"

#include <vector>

namespace raytrace {
  
  namespace ast {

    /* Expression that calls a particular function and returns the result. */
    class func_call : public expression {
    public:

      func_call(const std::string &fname,
		const std::vector<expression_ptr> &args,
		func_symbol_table *func_symtab);
      virtual ~func_call() {}
      
      virtual llvm::Value *codegen(llvm::Module *module, llvm::IRBuilder<> &builder);
      virtual type_spec typecheck();

    private:

      std::string fname;
      std::vector<expression_ptr> args;
      func_symbol_table *func_symtab;

      std::string get_symtab_name();
    };

    /* 
       Represents a function prototype, defining the return type, name and arguments of a function.
       A function may be declared multiple times, as long as the body is only defined once and all
       prototypes match.
     */
    class prototype : public global_declaration {
    public:
      
      //Locally defined functions.
      prototype(const std::string &name, const type_spec &return_type,
		const std::vector<function_argument> &args, func_symbol_table *func_symtab);

      //Externally defined functions.
      prototype(const std::string &name, const std::string &extern_name,
		const type_spec &return_type, const std::vector<function_argument> &args,
		func_symbol_table *func_symtab);

      virtual ~prototype() {}

      virtual llvm::Function *codegen(llvm::Module *module, llvm::IRBuilder<> &builder);

      const std::string &function_name() { return name; }
      void set_external(const std::string &extern_name);
      
      size_t num_args() { return args.size(); }
      const function_argument &get_arg(int i) { return args[i]; }
      const type_spec &get_return_type() { return return_type; }

      bool is_external() { return external; }
      
    private:

      std::string name, extern_name;
      type_spec return_type;
      std::vector<function_argument> args;
      bool external;
      
      func_symbol_table *func_symtab;

      //checks to see if this function has been previously defined (and if so, do the prototypes match).
      llvm::Function *check_for_entry();
    };

    typedef std::shared_ptr<prototype> prototype_ptr;
    
    /* Represents a full function definition. */
    class function : public global_declaration {
    public:
      
      function(const prototype_ptr &defn, const statement_list &body,
	       var_symbol_table *symtab, control_state *control);
      virtual ~function() {}

      virtual llvm::Function *codegen(llvm::Module *module, llvm::IRBuilder<> &builder);

    private:

      prototype_ptr defn;
      statement_list body;
      var_symbol_table *symtab;
      control_state *control;

      llvm::AllocaInst *create_argument_alloca(llvm::Function *f, const function_argument &arg_name);
      
    };

    typedef std::shared_ptr<function> function_ptr;

    /* Returns a value from a function. The expression pointer may be NULL, in which case nothing is returned. */
    class return_statement : public statement {
    public:
      
      return_statement(const expression_ptr &expr, control_state *control);
      virtual ~return_statement() {}

      virtual llvm::Value *codegen(llvm::Module *module, llvm::IRBuilder<> &builder);
      virtual bool is_terminator() { return true; }

    private:
      
      expression_ptr expr;
      control_state *control;

    };
    
  };
  
};

#endif