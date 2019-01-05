#include <stdio.h>
#include <stdlib.h>
#include "./lib.h"

#define sqr(x) ((x)*(x))
#define e 2.718281
#define PI 3.141592
#define PRECISION 1e-6


/**
 * 複素数オブジェクトを生成
 */
Complex* new_complex(int real, int imag) {
  Complex *node = malloc(sizeof(Complex));
  
  // 実部
  node->real = real;

  // 虚部
  node->imag = imag;
  return node;
}

/**
 * complex(複素数) の内容を
 * "実部 + 虚部 i" の形式で表示する
 */
void print_complex(Complex* x) {
  printf("%lf + %lf i\n", x->real, x->imag);
}

/**
 * 複素数同士の加算を行う
 */
Complex *add_complex(const Complex* x, const Complex* y)
{
  return new_complex(x->real + y->real, x->imag + y->imag);
}

/**
 * 複素数同士の減算を行う
 */
Complex *sub_complex(const Complex *x, const Complex *y)
{
  return new_complex(x->real - y->real, x->imag - y->imag);
}

/**
 * 複素数同士の乗算を行う
 */
Complex *mut_complex(const Complex *x, const Complex *y)
{
  float real = x->real * y->real - x->imag * y->imag;
  float imag = x->real * y->imag + y->real * x->imag;

  return new_complex(real, imag);
}

/**
 * 複素数同士の除算を行う
 */
Complex *div_complex(const Complex *x, const Complex *y)
{
  float real = (x->real * y->real + x->imag * y->imag) / (sqr(y->real) + sqr(y->imag));
  float imag = (x->imag * y->real - x->real * y->imag) / (sqr(y->real) + sqr(y->imag));

  return new_complex(real, imag);
}

/**
 * 指数関数 e^xを計算する
 */
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

/**
 * hyperbolic sinを計算する
 */
float sinh(const float x) {
  float hype = exp(x);
  return (hype - 1 / hype) / 2;
}

/**
 * hyperbolic cosを計算する
 */
float cosh(const float x) {
  float hype = exp(x); return (hype + 1 / hype) / 2;
}

/**
 * hyperbolic tanを計算する
 */
float tanh(const float x) {
  float hype = exp(x);
  return (hype - 1 / hype) / (hype + 1 / hype);
}


float sin(float x) //sin関数
{
  float t, p, z = 1, s = 0, u = 1;
  int i, count = 0;
  float y = x * PI / 180;//ラジアンは角度に転換

  // sin(x) = x - (x^3 / 3!) + (x^5 / 5!) - (x^7 / 7!) +...
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

/**
 * cosを計算する
 */
float cos(float x)
{
  float t, p, z = 1, s = 0, u = 1;
  int i, count=0;

  float y = (x * PI) / 180; //ラジアンは角度に転換
  
  // cos(x) = x - (x^2 / 2!) + (x^4 / 4!) - (x^6 / 6!) +...
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

/**
 * tanを計算する
 */
float tan(const float x) { return sin(x) / cos(x); }

/**
 * 自然対数log e(x) を計算する
 */
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

/**
 * xの平方根を計算する
 */
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

/**
 * arctanを計算する
 */
float arctan(float x)
{
  int i, count = 0, z = 1;
  float a = x, b = 1, sum = 0, item = 1;

  // arctan(x) = x − (x^3 / 3) + (x^5 / 5) − (x^7 / 7) + ...
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

/**
 * 複素数に対しarctanを計算する
 */
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

/**
 * 複素数に対して sinを計算する
 */
Complex *sin_complex(const Complex *x)
{
  float real = sin(x->real) * cosh(x->imag);
  float imag = cos(x->real) * sinh(x->imag);

  return new_complex(real, imag);
}

/**
 * 複素数に対して cos を計算する
 */
Complex *cos_complex(const Complex *x)
{
  float real = cos(x->real) * cosh(x->imag);
  float imag = - sin(x->real) * sinh(x->imag);

  return new_complex(real, imag);
}

/**
 * 複素数に対して tanを計算する
 */
Complex *tan_complex(const Complex *x)
{
  Complex* nume = new_complex(tan(x->real), tanh(x->imag));
  Complex* deno = new_complex(1, -tan(x->real) * tanh(x->imag));
  return div_complex(nume, deno);
}

/**
 * 複素数xについて，指数関数 e^xを計算する
 */
Complex *exp_complex(const Complex *x)
{
  float hype = exp(x->real);
  float real = hype * cos(x->imag);
  float imag = hype * sin(x->imag);
  return new_complex(real, imag);
}

/**
 * 複素数に対して 自然対数 log e(x) を計算する
 */
Complex *ln_complex(const Complex *x)
{
  float real = ln(sqrt(sqr(x->real) + sqr(x->imag)));
  float imag = arctan2(x);
  return new_complex(real, imag);
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
