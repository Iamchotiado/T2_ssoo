#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"


Process* process_init(int PID, char* nombre, int tiempo_llegada, int n_fabrica, int estado, int cantidad_rafagas, int* cpu_burst, int* io_burst, int quantum) {
  Process* process = calloc(1, sizeof(Process));
  *process = (Process){.PID = PID, .nombre = nombre, .tiempo_llegada = tiempo_llegada, .n_fabrica = n_fabrica, .estado = estado, .cantidad_rafagas = cantidad_rafagas, .cpu_bursts = cpu_burst, .io_bursts = io_burst, .next = NULL, .quantum = quantum};
  return process;
}