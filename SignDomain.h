
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
  static ZeroState join(const SignState &lhs, const SignState &rhs) {
    if (lhs.kind == Kind::Bottom)
      return rhs;
    if (rhs.kind == Kind::Bottom)
      return lhs;
    if (lhs.kind == rhs.kind)
      return lhs;
    if (lhs.kind == Kind::One && lhs.kind == Kind::Zero)
      return ZeroPlus;
    if (lhs.kind == Kind::Plus && lhs.kind == Kind::Zero)
      return ZeroPlus;
    if (lhs.kind == Kind::Minus && lhs.kind == Kind::Zero)
      return 
    return top();
  }

  bool operator==(const SignState &other) const { return kind == other.kind; }
  bool operator!=(const SignState &other) const { return kind != other.kind; }

  void print(llvm::raw_ostream &os) const { os << name(kind); }
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &os,
                                     const ZeroState &state) {
  state.print(os);
  return os;
}

} // namespace zero

#endif
