/*
# ----------------------------------------------------------------------------#
#                                UsoDisco.h                                   #
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

#ifndef USODISCO_H_
#define USODISCO_H_
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DIR 2000



typedef struct                  //estructura creada para almacenar los datos de cada hilo.
{
    int id;                     // posee un  campo para la identidad, uno para el estado
    char *dir;                  // (ocupado, libre),uno que almacena el directorio de trabajo
    int bussy;                  // mas un campo para el retorno.
    int gotIt;
    int size;
    char *returns;
    char *dad;
    int numberChilds;
    Queue q;
} DataThread;
DataThread Table[MAX_THREADS]; //arreglo de estructuras que almacenan los datos del hilo.


typedef struct dir 			//estructura creada para almacenar los datos de los 
{							//directorios que han sido analizados por pada hilo
    char *id;				//donde se almacena el path del directorio, el
    char *Dad;				//del padre, el numero de bloques que ocupa por
    int selfBlocks;			//si mismo, el numero de subdirectorios que tiene
    int numberChilds;		//el numero de bloques que ocupan sus subdirectorios
    int BlocksChilds;		//y un entero que hace las veces de booleano que 
    int printed;			//indica si ya fue impreso el resultado.
} dirInfo;
dirInfo List[MAX_DIR];		//arreglo que almacena los datos de los directorios analizados.



int findDir(char *dir);
int StillnumChilds();
int StillBussy();
void WorkerTread(DataThread *current);
int findDir(char *dir);

#endif /* USODISCO_H_ */