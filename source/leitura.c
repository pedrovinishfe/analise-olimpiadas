#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

// Converte string pra float tratando o caso "NA"
float texto_para_float(char* texto) {
    if(!texto || !*texto || strcmp(texto, "NA")==0) return 0.0;
    return atof(texto);
}

// Lê a linha do atleta caractere por caractere pra lidar com aspas
void processar_linha_atleta(char* buffer, Atleta* atleta) {
    int coluna = 0;
    int inicio = 0;
    int aspas = 0;
    int tamanho = strlen(buffer);
    char temp[TAMANHO_NOME];

    for(int k = 0; k <= tamanho; k++) {
        if(buffer[k] == '"') aspas = !aspas;

        // Só considera vírgula se não estiver dentro de aspas
        if((buffer[k] == ',' && !aspas) || k == tamanho || buffer[k] == '\n') {
            copiar_string_segura(temp, buffer, inicio, k);

            switch(coluna) {
                case 0: atleta->id = atoi(temp); break;
                case 1: strcpy(atleta->nome, temp); break;
                case 2: extrair_ano_de_string(temp, &atleta->ano_nascimento); break;
                // CORREÇÃO: A sigla NOC (ex: BRA) está na coluna 5! (A 6 é o nome "Brazil")
                case 5: strcpy(atleta->noc, temp); break;
                case 7: atleta->altura = texto_para_float(temp); break;
                case 8: atleta->peso = texto_para_float(temp); break;
            }
            coluna++;
            inicio = k + 1;
        }
    }
}

void processar_linha_resultado(char* buffer, Resultado* resultado) {
    int coluna = 0; int inicio = 0; int aspas = 0; int tamanho = strlen(buffer); char temp[TAMANHO_NOME];

    for(int k = 0; k <= tamanho; k++) {
        if(buffer[k] == '"') aspas = !aspas;
        if((buffer[k] == ',' && !aspas) || k == tamanho || buffer[k] == '\n') {
            copiar_string_segura(temp, buffer, inicio, k);
            switch(coluna) {
                case 0: // Quebra "1992 Summer" em ano e estação
                    resultado->ano_olimpiada = atoi(temp);
                    if(strstr(temp, "Summer")) strcpy(resultado->estacao, "Summer");
                    else if(strstr(temp, "Winter")) strcpy(resultado->estacao, "Winter");
                    else strcpy(resultado->estacao, "Unknown");
                    break;
                case 1: strcpy(resultado->evento, temp); break;
                case 4: strcpy(resultado->medalha, temp); break;
                case 6: resultado->id_atleta = atoi(temp); break;
                case 7: strcpy(resultado->noc, temp); break;
                case 8: strcpy(resultado->esporte, temp); break;
            }
            coluna++; inicio = k + 1;
        }
    }
}

void processar_linha_noc(char* buffer, NocRegiao* regiao) {
    int coluna=0, inicio=0, aspas=0, len=strlen(buffer); char temp[TAMANHO_NOME];
    for(int k=0; k<=len; k++) {
        if(buffer[k]=='"') aspas=!aspas;
        if((buffer[k]==',' && !aspas) || k==len || buffer[k]=='\n' || buffer[k]=='\r') {
            copiar_string_segura(temp, buffer, inicio, k);
            if(coluna==0) strcpy(regiao->noc, temp);
            if(coluna==1) strcpy(regiao->regiao, temp);
            coluna++; inicio=k+1;
        }
    }
}

void processar_linha_populacao(char* buffer, Populacao* pop) {
    int coluna=0, inicio=0, aspas=0, len=strlen(buffer); char temp[TAMANHO_NOME];
    for(int k=0; k<=len; k++) {
        if(buffer[k]=='"') aspas=!aspas;
        if((buffer[k]==',' && !aspas) || k==len || buffer[k]=='\n') {
            copiar_string_segura(temp, buffer, inicio, k);
            if(coluna==1) strcpy(pop->codigo_pais, temp);
            if(coluna>=4 && coluna<67) pop->populacao_por_ano[coluna-4] = atof(temp);
            coluna++; inicio=k+1;
        }
    }
}

// Função genérica pra ler qualquer arquivo CSV e não repetir código
void* ler_arquivo_generico(const char* arquivo, int* quantidade, int tamanho_struct, void(*funcao_processamento)(char*,void*)) {
    FILE* ponteiro_arquivo = fopen(arquivo, "r");
    if(!ponteiro_arquivo) return NULL;

    int total_linhas = 0;
    char buffer[TAMANHO_LINHA_CSV];

    // Conta quantas linhas tem pra alocar a memória certinha
    fgets(buffer, TAMANHO_LINHA_CSV, ponteiro_arquivo);
    while(fgets(buffer, TAMANHO_LINHA_CSV, ponteiro_arquivo)) total_linhas++;

    void* vetor_dados = malloc(total_linhas * tamanho_struct);
    if (!vetor_dados) { fclose(ponteiro_arquivo); return NULL; }

    rewind(ponteiro_arquivo);
    fgets(buffer, TAMANHO_LINHA_CSV, ponteiro_arquivo);

    int i = 0;
    while(fgets(buffer, TAMANHO_LINHA_CSV, ponteiro_arquivo)) {
        funcao_processamento(buffer, (char*)vetor_dados + (i * tamanho_struct));
        i++;
    }

    fclose(ponteiro_arquivo);
    *quantidade = i;
    return vetor_dados;
}

// Funções de atalho pra chamar a genérica
Atleta* ler_arquivo_bios(const char* a, int* n) { return (Atleta*)ler_arquivo_generico(a, n, sizeof(Atleta), (void(*)(char*,void*))processar_linha_atleta); }
Resultado* ler_arquivo_resultados(const char* a, int* n) { return (Resultado*)ler_arquivo_generico(a, n, sizeof(Resultado), (void(*)(char*,void*))processar_linha_resultado); }
NocRegiao* ler_arquivo_noc(const char* a, int* n) { return (NocRegiao*)ler_arquivo_generico(a, n, sizeof(NocRegiao), (void(*)(char*,void*))processar_linha_noc); }
Populacao* ler_arquivo_populacao(const char* a, int* n) { return (Populacao*)ler_arquivo_generico(a, n, sizeof(Populacao), (void(*)(char*,void*))processar_linha_populacao); }

void liberar_memoria(void* p) { if(p) free(p); }
