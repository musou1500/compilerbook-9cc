#include <stdio.h>
#include <time.h>
#define sqr(x) ((x)*(x))
#define e 2.718281
#define PI 3.14159265358979
#define PRECISION 1e-12

struct Complex
{
  double Real, Imag;
};

struct Matrix
{
  struct Complex mat[10][10];
  int r, c;
};

double QuickPower(double a, int x)
{
  double tmp = 1;

  for (; x; x >>= 1)
  {
    if (x & 1) tmp *= a;
    a *= a;
  }

  return tmp;
}

double Exp(double x)
{
  double a = 1, b = 1, sum = 0, item = 1;
  int count = 0;

  while (item > PRECISION)
  {
    sum += (item = b / a);
    a *= (++count); b *= x;
  }

  return sum;
}

double Sinh(const double x) { double Hype = Exp(x); return (Hype - 1 / Hype) /2; }

double Cosh(const double x) { double Hype = Exp(x); return (Hype + 1 / Hype) / 2; }

double Tanh(const double x) { double Hype = Exp(x); return (Hype - 1 / Hype) / (Hype + 1 / Hype); }

double Sin(double x) 
{
  double t, p, z = 1, s = 0, u = 1;
  int i, count = 0;
  double y = x * PI / 180;
  while (u > PRECISION)
  {
    t = 1; 
    for (i = 1; i <= (2 * count + 1); i++) t *= i; 
    
    p = 1; 
    for (i = 1; i <= (2 * count + 1); i++) p *= y;

    u = p / t;
    s += z * u;
    z *= -1;
    count++;
  }
  return s;
}

double Cos(double x)
{
  double t, p, z = 1, s = 0, u = 1;
  int i, count=0;

  double y = (x * PI) / 180; 
  while (u > PRECISION)
  {
    t = 1; 
    p = 1; 
    for (i = 1; i <= (2 * count); i++) t*=i; 
    
    for (i = 1; i <= (2 * count); i++) p *= y; 

    u = p / t;
    s += z * u; 
    z *= -1;
    count++; 
  }
  return s;
}

double Tan(const double x) { return Sin(x) / Cos(x); }

double Ln(double x)
{
  double a = 1, b = 1, sum = 0, item = 1, y = (x - 1) / (x + 1);
  int i, count = 0;
  if (x <= 0) { printf("error"); return 0; }
  else
  {
    while (item > PRECISION)
    {
      item = b / a;
      sum += (item = b / a);
      a = 2 * (++count) + 1;
      for (i = 2 * (count - 1) + 1; i <= 2 * count; i++) b *= y;
    }
  }
  return 2 * y * sum;
}


double Sqrt(double x)
{
  double k = x;
  while (1)
  {
    if ((sqr(k) > x - PRECISION) && (sqr(k) < x + PRECISION)) break;
    k = 0.5 * (k + x / k);
  }
  return k;
}

double Arctan(double x)
{
  int i, count = 0, z = 1;
  double a = x, b = 1, sum = 0, item = 1;
  if ((x > 1) || (x <- 1)) { printf("error"); return 0; }
  else
  {
    while (item > PRECISION)
    {
      sum += (z * a / b);
      z = z * -1;
      b = 2 * (++count) + 1;
      for (i = 2 * (count - 1) + 2; i <= 2 * count + 1; i++) a *= x;
      item = a / b;
    }
  }
  return sum;
}

double Arctan2(const struct Complex x)
{
  double Res;

  if ((!x.Real) && (!x.Imag)) { printf("Error\n"); return 0; }
  if (!x.Real)
    if (x.Imag > 0) Res = PI / 2;
    else Res = - PI / 2;
  else
  {
    double l = x.Imag / x.Real;
    if (x.Real > 0) Res = Arctan(l);
    else
      if (x.Imag >= 0) Res = Arctan(l) + PI;
      else Res = Arctan(l) -PI;
  }

  return Res;
}

void C_PI()
{
  int len, i;
  int numberator = 1, denominator = 3, result, carry;
  int flag = 1, count = 0;
  char *pi, *temp;
  printf("input the decimal number£∫");
  scanf("%d", &len);

  len += 2;

  pi = (char*)malloc(len * sizeof(char));
  temp = (char*)malloc(len * sizeof(char));

  for(i = 0; i < len; i++) pi[i] = temp[i] = 0;
  pi[1] = 2, temp[1] = 2;

  while (flag)
  {
    carry = 0;
    for(i = len - 1; i > 0; i--)
    {
      result = temp[i] * numberator + carry;
      temp[i] = result % 10;
      carry = result / 10;
    }

    carry = 0;
    for(i = 0; i < len; i++)
    {
      result = temp[i] + carry * 10;
      temp[i] = result / denominator;
      carry = result % denominator;
    }
    flag = 0;
    for(i = len - 1; i > 0; i--)
    {
      result = pi[i] + temp[i];
      pi[i] = result % 10;
      pi[i-1] += result / 10;
      flag |= temp[i];
    }

    numberator++;
    denominator += 2;
  }

  printf("PI = 3.");
  for(i = 2; i < len; i++) printf("%d", (int)pi[i]);
  printf("\n");
}


struct Complex getAComplex(double real, double imag)
{
  struct Complex Node;
  Node.Real = real;
  Node.Imag = imag;
  return Node;
}

void printAComplex(const struct Complex x)
{
  printf("%lf ", x.Real);
  if (!x.Imag) return;
  else printf((x.Imag > 0)?("+"):("-")), printf(" %lf i", x.Imag);
}

struct Complex AddComplex(const struct Complex x, const struct Complex y)
{
  struct Complex Node;

  Node.Real = x.Real + y.Real;
  Node.Imag = x.Imag + y.Imag;

  return Node;
}

struct Complex SubComplex(const struct Complex x, const struct Complex y)
{
  struct Complex Node;

  Node.Real = x.Real - y.Real;
  Node.Imag = x.Imag - y.Imag;

  return Node;
}

struct Complex MutComplex(const struct Complex x, const struct Complex y)
{
  struct Complex Node;

  Node.Real = x.Real * y.Real - x.Imag * y.Imag;
  Node.Imag = x.Real * y.Imag + y.Real * x.Imag;

  return Node;
}

struct Complex DivComplex(const struct Complex x, const struct Complex y)
{
  struct Complex Node;

  Node.Real = (x.Real * y.Real + x.Imag * y.Imag) / (sqr(y.Real) + sqr(y.Imag));
  Node.Imag = (x.Imag * y.Real - x.Real * y.Imag) / (sqr(y.Real) + sqr(y.Imag));

  return Node;
}

struct Complex QuickPowerComplex(struct Complex a, int x)
{
  struct Complex Node = getAComplex(1, 0);

  for (; x; x >>= 1)
  {
    if (x & 1) Node = MutComplex(Node, a);
    a = MutComplex(a, a);
  }

  return Node;
}

struct Complex SinComplex(const struct Complex x)
{
  struct Complex Node;

  Node.Real = Sin(x.Real) * Cosh(x.Imag);
  Node.Imag = Cos(x.Real) * Sinh(x.Imag);

  return Node;
}

struct Complex CosComplex(const struct Complex x)
{
  struct Complex Node;

  Node.Real = Cos(x.Real) * Cosh(x.Imag);
  Node.Imag = - Sin(x.Real) * Sinh(x.Imag);

  return Node;
}

struct Complex TanComplex(const struct Complex x)
{
  struct Complex Nume, Deno;

  Nume.Real = Tan(x.Real); Nume.Imag = Tanh(x.Imag);
  Deno.Real = 1; Deno.Imag = - Tan(x.Real) * Tanh(x.Imag);

  return DivComplex(Nume, Deno);
}

struct Complex ExpComplex(const struct Complex x)
{
  struct Complex Node;
  double Hype = Exp(x.Real);

  Node.Real = Hype * Cos(x.Imag);
  Node.Imag = Hype * Sin(x.Imag);

  return Node;
}

struct Complex LnComplex(const struct Complex x)
{
  struct Complex Node;

  Node.Real = Ln(Sqrt(sqr(x.Real) + sqr(x.Imag)));
  Node.Imag = Arctan2(x);

  return Node;
}

void RootComplex(const struct Complex x)
{
  struct Complex Node;

  double r = Sqrt(sqr(x.Real) + sqr(x.Imag));

  Node.Real = Sqrt((r + x.Real) / 2);
  Node.Imag = Sqrt((r - x.Real) / 2);

  printf("Root of "); printAComplex(x); printf(" is: ");

  if (Node.Imag < 0)
  {
    printf("%lf + %lf i, ", -Node.Real, Node.Imag);
    printf("%lf - %lf i\n",  Node.Real, Node.Imag);
  }
  else
  {
    printf("%lf + %lf i, ", Node.Real, Node.Imag);
    printf("%lf - %lf i\n", -Node.Real, Node.Imag);
  }
}

struct Matrix getAMatrix(int r, int c)
{
  struct Matrix Node;
  int i, j;
  Node.r = r; Node.c = c;
  double Real, Imag;
  printf("Input a matrix(r, c):\n", r, c);

  for (i = 0; i < r; i++)
    for (j = 0; j < c; j++)
    {
      scanf("(%lf, %lf)", &Real, &Imag); getchar();
      Node.mat[i][j] = getAComplex(Real, Imag);
    }

  return Node;
}

void printAMatrix(struct Matrix x)
{
  int i, j;

  for (i = 0; i < x.r; i++)
  {
    for (j = 0; j < x.c - 1; j++) printf("(%lf, %lf), ", x.mat[i][j].Real, x.mat[i][j].Imag);
    printf("(%lf, %lf)\n", x.mat[i][x.c - 1].Real, x.mat[i][x.c - 1].Imag);
  }
}

struct Matrix MatMul(struct Matrix a, struct Matrix b)
{
  int i, j, k;
  struct Matrix Tmp; Tmp.r = Tmp.c = 0;
  if (a.c != b.r) return Tmp; else Tmp.r = a.r, Tmp.c = b.c;

  for (i = 0; i < Tmp.r; i++)
    for (j = 0; j < Tmp.c; j++) Tmp.mat[i][j] = getAComplex(0, 0);

  for (i = 0; i < a.r; i++)
    for (j = 0; j < b.c; j++)
      for(k = 0; k < a.c; k++)
        Tmp.mat[i][j] = AddComplex(Tmp.mat[i][j], MutComplex(a.mat[i][k], b.mat[k][j]));
  return Tmp;
}

struct Matrix MatPow(struct Matrix a, int x)
{
  int i;

  struct Matrix Tmp; Tmp.r = Tmp.c = 0;
  if (a.c != a.r) return Tmp; else Tmp.r = a.r, Tmp.c = a.c;

  for (i = 0; i < a.r; i++) Tmp.mat[i][i] = getAComplex(1, 0);

  for (; x; x>>=1)
  {
    if (x & 1) Tmp = MatMul(Tmp, a);
    a = MatMul(a, a);
  }

  return Tmp;
}

void main(void)
{
  struct Complex a = getAComplex(1, 2);

  struct Complex c = SinComplex(a);
  printAComplex(c);
  
  RootComplex(a);
  printf("Power of %d = %lf\n", 3, QuickPower(3, 2));
  printf("Power of "); printAComplex(a); printf(" = "); printAComplex(QuickPowerComplex(a, 2)); puts("");

  struct Matrix Ma = getAMatrix(3, 1);
  struct Matrix Mb = getAMatrix(1, 3);
  struct Matrix Mc = getAMatrix(1, 1);
  printAMatrix(MatMul(Ma, Mb));
  printAMatrix(MatPow(Mc, 2));
}
