#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include "../process/main.c"

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
    // int cpu_bursts[n_rafagas];
    // int io_bursts[n_rafagas - 1];

    char** cpu_bursts = malloc(n_rafagas * sizeof(char*));
    char** io_bursts = malloc((n_rafagas -1) * sizeof(char*));
    int contador_cpu = 0;
    int contador_io = 0;
    for (int j = 4; j < 1000; j++)
    {
      if (j % 2 == 0)
      {
        cpu_bursts[contador_cpu] = line[j];
        contador_cpu ++;
        if (contador_cpu == n_rafagas)
        {
          break;
        }
      }
      else
      {
        io_bursts[contador_io] = line[j];
        contador_io ++;
      }
    };

    Process* proceso = process_init(PID, nombre, tiempo_llegada, n_fabrica, 1, n_rafagas, cpu_bursts, io_bursts);
    procesos[i] = *proceso;
    PID ++;
  };
  printf("%s...........\n", procesos[1].nombre);

  int cpu_use = 0;
  int tiempo = 0;
  int posiciones[100];
  int numero_llegadas = 0;

  while (1)
  {
    // si se esta usando la cpu
    if (cpu_use == 1)
    {
      /* code */
    };

    for (int i = 0; i < file -> len; i++)
    {
      Process proceso_check = procesos[i];
      if (proceso_check.tiempo_llegada == tiempo)
      {
        posiciones[numero_llegadas] = i;
        numero_llegadas ++;
      };
      
    }
    // si llego un solo nuevo proceso en este instante de tiempo
    if (numero_llegadas == 1)
    {
      /* code */
    }
    
    
    
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
