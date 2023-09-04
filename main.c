#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fo;

struct Stazione {
    unsigned int km;
    unsigned int numMacchine;
    int *macchine;
    unsigned int raggiungibili;
    unsigned int check;
    struct Stazione *next;
    struct Stazione *prev;
};

int funzCompare(const void *primoEl, const void *secondoEl) {
    return (*(int *) primoEl - *(int *) secondoEl);
}

void
insStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int numMacchine, int **macchine);

void delStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km);

int toAlloc(unsigned int numMacchine);

void insMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

void delMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

int pianificaRicorsivoDiretto(struct Stazione stazione, unsigned int origine);

void pianificaInverso(struct Stazione *stazione, unsigned int origine, unsigned int destinazione);

void pianificaPercorso(struct Stazione testa, struct Stazione coda, unsigned int origine,
                       unsigned int destinazione);


int main(int argc, char *argv[]) { //rimuovi argv

    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;
    char *comando = (char *) malloc(33 * sizeof(char));
    FILE *fp;
    if (argc != 1) {
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
    } else {
        //rimuovi
        fp = fopen("/Users/alessandrobertelli/Downloads/archivio_test_aperti/open_110.txt", "r");
        if ((fp) == NULL) {
            fprintf(fo, "Errore nell'apertura del file");
            exit(1);
        }

        fo = fopen("/Users/alessandrobertelli/Downloads/archivio_test_aperti/open_110_output.txt", "wt");
        if ((fo) == NULL) {
            fprintf(fo, "Errore nell'apertura del file");
            exit(1);
        }
    }
    while (fscanf(fp, "%s", comando) != EOF) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            unsigned int distanza = 0, numAuto = 0;
            fscanf(fp, "%u %u", &distanza, &numAuto);
            int *macchine = (int *) malloc(toAlloc(numAuto) * sizeof(int));
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
            pianificaPercorso(*testa, *coda, origine, destinazione);
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
    } else if ((*testa)->km == km) {
        fprintf(fo, "non aggiunta\n");
        free(newStazione);
        return;
    } else {
        struct Stazione *temp = (struct Stazione *) malloc(sizeof(struct Stazione));
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

    unsigned int demolito = 0;//allocare con malloc??????
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

int toAlloc(unsigned int numMacchine) {
    if (numMacchine < 25) {
        return 25;
    } else if (numMacchine < 60) {
        return 60;
    } else if (numMacchine < 125) {
        return 125;
    } else if (numMacchine < 200) {
        return 200;
    } else if (numMacchine < 300) {
        return 425;
    } else {
        return 512;
    }
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
                case 25:
                    stazione->macchine = realloc(stazione->macchine, 60 * sizeof(int));
                    break;
                case 60:
                    stazione->macchine = realloc(stazione->macchine, 125 * sizeof(int));
                    break;
                case 125:
                    stazione->macchine = realloc(stazione->macchine, 200 * sizeof(int));
                    break;
                case 200:
                    stazione->macchine = realloc(stazione->macchine, 300 * sizeof(int));
                    break;
                case 300:
                    stazione->macchine = realloc(stazione->macchine, 425 * sizeof(int));
                    break;
                case 425:
                    stazione->macchine = realloc(stazione->macchine, 512 * sizeof(int));
                    break;
                default:
                    break;
            }

            int posizioneInserimento = stazione->numMacchine;
            while (posizioneInserimento > 0 && stazione->macchine[posizioneInserimento - 1] > autonomia) {
                stazione->macchine[posizioneInserimento] = stazione->macchine[posizioneInserimento - 1];
                posizioneInserimento--;
            }
            stazione->macchine[posizioneInserimento] = autonomia;
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
        if (rottamed == 0) {
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

void pianificaInverso(struct Stazione *stazione, unsigned int origine, unsigned int destinazione) {
    struct Stazione *temp = malloc(sizeof(struct Stazione));
    stazione = stazione->prev->next; //fatto perche il puntatore dava problemi
    while (stazione->km > destinazione) {
        stazione->raggiungibili = stazione->check = 0;
        temp = stazione->prev;

        while (temp->km >= destinazione) {
            if ((((int) (stazione->km)) - ((int) (stazione->macchine[stazione->numMacchine - 1])) <=
                 (int) (temp->km))) {
                stazione->raggiungibili += 1;
            }
            temp = temp->prev;
            if (temp == NULL) {
                break;
            }
        }
        stazione = stazione->prev;
    }

    stazione->raggiungibili = 0;

    unsigned int contatore = 1;
    struct Stazione tappe[100]; //RICORDA DI MODIFICARE e inizializzare
    unsigned int numTappe = 0;

    while (stazione->km < origine) {
        stazione->check = contatore - stazione->raggiungibili;
        contatore++;
        stazione = stazione->next;
    }

    stazione->check = contatore;
    int aux = 0;

    while (stazione->km > destinazione) {
        tappe[numTappe++] = *stazione;//tappe[numTappe++] = stazione->km;

        if (numTappe > 2) {
            struct Stazione *candidato = malloc(sizeof(struct Stazione));
            *candidato = tappe[numTappe - 2];
            int maxRaggiungibile = (int) ((int) tappe[numTappe - 3].km) -
                                   ((int) tappe[numTappe - 3].macchine[tappe[numTappe - 3].numMacchine - 1]);
            while (candidato != stazione) {
                if ((candidato->km >= maxRaggiungibile) &&
                    (stazione->km > ((int) candidato->km) - ((int) candidato->macchine[candidato->numMacchine - 1]))) {
                    tappe[numTappe - 2] = *candidato;
                }
                candidato=candidato->prev;
            }
        }

        aux = stazione->check;
        temp = stazione->prev;

        int nStazRagg = stazione->raggiungibili;

        if (nStazRagg == 0 && stazione->km != destinazione) {
            fprintf(fo, "nessun percorso\n");
            return;
        }

        for (int i = 0; i < nStazRagg; ++i) {
            if (aux >= temp->check) {
                aux = temp->check;
                stazione = temp;
                temp = temp->prev;
            } else {
                temp = temp->prev;
            }
        }
    }
    tappe[numTappe + 1] = *stazione;//tappe[numTappe+1]=stazione->km;

    for (int i = 0; i < numTappe; ++i) {
        fprintf(fo, "%u ", tappe[i].km);
    }

    fprintf(fo, "%u\n", tappe[numTappe + 1].km);
}

void pianificaPercorso(const struct Stazione testa, const struct Stazione coda, unsigned int origine,
                       unsigned int destinazione) {

    if (destinazione == origine) {
        printf("%u", origine);
    } else if (destinazione > origine) {
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

        pianificaInverso(temp, origine, destinazione);
    }
}