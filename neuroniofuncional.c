#include <stdio.h>
#include <stdlib.h>


/*******************************/
/*     CONSTANTES DEFINIDAS    */
/*******************************/

//Para os Neuronios
#define Rede 1000
#define Excitadores 800
#define Inibidores (Rede - Excitadores)
#define Interacoes 1000

//seed do drand
#define seed 68111


int main(int argc, char const *argv[]) {

  //criação de uma seed para ficar pseudorandomico os valores
  srand48 (seed);

  /**********************/
  /*     DECLARAÇÕES    */
  /**********************/


  // Arquivo saida
  FILE *saida = fopen ("data.txt", "w");

  //Declaração da rede
  float a[Rede], b[Rede], c[Rede], d[Rede], S[Rede][Rede];
  float v[Rede], u[Rede], I[Rede];
  float re[Excitadores], ri[Inibidores];

  //Vetores de Disparos
  float disparado[Rede], disparando[Rede];

  //Auxiliares de manipulação de laços
  int i, j, t, corrente;


  /**********************/
  /*   INICIALIZAÇÕES   */
  /**********************/


  for (i = 0; i < Excitadores; i++) {re[i] = drand48();}
  for (i = 0; i < Inibidores; i++)  {ri[i] = drand48();}

  //Peso dos Exicitadores
  for (i = 0; i < Excitadores; i++){
    a[i] = 0.02;
    b[i] = 0.2;
    c[i] = -65 + 15 * re[i];
    d[i] = 8 - 6 * re[i];
  }

  //Peso dos Inibidores
  for (i = Excitadores; i < Rede; i++){
    a[i] = 0.02 + 0.08 * ri[(i - Excitadores - 1)];
    b[i] = 0.25 - 0.05 * ri[(i - Excitadores - 1)];
    c[i] = -65;
    d[i] = 2;
  }

  //inicializa a Rede
  for (i = 0; i < Rede; i++){
    //inicializa aqueles que dispararam
    disparando[i] = 0;
    //potencial inicial dos neuronios
    v[i] = -65;
    //taxa de restituição da membrana
    u[i] = (v[i] * b[i]);
    //Peso das ligações dos Exicitadores
    for (j = 0; j < Excitadores; j++) {S[i][j] = 0.5 * drand48();}
    //Peso das ligações dos Inibidores em S
    for (; j < Rede; j++) {S[i][j] = -drand48();}
    //Como um neuronio nao liga com si mesmo,
    // o peso dessas ligações será 0
    S[i][i] = 0;
  }


  /**********************/
  /*      SIMULAÇÃO     */
  /**********************/

  //SIMULAÇÃO DA COLUNA DA MATRIZ S
  for (t = 0; t < Interacoes; t++){

    //Resetar o vetor de disparandos e passar seu valor para disparados
    for (i = 0; i < Rede; i++){
      disparado[i] = disparando[i];
      if (disparado[i]){
        fprintf(saida, "%d\t%d\n", t, i);
      }
      disparando[i] = 0;
    }

    //CRIA UM INPUT ALEATORIO
    for (i = 0; i < Excitadores; i++) {I[i] = (5 * drand48());}
    for ( ; i < Rede; i++) {I[i] = (2 * drand48());}

    //SIMULAÇÃO DA LINHA DA MATRIZ S
    for (corrente = 0; corrente < Rede; corrente++){
      //ATUALIZA OS NOVOS VALORES DE ENTRADA
      for (i = 0; i < Rede; i++){
        if (disparado[i]) {I[corrente] += S[corrente][i];}
      }

      //ATUALIZA OS VALORES DE V e U
      v[corrente] += 0.5 * ((0.04 * (v[corrente] * v[corrente])) + 5 * v[corrente] + 140 - u[corrente] + I[corrente]);
      v[corrente] += 0.5 * ((0.04 * (v[corrente] * v[corrente])) + 5 * v[corrente] + 140 - u[corrente] + I[corrente]);
      u[corrente] += a[corrente] * (b[corrente] * v[corrente] - u[corrente]);

      //VERIFICA SE A POSIÇÃO CORRENTE DISPAROU
      if (v[corrente] >= 30){
        disparando[corrente] = 1;
        v[corrente]  = c[corrente];
        u[corrente] += d[corrente];
      }
    }
  }
  return 0;
}
