#pragma once

typedef struct process Process;
struct process
{
  /** PID proceso */
  int PID;
//   Nombre
  char* nombre;
  // tiempo en que llega a la fabrica
  int tiempo_llegada;
  // numero de fabrica
  int n_fabrica;
  // estado
  int estado;
  // 0 = RUNNING, 1 = READY, 2 = WAITING, 3 = FINISHED.
  
  int cantidad_rafagas;
  char** io_bursts;
  char** cpu_bursts;

  Process* next;
};

Process* process_init(int PID, char* nombre, int tiempo_llegada, int n_fabrica, int estado, int cantidad_rafagas, char** cpu_burst, char** io_burst);
void stop_process(Process proceso);





