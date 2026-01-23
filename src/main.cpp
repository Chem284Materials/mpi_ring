#include <iostream>
#include <vector>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  // Get my_rank and my_size
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Size of a large vector
  int N = 10000;

  // Decide which indices of the array each rank will store
  std::vector<int> start_index(world_size);
  std::vector<int> end_index(world_size);
  int current_start_index = 0;
  for (int irank = 0; irank < world_size; ++irank) {
    start_index[irank] = current_start_index;
    int nindices = N / world_size;
    if ( irank < N % world_size ) ++nindices;
    end_index[irank] = start_index[irank] + nindices;
    current_start_index = end_index[irank];
  }

  // Initialize the portion of the array that is stored on this rank
  std::vector<double> my_vec(end_index[my_rank] - start_index[my_rank]); 
  for (int index = 0; index < end_index[my_rank] - start_index[my_rank]; index++) {
    my_vec[index] = index + start_index[my_rank];
  }

  // Average the portion of the array stored on this rank
  double average = 0.0;
  for (int index = 0; index < end_index[my_rank] - start_index[my_rank]; index++) {
    average += my_vec[index] / N;
  }

  std::cout << "Rank: " << my_rank << "   Average: " << average << '\n';

  MPI_Finalize();
  return 0;
}

