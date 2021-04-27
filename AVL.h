
typedef struct noAVL{
	int info;
	struct noAVL *esq, *dir;
	int altura;
}NoAVL;

typedef struct{
	NoAVL *raiz;
}AVL;

void inicializa_AVL( AVL *p );
void insere_AVL( AVL *p, int info );
void remove_AVL( AVL *p, int info );
int busca_AVL( AVL a, int chave );
void pre_ordem_AVL( AVL a );
void em_ordem_AVL( AVL a );
void pos_ordem_AVL( AVL a );
void mostra_AVL( AVL a );
int maior_valor( AVL a );
int menor_valor( AVL a );
int altura_AVL( AVL a );
void desaloca_AVL( AVL *p );
void mostra_contador(); //mostra contador
