#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//variavel global para contar o numero de folhas
static int folhas = 0;

//estrutura da arvore binaria
typedef struct tree {
    int info;
    struct tree *esq;
    struct tree *dir;
} tree;

//funcao para imprimir a arvore em pre-ordem, em-ordem e pos-ordem
void print (tree *a, int type) {
    if (a != NULL) {
        if (type == 1) {
            printf("%d ", a->info);
        }
        print(a->esq, type);
        if (type == 2) {
            printf("%d ", a->info);
        }
        print(a->dir, type);
        if (type == 3) {
            printf("%d ", a->info);
        }
    }
}

//funcao para verificar se um elemento existe na arvore
int exist (tree *a, int x) {
    if (a == NULL) {
        return 0;
    } else if (a->info == x) {
        return 1;
    }

    return exist(a->esq, x) || exist(a->dir, x);
}

//funcao para contar o numero de elementos da arvore
int count (tree *a) {
    if (a != NULL){
        return 1 + count(a->esq) + count(a->dir);
    } else {
        return 0;
    }
}

//funcao para ler a arvore de um arquivo
tree *readfile (FILE *arq) {
    char c;
    int num;

    fscanf(arq, "%c", &c);
    fscanf(arq, "%d", &num);

    if (num == -1) {
        fscanf(arq, "%c", &c);
        return NULL;
    } else {
        tree *a = (tree*) malloc(sizeof(tree));
        a->info = num;
        a->esq = readfile(arq);
        a->dir = readfile(arq);
        fscanf(arq, "%c", &c);
        return a;
    }
}

//funcao para imprimir os nos folha da arvore
void printfolha (tree *a) {
    if (a != NULL) {
        if (a->esq == NULL && a->dir == NULL) {
            printf("%d ", a->info);
            folhas ++;
        }
        printfolha(a->esq);
        printfolha(a->dir);
    }
}

//funcao para liberar a memoria alocada para a arvore
void freetree(tree *a) {
    if (a != NULL) {
        freetree(a->esq);
        freetree(a->dir);
        free(a);
    }
}

//funcao para corrigir o nome do arquivo
void arqname (char *a){
    if (strstr(a, ".txt") == NULL) {
        strcat(a, ".txt");
    }
}

int main() {
    tree *a = NULL;
    FILE *arq;
    int x, menu = 0;
    char name[100];

    printf("\nPrograma de manipulacao de arvore binaria\n");
    printf("=========================================\n");

    //menu de opcoes
    while(menu != 6) {
        printf("\nEscolha uma opcao:\n");
        printf("1. Ler uma arvore\n");
        printf("2. Imprimir a arvore\n");
        printf("3. Verificar se um elemento existe na arvore\n");
        printf("4. Contar o numero de elementos da arvore\n");
        printf("5. Imprimir nos folha da arvore\n");
        printf("6. Sair\n");
        printf("\nOpcao: ");

        scanf("%d", &menu);

        switch(menu) {
            case 1:
                //obter o nome do arquivo
                getchar();
                printf("\nDigite o nome do arquivo .txt: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] ='\0';

                printf("\nLendo arquivo...\n");

                //adiciona a extensao .txt ao nome do arquivo, se necessario
                arqname(name);

                //abrir o arquivo
                arq = fopen(name, "r");
                if (arq == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    exit(1);
                }
                a = readfile(arq);
                fclose(arq);
                printf("Arquivo lido com sucesso!\n");
                break;
            case 2:
                while (1) {
                    printf("\nEscolha o tipo de impressao:\n");
                    printf("1. Pre-ordem\n");
                    printf("2. Em-ordem\n");
                    printf("3. Pos-ordem\n");
                    printf("\nOpcao: ");
                    scanf("%d", &x);
                    if (x >= 1 && x <= 3) {
                        switch (x) {
                            case 1: printf("\n Pre-ordem: "); break;
                            case 2: printf("\n Em-ordem: "); break;
                            case 3: printf("\n Pos-ordem: "); break;
                        }
                        print(a, x);
                        printf("\n");
                        break;
                    }
                    printf("Opcao invalida!\n");
                }
                break;
            case 3:
                printf("\nDigite o elemento a ser buscado: ");
                scanf("%d", &x);
                if (exist(a, x)) {
                    printf("\nO elemento %d existe na arvore.\n", x);
                } else {
                    printf("\nO elemento %d nao existe na arvore.\n", x);
                }
                break;
            case 4:
                printf("\n");
                printf("O numero de elementos na arvore e: %d\n", count(a));
                break;
            case 5:
                printf("\nNos folha: ");
                printfolha(a);
                printf("\nTotal: %d\n", folhas);
                folhas = 0;
                break;
            case 6:
                printf("Saindo...\n");
                freetree(a);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}