// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define TAM_STRING 50
#define ID_MISSAO_DOMINAR 1
#define ID_MISSAO_DESTRUIR 2

// Definição das cores para simplificar a lógica
#define COR_JOGADOR "Azul"
#define COR_INIMIGO "Vermelho"

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);
void limparBufferEntrada();

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *atacante, Territorio *defensor);
int sortearMissao(); 

// Função utilitária:
int verificarVitoria(const Territorio *mapa, int idMissao);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
// - Define o locale para português.
// - Inicializa a semente para geração de números aleatórios com base no tempo atual.
// - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
// - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
// - Define a cor do jogador e sorteia sua missão secreta.
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        return 1; // Encerra se falhar alocação
    }

    inicializarTerritorios(mapa);
    int missaoAtual = sortearMissao();
    int opcao = -1;
    

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
do {
        printf("\n\n\n===========================================\n");
        
        exibirMapa(mapa);
        exibirMissao(missaoAtual);
        exibirMenuPrincipal();
        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, missaoAtual)) {
                    printf("\n*** PARABENS! Voce completou sua missao e VENCEU o jogo! ***\n");
                    opcao = 0; // Força saída do loop
                } else {
                    printf("\n--- Missao ainda nao cumprida. Continue lutando! ---\n");
                }
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa() {
    Territorio *ptr = (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (ptr == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria.\n");
    }
    return ptr;
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio *mapa) {
    // Território 0 e 1 são do Jogador (Azul)
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, COR_JOGADOR);
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Franca");
    strcpy(mapa[1].cor, COR_JOGADOR);
    mapa[1].tropas = 3;

    // Territórios 2, 3 e 4 são Inimigos (Vermelho)
    strcpy(mapa[2].nome, "Australia");
    strcpy(mapa[2].cor, COR_INIMIGO);
    mapa[2].tropas = 2;

    strcpy(mapa[3].nome, "Egito");
    strcpy(mapa[3].cor, COR_INIMIGO);
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Russia");
    strcpy(mapa[4].cor, COR_INIMIGO);
    mapa[4].tropas = 3;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("Memoria liberada com sucesso.\n");
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACAO ---\n");
    printf("1. Atacar Territorio\n");
    printf("2. Verificar Objetivo (Vitoria)\n");
    printf("0. Sair do Jogo\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACAO ---\n");
    printf("1. Atacar Territorio\n");
    printf("2. Verificar Objetivo (Vitoria)\n");
    printf("0. Sair do Jogo\n");
}
// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int idMissao) {
    printf("\n[ MISSAO SECRETA ]: ");
    if (idMissao == ID_MISSAO_DOMINAR) {
        printf("Conquistar pelo menos 4 territorios.\n");
    } else if (idMissao == ID_MISSAO_DESTRUIR) {
        printf("Eliminar totalmente o exercito Vermelho.\n");
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio *mapa) {
    int origem, destino;
    
    printf("\n>>> FASE DE ATAQUE <<<\n");
    printf("Digite o ID do territorio de ORIGEM (seu): ");
    scanf("%d", &origem);
    
    printf("Digite o ID do territorio de DESTINO (inimigo): ");
    scanf("%d", &destino);
    limparBufferEntrada();

    // Validações básicas antes da batalha
    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("IDs invalidos!\n");
        return;
    }

    if (strcmp(mapa[origem].cor, COR_JOGADOR) != 0) {
        printf("Voce so pode atacar partindo de um territorio seu!\n");
        return;
    }

    if (strcmp(mapa[origem].cor, mapa[destino].cor) == 0) {
        printf("Voce nao pode atacar seu proprio territorio!\n");
        return;
    }

    if (mapa[origem].tropas <= 1) {
        printf("Tropas insuficientes para atacar (minimo 2).\n");
        return;
    }

    // Chama a simulação passando os endereços específicos
    simularAtaque(&mapa[origem], &mapa[destino]);
}
// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    printf("\nBatalha: %s (Atk) vs %s (Def)\n", atacante->nome, defensor->nome);
    
    // Simulação simples de dados (1 dado cada para simplificar este exemplo)
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("Vitoria do Atacante! Defensor perde 1 tropa.\n");
        defensor->tropas--;
    } else {
        printf("Defesa segurou! Atacante perde 1 tropa.\n");
        atacante->tropas--;
    }

    // Checagem de Conquista
    if (defensor->tropas <= 0) {
        printf(">>> TERRITORIO CONQUISTADO! <<<\n");
        printf("O territorio %s agora pertence ao exercito %s!\n", defensor->nome, atacante->cor);
        
        strcpy(defensor->cor, atacante->cor); // Muda a cor
        defensor->tropas = 1; // Ocupa com 1 tropa
        atacante->tropas--;   // Remove 1 da origem que foi mover pra lá
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return (rand() % 2) + 1;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio *mapa, int idMissao) {
    int qtdMeusTerritorios = 0;
    int qtdInimigos = 0;

    // Varredura única do mapa para coletar estatísticas
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, COR_JOGADOR) == 0) {
            qtdMeusTerritorios++;
        } else {
            qtdInimigos++; // Se não é meu, é inimigo (neste exemplo simples de 2 cores)
        }
    }

    // Validação baseada na missão
    if (idMissao == ID_MISSAO_DOMINAR) {
        // Objetivo: Ter 4 ou mais territórios
        if (qtdMeusTerritorios >= 4) return 1;
    } 
    else if (idMissao == ID_MISSAO_DESTRUIR) {
        // Objetivo: Inimigo ter 0 territórios
        if (qtdInimigos == 0) return 1;
    }

    return 0; // Ainda não venceu
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}