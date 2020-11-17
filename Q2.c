#include<stdio.h> 
#include<semaphore.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h> 
#include<sys/shm.h>
#include<fcntl.h>

int main() 
{    

    key_t key1= shmget(12329, 1024, IPC_CREAT | IPC_EXCL | 0644);
    char* buffer1= (char*) shmat(key1, NULL, 0);
    
    key_t key2= shmget(12339, 1024, IPC_CREAT | IPC_EXCL | 0622);
    char* buffer2= (char*) shmat(key2, NULL, 0);
    
    sem_t *s1 = sem_open ("s1", O_CREAT | O_EXCL, 0644, 1);
	sem_t *s2 = sem_open ("s2", O_CREAT | O_EXCL, 0644, 0);
	sem_t *s3 = sem_open ("s3", O_CREAT | O_EXCL, 0644, 0);
	sem_t *s4 = sem_open ("s4", O_CREAT | O_EXCL, 0644, 0);
	sem_t *s5 = sem_open ("s5", O_CREAT | O_EXCL, 0644, 0);

        pid_t x1;
        pid_t x2;
        pid_t x3;
        pid_t x4;

        x1=fork();
        x2=fork();
        x3=fork();
        x4=fork();

        if(x1==0 && x2!=0 && x3!=0 && x4!=0){
       
        sem_wait(s1);     
         FILE* file=fopen("fileA.txt","r");
        if(file){
         char arr[15];
         int i=0;
         char a;
          while(i<10){
               a=fgetc(file);
               arr[i]=a;
               i++;   
             }
          for(int j=0;j<10;j++)
          	buffer1[j]=arr[j];
         fclose(file);
        }
       else 
            printf("cannot open file!! ");
         
        printf("Process A Executed\n");    
        sem_post(s2);
       }


       else if(x2==0 && x1!=0 && x3!=0 && x4!=0){
		sem_wait(s2);   
        FILE* file=fopen("fileB.txt","r");
        if(file){
         char arr[15];
         int i=0;
         char a;
          while(i<10){
               a=fgetc(file);
               arr[i]=a;
               i++;   
             }
          for(int j=10, k=0;j<20;j++,k++)
          	buffer1[j]=arr[k];
         fclose(file);
        }
       else 
            printf("cannot open file!! ");      
       
       printf("Process B Executed\n"); 
       sem_post(s3);
        }

       else if(x3==0 && x2!=0 && x1!=0 && x4!=0){
        sem_wait(s3);   
        for (int i=0;i<20;i++){
           buffer2[i]=buffer1[i];    
       }
       printf("Process C Executed\n");
       sem_post(s4);
        }
       
       else if(x4==0 && x2!=0 && x3!=0 && x1!=0){
        sem_wait(s4);
          printf("\nThe Contents of buffer 2 are: ");    
          printf("%s\n\n", buffer2);
        
        printf("Process D Executed\n");     
        sem_post(s5);
     }
       else if(x4!=0 && x2!=0 && x3!=0 && x1!=0){
        sem_wait(s5);
      	printf("Parent Process\n"); 
        shmdt(buffer1);
        shmctl(key1, IPC_RMID, NULL);
        shmdt(buffer2);
        shmctl(key2, IPC_RMID, NULL);
        sem_unlink ("s1");   
        sem_close(s1);
        sem_unlink ("s2");   
        sem_close(s2);
        sem_unlink ("s3");   
        sem_close(s3);
        sem_unlink ("s4");   
        sem_close(s4);
        sem_unlink ("s5");   
        sem_close(s5);
        return 0;
        }
}
