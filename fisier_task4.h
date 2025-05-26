#ifndef FISIER_TASK4_H
#define FISIER_TASK4_H

/*

Task 4: gaseste cele mai lungi lanturi sau cicluri de celule vii in fiecare componenta conexa

Functii principale:
suntVecineT4: verifica daca doua celule sunt vecine
comparaCaiLexicograficT4: compara doua cai lexicografic pentru ordonare
dfsPentruComponentaT4: algoritmul DFS pentru gasirea tuturor cailor hamiltoniene posibile
gasesteCeaMaiBunaCaleInComponentaT4: determina cel mai bun lant sau ciclu din componenta
normalizeazaCaleFinalaT4: normalizeaza calea pentru afisare consistenta
proceseazaMatricePentruTask4: identifica componentele conexe si lanturi/cicluri
executaTask4: parcurge arborele din Task 3 si ruleaza algoritmul pentru fiecare nod

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "fisier_comun.h" 

typedef struct{
    int r, c;
    int id_org; 
} VarfT4;

typedef struct{
    VarfT4* noduri;
    int** mat_adj; // matrice_adiacenta
    int nr_noduri;
    int* cale_crt; // cale_curenta
    int* cale_opt; // cale_optima
    int lung_opt; // lungime_optima
    int* vizitat;
} DateDFS;

int comparaCaiLexicograficT4(const VarfT4* cale1, const VarfT4* cale2, int lung){
    for(int i = 0; i < lung; ++i){
        if(cale1[i].r < cale2[i].r) return 1; 
        if(cale1[i].r > cale2[i].r) return -1; 
        if(cale1[i].c < cale2[i].c) return 1; 
        if(cale1[i].c > cale2[i].c) return -1; 
    }
    return 0; 
}

//aplica DFS pentru gasirea tuturor cailor posibile intr-o componenta
void dfsPentruComponentaT4(DateDFS* date, int nod_crt, int adanc){
    date->cale_crt[adanc - 1] = nod_crt; 
    date->vizitat[nod_crt] = 1;

    if(adanc == date->nr_noduri){ 
        if(date->lung_opt == 0){ 
            date->lung_opt = date->nr_noduri;
            memcpy(date->cale_opt, date->cale_crt, date->nr_noduri * sizeof(int));
        }else{ 
            VarfT4 cale_crt_coord[date->nr_noduri];
            VarfT4 cale_opt_coord[date->nr_noduri];
            for(int i=0; i < date->nr_noduri; ++i){
                cale_crt_coord[i] = date->noduri[date->cale_crt[i]];
                cale_opt_coord[i] = date->noduri[date->cale_opt[i]];
            }

            if(comparaCaiLexicograficT4(cale_crt_coord, cale_opt_coord, date->nr_noduri) > 0)
                memcpy(date->cale_opt, date->cale_crt, date->nr_noduri * sizeof(int));
        }
    }else{
        typedef struct{ 
            VarfT4 v_info; 
            int idx_org; 
        }VecinSortabil;

        VecinSortabil* vecini = (VecinSortabil*)malloc(date->nr_noduri * sizeof(VecinSortabil));
        if(!vecini && date->nr_noduri > 0){ 
            printf("eroare malloc vecini in DFS\n"); 
            return; 
        }
        
        int nr_vecini = 0;

        for(int v = 0; v < date->nr_noduri; ++v){
            if(date->mat_adj[nod_crt][v] && !date->vizitat[v]){
                vecini[nr_vecini].v_info = date->noduri[v];
                vecini[nr_vecini].idx_org = v;
                nr_vecini++;
            }
        }

        for(int i = 0; i < nr_vecini - 1; i++){
            for(int j = 0; j < nr_vecini - i - 1; j++){
                if(vecini[j].v_info.r > vecini[j+1].v_info.r || 
                  (vecini[j].v_info.r == vecini[j+1].v_info.r && 
                   vecini[j].v_info.c > vecini[j+1].v_info.c)){
                    VecinSortabil temp = vecini[j];
                    vecini[j] = vecini[j+1];
                    vecini[j+1] = temp;
                }
            }
        }
        
        for(int i = 0; i < nr_vecini; ++i){
            int vecin = vecini[i].idx_org;
            dfsPentruComponentaT4(date, vecin, adanc + 1);
            if(date->lung_opt == date->nr_noduri)
                 break; 
        }
        if(date->nr_noduri > 0 && vecini != NULL)
            free(vecini);
    }
    date->vizitat[nod_crt] = 0; 
}

int gasesteCeaMaiBunaCaleInComponentaT4(VarfT4* noduri, int nr_noduri, int** adj, VarfT4* out_cale){
    if(nr_noduri == 0) return 0;
    if(nr_noduri == 1){ 
        out_cale[0] = noduri[0];
        return 1;
    }

    DateDFS date;
    date.noduri = noduri;
    date.mat_adj = adj;
    date.nr_noduri = nr_noduri;
    date.cale_crt = (int*)malloc(nr_noduri * sizeof(int));
    date.cale_opt = (int*)malloc(nr_noduri * sizeof(int));
    date.vizitat = (int*)calloc(nr_noduri, sizeof(int));
    date.lung_opt = 0;

    if(!date.cale_crt || !date.cale_opt || !date.vizitat){
        printf("eroare alocare memorie in gasesteCeaMaiBunaCaleInComponentaT4\n");
        if(date.cale_crt) free(date.cale_crt);
        if(date.cale_opt) free(date.cale_opt);
        if(date.vizitat) free(date.vizitat);
        return 0;
    }

    for(int start = 0; start < nr_noduri; ++start){
        dfsPentruComponentaT4(&date, start, 1);
    }

    int lung_rez = 0;
    if(date.lung_opt == nr_noduri){ 
        for(int i = 0; i < nr_noduri; ++i)
            out_cale[i] = date.noduri[date.cale_opt[i]];

        lung_rez = nr_noduri;
    }

    free(date.cale_crt);
    free(date.cale_opt);
    free(date.vizitat);

    return lung_rez;
}

void normalizeazaCaleFinalaT4(VarfT4* path, int lung){ 
    if(lung <= 1) return; 

    VarfT4* path_inv = (VarfT4*)malloc(lung * sizeof(VarfT4));
    if(!path_inv){ 
        printf("eroare malloc in normalizeazaCaleFinalaT4\n"); 
        return; 
    }
    
    for(int i = 0; i < lung; i++) 
        path_inv[i] = path[lung - 1 - i];

    if(comparaCaiLexicograficT4(path_inv, path, lung) > 0) 
        memcpy(path, path_inv, lung * sizeof(VarfT4));

    free(path_inv);
}

void proceseazaMatricePentruTask4(char** mat, int linii, int coloane, FILE* fout){
    VarfT4 celule_vii[linii * coloane];
    int nr_celule_vii = 0;
    int map_id[linii][coloane]; 

    for(int i=0; i<linii; ++i) 
        for(int j=0; j<coloane; ++j){ 
            map_id[i][j] = -1; 
            if(mat[i][j] == 'X'){
                celule_vii[nr_celule_vii].r = i;
                celule_vii[nr_celule_vii].c = j;
                celule_vii[nr_celule_vii].id_org = nr_celule_vii;
                map_id[i][j] = nr_celule_vii;
                nr_celule_vii++;
            }
        }

    if(nr_celule_vii == 0){
        fprintf(fout, "-1\n\n"); 
        return;
    }

    int** adj_list = (int**)malloc(nr_celule_vii * sizeof(int*));
    if(!adj_list){
        printf("eroare malloc adj_list\n"); 
        fprintf(fout, "-1\n"); 
        return; 
    }
    for(int i=0; i<nr_celule_vii; ++i){
        adj_list[i] = (int*)calloc(nr_celule_vii, sizeof(int));
        if(!adj_list[i]){
            printf("eroare malloc adj_list[i]\n"); 
            for(int j=0; j<i; j++) {
                free(adj_list[j]);
            }
            free(adj_list);
            fprintf(fout, "-1\n");
            return; 
        } 
    }

    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1}; 
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int k = 0; k < nr_celule_vii; ++k){
        int r1 = celule_vii[k].r;
        int c1 = celule_vii[k].c;
        for(int move = 0; move < 8; ++move){
            int r2 = r1 + dr[move];
            int c2 = c1 + dc[move];
            if(r2 >= 0 && r2 < linii && c2 >= 0 && c2 < coloane && mat[r2][c2] == 'X'){
                int id_vecin = map_id[r2][c2];
                adj_list[k][id_vecin] = 1;
            }
        }
    }

    int* viz_bfs = (int*)calloc(nr_celule_vii, sizeof(int));
    if(!viz_bfs){ 
        printf("eroare malloc viz_bfs\n"); 
        for(int i=0; i<nr_celule_vii; ++i) 
            free(adj_list[i]);
        free(adj_list);
        fprintf(fout, "-1\n");
        return; 
    }
    
    int lung_max_cale = 0; 
    VarfT4* cale_max = NULL; 

    for(int start = 0; start < nr_celule_vii; ++start){
        if(!viz_bfs[start]){
            int id_nod_comp[nr_celule_vii]; 
            int nr_noduri_comp = 0;
            
            int coada[nr_celule_vii];
            int head = 0, tail = 0;
            
            coada[tail++] = start; 
            viz_bfs[start] = 1;
            id_nod_comp[nr_noduri_comp++] = start;

            while(head < tail){
                int u = coada[head++];
                for(int v = 0; v < nr_celule_vii; ++v){
                    if(adj_list[u][v] && !viz_bfs[v]){
                        viz_bfs[v] = 1;
                        coada[tail++] = v;
                        id_nod_comp[nr_noduri_comp++] = v;
                    }
                }
            }
            
            if(nr_noduri_comp > 0){
                VarfT4* noduri_comp = (VarfT4*)malloc(nr_noduri_comp * sizeof(VarfT4));
                if(!noduri_comp){
                    printf("eroare malloc noduri_comp\n"); 
           
                    continue;
                }
                for(int k=0; k<nr_noduri_comp; ++k) 
                    noduri_comp[k] = celule_vii[id_nod_comp[k]];
                
                int** adj_comp = (int**)malloc(nr_noduri_comp * sizeof(int*));
                if(!adj_comp){
                    printf("eroare malloc adj_comp\n"); 
                    free(noduri_comp);
                    continue; 
                }
                for(int r=0; r<nr_noduri_comp; ++r){
                    adj_comp[r] = (int*)calloc(nr_noduri_comp, sizeof(int));
                    if(!adj_comp[r]){
                        printf("eroare malloc adj_comp[r]\n"); 
                        free(noduri_comp);
                        free(adj_comp);
                        continue; 
                    }
                }

                for(int u=0; u<nr_noduri_comp; ++u){
                    for(int v=0; v<nr_noduri_comp; ++v){
                        if(adj_list[id_nod_comp[u]][id_nod_comp[v]]){
                           adj_comp[u][v] = 1;
                        }
                    }
                }
                
                VarfT4* cale_comp = (VarfT4*)malloc(nr_noduri_comp * sizeof(VarfT4));
                if(!cale_comp){
                    printf("eroare malloc cale_comp\n"); 
                    free(noduri_comp);
                    free(adj_comp);
                    continue; 
                }

                int lung_cale = gasesteCeaMaiBunaCaleInComponentaT4(noduri_comp, nr_noduri_comp, adj_comp, cale_comp);
                
                if(lung_cale == nr_noduri_comp){ 
                    normalizeazaCaleFinalaT4(cale_comp, lung_cale);

                    if(lung_cale > lung_max_cale){
                        lung_max_cale = lung_cale;
                        free(cale_max); 
                        cale_max = (VarfT4*)malloc(lung_max_cale * sizeof(VarfT4));
                        if(!cale_max){ 
                            printf("eroare malloc cale_max\n"); 
                            lung_max_cale = 0; 
                            continue; 
                        }
                        memcpy(cale_max, cale_comp, lung_max_cale * sizeof(VarfT4));
                    }else if(lung_cale == lung_max_cale && lung_max_cale > 0){
                        if(comparaCaiLexicograficT4(cale_comp, cale_max, lung_max_cale) > 0){
                             memcpy(cale_max, cale_comp, lung_max_cale * sizeof(VarfT4));
                        }
                    }
                }
                
                free(noduri_comp);
                for(int r=0; r<nr_noduri_comp; ++r) 
                    free(adj_comp[r]);
                free(adj_comp);
                free(cale_comp);
            }
        }
    }

    if(lung_max_cale > 0){
        normalizeazaCaleFinalaT4(cale_max, lung_max_cale); 
        
        fprintf(fout, "%d\n", lung_max_cale - 1); 
        
        for(int k = 0; k < lung_max_cale; ++k){ 
            fprintf(fout, "(%d,%d)%s", cale_max[k].r, cale_max[k].c, (k == lung_max_cale - 1) ? "" : " ");
        }
        fprintf(fout, "\n"); 
    }else  
        fprintf(fout, "-1\n");
    
    free(cale_max); 
    free(viz_bfs);
    for(int i=0; i<nr_celule_vii; ++i) 
        free(adj_list[i]);
    free(adj_list);
}

void parcurgereSiRuleazaTask4(Nod* nod, int linii, int coloane, FILE* fout, int nivel, int k_max){
    if(!nod || nivel > k_max)
        return;
    proceseazaMatricePentruTask4(nod->matrice, linii, coloane, fout); 
    parcurgereSiRuleazaTask4(nod->stanga, linii, coloane, fout, nivel + 1, k_max);
    parcurgereSiRuleazaTask4(nod->dreapta, linii, coloane, fout, nivel + 1, k_max);
}

void executaTask4(Nod* radacina, int linii, int coloane, int knrgen, FILE* fout){
    parcurgereSiRuleazaTask4(radacina, linii, coloane, fout, 0, knrgen);
}

#endif