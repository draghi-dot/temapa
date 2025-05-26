#ifndef FISIER_TASK1_H
#define FISIER_TASK1_H

#include "fisier_comun.h" 

/*
calculeaza o noua generatie a matricei conform regulilor Game of Life:
o celula vie (X) cu mai putin de 2 vecini vii moare (devine +)
o celula vie (X) cu 2 sau 3 vecini vii ramane vie
o celula vie (X) cu mai mult de 3 vecini vii moare (devine +)
o celula moarta (+) cu exact 3 vecini vii devine vie (devine X)
*/

//evoluare generatie cu reguli standard
void evoluareGeneratieStandard(int linii, int coloane, char matrice_orig[linii][coloane+1], char matrice_noua[linii][coloane+1], Celula** modificari){
    *modificari = NULL; 
    for(int i = 0; i < linii; i++){
        for(int j = 0; j < coloane; j++){
            int veciniVii = numaraVeciniVii(i, j, linii, coloane, matrice_orig);
            if(matrice_orig[i][j] == 'X')   
                if(veciniVii < 2 || veciniVii > 3)
                    matrice_noua[i][j] = '+'; 
                else 
                    matrice_noua[i][j] = 'X'; 
            else    
                if(veciniVii == 3)
                    matrice_noua[i][j] = 'X'; 
                else 
                    matrice_noua[i][j] = '+'; 

            if(matrice_noua[i][j] != matrice_orig[i][j])
                *modificari = adaugaCelula(*modificari, i, j);
        }
        matrice_noua[i][coloane] = '\0';
    }
}

#endif 