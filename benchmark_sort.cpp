#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>

using namespace std;

// Tamanho máximo do vetor
const int VECTOR_SIZE = 15000;
// Intervalo entre cada teste
const int STEP = 1000;
// Quantidade de testes por função
const int TRIALS = 3;

// define o algoritmo ordenaçãp (vetor, início, fim)
typedef void (*algoritmo_ordenacao)(vector<int>& vetor, int end);

// protótipo das funções de buscas que serão executadas:
void insertion_sort(vector<int>& vetor, int end);
void selection_sort(vector<int>& vetor, int end);
void bubble_sort(vector<int>& vetor, int end);
void quick_sort(vector<int>& vetor, int end);
void merge_sort(vector<int>& vetor, int end);

// Define um vetor com os algoritmos a serem executados no loop:
vector<pair<string, algoritmo_ordenacao>> funcoes = {
	{ "Insertion sort", insertion_sort  },
	{ "Selection sort", selection_sort  },
	{ "Bubble sort",    bubble_sort     },
	{ "Quick sort",     quick_sort      },
	{ "Merge sort",     merge_sort      },
};

// ========== IMPLEMENTAÇÃO DOS ALGORITMOS DE ORDENAÇÃO ==========

// 1. Insertion sort.
void insertion_sort(vector<int>& vetor, int end){
    int key, holePos;
    // percorrer parte não-ordenada (começa em 1)
    for (int i = 1; i < end; i++) {
        key = vetor[i];
        holePos = i - 1;
        // percorrer (reverso) a parte ordenada até o início ou achar a posição
        while (holePos >= 0 && vetor[holePos] > key) {
            vetor[holePos + 1] = vetor[holePos];
            holePos = holePos - 1;
        }
        // inserir item na posição criada
        vetor[holePos + 1] = key;
    }
}

// 2. Selection sort.
void selection_sort(vector<int>& vetor, int end){
	for (int i = 0; i < end - 1; i++)
		for (int j = i + 1; j < end; j++)
			if (vetor[i] > vetor[j])
				swap(vetor[i], vetor[j]);
}

// 3. Bubble sort.
void bubble_sort(vector<int>& vetor, int end){
    int j = end - 1;
    bool houveTroca = false;
    do {
        houveTroca = false;
        for (int i = 0; i <= j; i++) {
            if (vetor[i + 1] < vetor[i]) {
                swap(vetor[i], vetor[i + 1]);
                houveTroca = true;
            }
        }
        j = j - 1;
    } while (houveTroca);
}

// 4. Quick sort.
int particao(vector<int>& vetor, int start, int end){
	//	seleção pivô de forma aleatória.
    int pivo = start + rand() % (end - start + 1); // aleatório entre 0 e end
	int x = vetor[pivo];
	// não estava no algoritmo do slide.
	// sem isso, não estava ordenando corretamente
    swap(vetor[pivo], vetor[end]);
	int i = start - 1;
	for (int j = start; j < end; j++) {
		if (vetor[j] <= x) {
			i++;
			swap(vetor[i], vetor[j]);
		}
	}
	swap(vetor[i + 1], vetor[end]);
	return i + 1;
}

void quick_sort(vector<int>& vetor, int start, int end){
	if (start < end) {
		int idx = particao(vetor, start, end);
		quick_sort(vetor, start, idx - 1);
		quick_sort(vetor, idx + 1, end);
	}
}

void quick_sort(vector<int>& vetor, int end){
	quick_sort(vetor, 0, end);
}

// 5. Merge sort.
// método para fazer o join dos vetores ordenados
void intercala(vector<int>& left, vector<int>& right, vector<int>& vec) {
    int i = 0; // índice para left
    int j = 0; // índice para right
    int k = 0; // índice para o vetor resultado

    // Compara elementos e insere o menor em vec
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            vec[k++] = left[i++];
        } else {
            vec[k++] = right[j++];
        }
    }
    // Copia os elementos restantes
    while (i < left.size()) {
        vec[k++] = left[i++];
    }
    while (j < right.size()) {
        vec[k++] = right[j++];
    }
}

// rotina recursiva do merge
void merge_sort(vector<int>& vetor, int end) {
	if (end < 2) return;
    int meio = end / 2;
    // vetores esquerda (left) e direita (right)
    vector<int> left(meio);
    vector<int> right(end - meio);

    // Copia os elementos para left e right
    for (int i = 0; i < meio; i++) {
        left[i] = vetor[i];
    }
    for (int i = meio; i < end; i++) {
        right[i - meio] = vetor[i];
    }

    // Chamadas recursivas
    merge_sort(left, meio);
    merge_sort(right, end - meio);

    // Intercala de volta ao vetor
    intercala(left, right, vetor);
}

// ========== FUNÇÕES PARA EXECUTAR OS TESTES ==========

// verifica se o vetor foi ordenado
int verifica_ordenacao(vector<int>& vetor, int end) {
	for (int i = 0; i < end - 1; i++)
		if (vetor[i] > vetor[i + 1])
			return i;
	return -1;
}

// executa o algoritmo de ordenação, retornando o tempo de execução em nanosegundos.
inline long benchmark(algoritmo_ordenacao algoritmo, vector<int> &vetor, int end) {
	// cria o vetor para cada teste (após um teste estará ordenado e no teste seguinte cairia no melhor caso)
	vector<int> vetor_teste(end);
	for (int i = 0; i < end; i++)
		vetor_teste[i] = vetor[i];
	auto start = chrono::high_resolution_clock::now();
	algoritmo(vetor_teste, end);
	auto stop = chrono::high_resolution_clock::now();
	// verifica ordenação (durante debug do código, descobri que nem sempre finalizava ordenado)
	int pos = 0;
	if ((pos = verifica_ordenacao(vetor_teste, end)) != -1) {
		cout << endl << "ERRO! "
				<< ", POS: " << pos
				<< ", Valores: v[" << pos << "] = " << vetor_teste[pos]
				<< ", v["<< pos + 1 << "] = "<< vetor_teste[pos + 1]
				<< endl;
		exit(1);
	}
	return chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
}

// imprime um cabeçalho CSV dos resultados
void imprime_cabecalho() {
	// cabeçalho do CSV gerado como saída:
	cout << "Caso_de_teste,Algoritmo";
	for (int i = STEP; i <= VECTOR_SIZE; i += STEP) {
		cout << ",Tamanho_" << i; //
	}
	cout << endl;
}

// testa o algoritmo de ordenação
void testa_caso(string tipo_teste, vector<int> &vetor, vector<pair<string, algoritmo_ordenacao>> funcoes) {
	for (const auto& [nome, funcao] : funcoes) {
		cout << tipo_teste << "," << nome;
		// usa o mesmo vetor, porém ordena de 0 a n, onde n varia de STEP em STEP
		for (int end = STEP; end <= VECTOR_SIZE; end += STEP) {
			long long soma_tempos = 0;
			// executa um número de vezes
			for (int t = 0; t < TRIALS; ++t) {
				// @suppress("Invalid arguments")
				soma_tempos += benchmark(funcao, vetor, end);
			}
			// calcula a média das execuções
			long media = soma_tempos / TRIALS;
			// imprime o tempo médio. O flush é necessário para forçar imprimir na tela.
			cout << "," << media << flush;
		}
		cout << endl;
	}
}

// main :)
int main(int argc, char **argv) {
	// cria três vetores para testar os casos: melhor, pior e aleatório
	vector<int> melhor_caso(VECTOR_SIZE), pior_caso(VECTOR_SIZE), caso_aleatorio(VECTOR_SIZE);
	// usa o relógio como semente do aleatório
	srand(time(0));
	for (int i = 0; i < VECTOR_SIZE; i++) {
		melhor_caso[i] = i;
		pior_caso[VECTOR_SIZE - i - 1] = i;
		caso_aleatorio[i] = rand() % VECTOR_SIZE; // aleatório de 0 a VECTOR_SIZE
	}
	cout << endl;
	// testa os casos
	imprime_cabecalho();
	testa_caso("Melhor caso (vetor já ordenado)",    melhor_caso,    funcoes);
	testa_caso("Pior caso (vetor em ordem inversa)", pior_caso,      funcoes);
	testa_caso("Caso médio (vetor aleatório)",       caso_aleatorio, funcoes);
	return 0;
}
