/**
@file laboratorio.c
@author Jeferson Grueso, Daniel Escamilla, Santiago Moreno
@Date 03 Septiembre 2014
@brief main funcion principal para definir variables y operaciones
@param argc, chat* argv[]
@param a, Vector A muestra y una serie de caracteres predefinidos
@param b, Vector B muestra una serie de caracteres predefinidos
@param C, Vectir que muestra los números sin repetir de A y B
@param D Vector que muestra los numeros repetidos de A y B
@param n, es el número


*/
//—————————————————————————
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#define n 10
//—————————————————————————
#pragma argsused


int main(int argc, char* argv[]){


int a[n]={1,2,3,4,5,7,9,8,0,11}, b[n]={6,3,7,9,2,4,5,12,0,7}, c[n*2], d[n], i, j, m=0, p=0, sw=0;
printf("\nA ");
for(i=0;i<n;i++){
printf("%d ",a[i]);
}
printf("\nB ");
for(i=0;i<n;i++){
printf("%d" ,b[i]);
}
//Proceso 1 (Guardar en el vector D los números que se repiten)
sw=0;
for(i=0;i<n;i++){
for(j=0;j<n;j++){
if(a[i]==b[j]){
sw=1;
}
}
if(sw==1){
d[p]=a[i];
p++;
sw=0;
}
}
//Proceso 2 (Guardar en el vector C los números que no se repiten)
for(i=0;i<n;i++){
for(j=0;j<n&&sw!=2;j++){
if(a[i]==b[j]){
sw=1;
b[j]=0;
}else{
sw=0;
}
if(sw==1){
sw=2;
}
}
if(sw==0){
c[m]=a[i];
m++;
}
sw=0;
}
for(i=0;i<n;i++){
for(j=0;j<n;j++){
if(b[i]==c[j]){
sw=1;
}
}
if(sw==0&&b[i]!=0){
c[m]=b[i];
m++;
}
}
//Imprimir los vectores ordenados
printf("\nC ");
for(i=0;i<m;i++){
printf("%d ",c[i]);
}
printf("\nD ");
for(i=0;i<p;i++){
printf("%d ",d[i]);
}
getch();
return 0;
}
//——————————————————————

