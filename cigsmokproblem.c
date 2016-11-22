#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#define MAX 10

void tabaco(void *msgThread);
void fosforo (void *msgThread);
void papel (void *msgThread);
void agente(void *msgThread);

sem_t semAgente,semTabaco,semFosforo,semPapel;

int main(int argc, char *argv[]){
	
	pthread_t thread_Tabaco, thread_Fosforo, thread_Papel, thread_Agente;

	char *msgTabaco="THREAD TABACO iniciada.\n";
	char *msgFosforo="THREAD FOSFORO iniciada.\n";
	char *msgPapel="THREAD PAPEL iniciada.\n";
	char *msgAgente="THREAD AGENTE iniciada.\n";

	sem_init(&semAgente,0,0);
	sem_init(&semTabaco,0,0);
	sem_init(&semFosforo,0,0);
	sem_init(&semPapel,0,0);

	pthread_create(&thread_Tabaco,NULL,(void*)&tabaco,(void*)msgTabaco);
	pthread_create(&thread_Fosforo,NULL,(void*)&fosforo,(void*)msgFosforo);
	pthread_create(&thread_Papel,NULL,(void*)&papel,(void*)msgPapel);
	pthread_create(&thread_Agente,NULL,(void*)&agente,(void*)msgAgente);

	pthread_join(thread_Agente,NULL);

	sem_destroy(&semAgente);
	sem_destroy(&semTabaco);
	sem_destroy(&semFosforo);
	sem_destroy(&semPapel);

	return 0;
}

void agente(void *msgThread){
	char *msg = (char*)msgThread;
	printf("%s\n",msg);
	sleep(0.1);
	srand(time(0));	
	for (int i=0;i<MAX;i++){
		printf("================= AGENTE ====================\n\n");
		switch(rand()%3){
			case 0:
				printf("O Agente colocou o TABACO na mesa!\n");
				printf("O Agente colocou o PAPEL na mesa!\n");
				printf("O Agente liberou a mesa para o FOSFORO SMOKER\n");
				printf("=============================================\n\n");
				sem_post(&semFosforo);
				sem_wait(&semAgente);
				break;
			case 1:
				printf("O Agente colocou o FOSFORO na mesa!\n");
				printf("O Agente colocou o TABACO na mesa!\n");
				printf("O Agente liberou a mesa para o PAPEL SMOKER\n");
				printf("=============================================\n\n");
				sem_post(&semPapel);
				sem_wait(&semAgente);
				break;
			case 2:
				printf("O Agente colocou o PAPEL na mesa!\n");
				printf("O Agente colocou o FOSFORO na mesa!\n");
				printf("O Agente liberou a mesa para o TABACO SMOKER\n");
				printf("=============================================\n\n");
				sem_post(&semTabaco);
				sem_wait(&semAgente);
				break;
		}
	}
	pthread_exit(0);
}

void tabaco (void *msgThread){
	char *msg = (char*)msgThread;
	printf("%s\n",msg);

	while(1){
		sem_wait(&semTabaco);
		printf("================= TABACO SMOKER =================\n\n");
		printf("Smoker Tabaco pega o PAPEL e o FOSFORO\n");
		printf("Mesa Liberada!\n");
		sem_post(&semAgente);
		printf("SMOKER Tabaco esta fumando.\n");
	}			
}

void fosforo(void *msgThread){
	char *msg = (char*)msgThread;
	printf("%s\n",msg);

	while(1){
		sem_wait(&semFosforo);
		printf("================= FOSFORO SMOKER =================\n\n");
		printf("Smoker Fosforo pega o PAPEL e o TABACO\n");
		printf("Mesa Liberada!\n");
		sem_post(&semAgente);
		printf("SMOKER Fosforo esta fumando.\n");
	}
}

void papel(void *msgThread){
	char *msg = (char*)msgThread;
	printf("%s\n",msg);
	while(1){
		sem_wait(&semPapel);
		printf("================= PAPEL SMOKER =================\n\n");
		printf("Smoker Papel pega o TABACO e o FOSFORO\n");
		printf("Mesa Liberada!\n");
		sem_post(&semAgente);
		printf("SMOKER Papel esta fumando.\n");
	}
}
