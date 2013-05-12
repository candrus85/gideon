#ifndef GD_SL_SPECIAL_TYPES_HPP
#define GD_SL_SPECIAL_TYPES_HPP

#include "compiler/types.hpp"

namespace raytrace {
  
  class ray_type : public type {
  public:
    
    ray_type(type_table *types) : type(types, "ray", "r") { }
    virtual llvm::Type *llvm_type() const;
    
  };

  class intersection_type : public type {
  public:

    intersection_type(type_table *types) : type(types, "isect", "is") { }
    virtual llvm::Type *llvm_type() const;

  };

  class light_type : public type {
  public:

    light_type(type_table *types) : type(types, "light", "lt") { }
    virtual llvm::Type *llvm_type() const;
  };

  //An instance of a distribution object.
  class dfunc_type : public type {
  public:

    dfunc_type(type_table *types);
    virtual llvm::Type *llvm_type() const;

    //creates a distribution from a shader
    virtual typed_value_container create(llvm::Module *module, llvm::IRBuilder<> &builder,
					 typed_value_vector &args) const;

    virtual typed_value_container initialize(llvm::Module *module, llvm::IRBuilder<> &builder) const;
    virtual llvm::Value *copy(llvm::Value *value, llvm::Module *module, llvm::IRBuilder<> &builder);
    virtual codegen_void destroy(llvm::Value *value, llvm::Module *module, llvm::IRBuilder<> &builder);

    virtual codegen_value op_add(llvm::Module *module, llvm::IRBuilder<> &builder,
				 codegen_value &lhs, codegen_value &rhs) const;

  private:
    
  };

  //Opaque type representing a shader function.
  class shader_handle_type : public type {
  public:

    shader_handle_type(type_table *types) : type(types, "shader_handle", "sh") { }
    virtual llvm::Type *llvm_type() const;

  };

  //Hidden placeholder type, used to pass a 'this' pointer in distributions and shaders.
  class context_ptr_type : public type {
  public:

    context_ptr_type(type_table *types) : type(types, "context_ptr", "ctx") { }
    virtual llvm::Type *llvm_type() const;
    
  };

  //Another hidden type, used to represent a module.
  class module_type : public type {
  public:
    
    module_type(type_table *types) : type(types, "module", "mod") { }
    virtual llvm::Type *llvm_type() const { assert("Module type has no corresponding LLVM type."); }

  };

};

#endif
