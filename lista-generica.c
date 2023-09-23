/* BOINGIU Keren Ioana - 312 CB */
#include "lista-generica.h"
#include <stdlib.h>

Lista *creare_lista()
{
    Lista *l = (Lista *)malloc(sizeof(Lista));
    l->len = 0;
    l->start = l->end = NULL;
    return l;
}

void eliberare_lista(Lista *l)
{
    Nod *el = l->start;
    while (el != NULL) {
        Nod *del = el;
        el = el->next;
        free(del);
    }

    free(l);
}

void inserare_inceput(Lista *l, void *data)
{
    Nod *el = (Nod *)malloc(sizeof(Nod));
    el->data = data;
    el->prev = NULL;
    el->next = l->start;
    
    if(l->end == NULL) {
        l->end = el;
    } else {
        l->start->prev = el;
    }
    l->start = el;

    l->len++;
}

void inserare_final(Lista *l, void *data)
{
    Nod *el = (Nod *)malloc(sizeof(Nod));
    el->data = data;
    el->next = NULL;
    el->prev = l->end;
    
    if(l->start == NULL) {
        l->start = el;
    } else {
        l->end->next = el;
    }
    l->end = el;
    
    l->len++;
}

void eliminare_inceput(Lista *l)
{
    if (l->start == NULL) {
        return;
    }

    Nod *del = l->start;
    l->start = l->start->next;

    if(l->start != NULL) {
        l->start->prev = NULL;
    } else {
        l->end = NULL;
    }

    l->len--;
    free(del);
}

void eliminare_final(Lista *l)
{
    if (l->end == NULL) {
        return;
    }

    Nod *del = l->end;
    l->end = l->end->prev;

    if(l->end != NULL) {
        l->end->next = NULL;
    } else {
        l->start = NULL;
    }

    l->len--;
    free(del);
}

void *preluare_data(Nod *n)
{
    return n != NULL ? n->data : NULL;
}
