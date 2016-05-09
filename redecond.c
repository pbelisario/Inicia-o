#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Trede 1000 //tamanho da rede
#define TNe 1000//quant de neuronios excitadores
#define TNi (Trede-TNe) //quant de neuronios inibidores
#define TEMPO 1000 //número de operações da simulação
#define Napse 40 //numero de sinapses por neuronio
#define t 1 //constante de decaimento

double randn (double mu, double sigma){
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
  if (call==1){
      call=!call;
      return (mu + sigma * (double) X2);
    }
  do{
      U1=-1+((double)rand()/RAND_MAX)*2;
      U2=-1+((double)rand()/RAND_MAX)*2;
      W=pow(U1,2)+pow(U2,2);
    }while (W>=1||W==0);
  mult=sqrt((-2*log(W))/W);
  X1=U1*mult;
  X2=U2*mult;
  call=!call;
  return (mu+sigma*(double)X1);
}

void main(int argc, char const *argv[]) {
  FILE * arq = fopen ("log.txt","w");
  //FILE * arq_potencial = fopen ("potencial.txt","w");
  double a[Trede],b[Trede],c[Trede],d[Trede],W[Trede][Trede];
  double v[Trede],u[Trede],Ia[Trede],Ip[Trede],G[Trede][Napse];
  double re[TNe],ri[TNi];
  int i,j,k,n,atual,p,fired[Trede],firing[Trede];
  int S[Trede][Napse];
  //rand entre 0 e 1:
  for (i=0;i<TNe;i++) re[i]=drand48();
  for (i=0;i<TNi;i++) ri[i]=drand48();
  //parametros dos excitadores:
  for (i=0;i<TNe;i++){
    a[i]=0.02;
    b[i]=0.25;
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
  for (i=0;i<Trede;i++) {
    firing[i]=0;
    v[i]=-65;
    u[i]=v[i]*b[i];
    Ia[i]=Ip[i]=0;
    for (j=0;j<TNe;j++) {W[i][j]=0.5*drand48();}
    for (;j<Trede;j++) {W[i][j]=-drand48();}
    W[i][i]=0;
    // Constroi as sinapses:
    for (j=0;j<Napse;j++) {
      //S[i][j]=rand()%Trede;
      S[i][j]=(Trede*(rand()%100))/100;
      G[i][j]=0;
    }
  }
  i=j=k=0;
  //******************************//
  //*********Simula Rede**********//
  //******************************//
  for (n=0;n<TEMPO;n++) {
    //printf("%d\n",n);
    //Resetar vetor fired e firing:
    for (i=0;i<Trede;i++) {
      fired[i]=firing[i];
      Ip[i]=0;
      if (fired[i])
        fprintf(arq,"%d  %d\n",n,i);
      firing[i]=0;
    }
    //Entrada aleatoria para os neuronios:
    for (i=0;i<TNe;i++) Ia[i]=5*randn(0,1);
    for (;i<Trede;i++) Ia[i]=2*randn(0,1);
    //Atualiza matriz de condutancia(G):
    for (i=0;i<Trede;i++) {
      for (j=0;j<Napse;j++) {
        G[i][j]=G[i][j]-((G[i][j])/t);
        if(G[i][j]<0) G[i][j]=0;
      }
    }
    //Realiza um passo para cada neuronio:
    for (atual=0;atual<Trede;atual++){
      //Calcula a soma das entradas:
      for (j=0;j<Napse;j++){
        p=S[atual][j];
        Ia[atual]+=Ip[atual];
      }
      //Atualiza v e u:
      v[atual]+=0.5*((0.04*(v[atual]*v[atual]))+5*v[atual]+140-u[atual]+Ia[atual]);
      v[atual]+=0.5*((0.04*(v[atual]*v[atual]))+5*v[atual]+140-u[atual]+Ia[atual]);
      u[atual]+=a[atual]*(b[atual]*v[atual]-u[atual]);
      //Determina se atual disparou e atualiza u e v:
      if (v[atual]>=30){
        firing[atual]=1;
        for (i=0;i<Napse;i++) {
          G[atual][i]=1;
        }
        v[atual]=c[atual];
        u[atual]+=d[atual];
      }
      //Envia I=G*W para o pos sinaptico:
      for (j=0;j<Napse;j++) {
        p=S[atual][j];
        Ip[p]+=W[p][atual]*G[atual][j];
      }
    }
  }
}
