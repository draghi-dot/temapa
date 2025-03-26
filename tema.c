#include <stdio.h>
#include <string.h>

void citireMatrice(FILE *fin, int linii, int coloane, char matrice[linii][coloane]) {
    for (int i = 0; i < linii; i++) {
        fgets(matrice[i], coloane, fin);
    }
}

void afisareMatrice(FILE *fout, int linii, int coloane, char matrice[linii][coloane]) {
    for (int i = 0; i < linii; i++) 
        fprintf(fout, "%s", matrice[i]);  
}

//functie de numarare a vecinilor vii
int numaraVeciniVii(int i, int j, int linii, int coloane, char matrice[linii][coloane]) {
    int veciniVii = 0;

    for (int i2 = -1; i2 <= 1; i2++) {
        for (int j2 = -1; j2 <= 1; j2++) {
            // ignoram celula curentă i2 = 0 si j2 = 0
            if (!(i2 == 0 && j2 == 0)) { 
                int i3 = i + i2;
                int j3 = j + j2;
                
                // verificam daca elementul este in matrice
                if (i3 >= 0 && i3 < linii && j3 >= 0 && j3 < coloane) 
                    if (matrice[i3][j3] == 'X') // dacă vecinul este viu
                        veciniVii++;         
            }
        }
    }
    return veciniVii;
}

//testam fiecare celula si decidem daca ramane vie, moare sau devine vie bazat pe numarul de vecini vii
void evoluareGeneratie(int linii, int coloane, char matrice[linii][coloane], char matriceNoua[linii][coloane]) {
    for (int i = 0; i < linii; i++) {
        for (int j = 0; j < coloane; j++) {
            int veciniVii = numaraVeciniVii(i, j, linii, coloane, matrice);
            
            if (matrice[i][j] == 'X')// celula este vie
                if (veciniVii < 2 || veciniVii > 3) 
                    matriceNoua[i][j] = '+'; // moare
                 else 
                    matriceNoua[i][j] = 'X'; // ramane vie
                
            else // celula este moartă
                if (veciniVii == 3) 
                    matriceNoua[i][j] = 'X'; // devine vie 
                else 
                    matriceNoua[i][j] = '+'; // ramane moarta
        }
    }
}

int main() {
    FILE *fin = fopen("in/data1.in", "r"); 
    FILE *fout = fopen("out/data1.out", "w");  

    if (fin == NULL || fout == NULL) {
        printf("Eroare la deschiderea fișierului.\n");
        return 1;
    }

    // citire nr task
    int task;
    fscanf(fin, "%d", &task);

    // citire nr coloane linii
    int linii, coloane;
    fscanf(fin, "%d", &linii);
    fscanf(fin, "%d", &coloane);

    // citire nr generatii
    int knrgen;
    fscanf(fin, "%d", &knrgen);

    char matrice[linii][coloane], matriceNoua[linii][coloane];

    // citire matricea initiala
    citireMatrice(fin, linii, coloane, matrice);

    // procesam generatiile
    for (int k = 0; k < knrgen; k++) {
        // afisare matrice curenta
        afisareMatrice(fout, linii, coloane, matrice);

        // genereaza matricea noua
        evoluareGeneratie(linii, coloane, matrice, matriceNoua);

        // copiaza matricea noua in matrice pt urmatoarea generatie
        for (int i = 0; i < linii; i++) 
            for (int j = 0; j < coloane; j++) 
                matrice[i][j] = matriceNoua[i][j];
            
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
