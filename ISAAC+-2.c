#include <stdio.h>
#include <stddef.h>

typedef unsigned long int ub4;

ub4 randrsl[256], randcnt;
static ub4 mm[256];
static ub4 aa=0, bb=0, cc=0;

void mix(ub4 a, ub4 b, ub4 c, ub4 d, ub4 e, ub4 f, ub4 g, ub4 h)
{
   a ^= b << 11; d += a; b += c;
   b ^= c >> 2;  e += b; c += d;
   c ^= d << 8;  f += c; d += e;
   d ^= e >> 16; g += d; e += f;
   e ^= f << 10; h += e; f += g;
   f ^= g >> 4;  a += f; g += h;
   g ^= h << 8;  b += g; h += a;
   h ^= a >> 9;  c += h; a += b;
}

void isaac_plus()
{
   register ub4 i, x, y;

   cc = cc + 1;
   bb = bb + cc;

   for (i = 0; i < 256; ++i)
   {
     x = mm[i];
     aa = (i % 2 == 0) ? aa ^ (aa << 13) : aa ^ (aa >> 6);
     aa = mm[(i + 128) % 256] + aa;
     mm[i] = y = mm[(x >> 2) % 256] + aa + bb;
     randrsl[i] = bb = mm[(y >> 10) % 256] + x;
   }
}

void randinit(int flag)
{
   int i;
   ub4 a, b, c, d, e, f, g, h;
   aa = bb = cc = 0;
   a = b = c = d = e = f = g = h = 0x9e3779b9;

   for (i = 0; i < 4; ++i)
   {
     mix(a, b, c, d, e, f, g, h);
   }

   for (i = 0; i < 256; i += 8)
   {
     if (flag)
     {
       a += randrsl[i]; b += randrsl[i + 1]; c += randrsl[i + 2]; d += randrsl[i + 3];
       e += randrsl[i + 4]; f += randrsl[i + 5]; g += randrsl[i + 6]; h += randrsl[i + 7];
     }
     mix(a, b, c, d, e, f, g, h);
     mm[i] = a; mm[i + 1] = b; mm[i + 2] = c; mm[i + 3] = d;
     mm[i + 4] = e; mm[i + 5] = f; mm[i + 6] = g; mm[i + 7] = h;
   }

   if (flag)
   {
     for (i = 0; i < 256; i += 8)
     {
       a += mm[i]; b += mm[i + 1]; c += mm[i + 2]; d += mm[i + 3];
       e += mm[i + 4]; f += mm[i + 5]; g += mm[i + 6]; h += mm[i + 7];
       mix(a, b, c, d, e, f, g, h);
       mm[i] = a; mm[i + 1] = b; mm[i + 2] = c; mm[i + 3] = d;
       mm[i + 4] = e; mm[i + 5] = f; mm[i + 6] = g; mm[i + 7] = h;
     }
   }

   isaac_plus();
   randcnt = 256;
}

int main()
{
  
  ub4 i, j;
  aa = bb = cc = (ub4)0;
  for (i = 0; i < 256; ++i) mm[i] = randrsl[i] = (ub4)0;
  randinit(1);
 //for (int i=0;i<0;i ++) isaac_plus();
  for (i = 0; i < 2; ++i)
  {
    isaac_plus();
    for (j = 0; j < 256; ++j)
    {
      printf("%.8lx", randrsl[j]);
    //  if ((j & 7) == 7) printf("\n");
    }
  }
}
