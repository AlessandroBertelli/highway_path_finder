#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fo;
struct Stazione {
    unsigned int km;
    unsigned int numMacchine;
    int *macchine;
    struct Stazione *next;
    struct Stazione *prev;
};
int funzCompare(const void *primoEl, const void *secondoEl) {
    return (*(int *) primoEl - *(int *) secondoEl);
}
void
insStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int numMacchine, int **macchine);

void delStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km);

void insMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

void delMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

int pianificaRicorsivoDiretto(struct Stazione stazione, unsigned int origine);

int pianificaRicorsivoInverso(struct Stazione stazione, unsigned int destinazione, unsigned int * tappe, unsigned int * numTappe);

void pianificaPercorso(struct Stazione coda, unsigned int origine,
                       unsigned int destinazione);


int main(int argc, char *argv[]) { //rimuovi argv

    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;
    char *comando = (char *) malloc(33 * sizeof(char));

/*
    //rimuovi
    FILE *fp;
    fp = fopen(argv[1], "r");
    if ((fp) == NULL) {
        fprintf(fo, "Errore nell'apertura del file");
        exit(1);
    }

    fo = fopen(argv[2], "wt");
    if ((fo) == NULL) {
        fprintf(fo, "Errore nell'apertura del file");
        exit(1);
    }


*/

    //rimuovi
    FILE *fp;
    fp = fopen("/Users/alessandrobertelli/Downloads/archivio_test_aperti/open_7.txt", "r");
    if ((fp) == NULL) {
        fprintf(fo, "Errore nell'apertura del file");
        exit(1);
    }

    fo = fopen("/Users/alessandrobertelli/Downloads/archivio_test_aperti/open_7_output.txt", "wt");
    if ((fo) == NULL) {
        fprintf(fo, "Errore nell'apertura del file");
        exit(1);
    }


    while (fscanf(fp, "%s", comando) != EOF) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            unsigned int distanza = 0, numAuto = 0;
            fscanf(fp, "%u %u", &distanza, &numAuto);
            int *macchine = (int *) malloc(numAuto * sizeof(int));
            for (int i = 0; i < numAuto; ++i) {
                fscanf(fp, "%d", &macchine[i]);
            }
            insStazione(&testa, &coda, distanza, numAuto, &macchine);
        } else if (strcmp(comando, "demolisci-stazione") == 0) {
            unsigned int distanza = 0;
            fscanf(fp, "%u", &distanza);
            delStazione(&testa, &coda, distanza);
        } else if (strcmp(comando, "aggiungi-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            fscanf(fp, "%u %u", &distanza, &autonomia);
            insMacchina(&testa, &coda, distanza, autonomia);
        } else if (strcmp(comando, "rottama-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            fscanf(fp, "%u %u", &distanza, &autonomia);
            delMacchina(&testa, &coda, distanza, autonomia);
        } else if (strcmp(comando, "pianifica-percorso") == 0) {
            unsigned int origine, destinazione;
            fscanf(fp, "%u %u", &origine, &destinazione);
            pianificaPercorso(*coda, origine, destinazione);
        } else {
            fprintf(fo, "Input non valido!");
        }
    }

    if (testa == coda) {
        free(testa);
    } else {
        free(testa);
        free(coda);
    }

}

void insStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int numMacchine,
                 int **macchine) {

    struct Stazione *newStazione = (struct Stazione *) malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->numMacchine = numMacchine;
    newStazione->macchine = *macchine;
    qsort(newStazione->macchine, numMacchine, sizeof(int), funzCompare);
    newStazione->next = NULL;
    newStazione->prev = NULL;

    struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));

    if (*testa == NULL) {
        *testa = newStazione;
        *coda = newStazione;
        fprintf(fo, "aggiunta\n");
        return;
    } else if ((*testa)->km > newStazione->km) {
        newStazione->next = *testa;
        newStazione->next->prev = newStazione;
        *testa = newStazione;
        fprintf(fo, "aggiunta\n");
    } else {
        temp = *testa;

        if (temp->km == km && ((*testa) == (*coda))) {
            fprintf(fo, "non aggiunta\n");
            free(newStazione);
            return;
        }

        while (temp->next != NULL && temp->next->km < newStazione->km) {
            temp = temp->next;
        }
        if (temp->next != NULL && temp->next->km == newStazione->km) {
            fprintf(fo, "non aggiunta\n");
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
            fprintf(fo, "aggiunta\n");
        }
    }
}
//

void delStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km) {

    if ((*coda) == NULL || (*testa) == NULL) {
        fprintf(fo, "non demolita\n");
        return;
    } else if ((*testa)->km > km) {
        fprintf(fo, "non demolita\n");
        return;
    } else if ((*coda)->km < km) {
        fprintf(fo, "non demolita\n");
        return;
    }

    unsigned int demolito = 0; //allocare con malloc??????
    struct Stazione *del_stazione = (struct Stazione *) malloc(sizeof(struct Stazione));

    if ((*testa) == (*coda)) {
        *testa = NULL;
        *coda = NULL;
        free(del_stazione);
        fprintf(fo, "demolita\n");
        return;
    } else if ((*testa)->next != NULL && (*testa)->km == km) {
        (*testa)->next->prev = NULL;
        *testa = (*testa)->next;
        demolito = 1;
    } else if ((*coda)->km == km) {
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
        fprintf(fo, "non demolita\n");
        return;
    }

    if (del_stazione->next != NULL) {
        del_stazione->next->prev = del_stazione->prev;
    }
    if (del_stazione->prev != NULL)
        del_stazione->prev->next = del_stazione->next;

    free(del_stazione);
    fprintf(fo, "demolita\n");
}

void insMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia) {
    if ((*testa)->km > km) {
        fprintf(fo, "non aggiunta\n");
        return;
    }
    if ((*coda)->km < km) {
        fprintf(fo, "non aggiunta\n");
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
        fprintf(fo, "non aggiunta\n");
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
            int i = (int) stazione->numMacchine - 1;
            while (i >= 0 && stazione->macchine[i] > autonomia) {
                stazione->macchine[i + 1] = stazione->macchine[i];
                i--;
            }
            stazione->macchine[i + 1] = (int) autonomia;
            stazione->numMacchine++;
            fprintf(fo, "aggiunta\n");
        } else {
            fprintf(fo, "non aggiunta\n");
            return;
        }
    }
}

void delMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia) {
    if ((*testa)->km > km) {
        fprintf(fo, "non rottamata\n");
        return;
    }
    if ((*coda)->km < km) {
        fprintf(fo, "non rottamata\n");
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
        fprintf(fo, "non rottamata\n");
        return;
    } else {
        int *temp = stazione->macchine;
        int rottamed = 0;
        for (int i = 0; i < stazione->numMacchine; ++i) {
            if (temp[i] == autonomia) {
                for (int j = i; j < stazione->numMacchine; ++j) {
                    temp[j] = temp[j + 1];
                }
                stazione->numMacchine--;
                fprintf(fo, "rottamata\n");
                rottamed = 1;
                break;
            }
        }
        if(rottamed==0) {
            fprintf(fo, "non rottamata\n");
        }
    }
}

int pianificaRicorsivoDiretto(struct Stazione stazione, unsigned int origine) {
    int corretto = 1;
    if (stazione.km != origine) {
        unsigned int destinazione = stazione.km;
        struct Stazione *temp = stazione.prev;
        while (temp->km >= origine) {
            if ((temp->km + temp->macchine[temp->numMacchine - 1]) >= destinazione) {
                stazione = *temp;
            }
            temp = temp->prev;
            if (temp == NULL) {
                break;
            }
        }
        if (stazione.km == destinazione) {
            return -1;
        }
        corretto *= pianificaRicorsivoDiretto(stazione, origine);
        if (corretto == 0) {
            fprintf(fo, "%u ", stazione.km);
        }
        while (temp != NULL) {
            temp = temp->prev;
        }
        free(temp);
        return corretto;
    } else {
        return 0;
    }
}

int pianificaRicorsivoInverso(struct Stazione stazione, unsigned int destinazione, unsigned int * tappe, unsigned int *numTappe) {
    int corretto = 1;

    if (stazione.km != destinazione) {
        unsigned int origine = stazione.km;
        unsigned int origMaxAuto = stazione.macchine[stazione.numMacchine-1];

        struct Stazione *temp = stazione.prev;
        while (temp->km >= destinazione) {
            if (origine<=temp->km+origMaxAuto) {
                stazione = *temp;
            }
            temp = temp->prev;
            if (temp == NULL) {
                break;
            }
        }
        if (stazione.km == origine) {
            return -1;
        }

        (tappe)[(*numTappe)++]=stazione.km;

        corretto *= pianificaRicorsivoInverso(stazione, destinazione, tappe, numTappe);

        while (temp != NULL) {
            temp = temp->prev;
        }
        free(temp);
        return corretto;
    } else {
        return 0;
    }
}

void pianificaPercorso(const struct Stazione coda, unsigned int origine,
                       unsigned int destinazione) {

    if(destinazione==origine){
        printf("%u",origine);
    }
    else if (destinazione > origine) {
        struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione *));
        *temp = coda;

        while (temp != NULL) {
            if (temp->km == destinazione) {
                break;
            }
            temp = temp->prev;
        }

        (pianificaRicorsivoDiretto(*temp, origine) == 0) ? (fprintf(fo, "%u\n", temp->km)) : (fprintf(fo,
                                                                                                      "nessun percorso\n"));
    } else {
        struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));
        *temp = coda;

        while (temp != NULL) {
            if (temp->km == origine) {
                break;
            }
            temp = temp->prev;
        }

        unsigned int * tappe = malloc(2*sizeof (unsigned int));
        unsigned int numTappe = 1;
        tappe[0]=temp->km;

        if (pianificaRicorsivoInverso(*temp, destinazione, tappe, &numTappe) == 0) {
            fprintf(fo,"%u",tappe[0]);
            for (int i = 1; i < numTappe; ++i) {
                fprintf(fo," %u",tappe[i]);
            }
            fprintf(fo,"\n");
        } else {
            fprintf(fo,"nessun percorso\n");
        }

    }
}