#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "stdbool.h"


struct Stazione {
    unsigned int km;
    unsigned int numMacchine;
    int *macchine;
    unsigned int raggiungibili;
    unsigned int check;

    unsigned int distanza;
    bool visited;
    struct Stazione *prevBFS;


    struct Stazione *next;
    struct Stazione *prev;
};

int funzCompare(const void *primoEl, const void *secondoEl) {
    return (*(int *) primoEl - *(int *) secondoEl);
}

void
insStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int numMacchine,
            const int macchine[], unsigned int numAuto);

void delStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km);

int toAlloc(unsigned int numMacchine);

void insMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

void delMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia);

int pianificaRicorsivoDiretto(struct Stazione stazione, unsigned int origine);

void pianificaInverso(struct Stazione *stazione, unsigned int origine, unsigned int destinazione);

void pianificaPercorso(struct Stazione testa, struct Stazione coda, unsigned int origine,
                       unsigned int destinazione);

void BFS(struct Stazione *testa, struct Stazione *stazDestinazione, unsigned int origine, unsigned int destinazione);


bool
stampaPercorso(struct Stazione sorgente, struct Stazione destinazione, unsigned int tappe[], unsigned int *numTappe);

int main() {

    struct Stazione *testa = NULL;
    struct Stazione *coda = NULL;
    char *comando = NULL;
    comando = (char *) malloc(33 * sizeof(char));

    while (scanf("%s", comando) != EOF) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            unsigned int distanza = 0, numAuto = 0;
            (assert(scanf("%u %u", &distanza, &numAuto)));
            int macchine[numAuto];
            for (int i = 0; i < numAuto; ++i) {
                (assert(scanf("%d", &macchine[i])));
            }
            insStazione(&testa, &coda, distanza, numAuto, macchine, numAuto);
        } else if (strcmp(comando, "demolisci-stazione") == 0) {
            unsigned int distanza = 0;
            (assert(scanf("%u", &distanza)));
            delStazione(&testa, &coda, distanza);
        } else if (strcmp(comando, "aggiungi-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            (assert(scanf("%u %u", &distanza, &autonomia)));
            insMacchina(&testa, &coda, distanza, autonomia);
        } else if (strcmp(comando, "rottama-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            (assert(scanf("%u %u", &distanza, &autonomia)));
            delMacchina(&testa, &coda, distanza, autonomia);
        } else if (strcmp(comando, "pianifica-percorso") == 0) {
            unsigned int origine = 0, destinazione = 0;
            (assert(scanf("%u %u", &origine, &destinazione)));
            pianificaPercorso(*testa, *coda, origine, destinazione);
        } else {
        }
    }
    if (testa == coda) {
        free(testa);
    } else {
        free(testa);
        free(coda);
    }
    free(comando);
    exit(0);
}

void insStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int numMacchine,
                 const int macchine[], unsigned int numAuto) {

    struct Stazione *newStazione = NULL;
    newStazione = malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->numMacchine = numMacchine;
    newStazione->macchine = NULL;
    newStazione->macchine = malloc(toAlloc(numAuto) * sizeof(int));
    for (int i = 0; i < numAuto; i++) {
        (newStazione->macchine[i] = macchine[i]);
    }
    if (numAuto > 0) {
        qsort(newStazione->macchine, numMacchine, sizeof(int), funzCompare);
    }
    newStazione->next = NULL;
    newStazione->prev = NULL;

    if (*testa == NULL) {
        *testa = newStazione;
        *coda = newStazione;
        printf("aggiunta\n");
        return;
    } else if ((*testa)->km > newStazione->km) {
        newStazione->next = *testa;
        newStazione->next->prev = newStazione;
        *testa = newStazione;
        printf("aggiunta\n");
    } else if ((*testa)->km == km) {
        printf("non aggiunta\n");
        free(newStazione);
        return;
    } else {
        struct Stazione *temp = NULL;
        temp = *testa;

        if (temp->km == km && ((*testa) == (*coda))) {
            printf("non aggiunta\n");
            free(newStazione);
            return;
        }

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
    newStazione = malloc(sizeof(struct Stazione));
    free(newStazione);
}


void delStazione(struct Stazione **testa, struct Stazione **coda, unsigned int km) {

    if ((*coda) == NULL || (*testa) == NULL) {
        printf("non demolita\n");
        return;
    } else if ((*testa)->km > km) {
        printf("non demolita\n");
        return;
    } else if ((*coda)->km < km) {
        printf("non demolita\n");
        return;
    }

    unsigned int demolito = 0;//allocare con malloc??????
    struct Stazione *del_stazione = NULL;
    del_stazione = malloc(sizeof(struct Stazione));
    del_stazione->next = NULL;
    del_stazione->prev = NULL;

    if ((*testa) == (*coda)) {
        *testa = NULL;
        *coda = NULL;
        free(del_stazione);
        printf("demolita\n");
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
        printf("non demolita\n");
        return;
    }

    if (del_stazione->next != NULL) {
        del_stazione->next->prev = del_stazione->prev;
    }
    if (del_stazione->prev != NULL)
        del_stazione->prev->next = del_stazione->next;

    printf("demolita\n");
    del_stazione = malloc(sizeof(struct Stazione));
    free(del_stazione);
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
    if (*testa == NULL) {
        printf("non aggiunta\n");
        return;
    }
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

            int posizioneInserimento = (int) stazione->numMacchine;
            while (posizioneInserimento > 0 && stazione->macchine[posizioneInserimento - 1] > autonomia) {
                stazione->macchine[posizioneInserimento] = stazione->macchine[posizioneInserimento - 1];
                posizioneInserimento--;
            }
            stazione->macchine[posizioneInserimento] = (int) autonomia;
            stazione->numMacchine++;

            printf("aggiunta\n");
        } else {
            printf("non aggiunta\n");
            return;
        }
    }
    stazione = malloc(sizeof(struct Stazione));
    free(stazione);
}

void delMacchina(struct Stazione **testa, struct Stazione **coda, unsigned int km, unsigned int autonomia) {
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
        int rottamed = 0;
        for (int i = 0; i < stazione->numMacchine; ++i) {
            if (temp[i] == autonomia) {
                for (int j = i; j < stazione->numMacchine - 1; ++j) {
                    temp[j] = temp[j + 1];
                }
                temp[stazione->numMacchine - 1] = 0;
                stazione->numMacchine--;
                printf("rottamata\n");
                rottamed = 1;
                break;
            }
        }
        if (rottamed == 0) {
            printf("non rottamata\n");
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
            printf("%u ", stazione.km);
        }
        while (temp != NULL) {
            temp = temp->prev;
        }
        //free(temp);
        return corretto;
    } else {
        return 0;
    }
}

void pianificaInverso(struct Stazione *stazione, unsigned int origine, unsigned int destinazione) {
    struct Stazione *temp = NULL;
    stazione = stazione->prev->next; //fatto perche il puntatore dava problemi


    while (stazione->km > destinazione) {
        stazione->raggiungibili = 0;
        stazione->check = 0;
        temp = stazione->prev;

        while (temp->km >= destinazione) {
            if (stazione->numMacchine > 0) {
                if ((((int) (stazione->km)) - ((int) (stazione->macchine[stazione->numMacchine - 1])) <=
                     (int) (temp->km))) {
                    stazione->raggiungibili += 1;
                }
            }
            temp = temp->prev;
            if (temp == NULL) {
                break;
            }
        }
        stazione = stazione->prev;
        if (stazione == NULL)
            break;
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

    while (stazione->km > destinazione) {
        tappe[numTappe++] = *stazione;

        if (numTappe > 2) {
            struct Stazione *candidato = &tappe[numTappe - 2];
            int maxRaggiungibile = (int) ((int) tappe[numTappe - 3].km) -
                                   ((int) tappe[numTappe - 3].macchine[tappe[numTappe - 3].numMacchine - 1]);
            while (candidato != stazione) {
                if ((candidato->km >= maxRaggiungibile) &&
                    (stazione->km > ((int) candidato->km) - ((int) candidato->macchine[candidato->numMacchine - 1]))) {
                    tappe[numTappe - 2] = *candidato;
                }
                candidato = candidato->prev;
            }
        }

        int aux = (int) stazione->check;
        temp = stazione->prev;

        int nStazRagg = (int) stazione->raggiungibili;

        if (nStazRagg == 0 && stazione->km != destinazione) {
            printf("nessun percorso\n");
            return;
        }

        for (int i = 0; i < nStazRagg; ++i) {
            if (aux >= temp->check) {
                aux = (int) temp->check;
                stazione = temp;
            }
            temp = temp->prev;
        }
    }
    tappe[numTappe + 1] = *stazione;//tappe[numTappe+1]=stazione->km;

    for (int i = 0; i < numTappe; ++i) {
        printf("%u ", tappe[i].km);
    }

    printf("%u\n", tappe[numTappe + 1].km);
}

void pianificaPercorso(const struct Stazione testa, const struct Stazione coda, unsigned int origine,
                       unsigned int destinazione) {

    if (destinazione == origine) {
        printf("%u\n", origine);
    } else if (destinazione > origine) {
        struct Stazione *temp = NULL;
        temp = malloc(sizeof(struct Stazione));
        *temp = coda;

        while (temp != NULL) {
            if (temp->km == destinazione) {
                break;
            }
            temp = temp->prev;
        }

        (pianificaRicorsivoDiretto(*temp, origine) == 0) ? (printf("%u\n", temp->km)) : (printf(
                "nessun percorso\n"));
    } else {
        struct Stazione *stazSorgente = NULL;
        stazSorgente = malloc(sizeof(struct Stazione));
        struct Stazione *stazDestinazione = NULL;
        stazDestinazione = malloc(sizeof(struct Stazione));
        *stazSorgente = testa;
        *stazDestinazione = coda;
        while (stazSorgente != NULL) {
            //if (temp->km == origine) {
            if (stazSorgente->km == origine) {
                break;
            }
            stazSorgente = stazSorgente->next;
        }
        while (stazDestinazione != NULL) {
            //if (temp->km == origine) {
            if (stazDestinazione->km == destinazione) {
                break;
            }
            stazDestinazione = stazDestinazione->prev;
        }

        //pianificaInverso(temp, origine, destinazione);
        //BFS(stazDestinazione, stazDestinazione, origine, destinazione);
        BFS(stazDestinazione, stazDestinazione, origine, destinazione);
        unsigned int tappe[100];
        unsigned int numTappe = 0;
        unsigned int *poNumTappe = &numTappe;


        if (stampaPercorso(*stazSorgente, *stazDestinazione, tappe, poNumTappe)) {
            for (int i = numTappe - 1; i > 0; --i) {
                printf("%u ", tappe[i]);
            }
            printf("%u\n",tappe[0]);
        }

    }
}

void BFS(struct Stazione *testa, struct Stazione *stazDestinazione, unsigned int origine, unsigned int destinazione) {

    struct Stazione *stazione = testa;
    while (stazione->km<origine) {
        stazione->visited = false;
        stazione->distanza = 0;
        stazione->prevBFS = NULL;
        stazione = stazione->next;
        if(stazione==NULL){
            break;
        }
    } //OTTIMIZZAZIONE SU
    /*while (stazione != NULL) {
        stazione->visited = false;
        stazione->distanza = 0;
        stazione->prevBFS = NULL;
        stazione = stazione->next;

    }*/

    stazione = testa;

    struct Stazione **queue = malloc(sizeof(struct Stazione) * 100);
    int fronte = 0, retro = 0;

    queue[retro++] = stazione;

    while (fronte < retro) {
        struct Stazione *analized = queue[fronte++];
        struct Stazione *temp = analized->next;

        while (temp != NULL) { //OTTIMIZZARE FINO A ORIGINE
            if (temp->numMacchine > 0) {
                if (((int) (temp->km)) - ((int) (temp->macchine[temp->numMacchine - 1])) <=
                    (int) (analized->km)) {
                    if (!temp->visited) {
                        temp->visited = true;
                        temp->distanza = analized->distanza + 1;
                        temp->prevBFS = analized;
                        queue[retro++] = temp;
                    }
                }
            }
            temp = temp->next;
        }
        analized->visited = true;
    }
}

bool stampaPercorso(struct Stazione sorgente, struct Stazione destinazione, unsigned int tappe[], unsigned int *numTappe) {
    if (sorgente.km == destinazione.km) {
        tappe[(*numTappe)++] = sorgente.km; // Corretto
        return true;
    } else if (sorgente.prevBFS == NULL) {
        printf("nessun percorso\n");
        return false;
    } else if (stampaPercorso(*(sorgente.prevBFS), destinazione, tappe, numTappe)) {
        tappe[(*numTappe)++] = sorgente.km; // Corretto
        return true;
    } else {
        return false;
    }
}
