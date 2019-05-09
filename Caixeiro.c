#include <stdio.h>
#include <stdlib.h>

typedef struct Tabela{
    int v;
    int custo;
}tabela;

int caixeiro_viajante(int n, int sol[n], int D[n][n]);
int conjuntos(int n, int S, int conj[S], int v, int u, int dist);
int subconjunto(int n, int S, int M, int m, int i, int conj[S], int D[n][n], tabela G[n][M], int *min);
void solucao(int n, int M, int sol[n], tabela G[n][M], int min);
int bitmap(int S, int conj[S]);

int main(){
    int i, j;
    int n;

    /// Lendo o arquivo
    FILE * arq;
    arq = fopen("pcv04.txt", "r");

    if(arq == 0){
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    fscanf(arq, "%d", &n); /// Quantidade de vertices

    int D[n][n]; /// Matriz de Distancias

    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            fscanf(arq, "%d", &D[i][j]);
        }
    }

    fclose(arq);

    /// Caixeiro Viajante
    int sol[n];
    printf("%d", caixeiro_viajante(n, sol, D));
    printf("\nCaminho: ");
    for(i=0; i<n; i++){
        printf("%d ", sol[i]);
    }

    printf("\n\n");
    system("PAUSE");
    return 0;
}

int caixeiro_viajante(int n, int sol[n], int D[n][n]){
    int i, S, j;
    int aux, dist;
    int m;
    int M = 1 << 0;
    int Min[n][n]; /// Variavel auxiliar para formar o cmainho

    for(i=1; i<n; i++){
        M = M | 1<<i;
    }

    tabela G[n][M];

    /// Passo Inicial: |S|=0
    m = 1 << 0;
    for(i=1; i<n; i++){
        G[i][m].custo = D[i][0];
        G[i][m].v = 0;
    }

    for(S=1; S<n-1; S++){ ///Tamanho de cada conjunto
        int conj[S];
        for(i=1; i<n; i++){ /// Percorre todos os vertices com excecao do vertice fonte
            for(j=1; j<n; j++){
                if(j!=i){
                    dist = 1;
                    do{
                        /// Obtendo os Conjuntos
                        aux = conjuntos(n, S, conj, i, j, dist);
                        dist++;

                        if(aux == -3 || aux == -1){
                            m = bitmap(S, conj); /// Posicao do conjunto na matriz

                            if(S==1){
                                G[i][m].custo = D[i][conj[0]] + G[conj[0]][1].custo;
                                G[i][m].v = conj[0];
                            }
                            else{
                                subconjunto(n, S, M, m, i, conj, D, G, -1);
                            }
                        }
                    }
                    while(aux == -1 || aux == -4);
                }
            }
        }
    }

    /// Chamada Final
    int conj[S];
    int custo;
    int min;
    j = 0;
    for(i=1; i<n; i++){
        conj[j] = i;
        j++;
    }
    m = bitmap(S, conj);
    custo = subconjunto(n, S, M, m, 0, conj, D, G, &min);
    solucao(n, M, sol, G, min);

    return custo;
}


int conjuntos(int n, int S, int conj[S], int v, int u, int dist){
    int i=0;
    int k;
    int vezes = S;
    int j;

    if(S==1){
        conj[i] = u;
        return -3;
    }
    else{
        conj[i] = u;
        i++;

        k=u+dist;
        if(k!=v && k<n){
            conj[i] = k;
            i++;
            vezes--;
            if(vezes==1)
                return -1;
        }
        else if(k<n)
            return -4;
    }
    return -2;
}

int subconjunto(int n, int S, int M, int m, int i, int conj[S], int D[n][n], tabela G[n][M], int *min){
    int k;
    int pos;
    int vezes=1;
    int aux;

    for(k=0; k<S; k++){
        int r;
        int j=0;
        int conj2[S-1];

        for(r=0; r<S; r++){
            if(r!=k){
                conj2[j] = conj[r]; /// Subconjunto de "conj"
                j++;
            }
            else
                pos=conj[k];
        }

        r = bitmap(S-1, conj2); /// Acessando o subconjunto na matriz G
        if(vezes==1){
            G[i][m].custo = D[i][pos] + G[pos][r].custo;
            G[i][m].v = pos;
            if(i==0)
                *min = pos;
            vezes++;
        }
        else{
            aux = D[i][pos] + G[pos][r].custo;
            if(aux < G[i][m].custo){
                G[i][m].custo = aux;
                G[i][m].v = pos;
                if(i==0)
                    *min = pos;
            }
        }
    }

    return G[i][m].custo;
}


void solucao(int n, int M, int sol[n], tabela G[n][M], int min){
    int i, k, m, j, r;
    int tam = n-2;
    int vezes;

    sol[0] = 0;
    sol[1] = min;

    for(i=2; i<n; i++){
        int conj[tam];

        k=0;
        for(j=1; j<n; j++){
            vezes=0;
            for(r=0; r<i; r++){
                if(j==sol[r]){
                    vezes++;
                    break;
                }
            }

            if(vezes==0){
                conj[k] = j;
                k++;
                if(k==tam)
                    break;
            }
        }

        m = bitmap(tam, conj);
        sol[i] = G[min][m].v;
        tam--;
    }
}


int bitmap(int S, int conj[S]){
    int i;
    int m = 1 << 0;

    for(i=0; i<S; i++){
        m = m | 1 << conj[i];
    }

    return m;
}
