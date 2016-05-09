#include <stdio.h>
#include <stdlib.h>
#define Trede 1000 //tamanho da rede
#define TNe 800 //quant de neuronios excitadores
#define TNi (Trede-TNe) //quant de neuronios inibidores
#define TEMPO 1000 //número de operações da simulação

void main(int argc, char const *argv[]) {
  FILE * arq = fopen ("log.txt","w");
  //FILE * arq_potencial = fopen ("potencial.txt","w");
  double a[Trede],b[Trede],c[Trede],d[Trede],S[Trede][Trede];
  double v[Trede],u[Trede],I[Trede];
  double re[TNe],ri[TNi];
  int i,j,k,n,atual,fired[Trede],firing[Trede];
  //rand entre 0 e 1:
  for (i=0;i<TNe;i++) re[i]=drand48();
  for (i=0;i<TNi;i++) ri[i]=drand48();
  //parametros dos excitadores:
  for (i=0;i<TNe;i++){
    a[i]=0.02;
    b[i]=0.2;
    c[i]=-65+15*re[i];
    d[i]=8-6*re[i];
  }
  //parametros dos inibidores:
  for (i=TNe;i<Trede;i++){
    a[i]=0.02+0.08*ri[i-TNe-1];
    b[i]=0.25-0.05*ri[i-TNe-1];
    c[i]=-65;
    d[i]=2;
  }
  //Constroi a rede:
  for (i=0;i<Trede;i++){
    //iniciar vetor firing:
    firing[i]=0;
    //potencial inicial dos neuronios:
    v[i]=-65;
    //recuperação da membrana do axonio:
    u[i]=v[i]*b[i];
    //Peso dos excitadores:
    for (j=0;j<TNe;j++) {S[i][j]=0.5*drand48();}
    //Peso dos inibidores:
    for (;j<Trede;j++) {S[i][j]=-drand48();}
    S[i][i]=0;
  }
  i=j=k=0;
  for (n=0;n<TEMPO;n++) {
    //printf("%d\n",n);
    //Resetar vetor fired e firing:
    for (i=0;i<Trede;i++) {
      fired[i]=firing[i];
      //printf("%d  ",fired[i]);
      if (fired[i])
        fprintf(arq,"%d  %d\n",n,i);
      firing[i]=0;
    }
    //printf("\n");
    //Entrada aleatoria para os neuronios:
    for (i=0;i<TNe;i++) I[i]=5*drand48();
    for (;i<Trede;i++) I[i]=2*drand48();
    //Realiza um passo para cada neuronio:
    for (atual=0;atual<Trede;atual++){
      //Calcula a soma das entradas:
      for (j=0;j<Trede;j++){
        if (fired[j]) I[atual]+=S[atual][j];
      }
      //Atualiza v e u:
      v[atual]+= 0.5*((0.04*(v[atual]*v[atual]))+5*v[atual]+140-u[atual]+I[atual]);
      v[atual]+= 0.5*((0.04*(v[atual]*v[atual]))+5*v[atual]+140-u[atual]+I[atual]);
      u[atual]+= a[atual]*(b[atual]*v[atual]-u[atual]);
      //Determina se atual disparou e atualiza u e v:
      if (v[atual]>=30){
        firing[atual]=1;
        v[atual]=c[atual];
        u[atual]+=d[atual];
      }
    // if (atual ==100) fprintf(arq_potencial, "%d %.3f\n",n,v[atual] );
      //printf("%d %.1lf %.1lf %.1lf %.1lf\n",atual,v[atual],u[atual],c[atual],u[atual]+d[atual]);
    }
    //printf("\n\n");
  }
}
