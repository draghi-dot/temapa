#ifndef BONUS_H
#define BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fisier_comun.h" 

// functia care aplica invers modificarile unei generatii
// pornind de la matricea 'urmatoare' si lista de 'modificari'
// pentru a obtine matricea 'anterioara'.

void aplicaModificariInvers(int linii, int coloane, const char matrice_urmatoare[linii][coloane+1], Celula* modificari_directe, char matrice_anterioara[linii][coloane+1]) {
    // copiem matricea urmatoare in cea anterioara
    for (int i = 0; i < linii; i++) 
        strcpy(matrice_anterioara[i], matrice_urmatoare[i]);
    
    // aplicam modificarile in sens invers
    Celula* celula = modificari_directe;
    while (celula != NULL) {
        int r = celula->linie;
        int c = celula->coloana;
        
        if (r >= 0 && r < linii && c >= 0 && c < coloane) {
            // inversam starea celulei: vie -> moarta, moarta -> vie
            if (matrice_anterioara[r][c] == 'X')
                matrice_anterioara[r][c] = '+';
            else 
                matrice_anterioara[r][c] = 'X';
        }
        celula = celula->urm;
    }
}

void executaBonus(FILE* fout, int linii, int coloane, Celula** stiva_modificari_directe, int nr_generatii_in_stiva, char matrice_generatia_K[linii][coloane+1]){

    if (nr_generatii_in_stiva < 0)  
        return;
    
    fprintf(fout, "\nMatricea generatiei %d:\n", nr_generatii_in_stiva);
    afisareMatrice(fout, linii, coloane, matrice_generatia_K);

    char matCurenta[linii][coloane+1];
    char matTemp[linii][coloane+1]; 
    for (int i = 0; i < linii; i++)
        matTemp[i][0] = '\0';

    for (int i = 0; i < linii; i++) 
        if (matrice_generatia_K[i] != NULL) 
            strcpy(matCurenta[i], matrice_generatia_K[i]);
        else 
            matCurenta[i][0] = '\0';
        
    

    char (*pCurenta)[coloane+1] = matCurenta;    
    char (*pAnterioara)[coloane+1] = matTemp; 

    for (int idx = nr_generatii_in_stiva - 1; idx >= 0; idx--){
        Celula* modificari = stiva_modificari_directe[idx];
        
        aplicaModificariInvers(linii, coloane, pCurenta, modificari, pAnterioara);
        
        char (*pAux)[coloane+1] = pCurenta;
        pCurenta = pAnterioara;
        pAnterioara = pAux;

        fprintf(fout, "\nMatricea reconstruita pentru gen %d:\n", idx);
        afisareMatrice(fout, linii, coloane, pCurenta);
    }

    fprintf(fout, "\nMatricea initiala(Gen 0):\n");
    afisareMatrice(fout, linii, coloane, pCurenta);
}

#endif