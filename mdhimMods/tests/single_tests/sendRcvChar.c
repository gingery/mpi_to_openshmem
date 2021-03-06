/*
 *  sendRcvChar.c
 *  mpiToOpenshmem
 *
 *  Created by gingery on 10/28/13.
 *  Copyright 2013 LANL. All rights reserved.
 *
 *  Simple send receive a character
 */
#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "mpi_to_openshmem.h"

int main(int argc, char *argv[])
{
  int provided, ret, size, rank;
  int nextpe;
  char src;
  char *dest;

  MPI_Status status;
  
  MPI_Init_thread(&argc, &argv,  MPI_THREAD_MULTIPLE, &provided);
  if (ret != MPI_SUCCESS) {
    printf("Error initializing MPI with threads\n");
    exit(1);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  nextpe = (rank + 1) % size;

  if (rank == 0) 
    src = 'a';
  else if (rank == 1)
    src = 'b';
  else
    src = 'c';

  dest = (char *) malloc (sizeof (*dest));
  if (dest == NULL){
    printf("Couldn't malloc.\n");
  }

  *dest = 'z';
  
  MPI_Barrier( MPI_COMM_WORLD );

  //shmem_int_put (dest, &src, 1, nextpe);
  MPI_Send(&src, 1, MPI_CHAR, nextpe, 123, MPI_COMM_WORLD);
  // shmem_int_get (dest, &src, 1, nextpe);
  MPI_Recv(dest, 1, MPI_CHAR, nextpe, 123, MPI_COMM_WORLD, &status);

  MPI_Barrier (MPI_COMM_WORLD );

  printf ("%4d: got %4c, %4c: ", rank, *dest,src);
  if (   ((rank == 0) && ( *dest = 'a')) 
      || ((rank == 1) && ( *dest = 'b'))
      || ((rank == 2) && ( *dest = 'c')) ){
      printf ("pe: %d, CORRECT\n", rank);
  }
  else{
      printf ("pe: %d, WRONG, expected a, b, or c\n", rank);
  }

  MPI_Barrier (MPI_COMM_WORLD );

  return 0;

}
