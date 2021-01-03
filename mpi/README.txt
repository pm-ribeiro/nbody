# Problema Nbody versão MPI

Instruções para execução

### Compilar os arquivos

No terminal

```bash
$ Make -f Makefile
```

### Executar entrada pequena
```bash
$ mpirun -np <numero_de_processos> ./nbody_simulation input_files/nbody_input-16384_16384.in <numero_de_threads>
```

### Executar entrada grande

```bash
$ mpirun -np <numero_de_processos> ./nbody_simulation input_files/nbody_input-32768_32768.in <numero_de_threads>
```

O algoritmo será executado 30 vezes, ao final será criado um arquivo .txt
contendo os tempos de execução (em segundos) de cada iteração do algoritmo.

Exemplo de nome do arquivo: in_grande_resultados_nbody_4_threads.txt
