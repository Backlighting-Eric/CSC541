#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

typedef enum { false, true } bool;

/* get the size of the file whose file pointer is inp*/
int get_file_size(FILE *inp);

void mem_binary_search(FILE *key,FILE *seek,int key_size,int seek_size,int *key_array,int *seek_array,int *hit);

void mem_linear_search(FILE *key,FILE *seek,int key_size,int seek_size,int *key_array,int *seek_array,int *hit);

void disk_binary_search(FILE *key,FILE *seek,int key_size,int seek_size,int *seek_array,int *hit);

void disk_linear_search(FILE *key,FILE *seek,int key_size,int seek_size,int *seek_array,int *hit);

void print_yes_no(int *seek_array,int seek_size,int *hit);

void free_and_close(FILE *key,FILE *seek,int *seek_array,int* hit);


int main(int argc,char *argv[]){
	if(argc!=4){
		printf("Command line arguments are incorrect.\n");
		exit(1);
	}

	struct timeval start,end;

	FILE *key=fopen(argv[2],"rb");
	FILE *seek=fopen(argv[3],"rb");

	int key_size=get_file_size(key);
	int seek_size=get_file_size(seek);

	int *seek_array=calloc(seek_size,sizeof(int));
	int *hit=calloc(seek_size,sizeof(int));
	fread(seek_array,seek_size,4,seek);
	fseek(seek,0,SEEK_END);

/*time start*/
gettimeofday(&start,NULL);

if(strncmp(argv[1],"--mem-lin",9)==0){
	int *key_array=calloc(key_size,sizeof(int));
	fread(key_array,key_size,4,key);
	fseek(key,0,SEEK_END);
	mem_linear_search(key,seek,key_size,seek_size,key_array,seek_array,hit);

	free(key_array);
}
else if(strncmp(argv[1],"--mem-bin",9)==0){
	int *key_array=calloc(key_size,sizeof(int));
	fread(key_array,key_size,4,key);
	fseek(key,0,SEEK_END);
	mem_binary_search(key,seek,key_size,seek_size,key_array,seek_array,hit);

	free(key_array);

}
else if(strncmp(argv[1],"--disk-lin",10)==0){
	disk_linear_search(key,seek,key_size,seek_size,seek_array,hit);

}
else if(strncmp(argv[1],"--disk-bin",10)==0){
	disk_binary_search(key,seek,key_size,seek_size,seek_array,hit);

}
else{
free_and_close(key,seek,seek_array,hit);
printf("Wrong argument: it should be --mem-lin, --mem-bin, --disk-lin, --disk-bin");
exit(1);
}
free_and_close(key,seek,seek_array,hit);

	gettimeofday(&end,NULL);

/*
	printf("Seconds: %d\n",start.tv_sec);
	printf("Microseconds: %d\n",start.tv_usec);

	printf("Seconds: %d\n",end.tv_sec);
	printf("Microseconds: %d\n",end.tv_usec);
	*/

	printf("Time: %ld.%06ld",end.tv_sec-start.tv_sec,end.tv_usec-start.tv_usec);
	return 0;

}

void free_and_close(FILE *key,FILE *seek,int *seek_array,int* hit){

	free(seek_array);
	free(hit);
	fclose(seek);
	fclose(key);
}

int get_file_size(FILE *inp){
	if(inp==NULL){
		perror("Error:");
		exit(1);
	}
	fseek(inp,0,SEEK_END);
	int size=ftell(inp)/4;
	rewind(inp);
	return size;
}

void disk_linear_search(FILE *key,FILE *seek,int key_size,int seek_size,int *seek_array,int *hit){
		int k,i,j;
		for(i=0;i<seek_size;++i){
			int target=*(seek_array+i);
			for(j=0;j<key_size;++j){
				fread(&k,sizeof(int),1,key);
				if(k==target){
					*(hit+i)=1;
					break;
				}
			}
			fseek(key,0,SEEK_SET);
		}
		print_yes_no(seek_array,seek_size,hit);
}

void disk_binary_search(FILE *key,FILE *seek,int key_size,int seek_size,int *seek_array,int *hit){
	int k,i;
	for(i=0;i<seek_size;++i){
		int left=0,right=key_size-1;
		int target=*(seek_array+i);
		int mid;
		while(left<=right){
			mid=left+(right-left)/2;
			fseek(key,mid*sizeof(int),SEEK_SET);
			fread(&k,sizeof(int),1,key);
			if(k==target){
				*(hit+i)=1;
				break;
			}
			else if(k<target){
				left=mid+1;
			}
			else{
				right=mid-1;
			}
		}
		fseek(key,0,SEEK_SET);
	}
	print_yes_no(seek_array,seek_size,hit);


}

void mem_binary_search(FILE *key,FILE *seek,int key_size,int seek_size,int *key_array,int *seek_array,int *hit){
	int i;
	for(i=0;i<seek_size;++i){
		int left=0,right=key_size-1;
		int target=*(seek_array+i);
		int mid;
		while(left<=right){
			mid=left+(right-left)/2;
			if(*(key_array+mid)==target){
				*(hit+i)=1;
				break;
			}
			else if(*(key_array+mid)<target){
				left=mid+1;
			}
			else{
				right=mid-1;
			}
		}
	}
	print_yes_no(seek_array,seek_size,hit);

}

void mem_linear_search(FILE *key,FILE *seek,int key_size,int seek_size,int *key_array,int *seek_array,int *hit){
	int i,j;
	for(i=0;i<seek_size;++i){
		for(j=0;j<key_size;++j){
			if(*(seek_array+i)==*(key_array+j)){
				*(hit+i)=1;
				break;
			}
		}
	}

	print_yes_no(seek_array,seek_size,hit);
}

void print_yes_no(int *seek_array,int seek_size,int *hit){
	int i;
	for(i=0;i<seek_size;++i){
		if(*(hit+i)){
			printf("%12d: Yes\n",*(seek_array+i));
		}
		else{
			printf("%12d: No\n",*(seek_array+i));
		}
	}
}
