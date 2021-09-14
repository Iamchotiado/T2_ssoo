#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file_manager/manager.h"
#include "../process/main.c"
#include "../queue/main.c"

Process proceso_check;
Process b;
Process proceso_ag;
Process* procesos_llegados;

Process* proceso_siguiente;
void desempatar (int posiciones[100], int cantidad) {
  int a;

  for (int i = 0; i < cantidad; ++i){
    for (int j = i + 1; j < cantidad; ++j){
        if (posiciones[i] > posiciones[j]){
          a = posiciones[i];
          posiciones[i] = posiciones[j];
          posiciones[j] = a;
        }
    }
  }
};

int main(int argc, char **argv)
{

  int Q = 100;
  
  printf("Hello T2!\n");

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  
  Process* procesos = calloc(file -> len, sizeof(Process));
  int PID = 1;
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    char* nombre = line[0];
    int tiempo_llegada = atoi(line[1]);
    int n_fabrica = atoi(line[2]);
    int n_rafagas = atoi(line[3]);

    int* cpu_bursts = calloc(n_rafagas, sizeof(int));
    int* io_bursts = calloc((n_rafagas -1), sizeof(int));
    int contador_cpu = 0;
    int contador_io = 0;
    for (int j = 4; j < 1000; j++)
    {
      if (j % 2 == 0)
      {
        cpu_bursts[contador_cpu] = atoi(line[j]);
        contador_cpu ++;
        if (contador_cpu == n_rafagas)
        {
          break;
        }
      }
      else
      {
        io_bursts[contador_io] = atoi(line[j]);
        contador_io ++;
      }
    };

    Process* proceso = process_init(PID, nombre, tiempo_llegada, n_fabrica, 1, n_rafagas, cpu_bursts, io_bursts, 0, 0, 0);
    procesos[i] = *proceso;
    PID ++;
  };

  printf("%s...........\n", procesos[2].nombre);

  int tiempo = 0;
  int posiciones[100];
  int numero_llegadas;
  Queue* cola = NULL;
  int cantidad_procesos = 0;
  Process* running = NULL;
  printf("\n TIEMPO %i\n", tiempo);
  while (1)
  {
    numero_llegadas = 0;
    // si se esta usando la cpu
    if (running != NULL && cola != NULL)
    {
      if (tiempo ==4)
      {
        printf("-------------Desoues de ceder la cpu-----------------\n");
        printf("------------------------------------------\n");
        imprimir_cola(cola -> proceso);
        printf("------------------------------------------\n");
      }
      // vemos si el proceso que esta en la cpu, cede la cpu, es decir que se le acabo su rafaga de burst
      // si es asi, lo pasamos a waiting y lo mandamos alfinal de la cola
      running = ceder_cpu(cola, cola -> proceso);
      
      // si el proceso termino su ejecucion
      if (running != NULL)
      {
        running = chequear_termino(cola -> proceso);
      }

      // si consume todo su quantum
      if (running != NULL)
      {
        running = chequear_quantum(cola, cola -> proceso);
      }
      // si, no ocurre ninguno de los eventos anteriores significa que sigue el mismo proceso en la CPU
    };
    if (tiempo ==4)
      {
        printf("-------------Desoues de todos los ifs-----------------\n");
        printf("------------------------------------------\n");
        imprimir_cola(cola -> proceso);
        printf("------------------------------------------\n");
      }
    // Vemos cuantos procesos llegaron en este tiempo
    for (int i = 0; i < file -> len; i++)
    {
      proceso_check = procesos[i];
      if (proceso_check.tiempo_llegada == tiempo)
      {
        posiciones[numero_llegadas] = i;
        numero_llegadas ++;
      };
    }
    if (tiempo ==4)
      {
        printf("-------------Desoues de todos los ifs2-----------------\n");
        printf("------------------------------------------\n");
        imprimir_cola(cola -> proceso);
        printf("------------------------------------------\n");
      }
    
    // si llego un solo nuevo proceso en este instante de tiempo
    if (numero_llegadas == 1)
    {
      proceso_ag = procesos[posiciones[0]];
      // si es el primer elemento en la cola la creamos
      if (cola == NULL)
      {
        cola = queue_init(&proceso_ag, cantidad_procesos, 0, 0, 0, 0);
        cola -> cantidad_procesos ++;
        sumar_fabrica_cola(cola, proceso_ag.n_fabrica);
        printf("Cola creada con nuevo proceso, numero de elementos en la cola: %i, de nombre %s\n", cola -> cantidad_procesos, cola -> proceso -> nombre);
      }
      // si no la agregamos alfinal de la cola
      else
      {
        agregar_alfinal(cola -> proceso, &proceso_ag);
        cola -> cantidad_procesos ++;
        sumar_fabrica_cola(cola, proceso_ag.n_fabrica);
        printf("Se agrego el proceso a la cola, numero de elementos en la cola: %i, de nombre %s\n", cola -> cantidad_procesos, proceso_ag.nombre);
        if (tiempo ==4)
        {
          printf("-------------Despues de agregar el proceso 4-----------------\n");
          printf("------------------------------------------\n");
          imprimir_cola(cola -> proceso);
          printf("------------------------------------------\n");
        }
      }
    };

    // si llega mas de un proceso, desempatamos
    if (numero_llegadas > 1)
    {

      procesos_llegados = calloc(numero_llegadas, sizeof(Process));
      for (int i = 0; i < numero_llegadas; i++)
      {
        procesos_llegados[i] = procesos[posiciones[i]];
      }
      
      // ordenamos segun menor numero de fabrica
      // descomentar linea de abajo si no se arregla
      // Process b;
      for (int i = 0; i < numero_llegadas; i++)
      {
        for (int j = i + 1; j < numero_llegadas; ++j){
            if (procesos_llegados[i].n_fabrica > procesos_llegados[j].n_fabrica){
              b = procesos_llegados[i];
              procesos_llegados[i] = procesos_llegados[j];
              procesos_llegados[j] = b;
            }
        }
      }
      // Todo lo comentado aca es el desempate segun el segundo criterio
      // vemos si hay algun empate en el numero de fabrica para desempatar por el segundo criterio
      // int f1 = 0;
      // int f2 = 0;
      // int f3 = 0;
      // int f4 = 0;
      // int pf1 = 0;
      // int pf2 = 0;
      // int pf3 = 0;
      // int pf4 = 0;
      // for (int i = 0; i < numero_llegadas; i++)
      // {
      //   if (procesos_llegados[i].n_fabrica == 1)
      //   {
      //     f1 ++;
      //     pf1 = i;
      //   }
      //   else if (procesos_llegados[i].n_fabrica == 2)
      //   {
      //     f2 ++;
      //     pf2 = i;
      //   }
      //   else if (procesos_llegados[i].n_fabrica == 3)
      //   {
      //     f3 ++;
      //     pf3 = i;
      //   }
      //   else if (procesos_llegados[i].n_fabrica == 4)
      //   {
      //     f4 ++;
      //     pf4 = i;
      //   }
      // }
      
      // ordenamos segun menor nombre de proceso
      // Process a;
      // for (int i = 0; i < numero_llegadas; i++)
      // {
      //   for (int j = i + 1; j < numero_llegadas; ++j){
      //       if (strcmp(procesos_llegados[i].nombre, procesos_llegados[j].nombre) < 0){
      //         a = procesos_llegados[i];
      //         procesos_llegados[i] = procesos_llegados[j];
      //         procesos_llegados[j] = a;
      //       }
      //   }
      // }

      // ahora que estan ordenados, agregamos todos al final de la cola
      // si es el primer elemento en la cola la creamos y agregamos el resto de los procesos
      if (cola == NULL)
      {
        cola = queue_init(&procesos_llegados[0], cantidad_procesos, 0, 0, 0, 0);
        cola -> cantidad_procesos ++;
        sumar_fabrica_cola(cola, procesos_llegados[0].n_fabrica);
        printf("Cola creada con nuevo proceso, numero de elementos en la cola: %i, de nombre %s\n", cola -> cantidad_procesos, cola -> proceso -> nombre);
        for (int i = 1; i < numero_llegadas; i++)
        {
          agregar_alfinal(cola -> proceso, &procesos_llegados[i]);
          cola -> cantidad_procesos ++;
          sumar_fabrica_cola(cola, procesos_llegados[i].n_fabrica);
          printf("Se agrego el proceso a la cola, numero de elementos en la cola: %i, de nombre %s\n", cola -> cantidad_procesos, procesos_llegados[i].nombre);
        }
        
      }
      // si no la agregamos alfinal de la cola
      else
      {
        for (int i = 0; i < numero_llegadas; i++)
        {
          agregar_alfinal(cola -> proceso, &procesos_llegados[i]);
          cola -> cantidad_procesos ++;
          sumar_fabrica_cola(cola, procesos_llegados[i].n_fabrica);
          printf("Se agrego el proceso a la cola, numero de elementos en la cola: %i, de nombre %s\n", cola -> cantidad_procesos, procesos_llegados[i].nombre);
        }
      }
      
    };
    if (tiempo ==4)
      {
        printf("\n-------------Antes de elegir proceso que va a la cpu(despues de que llego el proceso4)-----------------\n");
        printf("------------------------------------------\n");
        imprimir_cola(cola -> proceso);
        printf("------------------------------------------\n");
      }

    // vemo si hay algun proceso en la cpu, sino eligimos 1 para que pase a running
    if (running == NULL && cola != NULL)
    {
      // agregamos el siguiente proceso ready a la cpu y calculamos su quantum
      running = agregar_cpu(cola -> proceso);
      // si es que habia algun proceso ready para ser agregado a la cpu
      if (running != NULL)
      {
        int quantum = calcular_quantum(cola, running -> n_fabrica, Q);
        agregar_quantum(cola -> proceso, quantum);
        // la funcion agregar quantum imprime el quantum
        printf("El proceso: %s, paso a la cpu(running)\n", running -> nombre);
      }
    };

    // actualizamos las estadisticas de los procesos
    
    if (cola != NULL)
    {
      actualizar_datos(cola -> proceso);
    }
    
    // vemos que los procesos que terminaron io bursts pasen a ready
    if (cola != NULL)
    {
      pasar_a_ready(cola -> proceso);
    }
    // pasamos a la siguiente unidad de tiemppo
    if (tiempo > 0)
    {
      printf("------------------------------------------\n");
      imprimir_cola(cola -> proceso);
      printf("------------------------------------------\n");
    }
    
    sleep(1);
    tiempo ++;
    printf("\n TIEMPO %i\n", tiempo);
  }
  

  

  // while(1) {
  //   int numero_llegadas = 0;
  //   for (int i = 0; i < file->len; i++)
  //   {
  //     char **line = file->lines[i];
  //     // char nombre[254] = line[0];
  //     int tiempo_llegada = atoi(line[1]);
  //     int n_fabrica = atoi(line[2]);
  //     // int n_rafagas = atoi(line[3]);
  //     // int cpu_bursts[n_rafagas];
  //     // int io_bursts[n_rafagas - 1];
  //     printf(
  //         "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
  //         line[0], line[2], line[1], line[3]);
      
  //     if (tiempo_llegada == tiempo)
  //     {
  //       char* nombre = line[0];
  //       nombres[numero_llegadas] = nombre;
  //       posiciones[numero_llegadas] = i;
  //       nums_fabrica[numero_llegadas] = n_fabrica;
  //       numero_llegadas ++;
  //     }
  //   };


  //   // si  llego alguien en este tiempo
  //   if (numero_llegadas > 0)
  //   {
  //     for (int i = 0; i < numero_llegadas; i++)
  //     {
  //       /* code */
  //     }
      
  //   }
    
  // }

  // char* input = argv[1];
  // printf("input%s", input);
}
