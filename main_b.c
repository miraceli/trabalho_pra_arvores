#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai; 
} No;

typedef struct arvoreB {
  No* raiz;
  int ordem;
} ArvoreB;

//variável contador global
int contador = 0;

ArvoreB* criaArvore(int);
No* criaNo(ArvoreB*);
void percorreArvore(No*);
int pesquisaBinaria(No*, int);
int localizaChave(ArvoreB*, int);
No* localizaNo(ArvoreB*, int);
void adicionaChaveNo(No*, No*, int);
int transbordo(ArvoreB*,No*);
No* divideNo(ArvoreB*, No*);
void adicionaChaveRecursivo(ArvoreB*, No*, No*, int);
void adicionaChave(ArvoreB*, int);

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);

    return a;
}

No* criaNo(ArvoreB* arvore) {
    contador ++;
    int max = arvore->ordem * 2;
    No* no = malloc(sizeof(No));

    no->pai = NULL;
        
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

void percorreArvore(No* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++){
            percorreArvore(no->filhos[i]); //visita o filho a esquerda
            
            printf("%d ",no->chaves[i]);
        }

        percorreArvore(no->filhos[no->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;	
    contador ++;
    while (inicio <= fim) {	
        
        meio = (inicio + fim) / 2;	        
        if (no->chaves[meio] == chave) {	
		    return meio; //encontrou	
        } else if (no->chaves[meio] > chave) {
            fim	= meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; //não encontrou	
}

int localizaChave(ArvoreB* arvore, int chave) {	
    No *no = arvore->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return 1; //encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return 0; //não encontrou	
}

No* localizaNo(ArvoreB* arvore, int chave) {	
    No *no = arvore->raiz;
    contador ++;
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (no->filhos[i] == NULL)
            return no; //encontrou nó
        else
            no = no->filhos[i];
    }

    return NULL; //não encontrou nenhum nó
}

void adicionaChaveNo(No* no, No* novo, int chave) {

    contador ++;

    int i = pesquisaBinaria(no, chave);

    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;

    no->total++;
}

int transbordo(ArvoreB* arvore, No* no) {
    return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
    contador ++;
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;
    
    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        
        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;    
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
    adicionaChaveNo(no, novo, chave);
    
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem]; 
        No* novo = divideNo(arvore, no);

        if (no->pai == NULL) {
            No* pai = criaNo(arvore);            
            pai->filhos[0] = no;
            adicionaChaveNo(pai, novo, promovido);
            
            no->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        } else
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);

    adicionaChaveRecursivo(arvore, no, NULL, chave);
}

int main() {
    ArvoreB* arvore = criaArvore(1);

    // medio caso
    srand(time(NULL));
    for(int i=1; i<=100; i++){
        adicionaChave( arvore, rand() % 100 );
        printf("\n%d", contador);
    }
    
    /* pior caso
    for(int i=1; i<=100; i++){
        adicionaChave( arvore, i);
        printf("%d\n", contador);
    }*/
    
}
