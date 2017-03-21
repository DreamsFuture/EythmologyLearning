/*
Este codigo é livre para modificar e distribuir.
Por favor, mantenha meus creditos.
Desenvolvido por: Uisleandro Costa dos Santos 
Email: uisleandro@gmail.com
*/

#include "arvoreb.h"


//Função 1, criar um indice para o arquivo dbf
//void criarIndice(char * sbanco, char * sindice, char ** chaves)
arvoreInfo * criarIndice(char * sbanco, char * sindice)
{
    // Variaveis do DBF
    dbaseHeader header;
    dbaseFieldDescriptor * descritorDoCampo1;
    char * registro;
    char * chave;
    int i;
    
    // Variaveis da Arvore B
    arvoreInfo * info = NULL;

    FILE * dbf = fopen(sbanco, "rb");

    //Lê o cabecalho
    fread(&header, 1, sizeof(dbaseHeader), dbf);

    //Lê os descritores de campos da tabela
    descritorDoCampo1 = dbf_getDescritoresDeCampo(dbf);

    // O primeiro byte do registro é indicador de deletado
    registro = (char *) malloc( (header.dataRecordSize) * sizeof(char) );
    
    //Lê a primeira chave.
    fread(registro, (header.dataRecordSize), sizeof(char), dbf);

    //Cria a arvore arvore.
    chave = dbf_getColumn(descritorDoCampo1, registro, "NOME");
    
    //printf(" cc %s\n", chave);

    FILE * indice = fopen(sindice,"wb");
         arvoreCriar(indice, &info, chave, 0);
    fclose(indice);

    indice = fopen(sindice,"rb+");

    //header.numDeRegistros
    // consegui cem mil mas não consegui um milhão.
    //1000000
    printf("num. reg %d \n", header.numDeRegistros);
    
    for (i = 1; i < header.numDeRegistros; i++)
    {
        //Lê o registro e insere no indice
        fread(registro, (header.dataRecordSize), sizeof(char), dbf);
        
        if(registro[0] == MARCA_REGISTRO_EXCLUIDO)
        {
            continue;
        }

        chave = dbf_getColumn(descritorDoCampo1, registro, "NOME");

        //printf("{%s}",chave);

        inserirNaArvore(indice, info, info->posicaoDaRaiz, info->raiz, chave, i);

//UIS: DEBUG INFO
info->registros++;

    }
    
    info->cerro = (info->cmalloc) - (info->cfree);
    printf("mallocs em excesso = %d\n", info->cerro);
    printf("altura da arvore = %d\n", info->altura);
    

    fclose(dbf);
    fclose(indice);
    
    return info;
}

//2011
// Remove uma chave da B + Tree e do dbf
void removerRegistro(char * sbanco, char * sindice, char * chave)
{   
    dbaseHeader header;
    char * registro;
    int posicaoRegistro = -1;
        
    FILE * indice = fopen(sindice, "rb+");
    FILE * dbf = fopen(sbanco, "rb+");
    
    arvoreInfo * info = NULL;    
    carregarArvoreInfo(indice, &info);
    
    fread(&header, 1, sizeof(dbaseHeader), dbf);
    
    retiraDaArvore(indice, info, info->posicaoDaRaiz, info->raiz, chave, &posicaoRegistro);
    if(posicaoRegistro == -1) return;
    
    fseek(dbf, (header.headerSize * sizeof(char)) + (posicaoRegistro * (header.dataRecordSize)), SEEK_SET);
    
    registro = (char *) malloc( (header.dataRecordSize) * sizeof(char) );
    fread(registro, (header.dataRecordSize), sizeof(char), dbf);
    
    registro[0] = MARCA_REGISTRO_EXCLUIDO;

    fseek(dbf, (header.headerSize * sizeof(char)) + (posicaoRegistro * (header.dataRecordSize)), SEEK_SET);
    fwrite(registro, (header.dataRecordSize), sizeof(char), dbf);
    
    header.numDeRegistros -= 1;
    
    fseek(dbf, 0, SEEK_SET);
    fwrite(&header, 1, sizeof(dbaseHeader), dbf);

    fclose(dbf);
    fclose(indice);
    
}

int main(){
    
    printf("Criando e inserindo valores na arvore b+..\n");

    criarIndice("banco.dbf", "arvoreb.bin");

    printf("Percorrendo a arvore b+..\n");
    system("pause");
    percorrerArvore("banco.dbf", "arvoreb.bin", "saida1.txt");
    
    
    printf("removendo chave aaa\n");
    system("pause");
    removerRegistro("banco.dbf", "arvoreb.bin", "aaa");


    printf("Percorrendo a arvore b+..\n");
    system("pause");
    percorrerArvore("banco.dbf", "arvoreb.bin", "saida2.txt");

    return EXIT_SUCCESS;

}
