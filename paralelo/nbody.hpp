#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/time.h>

const double GRAVITATIONAL_CONSTANT         = 6.6726e-11; /* N(m/kg)2 */

const double DEFAULT_DOMAIN_SIZE_X          = 1.0e+18; /* m  */
const double DEFAULT_DOMAIN_SIZE_Y          = 1.0e+18; /* m  */
const double DEFAULT_DOMAIN_SIZE_Z          = 1.0e+18; /* m  */
const double DEFAULT_MASS_MAXIMUM           = 1.0e+18; /* kg */
const double DEFAULT_TIME_INTERVAL          = 1.0e+18; /* s  */
const int DEFAULT_NUMBER_OF_PARTICLES       = 1000;
const int DEFAULT_NUMBER_OF_TIMESTEPS       = 100;
const int DEFAULT_TIMESTEPS_BETWEEN_OUTPUTS = DEFAULT_NUMBER_OF_PARTICLES;
const bool DEFAULT_EXECUTE_SERIAL           = false;
const int DEFAULT_RANDOM_SEED               = 12345;

const int DEFAULT_STRING_LENGTH = 1023;            

const int PROGRAM_SUCCESS_CODE  =  0;

/*
 * Types
 */
struct Particle {
    float position_x; /* m   */
    float position_y; /* m   */
    float position_z; /* m   */
    float velocity_x; /* m/s */
    float velocity_y; /* m/s */
    float velocity_z; /* m/s */
    float mass;       /* kg  */
    float pad;
};

/*
 * Global variables - input
 */
extern char     base_filename[DEFAULT_STRING_LENGTH+1];

extern int      number_of_particles;
extern int      block_size;
extern float    domain_size_x;
extern float    domain_size_y;
extern float    domain_size_z;
extern float    time_interval;
extern int      number_of_timesteps;
extern int      timesteps_between_outputs;
extern bool     execute_serial;
extern unsigned random_seed;
extern float    mass_maximum;

/* 
 * Function Prototypes
 */
void Particle_input_arguments(FILE *input);

//Particle
void Particle_clear(Particle* this_particle);
void Particle_construct(Particle* this_particle);
void Particle_destruct(Particle* this_particle);
void Particle_set_position_randomly(Particle* this_particle);
void Particle_initialize_randomly(Particle* this_particle);
void Particle_initialize(Particle* this_particle);
void Particle_output(FILE* fileptr, Particle* this_particle);
void Particle_output_xyz(FILE* fileptr, Particle* this_particle);

//Particle array
Particle* Particle_array_allocate(int number_of_particles);
Particle* Particle_array_construct(int number_of_particles);
Particle* Particle_array_deallocate(Particle* this_particle_array, int number_of_particles);
Particle* Particle_array_destruct(Particle* this_particle_array, int number_of_particles);
void Particle_array_initialize_randomly(Particle* this_particle_array, int number_of_particles);
void Particle_array_initialize(Particle* this_particle_array, int number_of_particles);
void Particle_array_output(char* base_filename, Particle* this_particle_array, int number_of_particles, int timestep);
void Particle_array_output_xyz(FILE * fileptr, Particle* this_particle_array, int number_of_particles);

//Check
#ifdef CHECK
void Particle_check(Particle* this_particle, char* action, char* routine);
void Particle_array_check(Particle* this_particle_array, int number_of_particles,
						  char* action, char* routine);
#endif

/* wall_time */
long wtime();
