#include <iostream>
#include <iterator>
#include "minhas_funcoes.h"

using namespace std;

// algoritmo de busa linear interativa
int busca_linear(long lista[], long tamanho, long elem) {
    //long tamanho =  size(lista) / sizeof(lista[0]);
    for (int i = 0; i < tamanho; i++) {
        if (lista[i] == elem) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
   // para cada tamanho de array
    int qtd_testes = sizeof(tamanhos_array) / sizeof(tamanhos_array[0]);
    long tempos[qtd_testes][5];
    for (int i = 0; i < qtd_testes; i++){
        long tamanho = tamanhos_array[i];
        long lista[tamanho];
        cria_array(tamanho, lista);
        // executa o bench
        tempos[i][0] = tamanho;
        // o valor está no "primeiro quartil"
        tempos[i][1] = bechmark(busca_linear, lista, tamanho, tamanho/4);
        // o valor está no "primeiro quartil"
        tempos[i][2] = bechmark(busca_linear, lista, tamanho, tamanho/2);
        // o valor está no "primeiro quartil"
        tempos[i][3] = bechmark(busca_linear, lista, tamanho, 3 * tamanho/4);
        // o valor está no "primeiro quartil"
        tempos[i][4] = bechmark(busca_linear, lista, tamanho, tamanho + 1);

    }
    imprime_tabela("Busca Linear Iterativa", tempos);
	return 0;
}
