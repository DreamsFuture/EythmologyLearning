/*
Este codigo é livre para modificar e distribuir.
Por favor, mantenha meus creditos.
Desenvolvido por: Uisleandro Costa dos Santos 
Email: uisleandro@gmail.com
*/

#include "dbaseinfo.h"

// usando somente os includes de dbaseinfo.h


// A >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> B
#define A_MAIOR_QUE_B(A,B) (strcmp((char *)A,(char *)B) == 1 )  
// A <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< B
#define A_MENOR_QUE_B(A,B) (strcmp((char *)A,(char *)B) == -1)
// A ====================================== B
#define A_IGUAL_A_B(A,B) (strcmp((char *)A,(char *)B) == 0)

#define ATRIBUIR(A,B) strcpy((char *)A, (char *)B)
//#define ATRIBUIR(A,B) atribuirString((char *)A,(char *)B)

#define DECLARAR_CHAVE(VAR) char VAR[20]
#define MOSTRAR(A) printf("%s,", A)

// para a apresentacao
/*
#define DIM 4
#define MINIMO 2
#define CHARS 20
*/

// Configuração do nó da arvore para usar um setor do disco
/*
 um setor ocupa 512 bytes no windows xp,
 para verificar isso digite: "fsutil fsinfo ntfsinfo c:" no prompt de comando
 do windows.
*/
#define DIM 20
#define MINIMO 10
#define CHARS 20 // ver definição DECLARAR_CHAVE acima;
#define LIXO 18

// Configuração do nó da arvore para usar um cluster do disco
// um cluster ocupa 4096 bytes
/*
#define DIM 170
#define MINIMO 85
#define CHARS 20 // ver definição DECLARAR_CHAVE acima;
#define LIXO 4
*/


#define TRUE 1
#define FALSE 0
#define BOOL int


/* se a chave for char
#define A_MAIOR_QUE_B(A,B) (A > B)
#define A_MENOR_QUE_B(A,B) (A < B)
#define A_IGUAL_A_B(A,B) (A == B)

#define ATRIBUIR(A,B) A = B

#define DECLARAR_CHAVE(VAR) char VAR
#define MOSTRAR(A) printf("%c,", A)
//*/


enum Tipo {
    DELETADO,
    INTERNO,
    FOLHA,
    RAIZFOLHA,
    RAIZ
    };

// aqui é a pagina da arvore
typedef struct no
{
       int tipo; // identifica se é folha ou se é raiz, se é (folha e raiz) ou se é um nó intermediário
       int tamanho;
       char chaves[DIM][CHARS];
       int ponteiros[DIM+1];
       char lixo[LIXO];
} no;


/*Observe que o tamanho do no (pagina) inteiro tem que ser igual ao tamanho de um setor do disco*/

typedef struct no2
{
       char chave[CHARS];
       int esquerda;
       int direita;
} no2;

/* Arvore B IO*/
/* unsigned int suporta 4 bilhões de valores diferentes */

typedef struct arvoreInfo
{
    unsigned int posicaoDaRaiz;
    unsigned int posicaoDaPrimeiraFolha;
    unsigned int UltimaPosicao;
    no * raiz;
    
    unsigned int cmalloc; // contador-malloc
    unsigned int cfree; // contador-free
    unsigned int registros; // contador de registros
    int cerro; // contador de erros cm - cf;
    unsigned int altura;

} arvoreInfo;


/********************************************************************************************************/
char * tToString(enum Tipo t);
void writeValues(no * n);
/*********************************************************************************************************/


/********************************************************************************************************
********************************************************************************************************/


/*Apenas operações de disco para a arvore b/b+*/

int salvaNoDisco(FILE * arquivo, int posicao, no * pagina); // atualiza a ultima posição inserida.

// int salvaNoFimDoArquivo(FILE * arquivo, no * pagina); // salva um nó na ultima posicao não usada.
int salvaNoFimDoArquivo(FILE * arquivo, arvoreInfo * info, no * pagina);

no * buscaNoDisco(FILE * arquivo, int posicao);

//no * carregarArvoreInfo(FILE * arquivo); // serve na inserção e retirada de nos
void carregarArvoreInfo(FILE * arquivo, arvoreInfo ** info);

//void salvarCabecalho(FILE * arquivo);
void salvarCabecalho(FILE * arquivo, arvoreInfo * info);

//void citarNovaRaiz(int posicao, no * raiz); // salva a raiz e atualiza a raiz do cache.
void citarNovaRaiz(arvoreInfo * info, int posicao, no * raiz);


//no * arvoreCriar(FILE * arquivo, const char * chave, int esquerda);
void arvoreCriar(FILE * arquivo, arvoreInfo ** info, const char * chave, int esquerda);

//
void crescerArvore(FILE * arquivo, arvoreInfo * info, no2 * resto);

//no2 * insereEDuplicaFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pEsquerda);
//no2 * insereEDuplicaNaoFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pDireita);
no2 * insereEDuplicaFolha(FILE * arquivo, arvoreInfo * info, no * este, int posicaoDeste, const char * chave, int pEsquerda);
no2 * insereEDuplicaNaoFolha(FILE * arquivo, arvoreInfo * info, no * este, int posicaoDeste, const char * chave, int pDireita);

void insereSemDuplicarFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pEsquerda);
void insereSemDuplicarNaoFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pDireita);


//no2 * inserirNaArvore(FILE * arquivo, int posicaoDeste, no * este, const char * chave, int pEsquerda);
no2 * inserirNaArvore(FILE * arquivo, arvoreInfo * info, int posicaoDeste, no * este, const char * chave, int pEsquerda);



/********************************************************************************************************
PARA ARVORE_REMOVE()
********************************************************************************************************/

void removePosicaoFolha(no * este, int posicaoDaChave);
void removePosicaoNaoFolha(no * este, int posicaoDaChave);
void CompartilhaDireitaPEsquerda_FOLHA(no * meio, int pos, no * esquerda, no * direita);
void CompartilhaDireitaPEsquerda_NAOFOLHA(no * meio, int pos, no * esquerda, no * direita);
void CompartilhaEsquerdaPDireita_FOLHA(no * meio, int pos, no * esquerda, no * direita);
void CompartilhaEsquerdaPDireita_NAOFOLHA(no * meio, int pos, no * esquerda, no * direita);
void juntaFolhas(no * esquerda, no * direita);
void juntaPaginasInternas(no * meio, int pos, no * esquerda, no * direita);

//BOOL retiraDaArvore(FILE * arquivo, int posicaoDaRaiz, no * raiz, char * chave);
//BOOL retiraDaArvore(FILE * arquivo, arvoreInfo * info, int posicaoDaRaiz, no * raiz, const char * chave);
BOOL retiraDaArvore(FILE * arquivo, arvoreInfo * info, int posicaoDaRaiz, no * raiz, const char * chave, int * ponteiroRemovido);


/********************************************************************************************************
********************************************************************************************************/

//void percorrerArvore(char * sbanco, char * sindice);
void percorrerArvore(char * sbanco, char * sindice, char * ssaida);
