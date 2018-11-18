typedef struct {
  int x;
  int y;
} Data;

extern int add_int(int x, int y);
extern Data *new_data(int x, int y);
extern int sum_data(Data* d);
