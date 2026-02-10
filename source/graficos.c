#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

// Gera um arquivo SVG contendo um grafico de barras
void gerar_grafico_barra(char* nome_arquivo, char** array_rotulos, double* array_valores, int total_elementos, char* titulo_grafico, char* label_eixo_y) {
    FILE* ponteiro_arquivo = fopen(nome_arquivo, "w");
    if(!ponteiro_arquivo) {
        printf("Erro ao criar arquivo grafico: %s\n", nome_arquivo);
        return;
    }

    int largura_imagem = 800;
    int altura_imagem = 400;

    // Escreve o cabecalho padrao do SVG
    fprintf(ponteiro_arquivo, "<svg width='%d' height='%d' xmlns='http://www.w3.org/2000/svg'>\n", largura_imagem, altura_imagem);
    fprintf(ponteiro_arquivo, "<rect width='100%%' height='100%%' fill='white'/>\n");
    
    // Escreve o titulo centralizado
    fprintf(ponteiro_arquivo, "<text x='400' y='30' text-anchor='middle' font-size='20' font-family='Arial'>%s</text>\n", titulo_grafico);

    // Encontra o valor maximo para escalar as barras proporcionalmente
    double maior_valor = 0;
    for(int i = 0; i < total_elementos; i++) {
        if(array_valores[i] > maior_valor) maior_valor = array_valores[i];
    }
    if(maior_valor == 0) maior_valor = 1; // Evita divisao por zero

    double largura_da_barra = 700.0 / total_elementos;

    // Loop para desenhar cada barra e seu rotulo
    for(int i = 0; i < total_elementos; i++) {
        // Regra de tres para altura da barra (Maximo = 300px)
        double altura_retangulo = (array_valores[i] / maior_valor) * 300.0;
        
        double posicao_x = 50 + i * largura_da_barra;
        double posicao_y = 350 - altura_retangulo;

        // Desenha o retangulo azul
        fprintf(ponteiro_arquivo, "<rect x='%.2f' y='%.2f' width='%.2f' height='%.2f' fill='#4682B4'/>\n",
                posicao_x, posicao_y, largura_da_barra - 10, altura_retangulo);

        // Escreve o rotulo (nome do pais/atleta) abaixo da barra
        fprintf(ponteiro_arquivo, "<text x='%.2f' y='370' text-anchor='middle' font-size='10' font-family='Arial'>%s</text>\n",
                posicao_x + (largura_da_barra - 10)/2, array_rotulos[i]);

        // Escreve o valor numerico acima da barra
        fprintf(ponteiro_arquivo, "<text x='%.2f' y='%.2f' text-anchor='middle' font-size='10' fill='black' font-family='Arial'>%.0f</text>\n",
                posicao_x + (largura_da_barra - 10)/2, posicao_y - 5, array_valores[i]);
    }

    fprintf(ponteiro_arquivo, "</svg>");
    fclose(ponteiro_arquivo);
    printf("   [GRAFICO] Salvo em: %s\n", nome_arquivo);
}

// Gera um arquivo SVG contendo um grafico de linha (polyline)
void gerar_grafico_linha(char* nome_arquivo, double* dados_eixo_x, double* dados_eixo_y, int total_pontos, char* titulo_grafico, char* label_eixo_x, char* label_eixo_y) {
    FILE* ponteiro_arquivo = fopen(nome_arquivo, "w");
    if(!ponteiro_arquivo) return;

    // Configura area do SVG
    fprintf(ponteiro_arquivo, "<svg width='800' height='400' xmlns='http://www.w3.org/2000/svg'>\n");
    fprintf(ponteiro_arquivo, "<rect width='100%%' height='100%%' fill='white'/>\n");
    fprintf(ponteiro_arquivo, "<text x='400' y='30' text-anchor='middle' font-size='20'>%s</text>\n", titulo_grafico);

    // Determina os limites (minimo e maximo) para normalizacao dos eixos
    double min_x = dados_eixo_x[0], max_x = dados_eixo_x[0], max_y = 0;
    
    for(int i = 0; i < total_pontos; i++) {
        if(dados_eixo_x[i] < min_x) min_x = dados_eixo_x[i];
        if(dados_eixo_x[i] > max_x) max_x = dados_eixo_x[i];
        if(dados_eixo_y[i] > max_y) max_y = dados_eixo_y[i];
    }
    
    if(max_y == 0) max_y = 1;
    double intervalo_x = max_x - min_x;
    if(intervalo_x == 0) intervalo_x = 1; // Evita divisao por zero se houver so 1 ponto

    // Inicia a linha (polyline)
    fprintf(ponteiro_arquivo, "<polyline fill='none' stroke='red' stroke-width='2' points='");
    
    for(int i = 0; i < total_pontos; i++) {
        // Calcula posicao relativa (0 a 1) e expande para o tamanho do grafico
        double coord_x = 50 + ((dados_eixo_x[i] - min_x) / intervalo_x) * 700;
        double coord_y = 350 - (dados_eixo_y[i] / max_y) * 300;
        
        fprintf(ponteiro_arquivo, "%.2f,%.2f ", coord_x, coord_y);
    }
    fprintf(ponteiro_arquivo, "'/>\n");

    fprintf(ponteiro_arquivo, "</svg>");
    fclose(ponteiro_arquivo);
    printf("   [GRAFICO] Salvo em: %s\n", nome_arquivo);
}
