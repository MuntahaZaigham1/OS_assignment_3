
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <sys/shm.h>
#include <errno.h>







int main(int argc, char *argv[])
{
	//parent
	int id_sem=shmget(12341, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	int id_sem1=shmget(13342, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	int id_sem2=shmget(13343, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	int id_sem3=shmget(13344, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	int id_sem4=shmget(13345, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	sem_t *mutex;
	sem_t *index;
	sem_t *process1;
	sem_t *process2;
	sem_t *mutexCD;
	if (id_sem==-1 || id_sem1==-1 || id_sem2==-1 || id_sem3==-1 || id_sem4==-1)
	{	
		printf("ERROR! %d %d",errno,strerror(errno));
		return 1;
	}
	
	mutex= (sem_t*) shmat(id_sem, NULL, 0);
	index= (sem_t*) shmat(id_sem1, NULL, 0);
	process1= (sem_t*) shmat(id_sem2, NULL, 0);
	process2= (sem_t*) shmat(id_sem3, NULL, 0);
	mutexCD= (sem_t*) shmat(id_sem4, NULL, 0);
	sem_init(mutex, 2, 0);
	sem_init(index, 2, 0);
	sem_init(process1, 2, 0);
	sem_init(process2, 2, 0);
	sem_init(mutexCD, 2, 0);
	
	//shared memory
	int key=shmget(112221, 1024, IPC_CREAT | IPC_EXCL | 0666); 
	char* ptr= (char*) shmat(key, NULL, 0);
	printf("shm created with id= %d \n",key);
	
	int key1=shmget(112223, 1024, IPC_CREAT | IPC_EXCL | 0666); 
	char* ptr1= (char*) shmat(key1, NULL, 0);
	printf("shm1 created with id= %d \n",key1);
	
	pid_t pid1, pid2, pid3 ,pid4;
	
	pid1=fork();
	if(pid1==0){
		printf("hello i m process A\n");
		//sleep(4);
		FILE *fp;
		//char buff[12];
		fp = fopen("Desktop/file-1.txt", "r");
		fgets(ptr, 10, (FILE*)fp);
		int i=0;
		for(i=0;i<10;i++)printf("%c",ptr[i]);
		printf("\n");
		
		for(i=0;i<10;i++)sem_post(index);
		sem_post(process1);
		sem_post(mutex); 
	}
	else if(pid1>0){
		pid2=fork();
		if(pid2==0){
			sem_wait(mutex); 
			printf("hello i m process B\n");
			FILE *fp1;
			//char buff[12];
			fp1 = fopen("Desktop/file-2.txt", "r");
			int* n;
			
			sem_getvalue(index, &n);
			printf("hello i m process B %d \n",n);
			int j=0;
			for(j=n;j<n+10;j++)
			ptr[j] = fgetc(fp1);
			//fgets(ptr, 10, (FILE*)fp);
			int i=0;
			for(i=10;i<20;i++)printf("%c",ptr[i]);
			printf("\n");
			sem_post(process2);
			
		}
		else if(pid2>0){
			pid3=fork();
			if(pid3==0){
				sem_wait(process1);
				sem_wait(process2);
				printf("hello i m process C\n");
				//reads from ptr and write it in ptr1
				int i=0;
				for(i=0;i<20;i++){
					ptr1[i]=ptr[i];
				}
				sem_post(mutexCD);
				
			}
			else if(pid3>0){
				pid4=fork();
				if(pid4==0){
				sem_wait(mutexCD);
				printf("hello i m process D\n");
				
				int i=0;
				for(i=0;i<20;i++){
					printf("%c",ptr1[i]);
				}
				printf("\n");
				}
				else if(pid4>0){
				
				printf("hello i m parent of all\n");
				sem_destroy(mutex);
				shmdt(mutex);
				}
				
			}
		}
		
	}
	shmctl(id_sem, IPC_RMID, NULL);
	shmctl(id_sem1, IPC_RMID, NULL);
	shmctl(id_sem2, IPC_RMID, NULL);
	shmctl(id_sem3, IPC_RMID, NULL);
	shmctl(id_sem4, IPC_RMID, NULL);
	shmctl(key, IPC_RMID, NULL);
	shmctl(key1, IPC_RMID, NULL);
	shmdt(mutex);
	shmdt(ptr);
	shmdt(index);
	shmdt(ptr1);
	shmdt(process1);
	shmdt(process2);
	shmdt(mutexCD);
	sem_destroy(mutex);
	sem_destroy(ptr);
	sem_destroy(index);
	sem_destroy(ptr1);
	sem_destroy(process1);
	sem_destroy(process2);
	sem_destroy(mutexCD);
	return 0;	
}
