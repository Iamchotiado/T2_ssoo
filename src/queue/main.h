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
void actualizar_datos(Process* proceso);
void pasar_a_ready(Process* proceso);
Process* ceder_cpu(Queue* cola, Process* proceso);
void eliminar_de_cola(Queue* cola, int PID);
Process* chequear_termino(Queue* cola, Process* proceso);
Process* chequear_quantum(Queue* cola, Process* proceso);

void imprimir_cola(Process* proceso);


