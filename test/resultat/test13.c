int main()
{
  int x;
  int a;
  int b;
  a = 1;

  if (a == 1) {
    int a;
    a = 2;
    b = a;
  } else {
    a = 3;
    b = a;
  }

  return b;
}
