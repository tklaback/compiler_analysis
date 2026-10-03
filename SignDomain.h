
#ifndef SIGN_DOMAIN_H
#define SIGN_DOMAIN_H

#include "llvm/Support/raw_ostream.h"

namespace sign {

enum class Kind { Bottom, Minus, Zero, One, Plus, ZeroMinus, ZeroPlus, Top };

inline const char *name(Kind kind) {
  switch (kind) {
  case Kind::Bottom:
    return "bottom";
  case Kind::Minus:
    return "minus";
  case Kind::Zero:
    return "zero";
  case Kind::One:
    return "one";
  case Kind::Plus:
    return "plus";
  case Kind::ZeroMinus:
    return "zerominus";
  case Kind::ZeroPlus:
    return "zeroplus";
  case Kind::Top:
    return "top";
  }
  return "top";
}

struct SignState {
  Kind kind = Kind::Bottom;

  SignState() = default;
  /* implicit */ SignState(Kind kind) : kind(kind) {}

  static SignState bottom() { return Kind::Bottom; }
  static SignState top() { return Kind::Top; }

  bool isBottom() const { return kind == Kind::Bottom; }

  /// Least upper bound.  Two disagreeing facts lose all information.
  static SignState join(const SignState &lhs, const SignState &rhs) {
    if (lhs.kind == Kind::Bottom)
      return rhs;
    if (rhs.kind == Kind::Bottom)
      return lhs;
    if (lhs.kind == rhs.kind)
      return lhs;
    if ((lhs.kind == Kind::One && rhs.kind == Kind::Zero) ||
        (rhs.kind == Kind::One && lhs.kind == Kind::Zero))
      return Kind::ZeroPlus;
    if ((lhs.kind == Kind::Plus && rhs.kind == Kind::Zero) ||
        (rhs.kind == Kind::Plus && lhs.kind == Kind::Zero))
      return Kind::ZeroPlus;
    if ((lhs.kind == Kind::Minus && rhs.kind == Kind::Zero) ||
        (rhs.kind == Kind::Minus && lhs.kind == Kind::Zero))
      return Kind::ZeroMinus;
    if ((lhs.kind == Kind::Minus && rhs.kind == Kind::ZeroMinus) ||
        (rhs.kind == Kind::Minus && lhs.kind == Kind::ZeroMinus))
      return Kind::ZeroMinus;
    if ((lhs.kind == Kind::One && rhs.kind == Kind::Plus) ||
        (rhs.kind == Kind::One && lhs.kind == Kind::Plus))
      return Kind::Plus;
    if ((lhs.kind == Kind::Plus && rhs.kind == Kind::ZeroPlus) ||
        (rhs.kind == Kind::Plus && lhs.kind == Kind::ZeroPlus))
      return Kind::ZeroPlus;
    if ((lhs.kind == Kind::ZeroMinus && rhs.kind == Kind::Zero) ||
        (rhs.kind == Kind::ZeroMinus && lhs.kind == Kind::Zero))
      return Kind::ZeroMinus;
    if ((lhs.kind == Kind::ZeroPlus && rhs.kind == Kind::Zero) ||
        (rhs.kind == Kind::ZeroPlus && lhs.kind == Kind::Zero))
      return Kind::ZeroPlus;
    if ((lhs.kind == Kind::ZeroPlus && rhs.kind == Kind::One) ||
        (rhs.kind == Kind::ZeroPlus && lhs.kind == Kind::One))
      return Kind::ZeroPlus;
    return top();
  }

  bool operator==(const SignState &other) const { return kind == other.kind; }
  bool operator!=(const SignState &other) const { return kind != other.kind; }

  void print(llvm::raw_ostream &os) const { os << name(kind); }
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                                     const SignState &state) {
  state.print(os);
  return os;
}

} // namespace sign

#endif
