/****
 *
 * Arquivo TabelaDEA.c
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
#include "TabelaDEA.h"  /* Interface deste m�dulo      */
#include "Registros.h"  /* Processamento de registros  */

/***************** Defini��es de Fun��es ******************/

/****
 *
 * RedimensionaTabDEA(): Redimensiona uma tabela de dispers�o com
 *                       endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o atual
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o atual
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o
 *                             a ser usada
 *
 * Retorno: Endere�o da nova tabela
 *
 * Observa��o: O redimensionamento sempre dobra o tamanho atual da
 *             tabela
 *
 ****/
static tTabelaDEA RedimensionaTabDEA(tTabelaDEA tabela, int tamanhoTab, tFDispersaoDEA fDispersao)
{
  tTabelaDEA tabelaNova;
  int tamanhoNovo = tamanhoTab * 2;
  int i;

  tabelaNova = calloc(tamanhoNovo, sizeof(tColetorDEA));

  for(i =0; i<tamanhoNovo; i++){
   tabelaNova[i].status = VAZIO;
  }

  for(i=0;i<tamanhoTab;i++){
   if(tabela[i].status == OCUPADO){
      InsereDEA(tabelaNova, tamanhoNovo, tabela[i].chaveEIndice, fDispersao);
   }
  }
  free(tabelaNova);
  return tabelaNova;
}

/****
 *
 * CriaTabelaDEA(): Cria e inicializa uma tabela de dispers�o
 *                  com endere�amento aberto
 *
 * Par�metros:
 *      nElementos (entrada) - n�mero de posi��es da tabela
 *                             de dispers�o
 *
 * Retorno: A tabela de dispers�o criada
 *
 ****/
tTabelaDEA CriaTabelaDEA(int nElementos)
{
   tTabelaDEA  tabela;
   int         i;

   tabela = calloc(nElementos, sizeof(tColetorDEA));
   ASSEGURA( tabela, "Impossivel alocar a tabela de dispersao" );

   for (i = 0; i < nElementos; ++i)
         /* Todos os elementos est�o inicialmente desocupados */
      tabela[i].status = VAZIO;

   return tabela;
}

/****
 *
 * BuscaDEA(): Executa uma busca simples numa tabela de dispers�o
 *             com endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de
 *                             dispers�o a ser usada
 *
 * Retorno: �ndice do registro no arquivo de registros,
 *          se a chave for encontrada; -1, em caso contr�rio
 *
 ****/
int BuscaDEA( tTabelaDEA tabela, int tamanhoTab, tCEP chave, tFDispersaoDEA fDispersao )
{
   int i, pos;

   for(i = 0; i<tamanhoTab;i++){
      pos = fDispersao(chave, i, tamanhoTab);

      if(tabela[pos].status == VAZIO || tabela[pos].status == REMOVIDO){
         return -1;
      }

      if (tabela[pos].status == OCUPADO && memcmp(tabela[pos].chaveEIndice.chave, chave, sizeof(tCEP))){
         return tabela[pos].chaveEIndice.indice; //retornamos o indice do arquivo onde esta o endereço associado com o cep
      }
   }

   return -1;
}

/****
 *
 * InsereDEA(): Faz inser��o numa tabela de dispers�o com
 *              endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chaveEIndice (entrada) - a chave de busca e seu respectivo �ndice
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o a ser usada
 *
 * Retorno: 1, se houver inser��o; 0, em caso contr�rio
 *
 ****/
int InsereDEA( tTabelaDEA tabela, int tamanhoTab,
               tCEP_Ind chaveEIndice, tFDispersaoDEA fDispersao )
{
   int i, pos, posInsercao = -1;

   for(i=0;i<tamanhoTab;i++){
      pos = fDispersao(chaveEIndice.chave, i, tamanhoTab);

      if(tabela[pos].status == REMOVIDO && posInsercao < 0){
         posInsercao = pos;
         break;
      }

      if(tabela[pos].status == VAZIO && posInsercao < 0){
         posInsercao = pos;
         break;
      }

      if(tabela[pos].status == OCUPADO 
      && !memcmp(tabela[pos].chaveEIndice.chave, chaveEIndice.chave, sizeof(tCEP))){
         return 0;
      }
   }

   tabela[posInsercao].chaveEIndice = chaveEIndice;
   tabela[posInsercao].status = OCUPADO;

   return 1;
}

/****
 *
 * RemoveDEA(): Remove uma chave de uma tabela de dispers�o com
 *              endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *      chave (entrada) - a chave de busca
 *      fDispersao (entrada) - ponteiro para a fun��o de dispers�o a ser usada
 *
 * Retorno: 1, se a remo��o foi ok; 0, caso contr�rio
 *
 ****/
int RemoveDEA( tTabelaDEA tabela, int tamanhoTab, tCEP chave,
               tFDispersaoDEA fDispersao )
{
  int i, pos;

  for(i=0;i<tamanhoTab;i++){
   pos = fDispersao(chave, i, tamanhoTab);

   if(tabela[pos].status == VAZIO || tabela[pos].status == REMOVIDO){
      return 0; // chave não encontrada
   }

   if(tabela[pos].status == OCUPADO && !memcmp(tabela[pos].chaveEIndice.chave, chave, sizeof(tCEP))){
      tabela[pos].status = REMOVIDO;
      return 1;
   }
  }
  return 0;
}

/****
 *
 * NChavesDEA(): Calcula o n�mero de chaves na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de chaves na tabela de dispers�o
 *
 ****/
int NChavesDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, nChaves = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == OCUPADO)
         ++nChaves;

   return nChaves;
}

/****
 *
 * NColetoresVaziosDEA(): Calcula o n�mero de elementos vazios
 *                     na tabela de dispers�o
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de elementos vazios na tabela de dispers�o
 *
 ****/
int NColetoresVaziosDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, n = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == VAZIO)
         ++n;

   return n;
}

/****
 *
 * NChavesRemovidasDEA(): Determina o n�mero de chaves que foram removidas
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamanhoTab (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: O n�mero de conteiners que n�o s�o vazios
 *
 ****/
int NChavesRemovidasDEA(tTabelaDEA tabela, int tamanhoTab)
{
   int i, n = 0;

   for (i = 0; i < tamanhoTab; ++i)
      if (tabela[i].status == REMOVIDO)
         ++n;

   return n;
}

/****
 *
 * DestroiTabelaDEA(): Libera o espa�o ocupado por uma tabela
 *                  de dispers�o com endere�amento aberto
 *
 * Par�metros:
 *      tabela (entrada) - a tabela de dispers�o
 *      tamTabela (entrada) - tamanho da tabela de dispers�o
 *
 * Retorno: Nada
 *
 ****/
void DestroiTabelaDEA(tTabelaDEA *tabela)
{
      /* Libera o espa�o ocupado pelo array */
   free(*tabela);

   *tabela = NULL;
}
