#include "translation_utils.hpp"
#include "errors.hpp"
#include "info.hpp"

namespace mathvm {

AstVar* findVariable(const std::string& name, Scope* scope, AstNode* at) {
  AstVar* var = scope->lookupVariable(name);

  if (var == 0) {
    throw TranslationException(at, "Variable '%s' is not defined", name.c_str());
  }

  return var;
}

AstFunction* findFunction(const std::string& name, Scope* scope, AstNode* at) {
  AstFunction* function = scope->lookupFunction(name);

  if (function == 0) {
    throw TranslationException(at, "Function '%s' is not defined", name.c_str());
  }

  return function;
}

bool isTopLevel(AstFunction* function) {
  return function->name() == AstFunction::top_name;
}

bool isTopLevel(FunctionNode* function) {
  return function->name() == AstFunction::top_name;
}

bool isNumeric(VarType type) {
  return type == VT_INT || type == VT_DOUBLE;
}

bool hasNonEmptyStack(const AstNode* node) {
  if (node->isCallNode()) {
    return typeOf(node) != VT_VOID;
  }

  return node->isBinaryOpNode() 
         || node->isUnaryOpNode()
         || node->isStringLiteralNode()
         || node->isDoubleLiteralNode()
         || node->isIntLiteralNode()
         || node->isLoadNode();
}

static void castImpl(VarType from, VarType to, Bytecode* bc, AstNode* node);

void cast(AstNode* expr, VarType to, Bytecode* bc) {
  VarType from = typeOf(expr);
  castImpl(from, to, bc, expr);
}

void castImpl(VarType from, VarType to, Bytecode* bc, AstNode* node) {
  if (from == VT_DOUBLE && to == VT_INT) {
    bc->addInsn(BC_D2I);
  } else if (from == VT_INT && to == VT_DOUBLE) {
    bc->addInsn(BC_I2D);
  } else if (from != to) {
    throw TranslationException(node, "Illegal type: expected -- %s, actual -- %s", 
                               typeToName(to), typeToName(from));
  }
}

} // namespace mathvm