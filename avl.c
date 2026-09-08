#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura do no da arvore avl
typedef struct avl {
    int info;
    int FB;
    struct avl *esq;
    struct avl *dir;
} avl;

//--estrutura e operacoes da fila para a impressao em largura
typedef struct QueueNode {
    avl *node;
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

void enqueue(Queue *q, avl *node) {
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

avl* dequeue(Queue *q) {
    if (isEmpty(q)) return NULL;

    QueueNode *temp = q->front;
    avl *node = temp->node;

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
//--fim da estrutura e operacoes da fila

//--funcoes auxiliares gerais

//funcao para corrigir o nome do arquivo
void arqname(char *a) {
    if (strstr(a, ".txt") == NULL) {
        strcat(a, ".txt");
    }
}

//funcao para o calculo da altura da arvore
int altura(avl *a) {
    if (a == NULL) {
        return 0;
    } else {
        int esq = altura(a->esq);
        int dir = altura(a->dir);
        if (esq > dir) {
            return esq + 1;
        } else {
            return dir + 1;
        }
    }
}

//funcao para liberar a memoria alocada para a arvore
void freetree(avl *a) {
    if (a != NULL) {
        freetree(a->esq);
        freetree(a->dir);
        free(a);
    }
}

//--funcoes de visualizacao e percurso

//funcao para imprimir a arvore em largura usando a fila
void print_largura(avl *a) {
    if (a == NULL) return;

    Queue *q = createQueue();
    enqueue(q, a);

    while (!isEmpty(q)) {
        avl *curr = dequeue(q);
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
void print(avl *a, int type) {
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

//funcao para obter o nivel em que o no x se encontra
int getLevel(avl *a, int x, int level) {
    if (a == NULL) {
        return -1;
    }

    if (a->info == x) {
        return level;
    }

    int downlevel = getLevel(a->esq, x, level + 1);
    if (downlevel != -1) {
        return downlevel;
    }

    return getLevel(a->dir, x, level + 1);
}

//funcao para imprimir as folhas com valor menor que x
void avl_printfolha(avl *a, int value) {
    if (a == NULL) return;

    if (a->esq == NULL && a->dir == NULL) {
        if (a->info < value) {
            printf("%d ", a->info);
        }
        return;
    }

    avl_printfolha(a->esq, value);

    if (a->info < value) {
        avl_printfolha(a->dir, value);
    }
}

//funcao para verificar se um elemento existe na avl de forma eficiente O(log n)
int avl_existe(avl *a, int x) {
    if (a == NULL) return 0;
    else if (x == a->info) return 1;
    else if (x < a->info) return avl_existe(a->esq, x);
    else return avl_existe(a->dir, x);
}

//funcao para ajustar recursivamente os FBs de uma arvore lida de arquivo
void avl_ajustarFB(avl *a) {
    if (a != NULL) {
        int he = altura(a->esq);
        int hd = altura(a->dir);
        a->FB = hd - he;

        avl_ajustarFB(a->esq);
        avl_ajustarFB(a->dir);
    }
}

//funcao para ler a arvore de um arquivo no padrao (info esq dir)
avl *readfile(FILE *arq) {
    char c;
    int num;

    fscanf(arq, " %c", &c);
    fscanf(arq, "%d", &num);

    if (num == -1) {
        fscanf(arq, " %c", &c);
        return NULL;
    } else {
        avl *a = (avl*) malloc(sizeof(avl));
        a->info = num;
        a->FB = 0;
        a->esq = readfile(arq);
        a->dir = readfile(arq);
        fscanf(arq, " %c", &c);
        return a;
    }
}

//funcao de leitura da AVL com ajuste inicial de FBs
avl *LerAVL(FILE *arq) {
    avl *a = readfile(arq);
    avl_ajustarFB(a);
    return a;
}

//---algoritmos otimizados de rotacao

//declaracoes previas das rotacoes duplas e simples
avl *RotacaoEsqSimples(avl *r);
avl *RotacaoEsqDupla(avl *r);
avl *RotacaoDirSimples(avl *r);
avl *RotacaoDirDupla(avl *r);

//rotacao a esquerda geral
avl *RotacaoEsq(avl *r) {
    if (r->dir->FB == -1) r = RotacaoEsqDupla(r);
    else r = RotacaoEsqSimples(r);
    return r;
}

//rotacao a esquerda simples
avl *RotacaoEsqSimples(avl *r) {
    //Os nos a e b serao aqueles com FB modificados.
    avl *a = r;
    avl *b = a->dir;

    a->dir = b->esq;
    b->esq = a;

    if (b->FB == 1) {
        a->FB = 0;
        b->FB = 0;
    } else {
        a->FB = 1;
        b->FB = -1;
    }

    r = b;
    return r;
}

//rotacao a esquerda dupla
avl *RotacaoEsqDupla(avl *r) {
    //Os nos a, b, c serao aqueles com FBs modificados
    avl *a = r;
    avl *c = a->dir;
    avl *b = c->esq; //O no b sera a nova raiz
    c->esq = b->dir;
    a->dir = b->esq;
    b->esq = a;
    b->dir = c;

    //Setando os valores dos FBs de acordo com o FB da nova raiz
    switch (b->FB) {
        case -1:
            a->FB = 0;
            c->FB = 1;
            break;
        case 0:
            a->FB = 0;
            c->FB = 0;
            break;
        case 1:
            a->FB = -1;
            c->FB = 0;
            break;
    }

    b->FB = 0;
    r = b;
    return r;
}

//rotacao a direita geral
avl *RotacaoDir(avl *r) {
    if (r->esq->FB == 1) r = RotacaoDirDupla(r);
    else r = RotacaoDirSimples(r);
    return r;
}

//rotacao a direita simples
avl *RotacaoDirSimples(avl *r) {
    //Os nos a e b serao aqueles com FB modificados.
    avl *a = r->esq;
    avl *b = r;

    b->esq = a->dir;
    a->dir = b;

    if (a->FB == -1) {
        a->FB = 0;
        b->FB = 0;
    } else {
        a->FB = 1;
        b->FB = -1;
    }

    r = a;
    return r;
}

//rotacao a direita dupla
avl *RotacaoDirDupla(avl *r) {
    //Os nos a, b, c serao aqueles com FBs modificados
    avl *c = r;
    avl *a = c->esq;
    avl *b = a->dir;

    c->esq = b->dir;
    a->dir = b->esq;
    b->esq = a;
    b->dir = c;

    //Setando os valores dos FBs de acordo com o FB da nova raiz
    switch (b->FB) {
        case -1:
            a->FB = 0;
            c->FB = 1;
            break;
        case 0:
            a->FB = 0;
            c->FB = 0;
            break;
        case 1:
            a->FB = -1;
            c->FB = 0;
            break;
    }

    b->FB = 0;
    r = b;
    return r;
}

//--operacoes de insercao e remocao otimizadas

//funcao para inserir um elemento na arvore AVL
avl *Inserir(avl *r, int x, int *hMudou) {
    if (r == NULL) {
        r = (avl*) malloc(sizeof(avl));
        r->info = x;
        r->esq = NULL;
        r->dir = NULL;
        r->FB = 0;
        *hMudou = 1;
    } else {
        if (x <= r->info) {
            r->esq = Inserir(r->esq, x, hMudou);
            if (*hMudou == 1) {
                switch (r->FB) {
                    case -1:
                        r = RotacaoDir(r);
                        *hMudou = 0;
                        break;
                    case 0:
                        r->FB = -1;
                        *hMudou = 1;
                        break;
                    case 1:
                        r->FB = 0;
                        *hMudou = 0;
                        break;
                }
            }
        } else {
            r->dir = Inserir(r->dir, x, hMudou);
            if (*hMudou == 1) {
                switch (r->FB) {
                    case -1:
                        r->FB = 0;
                        *hMudou = 0;
                        break;
                    case 0:
                        r->FB = 1;
                        *hMudou = 1;
                        break;
                    case 1:
                        r = RotacaoEsq(r);
                        *hMudou = 0;
                        break;
                }
            }
        }
    }
    return r;
}

//funcao para remover um elemento da arvore AVL
avl *Remover(avl *r, int x, int *hMudou) {
    if (r != NULL) {
        if (r->info == x) {
            //caso 1: no folha
            if (r->esq == NULL && r->dir == NULL) {
                free(r);
                *hMudou = 1;
                return NULL;
            }
            //caso 2: apenas um filho
            else if (r->esq == NULL || r->dir == NULL) {
                avl *aux;
                if (r->esq == NULL) aux = r->dir;
                else aux = r->esq;

                free(r);
                *hMudou = 1;
                return aux;
            }
            //caso 3: dois filhos (maior da subarvore esquerda)
            else {
                avl *maiorEsq = r->esq;
                while (maiorEsq->dir != NULL) {
                    maiorEsq = maiorEsq->dir;
                }
                r->info = maiorEsq->info;
                r->esq = Remover(r->esq, r->info, hMudou);
                if (*hMudou == 1) {
                    switch (r->FB) {
                        case -1:
                            r->FB = 0;
                            *hMudou = 1;
                            break;
                        case 0:
                            r->FB = 1;
                            *hMudou = 0;
                            break;
                        case 1: {
                            int aux = r->dir->FB;
                            r = RotacaoEsq(r);
                            if (aux == 0) *hMudou = 0;
                            else *hMudou = 1;
                            break;
                        }
                    }
                }
            }
        } else if (x < r->info) {
            r->esq = Remover(r->esq, x, hMudou);
            if (*hMudou == 1) {
                switch (r->FB) {
                    case -1:
                        r->FB = 0;
                        *hMudou = 1;
                        break;
                    case 0:
                        r->FB = 1;
                        *hMudou = 0;
                        break;
                    case 1: {
                        int aux = r->dir->FB;
                        r = RotacaoEsq(r);
                        if (aux == 0) *hMudou = 0;
                        else *hMudou = 1;
                        break;
                    }
                }
            }
        } else {
            r->dir = Remover(r->dir, x, hMudou);
            if (*hMudou == 1) {
                switch (r->FB) {
                    case 1:
                        r->FB = 0;
                        *hMudou = 1;
                        break;
                    case 0:
                        r->FB = -1;
                        *hMudou = 0;
                        break;
                    case -1: {
                        int aux = r->esq->FB;
                        r = RotacaoDir(r);
                        if (aux == 0) *hMudou = 0;
                        else *hMudou = 1;
                        break;
                    }
                }
            }
        }
    }
    return r;
}

//---fim dos algoritmos otimizados

int main() {
    avl *a = NULL;
    FILE *arq;
    int x, menu = 0;
    int hMudou = 0;
    char name[100];

    printf("\nPrograma de manipulacao de arvore AVL\n");
    printf("=====================================\n");

    //menu de opcoes conforme especificacao
    while (menu != 8) {
        printf("\nEscolha uma opcao:\n");
        printf("1. Ler uma arvore de um arquivo fornecido pelo usuario\n");
        printf("2. Imprimir a arvore (opcoes: pre-ordem; em-ordem; pos-ordem; e em largura)\n");
        printf("3. Verificar se um elemento x existe na arvore\n");
        printf("4. Imprimir o nivel de um no x\n");
        printf("5. Imprimir as folhas menores que um valor x\n");
        printf("6. Inserir um no x na arvore\n");
        printf("7. Remover um no x da arvore\n");
        printf("8. Sair\n");
        printf("\nOpcao: ");

        scanf("%d", &menu);

        switch (menu) {
            case 1:
                //liberar a memoria alocada para a arvore anterior, se existir
                if (a != NULL) {
                    freetree(a);
                    a = NULL;
                }

                //obter o nome do arquivo
                getchar();
                printf("\nDigite o nome do arquivo .txt: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("\nLendo arquivo...\n");

                //adiciona a extensao .txt ao nome do arquivo, se necessario
                arqname(name);

                //abrir o arquivo
                arq = fopen(name, "r");
                if (arq == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }
                a = LerAVL(arq);
                fclose(arq);
                printf("Arquivo lido com sucesso!\n");
                break;

            case 2:
                if (a == NULL) {
                    printf("\nArvore vazia!\n");
                    break;
                }
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
                            case 1: printf("\nPre-ordem: "); break;
                            case 2: printf("\nEm-ordem: "); break;
                            case 3: printf("\nPos-ordem: "); break;
                            case 4: printf("\nLargura: "); break;
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
                if (avl_existe(a, x)) {
                    printf("\nO elemento %d existe na arvore.\n", x);
                } else {
                    printf("\nO elemento %d nao existe na arvore.\n", x);
                }
                break;

            case 4: {
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

            case 5:
                printf("\nFolhas menores que: ");
                scanf("%d", &x);
                printf("\nFolhas: ");
                avl_printfolha(a, x);
                printf("\n");
                break;

            case 6:
                printf("\nValor a ser inserido: ");
                scanf("%d", &x);
                hMudou = 0;
                a = Inserir(a, x, &hMudou);
                printf("Elemento %d inserido com sucesso!\n\n", x);
                break;

            case 7:
                printf("\nValor a ser removido: ");
                scanf("%d", &x);
                if (avl_existe(a, x) == 1) {
                    hMudou = 0;
                    a = Remover(a, x, &hMudou);
                    printf("Elemento %d removido com sucesso!\n\n", x);
                } else {
                    printf("Este elemento nao esta na arvore.\n\n");
                }
                break;

            case 8:
                printf("Saindo...\n");
                freetree(a);
                a = NULL;
                break;

            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}