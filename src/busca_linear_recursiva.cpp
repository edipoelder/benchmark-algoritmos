#include <iostream>
#include <iterator>
#include "minhas_funcoes.h"

using namespace std;

// algoritmo de busa linear interativa
int busca_linear_recursiva(long lista[], long tamanho, long idx, long elem) {

    // Base Case
    if (idx == tamanho) {
        return -1;
    }

    // Check the current element of the array
    else if (lista[idx] == elem) {

        // If it matches the key, return its index
        return idx;
    }

    // Recursive Call
    return busca_linear_recursiva(lista, tamanho, idx + 1, elem);
}

int busca_linear_recursiva(long lista[], long tamanho, long elem) {
    return busca_linear_recursiva(lista, tamanho, 0, elem);
}

int main(int argc, char **argv)
{
    // para cada tamanho de array
    int qtd = sizeof(tamanhos_array) / sizeof(tamanhos_array[0]);
    long tempos[qtd][5];

    // 5 testes para calcular a média
    for (int i = 0; i < qtd; i++){
        long tamanho = tamanhos_array[i];
        long lista[tamanho];
        cria_array(tamanho, lista);
        // zera os tempos
            for (int j = 0; j < 5; j++)
        tempos[i][j] = 0;
        // executa o bench
        tempos[i][0] = tamanho;
        // o valor está no "primeiro quartil"
        tempos[i][1] += bechmark(busca_linear_recursiva, lista, tamanho, 1);
        // o valor está no "primeiro quartil"
        tempos[i][2] += bechmark(busca_linear_recursiva, lista, tamanho, tamanho/2);
        // o valor está no "primeiro quartil"
        tempos[i][3] += bechmark(busca_linear_recursiva, lista, tamanho, 3 * tamanho/4);
        // o valor está no "primeiro quartil"
        tempos[i][4] += bechmark(busca_linear_recursiva, lista, tamanho, tamanho + 1);
    }
    imprime_tabela("Busca Linear Recursiva", tempos);
    return 0;
}
