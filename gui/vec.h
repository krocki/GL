#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union entry {
  void   * p;
  int    * i;
  float  * f;
  char   ** s;
} entry;

#define xstr(s) str(s)
#define str(s) #s

#define VEC_MIN_CAP 8

typedef struct node {
  void *x;
  struct node *l,*r;
} bin_tree;

typedef struct gen {
  void *data;
  int deepcopy;
  char *typename;
  size_t typesize;
  void (*fun)(struct gen*);
} gen;

typedef struct vec {
  entry *arr;
  size_t cap;
  size_t len;
} vec;

//void fun(struct gen *g) {
//  printf("fun(), g=%p;\n", g);
//}

void print(gen *g) {
  printf("g: size=%zu, %p, %p, %s, %d\n",
    g->typesize, g->data, g->fun, g->typename, g->deepcopy);
  if (NULL != g->fun) (g->fun)(g);
}

void print_vec(vec *v) {
  printf("vptr = %p\n", v);
  if (NULL == v) return;
  printf("v->arr=%p, v->cap=%zu, v->len=%zu\n",
    v->arr, v->cap, v->len
  );
  for (size_t i = 0; i < v->len; i++) {
    printf("%zu: %p (%s)\n", i, v->arr[i].p, *v->arr[i].s);
  }
}

void init(vec *v) {
  v->cap = VEC_MIN_CAP;
  v->arr = calloc(v->cap, sizeof(*v->arr));
  v->len = 0;
}

void release(vec *v) {
  v->len = 0, v->cap = 0;
  free(v->arr), v->arr = NULL;
}

#define push_back(v_ptr, d_ptr) \
  append(v_ptr, d_ptr, str(v_ptr), str(d_ptr), sizeof *(d_ptr)[0], sizeof *d_ptr);

void append(vec *v, const void *data, const char *arg0, const char *arg1, const size_t arg1_size, const size_t arg1_len) {

  printf("append: %s %s size of elem: %zu %zu elems\n", arg0, arg1, arg1_size, arg1_len/arg1_size);

  if (v->len == v->cap) {
    v->cap *= 2;
    v->arr = realloc(v->arr, v->cap * sizeof(*v->arr));
  }

  v->arr[v->len++].p = (void *)data;
}

//void print_tree(bin_tree *bt) {
//  printf("bintree %p\n", bt);
//}

//#define print(x) _Generic((x), \
//  int: printf("int: %d\n", x), \
//  bin_tree: print_tree(&x), \
//  default: printf("def\n") \
//)

#define typename(x) _Generic((x), \
  int: "int", \
  float: "float", \
  vec: "vec", \
  default: "other")

#define G(x) (gen){ \
  .typesize=sizeof((x)),\
  .data=&(x), \
  .typename=typename((x)), \
  .deepcopy=0 }

void cpy(gen *g) {
  void *tmp = g->data;
  g->data = malloc(g->typesize);
  memcpy(g->data, tmp, g->typesize);
  g->deepcopy = 1;
}

//int main(int argc, char **argv) {
//
//  puts("main");
//
//  vec v0 = {0}; vec v1 = {0}; vec v2 = {0};
//
//  print_vec(&v0); print_vec(&v1); print_vec(&v2);
//
//  init(&v0); init(&v1); init(&v2);
//
//  print_vec(&v0); print_vec(&v1); print_vec(&v2);
//
//  int arr0[] = { 4, 5, 3, 2, 3, 9 };
//  int arr1[] = { 8, 6, 5 };
//  int arr2[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
//
//  push_back(&v0, &arr0);
//  push_back(&v1, &arr1);
//  push_back(&v2, &arr2);
//
//  print_vec(&v0); print_vec(&v1); print_vec(&v2);
//
//  release(&v0); release(&v1); release(&v2);
//  print_vec(&v0); print_vec(&v1); print_vec(&v2);
//
//  //gen g;
//
//  //bin_tree n;
//  //{
//  //int x = 8;
//  ////print(x);
//
//  ////print(n);
//
//  //printf("x: %p, *x = %d\n", &x, x);
//
//  //g = G(x);
//  //print(&g);
//  //}
//
//  //cpy(&g);
//  //print(&g);
//  //void *buf = cpy(x);
//  //printf("buf: %p, *buf = %d\n", buf, *(int*)buf));
//
//  //print(&g);
//
//  //append(&v, &x);
//
//  //int x = 3;
//  //append(&v0, &x);
//  //append(&v0, &x);
//  //append(&v0, &x);
//
//  //print_vec(&v0);
//
//  return 0;
//
//}
