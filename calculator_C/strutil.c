#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "strutil.h"
#include <string.h>
#include <stdlib.h>


char *substr(const char *str, size_t n){
    char* subcadena = calloc((n + 1),sizeof(char));

    if(!subcadena){
        return NULL;
    }

    size_t largo = strlen(str);
    for(int i = 0; i < n && i < largo; i++){
        subcadena[i] = str[i];
    }

   
    return subcadena;
}

size_t cantidad_subcadenas(const char *str, char sep, size_t largo){
    size_t contador = 0;
    for(int i = 0; i < largo; i++){
        if(str[i] == sep){
            contador++;
        }
    }
    contador++;
    return contador;

}

char **split(const char *str, char sep){
    
    if(!str){
        char** arreglo_vacio = calloc(1,sizeof(char*));
        if(!arreglo_vacio){

            return NULL;
        }
        return arreglo_vacio;
	}
    
    size_t largo = cantidad_subcadenas(str,sep,strlen(str));
    

    char** arreglo_de_cadenas = calloc(largo+1,sizeof(char*)); 
    if(!arreglo_de_cadenas){
        return NULL;
    }

    size_t restar = 0; 
    size_t pos_arreglo = 0;
    int contador = 0;


    while(pos_arreglo<largo){
      
        
        if(str[contador] == sep || str[contador] == '\0'){
            arreglo_de_cadenas[pos_arreglo] = strndup(&str[contador - restar], restar);
            pos_arreglo++;
            restar = 0;
        }else{
            restar++; 
        }
        
        contador++;
           
        
    }
    
    return arreglo_de_cadenas;
}

char *join(char **strv, char sep){
    size_t cant = 0;
    int i = 0;
    while(strv[i]){
        cant += strlen(strv[i])+1;
        i++;
    }

    char* string = malloc((sizeof(char)*cant)+1);
    if(!string){
        return NULL;
    }

    int contador = 0;
    for(int j = 0; strv[j]; j++) {
        for(int k = 0; strv[j][k]; k++) {
            string[contador] = strv[j][k];
            contador++;
        }
        if(contador<cant-1 && sep!='\0') {
            string[contador] = sep;
            contador++;

        }
        
   
    }
    string[contador] = '\0';
    
    return string;
    //disclaimer: intente hacerlo con strcat pero lei en varios lugares que podia
    //transformarte la funcion en un orden mayor a o(n) asi que decidi hacerlo asi.

    
    
}


void free_strv(char *strv[]){

    for(int i = 0; strv[i]; i++){
        free(strv[i]);
    }
    free(strv);
}
