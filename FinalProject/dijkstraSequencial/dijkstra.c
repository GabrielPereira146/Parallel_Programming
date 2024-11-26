#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>
// Função de gerador de números aleatórios customizada (simula a função rand)
static unsigned long int next = 1;
int my_rand(void)
{
	// Gera um número aleatório usando a fórmula Linear Congruential Generator
	return ((next = next * 1103515245 + 12345) % ((u_long)RAND_MAX + 1));
}

// Função para definir a semente do gerador de números aleatórios
void my_srand(unsigned int seed)
{
	next = seed;
}

// Definição de uma estrutura para representar o grafo
struct Graph
{
	int nNodes;	 // Número de nós no grafo
	int *nEdges; // Array que mantém o número de arestas de cada nó
	int **edges; // Array de arrays que mantém as arestas (vértices vizinhos)
	int **w;	 // Array de arrays que mantém os pesos das arestas
};

// Função para criar um grafo aleatório
struct Graph *createRandomGraph(int nNodes, int nEdges, int seed)
{
	my_srand(seed); // Define a semente para o gerador de números aleatórios

	// Aloca memória para o grafo
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->nNodes = nNodes;
	graph->nEdges = (int *)malloc(sizeof(int) * nNodes);
	graph->edges = (int **)malloc(sizeof(int *) * nNodes);
	graph->w = (int **)malloc(sizeof(int *) * nNodes);

	int k, v;
	// Inicializa a estrutura do grafo (arestas e pesos)
	for (v = 0; v < nNodes; v++)
	{
		graph->edges[v] = (int *)malloc(sizeof(int) * nNodes); // Arestas para o nó
		graph->w[v] = (int *)malloc(sizeof(int) * nNodes);	   // Pesos das arestas
		graph->nEdges[v] = 0;								   // Inicializa o número de arestas como 0
	}

	// Geração aleatória das arestas do grafo
	for (int source = 0; source < nNodes; source++)
	{
		// Determina o número de arestas para cada nó (com variação aleatória)
		int nArestasVertice = (double)nEdges / nNodes * (0.5 + my_rand() / (double)RAND_MAX);
		// Cria as arestas aleatórias para cada nó
		for (k = nArestasVertice; k >= 0; k--)
		{
			int dest = my_rand() % nNodes;						// Escolhe aleatoriamente o destino da aresta
			int w = 1 + (my_rand() % 10);						// Gera um peso aleatório para a aresta
			graph->edges[source][graph->nEdges[source]] = dest; // Armazena o destino
			graph->w[source][graph->nEdges[source]++] = w;		// Armazena o peso da aresta
		}
	}

	return graph; // Retorna o grafo gerado
}

// Função que implementa o algoritmo de Dijkstra para encontrar o menor caminho
int *dijkstra(struct Graph *graph, int source)
{
	int nNodes = graph->nNodes;
	int *visited = (int *)malloc(sizeof(int) * nNodes);	  // Marca os nós visitados
	int *distances = (int *)malloc(sizeof(int) * nNodes); // Armazena a distância mínima para cada nó
	int k, v;

	// Inicializa os vetores de visitados e distâncias
	for (v = 0; v < nNodes; v++)
	{
		distances[v] = INT_MAX; // Inicializa a distância como infinita
		visited[v] = 0;			// Inicializa todos os nós como não visitados
	}
	distances[source] = 0; // A distância para o nó de origem é 0
	visited[source] = 1;   // Marca o nó de origem como visitado

	// Inicializa as distâncias dos vizinhos do nó de origem
	for (k = 0; k < graph->nEdges[source]; k++)
	{
		distances[graph->edges[source][k]] = graph->w[source][k];
	}

	// Algoritmo de Dijkstra
	for (v = 1; v < nNodes; v++)
	{
		int min = 0;
		int minValue = INT_MAX;
		// Encontra o nó não visitado com a menor distância
		for (k = 0; k < nNodes; k++)
		{
			if (visited[k] == 0 && distances[k] < minValue)
			{
				minValue = distances[k];
				min = k;
			}
		}

		visited[min] = 1; // Marca o nó com a menor distância como visitado

		// Atualiza as distâncias dos vizinhos do nó mínimo
		for (k = 0; k < graph->nEdges[min]; k++)
		{
			int dest = graph->edges[min][k];
			// Se o novo caminho for mais curto, atualiza a distância
			if (distances[dest] > distances[min] + graph->w[min][k])
			{
				distances[dest] = distances[min] + graph->w[min][k];
			}
		}
	}

	free(visited); // Libera a memória usada pelo vetor de visitados

	return distances; // Retorna o vetor de distâncias mínimas
}

int main(int argc, char **argv)
{
	struct timeval start, stop;
	int nNodes, nEdges, seed;

	// Recebe os parâmetros de entrada ou os lê do stdin
	if (argc == 4)
	{
		nNodes = atoi(argv[1]); // Número de nós
		nEdges = atoi(argv[2]); // Número de arestas
		seed = atoi(argv[3]);	// Semente para o gerador de números aleatórios
	}
	else
	{
		// Caso não sejam passados parâmetros, lê do stdin
		fscanf(stdin, "%d %d %d", &nNodes, &nEdges, &seed);
	}
	gettimeofday(&start, NULL);
	// Ajusta o número total de arestas com base no número de nós
	nEdges = nNodes * nEdges;

	// Cria o grafo aleatório com os parâmetros fornecidos
	struct Graph *graph = createRandomGraph(nNodes, nEdges, seed);

	// Executa o algoritmo de Dijkstra a partir do nó 0
	int *dist = dijkstra(graph, 0);

	// Calcula a média das distâncias
	double mean = 0;
	for (int v = 0; v < graph->nNodes; v++)
	{
		mean += dist[v];
	}

	// Imprime a média das distâncias
	fprintf(stdout, "%.2f\n", mean / nNodes);
	gettimeofday(&stop, NULL);
	double t = (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
				((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

	fprintf(stdout, "Tempo decorrido = %g ms\n", t);
	// Libera a memória alocada
	free(dist);
	for (int v = 0; v < graph->nNodes; v++)
	{
		free(graph->edges[v]);
		free(graph->w[v]);
	}
	free(graph->edges);
	free(graph->w);
	free(graph->nEdges);
	free(graph);

	return 0; // Finaliza o programa
}
