/* WSCAD - 9th Marathon of Parallel Programming
 * Simple Brute Force Algorithm for the
 * Traveling-Salesman Problem
 * Author: Emilio Francesquini - francesquini@ic.unicamp.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>

// Variáveis globais
int min_distance;  // Armazena a menor distância encontrada até o momento
int nb_towns;      // Número de cidades (número de vértices no problema do Caixeiro Viajante)

// Estrutura de dados que representa a matriz de distâncias
typedef struct
{
    int to_town;  // Cidade de destino
    int dist;     // Distância até a cidade de destino
} d_info;

// Matriz de distâncias entre as cidades (adjacência)
d_info **d_matrix;
// Vetor que armazena a distância de cada cidade até a origem
int *dist_to_origin;

/**
 * Função que verifica se uma cidade já foi visitada no caminho atual até um determinado nível.
 *
 * @param town A cidade a ser verificada.
 * @param depth A profundidade do caminho que está sendo explorado (quantas cidades foram visitadas).
 * @param path O vetor que armazena a sequência de cidades visitadas.
 * @return 1 se a cidade foi visitada, 0 caso contrário.
 */
int present(int town, int depth, int *path)
{
    int i;
    for (i = 0; i < depth; i++)  // Verifica se a cidade já está no caminho
        if (path[i] == town)
            return 1;  // A cidade já foi visitada
    return 0;  // A cidade não foi visitada
}

/**
 * Função recursiva que explora todos os caminhos possíveis para o problema do Caixeiro Viajante.
 *
 * @param depth A profundidade do caminho atual (quantas cidades foram visitadas).
 * @param current_length O comprimento atual do caminho (distância percorrida até o momento).
 * @param path O vetor de cidades visitadas.
 */
void tsp(int depth, int current_length, int *path)
{
    int i;
    if (current_length >= min_distance)  // Se o comprimento atual for maior ou igual à distância mínima, aborta a busca
        return;
    if (depth == nb_towns)  // Se todas as cidades foram visitadas
    {
        current_length += dist_to_origin[path[nb_towns - 1]];  // Adiciona a distância até a cidade de origem
        if (current_length < min_distance)  // Se o comprimento do caminho é menor que o mínimo encontrado até agora
            min_distance = current_length;  // Atualiza a distância mínima
    }
    else
    {
        int town, me, dist;
        me = path[depth - 1];  // Última cidade visitada no caminho
        for (i = 0; i < nb_towns; i++)  // Percorre todas as cidades para encontrar a próxima cidade a ser visitada
        {
            town = d_matrix[me][i].to_town;  // Obtém a cidade de destino
            if (!present(town, depth, path))  // Se a cidade ainda não foi visitada
            {
                path[depth] = town;  // Marca a cidade como visitada
                dist = d_matrix[me][i].dist;  // Distância até a cidade de destino
                tsp(depth + 1, current_length + dist, path);  // Chama recursivamente para o próximo nível
            }
        }
    }
}

/**
 * Função heurística que inicializa a matriz de distâncias com o vizinho mais próximo para cada cidade.
 *
 * @param x Vetor com as coordenadas x das cidades.
 * @param y Vetor com as coordenadas y das cidades.
 */
void greedy_shortest_first_heuristic(int *x, int *y)
{
    int i, j, k, dist;
    int *tempdist;  // Vetor temporário para armazenar as distâncias

    tempdist = (int *)malloc(sizeof(int) * nb_towns);

    // Para cada cidade, encontra o vizinho mais próximo
    for (i = 0; i < nb_towns; i++)
    {
        for (j = 0; j < nb_towns; j++)
        {
            int dx = x[i] - x[j];  // Diferença nas coordenadas x
            int dy = y[i] - y[j];  // Diferença nas coordenadas y
            tempdist[j] = dx * dx + dy * dy;  // Distância quadrada (evita o cálculo da raiz quadrada no início)
        }
        for (j = 0; j < nb_towns; j++)  // Encontra o vizinho mais próximo
        {
            int tmp = INT_MAX;
            int town = 0;
            for (k = 0; k < nb_towns; k++)
            {
                if (tempdist[k] < tmp)  // Se a distância for menor, atualiza
                {
                    tmp = tempdist[k];
                    town = k;
                }
            }
            tempdist[town] = INT_MAX;  // Marca a cidade como visitada
            d_matrix[i][j].to_town = town;  // Define a cidade de destino
            dist = (int)sqrt(tmp);  // Calcula a distância real
            d_matrix[i][j].dist = dist;  // Armazena a distância
            if (i == 0)
                dist_to_origin[town] = dist;  // Define a distância até a origem para o vizinho mais próximo
        }
    }

    free(tempdist);  // Libera a memória alocada
}

/**
 * Função de inicialização do problema do Caixeiro Viajante.
 *
 * Lê o número de cidades e as coordenadas de cada cidade a partir da entrada padrão,
 * e inicializa a matriz de distâncias e o vetor de distâncias até a origem usando a heurística.
 */
void init_tsp()
{
    int i, st;
    int *x, *y;

    min_distance = INT_MAX;  // Inicializa a distância mínima com o valor máximo possível
    printf("Numero de Cidades: ");
    st = scanf("%u", &nb_towns);  // Lê o número de cidades
    if (st != 1)
        exit(1);  // Se falhar na leitura, termina a execução

    // Aloca a memória para a matriz de distâncias e o vetor de distâncias até a origem
    d_matrix = (d_info **)malloc(sizeof(d_info *) * nb_towns);
    for (i = 0; i < nb_towns; i++)
        d_matrix[i] = (d_info *)malloc(sizeof(d_info) * nb_towns);
    dist_to_origin = (int *)malloc(sizeof(int) * nb_towns);

    x = (int *)malloc(sizeof(int) * nb_towns);  // Vetor de coordenadas x
    y = (int *)malloc(sizeof(int) * nb_towns);  // Vetor de coordenadas y

    printf("Coordenadas das cidades:\n");
    for (i = 0; i < nb_towns; i++)
    {
        printf("Cidade %d: ", i);
        st = scanf("%u %u", x + i, y + i);  // Lê as coordenadas das cidades
        if (st != 2)
            exit(1);  // Se falhar na leitura, termina a execução
    }

    greedy_shortest_first_heuristic(x, y);  // Aplica a heurística de vizinho mais próximo

    free(x);  // Libera a memória dos vetores de coordenadas
    free(y);
}

/**
 * Função principal que resolve o problema do Caixeiro Viajante.
 *
 * Inicializa o problema e usa a função recursiva tsp para explorar todos os caminhos possíveis.
 * Retorna a distância mínima encontrada.
 */
int run_tsp()
{
    int i, *path;
    struct timeval start, stop;
    init_tsp();  // Inicializa o problema do TSP
    gettimeofday(&start, NULL);  // Registra o tempo de início
    path = (int *)malloc(sizeof(int) * nb_towns);  // Aloca o vetor de caminhos
    path[0] = 0;  // Começa o caminho pela cidade 0

    tsp(1, 0, path);  // Chama a função recursiva para explorar todos os caminhos

    free(path);  // Libera a memória do vetor de caminhos
    for (i = 0; i < nb_towns; i++)
        free(d_matrix[i]);  // Libera a memória da matriz de distâncias
    free(d_matrix);
    gettimeofday(&stop, NULL);  // Registra o tempo de término
    double t = (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
                ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));  // Calcula o tempo decorrido

    fprintf(stdout, "Tempo decorrido = %g ms\n", t);  // Exibe o tempo decorrido
    return min_distance;  // Retorna a menor distância encontrada
}

/**
 * Função principal do programa.
 *
 * Chama a função run_tsp para resolver o problema e imprime a solução.
 */
int main()
{
    printf("Resultado: %d\n", run_tsp());  // Exibe a solução do problema
    return 0;
}
