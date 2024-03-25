#!/bin/bash

# Target product
target_product=6

for num_threads in {1..6}
do
  for num_ranks in {1..6}
  do
    # Check if the product is equal to the target
    product=$((num_threads * num_ranks))

    if [ $product -eq $target_product ]; then
      echo "Testing num_threads=$num_threads and num_ranks=$num_ranks"
      export OMP_NUM_THREADS=$num_threads
      mpirun -n $num_ranks pi.exe

      echo "Testing num_threads=$num_ranks and num_ranks=$num_threads"
      export OMP_NUM_THREADS=$num_ranks
      mpirun -n $num_threads pi.exe
    fi
  done
done
