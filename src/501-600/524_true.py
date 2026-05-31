from math import factorial

def R(k: int) -> int:
  m,c=0,0
  while k>>m:
    if (k>>m)&1: 
        c+=1
    m+=1
  o,l,b=1,0,0
  while k:
    l+=(k&1)
    if m==c and l>b:
      k^=(k & 1)
      a=m+1
    else:
      k//=2
      a=l
      if b<a: a+=1
      if a==0: a=1
    if a<=b:
      b-=1
      l-=1
      c-=1
    else:
      b=a-1
    o+=(a-1)*factorial(m)
    m-=1
  return o

print(R(int(12**12)))