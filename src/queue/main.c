#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

Process* proceso_agregar;
// Process* proceso_siguiente;
// Process* proceso_siguiente1;
Process** terminados;

Queue* queue_init(Process* proceso, int cantidad_procesos, int cantidad_f1, int cantidad_f2, int cantidad_f3, int cantidad_f4) {
    Queue* queue = calloc(1, sizeof(Queue));
    *queue = (Queue){.proceso = proceso, .cantidad_procesos = cantidad_procesos, .cantidad_f1 = cantidad_f1, .cantidad_f2 = cantidad_f2, .cantidad_f3 = cantidad_f3, .cantidad_f4 = cantidad_f4};
    return queue;
};

void agregar_alfinal(Process* insertado, Process* insertar) {
    if (insertado -> next == NULL) {
        insertado -> next = insertar;
        insertado -> next -> next = NULL;
        if (insertado -> next -> estado == 2)
        {
            printf("El proceso de nombre %s dejo la CPU y pasa a estado Waiting\n", insertado -> next -> nombre);
        }
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
        printf("EL proceso: %s  paso a la CPU(running), con un quantum de: %i\n", proceso -> nombre, proceso -> quantum);
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
        printf("El proceso: %s, le queda un quantum de: %i y un cpu burst de : %i\n", proceso ->nombre, proceso->quantum, proceso->cpu_bursts[proceso -> watch_cpu]);
    }

    // si el proceso esta en ready
    else if (proceso -> estado == 1)
    {
        // printf("Estoy en ready: %s\n", proceso->nombre);
        // no se actualiza nada por el momento
    }
    
    // si el proceso esta en waiting
    else if (proceso -> estado == 2)
    {
        // printf("\nESTOY EN WAITING\n");
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
        printf("El proceso: %s, le queda un IO burst de : %i\n", proceso ->nombre, proceso->io_bursts[proceso -> watch_io]);
    }

    // si el proceso esta en finished
    else if (proceso -> estado == 3)
    {
        printf("Soy: %s y termine\n", proceso -> nombre);
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
            printf("El proceso %s paso a estado Ready\n", proceso -> nombre);
        }
        
    }

    if (proceso -> next != NULL)
    {
        pasar_a_ready(proceso -> next);
    }
};

Process* ceder_cpu(Queue* cola, Process* proceso) {
    // proceso_siguiente = proceso -> next;
    // si el primer elemento de la cola es 0(RUNNING)
    // printf("\nENTROOOO1\n");
    if (proceso -> estado == 0)
    {
        // printf("\nENTROOOO2\n");
        // vemos si el cou burst en el que se esta llego a 0 y si este no es el ultimo, sino habria terminado
        if (proceso -> cpu_bursts[proceso -> watch_cpu] == 0 && proceso -> watch_cpu != (proceso -> cantidad_rafagas - 1))
        {
            // printf("\nENTROOOO3\n");
            // Cambiamos el estado a waiting
            proceso -> estado = 2;
            //  y no es unico
            if (proceso -> next != NULL)
            {
                // el proceso siguiente lo ponemos de primero
                cola -> proceso = proceso -> next; 
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
    else if (proceso -> next -> estado == 0)
    {
        // vemos si el cou burst en el que se esta llego a 0
        if (proceso -> next -> cpu_bursts[proceso -> next -> watch_cpu] == 0 && proceso -> next -> watch_cpu != (proceso -> next -> cantidad_rafagas - 1))
        {
            // Cambiamos el estado a waiting
            proceso_agregar = proceso -> next;
            proceso_agregar -> estado = 2;
            // el proceso siguiente lo ponemos de primero
            proceso -> next = proceso -> next -> next;                                        
            // lo agregamos alfinal de la cola
            agregar_alfinal(proceso, proceso_agregar);
            return NULL;
        }
        else
        {
            return proceso -> next;
        }  
    }
    
    // si no ha llegado a 0, sigue corriendo
    else
    {
        ceder_cpu(cola, proceso -> next);
    }
    
    
};

Process* chequear_termino(Queue* cola, Process* proceso) {
    if (proceso -> estado == 0)
    {
        // si termino los bursts time, es decir termino su ejecucion
        if (proceso -> cpu_bursts[proceso -> watch_cpu] == 0 && proceso -> watch_cpu == (proceso -> cantidad_rafagas - 1)) {
            proceso -> estado = 3;
            //  y no es unico
            if (proceso -> next != NULL)
            {
                // el proceso siguiente lo ponemos de primero
                cola -> proceso = proceso -> next; 
                // lo agregamos alfinal de la cola
                // agregar_alfinal(cola -> proceso, proceso);
            }
            // si es el unico y termino
            else
            {
                cola -> proceso = NULL;
            }
            printf("Proceso: %s termino su ejecucion y fue eliminado del sistema\n", proceso -> nombre);
            return NULL;
        }
        else
        {
            return proceso;
        }
        
    }
    else
    {
        chequear_termino(cola, proceso -> next);
    }
}


Process* chequear_quantum(Queue* cola, Process* proceso) {
    // proceso_siguiente1 = proceso -> next;
    if (proceso -> estado == 0)
    {
        if (proceso -> quantum == 0)
        {
            // Cambiamos el estado a ready
            proceso -> estado = 1;
            //  y no es unico
            if (proceso -> next != NULL)
            {
                // el proceso siguiente lo ponemos de primero
                cola -> proceso = proceso -> next; 
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

    else if (proceso -> next -> estado == 0)
    {
        // vemos si el quantum es 0
        if (proceso -> next -> quantum == 0)
        {
            // Cambiamos el estado a waiting
            proceso_agregar = proceso -> next;
            proceso_agregar -> estado = 2;

            // el proceso siguiente lo ponemos de primero
            proceso -> next = proceso -> next -> next;                                        
            // lo agregamos alfinal de la cola
            agregar_alfinal(proceso, proceso_agregar);
            return NULL;
        }
        else
        {
            return proceso -> next;
        }  
    }
    else
    {
        chequear_quantum(cola, proceso -> next);
    }
};


void imprimir_cola(Process* proceso) {
    if (proceso != NULL)
    {
        printf("Nombre: %s, Estado: %i\n", proceso -> nombre, proceso -> estado);
        imprimir_cola(proceso -> next);
    }
};

void crear_array_terminados(int size) {
    terminados = calloc(size, sizeof(Process*));
}
