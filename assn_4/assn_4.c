#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum {false,true} bool;
typedef enum {ADD,FIND,PRINT,END,CONTINUE} command_type;

typedef struct{
  command_type type;
  int key;
}command;

int order;

typedef struct{ /* B-tree order */
  int n;        /* Number of keys in node */
  int *key;     /* Node's keys */
  long *child;
}btree_node;
btree_node node;

FILE *fp;
char *buffer;
int *key_buf;
int *child_buf;
int len_buf;

size_t buffer_size=128;
size_t buffer_length;

long root_offset;
int parent_offset=-1;


void convert_to_command(char *buffer,command *com);
void read_node(long offset,btree_node *node);
void write_node(long offset,btree_node *node);
bool find(int key,int *parent_offset,int offset);
void add(int key,int offset,int parent_offset);

void copy_node2buf(btree_node *node);
void copy_buf2node(btree_node *node,int start,int end);
void add_one_key(int key,long km_child);
int ceiling(float num);
void print_single_node(btree_node *node);
void print_btree(long offset);

/*
---------------------------Queue
*/
struct QNode
{
    long key;
    struct QNode *next;
};

// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct Queue
{
    struct QNode *front, *rear;
};

// A utility function to create a new linked list node.
struct QNode* newNode(int k)
{
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue *q, int k);
struct QNode *deQueue(struct Queue *q);
bool isQueueEmpty(struct Queue *q);
int sizeQueue(struct Queue *q);
/*
---------------------------Queue
*/



int main(int argc,char *argv[]){
  if(argc!=3){
    printf("Command line arguments are incorrect.\n");
    exit(1);
  }
  char *fname=argv[1];
  order=strtol(argv[2],NULL,10);

  node.n=0;
  node.key=(int *)calloc(order-1,sizeof(int));
  node.child=(long *)calloc(order,sizeof(long));
  key_buf=(int *)calloc(order,sizeof(int));
  child_buf=(int *)calloc(order+1,sizeof(int));


  fp=fopen(fname,"r+b");
  if(fp==NULL){
    root_offset=sizeof(long);
    fp=fopen(fname,"w+b");
    fwrite(&root_offset,sizeof(long),1,fp);
  }
  else{
    fread(&root_offset,sizeof(long),1,fp);
    read_node(root_offset, &node);

  }
  while(true){
    buffer=(char *)malloc(buffer_size*sizeof(char));
    if(buffer==NULL){
      printf("Error\n");
      exit(1);
    }
    buffer_length=getline(&buffer,&buffer_size,stdin);
    //printf("you typed: %s\n",buffer);
    command com;
    convert_to_command(buffer,&com);
    if(com.type==END){
        fclose(fp);
        break;
    }
    else if(com.type==FIND){
        if(find(com.key, &parent_offset, root_offset)){
          printf("Entry with key=%d exists\n",com.key);
        }
        else{
          printf("Entry with key=%d does not exist\n",com.key);
        }
    }
    else if(com.type==ADD){
        add(com.key, root_offset, parent_offset);
    }
    else if(com.type==PRINT){
        print_btree(root_offset);
    }

  }

return 0;
}

void read_node(long offset,btree_node *node){
  //printf("read_node offset: %ld\n",offset);
  fseek(fp,offset,SEEK_SET);
  fread(&node->n,sizeof(int),1,fp);
  fread(node->key,sizeof(int),order-1,fp);
  fread(node->child,sizeof(long),order,fp);
  //print_single_node(node);
  //printf("\n");
}
void write_node(long offset,btree_node *node){
  //printf("write_node offset: %ld\n",offset);
  fseek(fp,offset,SEEK_SET);
  fwrite(&node->n,sizeof(int),1,fp);
  fwrite(node->key,sizeof(int),order-1,fp);
  fwrite(node->child,sizeof(long),order,fp);
  //print_single_node(node);
  //printf("\n");
}
bool is_leafnode(btree_node *node){
  int i=0;
  for(;i<node->n;++i){
    if(node->child[i]!=0){
      return false;
    }
  }
  return node->child[i]==0?true:false;
}

// start from offset to find parent offset of the key.
bool find(int key,int *parent_offset,int offset){
  //printf("find %d offset:%d\n",key,offset);
  if(offset==0){
    //printf("Entry with key=%d does not exist\n",key);
    return false;
  }
  else if(offset==root_offset){
    *parent_offset=-1;
  }
  read_node(offset, &node);
  int i=0;
  for(;i<node.n;++i){
    if(node.key[i]==key){
      //printf("Entry with key=%d exists\n",key);
      return true;
    }
    else if(key<node.key[i]){
      *parent_offset=offset;
      return find(key,parent_offset,node.child[i]);
    }
  }
  *parent_offset=offset;
  return find(key,parent_offset,node.child[node.n]);
}

/*
find if the key exists at first. if not, add the key into one leaf node. if the leaf node is full, split and promote the median key into the parent node.
*/
void add(int key,int offset,int parent_offset){
   while(true){
     read_node(offset,&node);
     if(is_leafnode(&node)){
       break;
     }
     int i=0;
     for(;i<node.n;++i){
       if(node.key[i]==key){
         printf("Entry with key=%d already exists\n",key);
         return;
       }
       else if(key<node.key[i]){
         parent_offset=offset;
         offset=node.child[i];
         break;
       }
     }
     if(i==node.n){
       parent_offset=offset;
       offset=node.child[node.n];
     }

   }
   int i=0;
   for(;i<node.n;++i){
     if(node.key[i]==key){
       printf("Entry with key=%d already exists\n",key);
       return;
     }
   }

   long km_child=0;
   int km=key;

   while(true){
   copy_node2buf(&node);
   add_one_key(km, km_child);
   if(len_buf<order){
     copy_buf2node(&node, 0, len_buf-1);
     write_node(offset, &node);
     return;
   }
   int m=ceiling(((float)len_buf-1)/2);
   copy_buf2node(&node, 0, m-1);
   write_node(offset, &node);
   int key_point=node.key[0];
   copy_buf2node(&node, m+1, len_buf-1);
   fseek(fp, 0, SEEK_END);
   km_child=ftell(fp);
   write_node(km_child, &node);
   km=key_buf[m];
   find(key_point, &parent_offset, root_offset);
   if(parent_offset==-1){
     node.n=1;
     node.key[0]=km;
     node.child[0]=offset;
     node.child[1]=km_child;
     fseek(fp, 0, SEEK_END);
     root_offset=ftell(fp);
     write_node(root_offset, &node);
     fseek(fp, 0, SEEK_SET);
     fwrite(&root_offset,sizeof(long),1,fp);
     return;
   }
   read_node(parent_offset, &node);
   offset=parent_offset;
 }
}

void copy_node2buf(btree_node *node){
  int i=0;
  for(;i<node->n;++i){
    key_buf[i]=node->key[i];
    child_buf[i]=node->child[i];
  }
  child_buf[i]=node->child[i];
  len_buf=node->n;
}
void copy_buf2node(btree_node *node,int start,int end){
  int i=0;
  node->n=end-start+1;
  for(;i<node->n;++i){
    node->key[i]=key_buf[start+i];
    node->child[i]=child_buf[start+i];
  }
  node->child[i]=child_buf[start+i];
}
void add_one_key(int key,long km_child){
  int i=0;
  while(i<len_buf&&key>key_buf[i]){
    i++;
  }
  int j=len_buf;
  while(j>i){
    key_buf[j]=key_buf[j-1];
    child_buf[j+1]=child_buf[j];
    j--;
  }
  //insert key and child offset approiately.
  key_buf[i]=key;
  child_buf[i+1]=km_child;
  len_buf++;
}

void convert_to_command(char *buffer,command *com){
  char buffer_ready[1024];
  strcpy(buffer_ready,buffer);
  free(buffer);

  char *token;
  token=strtok(buffer_ready," \n");
  if(strcmp(token,"end")==0){
    com->type=END;
  }
  else if(strcmp(token,"add")==0){
    com->type=ADD;
    token=strtok(NULL," \n");
    com->key=atoi(token);
  }
  else if(strcmp(token,"find")==0){
    com->type=FIND;
    token=strtok(NULL," \n");
    com->key=atoi(token);
  }
  else if(strcmp(token,"print")==0){
    com->type=PRINT;
  }
  else{
    com->type=CONTINUE;
    printf("command should be [add, find, print, end]\n");
  }
}

int ceiling(float num) {
    int inum = (int)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

void print_single_node(btree_node *node){
  int i;
for( i = 0; i < node->n - 1; i++ )
{
  printf( "%d,", node->key[ i ] );
}
printf( "%d ", node->key[ node->n - 1 ] );
}

void print_btree(long offset){
  struct Queue *q=createQueue();
  read_node(offset, &node);
  if(node.n==0){
    return;
  }
  enQueue(q, offset);
  int level=1;
  while(!isQueueEmpty(q)){
    int cnt=sizeQueue(q);
    printf(" %d: ",level++);
    while(cnt--){
        int off=deQueue(q)->key;
        read_node(off, &node);
        print_single_node(&node);
        if(!is_leafnode(&node)){
          int i=0;
          for(;i<=node.n;++i){
            enQueue(q, node.child[i]);
          }
        }
      }
      printf("\n");
  }
}

// The function to add a key k to q
void enQueue(struct Queue *q, int k)
{
    // Create a new LL node
    struct QNode *temp = newNode(k);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}
// Function to remove a key from given queue q
struct QNode *deQueue(struct Queue *q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
       return NULL;
    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;
    q->front = q->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
       q->rear = NULL;
    return temp;
}
bool isQueueEmpty(struct Queue *q){
  return q->front==NULL?true:false;
}
int sizeQueue(struct Queue *q){
  if(isQueueEmpty(q)){
    return 0;
  }
  struct QNode *head=q->front;
  int size=1;
  while(head!=q->rear){
    head=head->next;
    size++;
  }
  return size;
}
