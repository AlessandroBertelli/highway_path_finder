#include <stdio.h>
#include <stdlib.h>

struct Stazione {
    int km;
    int numMacchine;
    int *macchine;
    struct Stazione *next;
    struct Stazione *prev;
};

void insStazione(struct Stazione **testa, struct Stazione **coda, int km, int numMacchine, int *macchine) {

    struct Stazione *newStazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->numMacchine = numMacchine;
    newStazione->macchine = macchine;
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
        printf("non demolita\n");
        return;
    }
    if ((*coda)->km < km) {
        printf("non demolita\n");
        return;
    }

    int demolito = 0; //allocare con malloc??????
    struct Stazione *del_stazione = (struct Stazione *) malloc(sizeof(struct Stazione));;
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

void insMacchina(struct Stazione **testa, struct Stazione **coda, int km, int autonomia) {
    if ((*testa)->km > km) {
        printf("non aggiunta\n");
        return;
    }
    if ((*coda)->km < km) {
        printf("non aggiunta\n");
        return;
    }

    struct Stazione *stazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    stazione = (*testa);

    while (stazione->next != NULL) {
        if (stazione->km == km) {
            break;
        }
        stazione = stazione->next;
    }

    if ((stazione == *coda) && stazione->km != km) {
        printf("non aggiunta\n");
        return;
    }else {
        if(stazione->numMacchine<512){
            switch (stazione->numMacchine+1) {
                case 3:
                    stazione->macchine = realloc(stazione->macchine, 4 * sizeof(int));
                case 5:
                    stazione->macchine = realloc(stazione->macchine, 8 * sizeof(int));
                    break;
                case 9:
                    stazione->macchine = realloc(stazione->macchine, 16 * sizeof(int));
                    break;
                case 17:
                    stazione->macchine = realloc(stazione->macchine, 32 * sizeof(int));
                    break;
                case 33:
                    stazione->macchine = realloc(stazione->macchine, 64 * sizeof(int));
                    break;
                case 65:
                    stazione->macchine = realloc(stazione->macchine, 128 * sizeof(int));
                    break;
                case 129:
                    stazione->macchine = realloc(stazione->macchine, 256 * sizeof(int));
                    break;
                case 257:
                    stazione->macchine = realloc(stazione->macchine, 512 * sizeof(int));
                    break;
                default:
                    break;
            }
            stazione->macchine[++stazione->numMacchine] = autonomia;
            printf("aggiunta\n");
        }else{
            printf("non aggiunta\n");
            return;
        }
    }
}
/*
void allocaSpazio(struct Stazione *stazione) {
    switch ((stazione->numMacchine) + 1) {
        case 1 ... 4:
            stazione->macchine = realloc(stazione->macchine, 4 * sizeof(int));
            break;
        case 5 ... 8:
            stazione->macchine = realloc(stazione->macchine, 8 * sizeof(int));
            break;
        case 9 ... 16:
            stazione->macchine = realloc(stazione->macchine, 16 * sizeof(int));
            break;
        case 17 ... 32:
            stazione->macchine = realloc(stazione->macchine, 32 * sizeof(int));
            break;
        case 33 ... 64:
            stazione->macchine = realloc(stazione->macchine, 64 * sizeof(int));
            break;
        case 65 ... 128:
            stazione->macchine = realloc(stazione->macchine, 128 * sizeof(int));
            break;
        case 129 ... 256:
            stazione->macchine = realloc(stazione->macchine, 256 * sizeof(int));
            break;
        case 257 ... 512:
            stazione->macchine = realloc(stazione->macchine, 512 * sizeof(int));
            break;
        default:
            break;
    }
}
*/
int main() {
    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;

    int * test = malloc(4*sizeof (int));

    test[0] = 3;
    test[1] = 2;
    test[2] = 4;

    int * test2 = malloc(4*sizeof (int));

    test2[0] = 3;
    test2[1] = 2;
    test2[2] = 4;


    insStazione(&testa, &coda, 2,3,test);
    delStazione(&testa, &coda, 8);
    insStazione(&testa, &coda, 8,3,test2);
    insMacchina(&testa,&coda,8,56);
    insMacchina(&testa,&coda,8,34);

    printf("%d\n",(*testa).next->km);
    printf("%d\n",(*testa).next->macchine[3]);
    printf("%d\n",(*testa).next->macchine[4]);


    free(testa);
    free(coda);

    //utilizzare magari per macchine una lista?
}

