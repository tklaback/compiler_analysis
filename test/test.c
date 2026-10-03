int h(int a, int b) {
  int t = a;
  int u = t;
  t = b;
  return t * t;
}


int main() {
  h(5, 6);
  return 0;
}