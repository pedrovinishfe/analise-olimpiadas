#ifndef OLIMPIADAS_H
#define OLIMPIADAS_H

#include <stdio.h>

/*
 * CONFIGURAÇÕES GLOBAIS
 * TAMANHO_LINHA_CSV: 4096 para garantir que linhas longas no CSV não sejam cortadas.
 * MAXIMO_ATLETAS: Usando ID do atleta como índice direto no vetor para ter
 * acesso , em vez de ficar procurando o ID um por um
 */
#define TAMANHO_LINHA_CSV 4096
#define TAMANHO_NOME 300
#define TAMANHO_PEQUENO 100 
#define MAXIMO_ATLETAS 1000000 

// Structs: 

// Guarda os dados de cada atleta
typedef struct {
    int id;                     
    char nome[TAMANHO_NOME];    
    int ano_nascimento;         
    float altura;              
    float peso;             
    char noc[TAMANHO_PEQUENO];  
} Atleta;

// Guarda cada resultado/participação 
// Separamos ano e estação para facilitar as contas depois.
typedef struct {
    char medalha[TAMANHO_PEQUENO];  // tipo da medalha
    int id_atleta;                  
    int ano_olimpiada;              
    char estacao[20];               
    char evento[150];               
    char esporte[100];              
    char noc[TAMANHO_PEQUENO];      
} Resultado;

// Tabela pra converter o noc no nome do País
typedef struct {
    char noc[TAMANHO_PEQUENO];      
    char regiao[TAMANHO_PEQUENO];      
} NocRegiao;

// Dados de população para a questão per capita
typedef struct {
    char codigo_pais[TAMANHO_PEQUENO];      
    double populacao_por_ano[63]; 
} Populacao;

// Funções:

// Leitura dos arquivos CSV
Atleta* ler_arquivo_bios(const char* caminho_arquivo, int* total_registros);
Resultado* ler_arquivo_resultados(const char* caminho_arquivo, int* total_registros);
NocRegiao* ler_arquivo_noc(const char* caminho_arquivo, int* total_registros);
Populacao* ler_arquivo_populacao(const char* caminho_arquivo, int* total_registros);
void liberar_memoria(void* ponteiro_dados);

// Funções de ajuda (busca, limpeza de texto)
int buscar_indice_atleta_por_id(Atleta* lista_atletas, int total_atletas, int id_alvo);
char* buscar_nome_pais_por_noc(NocRegiao* lista_regioes, int total_regioes, char* noc_alvo);
double buscar_populacao_por_ano(Populacao* lista_populacao, int total_populacao, char* codigo_pais, int ano_alvo);
int verifica_evento_feminino(char* nome_evento);
void copiar_string_segura(char* destino, const char* origem, int inicio, int fim);
void extrair_ano_de_string(char* data_texto, int* destino_ano);

// Gráficos:
void gerar_grafico_barra(char* nome_arquivo, char** etiquetas, double* valores, int quantidade, char* titulo, char* label_eixo_y);
void gerar_grafico_linha(char* nome_arquivo, double* eixo_x, double* eixo_y, int quantidade, char* titulo, char* label_eixo_x, char* label_eixo_y);

// Lógica das questões:
void q1_total_medalhas_por_estacao(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);
void q2_genero_atletas_por_pais(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q3_medalhas_vs_populacao(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, Populacao* populacoes, int total_populacoes);
void q4_media_idade_medalhistas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q5_media_peso_medalhistas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q6_media_altura_medalhistas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q7_atletas_verao_e_inverno(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q8_ranking_pais_por_esporte(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);
void q9_evolucao_numero_atletas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q10_eficiencia_pais_medalhas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q11_primeira_medalha_pais(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q12_ranking_paises_total_medalhas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);
void q13_top_atletas_maiores_medalhistas(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);
void q14_ranking_paises_participacao(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);
void q15_ranking_ultimas_olimpiadas(Resultado* resultados, int total_resultados);
void q16_atletas_por_esporte_ano(Resultado* resultados, int total_resultados);
void q17_evolucao_participacao_feminina(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q18_ranking_atletas_mais_velhos(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q19_ranking_medalhistas_mais_velhos(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q20_atletas_com_mais_participacoes(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q21_peso_medio_medalhistas_ouro(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados);
void q22_ranking_paises_medalhistas_unicos(Atleta* atletas, int total_atletas, Resultado* resultados, int total_resultados, NocRegiao* regioes, int total_regioes);

#endif
