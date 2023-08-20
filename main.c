#include <stdio.h>
#include <stdlib.h>

int funzCompare (const void * primoEl, const void * secondoEl) {
    return ( *(int*)primoEl - *(int*)secondoEl );
}

struct Stazione {
    int km;
    int numMacchine;
    int *macchine;
    struct Stazione *next;
    struct Stazione *prev;
};

void insStazione(struct Stazione **testa, struct Stazione **coda, int km, int numMacchine, int **macchine) {

    struct Stazione *newStazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->numMacchine = numMacchine;
    newStazione->macchine = *macchine;
    qsort(newStazione->macchine,numMacchine,sizeof (int), funzCompare);
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

    struct Stazione *stazione;
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
    } else {
        if (stazione->numMacchine < 512) {
            switch (stazione->numMacchine + 1) {
                case 3:
                    stazione->macchine = realloc(stazione->macchine, 4 * sizeof(int));
                    break;
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
            int i = stazione->numMacchine - 1;
            while (i >= 0 && stazione->macchine[i] > autonomia) {
                stazione->macchine[i + 1] = stazione->macchine[i];
                i--;
            }
            stazione->macchine[i + 1] = autonomia;
            stazione->numMacchine++;
            printf("aggiunta\n");
        } else {
            printf("non aggiunta\n");
            return;
        }
    }
}

void delMacchina(struct Stazione **testa, struct Stazione **coda, int km, int autonomia) {
    if ((*testa)->km > km) {
        printf("non rottamata\n");
        return;
    }
    if ((*coda)->km < km) {
        printf("non rottamata\n");
        return;
    }

    struct Stazione *stazione;
    stazione = (*testa);

    while (stazione->next != NULL) {
        if (stazione->km == km) {
            break;
        }
        stazione = stazione->next;
    }

    if ((stazione == *coda) && stazione->km != km) {
        printf("non rottamata\n");
        return;
    } else {
        int *temp = stazione->macchine;
        for (int i = 0; i < stazione->numMacchine; ++i) {
            if(temp[i]==autonomia){
                for (int j = i; j < stazione->numMacchine; ++j) {
                    temp[j]=temp[j+1];
                }
                stazione->numMacchine--;
                printf("rottamata\n");
                break;
            }
        }


    }
}

int main() {

    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;

    int *test = (int *) malloc(4 * sizeof(int));

    test[0] = 34;
    test[1] = 21;
    test[2] = 54;

    int *test2 = (int *) malloc(4 * sizeof(int));

    test2[0] = 64;
    test2[1] = 43;
    test2[2] = 87;


    insStazione(&testa, &coda, 2, 3, &test);
    delStazione(&testa, &coda, 8);
    insStazione(&testa, &coda, 8, 3, &test2);
    insMacchina(&testa, &coda, 8, 56);
    insMacchina(&testa, &coda, 8, 35);

    printf("%d\n", (*testa).next->km);
    for (int i = 0; i < 5; ++i) {
        printf("%d\n", (*testa).next->macchine[i]);
    }

    delMacchina(&testa, &coda, 8, 43);
    delMacchina(&testa, &coda, 8, 64);


    printf("%d\n", (*testa).next->km);
    for (int i = 0; i < 5; ++i) {
        printf("%d\n", (*testa).next->macchine[i]);
    }




    free(testa);
    free(coda);

    //utilizzare magari per macchine una lista?
}

