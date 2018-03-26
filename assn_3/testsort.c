#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LIMIT 200

int main(){
  char fname_std[]="sort.bin";
  char fname_test[]="sort.bin.tmp";

  int i=0;
  FILE *fp_std=fopen(fname_std,"r+b");
  FILE *fp_test=fopen(fname_test,"r+b");
  int num_std,num_test;

  for(;i<LIMIT;++i){
    fread(&num_std,sizeof(int),1,fp_std);
    fread(&num_test,sizeof(int),1,fp_test);
    printf("%d:  std: %d  test: %d  same:%d\n",i,num_std,num_test,num_std==num_test?1:0);

  }

  return 0;


}
