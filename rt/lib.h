typedef struct {
  int x;
  int y;
} Coord;

typedef struct {
  float real, imag;
} Complex;

int add_int(int x, int y);
Coord *new_coord(int x, int y);
Coord *sum_coord(Coord*, Coord*);
void print_coord(Coord* c);

Complex *new_complex(int real, int imag);
void print_complex(Complex *x);
Complex *add_complex(const Complex *x, const Complex *y);
Complex *sub_complex(const Complex *x, const Complex *y);
Complex *mut_complex(const Complex *x, const Complex *y);
Complex *div_complex(const Complex *x, const Complex *y);


double Sin(double x);
double Cos(double x);
double Tan(double x);

float exp(float x);
float sinh(const float x);
float cosh(const float x);
float tanh(const float x);
float sin(float x);
float cos(float x);
float tan(const float x);
float ln(float x);
float sqrt(float x);
float arctan(float x);
float arctan2(const Complex *x);

Complex *sin_complex(const Complex *x);
Complex *cos_complex(const Complex *x);
Complex *tan_complex(const Complex *x);
Complex *exp_complex(const Complex *x);
Complex *ln_complex(const Complex *x);

