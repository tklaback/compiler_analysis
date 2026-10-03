#ifndef SIGN_TABLES_H
#define SIGN_TABLES_H

#include "SignDomain.h"

namespace sign {



// I had claude copy these tables directly from the book. Note: no 0+, 0-, or 1 are included right now. I recognize this TODO.

//          ⊥             0            -            +            ⊤
inline constexpr Kind addTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Zero,   Kind::Minus,  Kind::Plus,   Kind::Top  },
/* - */ {Kind::Bottom, Kind::Minus,  Kind::Minus,  Kind::Top,    Kind::Top  },
/* + */ {Kind::Bottom, Kind::Plus,   Kind::Top,    Kind::Plus,   Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top,    Kind::Top  },
};



inline constexpr Kind subTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Zero,   Kind::Plus,   Kind::Minus,  Kind::Top  },
/* - */ {Kind::Bottom, Kind::Minus,  Kind::Top,    Kind::Minus,  Kind::Top  },
/* + */ {Kind::Bottom, Kind::Plus,   Kind::Plus,   Kind::Top,    Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top,    Kind::Top  },
};




// I implemented the extra 3 abstract values here:
//            ⊥             -             0            1            +            0-                0+              ⊤
inline constexpr Kind mulTable[8][8] = {
/* ⊥  */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom,    Kind::Bottom,    Kind::Bottom},
/* -  */ {Kind::Bottom, Kind::Plus,   Kind::Zero,   Kind::Minus,  Kind::Minus,  Kind::ZeroPlus,  Kind::ZeroMinus, Kind::Top   },
/* 0  */ {Kind::Bottom, Kind::Zero,   Kind::Zero,   Kind::Zero,   Kind::Zero,   Kind::Zero,      Kind::Zero,      Kind::Zero  },
/* 1  */ {Kind::Bottom, Kind::Minus,  Kind::Zero,   Kind::One,    Kind::Plus,   Kind::ZeroMinus, Kind::ZeroPlus,  Kind::Top   },
/* +  */ {Kind::Bottom, Kind::Minus,  Kind::Zero,   Kind::Plus,   Kind::Plus,   Kind::ZeroMinus, Kind::ZeroPlus,  Kind::Top   },
/* 0- */ {Kind::Bottom, Kind::ZeroPlus,  Kind::Zero, Kind::ZeroMinus, Kind::ZeroMinus, Kind::ZeroPlus,  Kind::ZeroMinus, Kind::Top},
/* 0+ */ {Kind::Bottom, Kind::ZeroMinus, Kind::Zero, Kind::ZeroPlus,  Kind::ZeroPlus,  Kind::ZeroMinus, Kind::ZeroPlus,  Kind::Top},
/* ⊤  */ {Kind::Bottom, Kind::Top,    Kind::Zero,   Kind::Top,    Kind::Top,    Kind::Top,       Kind::Top,       Kind::Top   },
};


// A zero divisor is undefined, so those entries are bottom: no defined
// execution reaches them.
inline constexpr Kind divTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Bottom, Kind::Zero,   Kind::Zero,   Kind::Top  },
/* - */ {Kind::Bottom, Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top  },
/* + */ {Kind::Bottom, Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top  },
};

// Comparisons yield 0 or 1, so Plus here means "true" and Zero means "false".
inline constexpr Kind gtTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Zero,   Kind::Plus,   Kind::Zero,   Kind::Top  },
/* - */ {Kind::Bottom, Kind::Zero,   Kind::Top,    Kind::Zero,   Kind::Top  },
/* + */ {Kind::Bottom, Kind::Plus,   Kind::Plus,   Kind::Top,    Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top,    Kind::Top  },
};

inline constexpr Kind eqTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Plus,   Kind::Zero,   Kind::Zero,   Kind::Top  },
/* - */ {Kind::Bottom, Kind::Zero,   Kind::Top,    Kind::Zero,   Kind::Top  },
/* + */ {Kind::Bottom, Kind::Zero,   Kind::Zero,   Kind::Top,    Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Top,    Kind::Top,    Kind::Top,    Kind::Top  },
};

// clang-format on

/// Look a pair of states up in one of the tables above.
inline int tableIndex(Kind kind) {
  switch (kind) {
  case Kind::Bottom: return 0;
  case Kind::Zero:   return 1;
  case Kind::Minus:  return 2;
  case Kind::One:
  case Kind::Plus:   return 3;
  default:           return 4;
  }
}

inline Kind apply(const Kind table[5][5], Kind lhs, Kind rhs) {
  return table[tableIndex(lhs)][tableIndex(rhs)];
}

/// mulTable is indexed by enum order, so every Kind maps to its own row.
inline int mulIndex(Kind kind) { return static_cast<int>(kind); }

inline Kind apply(const Kind table[8][8], Kind lhs, Kind rhs) {
  return table[mulIndex(lhs)][mulIndex(rhs)];
}

} // namespace sign

#endif
