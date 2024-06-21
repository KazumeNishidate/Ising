#include <eggx.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>  
#include <time.h>
#include <gsl/gsl_rng.h>

#define X_MAX 50
#define Y_MAX 50
#define STEPS 100000
#define JKT   0.2   // J/kT
#define BKT   0.0   // B/kT

double ran1(void);

int main(void){
  int i, iu, id, j, ju, jd, k, sig;
  int mat[X_MAX][Y_MAX];
  int win;
  double de, pp;
  
  win = gopen(X_MAX*10,Y_MAX*10);
  layer(win,0,1);
  
  for(i=0;i<X_MAX;i++){  // initial state
    for(j=0;j<Y_MAX;j++){
      if(ran1()>0.5){
        mat[i][j]=1;  // up spin
        newcolor(win, "Yellow");
        fillrect(win,i*10,j*10,10,10);
      }else{
        mat[i][j]=-1; // down spin
      }
    }
  }

  for(k=0;k<STEPS;k++){
    i = (int)(X_MAX*ran1());
    j = (int)(Y_MAX*ran1());
    iu = i+1; ju = j+1;
    id = i-1; jd = j-1;
    if(id<0) id=X_MAX-1; if(iu==X_MAX) iu=0;      
    if(jd<0) jd=Y_MAX-1; if(ju==Y_MAX) ju=0;

    sig = mat[id][j]+mat[iu][j]+mat[i][jd]+mat[i][ju];
    de  = exp( -2.0*mat[i][j]*(sig*JKT + BKT) );
    pp  = ran1();

    if(pp<de) mat[i][j] *= -1;  // spin flip

    if(mat[i][j]==1){
      newcolor(win, "Yellow");
      fillrect(win,i*10,j*10,10,10);
    }else {
      newcolor(win, "Black");
      fillrect(win,i*10,j*10,10,10);
    }
    copylayer(win,1,0);    
  }
  getchar();
  gclose(win);
  return 0;
}

double ran1(){
  static const gsl_rng_type *T;
  static gsl_rng *r;
  static int cnt=0;

  if(cnt==0){
    gsl_rng_env_setup ();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_rng_set(r,time(NULL));
    cnt =1;
  }
  return gsl_rng_uniform(r);
}
