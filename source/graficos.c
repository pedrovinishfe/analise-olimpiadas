#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

void gerar_grafico_barra(char* nome_arquivo, char** etiquetas, double* valores, int quantidade, char* titulo, char* label_eixo_y) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if(!arquivo) {
        printf("Erro ao criar arquivo grafico: %s\n", nome_arquivo);
        return;
    }

    int largura = 800;
    int altura = 400;

    fprintf(arquivo, "<svg width='%d' height='%d' xmlns='http://www.w3.org/2000/svg'>\n", largura, altura);
    fprintf(arquivo, "<rect width='100%%' height='100%%' fill='white'/>\n");
    fprintf(arquivo, "<text x='400' y='30' text-anchor='middle' font-size='20' font-family='Arial'>%s</text>\n", titulo);

    double valor_maximo = 0;
    for(int i = 0; i < quantidade; i++) {
        if(valores[i] > valor_maximo) valor_maximo = valores[i];
    }
    if(valor_maximo == 0) valor_maximo = 1;

    double largura_barra = 700.0 / quantidade;

    for(int i = 0; i < quantidade; i++) {
        double altura_barra = (valores[i] / valor_maximo) * 300.0;
        double pos_x = 50 + i * largura_barra;
        double pos_y = 350 - altura_barra;

        fprintf(arquivo, "<rect x='%.2f' y='%.2f' width='%.2f' height='%.2f' fill='#4682B4'/>\n",
                pos_x, pos_y, largura_barra - 10, altura_barra);

        fprintf(arquivo, "<text x='%.2f' y='370' text-anchor='middle' font-size='10' font-family='Arial'>%s</text>\n",
                pos_x + (largura_barra - 10)/2, etiquetas[i]);

        fprintf(arquivo, "<text x='%.2f' y='%.2f' text-anchor='middle' font-size='10' fill='black' font-family='Arial'>%.0f</text>\n",
                pos_x + (largura_barra - 10)/2, pos_y - 5, valores[i]);
    }

    fprintf(arquivo, "</svg>");
    fclose(arquivo);
    printf("   [GRAFICO] Salvo em: %s\n", nome_arquivo);
}

void gerar_grafico_linha(char* nome_arquivo, double* eixo_x, double* eixo_y, int quantidade, char* titulo, char* label_eixo_x, char* label_eixo_y) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if(!arquivo) return;

    fprintf(arquivo, "<svg width='800' height='400' xmlns='http://www.w3.org/2000/svg'>\n");
    fprintf(arquivo, "<rect width='100%%' height='100%%' fill='white'/>\n");
    fprintf(arquivo, "<text x='400' y='30' text-anchor='middle' font-size='20'>%s</text>\n", titulo);

    double min_x = eixo_x[0], max_x = eixo_x[0], max_y = 0;
    for(int i = 0; i < quantidade; i++) {
        if(eixo_x[i] < min_x) min_x = eixo_x[i];
        if(eixo_x[i] > max_x) max_x = eixo_x[i];
        if(eixo_y[i] > max_y) max_y = eixo_y[i];
    }
    if(max_y == 0) max_y = 1;

    fprintf(arquivo, "<polyline fill='none' stroke='red' stroke-width='2' points='");
    for(int i = 0; i < quantidade; i++) {
        double px = 50 + ((eixo_x[i] - min_x) / (max_x - min_x)) * 700;
        double py = 350 - (eixo_y[i] / max_y) * 300;
        fprintf(arquivo, "%.2f,%.2f ", px, py);
    }
    fprintf(arquivo, "'/>\n");

    fprintf(arquivo, "</svg>");
    fclose(arquivo);
    printf("   [GRAFICO] Salvo em: %s\n", nome_arquivo);
}
