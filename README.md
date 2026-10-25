# Trabalho 01 - Sistemas Operacionais (CEFET-MG)

Este repositório contém a implementação do Trabalho Prático 1 da disciplina de Sistemas Operacionais.

## Objetivo

O trabalho é dividido em duas partes:

### Parte 1 - Cálculo de π com threads POSIX
Calcula uma aproximação de π usando a série de Leibniz–Gregory com pelo menos 1 bilhão de termos, distribuídos entre N threads. 
- Cada thread calcula uma soma parcial e grava seu resultado em `result[i]` (sem sincronização)
- Após todas as threads finalizarem, o programa principal soma os resultados parciais
- Foram realizadas medições com 1, 2, 4, 8, 16 e 32 threads em diferentes máquinas, com pelo menos 5 repetições cada
- Arquivos: `parte1/pi_calc.c`, `parte1/pi_calc` (binário), `parte1/graficos.py`, e gráficos gerados

### Parte 2 - Filas de mensagens POSIX
Sistema com dois processos independentes (A e B) comunicando-se via filas de mensagens POSIX:
- Processo A sorteia dois números aleatórios `n1`, `n2`, envia para B e recebe a soma `s`, imprimindo `n1 + n2 = s`
- Processo B recebe os números, calcula a soma, aguarda 1 segundo e devolve para A
- Foram testados cenários com diferentes quantidades de processos A e B
- Arquivos: `parte2/processo_a.c`, `parte2/processo_a`, `parte2/processo_b.c`, `parte2/processo_b`

## Como compilar

### Parte 1
```bash
cd parte1
gcc -o pi_calc pi_calc.c -lm -lpthread
```

### Parte 2
```bash
cd parte2
gcc -o processo_a processo_a.c -lrt
gcc -o processo_b processo_b.c -lrt
```
*(`-lrt` para filas de mensagens POSIX em algumas distribuições Linux)*

## Como executar

### Parte 1
```bash
./pi_calc
```
O programa já executa os testes com 1, 2, 4, 8, 16 e 32 threads (5 iterações cada, com 10^9 termos) e exibe uma tabela com média, desvio-padrão e coeficiente de variação.

Para medir o tempo conforme o roteiro, também pode usar:
```bash
time ./pi_calc
```

### Parte 2
Em dois terminais diferentes (ou em background):
```bash
./processo_b  # deve ser iniciado primeiro ou estar aguardando
./processo_a
```
Os processos utilizam nomes de fila POSIX personalizados (verificar no código). É possível inspecionar filas com:
```bash
ls /dev/mqueue/
```

## Observações
- Compile com `-lm -lpthread` (parte 1) e bibliotecas necessárias para POSIX mqueue (parte 2)
- Em experimentos de tempo, evitar carga excessiva na máquina (`top`, `htop`)
- Para dados estatísticos, recomenda-se >= 5 execuções por configuração
- O relatório completo (até 7 páginas) deve conter tabelas, gráficos, análises e respostas às questões do enunciado
