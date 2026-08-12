# Árvore Binária em C

Este projeto é uma implementação em linguagem C para manipulação e navegação em uma **Árvore Binária**, desenvolvida a partir de entrada de dados via arquivo de texto. O programa oferece uma interface de menu interativo no terminal para leitura, impressão em diferentes ordens, buscas, contagem de nós e identificação de folhas.

---

## 🚀 Funcionalidades

O sistema conta com um menu interativo com as seguintes opções:

1. **Ler uma árvore de um arquivo:** Carrega a estrutura de dados a partir de um arquivo `.txt` fornecido pelo usuário.
2. **Imprimir a árvore:** Exibe os elementos da árvore nos três percursos clássicos:
   - Pré-ordem (*Pre-order*)
   - Em-ordem (*In-order*)
   - Pós-ordem (*Post-order*)
3. **Verificar existência de elemento ($x$):** Realiza uma busca na árvore para verificar se determinado valor inteiro está presente.
4. **Contar o número de elementos:** Calcula e exibe a quantidade total de nós presentes na árvore.
5. **Imprimir nós folha:** Identifica e exibe todos os nós que não possuem filhos (folhas), além de informar a quantidade total de folhas.
6. **Sair:** Libera a memória alocada dinamicamente para a árvore e encerra o programa.

---

## 🛠️ Estrutura do Código

A árvore é representada pela seguinte estrutura (`struct`):

```c
typedef struct tree {
    int info;
    struct tree *esq;
    struct tree *dir;
} tree;
```

### Funções Principais

* `tree *readfile(FILE *arq)`: Realiza a leitura e reconstrução recursiva da árvore a partir do arquivo.
* `void print(tree *a, int type)`: Percorre e imprime a árvore de acordo com o tipo escolhido (1: Pré-ordem, 2: Em-ordem, 3: Pós-ordem).
* `int exist(tree *a, int x)`: Busca recursiva pelo elemento `x`. Retorna `1` se encontrado e `0` caso contrário.
* `int count(tree *a)`: Retorna o total de nós da árvore.
* `void printfolha(tree *a)`: Imprime os nós folha e contabiliza seu total.
* `void freetree(tree *a)`: Libera a memória alocada dinamicamente para evitar vazamentos de memória (*memory leaks*).

---

## 📋 Formato do Arquivo de Entrada

O arquivo de texto deve conter a representação aninhada/parentagada da árvore. Os nós vazios (ponteiros nulos) são representados pelo valor `-1`.

### Exemplo de formato (`arvore.txt`):
```text
(1(2(-1)(-1))(3(-1)(-1)))
```

**Representação visual da árvore acima:**
```text
     1
    / \
   2   3
```

---