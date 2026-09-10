// cria arrys para serem usados no benchmark
// os arays serão criados ordenados, de 0 a n.


#include <iostream>
#include <chrono>
#include <numeric>

#pragma once

using namespace std;

const long tamanhos_array[10] = {
    1000, 2000, 3000, 4000, 5000,
    6000, 7000, 8000, 9000, 10000
};

inline void cria_array(long tamanho, long lista[]) {
    for (long i = 0; i < tamanho; i++)
        lista[i] = i;
}

inline void imprime_tabela(string cabecalho, long tempos[][5]) {
    cout << "Algoritmo: " << cabecalho << endl;
    cout << "Tam. Array\tQ1(ns)\tQ2(ns)\tQ3(ns)\tQ4(ns)" << endl;
    int qtd = sizeof(tamanhos_array) / sizeof(tamanhos_array[0]);;
    for (int i = 0; i < qtd; i++) {
        cout << tempos[i][0];
        for (int j = 1; j < 5; j++)
            cout << "\t" << tempos[i][j];
    cout << endl;
    }
}

typedef int (*algoritmo_busca)(long[], long, long);

// executa o benchmark de um algoritmo de busca
inline long bechmark( algoritmo_busca algoritmo, long lista[], long tamanho, long valor) {
    // início da execução da função de busca
    auto start = std::chrono::high_resolution_clock::now();
    int pos = algoritmo(lista, tamanho, valor);
    auto end = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return duracao;
}