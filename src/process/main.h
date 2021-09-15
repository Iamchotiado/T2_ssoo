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
  int* io_bursts;
  int* cpu_bursts;
  // siguiente proceso en la cola
  Process* next;
  // quantum del proceso
  int quantum;
  // en que cpu_bursts estamos
  int watch_cpu;
  // en que io_bursts estamos
  int watch_io;

  // para las estadisticas

  // cantidad de veces que uso la cpu
  int veces_cpu;
  // numero de veces que fue interrumpido, se le acabo el quantum
  int veces_quantum;
  // tiempo de permanencia en el sistema
  int tiempo_permanencia;
  // tiempo en que pasa a running por primera vez
  int tiempo_running;
  //  tiempo que esta en ready and waiting
  int tiempo_cola;
};

Process* process_init(int PID, char* nombre, int tiempo_llegada, int n_fabrica, int estado, int cantidad_rafagas, int* cpu_burst, int* io_burst, int quantum, int watch_cpu, int watch_io);
void stop_process(Process proceso);





