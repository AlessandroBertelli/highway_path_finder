#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "assert.h"

struct Stazione {
    unsigned int km;
    unsigned int numMacchine;
    int *macchine;
    unsigned int distanza;
    bool visited;
    unsigned int proprioIndice;
    int prevBFS;
};

int funzCompare(const void *primoEl, const void *secondoEl) {
    return (*(int *) primoEl - *(int *) secondoEl);
}

int binarySearch(struct Stazione stazioni[], int basso, int alto, int km);

int ricercaPrecedenteBinario(const struct Stazione stazioni[], unsigned int numStazioni, unsigned int kmGenerico);

void insStazione(struct Stazione stazioni[], unsigned int *numStazioni, unsigned int km, unsigned int numMacchine,
                 const int macchine[], unsigned int numAuto);

void delStazione(struct Stazione stazioni[], unsigned int *numStazioni, unsigned int km);

int toAlloc(unsigned int numMacchine);

void insMacchina(struct Stazione stazioni[], unsigned int numStazioni, unsigned int km, unsigned int autonomia);

void delMacchina(struct Stazione stazioni[], unsigned int numStazioni, unsigned int km, unsigned int autonomia);

int pianificaRicorsivoDiretto(struct Stazione stazioni[], unsigned int numStazioni, unsigned int indiceStazione,
                              unsigned int origine);

void pianificaPercorso(struct Stazione stazioni[], unsigned int numStazioni, unsigned int origine,
                       unsigned int destinazione, int indiceDestinazione);

void BFS(struct Stazione stazioni[], unsigned int numStazioni, unsigned int indiceDestinazione, unsigned int origine);

bool stampaPercorso(struct Stazione stazioni[], unsigned int numStazioni, unsigned int sorgente,
                    unsigned int destinazione, unsigned int tappe[], unsigned int *numTappe);

int main() {
    struct Stazione stazioni[50000];
    unsigned int numStazioni = 0;
    char comando[20];

    while (scanf("%s", comando) != EOF) {
        if (strcmp(comando, "aggiungi-stazione") == 0) {
            unsigned int distanza = 0, numAuto = 0;
            (assert(scanf("%u %u", &distanza, &numAuto)));
            int macchine[numAuto];
            for (int i = 0; i < numAuto; ++i) {
                (assert(scanf("%d", &macchine[i])));
            }
            insStazione(stazioni, &numStazioni, distanza, numAuto, macchine, numAuto);
        } else if (strcmp(comando, "demolisci-stazione") == 0) {
            unsigned int distanza = 0;
            (assert(scanf("%u", &distanza)));
            delStazione(stazioni, &numStazioni, distanza);
        } else if (strcmp(comando, "aggiungi-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            (assert(scanf("%u %u", &distanza, &autonomia)));
            insMacchina(stazioni, numStazioni, distanza, autonomia);
        } else if (strcmp(comando, "rottama-auto") == 0) {
            unsigned int distanza = 0, autonomia = 0;
            (assert(scanf("%u %u", &distanza, &autonomia)));
            delMacchina(stazioni, numStazioni, distanza, autonomia);
        } else if (strcmp(comando, "pianifica-percorso") == 0) {
            unsigned int origine = 0, destinazione = 0;
            (assert(scanf("%u %u", &origine, &destinazione)));

            if (destinazione == origine) {
                printf("%u\n", origine);
            } else {
                int indiceDestinazione = binarySearch(stazioni, 0, numStazioni - 1, destinazione);
                //AGGIUNGI CASO IN TESTA E IN CODA
                if (destinazione > origine) {
                    pianificaPercorso(stazioni, numStazioni, origine, destinazione, indiceDestinazione);
                } else {
                    BFS(stazioni, numStazioni, indiceDestinazione, origine);
                    pianificaPercorso(stazioni, numStazioni, origine, destinazione, indiceDestinazione);
                }
            }
        } else {
        }
    }
    exit(0);
}

int ricercaPrecedenteBinario(const struct Stazione stazioni[], unsigned int numStazioni, unsigned int kmGenerico) {
    int sinistra = -1;
    int destra = numStazioni;
    int indicePrecedente = -1;

    while (sinistra + 1 < destra) {
        int medio = (sinistra + destra) / 2;
        if (stazioni[medio].km < kmGenerico) {
            sinistra = medio;
            indicePrecedente = medio;
        } else {
            destra = medio;
        }
    }

    return indicePrecedente;
}

int binarySearch(struct Stazione stazioni[], int basso, int alto, int km) {
    if (alto < basso)
        return -1;
    int mid = (basso + alto) / 2;
    if (km == stazioni[mid].km)
        return mid;
    if (km > stazioni[mid].km)
        return binarySearch(stazioni, (mid + 1), alto, km);
    return binarySearch(stazioni, basso, (mid - 1), km);
}

void insStazione(struct Stazione stazioni[], unsigned int *numStazioni, unsigned int km, unsigned int numMacchine,
                 const int macchine[], unsigned int numAuto) {

    if (binarySearch(stazioni, 0, (*numStazioni) - 1, km) != -1) {
        printf("non aggiunta\n");
        return;
    }

    if (*numStazioni >= 99999999) {
        printf("non aggiunta\n");
        return;
    }

    struct Stazione *newStazione = malloc(sizeof(struct Stazione));
    newStazione->km = km;
    newStazione->numMacchine = numMacchine;
    newStazione->macchine = malloc(toAlloc(numAuto) * sizeof(int));
    for (int i = 0; i < numAuto; i++) {
        (newStazione->macchine[i] = macchine[i]);
    }
    if (numAuto > 0) {
        qsort(newStazione->macchine, numMacchine, sizeof(int), funzCompare);
    }

    if (*numStazioni == 0) {
        stazioni[(*numStazioni)++] = *newStazione;
        printf("aggiunta\n");
        return;
    } else if (stazioni[0].km > km) {
        for (int i = ((int) *numStazioni); i > 0; i--) {
            stazioni[i] = stazioni[i - 1];
        }
        stazioni[0] = *newStazione;
        (*numStazioni)++;
        printf("aggiunta\n");
        return;
    } else if (stazioni[*numStazioni - 1].km < km) {
        stazioni[(*numStazioni)++] = *newStazione;
        printf("aggiunta\n");
        return;
    } else {
        int i = ricercaPrecedenteBinario(stazioni, *numStazioni, km);
        for (int j = (*numStazioni) - 1; j > i; --j) {
            stazioni[j + 1] = stazioni[j];
        }
        stazioni[i + 1] = *newStazione;
        (*numStazioni)++;
        printf("aggiunta\n");
        return;
    }
}

void delStazione(struct Stazione stazioni[], unsigned int *numStazioni, unsigned int km) {
    if (*numStazioni == 0) {
        printf("non demolita\n");
        return;
    }

    int pos = binarySearch(stazioni, 0, (*numStazioni) - 1, km);

    if (pos == -1) {
        printf("non demolita\n");
        return;
    }

    int i;
    for (i = pos; i < (*numStazioni) - 1; i++)
        stazioni[i] = stazioni[i + 1];

    (*numStazioni)--;
    printf("demolita\n");
    return;
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

void insMacchina(struct Stazione stazioni[], unsigned int numStazioni, unsigned int km, unsigned int autonomia) {
    if (numStazioni == 0) {
        printf("non aggiunta\n");
        return;
    }

    int pos = binarySearch(stazioni, 0, numStazioni - 1, km);

    if (pos != -1) {
        if (stazioni[pos].numMacchine >= 512) {
            printf("non aggiunta\n");
            return;
        }
        switch (stazioni[pos].numMacchine + 1) {
            case 25:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 60 * sizeof(int));
                break;
            case 60:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 125 * sizeof(int));
                break;
            case 125:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 200 * sizeof(int));
                break;
            case 200:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 300 * sizeof(int));
                break;
            case 300:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 425 * sizeof(int));
                break;
            case 425:
                stazioni[pos].macchine = realloc(stazioni[pos].macchine, 512 * sizeof(int));
                break;
            default:
                break;
        }
        int posizioneInserimento = stazioni[pos].numMacchine;
        while (posizioneInserimento > 0 && stazioni[pos].macchine[posizioneInserimento - 1] > autonomia) {
            stazioni[pos].macchine[posizioneInserimento] = stazioni[pos].macchine[posizioneInserimento - 1];
            posizioneInserimento--;
        }
        stazioni[pos].macchine[posizioneInserimento] = (int) autonomia;
        stazioni[pos].numMacchine++;
        printf("aggiunta\n");
        return;
    } else {
        printf("non aggiunta\n");
        return;
    }

}


void delMacchina(struct Stazione stazioni[], unsigned int numStazioni, unsigned int km, unsigned int autonomia) {
    if (numStazioni == 0) {
        printf("non rottamata\n");
        return;
    }

    int pos = binarySearch(stazioni, 0, numStazioni - 1, km);

    if (pos != -1) {
        int *macchine = stazioni[pos].macchine;
        bool rottamed = false;
        for (int j = 0; j < stazioni[pos].numMacchine; ++j) {
            if (macchine[j] == autonomia) {
                for (int k = j; k < stazioni[pos].numMacchine - 1; ++k) {
                    macchine[k] = macchine[k + 1];
                }
                macchine[stazioni[pos].numMacchine - 1] = 0;
                stazioni[pos].numMacchine--;
                printf("rottamata\n");
                rottamed = true;
                return;
            }
        }
        if (rottamed == 0) {
            printf("non rottamata\n");
            return;
        }
    } else {
        printf("non rottamata\n");
        return;
    }
}

int pianificaRicorsivoDiretto(struct Stazione stazioni[], unsigned int numStazioni, unsigned int indiceStazione,
                              unsigned int origine) {
    int corretto = 1;
    if (stazioni[indiceStazione].km != origine) {
        unsigned int destinazione = stazioni[indiceStazione].km;
        int indiceTemp = indiceStazione - 1;
        struct Stazione temp = stazioni[indiceTemp];
        while (temp.km >= origine) {
            if ((temp.km + temp.macchine[temp.numMacchine - 1]) >= destinazione) {
                indiceStazione = indiceTemp;
            }
            indiceTemp--;
            if (indiceTemp < 0) {
                break;
            }
            temp = stazioni[indiceTemp];
        }
        if (stazioni[indiceStazione].km == destinazione) {
            return -1;
        }
        corretto *= pianificaRicorsivoDiretto(stazioni, numStazioni, indiceStazione, origine);
        if (corretto == 0) {
            printf("%u ", stazioni[indiceStazione].km);
        }
        return corretto;
    } else {
        return 0;
    }
}

void pianificaPercorso(struct Stazione stazioni[], unsigned int numStazioni, unsigned int origine,
                       unsigned int destinazione, int indiceDestinazione) {

    if (destinazione > origine) {

        (pianificaRicorsivoDiretto(stazioni, numStazioni, indiceDestinazione, origine) == 0) ? (printf("%u\n",
                                                                                                       stazioni[indiceDestinazione].km))
                                                                                             : (printf(
                "nessun percorso\n"));
    } else {
        unsigned int stazSorgente = binarySearch(stazioni, 0, numStazioni - 1, origine);
        unsigned int stazDestinazione = binarySearch(stazioni, 0, numStazioni - 1, destinazione);

        unsigned int tappe[1000];
        unsigned int numTappe = 0;
        unsigned int *poNumTappe = &numTappe;
        if (stampaPercorso(stazioni, numStazioni, stazSorgente, stazDestinazione, tappe, poNumTappe)) {
            for (int i = numTappe - 1; i > 0; --i) {
                printf("%u ", tappe[i]);
            }
            printf("%u\n", tappe[0]);
        }
        /*for (int i = 0; i < numStazioni; ++i) {
            printf("%u %u\n", stazioni[i].km, stazioni[i].macchine[stazioni[i].numMacchine - 1]);
        }*/
    }
}

void BFS(struct Stazione stazioni[], unsigned int numStazioni, unsigned int indiceDestinazione, unsigned int origine) {

    for (int j = 0; j < numStazioni; ++j) {
        stazioni[j].visited = false;
        stazioni[j].distanza = 0;
        stazioni[j].prevBFS = -1;
        stazioni[j].proprioIndice = j;
    }

    struct Stazione queue[30000];
    int fronte = 0, retro = 0;

    queue[retro++] = stazioni[indiceDestinazione];

    while (fronte < retro) {
        struct Stazione *analized = &queue[fronte++];
        int indiceTemp = (int) (analized->proprioIndice + 1);

        while (indiceTemp < numStazioni && analized->proprioIndice < numStazioni) {
            if (stazioni[indiceTemp].numMacchine > 0 && stazioni[indiceTemp].km <= origine) {
                if (((int) (stazioni[indiceTemp].km)) -
                    ((int) (stazioni[indiceTemp].macchine[stazioni[indiceTemp].numMacchine - 1])) <=
                    (int) (analized->km)) {
                    if (!stazioni[indiceTemp].visited) {
                        stazioni[indiceTemp].visited = true;
                        stazioni[indiceTemp].distanza = analized->distanza + 1;
                        stazioni[indiceTemp].prevBFS = (int) (analized->proprioIndice);
                        if (retro < 30000) {
                            queue[retro++] = stazioni[indiceTemp];
                        }
                    }
                }
            } else {
                if (stazioni[indiceTemp].km > origine) {
                    indiceTemp = (int) numStazioni;
                    break;
                }
            }
            indiceTemp++;
        }
        analized->visited = true;
    }


}

bool stampaPercorso(struct Stazione stazioni[], unsigned int numStazioni, unsigned sorgente,
                    unsigned int destinazione, unsigned int tappe[], unsigned int *numTappe) {
    if (stazioni[sorgente].km == stazioni[destinazione].km) {
        tappe[(*numTappe)++] = stazioni[sorgente].km;
        return true;
    } else if (stazioni[sorgente].prevBFS == -1) {
        printf("nessun percorso\n");
        return false;
    } else if (stampaPercorso(stazioni, numStazioni, stazioni[sorgente].prevBFS, destinazione, tappe, numTappe)) {
        tappe[(*numTappe)++] = stazioni[sorgente].km;
        return true;
    } else {
        return false;
    }
}