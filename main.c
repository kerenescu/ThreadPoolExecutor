/* BOINGIU Keren Ioana - 312 CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "coada.h"
#include "stiva.h"

// ---- Structuri date ----
typedef struct
{
    unsigned short ID;
} Thread;

typedef struct
{
    unsigned short ID;
    Thread* thread;
    unsigned char prioritate;
    unsigned int timp_executie;
    unsigned int timp_total;
} Task;

typedef struct
{
    int Q;
    Stiva *pool;
    Coada *waiting;
    Coada *running;
    Coada *finished;
    Coada *reciclare_id;
    int max_id;
    unsigned int timp_total;
} TPE; // ThreadPoolExecutor


// ---- Initializare structuri ----
Task *creare_task(int ID, int prioritate, int timp_executie)
{
    Task *t = (Task *)malloc(sizeof(Task));
    t->ID = ID;
    t->prioritate = prioritate;
    t->timp_executie = timp_executie;
    t->timp_total = timp_executie;
    t->thread = NULL;
    return t;
}

Thread *creare_thread(int ID)
{
    Thread *t = (Thread *)malloc(sizeof(Thread));
    t->ID = ID;
    return t;
}

TPE *init_planificator(int C, int Q)
{
    // initializare structura container
    TPE *p = (TPE*)malloc(sizeof(TPE));
    p->Q = Q;
    p->max_id = 1;
    p->timp_total = 0;

    // initializare structuri date
    p->pool = creare_stiva();
    p->waiting = creare_coada();
    p->running = creare_coada();
    p->finished = creare_coada();
    p->reciclare_id = creare_coada();

    // alocare thread-uri
    int N = C * 2;
    int i;
    for (i = N - 1; i >= 0; i--) {
        stiva_push(p->pool, creare_thread(i));
    }

    return p;
}

void eliberare_element(void *data)
{
    free(data);
}

void eliberare_task(void *data)
{
    Task *t = (Task*)data;
    if (t->thread != NULL) {
        free(t->thread);
    }
    free(t);
}

void eliberare_planificator(TPE *p)
{
    eliberare_coada_elemente(p->waiting, eliberare_task);
    eliberare_coada_elemente(p->running, eliberare_task);
    eliberare_coada_elemente(p->finished, eliberare_task);
    eliberare_stiva_elemente(p->pool, eliberare_element);
    eliberare_coada_elemente(p->reciclare_id, eliberare_element);
    free(p);
}


// ---- Adaugare task-uri
int gasire_task_id(TPE *p)
{
    int res;

    if(coada_front(p->reciclare_id) != NULL) {
        res = *(int*)coada_front(p->reciclare_id);
        free(coada_pop(p->reciclare_id));
    } else {
        res = p->max_id;
        p->max_id++;
    }

    return res;
}

int cmp_taskuri(void *a, void *b)
{
    Task *t1 = (Task*) a;
    Task *t2 = (Task*) b;

    if(t1->prioritate > t2->prioritate) {
        return -1;
    } else if (t1->prioritate < t2->prioritate) {
        return 1;
    }

    if(t1->timp_executie < t2->timp_executie) {
        return -1;
    } else if (t1->timp_executie > t2->timp_executie) {
        return 1;
    }

    if(t1->ID < t2->ID) {
        return -1;
    } else if (t1->ID > t2->ID) {
        return 1;
    }

    return 0;
}

void adaugare_taskuri(TPE *p, int n, int prioritate,
    int timp_executie, FILE *out)
{
    int i;
    for (i = 0; i < n; i++) {
        Task *t = creare_task(gasire_task_id(p), prioritate,
            timp_executie);
        coada_push_sortat(p->waiting, t, cmp_taskuri);
        fprintf(out, "Task created successfully : ID %hu.\n", t->ID);
    }
}


// ---- Get task ----
int cmp_id_task(void *a, void *b)
{
    Task *t1 = (Task*) a;
    Task *t2 = (Task*) b;
    
    return t1->ID != t2->ID;
}

void get_task(TPE *p, int ID, FILE *out)
{
    Task *t;
    Task *task_id = creare_task(ID, 0, 0);
    int gasit = 0;
    
    // cautare in coada de running
    t = coada_find_element(p->running, task_id, cmp_id_task);
    if (t != NULL) {
        fprintf(out, "Task %hu is running (remaining_time = %u).\n", t->ID,
            t->timp_executie);
        gasit = 1;
    }

    // cautare in coada de waiting
    t = coada_find_element(p->waiting, task_id, cmp_id_task);
    if (t != NULL) {
        fprintf(out, "Task %hu is waiting (remaining_time = %u).\n", t->ID,
            t->timp_executie);
        gasit = 1;
    }

    // cautare in coada de finished
    t = coada_find_element(p->finished, task_id, cmp_id_task);
    if (gasit == 0 && t != NULL) {
        fprintf(out, "Task %hu is finished (executed_time = %u).\n", t->ID,
            t->timp_total);
        gasit = 1;
    }

    // daca task-ul nu a fost gasit in nicio coada
    if (!gasit) {
        fprintf(out, "Task %hu not found.\n", task_id->ID);
    }

    free(task_id);
}


// ---- Get thread ----
int cmp_id_thread_idle(void *a, void *b)
{
    Thread *t1 = (Thread*) a;
    Thread *t2 = (Thread*) b;
    
    return t1->ID != t2->ID;
}

int cmp_id_thread_activ(void *a, void *b)
{
    Thread *t1 = (Thread*) a;
    Task *t2 = (Task*) b;
    
    return t1->ID != t2->thread->ID;
}

void get_thread(TPE *p, int ID, FILE *out)
{
    Thread *th = NULL;
    Task *task = NULL;
    Thread *thread_id = creare_thread(ID);

    // cautare in pool threaduri
    th = stiva_find_element(p->pool, thread_id, cmp_id_thread_idle);
    if (th != NULL) {
        fprintf(out, "Thread %hhu is idle.\n", th->ID);
    }

    // cautare in coada running
    if (th == NULL) {
        task = coada_find_element(p->running, thread_id, cmp_id_thread_activ);
    }

    if (task != NULL) {
        th = task->thread;
        fprintf(out, "Thread %hhu is running task %hhu ", th->ID, task->ID);
        fprintf(out, "(remaining_time = %u).\n", task->timp_executie);
    }

    free(thread_id);
}


// ---- Afisare cozi ----
void afisare_task_waiting(void *data, FILE *out)
{
    Task *t = (Task*)data;
    fprintf(out, "(%hu: priority = %hhu, remaining_time = %u)", t->ID,
        t->prioritate, t->timp_executie);
}

void afisare_task_running(void *data, FILE *out)
{
    Task *t = (Task*)data;
    fprintf(out, "(%hu: priority = %hhu, remaining_time = %u,", t->ID,
        t->prioritate, t->timp_executie);
    fprintf(out, " running_thread = %hhu)", t->thread->ID);
}

void afisare_task_finished(void *data, FILE *out)
{
    Task *t = (Task*)data;
    fprintf(out, "(%hu: priority = %hhu, executed_time = %u)", t->ID,
        t->prioritate, t->timp_total);
}

void print_waiting(TPE *p, FILE *out)
{
    fprintf(out, "====== Waiting queue =======\n");
    coada_afisare(p->waiting, afisare_task_waiting, out);
}

void print_running(TPE *p, FILE *out)
{
    fprintf(out, "====== Running in parallel =======\n");
    coada_afisare(p->running, afisare_task_running, out);
}

void print_finished(TPE *p, FILE *out)
{
    fprintf(out, "====== Finished queue =======\n");
    coada_afisare(p->finished, afisare_task_finished, out);
}


// ---- Rulare task-uri ----
int cmp_id(void * a, void *b)
{
    int id1 = *(int *)a;
    int id2 = *(int *)b;

    return id1 - id2;
}

void finalizare_task(TPE *p, Task *t)
{
    // adaugare la coada finished
    coada_push(p->finished, t);

    // eliberare id task
    int *val = malloc(sizeof(int));
    *val = t->ID;
    coada_push_sortat(p->reciclare_id, val, cmp_id);

    // marcare thread ca disponibil
    stiva_push(p->pool, t->thread);
    t->thread = NULL;
    t->timp_executie = 0;
}

void rulare_cuanta(TPE *p, unsigned int timp)
{
    Coada *tmp = creare_coada();

    // verificare task-uri
    unsigned int max_run_time = 0;
    while (coada_front(p->running) != NULL) {
        Task *t = coada_pop(p->running);

        if (t->timp_executie > max_run_time) {
            max_run_time = t->timp_executie;
        }
        
        // verificare task finalizat
        if (t->timp_executie <= timp) {
            finalizare_task(p, t);
        } else {
            t->timp_executie -= timp;
            coada_push(tmp, t);
        }
    }

    if (max_run_time < timp && coada_front(p->waiting) == NULL && coada_front(p->running) == NULL) {
        p->timp_total += max_run_time;
    } else if (max_run_time != 0) {
        p->timp_total += timp;
    }

    // refacere coada task-uri
    while (coada_front(tmp) != NULL) {
        coada_push(p->running, coada_pop(tmp));
    }
    eliberare_coada(tmp);

    // planificare task-uri suplimentare daca este posibil
    while (coada_front(p->waiting) != NULL && stiva_front(p->pool) != NULL) {
        Task *t = coada_pop(p->waiting);
        t->thread = stiva_pop(p->pool);
        coada_push_sortat(p->running, t, cmp_taskuri);
    }
}

void run(TPE *p, int T, FILE *out)
{
    fprintf(out, "Running tasks for %d ms...\n", T);

    // if(p->timp_total == 0) {
        rulare_cuanta(p, 0);
    // }

    while (T > p->Q) {
        rulare_cuanta(p, p->Q);
        T -= p->Q;
    }

    rulare_cuanta(p, T);
}

void finish(TPE *p, FILE *out)
{
    // cat timp mai avem task-uri
    while (coada_front(p->waiting) != NULL ||
        coada_front(p->running) != NULL) {
        rulare_cuanta(p, p->Q);
    }
    fprintf(out, "Total time: %u", p->timp_total);
}


int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Argumente program incorecte.\n");
        exit(EXIT_FAILURE);
    }

    // deschidere fisiere
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (in == NULL || out == NULL) {
        fprintf(stderr, "Eroare deschidere fisiere input / output.\n");
        exit(EXIT_FAILURE);
    }

    int Q, C;
    fscanf(in, "%d", &Q);
    fscanf(in, "%d", &C);

    TPE *p = init_planificator(C, Q);
    char *comanda = (char*)malloc(256 * sizeof(char));
    int done = 0;
    while (!done) {
        if (fscanf(in, "%s", comanda) == EOF) {
            done = 1;
            break;
        }

        if (strcmp(comanda, "add_tasks") == 0) {
            int n, timp, prioritate;
            fscanf(in, "%d", &n);
            fscanf(in, "%d", &timp);
            fscanf(in, "%d", &prioritate);
            adaugare_taskuri(p, n, prioritate, timp, out);
        } else if (strcmp(comanda, "get_task") == 0) {
            int id;
            fscanf(in, "%d", &id);
            get_task(p, id, out);
        } else if (strcmp(comanda, "get_thread") == 0) {
            int id;
            fscanf(in, "%d", &id);
            get_thread(p, id, out);
        } else if (strcmp(comanda, "print") == 0) {
            fscanf(in, "%s", comanda);

            if (strcmp(comanda, "waiting") == 0) {
                print_waiting(p, out);
            } else if (strcmp(comanda, "running") == 0) {
                print_running(p, out);
            } else if (strcmp(comanda, "finished") == 0) {
                print_finished(p, out);
            }
        } else if (strcmp(comanda, "run") == 0) {
            int timp;
            fscanf(in, "%d", &timp);
            run(p, timp, out);
        } else if (strcmp(comanda, "finish") == 0) {
            finish(p, out);
            done = 1;
        }
    }
    free(comanda);
    eliberare_planificator(p);
    fclose(in);
    fclose(out);
    return 0;
}