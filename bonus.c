#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fisier_comun.h"
#include "fisier_task1.h"
#include "bonus.h"

/*

Bonus: operatii inverse pentru Game of Life

acest fisier contine implementari pentru:

1. aplicaModificariInvers - Functie care modifica starea celulelor invers
    pentru a reconstrui o generatie anterioara
2. executaBonus - Functie principala care aplica modificari inverse in mod
    repetat pentru a reconstrui generatiile anterioare pana la cea initiala

*/


int main() {
    FILE *fin = fopen("in/data1.in", "rt");  
    FILE *fout = fopen("test_bonus.out", "wt");
    
    
    int task_id, linii, coloane, k;
    fscanf(fin, "%d", &task_id);
    fscanf(fin, "%d %d", &linii, &coloane);
    fscanf(fin, "%d", &k);

    char matrice_initiala[linii][coloane+1];
    char matrice_curenta[linii][coloane+1];
    char matrice_urmatoare[linii][coloane+1];

    citireMatrice(fin, linii, coloane, matrice_initiala);
    fclose(fin);
    
    fprintf(fout, "Matricea initiala\n");
    afisareMatrice(fout, linii, coloane, matrice_initiala);
    
    for (int i = 0; i < linii; i++) 
        strcpy(matrice_curenta[i], matrice_initiala[i]);
    
    Celula** stiva_modificari = (Celula**)malloc(k * sizeof(Celula*));
    for (int i = 0; i < k; i++) 
        stiva_modificari[i] = NULL;
    
    
    for (int gen = 0; gen < k; gen++){
        evoluareGeneratieStandard(linii, coloane, matrice_curenta, matrice_urmatoare, &(stiva_modificari[gen]));
        
        fprintf(fout, "\nGeneratia %d:\n", gen + 1);
        afisareMatrice(fout, linii, coloane, matrice_urmatoare);

        for (int i = 0; i < linii; i++) 
            strcpy(matrice_curenta[i], matrice_urmatoare[i]);
    }
    
    fprintf(fout, "\nreconstruire matrice initiala\n");
    executaBonus(fout, linii, coloane, stiva_modificari, k, matrice_curenta);
    
    for (int i = 0; i < k; i++)
        elibereazaListaCelule(stiva_modificari[i]);
    
    free(stiva_modificari);
    
    fclose(fout);
    
    return 0;
}