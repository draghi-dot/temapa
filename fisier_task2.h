#ifndef FISIER_TASK2_H
#define FISIER_TASK2_H

#include "fisier_comun.h" 
#include "fisier_task1.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

task 2: identificarea si afisarea doar a celulelor care isi schimba starea in fiecare generatie

StivaModificari - structura pentru stocarea modificarilor din fiecare generatie
initStiva() - initializeaza stiva cu o capacitate data
pushStiva() - adauga o lista de modificari in stiva si redimensioneaza daca e necesar
elibereazaMemorieStiva() - elibereaza memoria folosita de stiva
executaTask2() - calculeaza evolutia matricei si afiseaza doar celulele modificate

*/

typedef struct {
    Celula** elemente;
    int varf;
    int capacitate;
} StivaModificari;

void initStiva(StivaModificari* stiva, int capacitate_initiala){
    if (capacitate_initiala <= 0) capacitate_initiala = 10; 
    stiva->elemente = (Celula**)malloc(capacitate_initiala * sizeof(Celula*));
    if (!stiva->elemente){
        printf("eroare alocare memorie pentru stiva Task 2 in initStiva\n");
        exit(1);
    }
    stiva->varf = 0;
    stiva->capacitate = capacitate_initiala;
}

void pushStiva(StivaModificari* stiva, Celula* modificari_generatie){
    if (stiva->varf == stiva->capacitate){
        stiva->capacitate *= 2;
        Celula** elemente_temp = (Celula**)realloc(stiva->elemente, stiva->capacitate * sizeof(Celula*));
        if (!elemente_temp){
            printf("eroare realocare memorie pentru stiva Task 2 in pushStiva\n");
            for(int i=0; i<stiva->varf; ++i)
                if(stiva->elemente[i] != NULL) 
                    elibereazaListaCelule(stiva->elemente[i]);
            
            free(stiva->elemente);
            exit(1);
        }
        stiva->elemente = elemente_temp;
    }
    stiva->elemente[stiva->varf++] = modificari_generatie;
}

void elibereazaMemorieStiva(StivaModificari* stiva){ 
    if (stiva->elemente){
        for (int i = 0; i < stiva->varf; i++)
            if(stiva->elemente[i] != NULL) 
                elibereazaListaCelule(stiva->elemente[i]);
        
        free(stiva->elemente);
        stiva->elemente = NULL;
    }
    stiva->varf = 0;
    stiva->capacitate = 0;
}

void executaTask2(FILE* fout, int linii, int coloane, int knrgen, const char matrice_initiala[linii][coloane+1]){
    StivaModificari stiva_modificari;
    initStiva(&stiva_modificari, knrgen > 0 ? knrgen : 10);

    char matrice_curenta_t2[linii][coloane + 1];
    char matrice_urmatoare_t2[linii][coloane + 1];
    for (int i = 0; i < linii; i++) {
        matrice_urmatoare_t2[i][0] = '\0'; 
    }

    for (int i = 0; i < linii; i++)
        if (strlen(matrice_initiala[i]) == 0)
            matrice_curenta_t2[i][0] = '\0';
        else 
            strcpy(matrice_curenta_t2[i], matrice_initiala[i]);

    char (*p_crt_t2)[coloane+1] = matrice_curenta_t2;
    char (*p_urm_t2)[coloane+1] = matrice_urmatoare_t2;

    for (int k = 0; k < knrgen; k++){
        Celula* modificari_gen_k = NULL;
        evoluareGeneratieStandard(linii, coloane, p_crt_t2, p_urm_t2, &modificari_gen_k); 
        pushStiva(&stiva_modificari, modificari_gen_k); 

        char (*temp_ptr_t2)[coloane+1] = p_crt_t2;
        p_crt_t2 = p_urm_t2;
        p_urm_t2 = temp_ptr_t2;
    }

    for (int k = 0; k < stiva_modificari.varf; k++){ 
        fprintf(fout, "%d", k + 1); 

        Celula* mod_curent = stiva_modificari.elemente[k];
        if (mod_curent != NULL) {
            while (mod_curent != NULL) {
                fprintf(fout, " %d %d", mod_curent->linie, mod_curent->coloana); 
                mod_curent = mod_curent->urm;
            }
        }
        fprintf(fout, "\n"); 
    }
    
    elibereazaMemorieStiva(&stiva_modificari);
}

#endif
