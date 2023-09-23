/* BOINGIU Keren Ioana - 312 CB */
#ifndef COADA_H
#define COADA_H

#include <stdio.h>
#include "lista-generica.h"

typedef struct
{
    Lista *l;
} Coada;

typedef void (*TFAfisare)(void*, FILE*);

// - functii coada
Coada *creare_coada();
void eliberare_coada(Coada *c);
void coada_push(Coada *c, void *data);
void *coada_pop(Coada *c);
void *coada_front(Coada *c);

// - functii ajutatoare coada
void eliberare_coada_elemente(Coada *c, TFElem eliberare_element);
void coada_push_sortat(Coada *c, void *data, TFCmp comparator);
void *coada_find_element(Coada *c, void *data, TFCmp comparator);
void coada_afisare(Coada *c, TFAfisare callback, FILE *out);

#endif // COADA_H