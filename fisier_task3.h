#ifndef FISIER_TASK3_H
#define FISIER_TASK3_H

#include "fisier_comun.h" 
#include "fisier_task1.h" 

/*

 task 3: Construieste un arbore cu toate starile matricei aplicand doua reguli diferite 
        si parcurge arborele in preordine pentru afisarea matricelor

Functiile principale:
evoluareGeneratieB() - aplica regula alternativa (celula devine vie daca are exact 2 vecini vii)
creazaNod() - aloca si initializeaza un nod nou in arbore
construiesteArbore() - construieste recursiv arborele de stari
parcurgerePreordine() - parcurge arborele in preordine si afiseaza matricele
elibereazaArbore() - elibereaza memoria alocata pentru arbore

*/

//evoluare generatie cu regula B
void evoluareGeneratieB(int linii, int coloane, char matrice[linii][coloane+1], char matriceNoua[linii][coloane+1], Celula** modificari){
    for (int i = 0; i < linii; i++){
        for (int j = 0; j < coloane; j++){
            int veciniVii = numaraVeciniVii(i, j, linii, coloane, matrice); 
            
            if (veciniVii == 2) 
                matriceNoua[i][j] = 'X';
            else 
                matriceNoua[i][j] = matrice[i][j]; 
            
            if (matriceNoua[i][j] != matrice[i][j]) 
                *modificari = adaugaCelula(*modificari, i, j);
        }
        matriceNoua[i][coloane] = '\0';
    }
}

//creeaza un nod nou pentru arbore
Nod* creazaNod(int linii, int coloane, char matrice_init[linii][coloane+1]){
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    if (!nou){
        printf("eroare alocare memorie pentru nod in creazaNod\n"); exit(1);}

    nou->matrice = copieMatrice(linii, coloane, matrice_init);
    nou->modificari = NULL;
    nou->stanga = NULL;
    nou->dreapta = NULL;
    return nou;
}

//construieste arborele recursiv
Nod* construiesteArbore(int linii, int coloane, char matrice_curenta[linii][coloane+1], int nivelCurent, int nivelMaxim){
    if(nivelCurent > nivelMaxim)
        return NULL;
        
    Nod* nod = creazaNod(linii, coloane, matrice_curenta);
    
    if (nivelCurent == 0) {
        // nod->modificari este initial NULL din creazaNod
        for (int i = 0; i < linii; i++) 
            for (int j = 0; j < coloane; j++) 
                if (matrice_curenta[i][j] == 'X') 
                    nod->modificari = adaugaCelula(nod->modificari, i, j);
    }
    
    if(nivelCurent == nivelMaxim)
        return nod; // am ajuns la adancimea maxima returnam nodul frunza
        
    char matriceB[linii][coloane+1];
    char matriceStandard[linii][coloane+1];
    Celula* modificariB = NULL;
    Celula* modificariStandard = NULL;

    evoluareGeneratieB(linii, coloane, matrice_curenta, matriceB, &modificariB);
    nod->stanga = construiesteArbore(linii, coloane, matriceB, nivelCurent + 1, nivelMaxim);
    if(nod->stanga) 
        nod->stanga->modificari = modificariB;
    else 
        elibereazaListaCelule(modificariB); 
    
    
    evoluareGeneratieStandard(linii, coloane, matrice_curenta, matriceStandard, &modificariStandard);
    nod->dreapta = construiesteArbore(linii, coloane, matriceStandard, nivelCurent + 1, nivelMaxim);
    if(nod->dreapta) 
        nod->dreapta->modificari = modificariStandard;
    else 
        elibereazaListaCelule(modificariStandard);
    
    return nod;
}

//parcurge arborele in preordine si afiseaza matricele
void parcurgerePreordine(FILE* fout, Nod* nod, int linii, int coloane){
    if(!nod)
        return;
        
    afisareMatriceDinamica(fout, linii, nod->matrice); 
    
    parcurgerePreordine(fout, nod->stanga, linii, coloane);
    parcurgerePreordine(fout, nod->dreapta, linii, coloane);
}

//elibereaza memoria pentru arbore
void elibereazaArbore(Nod* nod, int linii){
    if(!nod)
        return;
        
    elibereazaArbore(nod->stanga, linii);
    elibereazaArbore(nod->dreapta, linii);
    
    elibereazaMatrice(nod->matrice, linii);
    elibereazaListaCelule(nod->modificari); 
    free(nod);
}

#endif 
