#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>



#define LINHA  22000
#define COLUNA  22000
#define NUMLINHAMACRO LINHA
#define NUM_THREADS 8


typedef struct arg_struct {
    int id;
    int value;
}arguments;


int numPrimos = 0;
int matrix[LINHA][COLUNA];
int manager[NUMLINHAMACRO];
int status[NUM_THREADS];
pthread_mutex_t lock;

void initMacroManager(){
    for(int i = 0 ; i < NUMLINHAMACRO; i ++){
        manager[i] = 0;
    }
}
int hasToProcess(){
    int flag = 0;
    for (int i = 0 ; i <NUMLINHAMACRO; i ++){
        if(manager[i] == 0){
            return 1;
        }
    }
return flag;
}
int getNextMacro(){
    int flag = 0;
    if(hasToProcess() == 1){
        for (int i = 0 ; i <NUMLINHAMACRO; i ++){
            if(manager[i] == 0 ){
                return i;
            }
        }
    }else return -1;


}
int isPrime(const int number)
{
    if(number <= 1)
       return 0;
    int i, r, prime = 1;
    if(number != 2 && number % 2 == 0)
       prime = 0;
    else
        for(i = 3, r = sqrt(number) ; i <= r && prime ; i += 2)
            if (number % i == 0)
                prime = 0;
    return prime;
}
void initMatrix(){
    for(int i = 0 ; i <LINHA ; i ++){
    	for(int j = 0 ; j <COLUNA ; j ++){
            matrix[i][j] = rand()%29999;
        }
	}
}
int countPrimes(int line){
    int countPrime = 0;
    for(int i = 0; i < NUMLINHAMACRO; i ++){
            if(isPrime(matrix[line][i])){
                countPrime++;
        }
    }
    return countPrime;

}
void *worker(void* arg){
    arguments* auxArgs = (arguments*) arg;
    int idd = auxArgs->id;
    int value = auxArgs->value;
    //printf("%d-%d", id,value);
    int aux = 0;

       // printf("thred %d, numero %d nuemro de primos%d\n",id,value,numPrimos);
        aux = aux + countPrimes(value);
       // printf("valor do aux %d\n",aux);
    pthread_mutex_lock(&lock);
    numPrimos = numPrimos+aux;
    pthread_mutex_unlock(&lock);

    status[idd] = 2;
    pthread_exit(NULL);


}

int serial(){
    int aux = 0;
    for(int i = 0 ; i < NUMLINHAMACRO; i++){
        aux = aux + countPrimes(i);
        //printf("%d",aux);
    }

return aux;
}
void initStatusTheread(){
    for(int i = 0 ; i < NUM_THREADS ; i ++){
        status[i] = 0;
    }
}
int parallel(){
    numPrimos = 0;
    int countPrecessed = 0;
    pthread_t threads[NUM_THREADS];
    while(countPrecessed < NUMLINHAMACRO){

        for(int i = 0 ; i < NUM_THREADS ; i ++){
            if(status[i] == 0){
                status[i] = 1;
                int value = getNextMacro();
                manager[value] = 1;
                arguments* arg = (arguments*) malloc(sizeof(arguments));
                arg->id = i;
                arg->value = value;
                countPrecessed++;
                int ret =  pthread_create(&threads[i], NULL, &worker, (void*)arg);
            }else if(status[i] == 2){ status[i] = 0;}
        }

   }
    for(int i = 0 ; i < NUM_THREADS ; i ++){
        pthread_join(threads[i], NULL);
   }


return numPrimos;
}
int main(){


    //int valueserial = serial();
    //printf("%d\n",valueserial);
    initMacroManager();
    initMatrix();
    // numPrimos = countPrimes(0,0,LINHA,COLUNA);
   // printf("%d",numPrimos);
    initStatusTheread();
    clock_t t;
    t = clock();
     printf("%d\n",serial());
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
     printf("Serial demorou %f segundos\n", time_taken);

    t = clock();
    printf("%d\n",parallel());
     printf("%d\n",numPrimos);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
     printf("paralelo demorou %f segundos\n", time_taken);





}
