#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>

int contador = 0;

// Prot�tipos das fun��es privadas
NoAVL *rotacao_dir( NoAVL *pai ); // Fun��o exclusiva de AVL
NoAVL *rotacao_esq( NoAVL *pai ); // Fun��o exclusiva de AVL
NoAVL *aloca_no( int info ); // Modificado: considera campo 'altura'.
NoAVL *insere_avl( NoAVL *n, int info ); // Inclu�do teste para rota��es
NoAVL *remove_avl( NoAVL *n, int info );
NoAVL *remove_no( NoAVL *n );
NoAVL *busca_avl( NoAVL *n, int chave );
void pre_ordem( NoAVL *n );
void em_ordem( NoAVL *n );
void pos_ordem( NoAVL *n );
void pre_ordem_modificado( NoAVL *n, int nivel );
NoAVL *maior( NoAVL *n );
NoAVL *menor( NoAVL *n );
int altura( NoAVL *n ); // Mudou, ficou mais simples em AVL
int max(int x, int y ); // Utilit�ria, usada em AVL
int fator_balanceamento( NoAVL *n ); // Fun��o exclusiva de AVL
void desaloca_pos_ordem( NoAVL *n );


void inicializa_AVL( AVL *p ){
	p->raiz = NULL;
}

void insere_AVL( AVL *p, int info ){
	//  printf("Inserindo %d...\n", info);
	p->raiz = insere_avl( p->raiz, info );
}

NoAVL *insere_avl( NoAVL *n, int info ){
	contador ++;
	// 1. Faz a inser��o normal em uma ABB
	if( n == NULL ) // Chegou numa folha (ou �rvore vazia)
		return aloca_no( info );
			
	if( info < n->info ) // Perccore � esquerda.
		n->esq = insere_avl( n->esq, info );
	else
		if( info > n->info ) // Perccore � direita.
			n->dir = insere_avl( n->dir, info );
		else //  N�o faz nada, pois info j� existe.
			return n; // Retorna o pr�prio n�.

	// 2. Atualiza a altura do n�, ap�s a recurs�o
    n->altura = 1 + max( altura( n->esq ) , altura( n->dir) );
  
	// 3. Calcula fator de balanceamento.
    int fb = fator_balanceamento( n );

	// 4. Verifica o tipo de rota��o

	// Rota��o simples � direita.
	if( fb > 1 && fator_balanceamento( n->esq ) >= 0 )
		return rotacao_dir( n );

	// Rota��o simples � esquerda.
	if( fb < -1 && fator_balanceamento( n->dir ) <= 0 )
		return rotacao_esq( n );

	// Rota��o dupla � direita.
	if( fb > 1 && fator_balanceamento( n->esq ) < 0 ){
		n->esq = rotacao_esq( n->esq );
		return rotacao_dir( n );
	}

	// Rota��o dupla � esquerda.
	if( fb < -1 && fator_balanceamento( n->dir ) > 0 ){
		n->dir = rotacao_dir( n->dir );
		return rotacao_esq( n ); 
	} 

	return n; // Se n�o balancear, retorna o pr�prio n�.
}

void remove_AVL( AVL *p, int info ){
	//  // printf("Removendo %d...\n", info);		
	p->raiz = remove_avl( p->raiz, info );
}

NoAVL *remove_avl( NoAVL *n, int info ){
	// 1. Faz a remo��o normal em uma ABB
	if( n == NULL)
		return n;
	
	if( info < n->info )
		n->esq = remove_avl( n->esq, info );
	else
		if( info > n->info )
			n->dir = remove_avl( n->dir, info );
		else // Achou o n� a ser removido.
			n = remove_no ( n );

	// Se a �rvore tinha somente um n�, ent�o retorna...
	if ( n == NULL )
		return n;

	// 2. Atualiza a altura do n�, ap�s a recurs�o
    n->altura = 1 + max( altura( n->esq ) , altura( n->dir) );
  
	// 3. Calcula fator de balanceamento.
    int fb = fator_balanceamento( n );

	// 4. Verifica o tipo de rota��o

    // Rota��o simples � direita.
    if ( fb > 1 && fator_balanceamento( n->esq ) >= 0 ) 
        return rotacao_dir( n );
  
    // Rota��o dupla � direita.
    if ( fb > 1 && fator_balanceamento( n->esq ) < 0 ){ 
        n->esq = rotacao_esq( n->esq );
        return rotacao_dir( n );
    } 
  
    // Rota��o simples � esquerda.
    if ( fb < -1 && fator_balanceamento( n->dir ) <= 0 ) 
        return rotacao_esq( n );
  
    // Rota��o dupla � esquerda.
    if ( fb < -1 && fator_balanceamento( n->dir ) > 0 ){ 
        n->dir = rotacao_dir( n->dir );
        return rotacao_esq( n );
    } 

	return n; // Se n�o balancear, retorna o pr�prio n�.
}

NoAVL *remove_no( NoAVL *n ){
	NoAVL * aux;
	if( n->esq == NULL ){ // N� com 1 filho � direita ou n� folha
		aux = n->dir;
		free( n );
		return aux;
	}
	if( n->dir == NULL ){ // N� com 1 filho � esquerda
		aux = n->esq;
		free( n );
		return aux;
	}
	// N� com 2 filhos.
	NoAVL *m = maior( n->esq ); // Busca maior � esquerda.
	n->info = m->info;
	n->esq = remove_avl( n->esq, m->info );
	return n;
}

int busca_AVL( AVL a, int chave ){
	return busca_avl( a.raiz, chave ) != NULL;
}

NoAVL *busca_avl( NoAVL *n, int chave ){
	contador ++;
	if( n == NULL )
		return NULL;
	
	if( chave < n->info )
		return busca_avl( n->esq, chave );
	
	if( chave > n->info )
		return busca_avl( n->dir, chave );
	
	return n;
}

void pre_ordem_AVL( AVL a ){
	if( a.raiz == NULL)
		printf("Arvore vazia!\n");
	else{
		//  printf("Percurso na arvore (pre-ordem):\n");
		pre_ordem( a.raiz );
	}
	//  printf("\n");
}

void pre_ordem( NoAVL *n ){
	if( n != NULL ){
		//  printf("%d\n", n->info );
		pre_ordem( n->esq );
		pre_ordem( n->dir );
	}
}

void em_ordem_AVL( AVL a ){
	if( a.raiz == NULL)
		printf("Arvore vazia!\n");
	else{
		//  printf("Percurso na arvore (em-ordem):\n");
		em_ordem( a.raiz );
	}
	//  printf("\n");
}

void em_ordem( NoAVL *n ){
	if( n != NULL ){
		em_ordem( n->esq );
		//  printf("%d\n", n->info );
		em_ordem( n->dir );
	}
}

void pos_ordem_AVL( AVL a ){
	if( a.raiz == NULL)
		printf("Arvore vazia!\n");
	else{
		//  printf("Percurso na arvore (pos-ordem):\n");
		pos_ordem( a.raiz );
	}
	//  printf("\n");
}

void pos_ordem( NoAVL *n ){
	if( n != NULL ){
		pos_ordem( n->esq );
		pos_ordem( n->dir );
		//  printf("%d\n", n->info );
	}
}

void mostra_AVL( AVL a ){
	// printf("\n------------------------------------------------\n");
	// printf("Estrutura da Arvore (altura: %d):\n\n", altura_AVL( a ) );
	if( a.raiz == NULL )
		printf("(null)\n");
	else{
		// printf("%d\n", a.raiz->info );
		pre_ordem_modificado( a.raiz->esq, 1 );
		pre_ordem_modificado( a.raiz->dir, 1 );
	}
	// printf("------------------------------------------------\n\n");
}

void pre_ordem_modificado( NoAVL *n, int nivel ){
	int i;
	for( i = 0 ; i < nivel-1 ; i++ )
		// printf("    ");
	// printf("+---");

	if(n != NULL){
		// printf("%d\n", n->info );
		pre_ordem_modificado( n->esq, nivel+1 );
		pre_ordem_modificado( n->dir, nivel+1 );
	}
	else
		printf("(null)\n");
}

int maior_valor( AVL a ){
	NoAVL *p = maior ( a.raiz );
	if( p == NULL){
		// printf("Arvore vazia! Nao ha maior valor!\n");
		return 0;
	}
	else
		return p->info;
}

NoAVL *maior( NoAVL *n ){
	if( n == NULL)
		return NULL;
	
	if( n->dir == NULL)
		return n;
	else
		return maior( n->dir );
}

int menor_valor( AVL a ){
	NoAVL *p = menor ( a.raiz );
	if( p == NULL){
		// printf("Arvore vazia! Nao ha menor valor!\n");
		return 0;
	}
	else
		return p->info;	
}

NoAVL *menor( NoAVL *n ){
	if( n == NULL)
		return NULL;
	
	if( n->esq == NULL)
		return n;
	else
		return menor( n->esq );
}

int altura_AVL( AVL a ){
	return altura( a.raiz );
}

int altura( NoAVL *n ){
	if( n == NULL )
		return 0;

	return n->altura;
}

int max(int x, int y ){
	if( x > y )
		return x;
	else
		return y;
}

int fator_balanceamento( NoAVL *n ){
	if( n == NULL )
		return 0;

	return altura( n->esq ) - altura( n->dir );
}

void desaloca_AVL( AVL *p ){
	// printf("Desalocando arvore...\n");
	desaloca_pos_ordem( p->raiz );
	p->raiz = NULL;
}

void desaloca_pos_ordem( NoAVL *n ){
	if( n != NULL){
		desaloca_pos_ordem( n->esq );
		desaloca_pos_ordem( n->dir );
		free( n );
	}
}

NoAVL *aloca_no( int info ){
	contador ++;
	NoAVL *p = malloc( sizeof(NoAVL) );
	if( p == NULL)
		return NULL;

	p->info = info;
	p->esq = p->dir = NULL;
	p->altura = 1;  // Sempre criado como um n� folha.
	return p;
}

NoAVL *rotacao_dir( NoAVL *pai ){
	// printf("Rotacao a direita no %d...\n", pai->info);
	NoAVL *filho = pai->esq;
	NoAVL *neto = filho->dir;

	// Faz a rota��o
	filho->dir = pai;
	pai->esq = neto;

	// Atualiza as alturas
	pai->altura = max( altura( pai->esq ), altura( pai->dir ) ) + 1;
	filho->altura = max( altura( filho->esq ), altura( filho->dir ) ) + 1;

	//atualiza contador
	contador ++;
	// Retorna novo pai
	return filho;
}

NoAVL *rotacao_esq( NoAVL *pai ){
	// printf("Rotacao a esquerda no %d...\n", pai->info);
	NoAVL *filho = pai->dir;
	NoAVL *neto = filho->esq;

	// Faz a rota��o
	filho->esq = pai;
	pai->dir = neto;
  
	// Atualiza as alturas
	pai->altura = max( altura( pai->esq ), altura( pai->dir ) ) + 1;
	filho->altura = max( altura( filho->esq ), altura( filho->dir ) ) + 1;

	//atualiza contador
	contador ++;
	// Retorna novo pai
	return filho;
}

// mostrar contador
void mostra_contador(){
	printf("%d\n", contador);
}