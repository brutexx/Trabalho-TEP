/**********************************************************************
* Baseado no código disponibilizado em
*  Application Note: "Object-Oriented Programming in C"
* https://www.state-machine.com/doc/AN_OOP_in_C.pdf
*
* Aqui se implementa o número do tipo "racional"
*
* numero.h definiu a interface do "número_t virtual"
* que é usada aqui
* ********************************************************************/
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "matriz.h"

/*------------------------------------------------------------------*
 *  IMPLEMENTACAO DAS FUNCOES VIRTUAIS DE "COMPLEXO"                *
 * Uma função virtual é uma função que é declarada (mas não         *
 * implementada em uma classe base (NO CASO, NUMERO) e redefinida   *
 * pela classe derivada (NO CASO, COMPLEXO).                        *
 * Para declarar uma função como sendo virtual, é preciso associá-la*
 * com uma entrada da tabela vtbl da classe base.                   *
 * A redefinição da função na classe derivada sobrepõe a definição  *
 * da função na classe base.                                        *
 *                                                                  *
 * No fundo, a declaração da função virtual na classe base age      *
 * como uma espécie de indicador que especifica uma linha geral de  *
 * ação e estabelece uma interface de acesso.                       *
 *                                                                  *
 * A redefinição da função virtual pela classe derivada especifica  *
 * verdadeiramente IMPLEMENTA as operações realmente executadas     *
 * por cada método da tabela.                                       *
 *                                                                  *
 * Quando acessadas normalmente, funções virtuais se comportam como *
 * qualquer outro tipo de função membro da classe. Entretanto, o que*
 * torna funções virtuais importantes e capazes de suportar         *
 * polimorfismo em tempode execução é o seu modo de comportamento   *
 * quando acessado através de um apontador.                         *
 *                                                                  *
 * Lembre-se que um apontador para a classe base pode ser usado     *
 * para apontar para qualquer classe derivada daquela classe base,  *
 * desde que corretamente convertido ("cast").                      *
 *                                                                  *
 * Quando um apontador base aponta para um objeto derivado que      *
 * contém uma função virtual, o programa irá determina qual versão  *
 * daquela função chamar baseada no tipo do objeto apontado pelo    *
 * apontador.                                                       *
 *                                                                  *
 * Assim, quando objetos diferentes são apontados, versões          *
 * diferentes da função virtual são executadas.                     *
 * -----------------------------------------------------------------*/

/*------------------------------------------------------------------*
 * PROTOTIPOS DA IMPLEMENTACAO DAS FUNCOES VIRTUAIS                 *
 * note o "_" (underline) no final dos nomes das funções            *
 * Trata-se de uma convenção que usamos para nos lembrar que estas  *
 * funções são "static". Isto é: são declaradas e definidas pelo    *
 * compilador de tal forma que são reconhecidas apenas aqui dentro  *
 * deste arquivo  ____.c --> _______.o                              *
 * Estas funções não podem ser chamadas PELO NOME para serem        *
 * executadas por outros trechos de código em outros arquivos .c    *
 * Todavia, podem ser chamadas para serem executadas através de seus*
 * ENDEREÇOS. E são os endereços destas funções que são atribuídos  *
 * às respectivas posições da tabela vtbl que é apontada pelo campo *
 * super. O campo super a que se refere aqui é o PRIMEIRO campo da  *
 *                                                                  *
 * -----------------------------------------------------------------*/
static  Numero_pt copia_ (Numero_t const * const  me);

static  Numero_pt atribui_ (Numero_t const * const  me,
							              Numero_t       * const  outro) ;

static  Numero_pt soma_  (Numero_t const * const  me,
								          Numero_t const * const  outro,
							            Numero_t       * const  res);

static  Numero_pt subt_  (Numero_t const * const  me,
									        Numero_t const * const  outro,
									        Numero_t       * const  res);

static  Numero_pt mult_  (Numero_t const * const  me,
									        Numero_t const * const  outro,
									        Numero_t       * const  res);

static  Numero_pt divd_  (Numero_t const * const  me,
									        Numero_t const * const  outro,
									        Numero_t       * const  res);

static  Numero_pt ac_soma_ (Numero_t       * const  me,
									          Numero_t const * const  outro);

static  Numero_pt ac_subt_ (Numero_t       * const  me,
									          Numero_t const * const  outro);

static  Numero_pt ac_mult_ (Numero_t       * const  me,
									          Numero_t const * const  outro);

static  Numero_pt ac_divd_ (Numero_t       * const  me,
									          Numero_t const * const  outro);

static  int	compara_ 	(Numero_t const * const  me,
							         Numero_t const * const  outro);

static  char * imprime_  (Numero_t const * const  me);

static  void destroi_ (Numero_t * me);


/* ------------------------------------------------------- *
 * funções que são típicas de matrizes, mas não são *
 * funcoes da classe virtual básica número                 *
 * protótipos das funçoes get e set, por exemplo           *
 *---------------------------------------------------------*/
static double Get_ (Matriz_t const * const me,
                    int                    lin,
                    int                    col);


static inline
void Set_ (Matriz_t * const me,
           int              lin,
           int              col,
           double           val);

static inline
int Qtd_linhas_ (Matriz_t * const me);

static inline
int Qtd_colunas_ (Matriz_t * const me);

static Matriz_pt Resize_  (Matriz_t       *        me,
								           int            *        tam);

static Matriz_pt Ones_ (Matriz_t      * const me, 
                        int           *       tam);

static Matriz_pt Identidade_ (Matriz_t      * const me, 
                              int           *       tam);

static
Matriz_pt Multip_escalar_ (Matriz_t      * const me, 
                           double                num);

static
Matriz_pt Dot_ (Matriz_t const * const me, 
                Matriz_t const * const outro);

static void Transpor_ (Matriz_t      * const me);

static void Transpor_diag2_ (Matriz_t      * const me);

static void Reverse_horizontal_ (Matriz_t      * const me);

static void Reverse_vertical_ (Matriz_t      * const me);

static inline
void Acrescenta_linha_ (Matriz_t      * const me);

static inline
void Acrescenta_coluna_ (Matriz_t      * const me);

 /*---------------------------------------------*
 * implementação do construtor                  *
 * ---------------------------------------------*/
Matriz_pt Matriz_2d_criar (Matriz_pt  me,
              						 int       *tam,
              						 int       *val)
{
	/* tabela de funções virtuais da classe Numero_t  *
	 * Esta tabela estática será compartilhada por todos os números *
	 * da classe Matriz_t                                        */

    static struct NumeroVtbl const vtbl = {
        &copia_,
        &atribui_,
        &soma_,
        &subt_,
        &mult_,
        &divd_,
        &ac_soma_,
        &ac_subt_,
        &ac_mult_,
        &ac_divd_,
        &compara_,
        &imprime_,
        &destroi_
     };

         me = (Matriz_pt) Num_constroi ((Numero_pt) me);
                        /*constroi o Numero_t  */
                        /* no início de Matriz_t  */

         me->super.metodo = &vtbl;
            /* metodo aponta para vtbl de Matriz_t */
            /* as operações do "numero", a partir de agora,     */
            /* são as operações sobre double                    */

     /* Agora, mais uma tabela estática a ser compartilhada pelos     *
      * "Matriz_t": a tabela de interface                          *
      * note que a estrutura Interface incorpora os métodos Get e Set */
     static struct Matriz_Interface_st const interface = {
        &Get_,
        &Set_,
        &Resize_,
        &Ones_,
        &Identidade_,
        &Multip_escalar_,
        &Dot_,
        &Transpor_,
        &Transpor_diag2_,
        &Reverse_horizontal_,
        &Reverse_vertical_,
        &Acrescenta_linha_,
        &Acrescenta_coluna_,
     };

     me->Metodo = &interface;
            /* metodo aponta para vtbl de Matriz_t */
            /* as operações do "numero", a partir de agora,     */
            /* são as operações sobre double                    */

    /* aloca dinamicamente uma area de memoria para um double * */
    /* e atribui o endereço de memória alocada para o ponteiro */
    /* valor que está dentro da estrutura Matriz_st         */
	me->valor = (double **) malloc (tam[0]*sizeof(double *));
	if (me->valor == NULL)
	{	/*erro!!! não conseguiu alocar */
	    printf ("Erro na alocação de memória em Matriz_2d_criar");
	    printf ("Nao alocou o valor double *");
	    exit (1);
	} //Primeira dimensão alocada!
  
  for (int i = 0; i < tam[0]; i++) {
    
    me->valor[i] = (double *) malloc (tam[1]*sizeof(double));
	  if (me->valor == NULL)
	  {	/*erro!!! não conseguiu alocar */
	    printf ("Erro na alocação de memória em Matriz_2d_criar");
	    printf ("Nao alocou o valor double");
	    exit (1);
	  }
    
  } //Matriz 2d alocada!

	/* copia o double passado como parâmetro */
    /* no endereco de memoria recém alocado  */
  me->tam = tam;

  //Inserir os valores dentro da matriz
  for (int l = 0; l < tam[0]; l++) {
    for (int c = 0; c < tam[1]; c++) {
      Set_(me, l, c, val[l*tam[1] + c]); //l*tam[1] + c  viaja entre linhas corretamente
    }
  } //Matriz preenchida.

  return (me);

	/* ------------------------------------------------------------
	 * quando implementarmos o "meu_float", valor apontará para float
     * quando implementarmos o racional,   valor apontará para
     * um vetor com dois "long int"
     * quando implementarmos o complexo,   valor apontará para
     * um vetor com dois "double"
     * quando implementarmos o quaternion, valor apontará para
     * um vetor com quatro "double"
     * Por isso, cada tipo específico de número terminará de implementar
     * o seu construtor....
     *---------------------------------------------------------------*/
}


 /*---------------------------------------------*
 * implementação do set e get                   *
 * ---------------------------------------------*/

static inline 
double Get_ (Matriz_t const * const me,
             int                    lin,
             int                    col)
{
  if (me->tam[0] <= lin || me->tam[1] <= col)
    return 0;
  
  return me->valor[lin][col];
}


static inline
void Set_ (Matriz_t * const me,
           int              lin,
           int              col,
           double           val)
{
	me->valor[lin][col] = val;
}

static inline
int Qtd_linhas_ (Matriz_t * const me)
{
	return me->tam[0];
}

static inline
int Qtd_colunas_ (Matriz_t * const me)
{
	return me->tam[1];
}

static Matriz_pt Resize_  (Matriz_t       *        me,
								           int            *        tam)
{
  double * val = (double *) malloc(tam[0]*tam[1]*sizeof(double));

  //Inserir os valores dentro de val
  for (int l = 0; l < tam[0]; l++) //Quantidade nova de linhas
  { 
    for (int c = 0; c < tam[1]; c++) //Quantidade nova de colunas
    { 
      
      int ind = l*tam[1] + c; //l*tam[1] + c  viaja entre linhas corretamente
      val[ind] = Get_(me, l, c);
      
    }
  } //Matriz preenchida.
  
  me = Matriz_2d_criar(me, tam, val); //Bom.. isso funciona
  free(val);
  
  return me;
}

static Matriz_pt Ones_ (Matriz_t      * const me, 
                        int           *       tam)
{
  //Inserir "1" em todas as posicoes de "me"
  for (int l = 0; l < tam[0]; l++) 
  {
    for (int c = 0; c < tam[1]; c++) 
    {
      
      Set_(me, l, c, 1);
      
    }
  } //Matriz alterada.

  return me;
}

static Matriz_pt Identidade_ (Matriz_t      * const me, 
                              int           *       tam) 
{

  //Inserir "1" e "0" nas posicoes de "me"
  for (int l = 0; l < tam[0]; l++) 
  {
    for (int c = 0; c < tam[1]; c++) 
    {
      
      if (l == c) Set_(me, l, c, 1);
      else Set_(me, l, c, 0);
      
    }
  } //Matriz alterada.

  return me;
}

static
Matriz_pt Multip_escalar_ (Matriz_t      * const me, 
                           double                num)
{
  //Multiplicar todas as posicoes por um escalar
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_Colunas_(me); c++) 
    {

      double valor = Get_(me, l, c) * num;
      Set_(me, l, c, valor);
      
    }
  } //Matriz alterada.

  return me;
}

static
Matriz_pt Dot_ (Matriz_t const * const me, 
                Matriz_t const * const outro)
{
  //Nessa operação parece fazer sentido ter uma matriz-resultado
  int linha = Qtd_Linhas_(me), coluna = Qtd_Colunas_(outro);

  int tamanho[2] = {linha, coluna};
  double placeholder[linha][coluna]; //Para não faltar aqui ↓
  Matriz_pt res = Matriz_2d_criar(res, tamanho, placeholder);

  //Hora de multiplicar matrizes!
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_Colunas_(outro); c++) 
    {
      double valor = 0;
      
      //Agora é a hora da multiplicação linha vs coluna
      for (int i = 0; i < Qtd_linhas_(outro); i++) {
        valor += Get_(me, l, i) * Get_(me, i, c);
      }

      Set_ (res, l, c, valor);
      
    }
  } //Matriz feita.
  
  return res;
}

/* Para transpor estou assumindo que são matrizes quadradas - *
 * senão a operação perde o sentido.                          *
 * Mas, só por precaução, acho que elas aida funcionam        *
 * em retangulares.                                           */

static void Transpor_ (Matriz_t      * const me) 
{

  Matriz_pt outro = me;
  
  //Trocas as posicoes "l" & "c" para "c" & "l"
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_colunas_(me); c++) 
    {
      
      double novo_valor = Get_(outro, c, l);
      Set_(me, l, c, novo_valor);
      
    }
  } //Matriz alterada.
  
}

static void Transpor_diag2_ (Matriz_t      * const me) 
{

  Matriz_pt outro = me;
  
  //Trocas as posicoes "l" & "c" para "Qtd_Linha(outro)-1-c" & "Qtd_Coluna(outro)-1-l"
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_colunas_(me); c++) 
    {
      
      double novo_valor = Get_(outro, Qtd_Linhas_(outro)-1-c, Qtd_Colunas_(outro)-1-l);
      Set_(me, l, c, novo_valor);
      
    }
  } //Matriz alterada.
  
}

static void Reverse_horizontal_ (Matriz_t      * const me) 
{

  Matriz_pt outro = me;
  
  //Inverte a posição das linhas
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_colunas_(me); c++) 
    {
      
      double novo_valor = Get_(outro, Qtd_linhas_(outro)-1-l, c);
      Set_(me, l, c, novo_valor);
      
    }
  } //Matriz alterada.
  
}

static void Reverse_vertical_ (Matriz_t      * const me) 
{

  Matriz_pt outro = me;
  
  //Inverte a posição das colunas
  for (int l = 0; l < Qtd_linhas_(me); l++) 
  {
    for (int c = 0; c < Qtd_colunas_(me); c++) 
    {
      
      double novo_valor = Get_(outro, l, Qtd_colunas_(outro)-1-l);
      Set_(me, l, c, novo_valor);
      
    }
  } //Matriz alterada.
  
}

static inline
void Acrescenta_linha_ (Matriz_t      * const me) 
{
  int linha = Qtd_Linhas_(me)+1, coluna = Qtd_Colunas_(me);
  int tamanho[2] = {linha, coluna};
  
  Resize_(me, tamanho); //Surpreendentemente simples
}

static inline
void Acrescenta_coluna_ (Matriz_t      * const me)
{
  int linha = Qtd_Linhas_(me), coluna = Qtd_Colunas_(me)+1;
  int tamanho[2] = {linha, coluna};
  
  Resize_(me, tamanho);
}