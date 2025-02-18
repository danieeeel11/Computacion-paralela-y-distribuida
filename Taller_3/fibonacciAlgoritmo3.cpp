#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <time.h>

#include "mpi.h"

using namespace std;

int main ( int argc, char *argv[] );

const int MAX = 100;

void timestamp ( );
int fib(int n);

int main( int argc, char *argv[]) {
	int i;
	int id;
	int master = 0;
	int n;
	int n_factor;
	int n_hi;
	int n_lo;
	int p;
	int matriz_suma;
	int matrices_part;
	double wtime;

	n_lo = 1;
	n_hi = 100;
	MPI::Init ( argc, argv );
	p = MPI::COMM_WORLD.Get_size ( );
	id = MPI::COMM_WORLD.Get_rank ( );
	
	if ( id == master )
	{
		cout << "\n";
		cout << "'Cuenta multiplicación de matrices\n";
		cout << "  C++/MPI version\n";
		cout << "\n";
		cout << "  Programa para multiplicar matrices.\n";
		cout << "  Corriendo en " << p << " procesos\n";
		cout << "\n      N     Suma valores matriz resultante            Tiempo\n";
		cout << "\n";
	}
	n = n_lo;
	
	while ( n <= n_hi ){
		if ( id == master ){
		   wtime = MPI::Wtime ( );
		}
		MPI::COMM_WORLD.Bcast ( &n, 1, MPI::INT, master );

		matrices_part = fib ( n );
	
		MPI::COMM_WORLD.Reduce ( &matrices_part, &matriz_suma, 1, MPI::INT, MPI::SUM, master );

		if ( id == master ){
			wtime = MPI::Wtime ( ) - wtime;
		cout << "" << setw(8) << n
			<< "  " << setw(24) << matriz_suma
			<< "  " << setw(24) << wtime << "\n";
		}
		n = n + 1;
	}
   
   
	MPI::Finalize ( );

	if ( id == master ){
		cout << "\n";
		cout << "PRIME_MPI - Procesos maestro:\n";
		cout << "  Finalizacion del calculo normal.\n";
	}

	return 0;
}

int fib(int n){
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}

void timestamp() {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  char timestamp[32];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S.%f", tm);
  printf("%s\n", timestamp);
}
