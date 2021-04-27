#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AVL.h"

int main(int argc, char *argv[]) {

    AVL arvore;
	inicializa_AVL( &arvore );

    // log avl para o caso medio
    srand(time(NULL));
    for(int i=1; i<=100; i++){
        insere_AVL( &arvore, rand() % 100 );
        //printf("%d,", i);
        mostra_contador();
    }

    /* log avl para o pior caso
    for(int i=1; i<=100; i++){
        insere_AVL( &arvore, i );
        //printf("%d,", i);
        mostra_contador();
    }*/

return 0;
}
