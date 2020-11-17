#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include<time.h>

sem_t coronaPatient;
sem_t fluPatient; 
int potentialCPatients=0;   

void* Test(void* val) 
{   
       int t = (rand()%2);
       if(t==0){
         printf(" has Flu! \n");
         sem_post(&fluPatient);
         potentialCPatients--; 
      }
      else if(t==1){
        printf(" has Corona! we will do more tests. \n");
         sem_post(&coronaPatient); 
         potentialCPatients--; 
          int t1= (rand()%2);
          int t2= (rand()%2);
          if (t1==0 && t2==0){
             printf("Both tests came negative. Pateint is recovered! \n");
             sem_wait(&coronaPatient);
          }
         else{
          printf("one or both tests came positive. \n"); 
          t1= (rand()%2);
          t2= (rand()%2);
          printf("wait and do more tests until patient is recovered. \n");
          while(t1==0 || t2==0){
          printf("one or both tests came positive. \n"); 
          t1= (rand()%2);
          t2= (rand()%2);
         }
          printf("Both tests came negative. Pateint is recovered! \n");
          sem_wait(&coronaPatient);   
       }
     }
  printf("\n");
pthread_exit(NULL);
}


int main() 
{
    int cure;  
    /*key_t key = ftok ("/dev/null", 5);    
    int shmid = shmget (key, sizeof (int), 0611 | IPC_CREAT);
    int *potentialCPatients = (int *) shmat (shmid, NULL, 0); 
    *potentialCPatients = 0;*/
        
        int N;

        printf("Enter the potential corona patients \n");
        scanf("%d", &N);
	
        sem_init(&coronaPatient, 0, 0);
        sem_init(&fluPatient, 0, 0); 
        
        int k=N;  
        pthread_t id[k];
 
        for(int i=0; i<k; i++){
        potentialCPatients++;
        printf("Patient %d",i+1);
        pthread_create(&id[i], NULL,&Test, NULL);
        pthread_join(id[i], NULL); 
      }


       /* shmdt (potentialCPatients);
        shmctl (shmid, IPC_RMID, 0);*/
	sem_destroy(&coronaPatient);
        sem_destroy(&fluPatient);
	return 0; 
} 

