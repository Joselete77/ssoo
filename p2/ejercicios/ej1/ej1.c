//ej1.c
//Este ejercicio creara n hebras, creando cada una de ellas 2 valores aleatorios e imprimiendo el sumatorio de todos ellos al final del programa
//Pide macros definidas (????)
//Hace falta hacer malloc????

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

int errno;              //La creo aqui para que no me salga que errno esta undefined

void * valores_aleatorios(int *sumatorio){              //Funcion que genera y suma dos numeros aleatorios
    
    int x=1+rand()%9;                                   //Generamos un numero aleatorio del 1 al 9
    printf("Valor de x= %d\n", x);

    int y=1+rand()%9;                                   //Generamos un numero aleatorio del 1 al 9
    printf("Valor de y= %d\n", y);
    
    printf("Suma de la hebra actual = %d\n", x+y);

    *sumatorio = *sumatorio +x+y;                       //Incrementamos la suma de los valores totales obtenidos

    pthread_exit(NULL);                                 //Salimos de la hebra

}

int main(){
    int n;                              //Numero de hebras a crear
    printf("Numero de hebras: ");
    scanf("%d", &n);                    //El usuario introduce el numero de hebras que quiere crear
    
    srand(time(NULL));                  //Plantamos la semilla del numero aleatorio
    
    pthread_t thread[n];                //Creamos un vector para almacenar el valor de los hilos

    int suma=0;                         //Establecemos que al comenzar la suma de valores valga 0

    for (int i=0; i<n; i++){            //Entraremos en el bucle tantas veces como hebras creemos
        
        if (pthread_create(&(thread[i]), NULL, (void *) valores_aleatorios, &suma)){        //Si la creacion de la hebra nos devuelve algo distinto a 0, habra ocurrido un error
            printf("Error en la creacion de la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("Creacion de la hebra %d\n", i+1);

        if (pthread_join(thread[i], NULL)){                                                 //Si la espera de la hebra nos devuelve algo distinto a 0, habra ocurrido un error
            printf("Error al esperar la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
        printf("Suma total = %d\n",suma);       //Imprimimos finalmente el sumatorio de todos los valores creados por las n hebras
        exit (EXIT_SUCCESS);
        
}