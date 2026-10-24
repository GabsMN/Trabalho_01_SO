import matplotlib.pyplot as plt
import re

# ==============================================================================
# 1. COLE AQUI AS SAÍDAS DO TERMINAL EXATAMENTE COMO O CÓDIGO EM C IMPRIMIU
# ==============================================================================

saida_computador_1 = """
Resultados dos Testes de Desempenho (1e+09 termos, 5 iteracoes)
--------------------------------------------------------------------------
Threads    | Media (s)    | Desvio Padrao   | CV (%)     | Status CV 
--------------------------------------------------------------------------
1          | 2.6505       | 0.0499          | 1.88       | OK        
2          | 1.3604       | 0.0232          | 1.71       | OK        
4          | 0.6953       | 0.0126          | 1.81       | OK        
8          | 0.7063       | 0.0056          | 0.79       | OK        
16         | 0.7288       | 0.0133          | 1.82       | OK        
32         | 0.7207       | 0.0063          | 0.88       | OK        
--------------------------------------------------------------------------
"""

# Substitua o texto abaixo pela saída real que o Matheus vai te enviar
saida_computador_2 = """
Resultados dos Testes de Desempenho (1e+09 termos, 5 iteracoes)
--------------------------------------------------------------------------
Threads    | Media (s)    | Desvio Padrao   | CV (%)     | Status CV 
--------------------------------------------------------------------------
1          | 1.7652       | 0.0064          | 0.36       | OK        
2          | 0.8927       | 0.0111          | 1.24       | OK        
4          | 0.4570       | 0.0086          | 1.88       | OK        
8          | 0.3562       | 0.0066          | 1.84       | OK        
16         | 0.2331       | 0.0080          | 3.45       | OK        
32         | 0.2512       | 0.0112          | 4.45       | OK        
--------------------------------------------------------------------------
"""

# ==============================================================================
# 2. FUNÇÃO PARA EXTRAIR OS DADOS DA TABELA
# ==============================================================================
def extrair_dados(texto):
    threads = []
    medias = []
    # Busca linhas que comecem com números seguidos de | e mais números
    padrao = r"^\s*(\d+)\s*\|\s*([\d\.]+)\s*\|"
    for linha in texto.strip().split('\n'):
        match = re.search(padrao, linha)
        if match:
            threads.append(int(match.group(1)))
            medias.append(float(match.group(2)))
    return threads, medias

t1, m1 = extrair_dados(saida_computador_1)
t2, m2 = extrair_dados(saida_computador_2)

# ==============================================================================
# 3. GERAÇÃO DOS GRÁFICOS
# ==============================================================================

# Gráfico 1: Computador 1
plt.figure(figsize=(8, 5))
plt.plot(t1, m1, marker='o', linestyle='-', color='blue')
plt.title('Computador 1 (Mono-processado) - Tempo Médio de Execução')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo Médio (s)')
plt.xticks(t1)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('grafico1_comp1.png', bbox_inches='tight')
plt.show()

# Gráfico 2: Computador 2
plt.figure(figsize=(8, 5))
plt.plot(t2, m2, marker='s', linestyle='-', color='red')
plt.title('Computador 2 (Multi-processado) - Tempo Médio de Execução')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo Médio (s)')
plt.xticks(t2)
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('grafico2_comp2.png', bbox_inches='tight')
plt.show()

# Gráfico 3: Consolidado e Normalizado (Tn / T1)
norm_m1 = [val / m1[0] for val in m1]
norm_m2 = [val / m2[0] for val in m2]

plt.figure(figsize=(8, 5))
plt.plot(t1, norm_m1, marker='o', linestyle='-', color='blue', label='Comp 1 (AMD Ryzen 5)')
plt.plot(t2, norm_m2, marker='s', linestyle='-', color='red', label='Comp 2 (Multi-Core)')
plt.title('Gráfico Consolidado - Curvas Normalizadas (Tn / T1)')
plt.xlabel('Número de Threads')
plt.ylabel('Proporção de Desempenho (Tn / T1)')
plt.xticks(t1)
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig('grafico3_normalizado.png', bbox_inches='tight')
plt.show()