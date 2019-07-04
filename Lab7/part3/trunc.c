#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

void print(element_t ev) {
  char* e = ev;
  printf ("%s\n", e? e : "NULL");
}

//void printInt (element_t ev) {
//  int* e = ev;
//  printf ("%d\n", *e);
//}

void no_null_allowed (element_t* rv, element_t sv, element_t ev) {
  char **r = (char**) rv, *s = sv;
  int  *e  = ev;
  *r = *e<0? s : 0;
}

int no_negatives(element_t in) {
	intptr_t val = (intptr_t) in;
	if (val == -1 || val == 4294967295) {
		return 0;
	} else {
		return 1;
	}
}

int not_null (element_t e) {
  return e != NULL;
}

void truncate (element_t* rv, element_t sv, element_t ev) {
  char **r  = (char**) rv, *s = sv;
  int   *e  = ev;
  *r = strdup (s);
  if (strlen (*r) > *e)
    (*r) [*e] = 0;
}

void intValue (element_t* rv, element_t av) {
  int **r = (int**) rv;
  char *a = av, *e;
  *r = malloc(sizeof(int*));
  **r = strtol (a, &e, 0);
  if (*e)
    **r = -1;
}

void concat (element_t* rv, element_t av, element_t bv) {
  char **r = (char**) rv, *b = bv;
  *r = realloc(*r, strlen(*r) + strlen(b) + 2);
  if (strlen(*r))
    strcat(*r, " ");
  strcat(*r, b);
}

void max (element_t* rv, element_t av, element_t bv) {
  int **r = (int**) rv, *a = av, *b = bv;
  **r = *a > *b? *a: *b;
}

int main (int argc, char** argv) {

  struct list* l0 = list_create();
  for (int i=1; i<argc; i++)
    list_append (l0, argv [i]);

  struct list* l1 = list_create();
  list_map1 (intValue, l1, l0);

  struct list* l2 = list_create();
  list_map2 (no_null_allowed, l2, l0, l1);

  struct list* l3 = list_create();
  list_filter (no_negatives, l3, l1);

  struct list* l4 = list_create();
  list_filter (not_null, l4, l2);

  struct list* l5 = list_create();
  list_map2 (truncate, l5, l4, l3);

  list_foreach (print, l5);

  char* s = malloc(1);
  *s = 0;
  list_foldl (concat, (element_t*) &s, l5);
  printf ("%s\n", s);
  free (s);
  list_foreach (free, l5);

  int v = -1, *vp = &v;
  list_foldl (max, (element_t*) &vp, l3);
  list_foreach (free, l1);
  printf ("%d\n", v);

  list_destroy (l0);
  list_destroy (l1);
  list_destroy (l2);
  list_destroy (l3);
  list_destroy (l4);
  list_destroy (l5);
}