/**
@file laboratorio.c
@author Jeferson Grueso, Daniel Escamilla, Santiago Moreno
@Date 03 Septiembre 2014
@brief  funcion principal para definir variables y operaciones
@param argc, chat* argv[]
@param txt, Vector A muestra y una serie de caracteres predefinidos
@param aleatorio, 		Variable para generar numeros de codificacion
@param txt, 		vector que guarda la frase introducida
Algnuas modificciones hehcas
mimimimmimimimipirrripipipipippiuuuu
*/
//Librearias necesarias
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>	//Librearia para funcion srand (datos aleatorio)

int i;			//Variable de contador
int n;			//Variable de contador
int n1;			//Variable de contador
int aleatorio;  //Variable para generar 
int P=1;		
int U=10;
char txt[50];	//Vector para guardar la palabra ingresada

int generar();
/**Petición y contador de caracteres de frase a codificar*/

int generar(){
	
	for(n=0;(txt[n]=getchar()) != '\n';++n); 
	txt[n]='\0'; 

	
}

int codificar ();
	/**Generación de número al azar por letra introducida*/

int codificar(){
	    	srand(time(NULL)); //funciona para general datos aleatorios
		    for (i = 0; i < n; i ++) 	//for que asigna posiciones en vector

{
	 		aleatorio = P+(int)(((U-P+1.0)*rand())/(RAND_MAX+1.0)); //Variables para asignar valor a  posiciones
			printf("%d ", aleatorio);  //impresion de valor aleatorio
}
	
	
}
/** Funcion principal del programa*/
int main()
{ //Impresión de estructura para visualizar resultados obtenidos
			printf("*--------------------------------------*\n");
			printf("*   PROGRAMA PARA CODIFICAR FRASES     *\n");
			printf("*--------------------------------------*\n");

			printf("Por favor Ingrese una frase:\n"); 

generar();
			printf("*--------------------------------------*\n");
			printf("+ La frase %s tiene %d caracteres.       +\n",txt,n); 
			printf("*--------------------------------------*\n");

			printf("*...........................................*\n");
			printf("La generacion aleatoria de codificacion es:\n ");
			printf("*...........................................*\n");


codificar();
printf("\n");
system("PAUSE");
return 0;
}
