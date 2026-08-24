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

//--estrutura e operações da fila para a impressao em largura
typedef struct QueueNode {
    tree *node;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue* createQueue() {
    Queue *q = (Queue*) malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

int isEmpty(Queue *q) {
    return q->front == NULL;
}

void enqueue(Queue *q, tree *node) {
    if (node == NULL) return;

    QueueNode *newNode = (QueueNode*) malloc(sizeof(QueueNode));
    newNode->node = node;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

tree* dequeue(Queue *q) {
    if (isEmpty(q)) return NULL;

    QueueNode *temp = q->front;
    tree *node = temp->node;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return node;
}

void freeQueue(Queue *q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
    free(q);
}
//--fim da estrutura e operações da fila

//funcao para imprimir a arvore em largura usando a fila
void print_largura(tree *a) {
    if (a == NULL) return;

    Queue *q = createQueue();
    enqueue(q, a);

    while (!isEmpty(q)) {
        tree *curr = dequeue(q);
        printf("%d ", curr->info);

        if (curr->esq != NULL) {
            enqueue(q, curr->esq);
        }
        if (curr->dir != NULL) {
            enqueue(q, curr->dir);
        }
    }

    freeQueue(q);
}

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

//funcao para o calculo da altura da arvore
int altura (tree *a) {
    if (a == NULL) {
        return 0;
    }else{
        int esq = altura(a->esq);
        int dir = altura(a->dir);
        if (esq > dir) {
            return esq + 1;
        } else {
            return dir + 1;
        }
    }
}

//funcao para imprimir um nivel
void printlevel (tree *a, int level, int cont) {
    if (a != NULL) {
        if (cont == level) {
            printf("%d ", a->info);
        } else {
            printlevel(a->esq, level, cont + 1);
            printlevel(a->dir, level, cont + 1);
        }
    }
}

//funcao para contar nos em um nivel
int contlevel (tree *a, int level, int cont) {
    if (a == NULL) {
        return 0;
    } else {
        if (cont == level) {
            return 1;
        } else {
            return contlevel(a->esq, level, cont + 1) + contlevel(a->dir, level, cont + 1);
        }
    }
}

//Função auxiliar para verificar se a árvore está ordenada
int isOrderedHelper(tree *a, tree **prev) {
    if (a == NULL) {
        return 1;
    }

    // 1. Verifica a subárvore esquerda
    if (!isOrderedHelper(a->esq, prev)) {
        return 0;
    }

    // 2. O valor atual deve ser estritamente maior que o nó anterior visitado
    if (*prev != NULL && a->info <= (*prev)->info) {
        return 0;
    }
    *prev = a;

    // 3. Verifica a subárvore direita
    return isOrderedHelper(a->dir, prev);
}

// Função chamada pela main para verificar se a árvore está ordenada
int isOrdered(tree *a) {
    tree *prev = NULL;
    return isOrderedHelper(a, &prev);
}

//funcao para verificar se a arvore e completa
int isComplete(tree *a, int index, int totalNodes) {
    if (a == NULL) return 1;

    // Se o índice calculado for maior ou igual ao total de nós, a árvore tem "buracos"
    if (index >= totalNodes) return 0;

    return isComplete(a->esq, 2 * index + 1, totalNodes) &&
           isComplete(a->dir, 2 * index + 2, totalNodes);
}

int getLevel(tree *a, int x, int level) {
    if (a == NULL) {
        return -1;
    }

    if (a->info == x) {
        return level;
    }

    // Busca primeiro na subárvore esquerda
    int downlevel = getLevel(a->esq, x, level + 1);
    if (downlevel != -1) {
        return downlevel;
    }

    // Se não encontrou na esquerda, busca na subárvore direita
    return getLevel(a->dir, x, level + 1);
}

int main() {
    tree *a = NULL;
    FILE *arq;
    int x, menu = 0;
    char name[100];

    printf("\nPrograma de manipulacao de arvore binaria\n");
    printf("=========================================\n");

    //menu de opcoes
    while(menu != 9) {
        printf("\nEscolha uma opcao:\n");
        printf("1. Ler uma arvore\n");
        printf("2. Imprimir a arvore\n");
        printf("3. Verificar se um elemento existe na arvore\n");
        printf("4. Contar o numero de elementos da arvore\n");
        printf("5. Imprimir nos folha da arvore\n");
        printf("6. Verificar se a arvore esta ordenada\n");
        printf("7. Verificar se a arvore e completa\n");
        printf("8. Imprimir o nivel a qual o no x pertence\n");
        printf("9. Sair\n");
        printf("\nOpcao: ");

        scanf("%d", &menu);

        switch(menu) {
            case 1:
                //liberar a memoria alocada para a arvore anterior, se existir
                if(a != NULL) {
                    freetree(a);
                    a = NULL;
                }

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
                    break;
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
                    printf("4. Largura\n");
                    printf("\nOpcao: ");
                    scanf("%d", &x);
                    if (x >= 1 && x <= 4) {
                        switch (x) {
                            case 1: printf("\n Pre-ordem: "); break;
                            case 2: printf("\n Em-ordem: "); break;
                            case 3: printf("\n Pos-ordem: "); break;
                            case 4: printf("\n Largura: "); break;
                        }
                        if (x >= 1 && x <= 3) print(a, x);
                        else print_largura(a);

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
                printf("\n");
                if (isOrdered(a)) printf("A arvore esta ordenada.\n");
                else printf("A arvore nao esta ordenada.\n");
                break;
            case 7:
                printf("\n");
                if (a == NULL) {
                    printf("\nA arvore esta vazia (completa).\n");
                } else if (isComplete(a, 0, count(a))) {
                    printf("\nA arvore e completa.\n");
                } else {
                    printf("\nA arvore nao e completa.\n");
                }
                break;
            case 8: {
                printf("\nDigite o elemento para saber o nivel: ");
                scanf("%d", &x);
                int lvl = getLevel(a, x, 0);
                if (lvl != -1) {
                    printf("\nO elemento %d pertence ao nivel: %d\n", x, lvl);
                } else {
                    printf("\nO elemento %d nao foi encontrado na arvore.\n", x);
                }
                break;
            }
            case 9:
                printf("Saindo...\n");
                freetree(a);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
}