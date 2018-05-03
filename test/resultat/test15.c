int main()
{
  int *ptr;
  int a;

  a = 1;
  ptr = &a;
  *ptr = 2;

  return a;
}
