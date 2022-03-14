#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"
#include "cola.h"

//Para desarrollar la calculadora de Notacion Infija me basé en el
//Algoritmo Shunting Yard, fuente: https://es.wikipedia.org/wiki/Algoritmo_shunting_yard

void infix(char *linea){
    bool error = false;
    
    pila_t *pila = pila_crear();
    cola_t *cola = cola_crear();
    char **arreglo_linea = infix_split(linea);
    for (size_t i = 0; arreglo_linea[i]; i++){
    
        char *valor = arreglo_linea[i];

        struct calc_token token;
        
        struct calc_token token_tope;

        if (!calc_parse(valor, &token)){
        
            error = true;
            break;
        }

        if (token.type == TOK_NUM){
        
            
            cola_encolar(cola, valor);

        }else if (token.type == TOK_OPER && !pila_esta_vacia(pila)){
        
            if (!calc_parse((char *)pila_ver_tope(pila), &token_tope))
            {
                break;
            }

            while ((token_tope.type != TOK_LPAREN) && ((token.oper.asociatividad == ASSOC_LEFT && token.oper.precedencia <= token_tope.oper.precedencia) || (token.oper.asociatividad == ASSOC_RIGHT && token.oper.precedencia < token_tope.oper.precedencia))){
            
                
                cola_encolar(cola, pila_desapilar(pila));
                if (pila_esta_vacia(pila)){
                    break;
                }
                
                if (!calc_parse((char *)pila_ver_tope(pila), &token_tope)){
                
                    error = true;
                    break;
                }
                
             
            }
            pila_apilar(pila, valor);
        }else if ((token.type == TOK_OPER && pila_esta_vacia(pila))){
        
            pila_apilar(pila, valor);

        }else if (token.type == TOK_LPAREN){
        
            
            pila_apilar(pila, valor);

        }else if (token.type == TOK_RPAREN){    
          
            if (!calc_parse(pila_ver_tope(pila), &token_tope)){
               
                error = true;
                break;
                
                
            }
            
            while (token_tope.type != TOK_LPAREN && !pila_esta_vacia(pila)){
            
                cola_encolar(cola, pila_desapilar(pila));
                if (pila_esta_vacia(pila)){
                    break;
                    
                }
                if (!calc_parse(pila_ver_tope(pila), &token_tope)){
                
                    error = true;
                    break;
                }     
            }
            pila_desapilar(pila);
        }
    }

    //Cuando no hay mas tokens por leer, veo los que quedaron en la pila...
    struct calc_token token_final;
    while (!pila_esta_vacia(pila)){
    

        bool estado_aux = calc_parse(pila_ver_tope(pila), &token_final);
        if(!estado_aux || token_final.type == TOK_LPAREN || token_final.type == TOK_RPAREN){

            free_strv(arreglo_linea);
            pila_destruir(pila);
            cola_destruir(cola, NULL);
            return;
            
        }
        cola_encolar(cola, pila_desapilar(pila));
    }
    ///////////////FIN SHUNTING YARD ///////////////////////7

    if(!error){
        while (!cola_esta_vacia(cola)){
        
            fprintf(stdout,"%s ",(char*) cola_desencolar(cola));
            
        }
        fprintf(stdout,"\n");
    }
   
    free_strv(arreglo_linea);
    pila_destruir(pila);
    cola_destruir(cola, NULL);
}


int main(void) {
    size_t tam = 0;
    char* linea = NULL;
	
	while (getline(&linea, &tam, stdin) != -1) {
        infix(linea);
		
    }
    free(linea);
	return 0;
}