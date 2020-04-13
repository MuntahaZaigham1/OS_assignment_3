#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 

void *runner1(int n);
sem_t mutex; 
sem_t test_wait;
sem_t fluPatients;
sem_t coronaPatients;
void increaseFlu();
void increaseCorona();
int potentialC;
int main(int argc, char *argv[]){
 	srand(time(0)); 
	int n; /* an integer chosen by the user */
	printf("Number of patients to enter : ");
	scanf ("%d", &n);
	sem_init(&mutex, 0, 1);
	sem_init(&test_wait, 0, 1);
	sem_init(&fluPatients, 0, 0);
	sem_init(&coronaPatients, 0, 0);
	int i=0;
	pthread_t tid_s[n];		
	for(;i<n;i++){
	//sleep(2); 
	pthread_create(&tid_s[i], NULL, runner1,(void**)i);
	 
	}
	for(i=0;i<n;i++){
	//sleep(2);

	pthread_join(tid_s[i], NULL);
	}
	int* s;
	sem_getvalue(&fluPatients, &s);
	printf("\n--no. of flu patients=%d---\n",s);
	int *f;
	sem_getvalue(&coronaPatients,&f);
	printf("\n--no. of corona patients=%d---\n",f);

	printf("\n--no. of potential patients=---%d---\n",potentialC);
	
	sem_destroy(&mutex);
	sem_destroy(&test_wait);  
	sem_destroy(&fluPatients);
	sem_destroy(&coronaPatients);  
	return 0;

}


void *runner1(int n)
{

	printf("\n----------%d----------\n",n);
	sem_wait(&mutex); 
	int count=increasePotentialCPatients();
	sleep(1);
	printf("\n--count=---%d---\n",count);
	int test;
	test = randomFunction();
	printf("\n--test result=---%d---\n",test);
	if(test==0)
	{
		increaseFlu();
		count=decreasePotentialCPatients();
	}
	else if ( test ==1 )
	{
		
		increaseCorona();
		count=decreasePotentialCPatients();
	}
	
	sem_post(&mutex);
	
	return NULL;
}
int increasePotentialCPatients()
{
		potentialC++;
		return potentialC;
}
int decreasePotentialCPatients()
{
		potentialC--;
		return potentialC;
}

void increaseFlu()
{
	sem_post(&fluPatients);	
}
void increaseCorona()
{
	
	int test1=randomFunction();
	int test2=randomFunction();
		printf("\n-----test1 cornona-----%d----------\n",test1);
		printf("\n-----test2 cornona-----%d----------\n",test2);
	if( (test1 == 0 && test2 == 0) ){
		int *d;
		sem_getvalue(&coronaPatients,&d);
		if(d==0){
			//nothing
			
		}
		else{
			sem_wait(&coronaPatients);
		}
	}
	if(( test1 == 1 && test2 == 1)|| (test1 == 1 && test2 == 0) || (test1 == 0 && test2 == 1))
	sem_post(&coronaPatients);	
}

int randomFunction()
{
		int num=printRandoms(0,1);
		//0 means flu
		//1 means corona
		return num;
		

}
int printRandoms(int lower, int upper) 
{ 
	int num = (rand() % (upper - lower + 1)) + lower; 
        
    return num;
} 






