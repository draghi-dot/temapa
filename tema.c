#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

//includem headerele necesare pentru taskuri
#include "fisier_comun.h" // fisier cu structuri si functii comune
#include "fisier_task1.h"
#include "fisier_task2.h"
#include "fisier_task3.h" 
#include "fisier_task4.h" 

int main(const int argc, char const *argv[]){
    FILE *fin;
    FILE *fout;

    if ((fin = fopen(argv[1], "rt")) == NULL){
        printf("fisierul de intrare nu poate fi deschis \n");
        exit(1);
    }

    if ((fout = fopen(argv[2], "w")) == NULL){
        printf("fisierul de iesire nu poate fi deschis \n");
        exit(1);
    }
    

    int task, linii, coloane, knrgen; 

    fscanf(fin, "%d", &task);
    fscanf(fin, "%d", &linii);
    fscanf(fin, "%d", &coloane);
    fscanf(fin, "%d", &knrgen);
    
    int ch_consum; 
    while ((ch_consum = fgetc(fin)) != '\n' && ch_consum != EOF);

    char matrice_initiala[linii][coloane+1];
    
    citireMatrice(fin, linii, coloane, matrice_initiala); 
    
         if (task == 1) {
        //1. afisare stare initiala a matricei
        afisareMatrice(fout, linii, coloane, matrice_initiala); 

        //2. calculeza evolutia matricei pentru knrgen generatii
        if (knrgen > 0){
            char matrice_curenta_t1[linii][coloane + 1];
            char matrice_urmatoare_t1[linii][coloane + 1];
            Celula* modificari_t1 = NULL;

            for (int i = 0; i < linii; i++) 
                strcpy(matrice_curenta_t1[i], matrice_initiala[i]);
            
            for (int i = 0; i < linii; i++) {
                matrice_urmatoare_t1[i][0] = '\0'; 
            }
            
            char (*p_crt_t1)[coloane+1] = matrice_curenta_t1;
            char (*p_urm_t1)[coloane+1] = matrice_urmatoare_t1;

            for (int k = 0; k < knrgen; k++){ 
                evoluareGeneratieStandard(linii, coloane, p_crt_t1, p_urm_t1, &modificari_t1); 
                afisareMatrice(fout, linii, coloane, p_urm_t1); 
                elibereazaListaCelule(modificari_t1);       
                modificari_t1 = NULL;

                char (*temp_ptr_t1)[coloane+1] = p_crt_t1;
                p_crt_t1 = p_urm_t1;
                p_urm_t1 = temp_ptr_t1;
            }
        }
    }else if (task == 2){
          // task 2: calculeaza si afiseaza doar celulele care isi schimba starea in fiecare generatie
        executaTask2(fout, linii, coloane, knrgen, matrice_initiala);

    } else if (task == 3) {
        //task 3: Construieste un arbore cu toate starile matricei si il parcurge in preordine
        Nod* radacina = construiesteArbore(linii, coloane, matrice_initiala, 0, knrgen); 
        parcurgerePreordine(fout, radacina, linii, coloane); 
        elibereazaArbore(radacina, linii); 

    } else if (task == 4) {
        //task 4: gaseste toate componentele conexe si determina cel mai lung lant/ciclu in fiecare
        Nod* radacina_t3 = construiesteArbore(linii, coloane, matrice_initiala, 0, knrgen);
        executaTask4(radacina_t3, linii, coloane, knrgen, fout);
        elibereazaArbore(radacina_t3, linii); 
    }
    
    fclose(fin);
    fclose(fout);
    return 0;
}