/*
# ----------------------------------------------------------------------------#
#                                functions.c                                  #
# ----------------------------------------------------------------------------#
#                         Universidad Simón Bolívar                           #
#            Departamento de Computación y Tecnología de la Información       #
#                     Laboratorio de Sistemas Operativos                      #
#              Profesora Angela Di Serio y Profesor Carlos Gomez              #
#                       Fernando Gonzalez carnet: 08-10464                    #
#                          Giuli Latella carnet: 08-10596                     #
#                                 Grupo: C                                    #
# ----------------------------------------------------------------------------#
# ----------------------------------------------------------------------------#
*/

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAN_SIZE 1700


/*
* Funcion: HELP
* ------------------
* Funcion que despliega por terminal un cuadro de ayuda que contiene
* utilidad del comando UsoDisco, su sintaxis, parametros, etc, para
* luego terminar.
*
* No recibe parametros y no retorna valores para efectos del programa
*/

void HELP(){

    char helpManual[MAN_SIZE];

    strcpy(helpManual, 
      "\n############################################################\n" 
      "#                      UsoDisco Help                       #\n"  
      "############################################################\n"  
      "#                                                          #\n"  
      "#NOMBRE                                                    #\n"  
      "#   UsoDisco - calcula concurrentemente el espacio en      #\n"  
      "#             disco usado por los archivos regulares       #\n"  
      "#             a partir de un directorio usando hilos.      #\n"  
      "#                                                          #\n"  
      "#                                                          #\n"  
      "#SINOPSIS                                                  #\n"  
      "#  UsoDisco [-h] | [-n I-integer] [-d P-path] [-o F-path]  #\n"  
      "#                                                          #\n"  
      "#      UsoDisco se ejecuta con la opcion [-h] o con        #\n"  
      "#      las opciones [-n], [-d], [-o] pero no ambas         #\n"  
      "#      a la vez.                                           #\n"  
      "#                                                          #\n"  
      "#OPCIONES DE UsoDisco                                      #\n"  
      "#  -n I    es un flag que antecede a un entero I > 0 el    #\n"  
      "#      cual indica el nivel de concurrencia solicitado.    #\n"  
      "#      Por defecto crea un solo hilo trabajador.           #\n"  
      "#                                                          #\n"  
      "#   -d P   es un flag que antecede a la especificacion     #\n"  
      "#      del path P del directorio desde donde se calcula    #\n"  
      "#      el espacio utilizado. Por defecto lo realiza        #\n"  
      "#      desde el directorio actual.                         #\n"  
      "#                                                          #\n"  
      "#  -o F    es un flag que antecede a la especificacion     #\n"  
      "#      del nombre F del archivo que contendra la salida    #\n"     
      "#      con la lista de directorios y el espacio en         #\n"  
      "#      bloques ocupado por los archivos regulares.         #\n"
      "#      Archivo que se creara en el directorio de trabajo.  #\n"  
      "#      El valor por defecto es la salida standard.         #\n"  
      "#                                                          #\n"  
      "############################################################\n" ); 
    printf("%s\n",helpManual);
    exit(0);
}


/*
* Funcion: createQueue
* ----------------------
*Funcion utilizada para crear la cola que contendra
*los directorios a ser analizados por los hilos
*trabajadores.
*
*Recibe un parametro "q" del tipo Queue.
*/
void createQueue(Queue* q){

    (*q) = NULL;
}

/*
* Funcion: firstQueued
* ----------------------
*Funcion que retorna el primer directorio de la 
*cola.
*
*Recibe el parametro "q" del tipo Queue
*Retorna un apuntador al arreglo de caracteres que
*almacena el nombre del directorio cabeza.
*/
char *firstQueued(Queue q){

  if (q!=NULL){
      return q->Value;
  }
}

/*
* Funcion: firstQueuedDad
* ----------------------
*Funcion que retorna el padre del primer directorio de la 
*cola.
*
*Recibe el parametro "q" del tipo Queue
*Retorna un apuntador al arreglo de caracteres que
*almacena el nombre del padre del directorio cabeza.
*/
char *firstQueuedDad(Queue q){

  if (q!=NULL){
      return q->Dad;
  }
}
/*
* Funcion: Dequeue
* ----------------------
*Funcion que desencola un elemento de la lista de
*directorios a analizar por los hilos trabajadores.
*
*Recibe el parametro "*q" del tipo Queue.
*No posee retornos.
*/
void Dequeue(Queue *q){

  if ((*q) != NULL){
        Queue aux = (*q);
        (*q) = (*q)->post;
        free(aux);
     }  
    else{
      return;
    } 
}

/*
* Funcion: Enqueue
* ----------------------
*Funcion que encola un elemento a la lista de
*directorios a analizar por los hilos trabajadores.
*
*Recibe el parametro "*q" del tipo Queue.
*No posee retornos.
*/
void Enqueue(Queue *q, char *val, char *val2){

    if ((*q)==NULL){
        (*q) = (Queue)malloc ( sizeof(struct TNode));
        (*q)->Value = (char *) malloc ( (strlen(val) +1) * sizeof(char) );
        strcpy((*q)->Value, val);
        (*q)->Dad = (char *) malloc ( (strlen(val2) +1) * sizeof(char) );
        strcpy((*q)->Dad, val2);
        (*q)->post = NULL;
    }
    else{
        Queue aux = (*q);
        Queue prev = NULL;
        while((*q)!=NULL){
            prev = (*q);
            (*q) = (*q)->post;
        }
        (*q) = (Queue) malloc ( sizeof (struct TNode));
        (*q)->Value = (char *) malloc ( (strlen(val) +1) * sizeof(char) );
        (*q)->Dad = (char *) malloc ( (strlen(val2) +1) * sizeof(char) );
        strcpy((*q)->Dad, val2);
        strcpy((*q)->Value, val);
        (*q)->post = NULL;
        prev->post = (*q);
        (*q) = aux;
    }
}

/*
* Funcion: QueueCat
* ----------------------
*Funcion que concatena dos colas en una
*sola cola.
*
*Recibe los parametros "q y *p" del tipo Queue.
*los cuales son las dos colas a concatenar.
*No posee retornos.
*/
void QueueCat(Queue q, Queue *p){

  if (q != NULL){
    Enqueue(p,q->Value, q->Dad);
    QueueCat(q->post, p);
  }
}

/*
* Funcion: DestroyQueue
* ----------------------
*Funcion que destruye la cola.
*
*Recibe los parametros "*q" del tipo Queue
*no retorna nada.
*/
void DestroyQueue(Queue *q){
  if ((*q) == NULL){
    return;
  }
  else{
    Queue aux = (*q);
    (*q) = (*q)->post;
    free(aux);
    DestroyQueue(q);
  }
}


/*
* Funcion: sizeQueue
* ----------------------
*Funcion que indica el tamanio de la cola
*de directorios.
*
*Recibe un elemento q del tipo Queue.
*Retorna el numero de directorios encolados.
*/
int sizeQueue(Queue q){
  if (q == NULL){
    return 0;
  }
  else{
    return 1 + sizeQueue(q->post);
  }
}
