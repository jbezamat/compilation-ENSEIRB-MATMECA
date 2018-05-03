int g(float a, int b)
{
  int r;
  r = a;
  a = a + b;
  r = r + a;
  return r;
}

void f()
{
  float a;
  int b;
  int c;
  a = 1.5;
  b = 2;
  c = g(a, b);
}
