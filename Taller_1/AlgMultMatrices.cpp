# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <ctime>

# include "mpi.h"

using namespace std;

int main ( int argc, char *argv[] ); 

const int MAX = 100;

void timestamp ( );
int mult_matrices(int n, int id, int p);

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
	n_hi = 2000000;
	n_factor = 2;
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
		cout << "\n       N        Suma valores matriz resultante          Tiempo\n";
		cout << "\n";
	} 
	n = n_lo;
	
	while ( n <= n_hi ){
		if ( id == master ){
			wtime = MPI::Wtime ( );
		}
		MPI::COMM_WORLD.Bcast ( &n, 1, MPI::INT, master );

		matrices_part = mult_matrices ( n, id, p );

		MPI::COMM_WORLD.Reduce ( &matrices_part, &matriz_suma, 1, MPI::INT, MPI::SUM,
		master );

		if ( id == master ){
			wtime = MPI::Wtime ( ) - wtime;
			//cout << "\n";
			//cout << "       N        Suma valores matrices(1 y 2)          Tiempo\n";
			//cout << "\n";
			cout << "" << setw(8) << n
			<< "  " << setw(24) << matriz_suma
			<< "  " << setw(24) << wtime << "\n";
			//cout << "\n";
		}
		n = n * n_factor;
	}
	
	
	MPI::Finalize ( );

	if ( id == master ){
		cout << "\n";
		cout << "PRIME_MPI - Procesos maestro:\n";
		cout << "  Finalizacion del calculo normal.\n";
	}

	return 0; 
}

int mult_matrices(int n, int id, int p){
	int i;
	int count=0;
	int suma_mult=0;
	for ( i = 2 + id; i <= n; i = i + p )
  	{ 
		int matrix1[MAX][MAX], matrix2[MAX][MAX], result[MAX][MAX];
		int rows1, cols1, rows2, cols2;

		// Generar las dimensiones de las matrices aleatoriamente
		srand(time(NULL));
		rows1 = rand() % MAX + 1;
		cols1 = rand() % MAX + 1;
		rows2 = cols1;
		cols2 = rand() % MAX + 1;

		// Generar los elementos de la matriz 1 aleatoriamente
		for (int i = 0; i < rows1; i++) {
			for (int j = 0; j < cols1; j++) {
				matrix1[i][j] = rand() % 100;
			}
		}

		// Generar los elementos de la matriz 2 aleatoriamente
		for (int i = 0; i < rows2; i++) {
			for (int j = 0; j < cols2; j++) {
				matrix2[i][j] = rand() % 100;
			}
		}

		// Multiplicar las matrices
		for (int i = 0; i < rows1; i++) {
			for (int j = 0; j < cols2; j++) {
				result[i][j] = 0;
				for (int k = 0; k < cols1; k++) {
					result[i][j] += matrix1[i][k] * matrix2[k][j];
				}
			}
		}
		
		// Imprimir las matrices y la matriz resultante
		//cout << "        --> Matriz 1 [" << rows1 << "," << cols1 << "]";
		for (int i = 0; i < rows1; i++) {
			for (int j = 0; j < cols1; j++) {
				//cout << matrix1[i][j] << " ";
				count = count + matrix1[i][j];
			}
			//cout << endl;
		}

		//cout << " * Matriz 2 [" << rows2 << "," << cols2 << "]" << endl;
		for (int i = 0; i < rows2; i++) {
			for (int j = 0; j < cols2; j++) {
				//cout << matrix2[i][j] << " ";
				count = count + matrix2[i][j];
			}
			//cout << endl;
		}

		//cout << "            Matriz resultante [" << rows1 << "," << cols2 << "]";
		for (int i = 0; i < rows1; i++) {
			for (int j = 0; j < cols2; j++) {
				//cout << result[i][j] << " ";
				suma_mult = suma_mult + result[i][j];
			}
			//cout << endl;
		}
		//cout << " - Suma de los valores multiplicados: " << suma_mult << endl;
	}
	return suma_mult;
}

 void timestamp ( )
{
	# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm *tm;
	size_t len;
	time_t now;

	now = time ( NULL );
	tm = localtime ( &now );

	len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

	cout << time_buffer << "\n";

	return;
	# undef TIME_SIZE
}
