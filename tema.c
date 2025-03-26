#include <stdio.h>

int main(){
    FILE *file;

    file = fopen("in/data1.in", "r");

    //citire numar task
    int task;
    fscanf(file, "%d", &task);

    //citire nr coloane si linii
    int linii, coloane;
    fscanf(file, "%d", &linii);
    fscanf(file, "%d", &coloane);

    //citire nr generatiilor de calculat
    int knrgen;
    fscanf(file, "%d", &knrgen);

    //testam ce task avem de rezolvat
    if (t == 1){




    }



    return 0;
}