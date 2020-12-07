# Problema Nbody

Problema Nbody paralelo utilizando pthreads.


Instruções para execução

### Compilar os arquivos

```bash
# Compilação dos arquivos
$ make -f Makefile
```

### execução do arquivo de entrada pequeno

```bash
$ ./nbody_simulation input_files/nbody_input-16384_16384.in <numero_de_threads>
```

### execução do arquivo de entrada grande

```bash
$ ./nbody_simulation input_files/nbody_input-32768_32768.in <numero_de_threads>
```

O algoritmo será executado 30 vezes, ao final será criado um arquivo .txt
contendo os tempos de execução (em segundos) de cada iteração do algoritmo.

Exemplo de nome do arquivo: resultados_nbody_10_threads.txt
