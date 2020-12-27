#include "nbody.hpp"
#include <mpi.h>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <string>
using namespace std;

void calculate_force(Particle *this_particle1, Particle *this_particle2,
										 float *force_x, float *force_y, float *force_z)
{
	/* Particle_calculate_force */
	float difference_x, difference_y, difference_z;
	float distance_squared, distance;
	float force_magnitude;

	difference_x = this_particle2->position_x - this_particle1->position_x;
	difference_y = this_particle2->position_y - this_particle1->position_y;
	difference_z = this_particle2->position_z - this_particle1->position_z;

	distance_squared = difference_x * difference_x +
										 difference_y * difference_y +
										 difference_z * difference_z;

	distance = std::sqrt(distance_squared); //sqrtf(distance_squared);

	force_magnitude = GRAVITATIONAL_CONSTANT * (this_particle1->mass) * (this_particle2->mass) / distance_squared;

	*force_x = (force_magnitude / distance) * difference_x;
	*force_y = (force_magnitude / distance) * difference_y;
	*force_z = (force_magnitude / distance) * difference_z;
}

void nbody(Particle *d_particles, Particle *output, int inicio, int fim)
{
#pragma omp parallel
	{
#pragma omp for
		for (int id = inicio; id < fim; id++)
		{
			Particle *this_particle = &output[id];

			float force_x = 0.0f, force_y = 0.0f, force_z = 0.0f;
			float total_force_x = 0.0f, total_force_y = 0.0f, total_force_z = 0.0f;

			int i;
			for (i = inicio; i < fim; i++)
			{
				if (i != id)
				{
					calculate_force(d_particles + id, d_particles + i, &force_x, &force_y, &force_z);

					total_force_x += force_x;
					total_force_y += force_y;
					total_force_z += force_z;
				}
			}

			float velocity_change_x, velocity_change_y, velocity_change_z;
			float position_change_x, position_change_y, position_change_z;

			this_particle->mass = d_particles[id].mass;

			velocity_change_x = total_force_x * (time_interval / this_particle->mass);
			velocity_change_y = total_force_y * (time_interval / this_particle->mass);
			velocity_change_z = total_force_z * (time_interval / this_particle->mass);

			position_change_x = d_particles[id].velocity_x + velocity_change_x * (0.5 * time_interval);
			position_change_y = d_particles[id].velocity_y + velocity_change_y * (0.5 * time_interval);
			position_change_z = d_particles[id].velocity_z + velocity_change_z * (0.5 * time_interval);

			this_particle->velocity_x = d_particles[id].velocity_x + velocity_change_x;
			this_particle->velocity_y = d_particles[id].velocity_y + velocity_change_y;
			this_particle->velocity_z = d_particles[id].velocity_z + velocity_change_z;

			this_particle->position_x = d_particles[id].position_x + position_change_x;
			this_particle->position_y = d_particles[id].position_y + position_change_y;
			this_particle->position_z = d_particles[id].position_z + position_change_z;
		}
	}
}

/* main */
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Informe um arquivo com os parâmetros de entrada: ./nbody_simulation <input_file.in>\n";
		std::abort();
	}
	int number_of_threds = atoi(argv[2]);
	printf("Número de threads: %d\n", number_of_threds);
	omp_set_dynamic(0);										 // Explicitly disable dynamic teams
	omp_set_num_threads(number_of_threds); // Use number_of_threds for all consecutive parallel regions

	// ENTRADA
	FILE *input_data = fopen(argv[1], "r");
	Particle_input_arguments(input_data);

	// SAIDA
	// cria o arquivo para salvar os tempos de execução
	string out_file_name = "resultados_nbody_" + to_string(number_of_threds) + "_threads.txt";
	char *c_out_file_name = const_cast<char *>(out_file_name.c_str());

	FILE *results_file = fopen(c_out_file_name, "w");

	// -------------------------------------------------------------------------------------------------------------------
	MPI_Init(NULL, NULL);

	int myRank, worldSize;

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize); //total of ranks in the comunicator

	// MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
	MPI_Bcast(&number_of_particles, 1, MPI_INT, 0, MPI_COMM_WORLD);

	Particle *particle_array = nullptr;
	Particle *particle_array2 = nullptr;
	Particle *sub_particle_array = nullptr;
	particle_array = Particle_array_construct(number_of_particles);
	particle_array2 = Particle_array_construct(number_of_particles);
	sub_particle_array = Particle_array_construct(number_of_particles);

	if (myRank == 0)
	{
		printf("Processando simulação NBody....\n");
		Particle_array_initialize(particle_array, number_of_particles);
	}

	long start, end;

	for (int timestep = 1; timestep <= number_of_timesteps; timestep++)
	{
		if (myRank == 0)
		{
			start = wtime();
			printf("   Iteração %d OK\n", timestep);
		}

		// MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
		MPI_Bcast(particle_array, sizeof(Particle *) * number_of_particles, MPI_BYTE, 0, MPI_COMM_WORLD);

		int inicio = myRank * number_of_particles / worldSize;
		int fim = (myRank + 1) * number_of_particles / worldSize - 1;

		nbody(particle_array, particle_array2, inicio, fim);

		// MPI_Gather( void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)
		MPI_Gather(particle_array, sizeof(Particle) * number_of_particles, MPI_BYTE, particle_array2, sizeof(Particle) * number_of_particles, MPI_BYTE, 0, MPI_COMM_WORLD);
	}

	if (myRank == 0)
	{
		end = wtime();
		double time = (end - start) / 1000000.0;

		printf("Simulação NBody executada com sucesso.\n");
		printf("Nro. de Partículas: %d\n", number_of_particles);
		printf("Nro. de Iterações: %d\n", number_of_timesteps);
		printf("Tempo: %.8f segundos\n", time);

		// ------------------------------------------------
		printf("\nImprimindo saída em arquivo...\n");

		fprintf(results_file, "%f\n", time);

#ifdef VERBOSE
		//Imprimir saída para arquivo
		printf("\nImprimindo saída em arquivo...\n");
		FILE *fileptr = fopen("nbody_simulation.out", "w");
		Particle_array_output_xyz(fileptr, particle_array, number_of_particles);
		printf("Saída da simulação salva no arquivo nbody_simulation.out\n");
#endif
	}

	particle_array = Particle_array_destruct(particle_array, number_of_particles);
	particle_array2 = Particle_array_destruct(particle_array2, number_of_particles);

	fclose(results_file);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return PROGRAM_SUCCESS_CODE;
}
