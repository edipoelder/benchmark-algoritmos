#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

using namespace std;

const int START_SIZE = 0;
const int END_SIZE = 50000;
const int STEP = 1000;
const int TRIALS = 10;
// tamanho do salto na busca alternativa (2 em 2, conforme o slide de Selan)
const int SALTO = 2;

// define o algoritmo de busca
typedef bool (*algoritmo_busca)(const vector<int>&, int, int, int);

// protótipo das funções de buscas que serão executadas:
// parâmetros: const std::vector<int> &vec, int begin, int end, int target
bool busca_linear_iterativa(const std::vector<int>&, int, int, int);
bool busca_linear_recursiva(const std::vector<int>&, int, int, int);
bool busca_binaria_iterativa(const std::vector<int>&, int, int, int);
bool busca_binaria_recursiva(const std::vector<int>&, int, int, int);
bool busca_linear_alternativa_iterativa(const std::vector<int>&, int, int, int);
bool busca_linear_alternativa_recursiva(const std::vector<int>&, int, int, int);
bool busca_ternaria_iterativa(const std::vector<int>&, int, int, int);
bool busca_ternaria_recursiva(const std::vector<int>&, int, int, int);

// Define um vetor com os algoritmos a serem executados no loop:
vector<pair<string, algoritmo_busca>> funcoes = {
	{ "Busca Linear Iterativa",  busca_linear_iterativa  },
	{ "Busca Linear Recursiva",  busca_linear_recursiva  },
	{ "Busca Linear Alternativa Iterativa", busca_linear_alternativa_iterativa},
	{ "Busca Linear Alternativa Recursiva", busca_linear_alternativa_recursiva},
	{ "Busca Binaria Iterativa", busca_binaria_iterativa },
	{ "Busca Binaria Recursiva", busca_binaria_recursiva },
	{ "Busca Ternária Iterativa", busca_ternaria_iterativa },
	{ "Busca Ternária Recursiva", busca_ternaria_recursiva },
};

// Par (nome, ponteiro para a função) que será usado no loop de execução:
using Algoritmo = std::pair<std::string, algoritmo_busca>;


//1. Busca Linear Iterativa, adaptada do código presente no PDF para usar begin/end:
bool busca_linear_iterativa(const std::vector<int> &vec, int begin, int end, int target) {
	for (int i = begin; i < end; i++) {
		if (vec[i] == target) {
			return true;
		}
	}
	return false;
}

//2. Busca Linear Recursiva
bool busca_linear_recursiva(const std::vector<int> &vec, int idx, int end, int target) {
    // condição de parada (não achou)
	if (idx >= end) return false;
	if (vec[idx] == target) {
		return true;
	} else if (idx < end) {
		return busca_linear_recursiva(vec, idx + 1, end, target);
	} else {
		return false;
	}
}

//3. Busca Sequencial Alternativa - Versão Iterativa.
bool busca_linear_alternativa_iterativa(const std::vector<int> &vec, int begin, int end, int target) {
	// tamanho do salto (2 em 2, conforme o slide de Selan)
	int i = begin;

	// pra frente, de salto em salto
	while (i < end && vec[i] < target) {
		i += SALTO;
	}

	// para trás de um em um
	for (int j = 0; j < SALTO && i - j >= begin; j++) {
		if (vec[i - j] == target)
			return true;
	}
	return false;
}

//4. Busca Sequencial Alternativa - Versão Recursiva.
// parte recursiva
bool busca_linear_alternativa_recursiva_salto(const std::vector<int> &vec, int begin, int end, int salto, int target) {
    // condição de parada (não achou)
    if (begin >= end) return false;

    if (vec[begin] < target) {
        int proximo = begin + salto;
        if (proximo >= end) proximo = end - 1;   // garante que para
        return busca_linear_alternativa_recursiva_salto(vec, proximo, end, salto, target);
    }

    // varre para trás
    for (int j = 0; j < salto && begin - j >= 0; ++j) {
        if (vec[begin - j] == target) return true;
    }
    return false;
}

bool busca_linear_alternativa_recursiva(const std::vector<int> &vec, int begin, int end, int target) {
	return busca_linear_alternativa_recursiva_salto(vec, begin, end, SALTO, target);
}

//5. Busca Binária Iterativa.
bool busca_binaria_iterativa(const std::vector<int> &vec, int begin, int end, int target) {
    while (begin <= end) {
        int idx = begin + (end - begin) / 2;  // evita overflow e calcula o meio real

        if (vec[idx] == target)
            return true;
        else if (vec[idx] > target)
            end = idx - 1;      // alvo está à esquerda
        else
            begin = idx + 1;    // alvo está à direita
    }
    return false;
}

//6. Busca Binária Recursiva.
bool busca_binaria_recursiva(const std::vector<int>& vec, int begin, int end, int target) {
	// não encontrou!
	if (begin > end) return false;
	int idx = begin + (end - begin) / 2;
	if (vec[idx] == target)
		return true;
	else if (vec[idx] > target)
		return busca_binaria_recursiva(vec, begin, idx - 1, target);
	else
		return busca_binaria_recursiva(vec, idx + 1, end, target);
}

//7. Busca Ternária Iterativa.
bool busca_ternaria_iterativa(const std::vector<int>& vec, int begin, int end, int target) {
    while (begin <= end) {
        int terco_1 = begin + (end - begin) / 3;
        int terco_2 = begin + 2 * (end - begin) / 3;

        if (vec[terco_1] == target || vec[terco_2] == target)
            return true;
        else if (target < vec[terco_1]) // primeiro terço
            end = terco_1 - 1;
        else if (target > vec[terco_2]) // último terço
            begin = terco_2 + 1;
        else {                          // terço do meio
            begin = terco_1 + 1;
            end   = terco_2 - 1;
        }
    }
    return false;
}

//8. Busca Ternária Recursiva.
bool busca_ternaria_recursiva(const std::vector<int>& vec, int begin, int end, int target) {
	// condição de parada (não encontrou)!
	if (begin > end) return false;
	int terco_1 = begin + (end - begin) / 3;
	int terco_2 = begin + 2 * (end - begin) / 3;

	if (vec[terco_1] == target || vec[terco_2] == target)
		return true;
	else if (target < vec[terco_1]) // primeiro terço
		return busca_ternaria_recursiva(vec, begin, terco_1 - 1, target);
	else if (target > vec[terco_2]) // último terço
		return busca_ternaria_recursiva(vec, terco_2 + 1, end, target);
	else                            // terço do meio
		return busca_ternaria_recursiva(vec, terco_1 + 1, terco_2 - 1, target);
}

// executa o algoritmo de busca, retornando o tempo de execução em nanosegundos.
inline long benchmark(algoritmo_busca algoritmo, const vector<int> &vec, int begin, int end, int target) {
	auto start = chrono::high_resolution_clock::now();
	bool pos = algoritmo(vec, begin, end, target);   // <-- bool, não int
	auto stop = chrono::high_resolution_clock::now();

	// impede o compilador de eliminar a chamada por ser "não usada"
	volatile bool dummy = pos;
	(void) dummy;

	return chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
}

// main :)
int main(int argc, char **argv) {
	// vetor utilizando nos testes.
	vector<int> vec(END_SIZE, 1); // Cria o vetor preenchendo com 1

	// Valor a ser buscado (força o pior caso, já que o vetor contem somente 1)
	int target = 0;

	// cabeçalho do CSV gerado como saída:
	cout << "Algoritmo";
	for (int i = START_SIZE; i <= END_SIZE; i += STEP) {
		cout << ",Tamanho_" << i; //
	}
	cout << endl;

	// para cada função
    for (const auto& [nome, funcao] : funcoes) {
    	cout << nome;
    	int start = 0; // sempre busca a partir do início do vetor
    	for (int end = START_SIZE; end <= END_SIZE; end += STEP) {
			long long soma_tempos = 0;
			// executa um núm de vezes
			for (int t = 0; t < TRIALS; ++t) {
				soma_tempos += benchmark(funcao, vec, start, end, target);
			}
			// calcula a média
			long media = soma_tempos / TRIALS;
			// imprime o tempo médio.
			// o flush é necessário para forçar imprimir na tela.
			cout << "," << media << flush;
    	}
    	cout << endl << flush;
	}
	return 0;
}
