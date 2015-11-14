#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

//------------------------------------
//NAME: Emiliyan Sokolov
//CLASS: 11b 
//NUMBER: 15
//PROBLEM: StarCraft
//FILE_NAME: Emiliqn_Sokolov_15.c
//FILE_PURPOSE: This file contains the program
//---------------------------------------

pthread_mutex_t* lock;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_t *SCV;
int workers = 5;
int minerals = 5000;
int remaining_minerals = 5000;
int collected_minerals = 0;
int soldiers = 0;
int scv_alloc_counter = 6;
int mutex_alloc_counter = 2;

//------------------------------------
//FUNCTION: thread_func
//This is the main function for each worker thread.
//Here workers are mining,transporting etc.
//
//PARAMETERS: void* param
//The default parametar of the func
//This parametar receives the pthread_create arguments to this func
//------------------------------------

void* thread_func(void* param) {
	int index = (intptr_t)param;
	int flag = 0;
	int i;
	index+=1;
	
	while(remaining_minerals != 0 ){  
		if(flag != 1) {
			flag = 1;
			pthread_mutex_lock(&lock2);
			printf("SCV %d is mining\n",index);
			remaining_minerals-=8;
			pthread_mutex_unlock(&lock2);
			printf("SCV %d is transporting minerals\n",index);
			sleep(2);
		}
		for(i = 0; i < mutex_alloc_counter-1; i++) {		
			if(pthread_mutex_trylock(&lock[i]) == 0) {
				pthread_mutex_lock(&lock3);
				collected_minerals+=8;
				pthread_mutex_unlock(&lock3);
				pthread_mutex_unlock(&lock[i]);
				printf("SCV %d delivered minerals to Command center %d\n",index,i+1);
				flag = 2;
				break;
			}
		}
		if(flag != 2) {
			sleep(1);
		}
	}

	return NULL;
}

//-------------------------------
//FUNCTION: training_soldier
//This function is for creating the soldiers 
//PARAMETERS: Doesn't have
//-------------------------------
void training_soldier() {
	if(collected_minerals >= 50){
		pthread_mutex_lock(&lock3);
		collected_minerals-=50;
		pthread_mutex_unlock(&lock3);
		sleep(1);
		soldiers+=1;
		printf("You wanna piece of me, boy ?\n");
		if(soldiers == 20) {
			int tmp = minerals - remaining_minerals;
			printf("All minerals: %d, Remaining minerals: %d, Collected minerals: %d\n",minerals,remaining_minerals,tmp);
			exit(1);
		}
	}
	else{
		printf("Not enough minerals.\n");
	}

}

//-----------------------------
//FUNCTION: add_center
//This function is for adding new Command centers
//PARAMETERS: Doesn't have
//------------------------------

void add_center() {
	if(collected_minerals >= 400){
			pthread_mutex_lock(&lock3);
			collected_minerals-=400;
			pthread_mutex_unlock(&lock3);
			sleep(2);
			mutex_alloc_counter++;
			lock = realloc(lock,mutex_alloc_counter*(sizeof(pthread_mutex_t)));
			printf("Command center %d created.\n",mutex_alloc_counter-1);
	}
	else {
		printf("Not enough minerals.\n");
	}
	

}
//-----------------------------------------
//FUNCTION: training_worker
//This function creates the new workers
//PARAMETERS: Doesn't have
//-----------------------------------------

void training_worker() {
	if(collected_minerals >= 50) {
		pthread_mutex_lock(&lock3);
		collected_minerals -=50;
		pthread_mutex_unlock(&lock3);
		sleep(1);
		if(workers != 0 ){
			scv_alloc_counter++;
			SCV = realloc(SCV,(scv_alloc_counter*sizeof(pthread_t)));
			int rc = pthread_create(&SCV[workers],NULL,thread_func,(void *)(intptr_t)workers);
			if(rc) {
				printf("ERROR: pthread_create() return %d\n",rc);
				exit(-1);
			}
			workers++;
		}
		printf("SCV good to go, sir.\n");
	}
	else {
		printf("Not enough minerals.\n");
	}
}
//------------------------------------
//FUNCTION: st_input
//This function reads the contents of the standard input and depending on what character reads,
//calls the following function.
//This is the st_input_thread function
//PARAMETER: void* arg
//The default parameter of the function, which is not used in this case
//------------------------------------
void* st_input(void* arg) {
	char c;
	while(read(STDIN_FILENO, &c, 1) > 0) {   //checking each character 
		if(c == 'm') {
			training_soldier();
		}
		else if(c == 's') {
			training_worker();
		}
		else if(c == 'c') {
			add_center();
		}
		
	}
	return NULL;
}

//-----------------
//FUNCTION: main
//The main function of the program
//PARAMETER: Doesn't have
//-----------------------------------

int main() {
	//workers
	pthread_t st_input_thread;
	int rc;
	SCV = malloc(scv_alloc_counter*sizeof(pthread_t));		//allocating memory
    	lock = malloc(mutex_alloc_counter*sizeof(pthread_mutex_t));

    	rc = pthread_mutex_init(lock,NULL);
  		if(rc)	{
    			printf("ERROR: pthread_mutex_init() return %d\n",rc);
			exit(-1);
   	 	}
	rc = pthread_mutex_init(&lock2,NULL);
		if(rc) {
    			printf("ERROR: pthread_mutex_init() return %d\n",rc);
			exit(-1);
   	 	}
      	rc = pthread_mutex_init(&lock3,NULL);
      		if(rc) {
      			printf("ERROR: pthread_mutex_init() return %d\n",rc);
			exit(-1);
		}
      		

	for(int i = 0; i < workers; ++i) {

		rc = pthread_create(&SCV[i],NULL,thread_func,(void *)(intptr_t)i);  //creating workers threads
		if(rc) {
			printf("ERROR: pthread_create() return %d\n",rc);
			exit(-1);
		}

	}

	rc = pthread_create(&st_input_thread,NULL,st_input,NULL);
	if(rc) {
		printf("ERROR: pthread_create() return %d\n",rc);
		exit(-1);
	}

	for(int i = 0; i < workers; ++i) {
		pthread_join(SCV[i],NULL);				//waiting for workers threads to terminate
	}
	workers = 0;
	pthread_join(st_input_thread,NULL);

	rc = pthread_mutex_destroy(lock);
	if(rc){
		printf("ERROR: pthread_mutex_destroy() return %d\n",rc);
		exit(-1);
	}
	rc = pthread_mutex_destroy(&lock2);
	if(rc){
		printf("ERROR: pthread_mutex_destroy() return %d\n",rc);
		exit(-1);
	}
	rc = pthread_mutex_destroy(&lock3);
	if(rc){
		printf("ERROR: pthread_mutex_destroy() return %d\n",rc);
		exit(-1);
	}

	
	free(SCV);			//freeing memory
	free(lock);

	return 0;
}
