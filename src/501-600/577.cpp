#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

// sequence https://oeis.org/A000914
unsigned long long A000914(unsigned long long n)
{
  return n * (n+1) * (n+2) * (3*n+5) / 24;
}

// sequence https://oeis.org/A228317
unsigned long long A228317(unsigned long long n)
{
  return n * (n-1) * (n-2) * (3*n-5) /  8;
}

// sequence https://oeis.org/A236770
unsigned long long A236770(unsigned long long n)
{
  return n * (n+1) * (3*n*n + 3*n - 2) / 8;
}

int main()
{
  unsigned int size = 12345; // n

  // indices for A000914, A228317 and A236770
  unsigned int i000914 = 1;
  unsigned int i228317 = 3;
  unsigned int i236770 = 1;

  unsigned long long sum = 0;
  // no hexagons below n = 3
  for (unsigned int n = 3; n <= size; n++)
  {
    unsigned long long fast = 0;
    // method of calculation depends on n mod 3
    switch (n % 3)
    {
    case 0:
      fast = A236770(i236770++);
      break;
    case 1:
      fast = A228317(i228317++);
      break;
    case 2:
      fast = 3 * A000914(i000914++);
      break;
    }

    sum += fast;
  }

  // that's it !
  std::cout << sum << std::endl;
  return 0;
}