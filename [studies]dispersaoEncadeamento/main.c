/****
 *
 * Exemplo: Dispers�o com Encadeamento
 *
 * Se��o: 7.3.2
 *
 * Descri��o:
 *    -> A tabela de busca � implementada usando dispers�o
 *       com encadeamento em que cada n� de uma lista encadeada
 *       cont�m uma chave e a posi��o do respectivo registro
 *       no arquivo
 *
 *    -> Os registros s�o mantidos em arquivo
 *
 *    -> Tipo de busca: linear
 *
 *    -> Arquivo de dados: CEPs.bin
 *
 ****/

/*********************** Includes *************************/

#include <math.h>      /* Fun��es matem�ticas   */
#include <stdlib.h>    /* Miscel�nea de fun��es */
#include <stdio.h>     /* Fun��es de Entrada/Sa�da    */
#include <string.h>    /* Fun��es memXXX() e strXXX() */
#include <time.h>      /* Medi��o de tempo         */
#include <limits.h>    /* Limites dos tipos inteiros */
#include "Macros.h"    /* Macros usadas pelo programa */
#include "Tipos.h"     /* Tipos usados pelo programa  */
#include "TabelaDE.h"  /* Fun��es que lidam com      */
                       /* dispers�o com encadeamento */
#include "Registros.h" /* Processamento de registros     */
#include "leitura.h"   /* Fun��es de leitura via teclado */
#include "Lista.h"     /* Lista encadeada */

/******************* Alus�es de Fun��es *******************/

extern unsigned int DispersaoModular(const char *chave);
extern unsigned int DispersaoPolinomial(const char *chave);
extern unsigned int DispersaoMultiplicativa(const char *str);
extern unsigned int DispersaoRotativa(const char *chave);
extern int EhPrimo(int valor);
extern int ProximoPrimo(int valor);
extern tFDispersao EscolheFuncao(void);
extern void ApresentaMenu(const char *opcoes[], int nOpcoes);
extern void AtualizaArquivoBin( const char *nomeArquivo,
                                tListaSE removidos );
extern void MedidaDeTempo(void);

/********************* Fun��es de dispers�o ********************/

/****
 *
 * DispersaoModular(): Calcula o valor de dispers�o de uma chave
 *                     convertendo-a num valor inteiro
 *
 * Par�metros:
 *      chave (entrada) - a chave que ter� seu valor de dispers�o
 *                        calculado
 *
 * Retorno: O valor dispers�o da chave recebida como par�metro
 *
 ****/
unsigned int DispersaoModular(const char * chave)
{
   return (unsigned int)atoi(chave);
}

/****
 *
 * DispersaoPolinomial(): Calcula o valor de dispers�o de uma
 *                        chave usando o m�todo polinomial
 *
 * Par�metros:
 *      chave (entrada) - a chave que ter� seu valor de
 *                        dispers�o calculado
 *
 * Retorno: O valor de dispers�o da chave recebida como par�metro
 *
 ****/
unsigned int DispersaoPolinomial(const char * chave)
{
   unsigned int dispersao = 0,
                base = 2,
                primo = 31;

   while(*chave){
      dispersao = (dispersao*base + *chave++)%primo;
   }

   return dispersao;
}

/****
 *
 * DispersaoMultiplicativa(): Calcula o valor de dispers�o de uma
 *                            chave usando o m�todo multiplicativo
 *
 * Par�metros:
 *      chave (entrada) - a chave que ter� seu valor de
 *                        dispers�o calculado
 *
 * Retorno: O valor de dispers�o da chave recebida como par�metro
 *
 ****/
unsigned int DispersaoMultiplicativa(const char * str)
{
   unsigned int dispersao = 0;
   double a = (sqrt(5) - 1)/2, pInteira, pFracionaria;
   unsigned int tamTabela = 13;


   pFracionaria = modf(atoi(str)*a, &pInteira);

   dispersao = (unsigned int) floor(tamTabela * pFracionaria);

   return dispersao;
}

/****
 *
 * DispersaoRotativa(): Calcula o valor de dispers�o de uma
 *                            chave usando o m�todo rotativo
 *
 * Par�metros:
 *      chave (entrada) - a chave que ter� seu valor de
 *                        dispers�o calculado
 *
 * Retorno: O valor de dispers�o da chave recebida como par�metro
 *
 ****/
unsigned int DispersaoRotativa(const char *chave)
{
  unsigned int dispersao = 0;
  while(*chave){
   dispersao = (dispersao << 4) ^ (dispersao >> 28) ^ (*chave);
   chave++;
  }

  return dispersao;
}

/********************* Fun��es Locais *********************/

/****
 *
 * EhPrimo(): Verifica se um n�mero inteiro � primo
 *
 * Par�metros: valor (entrada) - o n�mero que ser� verificado
 *
 * Retorno: 1, se o n�mero for primo; 0, em caso contr�rio
 *
 ****/
int EhPrimo(int valor)
{
   int metade, divisor = 2;

   if (valor < 2)
      return 0;

      /* No m�ximo, um n�mero qualquer � divis�vel por sua metade */
   metade = valor/2;

      /* Tenta encontrar um divisor do valor recebido */
   while (divisor <= metade) {
      if ( !(valor % divisor) ) /* Se foi encontrado um divisor, ... */
         return 0; /* ...o n�mero n�o � primo */

      divisor++;
   }

   return 1; /* N�o foi encontrado nenhum divisor */
}

/****
 *
 * ProximoPrimo(): Retorna o menor inteiro primo positivo que �
 *                 maior do que ou igual ao valor recebido como
 *                 par�metro
 *
 * Par�metros: valor (entrada) - o n�mero que ser� verificado
 *
 * Retorno: O menor inteiro primo positivo que � maior do que
 *          ou igual ao valor recebido como par�metro; ou 0,
 *          se o valor recebido for menor do que ou igual a 1,
 *          ou se n�o for poss�vel encontrar um n�mero primo
 *          que caiba no tipo int.
 *
 ****/
int ProximoPrimo(int valor)
{
      /* Esta fun��o n�o lida com valores */
      /* menores do que ou iguais a 1     */
   if (valor <= 1)
      return 0;

      /* Tenta encontrar um n�mero primo por meio de tentativas */
   while (valor <= INT_MAX) {
      if ( EhPrimo(valor) ) /* Se for encontrado um n�mero   */
         return valor;      /* primo, retorna-se ele pr�prio */

      valor++;
   }

      /* N�o foi encontrado nenhum n�mero */
      /* primo que caiba no tipo int      */
   return 0;
}

/****
 *
 * ConstroiTabela(): L� arquivo de dados (bin�rio) contendo
 *                   os registros e constr�i uma tabela de
 *                   dispers�o
 *
 * Par�metros:
 *      stream (entrada) - stream associado ao arquivo de
 *                         registros
 *      tamTabela (sa�da) - tamanho da tabela criada
 *
 * Retorno: Ponteiro para a tabela criada, se a fun��o
 *          for bem sucedida; NULL, em caso contr�rio
 *
 ****/
static tTabelaDE ConstroiTabela(FILE *stream, tFDispersao fDispersao,
                                int *tamTabela, int usaPrimo)
{
   tCEP_Ind conteudo;
   tRegistroCEP umRegistro;
   tTabelaDE   aTabela;
   int       pos = 0;

   if (!stream)
      return NULL;   /* Stream � nulo */

         /* Calcula o n�mero de listas na tabela */
   *tamTabela = NumeroDeRegistros(stream, sizeof(tRegistroCEP)) /
                FATOR_DE_CARGA;

      /* Se for desejado que o tamanho da tabela  seja um */
      /* n�mero primo, obt�m-se o pr�ximo n�mero primo    */
      /* maior do que o tamanho da tabela                 */
   if (usaPrimo && !EhPrimo(*tamTabela))
      *tamTabela = ProximoPrimo(*tamTabela);

      /* Aloca e inicializa a tabela */
   aTabela = CriaTabelaDE(*tamTabela);

      /* Chamando sempre rewind() antes de qualquer  */
      /* processamento sequencial pode trazer grande */
      /* economia em tempo de depura��o!             */
   rewind(stream);

      /* L� cada registro do arquivo e acrescenta */
      /* sua chave (CEP) na lista                 */
   while (1) {
         /* L� um registro no arquivo */
      fread( &umRegistro, sizeof(umRegistro), 1, stream);

         /* Verifica se ocorreu erro ou o */
         /* final do arquivo foi atingido */
      if (ferror(stream) || feof(stream)) {
         break;
      }

         /* A chave n�o � armazenada como string no registro. */
         /* Portanto, n�o se pode usar strcpy().              */
      strncpy(conteudo.chave, umRegistro.CEP, TAM_CEP);

         /* Transforma a chave num string. Isso n�o � */
         /* essencial, mas facilita o processamnto.   */
      conteudo.chave[TAM_CEP] = '\0';

         /* �ndice do registro que cont�m a chave no arquivo */
      conteudo.indice = pos++;

         /* Acrescenta a chave � tabela de dispers�o */
      InsereDE(aTabela, *tamTabela, &conteudo, fDispersao);
   }

      /* Se ocorreu erro de leitura no */
      /* arquivo aborta o programa     */
   if (ferror(stream)) {
      ASSEGURA(0, "Erro de leitura de arquivo ");
   }

   return aTabela;
}

/****
 *
 * EscolheFuncao(): Apresenta um menu de fun��es de dispers�o
 *                      alternativas e permite que o usu�rio
 *                      escolha uma delas
 *
 * Par�metros: Nenhum
 *
 * Retorno: Ponteiro para a fun��o de dispers�o escolhida.
 *
 ****/
tFDispersao EscolheFuncao(void)
{
   const char* opcoes[] = { "Dispersao Modular",
                            "Dispersao Polinomial",
                            "Dispersao Multiplicativa",
                            "Dispersao Rotativa"
                          };
   int         op,
               nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]);

   printf("\n\t>>> Escolha da Funcao de dispersao <<<\n");

   ApresentaMenu(opcoes, nOpcoes);
   op = LeOpcao("1234");

   switch (op) {
      case '1': return DispersaoModular;
      case '2': return DispersaoPolinomial;
      case '3': return DispersaoMultiplicativa;
      case '4': return DispersaoRotativa;
   }

   return DispersaoModular; /* Para agradar o compilador */
}

/****
 *
 * ApresentaMenu(): Apresenta um menu de op��es para o usu�rio
 *
 * Par�metros:
 *      opcoes[] (entrada) - array de strings contendo as
 *                           op��es do usu�rio
 *      nOpcoes (entrada) - o n�mero de op��es
 *
 * Retorno: Nada
 *
 ****/
void ApresentaMenu(const char *opcoes[], int nOpcoes)
{
   int i;

   printf("\nEscolha uma das opcoes a seguir:\n");

   for (i = 0; i < nOpcoes; ++i)
      printf("\n\t(%d) %s", i + 1, opcoes[i]);

   printf("\n\n\t>>> ");
}

/****
 *
 * AtualizaArquivoBin(): Cria um novo arquivo bin�rio contendo
 *                       cada registro de outro arquivo bin�rio
 *                       cuja chave n�o fa�a parte de uma lista
 *                       de registros que devem ser removidos
 *
 * Par�metros:
 *      nomeArquivo (entrada) - nome do arquivo bin�rio original
 *      removidos (entrada) - lista contendo as estruturas que
 *                            N�O ser�o escritas no novo arquivo
 *
 * Retorno: Nenhum
 *
 ****/
void AtualizaArquivoBin( const char *nomeArquivo,
                         tListaSE removidos )
{
   char      ar[L_tmpnam]; /* Nome de um arquivo tempor�rio */
   FILE     *stream, /* Stream associado ao arquivo original */
            *streamTmp; /* Stream associado ao tempor�rio */
   tRegistroCEP umRegistro; /* Um registro do arquivo */
   tCEP    umaChave;
   int       ocorreuErro = 0; /* Indicar� se ocorreu erro */

      /* Tenta abrir o arquivo original para leitura */
   ASSEGURA( stream = fopen(nomeArquivo, "rb"),
             "Nao foi possivel atualizar arquivo" );

      /* Obt�m o nome de um arquivo tempor�rio */
   tmpnam(ar);

      /* Tenta abrir o arquivo tempor�rio */
      /* para escrita em modo bin�rio     */
   ASSEGURA( streamTmp = fopen(ar, "wb"),
             "Nao foi possivel criar arquivo temporario" );

      /* O la�o encerra quando n�o for */
      /* mais poss�vel ler um registro */
   while (1) {
         /* L� um registro no arquivo de entrada */
      fread(&umRegistro, sizeof(umRegistro), 1, stream);

         /* Testa se o final do arquivo de entrada  */
         /* foi atingido ou ocorreu erro de leitura */
      if (feof(stream) || ferror(stream)) {
         break; /* Processamento encerrado */
      }

         /* Cria uma chave (string). O campo CEP */
         /* do registro n�o � string.            */
      strncpy(umaChave, umRegistro.CEP, TAM_CEP);
      umaChave[TAM_CEP] = '\0';

         /* Se a chave do registro lido n�o faz parte da  */
         /* lista de removidos, escreve-o no novo arquivo */
      if (BuscaListaSE(&removidos, umaChave) < 0) {
            /* Escreve o registro lido no arquivo tempor�rio */
         fwrite(&umRegistro, sizeof(umRegistro), 1, streamTmp);

            /* Verifica se ocorreu erro de escrita */
         if (ferror(streamTmp)) {
            break;
         }
      }
   }

         /* Verifica se ocorreu erro de leitura ou escrita */
   if ( ferror(stream) || ferror(streamTmp) ) {
      ocorreuErro = 1;
   }

      /* Fecha os arquivos */
   fclose(stream);
   fclose(streamTmp);

   ASSEGURA(!ocorreuErro, "Erro na atualizacao do arquivo");

   ASSEGURA( !remove(nomeArquivo),
             "Impossivel remover o arquivo antigo" );

      /* Tenta rebatizar o arquivo tempor�rio */
      /* com o nome do arquivo original       */
   ocorreuErro = rename(ar, nomeArquivo);

   if (ocorreuErro) {
      printf( "\n\t>>> O resultado da atualizacao se encontra"
              "no arquivo \"%s\". Nao foi possivel "
              "rebatiza-lo.\n", ar );
      ASSEGURA( 0, "Erro em batismo de arquivo" );
   }
}

/****
 *
 * MedidaDeTempo(): Mede intervalos de tempo entre duas
 *                  chamadas consecutivas
 *
 * Par�metros: Nenhum
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o deve ser chamada aos pares; i.e.,
 *             na primeira chamada ela marca o in�cio da
 *             contagem de tempo e na segunda chamada ela
 *             apresenta o intervalo de tempo decorrido
 *             desde a �ltima chamada
 *
 ****/
void MedidaDeTempo(void)
{
   static int    numeroDeChamadas = 0;
   static time_t t0; /* Instante inicial */
   time_t        tf; /* Instante final   */

      /* Esta fun��o foi chamada mais uma vez */
   ++numeroDeChamadas;

      /* Se o n�mero da chamada for �mpar, esta */
      /* chamada � a primeira de um par         */
   if (numeroDeChamadas%2) { /* Calcula o instante inicial */
      t0 = time(NULL);
   } else { /* Calcula e apresenta o intervalo de   */
            /* tempo entre as duas �ltimas chamadas */
      tf = time(NULL);

      printf( "\n\n\t*** Tempo gasto na operacao: "
              "%5.2f segundos ***\n", difftime(tf, t0) );
   }
}

/********************* Fun��o main() **********************/

/****
 *
 * main(): Testa fun��es de busca, inser��o, remo��o e
 *         atualiza��o usando dispers�o com encadeamento
 *
 * Par�metros: nenhum
 *
 * Retorno: 0
 *
 ****/
int main(void)
{
   tRegistroCEP    umRegistro;
   const char*  opcoes[] = { "Estatisticas do banco de dados",
                             "Numero de CEPs em cada lista",
                             "Consulta dados de um CEP",
                             "Acrescenta um CEP",
                             "Remove um CEP",
                             "Recomecar",
                             "Encerra o programa"
                           };
   tTabelaDE      tabela;
   char         umCEP[TAM_CEP + 1]; /* Uma chave de busca */
   tCEP_Ind    conteudo;
   FILE        *stream,
               *streamChavesEmListas;
   tFDispersao   fDispersao;
   int          op, pos,
                tamTabela, nChaves, nListas,
                nOpcoes = sizeof(opcoes)/sizeof(opcoes[0]),
                tamanhoEhPrimo = 0,
                nRegistros,
                primeiroByte, /* Indica a posi��o do */
                              /* primeiro byte de um */
                              /* registro no arquivo */
                atualizarArq = 0; /* Indica se o arquivo */
                                  /* de registros ser�   */
                                  /* atualizado          */
   tListaSE    removidos; /* Chaves dos registros a ser  */
                          /* removidos quando o arquivo  */
                          /* de registros for atualizado */

      /* Tenta abrir o arquivo de dados */
   stream = fopen(NOME_ARQUIVO_BIN, "r+b");

      /* Garante que o arquivo foi aberto */
   ASSEGURA(stream,
            "\nImpossivel abrir arquivo de registros\n");

recomeco:
      /* Escolhe a fun��o de dispers�o */
   fDispersao = EscolheFuncao();

   /*                                                        */
   /* Verifica se o o tamanho da tabela ser� um n�mero primo */
   /*                                                        */

   printf("\n\n\t>>> Deseja que o tamanho da tabela "
          "seja primo (S/N)? ");
   op = LeOpcao("SsNn");

   if (op == 's' || op == 'S')
      tamanhoEhPrimo = 1;

   MedidaDeTempo();
   printf("\nCriando tabela...");

      /* L� o arquivo e cria a tabela de dispers�o */
   tabela = ConstroiTabela( stream, fDispersao,
                            &tamTabela, tamanhoEhPrimo );

      /* Certifica-se que a cria��o da tabela foi OK */
   ASSEGURA(tabela, "\nNao foi possivel criar a tabela\n");

   printf("\n...tabela criada com sucesso\n");
   MedidaDeTempo();

      /* Obt�m o n�mero de registros do arquivo */
   nRegistros = NumeroDeRegistros( stream,
                                   sizeof(umRegistro) );

      /* Inicia a lista que armazena os �ndices */
      /* dos registros removidos                */
   IniciaListaSE(&removidos);

      /* O la�o a seguir encerra quando o usu�rio */
      /* escolher a op��o de encerramento         */
   while (1) {
         /* Apresenta menu de op��es do programa */
         /* e l� a op��o do usu�rio              */
      ApresentaMenu(opcoes, nOpcoes);
      op = LeOpcao("1234567");

         /* Verifica se o usu�rio quer encerrar o programa */
      if (op == '7') {
            /* Fecha o arquivo que cont�m os registros */
         fclose(stream);

            /* Libera o espa�o ocupado pela tabela */
         DestroiTabelaDE(&tabela, tamTabela);

         break; /* Encerra o la�o*/
      }

      switch (op) {
         case '1': /* Estatisticas do banco de dados */
            printf("\n\n>>> Estatisticas do Banco "
                   "de Dados <<<\n");

            printf( "\n\t>>> Numero de registros no arquivo: %d",
                    nRegistros );

            printf( "\n\t>>> Fator de carga da tabela: %d",
                    FATOR_DE_CARGA );

            nChaves = NChavesDE(tabela, tamTabela);
            printf( "\n\t>>> Numero de chaves na tabela: %d",
                    nChaves );

            printf( "\n\t>>> Numero total de listas: %d",
                    tamTabela );

            nListas = NListasNaoVaziasDE( tabela, tamTabela );
            printf( "\n\t>>> Numero de listas nao "
                    "vazios: %d", nListas );

            printf( "\n\t>>> Numero de listas vazios: %d",
                    tamTabela - nListas );

            printf( "\n\t>>> Numero medio de chaves por "
                    "lista: %d", nChaves/tamTabela );

            break;

         case '2': /* Numero de CEPs em cada lista */
               /* Cria arquivo que conter� o resultado */
            streamChavesEmListas = fopen(NOME_ARQ_CHAVES, "w");

               /* Certifica-se que o arquivo foi aberto */
            ASSEGURA( streamChavesEmListas,
                      "\nArquivo nao pode ser aberto\n" );

            NChavesEmListasDE( tabela, tamTabela,
                             streamChavesEmListas);
            printf( "\n\t>>> O numero de chaves em "
                    "cada lista foi escrito no arquivo: %s",
                    NOME_ARQ_CHAVES);
            fclose(streamChavesEmListas);
            break;

         case '3': /* Consulta dados de um CEP */
               /* L� um CEP introduzido pelo usu�rio */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Procura o CEP na tabela de busca */
            pos = BuscaDE(tabela, tamTabela, umCEP, fDispersao);

               /* Se a chave for encontrada, utiliza-se */
               /* seu �ndice para recuperar o registro  */
               /* correspondente no arquivo             */
            if (pos < 0) {
               printf("\n>>> CEP nao foi encontrado\n");
            } else {
                  /* Utilizando o �ndice do registro, */
                  /* calcula-se a posi��o no arquivo  */
                  /* do primeiro byte do registro     */
               primeiroByte = pos*sizeof(tRegistroCEP);

                  /* Move apontador de posi��o para o     */
                  /* primeiro byte do registro. Se isso   */
                  /* n�o for poss�vel, aborta o programa. */
               ASSEGURA( !fseek(stream, primeiroByte, SEEK_SET),
                         "Erro de posicionamento em arquivo" );

                  /* L� o registro no arquivo */
               fread(&umRegistro, sizeof(umRegistro), 1, stream);

                  /* Se ocorreu erro de leitura aborta */
               ASSEGURA(!ferror(stream), "Erro de leitura");

                  /* Apresenta o registro ao usu�rio */
               ExibeRegistro(&umRegistro);

               putchar('\n'); /* Embelezamento */
            }

            break;

         case '4': /* Acrescenta um CEP */
               /* L� dados do novo CEP */
            LeRegistro(&umRegistro);

               /* O valor do campo 'numero' corresponde � */
               /* posi��o do registro no arquivo de CEPs  */
               /* (isso � coisa dos Correios)             */
            umRegistro.numero = nRegistros + 1;

               /* Cria a chave a ser inserida na tabela de */
               /* busca. Na tabela, a chabe � um string    */
               /* mas no registro CEP n�o � string         */
            strncpy(conteudo.chave, umRegistro.CEP, TAM_CEP);

               /* Torna a chave um string */
            conteudo.chave[TAM_CEP] = '\0';

            conteudo.indice = nRegistros + 1;

               /* Acrescemta novo elemento � tabela de busca */
            InsereDE( tabela, tamTabela, &conteudo, fDispersao );

               /* Move apontador de posi��o para o final */
               /* do arquivo. Se isso n�o for poss�vel,  */
               /* aborta o programa.                     */
            ASSEGURA( !fseek(stream, 0, SEEK_END),
                      "Erro de posicionamento em arquivo");

               /* Escreve o novo registro no final do arquivo */
            fwrite(&umRegistro, sizeof(umRegistro), 1, stream);

               /* Se ocorreu erro de escrita, aborta */
            ASSEGURA(!ferror(stream),
                     "Erro de escrita em arquivo ");

            ++nRegistros; /* Mais um registro */

            printf("\n>>> Acrescimo bem sucedido\n");

            break;

         case '5': /* Remove um CEP */
               /* L� um CEP introduzido pelo usu�rio */
            LeMatricula("CEP", umCEP, TAM_CEP + 1);

               /* Tenta remover o CEP da tabela de busca */
            pos = RemoveDE(tabela, tamTabela, umCEP, fDispersao);

               /* Se a chave foi removida acrescenta-se   */
               /* um novo elemento na lista de removidos. */
               /* A remo��odo registro do arquivo s� ser� */
               /* efetuada ao final do programa.          */

            if (pos >= 0) { /* O CEP foi removido */
                  /* Cria um novo elemento e acrescenta-o */
                  /* � lista de removidos                 */
               strcpy(conteudo.chave, umCEP);
               InsereListaSE(&removidos, &conteudo);

                  /* Arquivo de registros precisar� */
                  /* ser atualizado                 */
               atualizarArq = 1;

               printf("\n>>> Remocao bem sucedida\n");
            } else { /* O CEP n�o se encontra na tabela */
               printf("\n>>> CEP nao encontrado\n");
            }

            break;

         case '6': /* Remove um CEP */
               /* Libera o espa�o ocupado pela tabela */
            MedidaDeTempo();
            printf("\nDestruindo tabela...");
            DestroiTabelaDE(&tabela, tamTabela);
            printf("\n...tabela destruida com sucesso\n");
            MedidaDeTempo();

            tamanhoEhPrimo = 0;

            atualizarArq = 0;

            DestroiListaSE(&removidos);

            goto recomeco;

            break;

         default: /* O programa n�o deve chegar at� aqui */
            printf("\nEste programa contem um erro\n");
            return 1;
      } /* switch */
   } /* while */

      /* Atualiza arquivo bin�rio se for necess�rio */
   if (atualizarArq) {
      MedidaDeTempo();
      printf("\nAtualizando arquivo...");

         /* Atualiza arquivo bin�rio */
      AtualizaArquivoBin(NOME_ARQUIVO_BIN, removidos);

      printf("\n... Atualizacao bem sucedida");
      MedidaDeTempo();
   }

   printf("\nObrigado por usar este programa. Bye.\n");

   return 0;
}

/******************* Exemplo de Execu��o ******************


**********************************************************/
