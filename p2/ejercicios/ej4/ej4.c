//ej4.c
//Este programa crea dos hebras que incrementan una misma variable global

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

int global = 0;                         //Declaramos la variable global

void * incrementar(){
    for (int i=0; i<10000; i++){        //Incrementamos global 10000 veces
        global++;
    }
    printf("La variable global tras el incremento de la hebra vale %d\n", global);
    pthread_exit(NULL);
}

int main(int argc, const char* argv[]){
    printf("A continuacion se crearan dos hebras las cuales incrementaran 10000 una misma variable global\n");
    
    pthread_t thread[2];                //Se crea el vector para almacenar la direccion de las hebras
    
    for(int i=0; i<2; i++){
        if (pthread_create(&thread[i], NULL, (void *) incrementar, NULL)){
            printf("Error en la creacion de la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if(pthread_join(thread[i], NULL)){
            printf("Error al esperar la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("La variable global tras la creacion e incremento de ambas hebras vale %d\n", global);      //Imprimimos por pantalla el valor de la variable global tras ser incrementada
            exit(EXIT_SUCCESS);

}