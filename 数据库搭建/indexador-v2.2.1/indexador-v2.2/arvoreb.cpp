/*
Este codigo é livre para modificar e distribuir.
Por favor, mantenha meus creditos.
Desenvolvido por: Uisleandro Costa dos Santos 
Email: uisleandro@gmail.com
*/

#include "arvoreb.h"

char * tToString(enum Tipo t)
{
    switch(t)
    {
    case DELETADO: return "DELETADO";
    case INTERNO: return "INTERNO";
    case FOLHA: return "FOLHA";
    case RAIZFOLHA: return "RAIZFOLHA";
    case RAIZ: return "RAIZ";
    default: return "";
    }
}

void writeValues(no * n)
{
    if(!n) return;
    
    int i;

    printf("tipo: %s\n", tToString((enum Tipo)n->tipo));
    printf("tamanho: %d\n", n->tamanho);
    
    if(n->tamanho == 0) return;
    
    for(i=0; i < n->tamanho - 1; i++)
    {
        printf("%d -", n->ponteiros[i]);
        MOSTRAR(n->chaves[i]);
        printf("\n");
    }
    
        printf("%d -", n->ponteiros[i]);
        MOSTRAR(n->chaves[i]);
        printf("\n");

    /*
    for(i=0; i <= n->tamanho - 1; i++)
    {
         printf("%d,", n->ponteiros[i]);
    }
    */
    
    //printf("%d\n\n", n->ponteiros[i]);
    
}


/******************************************************************************************************
FUNÇÕES DE IO DA ARVORE B
******************************************************************************************************/


int salvaNoDisco(FILE * arquivo, int posicao, no * pagina)
{
    fseek(arquivo, posicao * sizeof(no), SEEK_SET);;
    fwrite(pagina, sizeof(no), 1, arquivo);
    return posicao;
}

/* não salva ARVORE_INFO no disco*/
int salvaNoFimDoArquivo(FILE * arquivo, arvoreInfo * info, no * pagina)
{
    if(info == NULL)
    {
        printf("erro: salvaNoFimDoArquivo() -> use arvoreCriar() ou ou carregarArvoreInfo() antes.");
        getchar();
        exit(1);
    }
    
    info->UltimaPosicao++;
    fseek(arquivo, info->UltimaPosicao * sizeof(no), SEEK_SET);
    fwrite(pagina, sizeof(no), 1, arquivo);
    return info->UltimaPosicao;
}

no * buscaNoDisco(FILE * arquivo, int posicao)
{
    fseek(arquivo, posicao * sizeof(no), SEEK_SET);
    no * este = (no *)malloc(sizeof(no));
    if(!este)
    {
        printf("Erro: buscaNoDisco -> Memoria insuficiente");

        (*(int *)0) = 0;

        getchar();
        exit(1);
    }
    
    fread(este, sizeof(no), 1, arquivo);
    return este;   
}

/*
    Carrega a estrutura ARVORE_INFO para a Memoria se não existe;
    retorna a raiz da arvore a partir do arquivo ou da Memoria.
*/
void carregarArvoreInfo(FILE * arquivo, arvoreInfo ** info)
{
    *info = (arvoreInfo *)malloc( sizeof(struct arvoreInfo) );

    if(!(*info))
    {
        printf("Erro: arvoreAbrir() -> Memoria insuficiente");
        getchar();
        exit(1);
    }

    //volta para o inicio do arquivo
    rewind(arquivo);

    //lê arvoreinfo
    fread((*info), sizeof(struct arvoreInfo), 1, arquivo);

    no * raiz = (no *)malloc(sizeof(no));
    
    if(!raiz)
    {
        printf("Erro: arvoreAbrir() -> Memoria insuficiente");
        getchar();
        exit(1);
    }
    
//UIS: INFORMACAO DE DEBUG
(*info)->cmalloc += 1;

    //vai para a posicao da raiz
    fseek(arquivo, (*info)->posicaoDaRaiz * sizeof(no), SEEK_SET);

    //lê a raiz;
    fread(raiz, sizeof(no), 1, arquivo);
    
    (*info)->raiz = raiz;
    
}

/*
    salvar as seguntes informações
    1 - indice da raiz;
    2 - indice da ultima folha;
*/
void salvarCabecalho(FILE * arquivo, arvoreInfo * info)
{
    if(!info)
    {
        printf("Erro: salvarCabecalho() -> use arvoreCriar() ou carregarArvoreInfo() antes.");
        getchar();
        exit(1);
    }
    
    fseek(arquivo, 0, SEEK_SET);
    fwrite(info, sizeof(struct arvoreInfo), 1, arquivo);
}


// TODO: talvez remover.
void citarNovaRaiz(arvoreInfo * info, int posicao, no * raiz)
{
    info->posicaoDaRaiz = posicao;
    info->raiz = raiz;
}



/*
    salvar uma estrutura contendo as seguintes informações;
    1 - indice da raiz;
    2 - indice da ultima folha;
    
    Cria a arvore e adiciona a primeira chave e ponteiro.

    insere em uma RAIZ FOLHA, contendo um único ponteiro
    por isso o ponteiro (int direita) está nulo.

*/
void arvoreCriar(FILE * arquivo, arvoreInfo ** info, const char * chave, int esquerda)
{
    if(!(*info))
    {
        *info = (arvoreInfo *)malloc(sizeof(struct arvoreInfo));
    }

    if(!(*info))
    {
        printf("erro: arvoreCriar() -> Memoria insuficiente \n");
        system("pause");
        exit(1);
    }

    
    (*info)->posicaoDaRaiz = 1;
    (*info)->posicaoDaPrimeiraFolha = 1;
    (*info)->UltimaPosicao = 1;
    (*info)->raiz = NULL;
    
//debug.
(*info)->cmalloc = 1;
(*info)->cfree = 0;
(*info)->registros = 1;
(*info)->cerro = 1;
(*info)->altura = 0;
    
    no * novaRaiz = (no *)malloc(sizeof(no));

    if(!novaRaiz)
    {
        printf("erro: arvoreCriar() -> Memoria insuficiente \n");
        system("pause");
        exit(1);
    }

    novaRaiz->tipo = RAIZFOLHA;
    novaRaiz->tamanho = 1;
    ATRIBUIR(novaRaiz->chaves[0], chave);
    novaRaiz->ponteiros[0] = esquerda;
    novaRaiz->ponteiros[1] = 0;
    
    fwrite((*info), sizeof(struct arvoreInfo), 1, arquivo);
    
    fseek(arquivo, 1 * sizeof(no), SEEK_SET);
    fwrite(novaRaiz, sizeof(no), 1, arquivo);
    

    (*info)->raiz = novaRaiz;

}




/*************************************************************************************
FUNÇÕES PARA ARVORE_INSERE
**************************************************************************************/

// duplica e insere na pagina do tipo FOLHA ou RAIZFOLHA;
no2 * insereEDuplicaFolha(FILE * arquivo, arvoreInfo * info, no * este, int posicaoDeste, const char * chave, int pEsquerda)
{
    no * esquerda = NULL, * direita = NULL;
    DECLARAR_CHAVE(meio);
    int i, j;
    BOOL inseriu = FALSE;
    int metade = este->tamanho / 2;
    
    esquerda = este;

    int ultimoPonteiro = esquerda->ponteiros[esquerda->tamanho];
    
    direita = (no *)malloc(sizeof(no));
    
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
    
    if(!direita)
    {
        printf("Erro: insereEDuplicaFolha() -> Memoria insuficiente!");
        system("pause");
        exit(1);
    }
    
    // identifica qual a chave do meio. que será ignorada na divisão do nó
    if( A_MENOR_QUE_B(chave, este->chaves[metade-1]) )
    {
        ATRIBUIR(meio, este->chaves[metade-1]);
                
        esquerda = (no *)malloc(sizeof(no));
        
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
        
        
        if(!esquerda)
        {
            printf("Erro: insereEDuplicaFolha() -> Memoria insuficiente!");
            system("pause");
            exit(1);
        }
        
        j = 0;
        /* aqui já exclui o meio.. pois meio fica na posicao metade-1*/
        for(i = 0; i < metade-1; i++)
        {
            //insere a chave e o ponteiro proximo na posição.
            if( (!inseriu) && A_MAIOR_QUE_B(este->chaves[i], chave) )
            {
                ATRIBUIR(esquerda->chaves[j], chave);
                esquerda->ponteiros[j] = pEsquerda;
                j++;
                inseriu = TRUE;
            }
            
            ATRIBUIR(esquerda->chaves[j], este->chaves[i]);
            esquerda->ponteiros[j] = este->ponteiros[i];
            j++;
        }
        
        if(!inseriu)
        {
            ATRIBUIR(esquerda->chaves[j], chave);
            esquerda->ponteiros[j] = pEsquerda;
        }
        
        j = 0;
        // copia a segunda metade do nó em um novo nó, (incluindo o meio).
        for(i = metade-1; i < este->tamanho; i++)
        {   
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j] = este->ponteiros[i];
            j++;
        }
        
        free(este);
        
//UIS: INFORMACAO DE DEBUG
info->cfree += 1;

    }
    else if( A_MAIOR_QUE_B(chave, este->chaves[metade]) )
    {
        // TODO: Erro -> algoritmo tendencioso
        // aprensenta erros quando a quantidade de elementos é impar
        
        ATRIBUIR(meio, este->chaves[metade]);
        esquerda = este;
        j =0;
        
        // copia a segunda metade do nó em um novo nó (incluindo o meio).
        for(i = metade; i < este->tamanho; i++)
        {
            //insere a chave e o ponteiro proximo na posição.
            if( (!inseriu) && A_MAIOR_QUE_B(este->chaves[i], chave) )
            {
                ATRIBUIR(direita->chaves[j], chave);
                direita->ponteiros[j] = pEsquerda;
                j++;
                inseriu = TRUE;
            }
            
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j] = este->ponteiros[i];
            j++;
        }
        
        if(!inseriu)
        {
            ATRIBUIR(direita->chaves[j], chave);
            direita->ponteiros[j] = pEsquerda;
        }
        
    }
    else
    {
        ATRIBUIR(meio, chave);
        esquerda = este;
        
        j = 0;
        
        ATRIBUIR(direita->chaves[j], chave);
        direita->ponteiros[j] = pEsquerda;
        
        j++;
        
        // copia a segunda metade do nó em um novo nó.
        for(i = metade; i < este->tamanho; i++)
        {            
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j] = este->ponteiros[i];
            j++;
        }
        
    }

    no2 * Meio = (no2 *)malloc(sizeof(no2));
    if(!meio) exit(1);
    ATRIBUIR(Meio->chave, meio);
    
    
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
    
    /* aqui no final do algoritmo cabe refazer o encadeamento, salvar o no esquerda e direita, liberar
    a Memoria e retornar um nó que aponta para os dois.. */
    
    
    esquerda->tamanho = metade;
    direita->tamanho = metade + 1;

    esquerda->tipo = direita->tipo = FOLHA;
    
    direita->ponteiros[direita->tamanho] = ultimoPonteiro;

    Meio->direita = salvaNoFimDoArquivo(arquivo, info, direita);
    
    esquerda->ponteiros[esquerda->tamanho] = Meio->direita;
    
    Meio->esquerda = salvaNoDisco(arquivo, posicaoDeste, esquerda);
    
    
    free(esquerda);
    free(direita);
    
//UIS: INFORMACAO DE DEBUG
info->cfree += 2;
    
    return Meio;
}

// duplica e insere na pagina que não seja do tipo RAIZ ou INTERMEDIARIO;
no2 * insereEDuplicaNaoFolha(FILE * arquivo, arvoreInfo * info, no * este, int posicaoDeste, const char * chave, int pDireita)
{
    no * esquerda = NULL, * direita = NULL;
    DECLARAR_CHAVE(meio);
    int i, j;
    BOOL inseriu = FALSE;
    int metade = este->tamanho / 2;
    
    int pMeio;
    
    
    direita = (no *)malloc(sizeof(no));
    
    if(!direita)
    {
        printf("Erro: insereEDuplicaNaoFolha() -> Memoria insuficiente!");
        system("pause");
        exit(1);
    }
    
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
    
    // identifica qual a chave do meio. que será ignorada na divisão do nó
    if( A_MENOR_QUE_B(chave, este->chaves[metade-1]) )
    {
             
        ATRIBUIR(meio, este->chaves[metade-1]);
        pMeio = este->ponteiros[metade];
                
        esquerda = (no *)malloc(sizeof(no));
        if(!esquerda)
        {
            printf("Erro: insereEDuplicaNaoFolha() -> Memoria insuficiente!");
            system("pause");
            exit(1);
        }
        
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
        
        j = 0;
        /* aqui já exclui o meio.. pois meio fica na posicao metade-1*/
        for(i = 0; i < metade-1; i++)
        {
            //insere a chave e o ponteiro proximo na posição.
            if( (!inseriu) && A_MAIOR_QUE_B(este->chaves[i], chave) )
            {
                ATRIBUIR(esquerda->chaves[j], chave);
                esquerda->ponteiros[j+1] = pDireita;
                j++;
                inseriu = TRUE;
            }
            
            ATRIBUIR(esquerda->chaves[j], este->chaves[i]);
            esquerda->ponteiros[j+1] = este->ponteiros[i+1];
            j++;
        }
        
        if(!inseriu)
        {
            ATRIBUIR(esquerda->chaves[j], chave);
            esquerda->ponteiros[j+1] = pDireita;
        }
        
        esquerda->ponteiros[0] = este->ponteiros[0];
        
        j = 0;
        
        
        direita->ponteiros[0] = pMeio;
        // copia a segunda metade do nó em um novo nó.
        for(i = metade; i < este->tamanho; i++)
        {   
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j+1] = este->ponteiros[i+1];
            j++;
        }
        
        free(este);
        
//UIS: INFORMACAO DE DEBUG
info->cfree += 1;
        
    }
    else if( A_MAIOR_QUE_B(chave, este->chaves[metade]) )
    {
        ATRIBUIR(meio, este->chaves[metade]);
        
        esquerda = este;
        j = 0;
        
        direita->ponteiros[0] = este->ponteiros[metade+1];
        
        // copia a segunda metade do nó em um novo nó.
        for(i = metade+1; i < este->tamanho; i++)
        {
            //insere a chave e o ponteiro proximo na posição.
            if( (!inseriu) && A_MAIOR_QUE_B(este->chaves[i], chave) )
            {
                ATRIBUIR(direita->chaves[j], chave);
                direita->ponteiros[j+1] = pDireita;
                j++;
                inseriu = TRUE;
            }
            
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j+1] = este->ponteiros[i+1];
            j++;
        }
        
        if(!inseriu)
        {
            ATRIBUIR(direita->chaves[j], chave);
            direita->ponteiros[j+1] = pDireita;
        }
        
    }
    else
    {
        ATRIBUIR(meio, chave);
        esquerda = este;
        
        direita->ponteiros[0] = pDireita;
        
        j = 0;
        // copia a segunda metade do nó em um novo nó.
        for(i = metade; i < este->tamanho; i++)
        {            
            ATRIBUIR(direita->chaves[j], este->chaves[i]);
            direita->ponteiros[j+1] = este->ponteiros[i+1];
            j++;
        }
        
        //printf("\n");
        
    }

    no2 * Meio = (no2 *)malloc(sizeof(no2));
    if(!meio) exit(1);
    ATRIBUIR(Meio->chave, meio);
    
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
    
    esquerda->tamanho = metade;
    direita->tamanho = metade;
    
    esquerda->tipo = direita->tipo = INTERNO;
    
    /* aqui no final do algoritmo cabe salvar o no esquerda e direita, liberar
    a Memoria e retornar um nó que aponta para os dois.. */
    
    Meio->esquerda = salvaNoDisco(arquivo, posicaoDeste, esquerda);
    Meio->direita = salvaNoFimDoArquivo(arquivo, info, direita);

    
    free(esquerda);
    free(direita);
    
//UIS: INFORMACAO DE DEBUG
info->cfree += 2;
    
    return Meio;
}

// insere na pagina que não seja do tipo FOLHA ou RAIZFOLHA;
void insereSemDuplicarFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pEsquerda)
{
       
    int i, j;
    int ultimoPonteiro = este->ponteiros[este->tamanho];
    
    //para cada item procura a posição para inserir..
    for(i=0; i < este->tamanho; i++)
    {
        if( A_MAIOR_QUE_B(este->chaves[i], chave) )
        {
            /*
                "aloca" espaço para a chave e para o ponteiro.
                Copia o item anterior para poder inserir um novo item.
            */
            for(j = este->tamanho; j > i ; j--)
            {
                ATRIBUIR(este->chaves[j], este->chaves[j-1]);
                este->ponteiros[j] = este->ponteiros[j-1];
            }
            break;
        }
        else if( A_IGUAL_A_B(este->chaves[i], chave) )
        {
             return;
        }
    }

    ATRIBUIR(este->chaves[i], chave);
    este->ponteiros[i] = pEsquerda;
    
    este->tamanho++;
    este->ponteiros[este->tamanho] = ultimoPonteiro;
    
    salvaNoDisco(arquivo, posicaoDeste, este);
}

// insere na pagina que não seja do tipo RAIZ ou INTERMEDIARIO;
void insereSemDuplicarNaoFolha(FILE * arquivo, no * este, int posicaoDeste, const char * chave, int pDireita)
{    
    int i, j;
    
    //para cada item procura a posição para inserir..
    for(i=0; i < este->tamanho; i++)
    {
        if( A_MAIOR_QUE_B(este->chaves[i], chave) )
        {
            /*
                "aloca" espaço para a chave e para o ponteiro.
                Copia o item anterior para poder inserir um novo item.
            */
            for(j = este->tamanho ; j > i ; j--)
            {
                ATRIBUIR(este->chaves[j], este->chaves[j-1]);
                este->ponteiros[j+1] = este->ponteiros[j];
            }
            break;
        }
        else if( A_IGUAL_A_B(este->chaves[i], chave) )
        {
            return;
        }
    }

    ATRIBUIR(este->chaves[i], chave);
    este->ponteiros[i+1] = pDireita;
    
    este->tamanho++;

    salvaNoDisco(arquivo, posicaoDeste, este);

}


//
void crescerArvore(FILE * arquivo, arvoreInfo * info, no2 * resto)
{
    no * novaRaiz = (no *)malloc(sizeof(no));
    
//UIS: INFORMACAO DE DEBUG
info->cmalloc += 1;
    
    novaRaiz->tamanho = 1;
    novaRaiz->tipo = RAIZ;
    ATRIBUIR(novaRaiz->chaves[0], resto->chave);
    
    // Aqui está errado...
    novaRaiz->ponteiros[0] = resto->esquerda;
    novaRaiz->ponteiros[1] = resto->direita;
    
    //citarNovaRaiz(info, salvaNoFimDoArquivo(arquivo, info, novaRaiz) , novaRaiz);
    info->posicaoDaRaiz = salvaNoFimDoArquivo(arquivo, info, novaRaiz);
    
    
//    free(info->raiz);
    
//DEBUG INFO
//info->cfree += 1;
    
    info->raiz = novaRaiz;

    salvarCabecalho(arquivo, info);

    
}


/**/
no2 * inserirNaArvore(FILE * arquivo, arvoreInfo * info, int posicaoDeste, no * este, const char * chave, int pEsquerda)
{

//problema: usar passagem de parametros por cópia;
// ou criar um garbage-collector para a raiz;

    no2 * resto = NULL;
    //char centro;
    int posicao;
    int i;

    int TipoDeste = este->tipo;

    if( (este->tipo == FOLHA) || (este->tipo == RAIZFOLHA) )
    {

           if(este->tamanho == DIM)
           {                

                //o nó já está cheio
                resto = insereEDuplicaFolha(arquivo, info, este, posicaoDeste, chave, pEsquerda);

                // erro: muda o tipo de este;
                if(TipoDeste == RAIZFOLHA)
                {
                    //Cita a folha mais a esquerda; só preciso alterar agora em arvore retira
                    //se a folha mais à esquerda for removida;
                    info->posicaoDaPrimeiraFolha = resto->esquerda;
                    
                    crescerArvore(arquivo, info, resto);
                    
                    info->altura++;
                    
                    
                }

           }
           else
           {
               
               //o nó ainda não está cheio
               insereSemDuplicarFolha(arquivo, este, posicaoDeste, chave, pEsquerda);

//DEBUG TESTE
if(este != info->raiz)
{
    info->cfree++;               
    free(este); // resolvido o problema com este free;
}

           }
           
           

           return resto;
    }
    else
    {
        
        //Procurar posição no nodo atual, fase de inserção por busca recursiva;
        
        // busca binária;
        int menor = 0;
        int maior = este->tamanho - 1;

        while (menor <= maior) 
        {
            posicao = ( menor + maior ) / 2;

            if ( A_MENOR_QUE_B(chave, este->chaves[posicao]) )
                maior = posicao - 1;
            else
                menor = posicao + 1;
        }

        //não sei se serve para todos os casos.
        if ( A_MAIOR_QUE_B(chave, este->chaves[maior]) )
            maior++;

        if(maior < 0) maior = 0;

        //1 atribuição.
        posicao = maior;

        resto = inserirNaArvore(arquivo, info, este->ponteiros[posicao], buscaNoDisco(arquivo, este->ponteiros[posicao]), chave, pEsquerda);

//UIS: INFORMACAO DE DEBUG, BUSCANODISCO
info->cmalloc += 1;

        if(resto) // Inserir resto no nodo..
        {
           if(este->tamanho == DIM)
           {
               no2 * novoResto = NULL;

                // insere em um nodo que está cheio
                if(este->tipo == RAIZ)
                {
                //CONTEM free(este);
                    novoResto = insereEDuplicaNaoFolha(arquivo, info, este, posicaoDeste, resto->chave, resto->direita);
                    
                    
                    crescerArvore(arquivo, info, novoResto);
                    free(resto);

info->altura++;

//UIS: INFORMACAO DE DEBUG
info->cfree += 1;




                    return NULL;                    
                }
                else
                {
                    // este->tipo == INTERNO
                    novoResto = insereEDuplicaNaoFolha(arquivo, info, este, posicaoDeste, resto->chave, resto->direita);
                    free(resto);


//UIS: INFORMACAO DE DEBUG
info->cfree += 1;

                    return novoResto;
                }
           }
           else
           {

               //o valor de posicaoDeste está errado.

               insereSemDuplicarNaoFolha(arquivo, este, posicaoDeste, resto->chave, resto->direita);

               free(resto);

//UIS: INFORMACAO DE DEBUG
info->cfree += 1;

if(este != info->raiz)
{
    //new
    free(este);
    //UIS: INFORMACAO DE DEBUG
    info->cfree += 1;
}

               return NULL;
           }
        } // end if 

    }


if(este != info->raiz)
{
    //new
    free(este);
    //UIS: INFORMACAO DE DEBUG
    info->cfree += 1;
}

    return NULL;
}







/*********************************************************************************************************
FUNÇÕES PARA REMOVER DA ARVORE
**********************************************************************************************************/


/*2011 -> preciso de um flag para a posicao removida. o falg será retornado.*/


BOOL retiraDaArvore(FILE * arquivo, arvoreInfo * info, int posicaoDaRaiz, no * raiz, const char * chave, int * ponteiroRemovido)
{
    no * filho;
    no * outroFilho;
    no * esquerda;
    no * direita;

    int posicaoDaEsquerda;
    int posicaoDaDirerita;

    //indica se é necessário juntar ou emprestar valores entre esquerda e direita;
    BOOL alterarFilhos;
    
    //Busca binária
    int menor = 0;
    int maior = raiz->tamanho - 1;
    int posicao;
    BOOL encontrou = FALSE;

    while (menor <= maior) 
    {
        posicao = ( menor + maior ) / 2;

        if ( A_IGUAL_A_B(chave, raiz->chaves[posicao]) )
        {
            encontrou = TRUE;
            break;
        }

        if ( A_MENOR_QUE_B(chave, raiz->chaves[posicao]) )
            maior = posicao - 1;
        else
            menor = posicao + 1;
    }

    //aqui ainda precisa de mais uma verificação, se não encontrou.

    //depois da busca verificar se a posicao é menor, maior ou igual à chave
    // retorna TRUE se for necessário fazer alguma alteração no nodo

    if( (raiz->tipo == FOLHA) || (raiz->tipo == RAIZFOLHA) )
    {
        if(encontrou)
        {
            *ponteiroRemovido = raiz->ponteiros[posicao];
            
            removePosicaoFolha(raiz, posicao);

            salvaNoDisco(arquivo, posicaoDaRaiz, raiz);

            if(raiz->tamanho < MINIMO) return TRUE;
            return FALSE;
        }
        else
        {
            return FALSE;
        }
    }

    //Sempre será necessário saber se o nodo é menor ou maior do que um determinado meio?
    if ( A_MENOR_QUE_B(chave, raiz->chaves[posicao]) )
    {
        // a esquerda é exatamente ele porque ha mais ponteiros doque chaves;
        esquerda = buscaNoDisco(arquivo, raiz->ponteiros[posicao]);
        filho = esquerda;
        direita = NULL;
    }
    else
    {
        //indica que é o ultimo ponteiro da direita, ou que chave = raiz->chaves[posicao]
        //que não tem chave no mesmo indice (não passa pela busca binária)
        posicao += 1;

        direita = buscaNoDisco(arquivo, raiz->ponteiros[posicao]);
        filho = direita;
        esquerda = NULL;
    }
    
    //sempre ocorre quando a quantidade de chaves do filho é menor do que o mínimo
    alterarFilhos = retiraDaArvore(arquivo, info, raiz->ponteiros[posicao], filho, chave, ponteiroRemovido);


    //se removido < minimo
    // if(filho->tamanho < MINIMO)
    if(alterarFilhos)
    {        
        
        // definir centro esquerda e direita;
        if(!esquerda)
        {
            esquerda = buscaNoDisco(arquivo, raiz->ponteiros[posicao - 1]);
            outroFilho = esquerda;
        }
        else
        {
            direita = buscaNoDisco(arquivo, raiz->ponteiros[posicao + 1]);
            outroFilho = direita;
        }

        //agora todas as perguntas podem ser feitas ao outroFilho do qual não foram retirados valores
        //o filho pode ser igual ao da esquerda ou ao da direita;

        //posicao agora tem que marcar o MEIO
        if(posicao == raiz->tamanho)
        {
            posicao = raiz->tamanho - 1;
        }
        
        //Novo, 23 ago 2010
        posicaoDaEsquerda = raiz->ponteiros[posicao];
        posicaoDaDirerita = raiz->ponteiros[posicao + 1];


        // INICIO: junta as paginas ou empresta valores de uma para a outra

        if( (outroFilho->tamanho) == MINIMO )
        {
            if(filho->tipo == FOLHA)
            {
                juntaFolhas(esquerda, direita);
            }
            else
            {
                juntaPaginasInternas(raiz, posicao, esquerda, direita);
            }

            //depois de juntar é necessário remover a posicao da raiz;
            // TODO: como foi que ele removeu a posicao certa 'g', só conseguiu porque 
            removePosicaoNaoFolha(raiz, posicao);

            // 1. aqui ha mais coisas a fazer. se a raiz->tipo == raiz;

            // 2. antes o tipo disso era raiz, mas agora passou a ser deletado;
            if(raiz->tipo == DELETADO)
            {
                //testar..
                //citarNovaRaiz(info, raiz->ponteiros[0], esquerda);
                info->raiz = esquerda;
                info->posicaoDaRaiz = raiz->ponteiros[0];

                if(esquerda->tipo == FOLHA)
                {
                    esquerda->tipo = RAIZFOLHA;
                }
                else
                {
                    esquerda->tipo = RAIZ;
                }

            }


            // salvar sempre a raiz depois de juntar suas folhas
            salvaNoDisco(arquivo, posicaoDaRaiz, raiz);
            salvaNoDisco(arquivo, posicaoDaEsquerda, esquerda);
            salvaNoDisco(arquivo, posicaoDaDirerita, direita);


            //verifica se o nó intermediario é menor do que o mínimo
            if(raiz->tamanho < MINIMO)
            {
                return TRUE;
            }

        }
        else // senão, nesse algoritmo, ( (outroFilho->tamanho) > MINIMO )
        {
            if(filho == esquerda)
            {
                if(filho->tipo == FOLHA)
                {
                    CompartilhaDireitaPEsquerda_FOLHA(raiz, posicao, esquerda, direita);
                }
                else
                {
                    CompartilhaDireitaPEsquerda_NAOFOLHA(raiz, posicao, esquerda, direita);
                }
            }
            else // filho == direita
            {
                if(filho->tipo == FOLHA)
                {
                    CompartilhaEsquerdaPDireita_FOLHA(raiz, posicao, esquerda, direita);
                }
                else
                {
                    CompartilhaEsquerdaPDireita_NAOFOLHA(raiz, posicao, esquerda, direita);
                }
            }

            // o que ocorre após modificar os valores de esquerda e direita?
            // salva esquerda e direita no disco; salva a raiz;
            salvaNoDisco(arquivo, posicaoDaRaiz, raiz);
            salvaNoDisco(arquivo, posicaoDaEsquerda, esquerda);
            salvaNoDisco(arquivo, posicaoDaDirerita, direita);

        }

        // FIM: junta as paginas ou empresta valores de uma para a outra

    }


    return FALSE;
    
    //rever isso:
    //quando não há necessidade de salvar o nodo? quando ele não foi alterado;

    //rever o significado dos retornos das chamadas recursivas;

}




/******************************************************************************************************

Funções auxiliares na deleção

*******************************************************************************************************/


void removePosicaoFolha(no * este, int posicaoDaChave)
{
    int i;
    
    if(posicaoDaChave > este->tamanho - 1) return;

    for(i = posicaoDaChave; i < este->tamanho; i++)
    {
         ATRIBUIR(este->chaves[i], este->chaves[i+1]);
         este->ponteiros[i] = este->ponteiros[i+1];
    }
    
    este->tamanho--;
    
    este->ponteiros[este->tamanho] = este->ponteiros[este->tamanho + 1];
    
    //só ocorrerá na raiz.
    if(este->tamanho == 0)
    {
         este->tipo = DELETADO;
    }

}

void removePosicaoNaoFolha(no * este, int posicaoDaChave)
{
    int i;
    
    if(posicaoDaChave > este->tamanho - 1) return;

    for(i = posicaoDaChave; i < este->tamanho; i++)
    {
         ATRIBUIR(este->chaves[i], este->chaves[i+1]);
         este->ponteiros[i+1] = este->ponteiros[i+2];
    }
    
    este->tamanho--;
    
    //só ocorrerá na raiz.
    if(este->tamanho == 0)
    {
         este->tipo = DELETADO;
    }
    
}

// ocorre um erro se direita < esquerda;
void CompartilhaDireitaPEsquerda_FOLHA(no * meio, int pos, no * esquerda, no * direita)
{
    
    if(esquerda->tamanho > direita->tamanho)
    {
        printf("erro: CompartilhaDireitaPEsquerda_NAOFOLHA() -> esquerda > direita\n");
        system("pause");
        exit(1);
    }
    
    int i, j;
    int total = esquerda->tamanho + direita->tamanho;
    int metade = total / 2;

    //marca o ultimo ponteiro da esquerda (que aponta p proxima pagina)
    int ultimoEsquerda = esquerda->ponteiros[esquerda->tamanho];

    j = 0;
    //copia valores da direita na esquerda.. a partir do primeiro da esquerda
    //a quantidade de valores vai até a metade
    for(i = esquerda->tamanho; i < metade; i++)
    {
        ATRIBUIR(esquerda->chaves[i], direita->chaves[j]);
        esquerda->ponteiros[i] = direita->ponteiros[j];
        j++;
    }

    esquerda->tamanho = metade;
    esquerda->ponteiros[esquerda->tamanho] = ultimoEsquerda;

    //depois de copiados os nodos da esquerda deve-se re-organizar e reduzir o tamanho da direita;
    i = 0;
    for(; j < direita->tamanho; j++)
    {
        ATRIBUIR(direita->chaves[i], direita->chaves[j]);
        direita->ponteiros[i] = direita->ponteiros[j];
        i++;
    }

    direita->ponteiros[i] = direita->ponteiros[j];

    direita->tamanho = total - metade;

    //cita o novo meio;
    ATRIBUIR(meio->chaves[pos], direita->chaves[0]);

    //
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}

// ocorre um erro se direita < esquerda;
void CompartilhaDireitaPEsquerda_NAOFOLHA(no * meio, int pos, no * esquerda, no * direita)
{
    
    if(esquerda->tamanho > direita->tamanho)
    {
        printf("erro: CompartilhaDireitaPEsquerda_NAOFOLHA() -> esquerda > direita\n");
        system("pause");
        exit(1);
    }
    
    int i, j;
    int total = esquerda->tamanho + direita->tamanho;
    int metade = total / 2;

    //deslocou 1;
    ATRIBUIR(esquerda->chaves[esquerda->tamanho], meio->chaves[pos]);
    esquerda->ponteiros[esquerda->tamanho + 1] = direita->ponteiros[0];

    j = 0;

    for(i = esquerda->tamanho + 1; i < metade; i++)
    {
        ATRIBUIR(esquerda->chaves[i], direita->chaves[j]);
        esquerda->ponteiros[i+1] = direita->ponteiros[j+1];
        j++;
    }


    //cita o novo meio;
    ATRIBUIR(meio->chaves[pos], direita->chaves[j]);

    i = 0;
    for(j = j + 1; j < direita->tamanho; j++)
    {
        ATRIBUIR(direita->chaves[i], direita->chaves[j]);
        direita->ponteiros[i] = direita->ponteiros[j];
        i++;
    }

    direita->ponteiros[i] = direita->ponteiros[j];

    direita->tamanho = total - metade;
    esquerda->tamanho = metade;

    //
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}

// ocorre um erro se esquerda < dieita;
void CompartilhaEsquerdaPDireita_FOLHA(no * meio, int pos, no * esquerda, no * direita)
{

    if(esquerda->tamanho < direita->tamanho)
    {
        printf("erro: CompartilhaEsquerdaPDireita_NAOFOLHA() -> esquerda < direita");
        system("pause");
        exit(1);
    }

    int i, j;
    int total = esquerda->tamanho + direita->tamanho;
    int metade = total / 2;
    int deslocamentoDireita = total - metade - direita->tamanho;
    int ultimoDir = direita->ponteiros[direita->tamanho];
    int ultimoEsq = esquerda->ponteiros[esquerda->tamanho];

    for(i = direita->tamanho - 1; i >= 0; i--)
    {
        ATRIBUIR(direita->chaves[i + deslocamentoDireita], direita->chaves[i]);
        direita->ponteiros[i + deslocamentoDireita] = direita->ponteiros[i];
    }

    direita->tamanho += deslocamentoDireita;
    direita->ponteiros[direita->tamanho] = ultimoDir;

    //TODO: testar isso..
    j = metade;

    for(i = 0; i < deslocamentoDireita; i++)
    {
        ATRIBUIR(direita->chaves[i], esquerda->chaves[j]);
        direita->ponteiros[i] = esquerda->ponteiros[j];
        j++;
    }

    esquerda->tamanho = metade;
    esquerda->ponteiros[esquerda->tamanho] = ultimoEsq;

    //cita o novo meio;
    ATRIBUIR(meio->chaves[pos], direita->chaves[0]);


    //
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}

// ocorre um erro se esquerda < dieita;
void CompartilhaEsquerdaPDireita_NAOFOLHA(no * meio, int pos, no * esquerda, no * direita)
{

    if(esquerda->tamanho < direita->tamanho)
    {
        printf("erro: CompartilhaEsquerdaPDireita_NAOFOLHA() -> esquerda < direita");
        system("pause");
        exit(1);
    }

    int i, j;
    int total = esquerda->tamanho + direita->tamanho;
    int metade = total / 2;
    int deslocamentoDireita = total - metade - direita->tamanho;
    int ultimoDir = direita->ponteiros[direita->tamanho];

    for(i = direita->tamanho - 1; i >= 0; i--)
    {
        ATRIBUIR(direita->chaves[i + deslocamentoDireita], direita->chaves[i]);
        direita->ponteiros[i + deslocamentoDireita] = direita->ponteiros[i];
    }

    direita->tamanho += deslocamentoDireita;
    direita->ponteiros[direita->tamanho] = ultimoDir;

    //TODO: testar isso..
    j = metade + 1;

    for(i = 0; i < deslocamentoDireita -1; i++)
    {
        ATRIBUIR(direita->chaves[i], esquerda->chaves[j]);
        direita->ponteiros[i] = esquerda->ponteiros[j];
    }

    //adicionando o meio
    direita->ponteiros[i] = esquerda->ponteiros[esquerda->tamanho];
    ATRIBUIR(direita->chaves[i], meio->chaves[pos]);

    ATRIBUIR(meio->chaves[pos], esquerda->chaves[metade]);
    esquerda->tamanho = metade;

    //
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}

//
void juntaFolhas(no * esquerda, no * direita)
{
    int i, j;
    
    j = esquerda->tamanho;

    for(i = 0; i < direita->tamanho; i++)
    {
        ATRIBUIR(esquerda->chaves[j], direita->chaves[i]);
        esquerda->ponteiros[j] = direita->ponteiros[i];
        j++;
    }

    esquerda->tamanho += direita->tamanho;
    esquerda->ponteiros[esquerda->tamanho] = direita->ponteiros[direita->tamanho];
    
    direita->tipo = DELETADO;
    direita->tamanho = 0;
    //
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}

/*
         Objetivo: juntar as paginas da direita e da esquerda em uma única
         Sempre junta-se as paginas na posição da esquerda.
         a chave da posicao do meio sempre desce.
*/
void juntaPaginasInternas(no * meio, int pos, no * esquerda, no * direita)
{
    int i, j;

    //inseriu o meio, este deve ser removido da raiz.
    ATRIBUIR(esquerda->chaves[esquerda->tamanho], meio->chaves[pos]);
    esquerda->ponteiros[esquerda->tamanho + 1] = direita->ponteiros[0];

    j = esquerda->tamanho + 1;

    for(i = 0; i < direita->tamanho; i++)
    {
        ATRIBUIR(esquerda->chaves[j], direita->chaves[i]);
        esquerda->ponteiros[j+1] = direita->ponteiros[i+1];
        j++;
    }

    esquerda->tamanho += (direita->tamanho + 1);
    
    direita->tipo = DELETADO;
    direita->tamanho = 0;
//    printf("FALTA SALVAR A ESQUERDA E A DIREITA\n");
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*  Modificada em 2011*
    sbanco = nome do arquivo do banco de dados;
    sindice = nome do arquivo do indice;
    ssaida = nome do arquivo de saida;
*/
void percorrerArvore(char * sbanco, char * sindice, char * ssaida)
{
    arvoreInfo * info = NULL;

    dbaseHeader header;
    char * registro;

    FILE * indice = fopen(sindice, "rb");
    FILE * dbf = fopen(sbanco, "rb");
    
    FILE * saidaTela = fopen(ssaida,"w");

    carregarArvoreInfo(indice, &info);
    fread(&header, 1, sizeof(dbaseHeader), dbf);
    
    // O primeiro byte do registro é indicador de deletado
    registro = (char *) malloc( (header.dataRecordSize) * sizeof(char) );

    int i, proximo;
    no * n = buscaNoDisco(indice, info->posicaoDaPrimeiraFolha);
    proximo = n->ponteiros[n->tamanho];
    
    fprintf(saidaTela, "[%s]\n", tToString((enum Tipo)n->tipo));

    for(i=0; i < n->tamanho; i++)
    {
        fseek(dbf, (header.headerSize * sizeof(char)) + (n->ponteiros[i] * (header.dataRecordSize)), SEEK_SET);
        //Lê o registro
        fread(registro, (header.dataRecordSize), sizeof(char), dbf);
        registro[header.dataRecordSize] = '\0';
        // conferindo..
        MOSTRAR(n->chaves[i]);
        fprintf(saidaTela, "%s\n", n->chaves[i]);
        
        printf("\n");
        //Mostra o registro
        printf("%s\n", registro);
        fprintf(saidaTela, "%s\n", registro);
    }
    
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    fprintf(saidaTela,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    

    system("pause");
    printf("Percorrendo o resto..\n");
    
    fprintf(saidaTela,"Percorrendo o resto..\n");

    while(proximo > 0)
    {
                  
        fprintf(saidaTela, "[%s]\n", tToString((enum Tipo)n->tipo));
                        
        for(i=0; i < n->tamanho; i++)
        {
            

            fseek(dbf, (header.headerSize * sizeof(char)) + (n->ponteiros[i] * (header.dataRecordSize)), SEEK_SET);
            //Lê o registro
            fread(registro, (header.dataRecordSize), sizeof(char), dbf);

            registro[header.dataRecordSize] = '\0';

            // conferindo..
            //MOSTRAR(n->chaves[i]);
            fprintf(saidaTela, "%s\n", n->chaves[i]);

            //printf("\n");

            //Mostra o registro
            //printf("%s\n", registro);
            fprintf(saidaTela, "%s\n", registro);

        }
        
        
        fprintf(saidaTela,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        
        proximo = n->ponteiros[n->tamanho];
        free(n);

        n = buscaNoDisco(indice, proximo);
    }

    printf("\n");
    fprintf(saidaTela, "\n");


    fclose(saidaTela);
    fclose(dbf);
    fclose(indice);
}

