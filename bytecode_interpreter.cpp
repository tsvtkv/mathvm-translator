#include "bytecode_interpreter.hpp"
#include "errors.hpp"

#include <iostream>

#define BIN_OP(type, op) {    \
  type upper = pop<type>();   \
  type lower = pop<type>();   \
  push<type>(upper op lower); \
}                              

namespace mathvm {

void BytecodeInterpreter::execute() {
  while (ins_ < bc()->length()) {
    switch (bc()->getInsn(ins_++)) {
      case BC_INVALID: 
        throw InterpreterException("Encountered invalid instruction");
      
      case BC_ILOAD0: push<int64_t>(0); break;      
      case BC_ILOAD1: push<int64_t>(1); break;      
      case BC_ILOADM1: push<int64_t>(-1); break;      
      case BC_DLOAD0: push<double>(0); break;      
      case BC_DLOAD1: push<double>(1); break;      
      case BC_DLOADM1: push<double>(-1); break;      

      case BC_ILOAD: load<int64_t>(); break;
      case BC_DLOAD: load<double>(); break;
      case BC_SLOAD: load<uint16_t>(); break;
      
      case BC_IPRINT: std::cout << pop<int64_t>(); break;
      case BC_DPRINT: std::cout << pop<double>(); break;
      case BC_SPRINT: std::cout << code_->constantById(pop<uint16_t>()); break;

      case BC_DADD: BIN_OP(double, +); break;
      case BC_DSUB: BIN_OP(double, -); break;
      case BC_DMUL: BIN_OP(double, *); break;
      case BC_DDIV: BIN_OP(double, /); break;

      case BC_IADD: BIN_OP(int64_t, +); break;
      case BC_ISUB: BIN_OP(int64_t, -); break;
      case BC_IMUL: BIN_OP(int64_t, *); break;
      case BC_IDIV: BIN_OP(int64_t, /); break;
      case BC_IMOD: BIN_OP(int64_t, %); break;

      case BC_SWAP: {
        int64_t upper = pop<int64_t>();
        int64_t lower = pop<int64_t>();
        push(upper);
        push(lower);
        break;
      }

      case BC_POP: remove(); break;
      
      case BC_STOP: return;
      
      default: throw InterpreterException("Not implemented instruction");
    }
  } // while
}

} // namespace mathvm