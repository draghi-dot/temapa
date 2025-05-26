#ifndef FISIER_COMUN_H
#define FISIER_COMUN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
fisier cu structuri de date si functii comune pentru Game of Life
 
structuri:
celula: reprezinta o pozitie in matrice care isi schimba starea
nod: reprezinta un nod in arborele de evolutie a matricei

functii:
adaugaCelula: adauga o celula in lista sortata dupa pozitie
citireMatrice: citeste o matrice din fisier
afisareMatrice: afiseaza matrice cu dimensiuni fixe
afisareMatriceDinamica: afiseaza matrice alocata dinamic
numaraVeciniVii: calculeaza numarul de vecini vii ai unei celule
copieMatrice: creeaza o copie dinamica a unei matrici
elibereazaMatrice: elibereaza memoria alocata pentru o matrice
elibereazaListaCelule: elibereaza memoria alocata pentru o lista de celule

*/


//structuri pentru stiva si celula
typedef struct Celula{
    int linie, coloana;
    struct Celula* urm;
}Celula;

//structura nod arbore
typedef struct Nod{
    char** matrice;
    Celula* modificari;
    struct Nod* stanga; //regula B stanga
    struct Nod* dreapta; //reguli standard dreapta
}Nod;

//adauga o celula in lista
Celula* adaugaCelula(Celula* lista, int linie, int coloana){
    Celula* nou = (Celula*)malloc(sizeof(Celula));
    if (!nou) { 
        printf("eroare alocare memorie in adaugaCelula\n"); 
        exit(1); 
    }
    nou->linie = linie;
    nou->coloana = coloana;
    nou->urm = NULL;

    if(!lista || linie < lista->linie || (linie == lista->linie && coloana < lista->coloana)){
        nou->urm = lista;
        return nou;
    }
    Celula* curent = lista;
    while(curent->urm && (curent->urm->linie < linie || (curent->urm->linie == linie && curent->urm->coloana <coloana)))
        curent = curent->urm;
    nou->urm = curent->urm;
    curent->urm = nou;
    return lista;
}

//citirea matricii 
void citireMatrice(FILE *fin, int linii, int coloane, char matrice[linii][coloane+1]){
    for(int i = 0; i < linii; i++){
        if(fgets(matrice[i], coloane + 1, fin) != NULL){ 
            char *p_newline = strchr(matrice[i], '\n');
            if (p_newline != NULL) {
                *p_newline = '\0'; 
            }else{
                if (!feof(fin) && !ferror(fin)) { 
                    int ch_extra;
                    while((ch_extra = fgetc(fin)) != '\n' && ch_extra != EOF);
                }
            }
        } else {
            matrice[i][0] = '\0'; 
        }
    }
}

//afisarea matricei
void afisareMatrice(FILE *fout, int linii, int coloane, const char matrice[linii][coloane+1]){
    for(int i = 0; i < linii; i++)
        fprintf(fout, "%s\n", matrice[i]);
    fprintf(fout, "\n");
}

//afisarea matricei dinamice
void afisareMatriceDinamica(FILE *fout, int linii, char** matrice){
    if (!matrice) return;
    for(int i = 0; i < linii; i++) {
        if (matrice[i]) fprintf(fout, "%s\n", matrice[i]);
    }
    fprintf(fout, "\n");
}

//numara vecinii vii
int numaraVeciniVii(int i, int j, int linii, int coloane, const char matrice[linii][coloane+1]){
    int veciniVii = 0;
    for(int i2 = -1; i2 <= 1; i2++){
        for(int j2 = -1; j2 <= 1; j2++){
            if(!(i2 == 0 && j2 == 0)){ 
                int i3 = i + i2;
                int j3 = j + j2;
                if(i3 >= 0 && i3 < linii && j3 >= 0 && j3 < coloane)
                    if (matrice[i3][j3] == 'X')
                    veciniVii++;
            }   
        }
    }
    return veciniVii;
}

//aloca si copiaza o matrice
char** copieMatrice(int linii, int coloane, const char matrice_sursa[linii][coloane+1]){
    char** copie = (char**)malloc(linii * sizeof(char*));
    if (!copie) { printf("eroare alocare memorie in copieMatrice (char**)\n"); exit(1); }
    for(int i = 0; i < linii; i++){
        copie[i] = (char*)malloc((coloane + 1) * sizeof(char)); 
        if (!copie[i]) { 
            printf("eroare alocare memorie in copieMatrice (char* pentru linia %d)\n", i); 
            for(int k=0; k<i; k++) free(copie[k]);
            free(copie);
            exit(1); 
        }
        if (strlen(matrice_sursa[i]) == 0) { 
             printf("EROARE: matrice_sursa[%d] este NULL in copieMatrice!\n", i);
             copie[i][0] = '\0'; 
        } else {
            strcpy(copie[i], matrice_sursa[i]);
        }
    }
    return copie;
}

//elibereaza memoria pentru o matrice alocata dinamic
void elibereazaMatrice(char** matrice, int linii){
    if (!matrice) return;
    for(int i = 0; i < linii; i++)
        free(matrice[i]); 
    free(matrice);
}

//elibereaza memoria pentru o lista de celule
void elibereazaListaCelule(Celula* lista){
    while(lista){
        Celula* temp = lista; 
        lista = lista->urm;
        free(temp);
    }
}

#endif 
