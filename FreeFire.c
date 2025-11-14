#include <stdio.h>     // Funções de entrada/saída (printf, scanf)
#include <stdlib.h>    // Funções utilitárias (exit, malloc)
#include <string.h>    // Manipulação de strings (strcmp, strcpy)
#include <time.h>      // Opcional: Para medir o tempo, mas focado no contador de operações

// ============================================================================
//         PROJETO FREEFIRE - INVENTARIO 
// ============================================================================
// Matrícula: 20250212488
// Faculdade: Estácio
// Aluno: Ângelo Oliveira dos Santos
// Cuso: Análise e desenvolvimento de sistemas
// Desafio: nível aventureiro
// Sistema de inventário que simula a mochila de loot inicial do jogador
// ============================================================================

// --- CONSTANTES ---
#define CAPACIDADE_MAXIMA 10 
#define MAX_NOME 30
#define MAX_TIPO 20

// -------------------------------------------------------------------
// 1. CRIAÇÃO DE STRUCTS
// -------------------------------------------------------------------

// Item: Estrutura base para o dado
struct Item {
    char nome[MAX_NOME];      // Nome do item (chave de busca)
    char tipo[MAX_TIPO];      // Tipo do item (ex: arma, cura)
    int quantidade;          // Quantidade do item
};
typedef struct Item Item;

// No: Estrutura para a Lista Encadeada
struct No {
    Item dados;              // Os dados do Item contidos neste nó
    struct No *proximo;      // Ponteiro para o próximo nó da lista
};
typedef struct No No;

// -------------------------------------------------------------------
// VARIÁVEIS GLOBAIS (CONTADORES DE COMPARAÇÕES)
// -------------------------------------------------------------------
// Usadas para contar a eficiência das buscas em tempo de execução
long long int contadorComparacoes = 0;

// -------------------------------------------------------------------
// VETOR (LISTA SEQUENCIAL) - ESTRUTURA E PROTÓTIPOS
// -------------------------------------------------------------------
Item mochilaVetor[CAPACIDADE_MAXIMA]; 
int contadorItensVetor = 0; 

void menuVetor(int *opcao);
void inserirItemVetor(Item mochila[], int *contador);
void removerItemVetor(Item mochila[], int *contador, const char nomeBusca[]);
void listarItensVetor(const Item mochila[], int contador);
void ordenarVetor(Item mochila[], int contador);
int buscarSequencialVetor(const Item mochila[], int contador, const char nomeBusca[]);
int buscarBinariaVetor(const Item mochila[], int contador, const char nomeBusca[]);

// -------------------------------------------------------------------
// LISTA ENCADEADA - ESTRUTURA E PROTÓTIPOS
// -------------------------------------------------------------------
No *mochilaLista = NULL; // Ponteiro inicial (Cabeça da lista)
int contadorItensLista = 0;

void menuLista(int *opcao);
void inserirItemLista(No **cabeca, int *contador);
void removerItemLista(No **cabeca, int *contador, const char nomeBusca[]);
void listarItensLista(const No *cabeca, int contador);
int buscarSequencialLista(const No *cabeca, const char nomeBusca[]);

// -------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MAIN)
// -------------------------------------------------------------------
int main() {
    int opcaoPrincipal;
    int opcaoSubMenu;

    printf("=== SISTEMA DE MOCHILA: VETOR vs. LISTA ENCADEADA ===\n");
    
    do {
        printf("\n--- SELEÇÃO DA ESTRUTURA ---\n");
        printf("1. Trabalhar com VETOR (Lista Sequencial)\n");
        printf("2. Trabalhar com LISTA ENCADEADA\n");
        printf("0. Sair do Programa\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcaoPrincipal) != 1) {
            while (getchar() != '\n'); 
            opcaoPrincipal = -1;
        }

        switch (opcaoPrincipal) {
            case 1:
                // Lógica para o VETOR
                do {
                    listarItensVetor(mochilaVetor, contadorItensVetor);
                    menuVetor(&opcaoSubMenu);
                    
                    char nomeBusca[MAX_NOME];
                    int posicao;

                    switch (opcaoSubMenu) {
                        case 1: inserirItemVetor(mochilaVetor, &contadorItensVetor); break;
                        case 2: 
                            printf("Digite o NOME do item para remover: ");
                            scanf("%29s", nomeBusca);
                            removerItemVetor(mochilaVetor, &contadorItensVetor, nomeBusca);
                            break;
                        case 3: 
                            printf("Digite o NOME do item para busca SEQUENCIAL: ");
                            scanf("%29s", nomeBusca);
                            contadorComparacoes = 0; // Zera o contador
                            posicao = buscarSequencialVetor(mochilaVetor, contadorItensVetor, nomeBusca);
                            if (posicao != -1) printf("[SUCESSO] Item encontrado na posicao %d.\n", posicao + 1);
                            else printf("[ERRO] Item '%s' nao encontrado.\n", nomeBusca);
                            printf("[PERFORMANCE] Comparações realizadas: %lld\n", contadorComparacoes);
                            break;
                        case 4:
                            ordenarVetor(mochilaVetor, contadorItensVetor);
                            break;
                        case 5: 
                            printf("Digite o NOME do item para busca BINÁRIA: ");
                            scanf("%29s", nomeBusca);
                            contadorComparacoes = 0; // Zera o contador
                            posicao = buscarBinariaVetor(mochilaVetor, contadorItensVetor, nomeBusca);
                            if (posicao != -1) printf("[SUCESSO] Item encontrado na posicao %d.\n", posicao + 1);
                            else printf("[ERRO] Item '%s' nao encontrado.\n", nomeBusca);
                            printf("[PERFORMANCE] Comparações realizadas: %lld\n", contadorComparacoes);
                            break;
                        case 0: printf("\nVoltando ao menu principal...\n"); break;
                        default: printf("\nOpcao invalida. Tente novamente.\n"); break;
                    }
                } while (opcaoSubMenu != 0);
                break;
            case 2:
                // Lógica para a LISTA ENCADEADA
                do {
                    listarItensLista(mochilaLista, contadorItensLista);
                    menuLista(&opcaoSubMenu);

                    char nomeBusca[MAX_NOME];
                    int posicao;

                    switch (opcaoSubMenu) {
                        case 1: inserirItemLista(&mochilaLista, &contadorItensLista); break;
                        case 2:
                            printf("Digite o NOME do item para remover: ");
                            scanf("%29s", nomeBusca);
                            removerItemLista(&mochilaLista, &contadorItensLista, nomeBusca);
                            break;
                        case 3: 
                            printf("Digite o NOME do item para busca SEQUENCIAL: ");
                            scanf("%29s", nomeBusca);
                            contadorComparacoes = 0; // Zera o contador
                            posicao = buscarSequencialLista(mochilaLista, nomeBusca);
                            if (posicao != -1) printf("[SUCESSO] Item encontrado na posicao %d.\n", posicao);
                            else printf("[ERRO] Item '%s' nao encontrado.\n", nomeBusca);
                            printf("[PERFORMANCE] Comparações realizadas: %lld\n", contadorComparacoes);
                            break;
                        case 0: printf("\nVoltando ao menu principal...\n"); break;
                        default: printf("\nOpcao invalida. Tente novamente.\n"); break;
                    }
                } while (opcaoSubMenu != 0);
                break;
            case 0:
                printf("\nFechando o programa. Comparação encerrada.\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcaoPrincipal != 0);

    return 0;
}

// -------------------------------------------------------------------
// IMPLEMENTAÇÕES: FUNÇÕES DO VETOR (LISTA SEQUENCIAL)
// -------------------------------------------------------------------

/**
 * Exibe o menu de opções para a estrutura de VETOR.
 */
void menuVetor(int *opcao) {
    printf("\n--- MOCHILA VETOR (Sequencial: %d/%d) ---\n", contadorItensVetor, CAPACIDADE_MAXIMA);
    printf("1. Inserir Item\n");
    printf("2. Remover Item (Custo O(n))\n");
    printf("3. Buscar Item (Sequencial - Custo O(n))\n");
    printf("4. Ordenar Itens por Nome (Selection Sort - Custo O(n²)\n");
    printf("5. Buscar Item (Binaria - Custo O(log n) - Requer ordenacao!)\n");
    printf("0. Voltar\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", opcao) != 1) {
        while (getchar() != '\n'); 
        *opcao = -1;
    }
}

/**
 * Insere um item no final do vetor (O(1) se houver espaço).
 */
void inserirItemVetor(Item mochila[], int *contador) {
    if (*contador >= CAPACIDADE_MAXIMA) {
        printf("\n[ALERTA VETOR] Capacidade maxima de %d atingida. Nao e possivel inserir.\n", CAPACIDADE_MAXIMA);
        return;
    }

    printf("\n--- CADASTRO VETOR ---\n");
    printf("Nome: ");       scanf("%29s", mochila[*contador].nome);
    printf("Tipo: ");       scanf("%19s", mochila[*contador].tipo);
    printf("Quantidade: ");   scanf("%d", &mochila[*contador].quantidade);

    (*contador)++; 
    printf("[SUCESSO VETOR] Item '%s' cadastrado.\n", mochila[*contador - 1].nome);
}

/**
 * Remove um item do vetor pelo nome. Requer deslocamento dos elementos (O(n)).
 */
void removerItemVetor(Item mochila[], int *contador, const char nomeBusca[]) {
    if (*contador == 0) {
        printf("\n[ALERTA VETOR] Mochila vazia. Nada para remover.\n");
        return;
    }

    // Procura a posição do item a ser removido
    int posicao = buscarSequencialVetor(mochila, *contador, nomeBusca); 
    contadorComparacoes = 0; // Zera o contador após a busca interna

    if (posicao != -1) {
        // Deslocamento: Traz todos os itens após a posição 'posicao' uma posição para trás.
        for (int i = posicao; i < (*contador) - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*contador)--;
        printf("[SUCESSO VETOR] Item '%s' removido e vetor compactado.\n", nomeBusca);
    } else {
        printf("[ERRO VETOR] Item '%s' nao encontrado.\n", nomeBusca);
    }
}

/**
 * Lista todos os itens do vetor.
 */
void listarItensVetor(const Item mochila[], int contador) {
    printf("\n--- INVENTARIO VETOR (Tamanho Logico: %d) ---\n", contador);
    if (contador == 0) {
        printf(">>> O Vetor esta vazio. <<<\n");
    } else {
        for (int i = 0; i < contador; i++) {
            printf("%d. [Nome: %s | Tipo: %s | Qtd: %d]\n", 
                i + 1, 
                mochila[i].nome, 
                mochila[i].tipo, 
                mochila[i].quantidade
            );
        }
    }
    printf("---------------------------------------------\n");
}

/**
 * Ordena o vetor usando Selection Sort pelo nome (O(n^2)).
 */
void ordenarVetor(Item mochila[], int contador) {
    if (contador < 2) {
        printf("[ALERTA VETOR] Necessarios pelo menos 2 itens para ordenar.\n");
        return;
    }
    
    // Implementacao do Selection Sort
    for (int i = 0; i < contador - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < contador; j++) {
            // Compara os nomes para encontrar o menor elemento
            if (strcmp(mochila[j].nome, mochila[min_idx].nome) < 0) {
                min_idx = j;
            }
        }
        
        // Troca o elemento encontrado com o elemento atual (i)
        if (min_idx != i) {
            Item temp = mochila[i];
            mochila[i] = mochila[min_idx];
            mochila[min_idx] = temp;
        }
    }
    printf("[SUCESSO VETOR] Itens ordenados por nome (Selection Sort).\n");
}

/**
 * Busca Sequencial no Vetor (O(n)). Conta comparações.
 */
int buscarSequencialVetor(const Item mochila[], int contador, const char nomeBusca[]) {
    for (int i = 0; i < contador; i++) {
        contadorComparacoes++; // Uma comparação por iteração
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            return i; // Posição encontrada
        }
    }
    return -1; // Não encontrado
}

/**
 * Busca Binária no Vetor (O(log n)). Requer que o vetor esteja ordenado. Conta comparações.
 */
int buscarBinariaVetor(const Item mochila[], int contador, const char nomeBusca[]) {
    int inicio = 0;
    int fim = contador - 1;

    // Garante que o vetor nao esteja vazio
    if (contador == 0) return -1;

    // Loop de busca
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int resultadoComparacao;
        
        // Realiza a comparação central
        contadorComparacoes++;
        resultadoComparacao = strcmp(mochila[meio].nome, nomeBusca);

        if (resultadoComparacao == 0) {
            return meio; // Encontrado
        } else if (resultadoComparacao < 0) {
            inicio = meio + 1; // Item está na metade superior
        } else {
            fim = meio - 1; // Item está na metade inferior
        }
    }
    return -1; // Não encontrado
}

// -------------------------------------------------------------------
// IMPLEMENTAÇÕES: FUNÇÕES DA LISTA ENCADEADA
// -------------------------------------------------------------------

/**
 * Exibe o menu de opções para a estrutura de LISTA ENCADEADA.
 */
void menuLista(int *opcao) {
    printf("\n--- MOCHILA LISTA ENCADEADA (Tamanho Logico: %d) ---\n", contadorItensLista);
    printf("1. Inserir Item (Custo O(1) - Insercao no inicio)\n");
    printf("2. Remover Item (Custo O(n))\n");
    printf("3. Buscar Item (Sequencial - Custo O(n))\n");
    printf("0. Voltar\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", opcao) != 1) {
        while (getchar() != '\n'); 
        *opcao = -1;
    }
}

/**
 * Insere um novo item no INÍCIO da lista encadeada (O(1)).
 */
void inserirItemLista(No **cabeca, int *contador) {
    // 1. Aloca memória para o novo nó (malloc)
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("[ERRO LISTA] Falha ao alocar memoria.\n");
        return;
    }

    // 2. Coleta os dados
    printf("\n--- CADASTRO LISTA ---\n");
    printf("Nome: ");       scanf("%29s", novoNo->dados.nome);
    printf("Tipo: ");       scanf("%19s", novoNo->dados.tipo);
    printf("Quantidade: ");   scanf("%d", &novoNo->dados.quantidade);

    // 3. O 'proximo' do novo nó aponta para o antigo primeiro (cabeca)
    novoNo->proximo = *cabeca;
    
    // 4. A cabeça da lista agora aponta para o novo nó
    *cabeca = novoNo;

    (*contador)++;
    printf("[SUCESSO LISTA] Item '%s' cadastrado (Inserido no inicio).\n", novoNo->dados.nome);
}

/**
 * Remove um item da lista encadeada pelo nome (O(n)). Requer percorrer a lista.
 */
void removerItemLista(No **cabeca, int *contador, const char nomeBusca[]) {
    No *atual = *cabeca;
    No *anterior = NULL;

    if (*cabeca == NULL) {
        printf("\n[ALERTA LISTA] Mochila vazia. Nada para remover.\n");
        return;
    }

    // 1. Busca: Percorre a lista até encontrar o nó
    while (atual != NULL && strcmp(atual->dados.nome, nomeBusca) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // 2. Verifica se o nó foi encontrado
    if (atual == NULL) {
        printf("[ERRO LISTA] Item '%s' nao encontrado na lista.\n", nomeBusca);
        return;
    }

    // 3. Remoção
    if (anterior == NULL) {
        // Se anterior é NULL, o nó a ser removido é o primeiro (cabeca)
        *cabeca = atual->proximo;
    } else {
        // Se não é o primeiro, o anterior aponta para o próximo do nó atual (pulando-o)
        anterior->proximo = atual->proximo;
    }

    free(atual); // Libera a memória do nó removido
    (*contador)--;
    printf("[SUCESSO LISTA] Item '%s' removido.\n", nomeBusca);
}

/**
 * Lista todos os itens da lista encadeada.
 */
void listarItensLista(const No *cabeca, int contador) {
    const No *atual = cabeca;
    int indice = 1;

    printf("\n--- INVENTARIO LISTA ENCADEADA (Tamanho Logico: %d) ---\n", contador);
    if (cabeca == NULL) {
        printf(">>> A Lista esta vazia. <<<\n");
    } else {
        while (atual != NULL) {
            printf("%d. [Nome: %s | Tipo: %s | Qtd: %d]\n", 
                indice++, 
                atual->dados.nome, 
                atual->dados.tipo, 
                atual->dados.quantidade
            );
            atual = atual->proximo;
        }
    }
    printf("------------------------------------------------------\n");
}

/**
 * Busca Sequencial na Lista Encadeada (O(n)). Conta comparações.
 */
int buscarSequencialLista(const No *cabeca, const char nomeBusca[]) {
    const No *atual = cabeca;
    int posicao = 1; // Posição lógica para exibição

    while (atual != NULL) {
        contadorComparacoes++; // Uma comparação por iteração
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            return posicao; // Retorna a posição (começando em 1)
        }
        atual = atual->proximo;
        posicao++;
    }
    return -1; // Não encontrado
}