//===- ZeroAnalysis.cpp - Transfer functions ------------------------------===//
//
// The transfer function: given what is known about an operation's operands,
// state what is known about its results.  This file and ZeroDomain.h are the
// two to replace when building a different analysis; the rest of the project
// is scaffolding.
//
// There are deliberately only two rules here, one of each kind an analysis
// needs: one that introduces facts out of nothing (constants), and one that
// propagates facts it was given (`and`).  Everything else is unknown.  Adding
// a third rule should be a matter of adding a third `if`.
//
//===----------------------------------------------------------------------===//

#include "SignAnalysis.h"

#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/Matchers.h"

using namespace mlir;

namespace sign {

void SignAnalysis::setToEntryState(SignLattice *lattice) {
  propagateIfChanged(lattice, lattice->join(SignState::top()));
}

bool operandsEqual(mlir::Operation *op) {
  if (op->getOperand(0) == op->getOperand(1))
    return true;
  auto load0 = op->getOperand(0).getDefiningOp<LLVM::LoadOp>();
  auto load1 = op->getOperand(1).getDefiningOp<LLVM::LoadOp>();
  if (load0 && load1 && load0.getAddr() == load1.getAddr())
    return true;
  return false;
  
}

LogicalResult
SignAnalysis::visitOperation(Operation *op,
                             ArrayRef<const SignLattice *> operands,
                             ArrayRef<SignLattice *> results) {
  SignState state;

  // Raising a result to top says "this operation could produce anything",
  // which is always a sound answer and is what every unhandled case does.
  auto unknown = [&] {
    setAllToEntryStates(results);
    return success();
  };

  // Only single-result integer operations are interesting here.  Calls, loads,
  // floats, and vectors all land in `unknown`.
  if (op->getNumResults() != 1 || !op->getResult(0).getType().isIntOrIndex())
    return unknown();
  SignLattice *result = results[0];

  // Constants take no operands and unary ops take one, so only read these
  // once an op is known to be binary; they stay bottom otherwise.
  SignState lhs, rhs;
  if (operands.size() >= 2) {
    lhs = operands[0]->getValue();
    rhs = operands[1]->getValue();
  }

  // Rule 1: a constant is zero or nonzero according to what it says.
  // This is the only rule that does not consult its operands, and without some
  // rule of this kind the analysis would have no facts to propagate at all.
  IntegerAttr value;
  if (matchPattern(op, m_Constant(&value))) {
    if (value.getValue().isNegative())
      state = Kind::Minus;
    else if (value.getValue().isZero())
      state = Kind::Zero;
    else if (value.getValue().isOne())
      state = Kind::One;
    else
      state = Kind::Plus;
    propagateIfChanged(result, result->join(state));
    return success();
  }

  // Adding two positives is a positive
  // Multiplying the same number returns a positive
  // subtracting the same numbers returns 0
  // + / + = positive, not top
  // division by zero (zero or Top)
  // something that can be negative or zero?


  if (llvm::isa<mlir::LLVM::MulOp>(op)) {
    
    if (operandsEqual(op)) {
      state = Kind::ZeroPlus;
      propagateIfChanged(result, result->join(state));
    }
    return success();
  }

  if (llvm::isa<mlir::LLVM::SubOp>(op)) {
    
    if (operandsEqual(op)) {
      state = Kind::Zero;
      propagateIfChanged(result, result->join(state));
      return success();
    }

    // Plus - Minus = Plus
    if (lhs == Kind::Plus && rhs == Kind::Minus) {
      state = Kind::Plus;
      propagateIfChanged(result, result->join(state));
      return success();
    }
  }

  if (llvm::isa<mlir::LLVM::SDivOp>(op)) {
    // - / - = +
    if (lhs == Kind::Minus && rhs == Kind::Minus) {
      state = Kind::Plus;
      propagateIfChanged(result, result->join(state));
      return success();
    }
    // 0/{+, -} = 0
    if (lhs == Kind::Zero && (rhs == Kind::Plus || rhs == Kind::Minus)) {
      state = Kind::Zero;
      propagateIfChanged(result, result->join(state));
      return success();
    }

    // x/x = 1
    if (operandsEqual(op) && !(rhs == Kind::Zero || rhs == Kind::ZeroMinus || rhs == Kind::ZeroPlus)) {
      state = Kind::One;
      propagateIfChanged(result, result->join(state));
      return success();
    }

  }

  if (llvm::isa<mlir::LLVM::ICmpOp>(op)) {
    state = Kind::ZeroPlus;
    propagateIfChanged(result, result->join(state));
    return success();
  }

  if (llvm::isa<mlir::LLVM::ZExtOp>(op)) {
    state = Kind::ZeroPlus;
    propagateIfChanged(result, result->join(state));
    return success();
  }

  return unknown();
}

} // namespace sign
