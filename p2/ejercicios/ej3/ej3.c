//ej3.c
//Este ejercicio creara un vector de 10 elementos relleno con numeros aleatorios entre 1 y 9, y usando hebras suma sus valores
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

typedef struct {    //Estructura matriz que pasaremos a las funciones que manejan los hilos
  int **matriz;     //Matriz
  int nFil;         //Filas de la matriz
  int nCol;         //Columnas de la matriz
  int i;            //Iterador para acceder a los diferentes vectores de la matriz
} matriz;


int ** reservarMemoria(int nFil, int nCol){             //Reservamos memoria para la matriz
	int **matriz;
	matriz = (int **)malloc (nFil*sizeof(int *));       //Reservamos memoria para el vector de filas
	for (int i=0;i<nFil;i++){
		matriz[i] = (int *) malloc (nCol*sizeof(int));  //Reservamos memoria para los vectores de las columnas
	}
	return matriz;                                      //Devolvemos la matriz
}


void rellenaMatriz(int **matriz, int nFil, int nCol){   //Rellenamos la matriz de numeros aleatorios del 1 al 9
  srand(time(NULL));                                    
	for (int i = 0; i < nFil; ++i){
		for (int j = 0; j < nCol; ++j){
			matriz[i][j] = ((rand()%9)+1);             
		}
	}
}

void imprimeMatriz (int **matriz, int nFil, int nCol){  //Imprimimos los elementos de la matriz
	printf("La matriz es:\n");
	for (int i = 0; i < nFil; ++i){
		printf("|");
		for (int j = 0; j < nCol; ++j){
			printf(" %d ",matriz[i][j]);
		}
		printf("|\n");
	}
}


void * sumaVector (void* d){            //Esta será la función que ejecuten las hebras.

    int *suma = malloc(sizeof(int));    //Reservamos memoria para un puntero de tipo int
    *suma = 0;                          //Como lo vamos a usar de contador lo ponemos a 0
    
    matriz *m;                          
    m = (matriz*) d;
    
    int n = ++m->i;                     //Almacenamos el valor de i en n con el objetivo de tener el resto de hebras el menor tiempo posible bloqueadas ya que la otra opción es trabajar con i directamente y mantenerlas bloqueadas mientras se ejecuta el bucle
    for (int j = 0; j < m->nCol; j++){
        *suma += m->matriz[n][j];       //Accedemos únicamente a las columnas de la fila i de la matriz lo que equivale a uno de los vectores
    }

	pthread_exit((void**) suma);        //Devolvemos el valor del numero de lineas con pthread_exit
}

int main(int argc, char const *argv[]){
  
    matriz *m = malloc(sizeof(matriz));
    m->i=-1;                                                //Partimos de -1 para que en la primera iteracion al entrarnen la funcion, valga 0
    m->nFil = atoi(argv[1]);                                //Guardamos en nFil el número pasado como argumento ya que este será el numero de filas de la matriz de numeros y el numero de hilos que generaremos
    
    if (argc < 2 || (m->nFil != 2 && m->nFil != 5)){        //Comprobamos que se haya llamado al programa de forma adecuada
        printf("Error, llame al programa asi:\n");
        printf("./ej3 2 o /ej3 5\n");
    }
    
    m->nCol = 10/m->nFil;                                   //Creamos un puntero para la matriz y definimos el número de columnas que será 10/nFil
    m->matriz = reservarMemoria (m->nFil, m->nCol);         //Reservamos memoria para la matriz

    rellenaMatriz(m->matriz, m->nFil, m->nCol);             //Rellenamos la matriz con numeros aleatorios del 1 al 9
    imprimeMatriz(m->matriz, m->nFil, m->nCol);             //Imprimimos la matriz para comprobar luego el resultado

    pthread_t thread[m->nFil];                              //Creamos un vector para almacenar los identificadores de los hilos
    int *ret, suma = 0;                                     //ret es el puntero que recibirá los valores devueltos por los hilos, suma almacenará la suma de los valores devueltos por los hilos
    
    for (int i = 0; i < m->nFil; i++){                      //Crearemos tantos hilos como filas tenga la matriz
        
        if(pthread_create(&(thread[i]), NULL, (void*) sumaVector, (void*) m)){
            printf("Error en la creacion de la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        
        }

        if(pthread_join(thread[i], (void**) &ret)){
            printf("Error al esperar la hebra. Codigo de error %d\n", errno);
            exit(EXIT_FAILURE);
        } 

        suma = suma + *ret;                                 //En cada iteración del bucle sumamos el valor devuelto por la hebra recogida
    }

    printf("La suma de los numeros de todos los valores es: %d\n", suma);
    exit(EXIT_SUCCESS);
}     
