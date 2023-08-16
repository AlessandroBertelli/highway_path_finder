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
        if (temp->km == newStazione->km) {
            printf("non aggiunta\n");
            //free(newStazione);
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
    free(temp);
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
    struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));
    struct Stazione *del_stazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    temp = *testa;

    while (temp->next != NULL) {
        temp = temp->next;
        if (temp->km == km){
            del_stazione=temp;
            free(temp);
        }
    }

    if (*testa == NULL || del_stazione == NULL) {
        printf("non demolita\n");
        return;
    }

    if (*testa == del_stazione)
        *testa = del_stazione->next;
    if (*coda == del_stazione)
        *coda = del_stazione->prev;

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

    delStazione(&testa, &coda, 3);

}

