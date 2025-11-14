#include <stdio.h>     // Funcoes de entrada/saida (printf, scanf, fgets)
#include <stdlib.h>    // Funcoes utilitarias (malloc, free)
#include <string.h>    // Manipulacao de strings (strcmp, strcpy)
#include <time.h>      // Medicao de tempo (clock, CLOCKS_PER_SEC)

// ============================================================================
//         PROJETO FREEFIRE - TORRE DE FUGA 
// ============================================================================
// Matrícula: 20250212488
// Faculdade: Estácio
// Aluno: Ângelo Oliveira dos Santos
// Curso: Análise e desenvolvimento de sistemas
// Desafio: nível mestre - Código da Ilha
// Sistema de organização dos componentes para ativação da torre de fuga
// ============================================================================

// --- CONSTANTES ---
#define CAPACIDADE_MOCHILA 10    // Capacidade maxima da mochila
#define MAX_NOME 30
#define MAX_TIPO 20

// -------------------------------------------------------------------
// 1. CRIAÇÃO DA STRUCT COMPONENTE
// -------------------------------------------------------------------
struct Componente {
    char nome[MAX_NOME];      // Nome do componente (ex: "VigaDeAco")
    char tipo[MAX_TIPO];      // Tipo (ex: "estrutural", "eletronico", "energia")
    int prioridade;          // Nível de prioridade (1 a 5)
    int quantidade;          // Quantidade do componente
};
typedef struct Componente Componente;

// -------------------------------------------------------------------
// VARIÁVEIS GLOBAIS DE MEDIÇÃO
// -------------------------------------------------------------------
long long int contadorComparacoes = 0;
double tempoExecucao = 0.0;
int ordenadoPorNome = 0; // 0 = não ordenado, 1 = ordenado

// -------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -------------------------------------------------------------------
void lerStringSegura(char *str, int tamanho);
void mostrarMenuPrincipal(Componente mochila[], int tamanho);
void adicionarComponente(Componente mochila[], int *tamanho);
void descartarComponente(Componente mochila[], int *tamanho);
void listarComponentes(const Componente mochila[], int tamanho);
void organizarMochila(Componente mochila[], int tamanho);
void buscaBinariaComponente(const Componente mochila[], int tamanho);
void trocar(Componente *a, Componente *b);

// Algoritmos de ordenação
void bubbleSortNome(Componente lista[], int tamanho);
void insertionSortTipo(Componente lista[], int tamanho);
void selectionSortPrioridade(Componente lista[], int tamanho);

// -------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MAIN)
// -------------------------------------------------------------------
int main() {
    Componente mochila[CAPACIDADE_MOCHILA];
    int numComponentes = 0;
    int opcao;
    
    printf("=== PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE) ===\n");
    printf("=== SISTEMA DE ATIVACAO DA TORRE DE FUGA ===\n\n");

    do {
        mostrarMenuPrincipal(mochila, numComponentes);
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); 
            opcao = -1;
        }
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                adicionarComponente(mochila, &numComponentes);
                ordenadoPorNome = 0; // Ao adicionar, marca como não ordenado
                break;
            case 2:
                descartarComponente(mochila, &numComponentes);
                ordenadoPorNome = 0; // Ao remover, marca como não ordenado
                break;
            case 3:
                listarComponentes(mochila, numComponentes);
                break;
            case 4:
                organizarMochila(mochila, numComponentes);
                break;
            case 5:
                buscaBinariaComponente(mochila, numComponentes);
                break;
            case 6:
                printf("\n=== ATIVANDO TORRE DE FUGA ===\n");
                printf("Sequencia de componentes verificada...\n");
                printf("Energia fluindo para os sistemas...\n");
                printf("TORRE ATIVADA! FUGA BEM SUCEDIDA!\n");
                printf("Obrigado por jogar FreeFire - Codigo da Ilha!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        
        if (opcao != 6) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 6);

    return 0;
}

// -------------------------------------------------------------------
// FUNÇÕES DE UTILIDADE
// -------------------------------------------------------------------

void lerStringSegura(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    str[strcspn(str, "\n")] = '\0';
}

void mostrarMenuPrincipal(Componente mochila[], int tamanho) {
    printf("\n--- ---\n");
    printf("PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("--- ---\n");
    printf("Itens na Mochila: %d/%d\n", tamanho, CAPACIDADE_MOCHILA);
    printf("Status da Ordenacao por Nome: %s\n", ordenadoPorNome ? "ORDENADO" : "NAO ORDENADO");
    printf("\n1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome)\n");
    printf("6. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("--- ---\n");
    printf("Escolha uma opcao: ");
}

void adicionarComponente(Componente mochila[], int *tamanho) {
    if (*tamanho >= CAPACIDADE_MOCHILA) {
        printf("\n[ERRO] Mochila cheia! Capacidade maxima: %d\n", CAPACIDADE_MOCHILA);
        return;
    }

    printf("\n--- Coletando Novo Componente ---\n");
    
    printf("Nome: ");
    lerStringSegura(mochila[*tamanho].nome, MAX_NOME);
    
    printf("Tipo: ");
    lerStringSegura(mochila[*tamanho].tipo, MAX_TIPO);
    
    printf("Prioridade (1-5): ");
    if (scanf("%d", &mochila[*tamanho].prioridade) != 1 || 
        mochila[*tamanho].prioridade < 1 || 
        mochila[*tamanho].prioridade > 5) {
        printf("[AVISO] Prioridade invalida. Definindo como 3.\n");
        mochila[*tamanho].prioridade = 3;
    }
    
    printf("Quantidade: ");
    if (scanf("%d", &mochila[*tamanho].quantidade) != 1 || 
        mochila[*tamanho].quantidade < 1) {
        printf("[AVISO] Quantidade invalida. Definindo como 1.\n");
        mochila[*tamanho].quantidade = 1;
    }
    getchar(); // Limpa buffer
    
    (*tamanho)++;
    printf("[SUCESSO] Componente '%s' adicionado a mochila!\n", mochila[*tamanho - 1].nome);
}

void descartarComponente(Componente mochila[], int *tamanho) {
    if (*tamanho == 0) {
        printf("\n[AVISO] Mochila vazia! Nada para descartar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Descartar Componente ---\n");
    printf("Nome do componente a descartar: ");
    lerStringSegura(nomeBusca, MAX_NOME);

    // Busca sequencial
    int posicao = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("[ERRO] Componente '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    // Remove deslocando elementos
    for (int i = posicao; i < *tamanho - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*tamanho)--;
    printf("[SUCESSO] Componente '%s' descartado!\n", nomeBusca);
}

void listarComponentes(const Componente mochila[], int tamanho) {
    printf("\n--- Inventario da Mochila (%d/%d) ---\n", tamanho, CAPACIDADE_MOCHILA);
    
    if (tamanho == 0) {
        printf("Mochila vazia. Colete componentes para construir a torre!\n");
        return;
    }

    printf("Pos | Nome                     | Tipo                  | Prioridade | Quantidade\n");
    printf("------------------------------------------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-3d | %-22s | %-20s | %-10d | %d\n", 
            i + 1, 
            mochila[i].nome, 
            mochila[i].tipo, 
            mochila[i].prioridade,
            mochila[i].quantidade
        );
    }
}

void organizarMochila(Componente mochila[], int tamanho) {
    if (tamanho < 2) {
        printf("\n[AVISO] Necessario pelo menos 2 componentes para organizar.\n");
        return;
    }

    int opcao;
    printf("\n--- Estrategias de Organizacao ---\n");
    printf("1. Ordenar por NOME (Bubble Sort)\n");
    printf("2. Ordenar por TIPO (Insertion Sort)\n");
    printf("3. Ordenar por PRIORIDADE (Selection Sort)\n");
    printf("Escolha o metodo: ");
    
    scanf("%d", &opcao);
    getchar();

    contadorComparacoes = 0;
    tempoExecucao = 0.0;

    switch (opcao) {
        case 1:
            printf("\n[Aplicando Bubble Sort por Nome...]\n");
            bubbleSortNome(mochila, tamanho);
            ordenadoPorNome = 1;
            break;
        case 2:
            printf("\n[Aplicando Insertion Sort por Tipo...]\n");
            insertionSortTipo(mochila, tamanho);
            ordenadoPorNome = 0;
            break;
        case 3:
            printf("\n[Aplicando Selection Sort por Prioridade...]\n");
            selectionSortPrioridade(mochila, tamanho);
            ordenadoPorNome = 0;
            break;
        default:
            printf("\nOpcao invalida!\n");
            return;
    }

    printf("[ORGANIZACAO CONCLUIDA]\n");
    printf("Metricas: %lld comparacoes | %.6f segundos\n", contadorComparacoes, tempoExecucao);
}

void buscaBinariaComponente(const Componente mochila[], int tamanho) {
    if (tamanho == 0) {
        printf("\n[ERRO] Mochila vazia! Nada para buscar.\n");
        return;
    }

    if (!ordenadoPorNome) {
        printf("\n[ALERTA] A mochila nao esta ordenada por nome!\n");
        printf("A busca binaria requer ordenacao por nome primeiro.\n");
        printf("Deseja ordenar agora? (s/n): ");
        
        char resposta;
        scanf("%c", &resposta);
        getchar();
        
        if (resposta == 's' || resposta == 'S') {
            // Cria copia temporaria para ordenar
            Componente temp[CAPACIDADE_MOCHILA];
            memcpy(temp, mochila, sizeof(Componente) * tamanho);
            bubbleSortNome(temp, tamanho);
            printf("[Mochila ordenada para busca]\n");
            
            // Continua com a busca na copia ordenada
            char nomeBusca[MAX_NOME];
            printf("Nome do componente-chave: ");
            lerStringSegura(nomeBusca, MAX_NOME);
            
            contadorComparacoes = 0;
            int posicao = -1;
            int inicio = 0, fim = tamanho - 1;
            
            while (inicio <= fim) {
                int meio = inicio + (fim - inicio) / 2;
                contadorComparacoes++;
                int cmp = strcmp(temp[meio].nome, nomeBusca);
                
                if (cmp == 0) {
                    posicao = meio;
                    break;
                } else if (cmp < 0) {
                    inicio = meio + 1;
                } else {
                    fim = meio - 1;
                }
            }
            
            if (posicao != -1) {
                printf("\n[COMPONENTE-CHAVE ENCONTRADO!]\n");
                printf("Componente: %s | Tipo: %s | Prioridade: %d | Quantidade: %d\n", 
                    temp[posicao].nome, temp[posicao].tipo, temp[posicao].prioridade, temp[posicao].quantidade);
            } else {
                printf("\n[COMPONENTE NAO ENCONTRADO]\n");
            }
            printf("Comparacoes realizadas: %lld\n", contadorComparacoes);
            return;
        }
    }

    // Se já está ordenado, faz busca normal
    char nomeBusca[MAX_NOME];
    printf("Nome do componente-chave: ");
    lerStringSegura(nomeBusca, MAX_NOME);
    
    contadorComparacoes = 0;
    int posicao = -1;
    int inicio = 0, fim = tamanho - 1;
    
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        contadorComparacoes++;
        int cmp = strcmp(mochila[meio].nome, nomeBusca);
        
        if (cmp == 0) {
            posicao = meio;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    
    if (posicao != -1) {
        printf("\n[COMPONENTE-CHAVE ENCONTRADO!]\n");
        printf("Componente: %s | Tipo: %s | Prioridade: %d | Quantidade: %d\n", 
            mochila[posicao].nome, mochila[posicao].tipo, mochila[posicao].prioridade, mochila[posicao].quantidade);
    } else {
        printf("\n[COMPONENTE NAO ENCONTRADO]\n");
    }
    printf("Comparacoes realizadas: %lld\n", contadorComparacoes);
}

// -------------------------------------------------------------------
// ALGORITMOS DE ORDENAÇÃO (Implementações)
// -------------------------------------------------------------------

void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSortNome(Componente lista[], int tamanho) {
    clock_t inicio = clock();
    int trocou;

    for (int i = 0; i < tamanho - 1; i++) {
        trocou = 0;
        for (int j = 0; j < tamanho - 1 - i; j++) {
            contadorComparacoes++;
            if (strcmp(lista[j].nome, lista[j + 1].nome) > 0) {
                trocar(&lista[j], &lista[j + 1]);
                trocou = 1;
            }
        }
        if (!trocou) break;
    }

    clock_t fim = clock();
    tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

void insertionSortTipo(Componente lista[], int tamanho) {
    clock_t inicio = clock();
    
    for (int i = 1; i < tamanho; i++) {
        Componente chave = lista[i];
        int j = i - 1;

        while (j >= 0) {
            contadorComparacoes++;
            if (strcmp(lista[j].tipo, chave.tipo) > 0) {
                lista[j + 1] = lista[j];
                j--;
            } else {
                break;
            }
        }
        lista[j + 1] = chave;
    }

    clock_t fim = clock();
    tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

void selectionSortPrioridade(Componente lista[], int tamanho) {
    clock_t inicio = clock();
    
    for (int i = 0; i < tamanho - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < tamanho; j++) {
            contadorComparacoes++;
            if (lista[j].prioridade < lista[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocar(&lista[i], &lista[min_idx]);
        }
    }
    
    clock_t fim = clock();
    tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
}