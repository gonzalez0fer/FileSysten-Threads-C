/*
# ----------------------------------------------------------------------------#
#                         Universidad Simón Bolívar                           #
#            Departamento de Computación y Tecnología de la Información       #
#                     Laboratorio de Sistemas Operativos                      #
#              Profesora Angela Di Serio y Profesor Carlos Gomez              #
# ----------------------------------------------------------------------------#                               
#                                                                             #                         
#            ______         <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<          #        
#         .-'      `-.         ______    ______    ______                     #  
#       .'            `.      |------|  |------|  |------|     <------        #
#      /                \     |  '''  | |  '''  | |  '''  |                   #
#     ;     _______      ;`   |_______| |_______| |_______|                   #
#     |    |------|      |; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<          #
#     ;    |  '''  |     ;| _   _          _____                              #
#     '\   |_______|     / ; | | |___  ___ |  _ \(_)___  ___ ___              #
#      \`.           .' /  | | | / __|/ _ \| | | | / __|/ __/ _ \             #
#       `.`-._____.-' .'   | |_| \__ \ (_) | |_| | \__ \ (_| (_) |            #
#         / /`_____.-'      \___/|___/\___/|____/|_|___/\___\___/             #
#        / / /                                                                #
#       / / /               Fernando Gonzalez carnet: 08-10464                #
#      / / /                    Giuli Latella carnet: 08-10596                #
#     / / /                                Grupo: C                           #
#    / / /                                                                    #
#    \/_/                                                                     #
# ----------------------------------------------------------------------------#
# ----------------------------------------------------------------------------# 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include "functions.h"
#include "UsoDisco.h"

int numberDir = 0;              //variable global que lleva el tamanio del arreglo de archivos.
int totalDir = 0;               //variable que lleva el total de directorios analizados.
int numberThreads;              //variable que almacena el numero de hilos trabajadores a crear.
int outputBool;			        //variable de apoyo para definir canal de la salida.
char *outputDir;                //variable que almacena la ruta del archivo de salida.
char *inputDir;                 //variable que almacena la ruta del directorio a analizar.
DIR *inputDirD;			        //variable que almacena el descriptor del archivo a ser analizado.
FILE *outputDirD; 		        //variable que almacena el descriptor del de salida.
int Summatory;                  //variable que lleva el tamanio en bloques del directorio de entrada.




////////////////////////////////////////////////////////////////////
//****************************************************************//
//***************************  MAIN  *****************************//
//****************************************************************//
////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) 
{


    char *path = getenv("PWD");	    //almacena el path del directorio de trabajo.
    outputBool = 0;				    //si outputBool es cero, la salida es por terminal.
    struct dirent *conten;          //estructura definida para la libreria dirent.h para recorrer Filesystem.
    struct stat dataFile;           //estructura del stat que contiene los datos a consultar.
    char forPrint[1000];            //buffer de impresion.
    char aux[1000];                 //arreglos de caracteres auxiliares.                 
    char aux2[1000];
    int i;
    Queue Directory;                //inicializacion de una cola para los directorios a explorar.
    createQueue(&Directory);

    /*
    *Si la invocacion no posee parametros, el programa correra, mostrara el 
    *analisis para el directorio de trabajo actual por salida standard con 
    *solo un hilo trabajador.
    */
    if (argc==1) {

        inputDir = (char*) malloc(strlen(path)* sizeof(char));
        strcpy(inputDir,path);
        numberThreads =1;
    }

    /*
    *Si la invocacion del programa posee un solo parametro.
    *Solo es valido el flag -h que llama a la funcion help,
    *pues los demas flags requieren de mayor numero de parametros
    */ 
    else if(argc == 2) { 
      

        if (!strcmp(argv[1], "-h")){
            HELP();
        }

        else if (!strcmp(argv[1], "-n") || !strcmp(argv[1], "-o") 
            || !strcmp(argv[1], "-d")){

            printf("\nERROR: Invocacion erronea: faltan valores de entrada..."
             "\nPruebe con './UsoDisco -h' para verificar sintaxis.\n\n");
            exit(0);
        }

        else{

            printf("\nERROR: Invocacion erronea: flags erroneos... \nPruebe con "
                "'./UsoDisco -h' para verificar sintaxis.\n\n");
            exit(0);

        }
    }

    /*
    *Si la invocacion del programa posee dos parametros, este solo
    *podra ser un flag valido ('-n', '-d, '-o') y su parametro asociado.
    */ 
    else if (argc == 3) {

    	//Verifico si es un flag valido.
        if ((!strcmp(argv[1], "-n")) || (!strcmp(argv[1], "-o"))
            || (!strcmp(argv[1],"-d"))){

        	/*
        	*El programa correra con el numero de hilos trabajadores
        	*indicado en la invocacion, directorio de trabajo actual
        	*para ser analizado y la salida sera la standard.
        	*/
            if ((!strcmp(argv[1], "-n") && atoi(argv[2]) > 0)){
                inputDir = (char*) malloc(strlen(path)* sizeof(char));
                strcpy(inputDir,path);
                numberThreads = atoi(argv[2]);
            }
            /*
            El programa correra con un solo hilo trabajador, el directorio
            *a ser analizado sera el directorio actual de trabajo y la
            *salida sera en un archivo con el nombre indicado en la 
            *invocacion y se guardara en el directorio de trabajo actual.
            */
            else if (!strcmp(argv[1], "-o") ){
                inputDir = (char*) malloc(strlen(path)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[2]) + strlen(path)+2)
                	* sizeof(char));
                strcpy(inputDir,path);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[2]);
                outputBool = 1;
                numberThreads = 1;
            }
            /*
            *El programa correra con un solo hilo trabajador, analizando el
            *directorio indicado en la invocacion y su salida sera la 
            *standard por terminal
            */
            else if(!strcmp(argv[1],"-d")){

                inputDir = (char*) malloc((strlen(argv[2])+1)* sizeof(char));
                strcpy(inputDir,argv[2]);
                numberThreads = 1;
 
            }
            else{
                printf("\nERROR: Invocacion erronea: argumento del tipo" 
                "erroneo... \nPruebe con './UsoDisco -h' para verificar "
                "la sintaxis.\n\n");
                exit(0); 

            }       
        }
        else{
            printf("\nERROR: Invocacion erronea: flag erroneo... \nPruebe "
                "con './UsoDisco -h' para verificar la sintaxis.\n\n");
            exit(0);  
        }                         
    }

    /*
    *Si la invocacion posee 3 argumentos, no correra pues haran falta 
    *parametros.
    */
    else if (argc==4){
            printf("\nERROR: Invocacion erronea: faltan argumentos..." 
            	"\nPruebe con './UsoDisco -h' para verificar la sintaxis.\n\n");
            exit(0);       
    }

     /*
    *Si la invocacion del programa posee cuatro parametros, estos solo
    *podran ser con flags validos ('-n', '-d, '-o') y sus parametros 
    *asociados del tipo correcto.
    */    
    else if (argc==5) {
    	//Verifico que no existan flags iguales.
        if (strcmp(argv[1], argv[3])){

        	/*
        	*El programa corerra con el numero de hilos trabajadores indicados
        	*en la invocacion, analizando el directorio indicado en la misma
        	*e imprimiendo por la salida standard.
        	*/
            if ( !strcmp(argv[1], "-n") && atoi(argv[2]) > 0 && 
            	!strcmp(argv[3],"-d") ){

                inputDir = (char*) malloc((strlen(argv[4])+1)* sizeof(char));
                strcpy(inputDir,argv[4]);
                numberThreads = atoi(argv[2]);
            }
        	/*
        	*El programa corerra con el numero de hilos trabajadores indicados
        	*en la invocacion, analizando el directorio indicado en la misma
        	*e imprimiendo por la salida standard.
        	*/
            else if (!strcmp(argv[1],"-d") && !strcmp(argv[3], "-n") 
            		&& atoi(argv[4]) > 0) {

                inputDir = (char*) malloc((strlen(argv[2])+1)* sizeof(char));
                strcpy(inputDir,argv[2]);
                numberThreads = atoi(argv[4]);

            } 

        	/*
			*El programa correra con el numero de hilos trabajadores indicados
			*en la invocacion, y el nombre del archivo donde estara indicada
			*la salida del programa.
        	*/
            else if ( !strcmp(argv[1], "-n") && atoi(argv[2]) > 0 && 
            	!strcmp(argv[3],"-o")){

                inputDir = (char*) malloc((strlen(path)+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[4])+strlen(path)+1)
                    * sizeof(char));
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[4]);
                outputBool = 1;
                strcpy(inputDir,argv[2]);
                numberThreads = atoi(argv[2]); 

            }

      		/*
			*El programa correra con el numero de hilos trabajadores indicados
			*en la invocacion, y el nombre del archivo donde estara indicada
			*la salida del programa.
        	*/
            else if (!strcmp(argv[1],"-o") && !strcmp(argv[3], "-n")
                && atoi(argv[4]) > 0){

                inputDir = (char*) malloc((strlen(path)+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[2])+strlen(path)+1)
                    * sizeof(char));
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[2]);
                outputBool = 1;
                strcpy(inputDir,path);
                numberThreads = atoi(argv[4]); 

            }

            /*
            *El programa correra con un solo hilo trabajador, analizando
            *el directorio indicado en la invocacion y el nombre del
            *directorio de salida.
            */
            else if ( !strcmp(argv[1], "-o")  && !strcmp(argv[3],"-d")){

                inputDir = (char*) malloc((strlen(argv[4])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[2])+strlen(path)+1)
                    * sizeof(char));
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[2]);
                outputBool = 1;
                strcpy(inputDir,argv[4]);
                numberThreads = 1; 

            }

            /*
            *El programa correra con un solo hilo trabajador, analizando
            *el directorio indicado en la invocacion y el nombre del
            *directorio de salida.
            */
            else if (!strcmp(argv[1],"-d") && !strcmp(argv[3], "-o")){

                inputDir = (char*) malloc((strlen(argv[2])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[4])+strlen(path)+1)
                    * sizeof(char));
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[4]);
                outputBool = 1;
                strcpy(inputDir,argv[2]);
                numberThreads = 1; 
            }
            else{
                printf("\nERROR: Invocacion erronea: argumento del tipo" 
               " erroneo... \nPruebe con './UsoDisco -h' para verificar "
                "la sintaxis.\n\n");
                exit(0); 
            }
        } 
        
        else{
            printf("\nERROR: Invocacion erronea: flags erroneos... \nPruebe "
                "con './UsoDisco -h' para verificar la sintaxis.\n\n");
            exit(0);
        }
    }

    /*
    *Si la invocacion posee 5 argumentos, no correra, pues haran falta 
    *parametros.
    */
    else if (argc==6){
            printf("\nERROR: Invocacion erronea: faltan argumentos... \nPruebe "
            "con './UsoDisco -h' para verificar la sintaxis.\n\n");
            exit(0);       
    }

    /*
    *Si la invocacion del programa posee seis parametros, estos solo
    *podran ser con flags validos ('-n', '-d, '-o') y sus parametros 
    *asociados del tipo correcto.
    */       
    else if (argc==7) {
        //Verifico que los flags sean diferentes el uno del otro...
        if (strcmp(argv[1], argv[3]) && strcmp(argv[3], argv[5]) 
            && strcmp(argv[5], argv[1])){
            /*
            *En todas estas instancias el programa correra con todos los
            *argumentos introducidos en la invocacion, es decir:
            *numero de hilos trabajadores, directorio a ser analizado
            *y el nombre del archivo de salida a ser creado en la carpeta
            *de trabajo actual.
            */
            if ( !strcmp(argv[1], "-n") && atoi(argv[2]) > 0 
                && !strcmp(argv[3],"-d") && !strcmp(argv[5],"-o") ){

                inputDir = (char*) malloc((strlen(argv[4])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[6])+strlen(path)+1)
                    * sizeof(char));
                strcpy(inputDir,argv[4]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[6]);
                outputBool = 1;
                numberThreads = atoi(argv[2]);

            }
            /*
            """""""""""""""""""
            """""""""""""""""""
            */
            else if ( !strcmp(argv[1], "-n") && atoi(argv[2]) > 0 
                && !strcmp(argv[3],"-o") && !strcmp(argv[5],"-d") ){

                inputDir = (char*) malloc((strlen(argv[6])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[4])+strlen(path)+1)
                    * sizeof(char));
                strcpy(inputDir,argv[6]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[4]);
                outputBool = 1;
                numberThreads = atoi(argv[2]);

            }
            /*
            """""""""""""""""""
            """""""""""""""""""
            */
            else if ( !strcmp(argv[1], "-d") && !strcmp(argv[3],"-n") 
                && atoi(argv[4]) > 0 && !strcmp(argv[5],"-o") ){

                inputDir = (char*) malloc((strlen(argv[2])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[6])+strlen(path)+1)* 
                    sizeof(char));
                strcpy(inputDir,argv[2]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[6]);
                outputBool = 1;
                numberThreads = atoi(argv[4]);
            } 
            /*
            """""""""""""""""""
            """""""""""""""""""
            */
            else if (  !strcmp(argv[1], "-d") && !strcmp(argv[3],"-o") 
                && !strcmp(argv[5],"-n") && atoi(argv[6]) > 0 ){


                inputDir = (char*) malloc((strlen(argv[2])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[4])+strlen(path)+1)* 
                    sizeof(char));
                strcpy(inputDir,argv[2]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[4]);
                outputBool = 1;
                numberThreads = atoi(argv[6]);
             }
            /*
            """""""""""""""""""
            """""""""""""""""""
            */
            else if (  !strcmp(argv[1], "-o") && !strcmp(argv[3],"-n") 
                && atoi(argv[4]) > 0 && !strcmp(argv[5],"-d") ){

                inputDir = (char*) malloc((strlen(argv[6])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[2])+strlen(path)+1)
                    * sizeof(char));
                strcpy(inputDir,argv[6]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[2]);
                outputBool = 1;
                numberThreads = atoi(argv[4]);
            }
            /*
            """""""""""""""""""
            """""""""""""""""""
            */ 
            else if (  !strcmp(argv[1], "-o") && !strcmp(argv[3],"-d")
                && !strcmp(argv[5],"-n") && atoi(argv[6]) > 0){

                inputDir = (char*) malloc((strlen(argv[4])+1)* sizeof(char));
                outputDir = (char*) malloc((strlen(argv[2])+ strlen(path)+1)
                    * sizeof(char));
                strcpy(inputDir,argv[4]);
                strcpy(outputDir,path);
                strcat(outputDir,"/");
                strcat(outputDir, argv[2]);
                outputBool = 1;
                numberThreads = atoi(argv[6]);
            }    
   
            else{
                printf("\nERROR: Invocacion erronea: error de tipos... \nPruebe "
                "con './UsoDisco -h' para verificar la sintaxis.\n\n");
                exit(0);   
            }
        }

        else{
            printf("\nERROR: Invocacion erronea: flags erroneos... \nPruebe "
                "con './UsoDisco -h' para verificar la sintaxis.\n\n");
            exit(0);
        }
    }


    /*
    *Abre el archivo a ser analizado, si no existe regreso un mensaje
    *de error avisandole al usuario.
    */
    inputDirD = opendir(inputDir);
    if (!inputDirD){
        printf("\nERROR: Invocacion erronea: el directorio a ser "
            "analizado no existe.. \nPruebe con otra ruta para "
            " analizar...\n\n");
        exit(0);
    } 

    /*
    *Si el metodo de salida es por un archivo, si existe previamente
    *lo elimino y lo vuelvo a crear con el flag de append.
    */
    if (outputBool){
        
        remove(outputDir);
        outputDirD = fopen(outputDir, "a+");
    }

    


    //////////////////////////////////////////////////////////////
    //**********************************************************//
    //*********  Analizando el directorio de entrada  **********//
    //**********************************************************//
    //////////////////////////////////////////////////////////////


    while( (conten = readdir(inputDirD)) != NULL){
        //Almacenando la cantidad de bloques que ocupa 
        //el directorio de entrada por si mismo.

        if (!strcmp(conten->d_name,".")){
            lstat(inputDir, &dataFile);
            Summatory = Summatory + dataFile.st_blocks;
            List[totalDir].selfBlocks = List[totalDir].selfBlocks +
             dataFile.st_blocks;
            continue;
        }
        if(strcmp(conten->d_name, "..")){
            if(conten->d_type == DT_DIR){
                //Si el archivo es del tipo directorio, lo encolo
                //en la cola de directorios a ser analizados por
                //los hilos trabajadores.

                if(strcmp(inputDir,"/")){
                    strcpy(aux, inputDir);
                    strcat(aux, "/");
                }
                else{
                    strcpy(aux,inputDir);
                }
                strcat(aux, conten->d_name);
                Enqueue(&Directory, aux, inputDir);
                numberDir = sizeQueue(Directory);
                List[totalDir].numberChilds ++;
                
            }
            else if(conten->d_type == DT_REG){
                //Si es un archivo regular, sumo al directorio actual
                //el total de bloques que ocupa y lo almaceno en la primera
                //posicion de la lista de analizados junto con el id.

                if(strcmp(inputDir,"/")){
                    strcpy(aux, inputDir);
                    strcat(aux, "/");
                }
                else{
                    strcpy(aux,inputDir);
                }
                strcat(aux, conten->d_name);
                lstat(aux, &dataFile);
                List[totalDir].selfBlocks = List[totalDir].selfBlocks +
                 dataFile.st_blocks;                
                Summatory = Summatory + dataFile.st_blocks;

            }
        }
        List[totalDir].id = (char*) malloc ((strlen(inputDir)+1) * sizeof (char));
        strcpy(List[totalDir].id, inputDir);
        List[totalDir].printed = 0;
    }

    totalDir ++;



    //////////////////////////////////////////////////////////////
    //**********************************************************//
    //************  Creando los hilos trabajadores  ************//
    //**********************************************************//
    //////////////////////////////////////////////////////////////

    //Tabla que almacena los identificadores de los hilos.
    pthread_t ThreadId[numberThreads];      
    for (i = 0; i < numberThreads; i++){
        //Todos los hilos se guardan en un arreglo de estructuras que
        //les hace referencia y almacena datos para su funcionamiento.

        Table[i].id = i;
        Table[i].bussy = 0;
        Table[i].gotIt = 0;
        pthread_create(&ThreadId[i],NULL, (void*) WorkerTread, (void*) &Table[i]);
    }
 



    //////////////////////////////////////////////////////////////
    //**********************************************************//
    //***********  Asignacion de directorios a hilos ***********//
    //**********************************************************//
    //////////////////////////////////////////////////////////////

    while(numberDir > 0 || StillBussy() == -1){

        if (numberDir){
            for (i=0; i<numberThreads; i++){
                //Le entrego un path del primer directorio en la cola a un
                //hilo libre para que lo analice y lo desencolo.

                if (Table[i].bussy == 0 && Table[i].gotIt == 0 && numberDir){
                    Table[i].dir = (char*) malloc ((strlen
                        (firstQueued(Directory))+1)* sizeof (char));
                    Table[i].dad =  (char*) malloc ((strlen
                        (firstQueuedDad(Directory))+1)* sizeof (char));
                    strcpy(Table[i].dir, firstQueued(Directory));
                    strcpy(Table[i].dad, firstQueuedDad(Directory));
                    Dequeue(&Directory);
                    numberDir = sizeQueue(Directory);
                    Table[i].bussy = 1;
                }
            }
        }


    //////////////////////////////////////////////////////////////
    //**********************************************************//
    //**********  Resultados entregados por los hilos **********//
    //**********************************************************//
    //////////////////////////////////////////////////////////////

        for (i = 0; i < numberThreads; ++i){
            //Revisa que hilo ha terminado de analizar y envia a 
            //la lista de analizados los resultados. Encola en 
            //la cola de directorios a analizar los subdirectorios
            //encontrados por el hilo.

            if (Table[i].gotIt == 1){
                List[totalDir].id = (char*) malloc ((strlen
                    (Table[i].returns)+1)* sizeof(char));
                strcpy(List[totalDir].id, Table[i].returns);
                List [totalDir].Dad = (char*) malloc ((strlen
                    (Table[i].dad)+1)* sizeof(char));
                strcpy(List[totalDir].Dad, Table[i].dad);
                List[totalDir].selfBlocks = Table[i].size;
                List[totalDir].BlocksChilds = 0;
                List[totalDir].numberChilds = Table[i].numberChilds;
                List[totalDir].printed = 0;
                totalDir ++;
                Table[i].returns = NULL;
                Table[i].dir = NULL;
                Table[i].gotIt = 0;

            if (Table[i].q){
                QueueCat(Table[i].q , &Directory);
                DestroyQueue(&Table[i].q);
                numberDir = sizeQueue(Directory);                       
                    
                }

            }
        }

    }

    //////////////////////////////////////////////////////////////
    //**********************************************************//
    //**********  Sumalizando resultados en la lista  **********//
    //**********************************************************//
    //////////////////////////////////////////////////////////////

    while (StillnumChilds()==-1 || List[0].printed == 0){
        //Se recorre el arbol aplanado de directorios desde las
        //hojas hasta la raiz, sumando de hijo a padre la cantidad 
        //total de bloques que ocupan y luego se imprimen los
        //resultados.

        for (i = 0; i < totalDir; ++i){

            if(List[i].numberChilds == 0 && List[i].printed != 1){
                if (strcmp(List[i].id, inputDir)){
                List[findDir(List[i].Dad)].BlocksChilds = 
                List[findDir(List[i].Dad)].BlocksChilds + List[i].selfBlocks+ 
                List[i].BlocksChilds;
                List[findDir(List[i].Dad)].numberChilds --;
            }
                if (!outputBool){
                   printf("%d %s\n", List[i].selfBlocks + 
                    List[i].BlocksChilds, List[i].id);
                   List[i].printed = 1;
                }
                else{

                    fprintf(outputDirD, "%d %s\n", List[i].selfBlocks + 
                        List[i].BlocksChilds, List[i].id);
                    List[i].printed = 1;

                    }
                }

            }
        if (List[0].printed == 1){
            exit(0);
        }
    }

}


////////////////////////////////////////////////////////////////////
//****************************************************************//
//************************  FUNCIONES  ***************************//
//****************************************************************//
////////////////////////////////////////////////////////////////////



/*
*Funcion: findDir
*-----------------
*Funcion que retorna la posicion de un directorio
*en la lista de directorios analizados previamente
*por un hilo trabajador.
*
*Recibe: arreglo de caracteres que coincide con el
*path del directorio a encontrar.
*Retorna: entero con la posicion del directorio.
*/
int findDir(char *dir){
    int i = 0;
    for (i = 0; i < totalDir; ++i)
    {
        if (!strcmp(List[i].id, dir)){
            return i;
        }
    }
    return -1;
}

/*
*Funcion: StillnumChilds
*-------------------------
*Funcion que retorna si aun quedan subdirectorios
*por agregarle el numero de bloques que ocupan
*al numero de bloques que ocupa el directorio padre.
*
*No recibe nada.
*Retorna: -1 si aun quedan subdirectorios por
*transferir el numero de bloques que ocupan.
*/
int StillnumChilds(){
    int i;
    for(i=0; i<totalDir; i++){
        if (List[i].numberChilds > 0){
            return -1;
        }
    }
}

/*
*Funcion: StillBussy
*---------------------
*Funcion que retorna si aun quedan hilos trabajadores
*ocupados en retornar una resultado.
*
*No recibe nada.
*Retorna: -1 si aun quedan hilos ocupados.
*/
int StillBussy(){
    int i;
    for(i=0; i<numberThreads; i++){
        if (Table[i].bussy == 1 || Table[i].gotIt == 1){
            return -1;
        }
    }
}


/*
* Funcion: WorkerTread
* -----------------------
*Funcion que evalua los directorios que se encuentran en la cola
*de directorios a ser analizados para crear dos listas, una de
*archivos analizados para imprimir los resultados y la segunda
*con otros directorios a ser encolados.
*
*Recibe: Nodo que contiene todos los datos necesarios del hilo
*y variables compartidas por el hilo Master y los trabajadores.
*No retorna nada, solo modifica su estructura dejando un string
*con la info necesaria para la impresion.
*/
void WorkerTread(DataThread *current){

    DIR *DirDescriptor;
    struct dirent *DirConten;
    struct stat dataFile;
    int Summatory;
    char aux[10000];
    char aux2[10000];
    strcpy(aux,"\0");
    strcpy(aux2,"\0"); 

do  {
        while (current->bussy != 1){
            sleep(0.5);
        }
            createQueue(&current->q);
            DirDescriptor = opendir (current->dir);
                if (!DirDescriptor){
                printf("\nERROR: no abrio el directorio, no tiene los privilegios...\n\n");
                exit(0);

                } 
                current->numberChilds = 0;
                current->size = 0;
                while( (DirConten = readdir(DirDescriptor)) != NULL){
                    if (!strcmp(DirConten->d_name,".")){
                        lstat(current->dir, &dataFile);
                        Summatory = Summatory + dataFile.st_blocks;
                        continue;
                    }
                    if(strcmp(DirConten->d_name, "..")){
                        if(DirConten->d_type == DT_DIR){

                                sprintf(aux ,"%s%s%s", current->dir,"/", DirConten->d_name);
                                lstat(aux, &dataFile);

                                Enqueue(&current->q, aux, current->dir);
                                current->numberChilds ++;
                        }
                        else{
                                sprintf(aux2, "%s%s%s", current->dir,"/",DirConten->d_name);
                                lstat(aux2, &dataFile);
                                Summatory = Summatory + dataFile.st_blocks;
                        }
                    }
                }
            current->size = Summatory;
            Summatory = 0;
            current->returns = (char*) malloc ((strlen(current->dir)+1)* sizeof(char));
            strcpy(current->returns, current-> dir);
            current->gotIt = 1;
            current->bussy = 0;
        
    }while (1);
}


 

