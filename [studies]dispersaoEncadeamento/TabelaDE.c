/****
 *
 * Arquivo TabelaDE.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2008
 * �ltima modifica��o: 07/06/2016
 *
 * Descri��o: Implementa��o de opera��es de busca, inser��o e
 *            remo��o usando dispers�o com encadeamento
 *
 ****/

/*********************** Includes *************************/

#include "TabelaDE.h"   /* Interface deste m�dulo      */
#include <stdlib.h>     /* Miscel�nea de fun��es       */
#include <stdio.h>      /* Fun��es de E/S              */
#include <string.h>     /* Fun��es strxxx() e memxxx() */
#include "Registros.h"  /* Processamento de registros  */
#include "Lista.h"      /* Lista encadeada */

/********************* Fun��es Locais *********************/

/****
 *
 * ComparaListas(): compara duas estruturas do tipo
 *                  tElementosEmLista
 *
 * Par�metros:
 *      p1 (entrada) - ponteiro para a primeira estrutura
 *      p2 (entrada) - ponteiro para a segunda estrutura
 *
 * Retorno:
 *   0, se os campos nElementos das estruturas forem iguais
 *   > 0, se o campo nElementos da primeira estrutura for maior
 *   < 0, se o campo nElementos da primeira estrutura for menor
 *
 * Observa��o: Esta fun��o � usada para ordenar as listas
 *             de acordo com seus n�meros de elementos
 *
 ****/
static int ComparaListas(const void *e1, const void *e2)
{
   tElementosEmLista ec1 = *(tElementosEmLista *)e1,
                     ec2 = *(tElementosEmLista *)e2;

   return (int) ec1.nElementos - ec2.nElementos;
}

/********************* Fun��es Globais ********************/

/****
 *
 * CriaTabelaDE(): Cria e inicializa uma tabela de dispers�o
 *                 com encadeamento
 *
 * Par�metros:
 *      nElementos (entrada) - n�mero de posi��es da tabela
 *                             de dispers�o
 *
 * Retorno: A tabela de dispers�o criada
 *
 ****/
tTabelaDE CriaTabelaDE(int nElementos)
{
   tTabelaDE tabela;
   int       i;

      /* Aloca o array que representa a tabela de dispers�o */
   tabela = calloc(nElementos, sizeof(tListaSE));

      /* Certifica que houve aloca��o */
   ASSEGURA(tabela, "Impossivel alocar tabela de dispersao");

      /* Inicializa as listas encadeadas */
   for (i = 0; i < nElementos; ++i)
      IniciaListaSE(tabela + i);

   return tabela;
}

/****
 *
 * BuscaDE(): Executa uma busca simples numa tabela de
 *            dispers�o com encadeamento
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o a ser usada
 *
 * Retorno: �ndice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contr�rio
 *
 ****/
int BuscaDE(tTabelaDE tabela, int tamTabela, tCEP chave, tFDispersao fDispersao)
{
  int postColetor;
  postColetor = fDispersao(chave)%tamTabela;

  if(!(postColetor > 0 && postColetor < tamTabela)){
   return -1;
  }

   //adcionar o indice encontrado a tabela pra gente conseguir fazer a busca na linked list especifica
  return BuscaListaSE(tabela + postColetor, chave);
}

/****
 *
 * InsereDE(): Insere uma nova chave numa tabela de dispers�o
 *             com encadeamento
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *      conteudo (entrada) - a chave de busca e seu
 *                           respectivo �ndice
 *      fDispersao (entrada) - ponteiro para a fun��o de
 *                             dispers�o a ser usada
 *
 * Retorno: Nada
 *
 ****/
void InsereDE( tTabelaDE tabela, int tamTabela, 
const tCEP_Ind *conteudo, tFDispersao fDispersao ){
   int posColetor;
   posColetor = fDispersao(conteudo->chave)%tamTabela;

   if(!(posColetor >= 0 && posColetor < tamTabela)){
      return -1;
   }

   return InsereListaSE(&tabela[posColetor], conteudo);
}

/****
 *
 * RemoveDE(): Remove um item de uma tabela dispers�o com
 *             encadeamento
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de
 *                             dispers�o a ser usada
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveDE(tTabelaDE tabela, int tamTabela, tCEP chave, tFDispersao fDispercao)
{
  int posColetor = fDispercao(chave)%tamTabela;

  if(!(posColetor >= 0 && posColetor < tamTabela)){
   return -1;
  }

  return RemoveListaSE(&tabela[posColetor], chave);
}

/****
 *
 * NChavesEmListasDE(): Escreve num arquivo o n�mero de
 *                        elementos em cada cont�iner da
 *                        tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *      stream (entrada) - stream associado ao arquivo onde o
 *                         resultado da opera��o ser� escrito
 *
 * Retorno: Nada
 *
 ****/
void NChavesEmListasDE( tTabelaDE tabela, int tamTabela, FILE *stream )
{
   int                i, n, nMax, frequencia;
   tElementosEmLista *ar;

   ASSEGURA(stream, "\nStream recebido e' NULL\n");

      /* Aloca o array que ir� armazenar �ndices de */
      /* listas e respectivos n�meros de elementos  */
   ar = calloc(tamTabela, sizeof(tElementosEmLista));

   ASSEGURA(ar, "\nNao foi possivel alocar array\n");

      /* Preenche o conte�do do array */
   for (i = 0; i < tamTabela; ++i) {
      ar[i].iLista = i;
      ar[i].nElementos = ComprimentoListaSE(tabela[i]);
   }

      /* Ordena o array de acordo com */
      /* os n�meros de elementos      */
   qsort( ar, tamTabela, sizeof(tElementosEmLista),
          ComparaListas );

   for (i = 0; i < tamTabela; ++i)
      fprintf( stream, "%d\t%d\n", ar[i].iLista,
               ar[i].nElementos );

   nMax = ar[tamTabela - 1].nElementos;

   fprintf(stream, "\n");

      /* O c�digo a seguir pode ser bastante melhorado, */
      /* uma vez que o array � ordenado pelo n�mero de */
      /* elementos em cada cont�iner                   */
   for (n = 0; n <= nMax; ++n) {
      frequencia = 0;

      for (i = 0; i < tamTabela; ++i)
         if (ar[i].nElementos == n)
            ++frequencia;

      if (frequencia)
         fprintf( stream, "Numero de listas com %d "
                  "elementos:\t%4.d\n", n, frequencia );
   }

   free(ar); /* O array n�o � mais necess�rio */
}

/****
 *
 * NChavesDE(): Calcula o n�mero de chaves na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de chaves na tabela de dispers�o
 *
 ****/
int NChavesDE(tTabelaDE tabela, int tamTabela)
{
   int i, n = 0;

   for (i = 0; i < tamTabela; ++i)
      n += ComprimentoListaSE(tabela[i]);

   return n;
}

/****
 *
 * NListasNaoVaziasDE(): Determina o n�mero de listas que n�o
 *                     s�o vazias numa tabela de dispers�o
 *                     com encadeamento
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de conteiners que n�o s�o vazios
 *
 ****/
int NListasNaoVaziasDE( tTabelaDE tabela, int tamTabela )
{
   int i, n = 0;

   for (i = 0; i < tamTabela; ++i)
      if (tabela[i])
         n++;

   return n;
}

/****
 *
 * DestroiTabelaDE(): Libera o espa�o ocupado por uma tabela
 *                  de dispers�o com encadeamento
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaDE(tTabelaDE *tabela, int tamTabela)
{
   int i;

      /* Destr�i as listas encadeadas */
   for (i = 0; i < tamTabela; ++i) {
      DestroiListaSE(*tabela + i);
   }

      /* Libera o espa�o ocupado pelo array */
   free(*tabela);

   *tabela = NULL;
}
