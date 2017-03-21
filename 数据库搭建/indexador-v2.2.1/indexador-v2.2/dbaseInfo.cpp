/*
Este codigo é livre para modificar e distribuir.
Por favor, mantenha meus creditos.
Desenvolvido por: Uisleandro Costa dos Santos 
Email: uisleandro@gmail.com
*/

#include "dbaseinfo.h"

char * removeEspacos(char * linha)
{     
     char * result = (char *) malloc( 20 * sizeof(char));
     int i = 0;
     int j = 0;
     
     while(linha[i] != '\0')
     {
           while(linha[i] == ' ')
           {
                i++;
           }
           //if(linha[i] == '\0') break;
           
           result[j] = linha[i];
           j++;
           i++;
     }
     
     result[j] = '\0';
     
     return result;

}



// recebe uma linha e as chaves desejadas como parametro
// e devolve os valores correspondentes aos valores da chave na linha
char * dbf_getChaves( dbaseFieldDescriptor * primeiroDescritor, char * linha, int tamanhodaChave, int argc, char ** campos)
{
    dbaseFieldDescriptor * descritorDeCampo;
    int i, j = 0, k = 0;
    char tmp;
    char * valores = (char *)malloc( 20 * sizeof(char) );
    valores[0] = '\0';
	
	for(i = 0; i < argc; i++)
	{
    	descritorDeCampo = primeiroDescritor;
        
        while(descritorDeCampo)
        {
                if( strcmp(descritorDeCampo->nome, campos[i]) == 0 )
                {
                     tmp = linha[ descritorDeCampo->endereco + descritorDeCampo->tamanho ];
                     
                     linha[ descritorDeCampo->endereco + descritorDeCampo->tamanho ] = '\0';
                     
                     // remover os espacos da linha..
                     char * limpo = removeEspacos(&linha[ descritorDeCampo->endereco ]);
                     
                     if(j > 0)
                     {
                          //valores[ tamanhodaChave - (tamanhodaChave/j) ] = '\0';
                          valores[ j * 5 ] = '\0';
                     }
                     
                     strcat(valores,  limpo);
                     
                     free(limpo);
                     
                     linha[ descritorDeCampo->endereco + descritorDeCampo->tamanho ] = tmp;
                     
                     j++;
                     
                }
                
                descritorDeCampo = descritorDeCampo->proximo;
        }
		
	}
	
	valores[tamanhodaChave] = '\0';
	
	return valores;
}


/*
    OBS: o valor do campo termina no primeiro espaço
*/
char * dbf_getColumn( dbaseFieldDescriptor * primeiroDescritor, char * linha, char * campo)
{
    char * valor;
    dbaseFieldDescriptor * descritorDeCampo;
    int i, j = 0;
    
    valor = (char *)malloc( 20 * sizeof(char) );
    	
    descritorDeCampo = primeiroDescritor;
    
    while(descritorDeCampo)
    {
            if( strcmp(descritorDeCampo->nome, campo) == 0 )
            {
                 for( i = descritorDeCampo->endereco; i < descritorDeCampo->endereco + descritorDeCampo->tamanho; i++)
                 {
                      if(linha[i] == ' ') break;
                      valor[j] = linha[i];
                      j++;
                      
                 }             
                 break;
            }
            descritorDeCampo = descritorDeCampo->proximo;
    }


    valor[19] = '\0';
    valor[j] = '\0';
    
    return valor;
    
}


void dbf_setColumn( dbaseFieldDescriptor * primeiroDescritor, char * linha, char * campo, char * valor)
{
    
    dbaseFieldDescriptor * descritorDeCampo;
    int i, j = 0;
    	
    descritorDeCampo = primeiroDescritor;

    
    while(descritorDeCampo)
    {
            if( strcmp(descritorDeCampo->nome, campo) == 0 )
            {   
                
                if(descritorDeCampo->tipoDeDados == 'N')
                {
                     while(valor[j] != '\0') j++;
                     j--;
    
                     for( i = descritorDeCampo->endereco + descritorDeCampo->tamanho -1; i >= descritorDeCampo->endereco; i--)
                     {
                          if(j >= 0)
                          {
                                linha[i] = valor[j];
                                j--;
                          }
                          else
                          {
                                linha[i] = ' ';
                          }
                     }
                 }
                 else
                 {
                     for( i = descritorDeCampo->endereco; i < descritorDeCampo->endereco + descritorDeCampo->tamanho; i++)
                     {
                          if(valor[j] != '\0')
                          {
                              linha[i] = valor[j];
                              j++;
                          }
                          else
                          {
                              linha[i] = ' ';
                          }
                     }
                 }
                 break;
            }
            
            descritorDeCampo = descritorDeCampo->proximo;
    }
    
}

/* Lê os descritores de campo do arquivo */
dbaseFieldDescriptor * dbf_getDescritoresDeCampo(FILE * dbf)
{
     
    dbaseFieldDescriptor * descritorDeCampo = NULL;
    dbaseFieldDescriptor * primeiroDescritor = NULL;
     
    unsigned char proximoByte = 0x0;
        
    // vou adicionar isso ha uma lista chamada cabecalho.
    while(proximoByte != MARCA_FINAL_DO_CABECALHO)
    {
        if(descritorDeCampo)
        {
            descritorDeCampo->proximo = (dbaseFieldDescriptor *) malloc(sizeof(dbaseFieldDescriptor));
            descritorDeCampo = descritorDeCampo->proximo;
        }
        else
        {
            descritorDeCampo = (dbaseFieldDescriptor *)malloc(sizeof(dbaseFieldDescriptor));
            primeiroDescritor = descritorDeCampo;
            descritorDeCampo->proximo = NULL;
        }
        
        fread(descritorDeCampo, 1, sizeof(dbaseFieldDescriptor), dbf);
        
        
        fread(&proximoByte, 1, sizeof(char), dbf);
        fseek(dbf, -1 * sizeof(char), SEEK_CUR);
    }
    
    // recuperando a posição
    fseek(dbf, 1 * sizeof(char), SEEK_CUR);
    
    return primeiroDescritor;
     
}


void randmizeString(char * str, int tamanho)
{
     int i;
     
     for(i = 0; i < tamanho;i++)
     {
          if( rand() % 3 == 0)
          {
                str[i] = 'a' + (rand() ) % 25;
          }
          else
          {
                str[i] = 'A' + (rand() ) % 25;
          }
          
     }
     str[i] = '\0';
}


int dbaseinfo_main()
{   
    
    srand( time(NULL) );
         
//    char fieldNome[11];
    
    dbaseHeader header;
    
    
    FILE * dbf = fopen("banco.dbf", "rb+");
    
    fread(&header, 1, sizeof(dbaseHeader), dbf);

    dbaseFieldDescriptor * primeiroDescritor = dbf_getDescritoresDeCampo(dbf);

    
    /* // inicio dos registros. local onde começa o primeiro registro;    
    printf("inicio %d \n", espacamento);
    printf("inicio %d \n", header.headerSize);
    */
    
    char * registro;
    int i;
    
    // O primeiro byte do registro é indicador de deletado
    registro = (char *) malloc( (header.dataRecordSize) * sizeof(char) );
    
    for (i = 0; i < header.numDeRegistros; i++)
    {
        //Lê o registro e insere no indice
        fread(registro, (header.dataRecordSize), sizeof(char), dbf);
        
        if(registro[0] == MARCA_REGISTRO_EXCLUIDO)
        {                              
            continue;
        }
        
        printf("%s\n", registro);
        
        /*
        setCampo( primeiroDescritor, registro, "ID", "x09");
        setCampo( primeiroDescritor, registro, "CIDADE", "1234567890.1234567890.1234567890");
        setCampo( primeiroDescritor, registro, "NOME", "Uisleandro Costa dos Santos");
        setCampo( primeiroDescritor, registro, "BAIRRO", "imbui");
        //*/
        
        /*
        char ** campos = malloc( 2 * sizeof (char *) );
        campos[0] = "CIDADE";
        campos[1] = "NOME";
        campos[2] = "BAIRRO";
        printf("{%s}\n", dbf_getChaves(primeiroDescritor, registro, 20, 2, campos) );        
        //*/
    
    }
    
    
    //fclose(dbf);
    
        system("pause");
    
    
    //dbf = fopen("sdbfteste2.dbf", "rb+");
    
    // para ficar no inicio dos registros
    // fseek(dbf, header.headerSize, SEEK_SET);
    
     fseek(dbf, header.headerSize + (header.numDeRegistros * header.dataRecordSize) , SEEK_SET);
    
    
    int id = 0;
    char * ids = dbf_getColumn(primeiroDescritor, registro, "ID");
    id = atoi(ids);
    char stingRandomico[10];
    
    
    //criando 100 cidades diferentes;
    char cidades[100][10];
    for(i = 0; i < 100; i++)
    {
        randmizeString(cidades[i], 10);
    }
    
    //criando 10 bairros difetentes
    char bairros[10][10];
    for(i = 0; i < 100; i++)
    {
        randmizeString(bairros[i], 10);
    }
    
    
    for(i = 0; i < 1000000; i++)
    {
          id++;
          sprintf(ids, "%d", id);          
          dbf_setColumn( primeiroDescritor, registro, "ID", ids);
          
          randmizeString(stingRandomico, 10);          
          dbf_setColumn( primeiroDescritor, registro, "NOME", stingRandomico);
          
          dbf_setColumn( primeiroDescritor, registro, "CIDADE", cidades[ rand() % 100 ] );
          
          dbf_setColumn( primeiroDescritor, registro, "BAIRRO", bairros[ rand() % 10 ] );
          
          //printf("%s\n", registro);
          
          fwrite(registro, header.dataRecordSize, sizeof(char), dbf);
    }
    
    header.numDeRegistros += 1000000;

    /*//char final = MARCA_FINAL_DO_ARQUIVO;
    
      if( fwrite(&final, 1, sizeof(char), dbf) < 0)
      {
              printf("erro\n");
      }
    */


      rewind(dbf);
      fwrite(&header, 1, sizeof(dbaseHeader), dbf);

    
    fclose(dbf);
       
    
    system("pause");
    return -1;
}

/*
substituia strcpy.
void atribuirString(char * a, char * b)
{
    int i = 0;

    while(b[i] != '\0')
    {
        a[i] = b[i];
        i++;

        if(i == 19) break; // sei eu definir chaves como chaves[n][20]
    }

    a[i] = '\0';
}
*/
