#ifndef OLIMPIADAS_H
#define OLIMPIADAS_H

#include <stdio.h>

// Definição de constantes para facilitar a manutenção e leitura de buffers
#define MAX_LINHA 2048
#define MAX_NOME 150

// Estrutura do atleta.
// Dia, mês e ano separados para cálculos de idade
typedef struct {
    int id;
    char nome[MAX_NOME];
    char sexo;           
    int dia_nasc; 
    int mes_nasc;
    int ano_nasc;
    float altura;
    float peso;
    char time[100];
    char noc[5]; // Código do País 
} Atleta;

// Estrutura para os Resultados/Medalhas.
// (atleta_id) para permitir o cruzamento de dados.
typedef struct {
    char medalha[15];
    int atleta_id;    
    int ano_jogos;    
    char estacao[10]; // Verão ou Inverno
} Resultado;

// Estrutura auxiliar para agregação de dados (Group By).
// Usada para gerar relatórios estatísticos por país e ano.
typedef struct {
    char pais[5];
    int ano;
    int qtd_mulheres;
} EstatisticaPais;

// --- Assinaturas das Funções ---

// Funções de Leitura e Memória
Atleta* ler_bios(const char* nome_arquivo, int* qtd);
Resultado* ler_resultados(const char* nome_arquivo, int* qtd);
void liberar_memoria(void* dados);

// Funções Auxiliares e de Busca
int buscar_indice_atleta_por_id(Atleta* atletas, int qtd_atletas, int id_alvo);
void exportar_grafico_csv(const char* nome_arquivo, char* header, EstatisticaPais* dados, int qtd);

// Funções de Lógica de Negócio (Questões)
void questao_4_idade_media_precisa(Atleta* atletas, int n_atl, Resultado* resultados, int n_res, int ano_alvo);
void questao_17_evolucao_mulheres_por_pais(Atleta* atletas, int n_atl, Resultado* resultados, int n_res);

#endif
