/* BOINGIU Keren Ioana - 312 CB */
#include "stiva.h"
#include <stdlib.h>

Stiva *creare_stiva()
{
    Stiva *s = (Stiva *)malloc(sizeof(Stiva));
    s->l = creare_lista();
    return s;
}

void eliberare_stiva(Stiva *s)
{
    while (stiva_front(s) != NULL) {
        stiva_pop(s);
    }
    eliberare_lista(s->l);
    free(s);
}

void stiva_push(Stiva *s, void *data)
{
    inserare_final(s->l, data);
}

void *stiva_pop(Stiva *s)
{
    void *data = stiva_front(s);
    eliminare_final(s->l);
    return data;
}

void *stiva_front(Stiva *s)
{
    return preluare_data(s->l->end);
}


// ---- Functii ajutatoare stiva ----
void eliberare_stiva_elemente(Stiva *s, TFElem eliberare_element)
{
    while (stiva_front(s) != NULL) {
        eliberare_element(stiva_pop(s));
    }
    eliberare_stiva(s);
}

void *stiva_find_element(Stiva *s, void *data, TFCmp comparator)
{
    Stiva *tmp = creare_stiva();
    void *result = NULL;

    while (stiva_front(s) != NULL) {
        if (comparator(data, stiva_front(s)) == 0) {
            result = stiva_front(s);
        }

        stiva_push(tmp, stiva_pop(s));
    }

    while (stiva_front(tmp) != NULL) {
        stiva_push(s, stiva_pop(tmp));
    }

    eliberare_stiva(tmp);
    return result;
}
