#pragma once

#include "../process/main.h"

typedef struct queue
{
  /** Arreglo de procesos */
  Process* procesos;
  /** Largo del arreglo "procesos" */
  int cantidad_procesos;
} Queue;

Queue* queue_init(int cantidad_procesos);
void add_process(Process proceso);
Process delete_process(Process* procesos, Process proceso);
void add_to_end(Process* procesos, Process proceso);
