/*
 * Lista simplesmente encadeada - operações elementares
 * Inclusão, consulta, alteração e remoção.
 *
 * Cada nó armazena um registro: código (inteiro, único) e nome.
 *
 * Compilar: gcc -Wall -o lista lista_encadeada.c
 * Executar: ./lista
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

/* ---------- Estruturas ---------- */

typedef struct No {
    int codigo;
    char nome[TAM_NOME];
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    int tamanho;
} Lista;

/* ---------- Funções auxiliares de entrada ---------- */

static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static int ler_inteiro(const char *msg) {
    int valor, r;
    printf("%s", msg);
    while ((r = scanf("%d", &valor)) != 1) {
        if (r == EOF) {                 /* fim da entrada: evita loop infinito */
            printf("\nEntrada encerrada. Saindo...\n");
            exit(0);
        }
        limpar_buffer();
        printf("Entrada invalida. %s", msg);
    }
    limpar_buffer();
    return valor;
}

static void ler_texto(const char *msg, char *dest, int tam) {
    for (;;) {
        printf("%s", msg);
        if (fgets(dest, tam, stdin) == NULL) {      /* EOF */
            printf("\nEntrada encerrada. Saindo...\n");
            exit(0);
        }
        if (strchr(dest, '\n') == NULL)             /* texto maior que o limite: */
            limpar_buffer();                        /* descarta o excesso        */
        dest[strcspn(dest, "\n")] = '\0';
        if (dest[0] != '\0')
            return;
        printf("O nome nao pode ser vazio.\n");
    }
}

/* ---------- Operações básicas da lista ---------- */

void inicializar(Lista *l) {
    l->inicio = NULL;
    l->tamanho = 0;
}

/* CONSULTA: retorna ponteiro para o nó com o código, ou NULL */
No *buscar(const Lista *l, int codigo) {
    No *atual = l->inicio;
    while (atual != NULL) {
        if (atual->codigo == codigo)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

static No *criar_no(int codigo, const char *nome) {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return NULL;
    }
    novo->codigo = codigo;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->prox = NULL;
    return novo;
}

/* INCLUSÃO no início */
int inserir_inicio(Lista *l, int codigo, const char *nome) {
    if (buscar(l, codigo) != NULL) return 0;   /* código duplicado */
    No *novo = criar_no(codigo, nome);
    if (novo == NULL) return 0;
    novo->prox = l->inicio;
    l->inicio = novo;
    l->tamanho++;
    return 1;
}

/* INCLUSÃO no fim */
int inserir_fim(Lista *l, int codigo, const char *nome) {
    if (buscar(l, codigo) != NULL) return 0;
    No *novo = criar_no(codigo, nome);
    if (novo == NULL) return 0;
    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        No *atual = l->inicio;
        while (atual->prox != NULL)
            atual = atual->prox;
        atual->prox = novo;
    }
    l->tamanho++;
    return 1;
}

/* INCLUSÃO em uma posição (0 = início, tamanho = fim) */
int inserir_posicao(Lista *l, int pos, int codigo, const char *nome) {
    if (pos < 0 || pos > l->tamanho) return -1;  /* posição inválida */
    if (buscar(l, codigo) != NULL) return 0;     /* duplicado */
    if (pos == 0) return inserir_inicio(l, codigo, nome);

    No *novo = criar_no(codigo, nome);
    if (novo == NULL) return 0;
    No *anterior = l->inicio;
    for (int i = 0; i < pos - 1; i++)
        anterior = anterior->prox;
    novo->prox = anterior->prox;
    anterior->prox = novo;
    l->tamanho++;
    return 1;
}

/* ALTERAÇÃO do nome de um registro */
int alterar(Lista *l, int codigo, const char *novo_nome) {
    No *no = buscar(l, codigo);
    if (no == NULL) return 0;
    strncpy(no->nome, novo_nome, TAM_NOME - 1);
    no->nome[TAM_NOME - 1] = '\0';
    return 1;
}

/* REMOÇÃO por código */
int remover(Lista *l, int codigo) {
    No *atual = l->inicio;
    No *anterior = NULL;

    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) return 0;   /* não encontrado */

    if (anterior == NULL)
        l->inicio = atual->prox;   /* removendo o primeiro */
    else
        anterior->prox = atual->prox;

    free(atual);
    l->tamanho--;
    return 1;
}

void listar(const Lista *l) {
    if (l->inicio == NULL) {
        printf("\nA lista esta vazia.\n");
        return;
    }
    printf("\n--- Lista (%d elemento%s) ---\n", l->tamanho,
           l->tamanho == 1 ? "" : "s");
    printf("INICIO -> ");
    for (No *p = l->inicio; p != NULL; p = p->prox)
        printf("[%d | %s] -> ", p->codigo, p->nome);
    printf("NULL\n");
}

void liberar(Lista *l) {
    No *atual = l->inicio;
    while (atual != NULL) {
        No *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    l->inicio = NULL;
    l->tamanho = 0;
}

/* ---------- Interface com o usuário ---------- */

static void menu(void) {
    printf("\n=========== LISTA ENCADEADA ===========\n");
    printf(" 1 - Incluir no inicio\n");
    printf(" 2 - Incluir no fim\n");
    printf(" 3 - Incluir em uma posicao\n");
    printf(" 4 - Consultar por codigo\n");
    printf(" 5 - Listar todos\n");
    printf(" 6 - Alterar nome\n");
    printf(" 7 - Remover por codigo\n");
    printf(" 0 - Sair\n");
    printf("=======================================\n");
}

int main(void) {
    Lista lista;
    int opcao, codigo, pos, r;
    char nome[TAM_NOME];

    inicializar(&lista);

    do {
        menu();
        opcao = ler_inteiro("Escolha uma opcao: ");

        switch (opcao) {
        case 1:
            codigo = ler_inteiro("Codigo: ");
            ler_texto("Nome: ", nome, TAM_NOME);
            printf("%s", inserir_inicio(&lista, codigo, nome)
                   ? "Registro incluido no inicio.\n"
                   : "Nao foi possivel incluir (codigo ja existe?).\n");
            break;

        case 2:
            codigo = ler_inteiro("Codigo: ");
            ler_texto("Nome: ", nome, TAM_NOME);
            printf("%s", inserir_fim(&lista, codigo, nome)
                   ? "Registro incluido no fim.\n"
                   : "Nao foi possivel incluir (codigo ja existe?).\n");
            break;

        case 3:
            printf("Posicoes validas: 0 a %d\n", lista.tamanho);
            pos = ler_inteiro("Posicao: ");
            codigo = ler_inteiro("Codigo: ");
            ler_texto("Nome: ", nome, TAM_NOME);
            r = inserir_posicao(&lista, pos, codigo, nome);
            if (r == 1)       printf("Registro incluido na posicao %d.\n", pos);
            else if (r == -1) printf("Posicao invalida.\n");
            else              printf("Nao foi possivel incluir (codigo ja existe?).\n");
            break;

        case 4: {
            codigo = ler_inteiro("Codigo a consultar: ");
            No *no = buscar(&lista, codigo);
            if (no != NULL)
                printf("Encontrado -> Codigo: %d | Nome: %s\n", no->codigo, no->nome);
            else
                printf("Codigo %d nao encontrado.\n", codigo);
            break;
        }

        case 5:
            listar(&lista);
            break;

        case 6:
            codigo = ler_inteiro("Codigo do registro a alterar: ");
            if (buscar(&lista, codigo) == NULL) {
                printf("Codigo %d nao encontrado.\n", codigo);
            } else {
                ler_texto("Novo nome: ", nome, TAM_NOME);
                alterar(&lista, codigo, nome);
                printf("Registro alterado com sucesso.\n");
            }
            break;

        case 7:
            codigo = ler_inteiro("Codigo a remover: ");
            printf("%s", remover(&lista, codigo)
                   ? "Registro removido.\n"
                   : "Codigo nao encontrado.\n");
            break;

        case 0:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    liberar(&lista);   /* evita vazamento de memoria */
    return 0;
}
