int main() {
  // Constants: minus, zero, one, plus.
  int neg = -7;
  int zero = 0;
  int one = 1;
  int pos = 5;

  int sq = pos * pos;   // mul, equal operands -> zeroplus
  int newval = neg * neg;   // mul, equal operands -> zeroplus
  int ident = neg * 1;
  int diff = pos - pos; // sub, equal operands -> zero
  int pm = pos - neg;   // Plus - Minus -> plus
  int nn = neg / neg;   // Minus / Minus -> plus
  int zp = zero / pos;  // Zero / Plus -> zero
  int zm = zero / neg;  // Zero / Minus -> zero
  int xx = pos / pos;   // x / x -> one
  int cmp = pos > zero; // icmp -> zeroplus
  int two_pos = pos + pos;

  return one + sq + ident + diff + pm + nn + newval + zp + zm + xx + cmp + two_pos;
}
