/* BOINGIU Keren Ioana - 312 CB */
#include "coada.h"
#include <stdlib.h>
#include <stdio.h>


// ---- Functii coada ----
Coada *creare_coada()
{
    Coada *c = (Coada *)malloc(sizeof(Coada));
    c->l = creare_lista();
    return c;
}

void eliberare_coada(Coada *c)
{
    while (coada_front(c) != NULL) {
        coada_pop(c);
    }
    eliberare_lista(c->l);
    free(c);
}

void coada_push(Coada *c, void *data)
{
    inserare_final(c->l, data);
}

void *coada_pop(Coada *c)
{
    void *data = coada_front(c);
    eliminare_inceput(c->l);
    return data;
}

void *coada_front(Coada *c)
{
    return preluare_data(c->l->start);
}


// ---- Functii ajutatoare coada ----
void eliberare_coada_elemente(Coada *c, TFElem eliberare_element)
{
    while (coada_front(c) != NULL) {
        eliberare_element(coada_pop(c));
    }
    eliberare_coada(c);
}

void coada_push_sortat(Coada *c, void *data, TFCmp comparator)
{
    Coada *tmp = creare_coada();
    
    // cautare pozitie potrivita
    while (coada_front(c) != NULL && comparator(data, coada_front(c)) >= 0) {
        coada_push(tmp, coada_pop(c));
    }

    // inseram elementul la pozitia potrivita
    coada_push(tmp, data);

    // mutam totul in coada temporara
    while (coada_front(c) != NULL) {
        coada_push(tmp, coada_pop(c));
    }

    // mutam toate elementele in coada originala
    while (coada_front(tmp) != NULL) {
        coada_push(c, coada_pop(tmp));
    }

    eliberare_coada(tmp);
}

void *coada_find_element(Coada *c, void *data, TFCmp comparator)
{
    Coada *tmp = creare_coada();
    void *result = NULL;

    while (coada_front(c) != NULL) {
        if (result == NULL && comparator(data, coada_front(c)) == 0) {
            result = coada_front(c);
        }

        coada_push(tmp, coada_pop(c));
    }

    while (coada_front(tmp) != NULL) {
        coada_push(c, coada_pop(tmp));
    }

    eliberare_coada(tmp);
    return result;
}

void coada_afisare(Coada *c, TFAfisare afisare_elem, FILE *out)
{
    Coada *tmp = creare_coada();

    fprintf(out, "[");
    while (coada_front(c) != NULL) {
        afisare_elem(coada_front(c), out);
        coada_push(tmp, coada_pop(c));

        if(coada_front(c) != NULL) {
            fprintf(out, ",\n");
        }
    }
    fprintf(out, "]\n");

    while (coada_front(tmp) != NULL) {
        coada_push(c, coada_pop(tmp));
    }

    eliberare_coada(tmp);
}