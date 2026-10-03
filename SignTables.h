#ifndef SIGN_TABLES_H
#define SIGN_TABLES_H

#include "SignDomain.h"

namespace sign {


// clang-format off

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

inline constexpr Kind mulTable[5][5] = {
/* ⊥ */ {Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom, Kind::Bottom},
/* 0 */ {Kind::Bottom, Kind::Zero,   Kind::Zero,   Kind::Zero,   Kind::Zero },
/* - */ {Kind::Bottom, Kind::Zero,   Kind::Plus,   Kind::Minus,  Kind::Top  },
/* + */ {Kind::Bottom, Kind::Zero,   Kind::Minus,  Kind::Plus,   Kind::Top  },
/* ⊤ */ {Kind::Bottom, Kind::Zero,   Kind::Top,    Kind::Top,    Kind::Top  },
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

} // namespace sign

#endif
