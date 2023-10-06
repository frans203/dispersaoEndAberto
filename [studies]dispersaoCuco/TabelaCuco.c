/****
 *
 * Arquivo TabelaCuco.c
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 04/08/2008
 * �ltima modifica��o: 08/06/2010
 *
 * Descri��o: Implementa��o de opera��es de busca, inser��o e
 *            remo��o usando dispers�o aberto
 *
 ****/

/*********************** Includes *************************/

#include <stdlib.h>     /* Miscel�nea de fun��es       */
#include <stdio.h>      /* Fun��es de E/S              */
#include <string.h>     /* Fun��es strxxx() e memxxx() */
#include <math.h>     /* Fun��es strxxx() e memxxx() */
#include "TabelaCuco.h"  /* Interface deste m�dulo      */
#include "Registros.h"  /* Processamento de registros  */

/********************* Fun��es Locais *********************/

/****
 *
 * RedimensionaCuco(): Redimensiona uma tabela de dispers�o cuco
 *
 * Par�metros:
 *     tabela (entrada/sa�da) - a tabela de dispers�o
 *     chaveEIndice (entrada) - o par chave/�ndice que ser� inserido
 *                              ap�s o redimensionamento da tabela
 *
 * Retorno: Nada
 *
 ****/
static void RedimensionaCuco(tTabelaCuco *tabela, tCEP_Ind chaveEIndice)
{
   tTabelaCuco novaTabela;
   int i, teste, novoTamanho;

   novoTamanho = 1.5*tabela->tam;

   CriaTabelaCuco(&novaTabela, novoTamanho, tabela->fD1, tabela->fD2);

   for(i =0 ;i<tabela->tam;i++){
      if(tabela->tab1[i].status == OCUPADO){
         InsereCuco(&novaTabela, tabela->tab1[i].chaveEIndice);
      }

      if(tabela->tab2[i].status == OCUPADO){
         InsereCuco(&novaTabela, tabela->tab2[i].chaveEIndice);
      }
   }

   DestroiTabelaCuco(tabela);

   *tabela = novaTabela;

   InsereCuco(tabela, chaveEIndice);

}

/********************* Fun��es Globais ********************/

/****
 *
 * CriaTabelaCuco(): Cria e inicializa uma tabela de dispers�o cuco
 *
 * Par�metros:
 *     tabelas (sa�da) - a tabela cuco criada e iniciada
 *     nElementos (entrada) - n�mero de posi��es da tabela
 *                            de dispers�o
 *     fD1, fD2 (entrada) - as fun��es de dispers�o utilizadas
 *
 * Retorno: Nada
 *
 ****/
void CriaTabelaCuco( tTabelaCuco *tabelas, int nElementos,
                     tFDispersaoCuco fD1, tFDispersaoCuco fD2 )
{
   int i;

      /* Aloca espa�o para as duas tabelas */
   tabelas->tab1 = calloc(nElementos, sizeof(tColetorCuco));
   tabelas->tab2 = calloc(nElementos, sizeof(tColetorCuco));
   ASSEGURA( tabelas->tab1 && tabelas->tab2,
             "\nImpossivel alocar a tabela de dispers�o\n" );

      /* Inicia as duas tabelas */
   for (i = 0; i < nElementos; ++i) {
         /* Todos os elementos est�o inicialmente desocupados */
      tabelas->tab1[i].status = VAZIO;
      tabelas->tab2[i].status = VAZIO;
   }

      /* Inicia as fun��es de dispers�o */
   tabelas->fD1 = fD1;
   tabelas->fD2 = fD2;

      /* Inicia o n�mero de chaves nas tabelas */
   tabelas->nChaves = 0;
}

/****
 *
 * BuscaCuco(): Executa uma busca simples numa tabela de dispers�o cuco
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *
 * Retorno: �ndice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contr�rio
 *
 ****/
int BuscaCuco( tTabelaCuco tabela, tCEP chave )
{
  int pos;
  pos = tabela.fD1(chave, tabela.tam);

  if(tabela.tab1[pos].status == OCUPADO && !strcmp(tabela.tab1[pos].chaveEIndice.chave, chave)){
      return tabela.tab1[pos].chaveEIndice.indice;
  }

  pos = tabela.fD2(chave, tabela.tam);

  if(tabela.tab2[pos].status == OCUPADO && !strcmp(tabela.tab2[pos].chaveEIndice.chave, chave)){
   return tabela.tab2[pos].chaveEIndice.indice;
  }

  return -1;
}

/****
 *
 * InsereCuco(): Faz inser��o numa tabela de dispers�o cuco
 *
 * Par�metros:
 *      tabela (entrada/sa�da) - a tabela de dispers�o
 *      chaveEIndice (entrada) - o par chave/�ndice a ser inserido
 *
 * Retorno: 1, se houver inser��o; 0, em caso contr�rio
 *
 ****/
int InsereCuco(tTabelaCuco *tabela, tCEP_Ind chaveEIndice)
{
   int          i,
                pos,
                maxDesalojamentos;
   tCEP_Ind     aux;

      /* Verifica se a chave j� existe na tabela */
   if (BuscaCuco(*tabela, chaveEIndice.chave) >= 0)
      return 0; /* Chave j� existe na tabela */

      /* Calcula o valor limite de desalojamentos. O valor   */
      /* nlog n � sugerido pelos criadores da dispers�o cuco */
   maxDesalojamentos = tabela->nChaves*log2(tabela->nChaves);

      /* Tenta efetuar a inser��o numa das tabelas */
   for (i = 0; i < maxDesalojamentos; ++i) {
      /*                                  */
      /* Tenta inserir na primeira tabela */

      pos = tabela->fD1(chaveEIndice.chave, tabela->tam);

      if (tabela->tab1[pos].status == VAZIO) {
         /*                                                                */
         /* Foi encontrado um espa�o vazio na primeira tabela. Efetua-se a */
         /* inser��o e retorna-se  indicando o sucesso da opera��o         */
         /*                                                                */

            /* Insere o par chave/�ndice */
         tabela->tab1[pos].chaveEIndice = chaveEIndice;

         tabela->tab1[pos].status = OCUPADO; /* Esta posi��o passa a ser ocupada */

         ++tabela->nChaves; /* O n�mero de chaves aumentou */

         return 1;
      } else {
         /*                                                                     */
         /* N�o foi encontrado um espa�o vazio. � preciso desalojar a chave que */
         /* se encontra nessa posi��o e tentar inseri-la na segunda tabela      */
         /*                                                                     */

         aux = tabela->tab1[pos].chaveEIndice; /* Guarda chave a ser desalojada */
            /* Armazena a nova chave no lugar da chave desalojada */
         tabela->tab1[pos].chaveEIndice = chaveEIndice;
            /* Tentar-se-a inserir a chave desalojada na segunda tabela */
         chaveEIndice = aux;
      }

      /*                                  */
      /* Tenta inserir na segunda tabela */
      /*                                  */

      pos = tabela->fD2(chaveEIndice.chave, tabela->tam);

      if (tabela->tab2[pos].status == VAZIO) {
         /*                                                             */
         /* Foi encontrado um espa�o vazio na segunda tabela. Efetua-se */
         /* a inser��o e retorna-se indicando o sucesso da opera��o     */
         /*                                                             */

            /* Insere o par chave/�ndice */
         tabela->tab2[pos].chaveEIndice = chaveEIndice;

            /* Esta posi��o passa a ser ocupada */
         tabela->tab2[pos].status = OCUPADO;

         ++tabela->nChaves; /* O n�mero de chaves aumentou */

         return 1;
      } else {
         /*                                                                     */
         /* N�o foi encontrado um espa�o vazio. � preciso desalojar a chave que */
         /* se encontra nessa posi��o e tentar inseri-la na primeira tabela     */
         /*                                                                     */

            /* Guarda a chave a ser desalojada */
         aux = tabela->tab2[pos].chaveEIndice;
            /* Armazena a nova chave no lugar da chave desalojada */
         tabela->tab2[pos].chaveEIndice = chaveEIndice;
            /* Tentar-se-� inserir a chave desalojada na primeira tabela */
         chaveEIndice = aux;
      }
   }

   /*                                                                   */
   /* Se o la�o terminou sem que houvesse retorno, considera-se que     */
   /* o processo entrou em repeti��o  sem fim (de acordo com o crit�rio */
   /* especificado). Portanto, a tabela ser� reconstru�da e a chave     */
   /* restante ser� inserida nessa nova tabela.                         */
   /*                                                                   */

   RedimensionaCuco(tabela, chaveEIndice);

   return 1;
}

/****
 *
 * RemoveCuco(): Remove uma chave de uma tabela de dispers�o cuco
 *
 * Par�metros:
 *      tabela (entrada/sa�da) - a tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveCuco(tTabelaCuco *tabela, tCEP chave)
{
   int pos;

   pos = tabela->fD1(chave, tabela->tam);

   if(tabela->tab1[pos].status == OCUPADO && !strcmp(tabela->tab1[pos].chaveEIndice.chave, chave)){
      tabela->tab1[pos].status = VAZIO;
      tabela->nChaves--;

      return 1;
   }

   pos = tabela->fD2(chave, tabela->tam);

   if(tabela->tab2[pos].status == OCUPADO && !strcmp(tabela->tab2[pos].chaveEIndice.chave, chave)){
      tabela->tab2[pos].status = VAZIO;
      tabela->nChaves--;

      return 1;
   }

   return -1;
}

/****
 *
 * NChavesCuco(): Calcula o n�mero de chaves na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *
 * Retorno: O n�mero de chaves na tabela de dispers�o
 *
 ****/
int NChavesCuco(tTabelaCuco tabela)
{
   int i, nChaves = 0;

   for (i = 0; i < tabela.tam; ++i) {
      if (tabela.tab1[i].status == OCUPADO)
         ++nChaves;

      if (tabela.tab2[i].status == OCUPADO)
         ++nChaves;
   }

   ASSEGURA(tabela.nChaves == nChaves, "\nErro na contagem de numero de chaves\n");

   return nChaves;
}

/****
 *
 * NElementosVaziosCuco(): Calcula o n�mero de elementos vazios
 *                     na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *
 * Retorno: O n�mero de elementos vazios na tabela de dispers�o
 *
 ****/
int NElementosVaziosCuco(tTabelaCuco tabela)
{
   int i, n = 0;

   for (i = 0; i < tabela.tam; ++i) {
      if (tabela.tab1[i].status == VAZIO)
         ++n;

      if (tabela.tab2[i].status == VAZIO)
         ++n;
   }

   return n;
}

/****
 *
 * DestroiTabelaCuco(): Libera o espa�o ocupado por uma tabela
 *                  de dispers�o com endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaCuco(tTabelaCuco *tabela)
{
      /* Libera o espa�o ocupado por cada array */
   free(tabela->tab1);
   free(tabela->tab2);

      /* Os arrays n�o s�o mais v�lidos */
   tabela->tab1 = NULL;
   tabela->tab2 = NULL;
}
