# Projeto final

Introdução à Programação Paralela e Distribuída

## Autores

- Daniel de Almeida
- Elisa Yamashita Rodrigues
- Gabriel dos Santos Pereira
- Leandro Apolinário Bento

# Atividades Realizadas

## 1. Estudo dos algorítmos

Numa primeira etapa, foi realizada uma coleta de algoritmos com paralelização viável para testes de viabilidade para este trabalho, pré selecionados de uma lista prévia disponibilizada pelo professor. Os seguintes algoritmos foram avaliados:
- Sudoku;
- Jogo da Vida;
- Algoritmo de Dijkstra;
- Problema do Caixeiro Viajante;
- Algoritmo de Karatsuba.
  
Uma filtragem foi redigida nesta etapa para entendimento dos métodos aos quais os algoritmos dependiam, aplicabilidade no mundo real e interesse do grupo no problema descrito, além da capacidade e desafio de paralelização. O algoritmo de resolução do Sudoku, mesmo sequencialmente, pareceu muito complexo, assim como o Jogo da Vida na implementação encontrada, isso fez com que fossem descartados ainda nesta primeira etapa. O problema do Método de Multiplicação de Karatsuba é porque o método em si já se pareceu muito complexo, antes mesmo da implementação do algoritmo sequencial. Restando duas opções, foi averiguada a viabilidade de implementação pelo docente da disciplina, um detalhamento de ambos o problema é mostrado a seguir:
- **Algoritmo de Dijkstra:** O algoritmo de Dijkstra é um algoritmo que propõe encontrar a árvore mínima dada um grafo ponderado. O problema pode ser útil em aplicações de cálculo de rotas, por exemplo. A computação desse problema é paralelizável tanto na geração do grafo quanto na computação de distâncias.
- **Problema do Caixeiro Viajante:** O Problema do Caixeiro Viajante é um problema que se propõe a encontrar o caminho menos custoso para visitar todos os vértices de um grafo sem repetir nenhuma aresta, voltando ao ponto inicial. A computação desse problema é paralelizável a partir das chamadas recursivas do algoritmo.

A partir dos dois problemas selecionados, os trabalhos de paralelização para ambos foram redigidos. É digno de nota que ambos foram paralelizados a partir da biblioteca OpenMP para a linguagem C.

## 2. Problema do Caixeiro Viajante

Para o Problema do Caixeiro Viajante existem possíveis abordagens que foram avaliadas para a paralelização do algoritmo pelo grupo, a partir da implementação sequencial escolhida, utilizando:
- Tasks, na chamada recursiva do algoritmo;
- Iterações que calculam os subcaminhos a partir de um ponto do grafo;
- Dividir grafo original em subgrafos para a paralelização.

Para a abordagem onde se envolviam as tasks da biblioteca OpenMP, foram encontrados problemas principalmente pelas políticas de acesso das variáveis. Além de diversas regiões críticas dentro da função recursiva, existe uma variável que é passada como ponteiro para a recursão, ao mesmo tempo que é compartilhada globalmente no programa. A variável path na implementação escolhida representa o caminho total do algoritmo, e além de ser compartilhada globalmente e ser acessada diversas vezes na função da recursão, o que a torna uma memória compartilhada, a mesma é passada como ponteiro para ser modificada pela chamada filha da chamada atual da recursão. Esta abordagem proporcionou muitos erros de estouro de memória, falhas de segmentação e resultados incorretos na versão paralela.

A abordagem que calcula subcaminhos a partir de um nó já calculado do grafo não ocasionou erros de execução, as variáveis são mais facilmente mapeadas e protegidas, o problema foi o desempenho do algoritmo. A chamada das iterações de visitação às outras cidades ocorre dentro da função recursiva, isto proporciona um efeito cascata onde o número de threads necessárias aumentam exponencialmente, derrubando o desempenho do algoritmo em sua versão paralela, sendo inclusive pior do que a versão sequencial.

A ideia de dividir o grafo em subgrafos antes da inicialização do algoritmo para a paralelização num primeiro ponto de vista parece boa. As variáveis de cada subgrafo se tornam totalmente privadas e usadas apenas no fim do algoritmo, fora o fato do número de subproblemas não aumentar de maneira exponencial, mas a está errada no próprio conceito do algoritmo. Ao dividir um problema em subgrafos, não existe garantia de que os subproblemas escolhidas contém a combinação necessária para encontrar os caminhos menores de cada sub-região, sem contar que não leva em conta o ponto de início original do algoritmo, quebrando o próprio funcionamento taxonômico do algoritmo.

## 3. Algoritmo de Dijkstra

O algoritmo de Dijkstra é uma opção mais fácil de ser paralelizada a partir da implementação original encontrada e dos estudos realizados a partir da mesma. O algoritmo de Dijkstra possui uma região paralelizável muito clara para uma tentativa de paralelismo: As iterações da tabela de distâncias para encontrar o nó mínimo, o laço principal do algoritmo.

A partir dessa região paralelizável, foi realizada uma implementação utilizando a diretiva omp for. Os resultados a partir dessa execução já foram inicialmente satisfatórios para um grafo de tamanho considerável, mas existe um outro problema: o cálculo de sub caminhos é dependente da iteração atual, ocasionando desbalanceamento de carga. Então foram testadas as diferentes estratégias de balanceamento de carga já pré-definidas pela biblioteca OpenMP e suas respectivas médias de tempo de execução para um grafo de 150000 nós e 80000 arestas:
- Sequencial: 12847,06 ms;
- Static com 4 threads: 4523,482 ms;
- Dynamic (blocos de 20) com 4 threads: 4013,368 ms;
- Auto com 4 threads: 4127,996 ms.

A partir desses dados foi possível calcular o speedup de cada um:
- Static com 4 threads ≈ 2,84;
- Dynamic (blocos de 20) com 4 threads ≈ 3,2;
- Auto com 4 threads ≈ 3,11.

Concluindo, temos o melhor desempenho mostrado pelo balanceamento Dinâmico, com blocos de tamanho 20, apresentando um speedup de 3,2.

## 4. Conclusão

Os resultados obtidos obtidos neste trabalho foram satisfatórios a partir das abordagens para o algoritmo de Dijkstra, conseguindo um bom speedup, ainda que o mesmo não tenha alcançado o valor ideal, devido ao limite da parte sequencial da implementação. Por sua vez, o Problema do Caixeiro Viajante se tornou relativamente mais difícil de paralelizar e o no escopo deste trabalho não pode ser feito com êxito.

