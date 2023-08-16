#include <stdio.h>
#include <stdlib.h>

struct Stazione {
    int km;
    int numMacchine;
    int macchine;
    struct Stazione *next;
    struct Stazione *prev;
};

struct Stazione *trovaStazione(struct Stazione **testa, int km) {
    if ((*testa)->km == km)
        return (*testa);
    while ((*testa)->next != NULL) {
        (*testa) = (*testa)->next;
        if ((*testa)->km == km)
            return (*testa);
    }
    return NULL;
}

void inserisci(struct Stazione **testa, int km) {

    struct Stazione *newStazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->next = NULL;
    newStazione->prev = NULL;

    struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));

    if (*testa == NULL)
        *testa = newStazione;

    else if ((*testa)->km >= newStazione->km) {
        newStazione->next = *testa;
        newStazione->next->prev = newStazione;
        *testa = newStazione;

    } else {
        temp = *testa;

        // locate the node after which the new node
        // is to be inserted
        while (temp->next != NULL && temp->next->km < newStazione->km)
            temp = temp->next;

        newStazione->next = temp->next;

        if (newStazione->next != NULL)
            newStazione->next->prev = newStazione;

        temp->next = newStazione;
        newStazione->prev = temp;
    }
    free(temp);
}

void eliminaStazione(struct Stazione **testa, struct Stazione *del_stazione) {

    if (*testa == NULL || del_stazione == NULL)
        return;

    // if del_Stazione is the head Stazione, point the head pointer to the next of del_Stazione
    if (*testa == del_stazione)
        *testa = del_stazione->next;

    // if del_Stazione is not at the last Stazione, point the prev of Stazione next to del_Stazione to the previous of del_Stazione
    if (del_stazione->next != NULL)
        del_stazione->next->prev = del_stazione->prev;

    // if del_Stazione is not the first Stazione, point the next of the previous Stazione to the next Stazione of del_Stazione
    if (del_stazione->prev != NULL)
        del_stazione->prev->next = del_stazione->next;

    // free the memory of del_Stazione
    free(del_stazione);
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


}

