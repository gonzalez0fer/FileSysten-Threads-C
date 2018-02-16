/*
# ----------------------------------------------------------------------------#
#                                functions.h                                  #
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

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_THREADS 1000



typedef struct TNode *Queue;    //estructura que hara las veces de la cola.

struct TNode                    //estructura Nodo que almacena los datos de
{                               //los directorios a almacenar
    char *Value;
    char *Dad;
    Queue post;
};


void createQueue(Queue* q);
void DestroyQueue(Queue *q);
char* firstQueued(Queue q);
char* firstQueuedDad(Queue q);
void Dequeue(Queue *q);
void Enqueue(Queue *q, char *val, char *val2);
int sizeQueue(Queue q);
void QueueCat(Queue q, Queue *p);
void HELP(); 

#endif /* FUNCTIONS_H_ */