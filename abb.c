#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

char *strdup(const char * clave){
    char *duplicado = malloc((strlen(clave) + 1) * sizeof(char));
    strcpy(duplicado, clave);
    return duplicado;
}

//Estructuras de arbol--------------------------------------------------------------------


typedef struct abb_nodo_t abb_nodo_t;

struct abb {
    size_t cantidad;
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_nodo_t {
    char *clave;
    void *valor;
    abb_nodo_t *izq;
    abb_nodo_t *der;
    abb_nodo_t *padre;
};

//Primitivas de nodos--------------------------------------------------------------------
abb_nodo_t *nodo_crear(const char *clave, void *valor, abb_nodo_t *padre){
    abb_nodo_t *nodo = malloc(sizeof(abb_nodo_t));
    if(!nodo) return NULL;
    nodo->clave = strdup(clave);
    nodo->valor = valor;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->padre = padre;
    return nodo;
}

//Funciones auxiliares-----------------------------------------------------------------------
abb_nodo_t *buscar_menor_der(abb_t *arbol, abb_nodo_t *nodo){
    abb_nodo_t *menor_der;
    if(nodo->der){
        menor_der = nodo->der;
    }
    while(true){
        if(menor_der->izq){
            menor_der = menor_der->izq;
        }
        else break;
    }
    return menor_der;
}

void swap(abb_nodo_t *nodo1, abb_nodo_t *nodo2){
        void *dato_auxiliar = nodo1->valor;
        char *clave_auxiliar = nodo1->clave;
        nodo1->clave = nodo2->clave;
        nodo1->valor = nodo2->valor;
        nodo2->valor = dato_auxiliar;
        nodo2->clave = clave_auxiliar;
}

bool abb_guardar_auxiliar(abb_t *arbol, abb_nodo_t *nodo, const char *clave, void *valor){
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion){
        if(arbol->destruir_dato){
            arbol->destruir_dato(nodo->valor);
        }
        nodo->valor = valor;
        return true;
    }

    else if((comparacion > 0 && !nodo->izq) || (comparacion < 0 && !nodo->der)){
        abb_nodo_t *nodo_nuevo = nodo_crear(clave, valor, nodo);
        if(nodo_nuevo == NULL) return false;
        if(comparacion > 0) nodo->izq = nodo_nuevo;
        else nodo->der = nodo_nuevo;
        arbol->cantidad++;
        return true;
    }

    else if(comparacion > 0) return abb_guardar_auxiliar(arbol, nodo->izq, clave, valor);
    else return abb_guardar_auxiliar(arbol, nodo->der, clave, valor);
}

bool abb_pertenece_auxiliar(const abb_t *arbol, const char *clave, abb_nodo_t *nodo){
    if(nodo == NULL) return false;
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion) return true;
    else if((comparacion > 0 && !nodo->izq) || (comparacion < 0 && !nodo->der)) return false;
    else if(comparacion > 0) return abb_pertenece_auxiliar(arbol, clave, nodo->izq);
    else return abb_pertenece_auxiliar(arbol, clave, nodo->der);
}

void *abb_obtener_auxiliar(const abb_t *arbol, const char *clave, abb_nodo_t *nodo){
    if(nodo == NULL) return NULL;
    int comparacion = (arbol->cmp(nodo->clave, clave));
    if(!comparacion) return nodo->valor;
    else if(comparacion > 0) return abb_obtener_auxiliar(arbol, clave, nodo->izq);
    else return abb_obtener_auxiliar(arbol, clave, nodo->der);
}

void destruir_nodo(abb_t *arbol, abb_nodo_t *nodo){
    if(nodo){
        free(nodo->clave);
        if(arbol->destruir_dato) arbol->destruir_dato(nodo->valor);
        free(nodo);
    }
}

void destruir_nodos(abb_t *arbol, abb_nodo_t *nodo){
    if(!nodo) return;
    destruir_nodos(arbol, nodo->izq);
    destruir_nodos(arbol, nodo->der);
    destruir_nodo(arbol, nodo);
}

void *abb_borrar_auxiliar(abb_t *arbol, abb_nodo_t *nodo, const char *clave){
    if(nodo == NULL) return NULL;
    //Variable que se usará mas adelante
    int comparacion;

    //Se fija si el nodo actual es el que se quiere cambiar. Si es, entra
    if(!arbol->cmp(nodo->clave, clave)){
        //Si no es la raiz, se fija si es hijo derecho o izquierdo de su padre
        if(nodo->padre != NULL) comparacion = arbol->cmp(nodo->clave, nodo->padre->clave);
        
        //Si es una hoja (no tiene hijos), simplemente actualiza la referencia del padre. Si el nodo es la raiz, pone la raiz en NULL
        if(!nodo->izq && !nodo->der){
            if(nodo->padre == NULL) arbol->raiz = NULL;
            else if(comparacion > 0) nodo->padre->der = NULL;
            else nodo->padre->izq = NULL;
        }

        //Si tiene un hijo, actualiza la referencia su padre a su hijo. SI es la raiz, pone a su hijo como raiz
        else if(!nodo->izq || !nodo->der){
            if(nodo->izq){
                if(nodo->padre == NULL){
                    arbol->raiz = nodo->izq;
                    arbol->raiz->padre = NULL;
                }
                else if(comparacion > 0) nodo->padre->der = nodo->izq;
                else nodo->padre->izq = nodo->izq;  
            }
            else{
                if(arbol->raiz == NULL){ 
                    arbol->raiz = nodo->der;
                    arbol->raiz->padre = NULL;
                }
                else if(comparacion < 0) nodo->padre->der = nodo->der;
                else nodo->padre->izq = nodo->der;
            }
        }

        //Si tiene dos hijos, se hace lo que dijeron en la práctica
        //Se swapea el nodo, con su menor hijo derecho, y luego se llama a borrar de vuelta, con el que quedo en la posicion del menor hijo derecho
        //Este va a estar en uno de los dos primeros casos}//Por ultimo se devuelve el dato
        else{
            abb_nodo_t *menor_der = buscar_menor_der(arbol, nodo);
            swap(nodo, menor_der);
            //SI no me equivoco, la cantidad se actualiza dentro de este borrar_auxiliar:
            void *dato_auxiliar = abb_borrar_auxiliar(arbol, menor_der, clave);
            return dato_auxiliar;
        }

        ///Se guarda el dato, se liberan la clave y el nodo, se resta la cantidad, y se devuelve el dato.
        void *dato_auxiliar = nodo->valor;
        free(nodo->clave);
        free(nodo);
        arbol->cantidad--;
        return dato_auxiliar;
    }

    //SI no era el nodo que estaba buscando, llama a borrar_auxiliar con su hijo izquierdo o derecho según corresponda
    else if(arbol->cmp(nodo->clave, clave) > 0) return abb_borrar_auxiliar(arbol, nodo->izq, clave);
    else return abb_borrar_auxiliar(arbol, nodo->der, clave);
}

//Primitivas arbol-----------------------------------------------------------------------
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->destruir_dato = destruir_dato;
    abb->cmp = cmp;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        abb_nodo_t *nodo_nuevo = nodo_crear(clave, dato, NULL);
        if(!nodo_nuevo) return false;
        arbol->raiz = nodo_nuevo;
        arbol->cantidad++;
    }
    else{
        if(!(abb_guardar_auxiliar(arbol, arbol->raiz, clave, dato))) return false;
    }
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if(arbol->raiz == NULL) return NULL;
    void *valor = abb_borrar_auxiliar(arbol, arbol->raiz, clave);
    return valor;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    return abb_obtener_auxiliar(arbol, clave, arbol->raiz);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return (abb_pertenece_auxiliar(arbol, clave, arbol->raiz));
}

size_t abb_cantidad(abb_t *arbol){
    if(arbol->raiz == NULL) return 0;
    return(arbol->cantidad);
}

void abb_destruir(abb_t *arbol){
    destruir_nodos(arbol, arbol->raiz);
    free(arbol);
}

//Iterador interno-----------------------------------------------------------------------
void abb_in_order_recursivo(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
    if(!nodo) return;
    abb_in_order_recursivo(nodo->izq, visitar, extra);
    if(!visitar(nodo->clave, nodo->valor, extra)) return;
    abb_in_order_recursivo(nodo->der, visitar, extra);
}



void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    abb_in_order_recursivo(arbol->raiz, visitar, extra);
}

//Iterador externo-----------------------------------------------------------------------

typedef struct abb_iter abb_iter_t;

struct abb_iter {
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    
    if(!arbol) return NULL;
    
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;

    pila_t* pila = pila_crear();
    if(!pila){
        free(iter);
        return NULL;
    }
    
    if(arbol->raiz){
        abb_nodo_t *nodo = arbol->raiz;
        pila_apilar(pila, nodo);
        while(nodo->izq != NULL){
            nodo = nodo->izq;
            pila_apilar(pila, nodo);
                
        }
    }
            
    iter->pila = pila;

    return iter;
}
bool abb_iter_in_avanzar(abb_iter_t *iter){

    if(!iter || !iter->pila || pila_esta_vacia(iter->pila)){
        return false;
    }

    abb_nodo_t *nodo = pila_desapilar(iter->pila);

    if(nodo->der != NULL){
        pila_apilar(iter->pila, nodo);
        while(nodo->izq != NULL){
            nodo = nodo->izq;
            pila_apilar(iter->pila, nodo);
        }
    }
    free(nodo);
     return true; 
 
}
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(!iter || !iter->pila || pila_esta_vacia(iter->pila)){ 
        return NULL;
    }
    abb_nodo_t* nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    if(!iter) return;
    if(iter->pila) pila_destruir(iter->pila, NULL);
    free(iter);
}





