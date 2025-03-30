#include <stdio.h>
#include <string.h>

// citirea matricii 
void citireMatrice(FILE *fin, int linii, int coloane, char matrice[linii][coloane+1]){
    for (int i = 0; i < linii; i++){
        if (fgets(matrice[i], coloane+1, fin) != NULL){
            size_t len = strlen(matrice[i]);
            if (len > 0 && matrice[i][len-1] == '\n'){
                matrice[i][len-1] = '\0';
            } else{
                int ch;
                while ((ch = fgetc(fin)) != '\n' && ch != EOF); // golim restul liniei daca a fost trunchiata
            }
        }
    }
}

// afiseara matrucei
void afisareMatrice(FILE *fout, int linii, int coloane, char matrice[linii][coloane+1]){
    for (int i = 0; i < linii; i++)
        fprintf(fout, "%s\n", matrice[i]);

    fprintf(fout, "\n");
}

// numara vecinii vii
int numaraVeciniVii(int i, int j, int linii, int coloane, char matrice[linii][coloane+1]){
    int veciniVii = 0;
    for (int i2 = -1; i2 <= 1; i2++){
        for (int j2 = -1; j2 <= 1; j2++){
            if (!(i2 == 0 && j2 == 0)) { 
                int i3 = i + i2;
                int j3 = j + j2;

                if (i3 >= 0 && i3 < linii && j3 >= 0 && j3 < coloane)
                    if (matrice[i3][j3] == 'X')
                    veciniVii++;
            }   
        }
    }
    return veciniVii;
}

// determinarea starii celulei cu ajutorul functiei numaravecinivii
void evoluareGeneratie(int linii, int coloane, char matrice[linii][coloane+1], char matriceNoua[linii][coloane+1]){
    for (int i = 0; i < linii; i++){
        for (int j = 0; j < coloane; j++){
            int veciniVii = numaraVeciniVii(i, j, linii, coloane, matrice);
            if (matrice[i][j] == 'X')  // celula este vie
                if (veciniVii < 2 || veciniVii > 3)
                    matriceNoua[i][j] = '+'; // moare
                else 
                    matriceNoua[i][j] = 'X'; // ramane vie
            else  // celula este moarta
                if (veciniVii == 3)
                    matriceNoua[i][j] = 'X'; // devine vie
                else 
                    matriceNoua[i][j] = '+'; // ramane moarta
        }
        matriceNoua[i][coloane] = '\0';
    }
}

int main(char const *argv[]) {

    FILE *fin = fopen("in/data2.in", "rt"); 
    FILE *fout = fopen("out/data2.out", "wt");  
    

    if (fin == NULL || fout == NULL){
        printf("Eroare la deschiderea fisierului.\n");
        return 1;
    }

    // citim numar task, numarul linii, numarul coloane si numarul de generatii.
    int task, linii, coloane, knrgen;
    fscanf(fin, "%d", &task);
    fscanf(fin, "%d", &linii);
    fscanf(fin, "%d", &coloane);
    fscanf(fin, "%d", &knrgen);
    
    char matrice[linii][coloane+1], matriceNoua[linii][coloane+1];

    //citire si afisare gen 0
    citireMatrice(fin, linii, coloane, matrice);
    afisareMatrice(fout, linii, coloane, matrice);

    // procesam fiecare generatie & o retinem si afisam
    for (int k = 0; k < knrgen; k++){
        evoluareGeneratie(linii, coloane, matrice, matriceNoua);

        for (int i = 0; i < linii; i++)
            strcpy(matrice[i], matriceNoua[i]);

        afisareMatrice(fout, linii, coloane, matrice);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
