#include <stdio.h>  // Necessario para as funcoes de entrada/saida (printf, scanf)
#include <string.h> // Necessario para manipulacao de strings (strcpy, strcmp)
#include <stdlib.h> // Necessario para funcoes utilitarias (exit, etc.)

// --- CONSTANTES ---
#define CAPACIDADE_MAXIMA 10 
#define MAX_NOME 30
#define MAX_TIPO 20

// -------------------------------------------------------------------
// REQUISITO: Criacao da struct 'Item'
// -------------------------------------------------------------------
struct Item {
    char nome[MAX_NOME];     
    char tipo[MAX_TIPO];     
    int quantidade;         
};

// Define um novo tipo chamado 'Item' para simplificar a declaracao
typedef struct Item Item;

// -------------------------------------------------------------------
// PROTOTIPOS DAS FUNCOES OBRIGATORIAS
// -------------------------------------------------------------------
void exibirMenu();
void inserirItem(Item mochila[], int *contador);
void removerItem(Item mochila[], int *contador);
void listarItens(const Item mochila[], int contador);
void buscarItem(const Item mochila[], int contador);
int buscarPosicao(const Item mochila[], int contador, const char nomeBusca[]);


// --- Variavel Global para o Sistema ---
// REQUISITO: Vetor de structs para a lista sequencial (mochila)
Item mochila[CAPACIDADE_MAXIMA]; 

// Variavel que rastreia quantos itens estao CADASTRADOS (tamanho logico do vetor)
int contadorItens = 0; 


// -------------------------------------------------------------------
// FUNCAO PRINCIPAL
// -------------------------------------------------------------------
int main() {
    int opcao;

    printf("=== SISTEMA DE INVENTARIO (MOCHILA) ===\n");
    
    // REQUISITO: Uso de laco 'do-while' para o fluxo principal do programa (menu interativo)
    do {
        listarItens(mochila, contadorItens); // REQUISITO: Lista itens apos cada operacao
        exibirMenu();
        
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de entrada invalida (nao-numerica)
            while (getchar() != '\n'); 
            opcao = -1; // Garante que caia no 'default'
        }

        switch (opcao) {
            case 1:
                inserirItem(mochila, &contadorItens);
                break;
            case 2:
                removerItem(mochila, &contadorItens);
                break;
            case 3:
                buscarItem(mochila, contadorItens);
                break;
            case 0:
                printf("\nFechando a mochila. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}


// -------------------------------------------------------------------
// IMPLEMENTACOES DAS FUNCOES
// -------------------------------------------------------------------

/**
 * Exibe o menu de opcoes para o usuario.
 */
void exibirMenu() {
    printf("\n--- OPCOES DA MOCHILA ---\n");
    printf("1. Cadastrar novo item\n");
    printf("2. Remover item (pelo nome)\n");
    printf("3. Buscar item (pelo nome)\n");
    printf("0. Sair\n");
}

/**
 * Insere um novo item no vetor da mochila, se houver espaco.
 * mochila O vetor de structs Item.
 *  contador Ponteiro para o contador de itens (para atualiza-lo).
 */
void inserirItem(Item mochila[], int *contador) {
    // Verifica se a capacidade maxima foi atingida
    if (*contador >= CAPACIDADE_MAXIMA) {
        printf("\nMOCHILA CHEIA! Capacidade maxima de %d itens atingida.\n", CAPACIDADE_MAXIMA);
        return;
    }

    printf("\n--- CADASTRO DE ITEM ---\n");
    
    // REQUISITO: Leitura de dados (usando scanf e %s para strings)
    
    printf("Nome do item (max %d caracteres): ", MAX_NOME - 1);
    scanf("%29s", mochila[*contador].nome); // Protecao contra overflow: %29s
    
    printf("Tipo do item (ex: arma, municao, cura): ");
    scanf("%19s", mochila[*contador].tipo);
    
    printf("Quantidade: ");
    scanf("%d", &mochila[*contador].quantidade);

    // Incrementa o contador para refletir o novo item na mochila
    (*contador)++; 
    printf("\n[SUCESSO] Item '%s' cadastrado!\n", mochila[*contador - 1].nome);
}

/**
 * Remove um item da mochila pelo nome, realocando os itens subsequentes.
 * mochila O vetor de structs Item.
 *  contador Ponteiro para o contador de itens.
 */
void removerItem(Item mochila[], int *contador) {
    if (*contador == 0) {
        printf("\n[ALERTA] A mochila esta vazia. Nada para remover.\n");
        return;
    }

    char nomeRemover[MAX_NOME];
    printf("\n--- REMOCAO DE ITEM ---\n");
    printf("Digite o NOME do item que deseja remover: ");
    scanf("%29s", nomeRemover);

    // REQUISITO: Uso de busca sequencial para localizar a posicao
    int posicao = buscarPosicao(mochila, *contador, nomeRemover);

    if (posicao != -1) {
        printf("[SUCESSO] Item '%s' removido.\n", mochila[posicao].nome);
        
        // Deslocamento dos itens subsequentes para "cobrir" o espaco vazio
        // REQUISITO: Uso de laco 'for' para manipulacao do vetor
        for (int i = posicao; i < (*contador) - 1; i++) {
            // Copia o conteudo da struct do item seguinte (i + 1) para a posicao atual (i)
            mochila[i] = mochila[i + 1];
        }

        // Decrementa o contador, reduzindo o tamanho logico do vetor
        (*contador)--;
    } else {
        printf("\n[ERRO] Item '%s' nao encontrado na mochila.\n", nomeRemover);
    }
}

/**
 * Lista todos os itens atualmente registrados na mochila.
 *  mochila O vetor de structs Item.
 *  contador O numero atual de itens na mochila.
 */
void listarItens(const Item mochila[], int contador) {
    printf("\n========================================\n");
    printf("    INVENTARIO ATUAL (TOTAL: %d/%d)   \n", contador, CAPACIDADE_MAXIMA);
    printf("========================================\n");

    if (contador == 0) {
        printf(">>> A mochila esta vazia. Comece a coletar recursos! <<<\n");
    } else {
        // REQUISITO: Uso de laco 'for' para percorrer o vetor
        for (int i = 0; i < contador; i++) {
            printf("%d. [Nome: %s | Tipo: %s | Qtd: %d]\n", 
                i + 1, 
                mochila[i].nome, 
                mochila[i].tipo, 
                mochila[i].quantidade
            );
        }
    }
    printf("----------------------------------------\n");
}

/**
 *  Implementa a funcao de busca sequencial para localizar a posicao de um item.
 *  mochila O vetor de structs Item.
 *  contador O numero atual de itens.
 *  nomeBusca O nome do item a ser buscado.
 * O indice do item se encontrado, ou -1 se nao encontrado.
 */
int buscarPosicao(const Item mochila[], int contador, const char nomeBusca[]) {
    // REQUISITO: Busca sequencial
    // Percorre cada elemento do vetor um por um, do inicio ao fim.
    for (int i = 0; i < contador; i++) {
        // Compara se o nome do item no indice 'i' e igual ao nome buscado
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            return i; // Retorna a posicao (indice) onde o item foi encontrado
        }
    }
    return -1; // Retorna -1 se o item nao for encontrado apos percorrer todo o vetor
}

/**
 *  Permite ao usuario buscar um item e exibe seus dados.
 *  mochila O vetor de structs Item.
 * contador O numero atual de itens.
 */
void buscarItem(const Item mochila[], int contador) {
    if (contador == 0) {
        printf("\n[ALERTA] A mochila esta vazia. Nada para buscar.\n");
        return;
    }
    
    char nomeBusca[MAX_NOME];
    printf("\n--- BUSCA SEQUENCIAL ---\n");
    printf("Digite o NOME do item que deseja buscar: ");
    scanf("%29s", nomeBusca);
    
    int posicao = buscarPosicao(mochila, contador, nomeBusca);
    
    if (posicao != -1) {
        // Se a posicao for valida (diferente de -1), exibe os dados
        printf("\n[ENCONTRADO!] Dados do Item %s:\n", mochila[posicao].nome);
        printf(" > Tipo: %s\n", mochila[posicao].tipo);
        printf(" > Quantidade: %d\n", mochila[posicao].quantidade);
    } else {
        printf("\n[ERRO] Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}