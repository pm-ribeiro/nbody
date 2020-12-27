Problema Nbody

Instruções para execução

-> Compilar os arquivos

No terminal
make -f Makefile

-> Execução do arquivo de entrada pequeno

./nbody_simulation input_files/nbody_input-16384_16384.in <numero_de_threads>

-> Execução do arquivo de entrada grande

./nbody_simulation input_files/nbody_input-32768_32768.in <numero_de_threads>

O algoritmo será executado 30 vezes, ao final será criado um arquivo .txt
contendo os tempos de execução (em segundos) de cada iteração do algoritmo.

Exemplo de nome do arquivo: resultados_nbody_10_threads.txt
