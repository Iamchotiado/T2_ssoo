#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

Process* proceso_siguiente;

Queue* queue_init(Process* proceso, int cantidad_procesos, int cantidad_f1, int cantidad_f2, int cantidad_f3, int cantidad_f4) {
    Queue* queue = calloc(1, sizeof(Queue));
    *queue = (Queue){.proceso = proceso, .cantidad_procesos = cantidad_procesos, .cantidad_f1 = cantidad_f1, .cantidad_f2 = cantidad_f2, .cantidad_f3 = cantidad_f3, .cantidad_f4 = cantidad_f4};
    return queue;
};

void agregar_alfinal(Process* insertado, Process* insertar) {
    if (insertado -> next == NULL) {
        insertado -> next = insertar;
        insertado -> next -> next = NULL;
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
    else if(proceso -> next != NULL)
    {
        agregar_cpu(proceso -> next);
    }
    return NULL;
    
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

};

int calcular_quantum(Queue* cola, int n_fabrica, int Q) {
    int n_i;

    if (n_fabrica == 1)
    {
        n_i = cola -> cantidad_f1;
    }
    else if (n_fabrica == 2)
    {
        n_i = cola -> cantidad_f2;
    }
    else if (n_fabrica == 3)
    {
        n_i = cola -> cantidad_f3;
    }
    else if (n_fabrica == 4)
    {
        n_i = cola -> cantidad_f4;
    }

    int f = 0;
    if (cola -> cantidad_f1 > 0)
    {
        f ++;
    }
    if (cola -> cantidad_f2 > 0)
    {
        f ++;
    }
    if (cola -> cantidad_f3 > 0)
    {
        f ++;
    }
    if (cola -> cantidad_f4 > 0)
    {
        f ++;
    }

    int quantum = floor(Q / (n_i * f));
    return quantum;
};

void agregar_quantum(Process* proceso, int quantum) {
    // si esta running
    if (proceso -> estado == 0) {
        proceso -> quantum = quantum;
    }
    else
    {
        agregar_quantum(proceso -> next, quantum);
    }
}

void actualizar_datos(Process* proceso) {
    // si el proceso esta en running
    if (proceso -> estado == 0)
    {
        // actualizamos el quantum y el cpu burst
        proceso -> quantum --;
        for (int i = 0; i < proceso -> cantidad_rafagas; i++)
        {
            // buscamos el primero de los cpu bursts que no ha terminado
            if (proceso -> cpu_bursts[i] > 0)
            {
                int previo = proceso -> cpu_bursts[i];
                proceso -> cpu_bursts[i] = previo - 1;
                proceso -> watch_cpu = i;
                break;
            }
        }
    }

    // si el proceso esta en ready
    else if (proceso -> estado == 1)
    {
        // no se actualiza nada por el momento
    }
    
    // si el proceso esta en waiting
    else if (proceso -> estado == 2)
    {
        // actualizamos el io bursts
        for (int i = 0; i < proceso -> cantidad_rafagas - 1; i++)
        {
            // buscamos el primero de los io bursts que no ha terminado
            if (proceso -> io_bursts[i] > 0)
            {
                int previo = proceso -> io_bursts[i];
                proceso -> io_bursts[i] = previo - 1;
                proceso -> watch_io = i;
                break;
            }
        }
    }

    // si el proceso esta en finished
    else if (proceso -> estado == 3)
    {
        // no deberia entrar nunca a este if porque se supone
        // que el proceso debio haber sido sacado al inicio de la unidad de tiempo
    }
    


    // si esque hay un proceso en la posicion siguiente de la cola, actualizamos sus datos
    if (proceso -> next != NULL)
    {
        actualizar_datos(proceso -> next);
    }
    
};

void pasar_a_ready(Process* proceso) {
    // si el proceso esta en waiting
    if (proceso -> estado == 2)
    {
        // vemos si el io burst en el que se esta llego a 0
        if (proceso -> io_bursts[proceso -> watch_io] == 0)
        {
            // Cambiamos el estado a ready
            proceso -> estado = 1;
        }
        
    }

    if (proceso -> next != NULL)
    {
        pasar_a_ready(proceso -> next);
    }
};

Process* ceder_cpu(Queue* cola, Process* proceso) {
    proceso_siguiente = proceso -> next;
    // si el primer elemento de la cola es 0(RUNNING)
    if (proceso -> estado == 0)
    {
        // vemos si el cou burst en el que se esta llego a 0 y si este no es el ultimo, sino habria terminado
        if (proceso -> cpu_bursts[proceso -> watch_cpu] == 0 && proceso -> watch_cpu != (proceso -> cantidad_rafagas - 1))
        {
            // Cambiamos el estado a waiting
            proceso -> estado = 2;
            //  y no es unico
            if (proceso_siguiente != NULL)
            {
                // el proceso siguiente lo ponemos de primero
                cola -> proceso = proceso_siguiente; 
                // lo agregamos alfinal de la cola
                agregar_alfinal(cola -> proceso, proceso);
            }
            return NULL;
        }
        else
        {
            return proceso;
        }  
    }
    else if (proceso_siguiente -> estado == 0)
    {
        // vemos si el cou burst en el que se esta llego a 0
        if (proceso_siguiente -> cpu_bursts[proceso_siguiente -> watch_cpu] == 0 && proceso_siguiente -> watch_cpu != (proceso_siguiente -> cantidad_rafagas - 1))
        {
            // Cambiamos el estado a waiting
            proceso_siguiente -> estado = 2;

            // el proceso siguiente lo ponemos de primero
            proceso -> next = proceso_siguiente -> next;                                        
            // lo agregamos alfinal de la cola
            agregar_alfinal(proceso, proceso_siguiente);
            return NULL;
        }
        else
        {
            return proceso_siguiente;
        }  
    }
    
    // si no ha llegado a 0, sigue corriendo
    else
    {
        ceder_cpu(cola, proceso -> next);
    }
    
    
};

Process* chequear_termino(Process* proceso) {
    if (proceso -> estado == 0)
    {
        // si termino los bursts time, es decir termino su ejecucion
        if (proceso -> cpu_bursts[proceso -> watch_cpu] == 0 && proceso -> watch_cpu == (proceso -> cantidad_rafagas - 1)) {
            proceso -> estado = 3;
            return NULL;
        }
        else
        {
            return proceso;
        }
        
    }
    else
    {
        chequear_termino(proceso -> next);
    }
}


Process* chequear_quantum(Queue* cola, Process* proceso) {
    proceso_siguiente = proceso -> next;
    if (proceso -> estado == 0)
    {
        if (proceso -> quantum == 0)
        {
            // Cambiamos el estado a ready
            proceso -> estado = 1;
            //  y no es unico
            if (proceso_siguiente != NULL)
            {
                // el proceso siguiente lo ponemos de primero
                cola -> proceso = proceso_siguiente; 
                // lo agregamos alfinal de la cola
                agregar_alfinal(cola -> proceso, proceso);
            }
            return NULL;
        }
        else
        {
            return proceso;
        }
    }

    else if (proceso_siguiente -> estado == 0)
    {
        // vemos si el quantum es 0
        if (proceso_siguiente -> quantum == 0)
        {
            // Cambiamos el estado a waiting
            proceso_siguiente -> estado = 2;

            // el proceso siguiente lo ponemos de primero
            proceso -> next = proceso_siguiente -> next;                                        
            // lo agregamos alfinal de la cola
            agregar_alfinal(proceso, proceso_siguiente);
            return NULL;
        }
        else
        {
            return proceso_siguiente;
        }  
    }
    else
    {
        chequear_quantum(cola, proceso -> next);
    }
}