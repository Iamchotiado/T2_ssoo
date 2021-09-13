#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"


Queue* queue_init(Process* proceso, int cantidad_procesos, int cantidad_f1, int cantidad_f2, int cantidad_f3, int cantidad_f4) {
    Queue* queue = calloc(1, sizeof(Queue));
    *queue = (Queue){.proceso = proceso, .cantidad_procesos = cantidad_procesos, .cantidad_f1 = cantidad_f1, .cantidad_f2 = cantidad_f2, .cantidad_f3 = cantidad_f3, .cantidad_f4 = cantidad_f4};
    return queue;
};

void agregar_alfinal(Process* insertado, Process* insertar) {
    if (insertado -> next == NULL) {
        insertado -> next = insertar;
    }
    else
    {
        agregar_alfinal(insertado -> next, insertar);
    }  
};

Process* agregar_cpu(Process* proceso) {
    // si esta ready
    if (proceso -> estado == 1) {
        proceso -> estado = 0;
        return proceso;
    }
    else
    {
        agregar_cpu(proceso -> next);
    }
    
};

void sumar_fabrica_cola(Queue* cola, int n_fabrica) {
    if (n_fabrica == 1)
    {
        cola -> cantidad_f1 ++;
    }
    else if (n_fabrica == 2)
    {
        cola -> cantidad_f2 ++;
    }
    else if (n_fabrica == 3)
    {
        cola -> cantidad_f3 ++;
    }
    else if (n_fabrica == 4)
    {
        cola -> cantidad_f4 ++;
    }

}

int calcular_quantum(Queue* cola, int n_fabrica, int Q) {
    if (n_fabrica == 1)
    {
        cola -> cantidad_f1 ++;
    }
    else if (n_fabrica == 2)
    {
        cola -> cantidad_f2 ++;
    }
    else if (n_fabrica == 3)
    {
        cola -> cantidad_f3 ++;
    }
    else if (n_fabrica == 4)
    {
        cola -> cantidad_f4 ++;
    }

}

void agregar_quantum(Process* proceso, int quantum) {

}