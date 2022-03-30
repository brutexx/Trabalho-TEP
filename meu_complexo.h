/**********************************************************************
* Baseado no código disponibilizado em
*  Application Note: "Object-Oriented Programming in C"
* https://www.state-machine.com/doc/AN_OOP_in_C.pdf
*
* Aqui se apresenta a interface do número do tipo "complexo"
*
* numero.h definiu a interface do "número_t virtual"
* ********************************************************************/

#ifndef MEUCOMPLEXO_H
#define MEUCOMPLEXO_H

#include "numero.h" /*a interface da classe básica */

struct MeuComplexo_Interface_st;
/*-------------------------------------------------------------*
 *  definição da estrutura MeuComplexo_st                        *
 * * trouxe a definição da estrutura para o "___.h" porque vou *
 * fazer a herança da estrutura de um "____.h" pai para        *
 * um "_____.h" filho                                          *
 * Aqui, iremos agregar duas "features" importantes:           *
 *                                                             *
 * a) uma tabela de interfaces que irá permitir acessar de     *
 *    forma mais legível os métodos de Numero_t. O ponteiro    *
 *    MeuComplexo_t é convertido para Numero_t antes de ser      *
 *    passado como argumento para as funções da classe base.   *
 *    E o resultado retornado da classe base é convertido "de  *
 *    volta" para "MeuComplexo_t".
 *                                                             *
 * b) os atributos (valores) numéricos do MeuComplexo_t          *
 *     (FINALMENTE  !!!!   :)                                  *
 * ------------------------------------------------------------*/
 struct MeuComplexo_st {
    struct Numero_st super;
                     /* <== herda a "super-classe Numeros",
                      * isto é, a classe superior a MeuDouble na
                      * hierarquia dos números
                      * Com isso, herda as operações da
                      * tabela de métodos comuns a todos os Numeros
                      * assim como os atributos desta classe base,
                      * (caso houvessem)       */

    struct MeuComplexo_Interface_st const *  Metodo;
                      /* o ponteiro para a tabela de
                       * ponteiros para funções
                       * que são apenas as chamadas para os métodos
                       * da super-classe, porém fazendo as
                       * devidas conversões na "entrada" e na
                       * saída               */

    /* o atributo  desta classe é o ponteiro para dois doubles  */
    double * valor;
}  ;
/* ----------------------------------------------------------*
 * declaro o ponteiro para o número do tipo MeuComplexo_t      *
 * que não estão "dentro" da tabela de funções virtuais      *
 * ----------------------------------------------------------*/
typedef struct MeuComplexo_st *MeuComplexo_pt;
typedef struct MeuComplexo_st  MeuComplexo_t;



struct MeuComplexo_Interface_st {
	MeuComplexo_pt (*copia)	(MeuComplexo_t const * const me);

    MeuComplexo_pt (*atribui)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t       * const  outro);

    MeuComplexo_pt (*soma)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t const * const  outro,
							 MeuComplexo_t       * const res);

    MeuComplexo_pt (*subt)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t const * const  outro,
							 MeuComplexo_t       * const res);

    MeuComplexo_pt (*mult)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t const * const  outro,
							MeuComplexo_t        * const res);

    MeuComplexo_pt (*divd)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t const * const  outro,
							 MeuComplexo_t       * const res);

    MeuComplexo_pt (*ac_soma)	(MeuComplexo_t       * const  me,
							     MeuComplexo_t const * const  outro);

    MeuComplexo_pt (*ac_subt)	(MeuComplexo_t       * const  me,
							     MeuComplexo_t const * const outro);

    MeuComplexo_pt (*ac_mult)	(MeuComplexo_t       * const  me,
							     MeuComplexo_t const * const outro);

    MeuComplexo_pt (*ac_divd)	(MeuComplexo_t        * const  me,
							     MeuComplexo_t const * const outro);

    int			(*compara)	(MeuComplexo_t const * const  me,
							 MeuComplexo_t const * const  outro);

    char *      (*imprime)  (MeuComplexo_t const * const  me);

    void        (*destroi)  (MeuComplexo_t       *        me);
    /*------------------------------------------------------*/
    void       (*get     ) (MeuComplexo_t  const * const me,
							double               * ptReal,
	                        double 				 * ptImag);

	void   	   (*set     ) (MeuComplexo_t        * const me,
	                          double             valorReal,
	                          double 		     valorImag);

	double       (*getReal ) (MeuComplexo_t  const * const me);

	void   	   (*setReal ) (MeuComplexo_t        * const me,
	                          double             valorReal);


	double       (*getImag ) (MeuComplexo_t  const * const me);

	void   	   (*setImag ) (MeuComplexo_t        * const me,
	                          double             valorReal);

	MeuComplexo_pt  (*conjugado) (MeuComplexo_t   const * const me,
	                              MeuComplexo_t         * const conj);


    double  (*modulo) (MeuComplexo_t   const * const me);

    double (*argumento) (MeuComplexo_t   const * const me);
    /*------------------------------------------------------*/
};

typedef struct MeuComplexo_Interface_st *MeuComplexo_Interface_pt;


/* protótipo do construtor   */
MeuComplexo_pt Complexo_constroi (MeuComplexo_pt  me,
								  double valorReal,
								  double valorImag);



#endif /* MEUCOMPLEXO_H */
