#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

void copiar_string_segura(char* destino, const char* origem, int inicio, int fim) {
    int k = 0;
    for (int j = inicio; j < fim; j++) {
        if (origem[j] != '"' && k < (TAMANHO_NOME - 1)) {
            destino[k++] = origem[j];
        }
    }
    destino[k] = '\0';
}

void extrair_ano_de_string(char* data_texto, int* destino_ano) {
    if(!data_texto || strlen(data_texto) < 4) {
        *destino_ano = 0;
        return;
    }
    char temp[5];
    strncpy(temp, data_texto, 4);
    temp[4] = '\0';
    *destino_ano = atoi(temp);
}

int verifica_evento_feminino(char* nome_evento) {
    if (strstr(nome_evento, "Women") != NULL || strstr(nome_evento, "Ladies") != NULL) {
        return 1;
    }
    return 0;
}

int buscar_indice_atleta_por_id(Atleta* lista_atletas, int total_atletas, int id_alvo) {
    for(int i = 0; i < total_atletas; i++) {
        if(lista_atletas[i].id == id_alvo) return i;
    }
    return -1;
}

/*
 * ATENCAO: Atualizado para NocRegiao
 */
char* buscar_nome_pais_por_noc(NocRegiao* lista_regioes, int total_regioes, char* noc_alvo) {
    for(int i = 0; i < total_regioes; i++) {
        if(strcmp(lista_regioes[i].noc, noc_alvo) == 0) return lista_regioes[i].regiao;
    }
    return noc_alvo;
}

double buscar_populacao_por_ano(Populacao* lista_populacao, int total_populacao, char* codigo_pais, int ano_alvo) {
    for(int i = 0; i < total_populacao; i++) {
        if(strcmp(lista_populacao[i].codigo_pais, codigo_pais) == 0) {
            int indice = ano_alvo - 1960;
            if(indice >= 0 && indice < 63) {
                return lista_populacao[i].populacao_por_ano[indice];
            }
        }
    }
    return 0.0;
}
