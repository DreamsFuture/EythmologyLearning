/*
Este codigo é livre para modificar e distribuir.
Por favor, mantenha meus creditos.
Desenvolvido por: Uisleandro Costa dos Santos 
Email: uisleandro@gmail.com
*/

#include "includes.h"

#define MARCA_FINAL_DO_CABECALHO 0xD
#define MARCA_REGISTRO_EXCLUIDO 0x2A
#define MARCA_FINAL_DO_ARQUIVO 0x1A

typedef struct dbaseHeader
{       
       unsigned char versao; // h 1
       char ano; // d 1
       char mes; // d 1
       char dia; // d 1
       
       int numDeRegistros; // d 4
       
       short headerSize; // d 2
       short dataRecordSize; // d 2
       
       short reservado1; // 2b
       
       char isTransacaoIncompleta; // h 1
       char isCriptografado; // h 1
       
       char reservado2[12]; // 12b
       
       char MDX; // flag que indica se existe um arquivo MDX 0x1 ou 0x0
       char LANG; // X
       
       short reservado3; // 2b         
} dbaseHeader;


// descreve os campos do arquivo DBF
// 32b
typedef struct dbaseFieldDescriptor
{
        char nome[11]; // 11
        char tipoDeDados; // 1 b ascii
        int endereco;  // 4 b (int)
        char tamanho; // 1 b
        char casasDecimais; // 1 b
        char reservados[10];
        struct dbaseFieldDescriptor * proximo;

/*
        short reservado1; // 2 b
        char workArea; // 1 b
        short reservado2; // 2 b
        char flagForSetFields; // 1 b
        char reservado3[3]; // 7 b (erro: mudei de 7 para 3 bytes)
        char indexFieldFlag; 
*/
            
} dbaseFieldDescriptor;


char * removeEspacos(char * linha);

char * dbf_getChaves( dbaseFieldDescriptor * primeiroDescritor, char * linha, int tamanhodaChave, int argc, char ** campos);
char * dbf_getColumn( dbaseFieldDescriptor * primeiroDescritor, char * linha, char * campo);
void dbf_setColumn( dbaseFieldDescriptor * primeiroDescritor, char * linha, char * campo, char * valor);

dbaseFieldDescriptor * dbf_getDescritoresDeCampo(FILE * dbf);

void randmizeString(char * str, int tamanho);


//void atribuirString(char * a, char * b);
