#include <mpi.h>
#include <stdio.h>
#include <math.h>
float f(float x) {
   return x * x; // Example: y = x^2
}
float trapezoid_area(float a, float b, float d) {
   float area = 0.0f;
   
   for (float x = a; x < b; x += d) {
       area += (f(x) + f(x + d)) * d / 2.0f; // Trapezoidal rule formula
   }
   return area;
}
 
int main(int argc, char** argv) {
   int rank, size, n;
   float a = 0.0f, b = 1.0f;  // Limits of integration
   float d, region, start, end, local_area, total_area;
   double local_start_time, local_end_time, local_elapsed_time, max_time;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
   MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processes
 
   if (rank == 0) {
       printf("Enter the number of intervals: ");
       scanf("%d", &n);
   }
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
   // Calculate delta (width of each subinterval)
   d = (b - a) / n;
   
   region = (b - a) / size;
   start = a + rank * region;
   end = start + region;
   local_start_time = MPI_Wtime();
 
   local_area = trapezoid_area(start, end, d);
 
   local_end_time = MPI_Wtime();
   local_elapsed_time = local_end_time - local_start_time;
 
   MPI_Reduce(&local_area, &total_area, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
 
   MPI_Reduce(&local_elapsed_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 
   if (rank == 0) {
       printf("The total area under the curve is: %f\n", total_area);
       printf("Maximum time taken by any process: %f seconds\n", max_time);
   }
 
   MPI_Finalize();
   return 0;
}