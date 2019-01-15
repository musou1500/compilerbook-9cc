#include <time.h>
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

Complex *root_complex(Complex *x)
{
  float r = sqrt(sqr(x->real) + sqr(x->imag));
  float real = sqrt((r + x->real) / 2);
  float imag = sqrt((r - x->real) / 2);
  return new_complex(real, imag);
}


float qpower(float a, int x) {
  float tmp = 1;

  for (; x; x >>= 1)
  {
    if (x & 1) tmp *= a;
    a *= a;
  }

  return tmp;
}



Matrix *new_matrix(int row_len, int col_len, Complex ***data_raw)
{
  Matrix *mat = malloc(sizeof(Matrix));
  mat->r = row_len;
  mat->c = col_len;

  typedef Complex (**DATA_PTR)[row_len][col_len];
  DATA_PTR data = data_raw;

  for (int i = 0; i < row_len; i++) {
    for (int j = 0; j < col_len; j++) {
      mat->mat[i][j] = data[i][j];
    }
  }

  return mat;
}



Matrix *mat_mul(Matrix* a, Matrix* b)
{
  int tmp_r = 0;
  int tmp_c = 0;
  if (a->c != b->r) {
    return new_matrix(0, 0, NULL);
  } else {
    tmp_r = a->r;
    tmp_c = b->c;
  }

  Complex *data[10][10];
  for (int i = 0; i < tmp_r; i++) {
    for (int j = 0; j < tmp_c; j++) {
      data[i][j] = new_complex(0, 0);
    }
  }

  for (int i = 0; i < a->r; i++) {
    for (int j = 0; j < b->c; j++) {
      for(int k = 0; k < a->c; k++) {
        data[i][j] = add_complex(data[i][j], mut_complex(a->mat[i][k], b->mat[k][j]));
      }
    }
  }

  return new_matrix(tmp_r, tmp_c, (Complex ***)data);
}

Matrix *mat_pow(Matrix *a, int x)
{
  int tmp_r = 0;
  int tmp_c = 0;
  if (a->c != a->r) {
    return new_matrix(0, 0, NULL);
  } else {
    tmp_r = a->r;
    tmp_c = a->c;
  }
  
  Complex *data[10][10];
  for (int i = 0; i < a->r; i++){
    data[i][i] = new_complex(1, 0);
  }

  Matrix *tmp = new_matrix(tmp_r, tmp_c, (Complex ***)data);
  for (; x; x>>=1)
  {
    if (x & 1) {
      tmp = mat_mul(tmp, a);
    }

    a = mat_mul(a, a);
  }

  return tmp;
}


void print_matrix(Matrix *x)
{
  for (int i = 0; i < x->r; i++)
  {
    for (int j = 0; j < x->c - 1; j++) {
			printf("(%lf, %lf), ", x->mat[i][j]->real, x->mat[i][j]->imag);
		}

    printf("(%lf, %lf)\n", x->mat[i][x->c - 1]->real, x->mat[i][x->c - 1]->imag);
  }
}


float C_PI(int len)
{
  int numberator = 1, denominator = 3, result, carry;
  int flag = 1, count = 0;
  char *pi, *temp;

  len += 2;

  pi = (char*)malloc(len * sizeof(char));
  temp = (char*)malloc(len * sizeof(char));

  for(int i = 0; i < len; i++) {
    pi[i] = temp[i] = 0;
  }

  pi[1] = 2, temp[1] = 2;

  while (flag)
  {
    carry = 0;
    for(int i = len - 1; i > 0; i--)
    {
      result = temp[i] * numberator + carry;
      temp[i] = result % 10;
      carry = result / 10;
    }

    carry = 0;
    for(int i = 0; i < len; i++)
    {
      result = temp[i] + carry * 10;
      temp[i] = result / denominator;
      carry = result % denominator;
    }
    flag = 0;
    for(int i = len - 1; i > 0; i--)
    {
      result = pi[i] + temp[i];
      pi[i] = result % 10;
      pi[i-1] += result / 10;
      flag |= temp[i];
    }

    numberator++;
    denominator += 2;
  }

  float sum = 3;
  for(int i = 2; i < len; i++) {
    sum += 1.0 / qpower(10, i - 1) * (int)pi[i];
  }

  return sum;
}


#ifdef TEST
int main(int argc, const char *argv[])
{
	// 双曲線関数
  /* float sin_result = sinh(2); */
  /* float cos_result = cosh(2); */
  /* float tan_result = tanh(2); */
  /* printf("%lf\n", sin_result); */
  /* printf("%lf\n", cos_result); */
  /* printf("%lf\n", tan_result); */

	/* // 逆三角関数 */
  /* float arctan2_result = arctan2(new_complex(1, 2)); */
  /* printf("%lf\n", arctan2_result); // 7.389057 */

  /* // 平方根(実数) */
  /* float sqrt_result = sqrt(4); */
  /* printf("%lf\n", sqrt_result); */

  /* // 平方根(複素数) */
  /* Complex* root_result = root_complex(new_complex(1, 2)); */
  /* if (root_result->imag < 0) */
  /* { */
    /* printf("%lf + %lf i, ", -root_result->real, root_result->imag); */
    /* printf("%lf - %lf i\n", root_result->real, root_result->imag); */
  /* } */
  /* else */
  /* { */
    /* printf("%lf + %lf i, ", root_result->real, root_result->imag); */
    /* printf("%lf - %lf i\n", -root_result->real, root_result->imag); */
  /* } */

  /* // 冪演算 */
  /* float qpower_result = qpower(10, 2); */
  /* printf("qpower %lf\n", qpower_result); */


  /* // 行列の乗算 */
  /* printf("multiply matrix\n"); */
  /* Complex *mat_a_data[3][1] = { */
    /* { new_complex(2, 3) }, */
    /* { new_complex(5, 1) }, */
    /* { new_complex(7, 2) } */
  /* }; */
  /* Matrix *mat_a = new_matrix(3, 1, (Complex ***)mat_a_data); */
  /* printf("a:\n"); */
  /* print_matrix(mat_a); */
  /* Complex *mat_b_data[1][3] = { */
    /* { */
      /* new_complex(2, 3), */
      /* new_complex(5, 1), */
      /* new_complex(4, 3), */
    /* } */
  /* }; */
  /* Matrix *mat_b = new_matrix(1, 3, (Complex ***)mat_b_data); */
  /* printf("b:\n"); */
  /* print_matrix(mat_b); */
  clock_t start,end;
  start = clock();
  printf("%lf\n", C_PI(atoi(argv[1])));
  end = clock();
  printf("%lf秒かかりました\n",(double)(end-start)/CLOCKS_PER_SEC);

  /* print_matrix(mat_mul(mat_a, mat_b)); */

  
  // 行列の冪演算
  /* printf("power matrix\n"); */
  /* Complex *mat_c_data[1][1] = { */
    /* { new_complex(2, 3) } */
  /* }; */
  /* Matrix *mat_c = new_matrix(1, 1, (Complex ***)mat_c_data); */
  /* print_matrix(mat_pow(mat_c, 2)); */
  return 0;
}
#endif
