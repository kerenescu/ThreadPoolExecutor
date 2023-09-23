/* BOINGIU Keren Ioana - 312 CB */
#ifndef LISTA_GENERICA_H
#define LISTA_GENERICA_H

typedef struct Nod Nod;
struct Nod
{
    void *data;
    Nod *next;
    Nod *prev;
};

typedef struct
{
    Nod *start;
    Nod *end;
    int len;
} Lista;

typedef void (*TFElem)(void*);
typedef int (*TFCmp)(void*, void*);

Lista *creare_lista();
void eliberare_lista(Lista *l);
void inserare_inceput(Lista *l, void *data);
void inserare_final(Lista *l, void *data);
void eliminare_inceput(Lista *l);
void eliminare_final(Lista *l);
void *preluare_data(Nod *n);

#endif // LISTA_GENERICA_H