
# ----------------------------------------------------------------------------#
#                                  Makefile                                   #
# ----------------------------------------------------------------------------#
#                         Universidad Simón Bolívar                           #
#            Departamento de Computación y Tecnología de la Información       #
#                     Laboratorio de Sistemas Operativos                      #
#              Profesora Angela Di Serio y Profesor Carlos Gomez              #
#                       Fernando Gonzalez carné: 08-10464                     #
#                          Giuli Latella carné: 08-10596                      #
#                                 Grupo: C                                    #
# ----------------------------------------------------------------------------#
# ----------------------------------------------------------------------------#


UsoDisco: UsoDisco.o functions.o 
	gcc UsoDisco.o functions.o -o UsoDisco -lpthread

UsoDisco.o: UsoDisco.c UsoDisco.h
	gcc -c UsoDisco.c

functions.o: functions.c functions.h
	gcc -c functions.c


clear:
	rm UsoDisco.o UsoDisco

