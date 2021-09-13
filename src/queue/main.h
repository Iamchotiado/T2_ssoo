#pragma once

#include "../process/main.h"

typedef struct queue
{
  /** Puntero a el primer proceso en la lista ligada */
  Process* proceso;
  /** cantidad de procesos dentro de la lista */
  int cantidad_procesos;
  /** cantidad de procesos dentro de la lista de la fabrica 1*/
  int cantidad_f1;
  /** cantidad de procesos dentro de la lista de la fabrica 2*/
  int cantidad_f2;
  /** cantidad de procesos dentro de la lista de la fabrica 3*/
  int cantidad_f3;
  /** cantidad de procesos dentro de la lista de la fabrica 4*/
  int cantidad_f4;
} Queue;

Queue* queue_init(Process* proceso, int cantidad_procesos, int cantidad_f1, int cantidad_f2, int cantidad_f3, int cantidad_f4);
void agregar_alfinal(Process* insertado, Process* insertar);
Process* agregar_cpu(Process* proceso);
void actualizar_estadisticas(Process* proceso); 
void sumar_fabrica_cola(Queue* cola, int n_fabrica);
int calcular_quantum(Queue* cola, int n_fabrica, int Q);
void agregar_quantum(Process* proceso, int quantum);


void add_process(Process proceso);
Process delete_process(Process* procesos, Process proceso);
void add_to_end(Process* procesos, Process proceso);
