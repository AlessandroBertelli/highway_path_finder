#include <stdio.h>
#include <stdlib.h>

struct Stazione {
    int km;
    int numMacchine;
    int macchine;
    struct Stazione *next;
    struct Stazione *prev;
};

/*
struct Stazione *trovaStazione(struct Stazione **testa, struct Stazione **coda, int km) {
    if ((*testa)->km == km)
        return (*testa);
    if ((*testa)->km > km)
        return NULL;
    if ((*coda)->km == km)
        return (*coda);
    if ((*coda)->km < km) {
        return NULL;
    }

    struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));
    temp = *testa;

    while (temp->next != NULL) {
        temp = temp->next;
        if (temp->km == km){
            free(temp);
            return ;
        }
    }
    return NULL;
}*/

void insStazione(struct Stazione **testa, struct Stazione **coda, int km) {

    struct Stazione *newStazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->next = NULL;
    newStazione->prev = NULL;

    struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));

    if (*testa == NULL) {
        *testa = newStazione;
        *coda = newStazione;
        printf("aggiunta\n");
    } else if ((*testa)->km > newStazione->km) {
        newStazione->next = *testa;
        newStazione->next->prev = newStazione;
        *testa = newStazione;
        printf("aggiunta\n");
    } else {
        temp = *testa;

        while (temp->next != NULL && temp->next->km < newStazione->km) {
            temp = temp->next;
        }
        if (temp->next != NULL && temp->next->km == newStazione->km) {
            printf("non aggiunta\n");
            free(newStazione);
        } else {
            newStazione->next = temp->next;

            if (newStazione->next != NULL) {
                newStazione->next->prev = newStazione;
            } else {
                *coda = newStazione;
            }
            temp->next = newStazione;
            newStazione->prev = temp;
            printf("aggiunta\n");
        }
    }
}

void delStazione(struct Stazione **testa, struct Stazione **coda, int km) {

    if ((*testa)->km > km) {
        printf("non demolita");
        return;
    }
    if ((*coda)->km < km) {
        printf("non demolita");
        return;
    }

    int demolito = 0;
    struct Stazione *del_stazione = (struct Stazione *) malloc(sizeof(struct Stazione));

    if ((*testa)->km == km) {
        (*testa)->next->prev = NULL;
        *testa = (*testa)->next;
        demolito = 1;
    }
    if ((*coda)->km == km) {
        (*coda)->prev->next = NULL;
        *coda = (*coda)->prev;
        demolito = 1;
    }

    if (demolito == 0) {

        del_stazione = *testa;

        while (del_stazione->next != NULL && demolito == 0) {
            del_stazione = del_stazione->next;
            if (del_stazione->km == km) {
                demolito = 1;
            }
        }
    }

    if (*testa == NULL || demolito == 0) {
        printf("non demolita\n");
        return;
    }

    if (del_stazione->next != NULL) {
        del_stazione->next->prev = del_stazione->prev;
    }
    if (del_stazione->prev != NULL)
        del_stazione->prev->next = del_stazione->next;

    free(del_stazione);
    printf("demolita\n");
}

// print the doubly linked list
void displayList(struct Stazione *Stazione) {
    struct Stazione *last;

    while (Stazione != NULL) {
        printf("%d->", Stazione->km);
        last = Stazione;
        Stazione = Stazione->next;
    }
    if (Stazione == NULL)
        printf("NULL\n");
}

int main() {
    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;

    insStazione(&testa, &coda, 1);

    insStazione(&testa, &coda, 4);

    insStazione(&testa, &coda, 8);

    insStazione(&testa, &coda, 19);

    insStazione(&testa, &coda, 19);

    delStazione(&testa, &coda, 1);

    delStazione(&testa, &coda, 8);

    delStazione(&testa, &coda, 8);

    insStazione(&testa, &coda, 8);

    delStazione(&testa, &coda, 8);

}

