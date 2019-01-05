#include <stdio.h>
#include <stdlib.h>
#include "./lib.h"

#define sqr(x) ((x)*(x))
#define e 2.718281
#define PI 3.141592
#define PRECISION 1e-6

Complex* new_complex(int real, int imag) {
  Complex *node = malloc(sizeof(Complex));
  node->real = real;
  node->imag = imag;
  return node;
}

void print_complex(Complex* x) {
  printf("%lf + %lf i\n", x->real, x->imag);
}

Complex *add_complex(const Complex* x, const Complex* y)
{
  return new_complex(x->real + y->real, x->imag + y->imag);
}

Complex *sub_complex(const Complex *x, const Complex *y)
{
  return new_complex(x->real - y->real, x->imag - y->imag);
}

Complex *mut_complex(const Complex *x, const Complex *y)
{
  float real = x->real * y->real - x->imag * y->imag;
  float imag = x->real * y->imag + y->real * x->imag;

  return new_complex(real, imag);
}

Complex *div_complex(const Complex *x, const Complex *y)
{
  float real = (x->real * y->real + x->imag * y->imag) / (sqr(y->real) + sqr(y->imag));
  float imag = (x->imag * y->real - x->real * y->imag) / (sqr(y->real) + sqr(y->imag));

  return new_complex(real, imag);
}

float exp(float x) {
  float a = 1, b = 1, sum = 0, item = 1;
  int count = 0;

  while (item > PRECISION)
  {
    sum += (item = b / a);
    a *= (++count), b *= x;
  }

  return sum;
}

float sinh(const float x) {
  float hype = exp(x);
  return (hype - 1 / hype) / 2;
}

float cosh(const float x) {
  float hype = exp(x); return (hype + 1 / hype) / 2;
}

float tanh(const float x) {
  float hype = exp(x);
  return (hype - 1 / hype) / (hype + 1 / hype);
}

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


float sin(float x) //sin関数
{
  float t, p, z = 1, s = 0, u = 1;
  int i, count = 0;
  float y = x * PI / 180;//ラジアンは角度に転換
  while (u > PRECISION)
  {
    t = 1; //初期化する
    for (i = 1; i <= (2 * count + 1); i++) t *= i; //テーラー展開分母部分
    // t = Fac[(count<<1)+1];
    p = 1; //初期化する
    for (i = 1; i <= (2 * count + 1); i++) p *= y;//テーラー展開分子部分

    u = p / t;
    s += z * u;//アイテムプラス
    z *= -1;
    count++;//次ぎのアイテム
  }
  return s;
}

float cos(float x)
{
  float t, p, z = 1, s = 0, u = 1;
  int i, count=0;

  float y = (x * PI) / 180; //ラジアンは角度に転換
  while (u > PRECISION)
  {
    t = 1; //初期化
    p = 1; //初期化
    for (i = 1; i <= (2 * count); i++) t*=i; //テーラー展開分母部分
    // t = Fac[count<<1];
    for (i = 1; i <= (2 * count); i++) p *= y; //テーラー展開分子部分

    u = p / t;
    s += z * u; //アイテムプラス
    z *= -1;
    count++; //次ぎのアイテム
  }
  return s;
}

float tan(const float x) { return sin(x) / cos(x); }

float ln(float x)
{
  float a = 1, b = 1, sum = 0, item = 1, y = (x - 1) / (x + 1);
  int i, count = 0;
  if (x <= 0) {
    printf("error");
    return 0;
  } else {
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

float sqrt(float x)
{
  float k = x;
  while (1)
  {
    if ((sqr(k) > x - PRECISION) && (sqr(k) < x + PRECISION)) break;
    k = 0.5 * (k + x / k);
  }
  return k;
}

float arctan(float x)
{
  int i, count = 0, z = 1;
  float a = x, b = 1, sum = 0, item = 1;
  if ((x > 1) || (x < -1)) {
    printf("error");
    return 0;
  } else {
    while (item > PRECISION) {
      sum += (z * a / b);
      z = z * -1;
      b = 2 * (++count) + 1;
      for (i = 2 * (count - 1) + 2; i <= 2 * count + 1; i++) a *= x;
      item = a / b;
    }
  }

  return sum;
}

float arctan2(const Complex* x) {
  float Res;

  if ((!x->real) && (!x->imag)) { printf("Error\n"); return 0; }
  if (!x->real)
    if (x->imag > 0) Res = PI / 2;
    else Res = - PI / 2;
  else
  {
    float l = x->imag / x->real;
    if (x->real > 0) Res = arctan(l);
    else
      if (x->imag >= 0) Res = arctan(l) + PI;
      else Res = arctan(l) -PI; 
  }

  return Res;
}

Complex *sin_complex(const Complex *x)
{
  float real = sin(x->real) * cosh(x->imag);
  float imag = cos(x->real) * sinh(x->imag);

  return new_complex(real, imag);
}

Complex *cos_complex(const Complex *x)
{
  float real = cos(x->real) * cosh(x->imag);
  float imag = - sin(x->real) * sinh(x->imag);

  return new_complex(real, imag);
}

Complex *tan_complex(const Complex *x)
{
  Complex* nume = new_complex(tan(x->real), tanh(x->imag));
  Complex* deno = new_complex(1, -tan(x->real) * tanh(x->imag));
  return div_complex(nume, deno);
}

Complex *exp_complex(const Complex *x)
{
  float hype = exp(x->real);
  float real = hype * cos(x->imag);
  float imag = hype * sin(x->imag);
  return new_complex(real, imag);
}

Complex *ln_complex(const Complex *x)
{
  float real = ln(sqrt(sqr(x->real) + sqr(x->imag)));
  float imag = arctan2(x);
  return new_complex(real, imag);
}

int add_int(int x, int y) {
  return x + y;
}

Coord *new_coord(int x, int y) {
  Coord* d = malloc(sizeof(Coord));
  d->x = x;
  d->y = y;
  return d;
}

Coord *sum_coord(Coord *a, Coord* b) {
  return new_coord(a->x + b->x, a->y + b->y);
}

void print_coord(Coord* c) {
  printf("%d %d\n", c->x, c->y);
}

int main(int argc, const char *argv[])
{
  double res_sin = Sin(2.0);
  double res_cos = Cos(2.0);
  double res_tan = Tan(2.0);
  printf("sin(2) = %lf\n", res_sin);
  printf("cos(2) = %lf\n", res_cos);
  printf("tan(2) = %lf\n", res_tan);
  return 0;
}
