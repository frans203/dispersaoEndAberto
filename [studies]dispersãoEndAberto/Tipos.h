/****
 *
 * Arquivo Tipos.h
 *
 * Autor: Ulysses de Oliveira
 *
 * Data de Cria��o: 07/06/2010
 * �ltima modifica��o: 07/06/2010
 *
 * Descri��o: Tipos utilizados pelo programa de dispers�o com endere�amento aberto
 *
 ****/

#ifndef _Tipos_H_ /* Previne inclus�o m�ltipla deste arquivo */
#define _Tipos_H_

#include "Macros.h"  /* Macros usadas pelo programa */

typedef char tCEP[TAM_CEP + 1]; /* Tipo da chave */

   /* Tipo dos pares chave/�ndice */
typedef struct  {
           tCEP chave;  /* Chave de um registro */
           int  indice; /* �ndice do registro no arquivo */
        } tCEP_Ind;

   /* Tipo usado para indicar o status */
   /* de um elemento da tabela         */
typedef enum {VAZIO, OCUPADO, REMOVIDO} tStatusDEA;

   /* Tipo dos elementos da tabela de busca */
typedef struct {
           tCEP_Ind   chaveEIndice;
           tStatusDEA  status;
        } tColetorDEA, *tTabelaDEA;

   /* Tipo de um ponteiro para uma fun��o de */
   /* dispers�o com endere�amento aberto     */
typedef unsigned (*tFDispersaoDEA) (tCEP, unsigned, unsigned);

   /* Tipo usado para armazenamento de */
   /* registros no arquivo bin�rio     */
typedef struct {
           int  numero;
           char UF[TAM_UF];
           int  localidadeNumero;
           char nomeAbr[MAX_NOME];
           char nome[MAX_NOME];
           int  bairroInicio;
           int  bairroFim;
           char CEP[TAM_CEP];
           char complemento[MAX_COMP];
           char tipoLogradouro[MAX_TIPO_LOG];
           char statusTipoLogradouro;
           char nomeSemAcento[MAX_NOME];
           char chaveDNE[TAM_DNE];
         } tRegistroCEP;

#endif
