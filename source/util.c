#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

// Copia um trecho de uma string para outra, removendo aspas e respeitando o tamanho maximo
void copiar_string_segura(char* destino, const char* origem, int inicio, int fim) {
    int indice_destino = 0;
    
    // Percorre o intervalo definido na string de origem
    for (int indice_origem = inicio; indice_origem < fim; indice_origem++) {
        
        // Copia apenas se nao for aspas e se houver espaco no buffer
        if (origem[indice_origem] != '"' && indice_destino < (TAMANHO_NOME - 1)) {
            destino[indice_destino++] = origem[indice_origem];
        }
    }
    destino[indice_destino] = '\0'; // Finaliza a string
}

// Pega os 4 primeiros caracteres de uma string e converte para inteiro (Ano)
void extrair_ano_de_string(char* data_texto, int* destino_ano) {
    // Validacao basica
    if(!data_texto || strlen(data_texto) < 4) {
        *destino_ano = 0;
        return;
    }
    
    char buffer_ano[5];
    strncpy(buffer_ano, data_texto, 4); // Copia os 4 digitos
    buffer_ano[4] = '\0';
    
    *destino_ano = atoi(buffer_ano); // Converte para int
}

// Verifica palavras-chave para determinar se o evento e feminino
int verifica_evento_feminino(char* nome_evento) {
    // Busca por "Women" ou "Ladies" dentro do nome do evento
    if (strstr(nome_evento, "Women") != NULL || strstr(nome_evento, "Ladies") != NULL) {
        return 1; // Verdadeiro
    }
    return 0; // Falso
}

// Busca linear simples para encontrar o indice de um atleta pelo ID
int buscar_indice_atleta_por_id(Atleta* lista_atletas, int total_atletas, int id_alvo) {
    for(int indice = 0; indice < total_atletas; indice++) {
        if(lista_atletas[indice].id == id_alvo) return indice;
    }
    return -1; // Nao encontrado
}

/*
 * Busca o nome completo da regiao/pais baseado na sigla NOC (Ex: BRA -> Brazil)
 */
char* buscar_nome_pais_por_noc(NocRegiao* lista_regioes, int total_regioes, char* noc_alvo) {
    for(int indice = 0; indice < total_regioes; indice++) {
        if(strcmp(lista_regioes[indice].noc, noc_alvo) == 0) return lista_regioes[indice].regiao;
    }
    return noc_alvo; // Se nao achar, retorna a propria sigla
}

// Retorna a populacao de um pais em um determinado ano (baseado no offset de 1960)
double buscar_populacao_por_ano(Populacao* lista_populacao, int total_populacao, char* codigo_pais, int ano_alvo) {
    
    // Busca o pais correto na lista
    for(int indice_pais = 0; indice_pais < total_populacao; indice_pais++) {
        
        if(strcmp(lista_populacao[indice_pais].codigo_pais, codigo_pais) == 0) {
            
            // Calcula o indice do ano no array (Base do Banco Mundial costuma ser 1960)
            int indice_ano = ano_alvo - 1960;
            
            // Verifica limites do array de populacao (assumindo tamanho ~63 anos)
            if(indice_ano >= 0 && indice_ano < 63) {
                return lista_populacao[indice_pais].populacao_por_ano[indice_ano];
            }
        }
    }
    return 0.0; // Retorna 0 se pais ou ano nao forem encontrados
}
