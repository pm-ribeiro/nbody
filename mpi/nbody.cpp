#include "nbody.hpp"

/*
 * Global variables - input
 */
char     base_filename[DEFAULT_STRING_LENGTH+1];

int      number_of_particles;
int      block_size;
float    domain_size_x;
float    domain_size_y;
float    domain_size_z;
float    time_interval;
int      number_of_timesteps;
int      timesteps_between_outputs;
bool     execute_serial;
unsigned random_seed;
float    mass_maximum;


/*
 * Get command line arguments.
 */
void Particle_input_arguments(FILE *input) { 
	number_of_particles       = DEFAULT_NUMBER_OF_PARTICLES;
	block_size                = DEFAULT_NUMBER_OF_PARTICLES;
	domain_size_x             = DEFAULT_DOMAIN_SIZE_X;
	domain_size_y             = DEFAULT_DOMAIN_SIZE_Y;
	domain_size_z             = DEFAULT_DOMAIN_SIZE_Z;
	time_interval             = DEFAULT_TIME_INTERVAL;
	number_of_timesteps       = DEFAULT_NUMBER_OF_PARTICLES;
	timesteps_between_outputs = DEFAULT_TIMESTEPS_BETWEEN_OUTPUTS;
	execute_serial            = DEFAULT_EXECUTE_SERIAL;
	random_seed               = DEFAULT_RANDOM_SEED;
	mass_maximum              = DEFAULT_MASS_MAXIMUM;

	if(fscanf(input, "%d", &number_of_particles) != 1) {
		fprintf(stderr, "ERROR: cannot read number of particles from standard input!\n");
		std::abort();
	}

	if(number_of_particles < 1) {
		fprintf(stderr, "ERROR: cannot have %d particles!\n", number_of_particles);
		std::abort();
	}

	if(number_of_particles == 1) {
		fprintf(stderr, "There is only one particle, therefore no forces.\n");
		std::abort();
	}
	//
	if(fscanf(input, "%d", &block_size) != 1) {
		fprintf(stderr, "ERROR: cannot read block size from standard input!\n");
		std::abort();
	}

	if(block_size <= 0) {
		fprintf(stderr, "ERROR: cannot have %d as block size!\n", block_size);
		std::abort();
	}
	
	if(number_of_particles % block_size != 0) {
		fprintf(stderr, "ERROR: block size must be divisable by number of particles!\n");
		std::abort();
	}

	if(fscanf(input, "%f", &domain_size_x) != 1) {
		fprintf(stderr, "ERROR: cannot read domain size X from standard input!\n");
		std::abort();
	}

	if(domain_size_x <= 0.0) {
		fprintf(stderr, "ERROR: cannot have a domain whose X dimension has length %f!\n", domain_size_x);
		std::abort();
	}

	if(fscanf(input, "%f", &domain_size_y) != 1) {
		fprintf(stderr, "ERROR: cannot read domain size Y from standard input!\n");
		std::abort();
	}

	if(domain_size_y <= 0.0) {
		fprintf(stderr, "ERROR: cannot have a domain whose Y dimension has length %f!\n", domain_size_y);
		std::abort();
	}

	if(fscanf(input,"%f", &domain_size_z) != 1) {
		fprintf(stderr, "ERROR: cannot read domain size Z from standard input!\n");
		std::abort();
	}

	if(domain_size_z <= 0.0) {
		fprintf(stderr, "ERROR: cannot have a domain whose Z dimension has length %f!\n", domain_size_z);
		std::abort();
	}

	if(fscanf(input, "%f", &time_interval) != 1) {
		fprintf(stderr, "ERROR: cannot read time interval from standard input!\n");
		std::abort();
	}

	if(time_interval <= 0.0) {
		fprintf(stderr, "ERROR: cannot have a time interval of %f!\n", time_interval);
		std::abort();
	}

	if(fscanf(input, "%d", &number_of_timesteps) != 1) {
		fprintf(stderr, "ERROR: cannot read number of timesteps from standard input!\n");
		std::abort();
	}

	if(number_of_timesteps <= 0) {
		fprintf(stderr, "ERROR: cannot have %d timesteps!\n", number_of_timesteps);
		std::abort();
	}

	if(fscanf(input, "%d", &timesteps_between_outputs) != 1) {
		fprintf(stderr, "ERROR: cannot read timesteps between outputs from standard input!\n");
		std::abort();
	}

	if(timesteps_between_outputs <= 0) {
		fprintf(stderr, "ERROR: cannot have %d timesteps between outputs!\n", timesteps_between_outputs);
		std::abort();
	}
	
	int aux_serial;
	if(fscanf(input, "%d", &aux_serial) != 1) {
		fprintf(stderr, "ERROR: cannot read serial from standard input!\n");
		std::abort();
	}

	if(aux_serial != 0 && aux_serial != 1) {
		fprintf(stderr, "ERROR: serial must be 0 (false) or 1 (true)!\n");
		std::abort();
	}
#ifdef VERBOSE
	execute_serial = (aux_serial == 0) ? false : true;
#endif

	if(fscanf(input, "%d", &random_seed) != 1) {
		fprintf(stderr, "ERROR: cannot read random seed from standard input!\n");
		std::abort();
	}

	if(fscanf(input, "%f", &mass_maximum) != 1) {
		fprintf(stderr, "ERROR: cannot read mass maximum from standard input!\n");
		std::abort();
	}

	if(mass_maximum <= 0.0) {
		fprintf(stderr, "ERROR: cannot have a maximum mass of %f!\n", mass_maximum);
		std::abort();
	}

	fgetc(input);
	fgets(base_filename, DEFAULT_STRING_LENGTH, input);
	if (base_filename[strlen(base_filename)-1] == '\n') {
		base_filename[strlen(base_filename)-1] = '\0';
	}
}

/*
 * Clear the particle's data.
 */
void Particle_clear(Particle* this_particle) {
#ifdef CHECK
	Particle_check(this_particle, "clear", "Particle_clear");
#endif

	this_particle->position_x = 0.0;
	this_particle->position_y = 0.0;
	this_particle->position_z = 0.0;
	this_particle->velocity_x = 0.0;
	this_particle->velocity_y = 0.0;
	this_particle->velocity_z = 0.0;
	this_particle->mass       = 0.0;
}

/*
 * Construct the particle.
 */
void Particle_construct(Particle* this_particle) { 
#ifdef CHECK
	Particle_check(this_particle, "construct", "Particle_construct");
#endif
	
	Particle_clear(this_particle);
}

/*
 * Destroy the particle.
 */
void Particle_destruct (Particle* this_particle) { 
#ifdef CHECK
	Particle_check(this_particle, "destruct", "Particle_destruct");
#endif

	Particle_clear(this_particle);
}

/*
 * Initialize the particle by setting its data randomly.
 */
void Particle_set_position_randomly(Particle* this_particle) { 
#ifdef CHECK
	Particle_check(this_particle, "randomly set the position", "Particle_set_randomly");
#endif

	this_particle->position_x = domain_size_x * (static_cast<float>(random()) / (static_cast<float>(RAND_MAX) + 1.0));
	this_particle->position_y = domain_size_y * (static_cast<float>(random()) / (static_cast<float>(RAND_MAX) + 1.0));
	this_particle->position_z = domain_size_z * (static_cast<float>(random()) / (static_cast<float>(RAND_MAX) + 1.0));
}

/*
 * Initialize the particle by setting its data randomly.
 */
void Particle_initialize_randomly(Particle* this_particle) { 
#ifdef CHECK
	Particle_check(this_particle, "randomly initialize", "Particle_initialize_randomly");
#endif

	Particle_clear(this_particle);
	Particle_set_position_randomly(this_particle);

	this_particle->mass = mass_maximum * (static_cast<float>(random()) / (static_cast<float>(RAND_MAX) + 1.0));

#ifdef CHECK_VAL
	printf("mass %g\n", this_particle->mass);
#endif
}

/*
 * Initialize the particle.
 */
void Particle_initialize(Particle* this_particle) { 
#ifdef CHECK
	Particle_check(this_particle, "initialize", "Particle_initialize");
#endif

	Particle_initialize_randomly(this_particle);
}

void Particle_output(FILE* fileptr, Particle* this_particle) { 
	fprintf(fileptr, "%g %g %g %g %g %g %g\n", 
			this_particle->position_x,
			this_particle->position_y,
			this_particle->position_z,
			this_particle->velocity_x,
			this_particle->velocity_y,
			this_particle->velocity_z,
			this_particle->mass);
}

void Particle_output_xyz (FILE* fileptr, Particle* this_particle) {
	fprintf(fileptr, "C %g %g %g\n", 
			this_particle->position_x, this_particle->position_y, this_particle->position_z);
}

/*
 * Allocate and return an array of particles.
 */
Particle* Particle_array_allocate(int number_of_particles) { 
	Particle* this_particle_array = nullptr;

#ifdef CHECK
	if (number_of_particles < 0) {
		fprintf(stderr, "ERROR: illegal number of particles %d to allocate\n", number_of_particles);
		fprintf(stderr, "  in Particle_array_construct\n");
		std::abort();
    }
#endif

	if(number_of_particles == 0) 
		 return nullptr;

	this_particle_array = new Particle[number_of_particles];
	if(this_particle_array == nullptr) {
		fprintf(stderr, "ERROR: can't allocate a particle array of %d particles\n", number_of_particles);
		fprintf(stderr, "  in Particle_array_construct\n");
		std::abort();
	}

	return this_particle_array;
}

/*
 * Construct and return an array of particles, cleared.
 */
Particle* Particle_array_construct(int number_of_particles) { 
	Particle* this_particle_array = nullptr;

	this_particle_array = Particle_array_allocate(number_of_particles);

	for(int index = 0; index < number_of_particles; index++) {
		Particle_construct(&(this_particle_array[index]));
	}

	return this_particle_array;
}

/*
 * Deallocate the array of particles, and return NULL.
 */
Particle* Particle_array_deallocate(Particle* this_particle_array, int number_of_particles) { 
#ifdef CHECK
    Particle_array_check(this_particle_array, number_of_particles, "deallocate", "Particle_array_deallocate");
#endif
	
	delete[] this_particle_array;
	this_particle_array = nullptr;
	
	return nullptr;
}

/*
 * Destroy the array of particles, and return NULL.
 */
Particle* Particle_array_destruct(Particle* this_particle_array, int number_of_particles) { 
#ifdef CHECK
	Particle_array_check(this_particle_array, number_of_particles, "destroy", "Particle_array_destruct");
#endif

	for(int index = number_of_particles - 1; index >= 0; index--) {
		Particle_destruct(&(this_particle_array[index]));
	}

	return Particle_array_deallocate(this_particle_array, number_of_particles);
}

/*
 * Initialize the array of particles by setting its data randomly.
 */
void Particle_array_initialize_randomly(Particle* this_particle_array, int number_of_particles) {
#ifdef CHECK
	Particle_array_check(this_particle_array, number_of_particles, 
                        "initialize randomly", "Particle_array_initialize_randomly");
#endif

	for(int index = 0; index < number_of_particles; index++) {
		Particle_initialize_randomly(&(this_particle_array[index]));
	}
}

/*
 * Initialize the array of particles.
 */
void Particle_array_initialize(Particle* this_particle_array, int number_of_particles) {
	Particle_array_initialize_randomly(this_particle_array, number_of_particles);
} 

/*
 * Particle_array_output 
 */
void Particle_array_output(char* base_filename, Particle* this_particle_array, int number_of_particles, int timestep) { 
	FILE* fileptr  = nullptr;
	char* filename = nullptr;
	int   filename_length;

#ifdef CHECK
	Particle_array_check(this_particle_array, number_of_particles, "output", "Particle_array_output");
#endif /* #ifdef CHECK */

	filename_length = strlen(base_filename) + 1 + 8 + 1 + 3;
	filename        = new char[filename_length + 1];
	
	if(filename == nullptr) {
		fprintf(stderr, "ERROR: can't allocate the filename string\n");
		fprintf(stderr, "  %s_%8.8d.txt\n", base_filename, timestep);
		fprintf(stderr, "  in Particle_array_output\n");

		std::abort();
	} /* if (filename == (char*)NULL) */

	sprintf(filename, "%s_%8.8d.txt", base_filename, timestep);
	
	fileptr = fopen(filename, "w");
	if(fileptr == nullptr) {
		fprintf(stderr, "ERROR: can't open the output file named\n");
		fprintf(stderr, "  %s\n", filename);
		fprintf(stderr, "  in Particle_array_output\n");

		std::abort();
	} /* if (fileptr == (FILE*)NULL) */

	fprintf(fileptr, "%d %d %d %g %g %g %g %g %d\n",
			number_of_particles, number_of_timesteps, timesteps_between_outputs,
			domain_size_x, domain_size_y, domain_size_z,
			mass_maximum, time_interval,
			random_seed);

	fprintf(fileptr, "%d\n", timestep);

	for(int index = 0; index < number_of_particles; index++) {
		Particle_output(fileptr, &(this_particle_array[index]));
	} /* for index */

	if(fclose(fileptr) != 0) {
		fprintf(stderr, "ERROR: can't close the output file named\n");
		fprintf(stderr, "  %s\n", filename);
		fprintf(stderr, "  in Particle_array_output\n");

		std::abort();
	} /* if (fclose(fileptr) != 0) */

	fileptr = nullptr;
	delete[] filename; 
	filename = nullptr;
} /* Particle_array_output */

/* Outputs particle positions in a format that VMD can easily visualize. */
void Particle_array_output_xyz(FILE * fileptr, Particle* this_particle_array, int number_of_particles) {
	fprintf(fileptr, "%d\nNBody\n", number_of_particles);
	
	for(int index = 0; index < number_of_particles; index++) {
		Particle_output_xyz(fileptr, &(this_particle_array[index]));
	}
}

#ifdef CHECK
/*
 * Check that the particle exists.
 */
void Particle_check (Particle* this_particle, char* action, char* routine) { 
	if (this_particle != (Particle*)NULL) 
		return;
	
	fprintf(stderr, "ERROR: can't %s a nonexistent particle\n", 
			((action == (char*)NULL) || (strlen(action) == 0)) ? "perform an unknown action on" : action);
	fprintf(stderr, "  in %s\n", 
			((routine == (char*)NULL) || (strlen(routine) == 0)) ? "an unknown routine" : routine);

	std::abort();
}

void Particle_array_check(Particle* this_particle_array, int number_of_particles,
						  char* action, char* routine) {
	if(number_of_particles < 0) {
		fprintf(stderr, "ERROR: illegal number of particles %d\n", number_of_particles);
		fprintf(stderr, "  to %s\n", 
				((action == (char*)NULL) || (strlen(action) == 0)) ? "perform an unknown action on" : action);
		fprintf(stderr, "  in %s\n",
				((routine == (char*)NULL) || (strlen(routine) == 0)) ? "an unknown routine" : routine);

		std::abort();
	}

	if(number_of_particles == 0) {
		if (this_particle_array == (Particle*)NULL) 
			return (Particle*)NULL;

		fprintf(stderr, "ERROR: can't %s\n",
              ((action == (char*)NULL) || (strlen(action) == 0)) ? "perform an unknown action on" : action);
		fprintf(stderr, "  an existing particle array of length 0\n");
		fprintf(stderr, "  in %s\n",
              ((routine == (char*)NULL) || (strlen(routine) == 0)) ? "an unknown routine" : routine);

		std::abort();
	}
   
	if(this_particle_array == (Particle*)NULL) {
		fprintf(stderr, "ERROR: can't %s\n",
              ((action == (char*)NULL) || (strlen(action) == 0)) ? "perform an unknown action on" : action);
		fprintf(stderr, "  a nonexistent array of %d particles\n", number_of_particles);
		fprintf(stderr, "  in %s\n", 
              ((routine == (char*)NULL) || (strlen(routine) == 0)) ? "an unknown routine" : routine);

		std::abort();
    }
} 
#endif /* #ifdef CHECK */

/* wall_time */
long wtime() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000 + t.tv_usec;
}
