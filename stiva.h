/* BOINGIU Keren Ioana - 312 CB */
#ifndef STIVA_H
#define STIVA_H

#include "lista-generica.h"

typedef struct
{
    Lista *l;
} Stiva;


// - functii stiva
Stiva *creare_stiva();
void eliberare_stiva(Stiva *s);
void stiva_push(Stiva *s, void *data);
void *stiva_pop(Stiva *s);
void *stiva_front(Stiva *s);

// - functii ajutatoare stiva
void eliberare_stiva_elemente(Stiva *s, TFElem eliberare_element);
void *stiva_find_element(Stiva *s, void *data, TFCmp comparator);

#endif // STIVA_H