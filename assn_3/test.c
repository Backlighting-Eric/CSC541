#include<stdio.h>
#include<stdlib.h>

int main(){
  int len=167,i=0;
  char prefix[]="input.bin.";
  char *fname=malloc(sizeof(prefix)+sizeof(4));
  int size=0;
  for(;i<=len;++i){


  sprintf(fname,"%s%03d",prefix,i);
  FILE *fp=fopen(fname,"r+b");
  if(fp==NULL){
    perror("file not exist.\n");
    return 0;
  }
  int num;
  int num_prev=0;
  while(fread(&num,sizeof(int),1,fp)==1){
    if(num_prev>num){
      printf("ascending error");
    }
    //printf("%d: %d\n",i++,num);
    num_prev=num;
  }
  size+=ftell(fp);
  printf("fname: %03d okay.\n",i);
  fclose(fp);
}
printf("total size:%d\n",size);
  return 0;
}

/*
input.bin.000
0: 2322574
1: 4068141
2: 12335775
3: 17739901

input.bin.001
0: 492917
1: 2137072
2: 8425414
3: 8717091

input.bin.002
0: 2784234
1: 2878961
2: 3598871
3: 4854375


*/
