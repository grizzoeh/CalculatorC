#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"


//Para simular la calculadora de Notacion Posfija voy a modelar la expresión 
//como una pila cuyo tope es el operador (por ejemplo en 3 2 -, el tope es -); 
//entonces,lo primero que se desapila es el operador, y luego los operandos en orden inverso.
//Aplicando el operador. El resultado quedara como unico elemento de la pila

void calculadora(char* linea){
    
    bool error = false;
    pilanum_t* pila = pilanum_crear();
    char** arreglo_linea = dc_split(linea);
    
    calc_num resultado;
    for (size_t i = 0; arreglo_linea[i]; i++){
        char* valor = arreglo_linea[i];

        struct calc_token token;
        if (!calc_parse(valor, &token)) {
            error = true;
            break;
        }

       
        if(token.type==TOK_NUM){
            apilar_num(pila,token.value);
            
        }
        else if(token.type==TOK_OPER){

            if(token.oper.num_operandos==2){
                if(!desapilar_num(pila,&token.value)){
                    error = true;
                    break;
                }
                calc_num aux1 = token.value;
                if(!desapilar_num(pila,&token.value)){
                    error = true;
                    break;
                }
                calc_num aux2 = token.value;

                if(*valor == '+'){
                    resultado = aux1 + aux2;
                    apilar_num(pila,resultado);

                }

                if(*valor == '-'){
                    
                    resultado = aux2 - aux1;
                    apilar_num(pila,resultado);

                }

                if(*valor == '*'){
                    
                    
                    resultado = aux2 * aux1;
                    apilar_num(pila,resultado);

                }

                if(*valor == '/'){
                    if(aux1==0){
                        error = true;
                        break;

                    }
                    resultado = aux2 / aux1;
                    apilar_num(pila,resultado);

                }

                if(*valor == '^'){

                    if(aux1<0){
                        error = true;
                        break;
                    }
                    double operacion = pow((double)aux2,(double)aux1);
                    resultado = (calc_num)operacion;
                    apilar_num(pila,resultado);

                }
                
                if(strcmp(valor, "log") == 0){
                    if(aux1<2){
                        error = true;
                        break;

                    }
                    resultado = (calc_num) (double) (log((double)aux2)/log((double)aux1));
                    apilar_num(pila,resultado);
                }
            
            }else if(token.oper.num_operandos==1){
                if(strcmp(valor, "sqrt") == 0){
                    if(!desapilar_num(pila,&token.value)){
                        error = true;
                        break;
                    
                    }
                    calc_num val1 = token.value;
                    if(val1<0){
                        error = true;
                        break;

                    }
                    double operacion = sqrt((double)val1);
                    resultado = (calc_num)operacion;
                    apilar_num(pila,resultado);
                }


            }else if(token.oper.num_operandos==3){
                if(*valor == '?'){
                    if(!desapilar_num(pila,&token.value)){
                        error = true;
                        break;
                        
                    }
                    calc_num aux1 = token.value;
                    if(!desapilar_num(pila,&token.value)){
                        error = true;
                        break;
                       
                }   
                    calc_num aux2 = token.value;
                    if(!desapilar_num(pila,&token.value)){
                        error = true;
                        break;
                    }
                    calc_num aux3 = token.value;
                    resultado = aux3 ? aux2 : aux1;
                    apilar_num(pila,resultado);
                }
            }
        }
    }
    //ya no hay tokens por leer...


    struct calc_token token_final;
    desapilar_num(pila,&token_final.value);

    if(!pila_esta_vacia(pila)){
        error=true;
    }
    
    if (!error){

        fprintf(stdout,"%ld", resultado);
        fprintf(stdout,"\n");

    }
    else{
        fprintf(stdout,"ERROR");
        fprintf(stdout,"\n");
    }
    free_strv(arreglo_linea);
    pilanum_destruir(pila);

}


int main(void) {
    size_t tam = 0;
    char* linea = NULL;
	
	while (getline(&linea, &tam, stdin) != -1) {
        calculadora(linea);
		
    }
    free(linea);
	return 0;
}
