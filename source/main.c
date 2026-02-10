#include <stdio.h>
#include <stdlib.h>
#include "../include/olimpiadas.h"

int main() {
    // Variaveis para armazenar o tamanho de cada vetor de dados
    int total_atletas=0, total_resultados=0, total_regioes=0, total_populacao=0;

    // Carregamento dos arquivos CSV para a memoria
    // Certifique-se que os arquivos estao na pasta correta ao executar
    Atleta* atletas = ler_arquivo_bios("bios.csv", &total_atletas);
    Resultado* resultados = ler_arquivo_resultados("results.csv", &total_resultados);
    NocRegiao* regioes = ler_arquivo_noc("noc_regions.csv", &total_regioes);
    Populacao* populacao = ler_arquivo_populacao("populations.csv", &total_populacao);

    // Verificacao basica se os arquivos principais foram lidos
    if(!atletas || !resultados || !regioes) {
        printf("Falha ao carregar CSVs. Verifique se estao na mesma pasta do executavel.\n");
        return 1;
    }

    int opcao_usuario;
    do {
        // Exibicao do Menu Principal
        printf(" 1. Comparar Verao/Inverno (10 Paises)\n");
        printf(" 2. Genero dos Atletas (Pais Especifico)\n");
        printf(" 3. Medalhas vs Populacao (Pais/Ano)\n");
        printf(" 4. Media de Idade (Medalhistas/Ano)\n");
        printf(" 5. Media de Peso (Medalhistas/Ano)\n");
        printf(" 6. Media de Altura (Medalhistas/Ano)\n");
        printf(" 7. Atletas Multiesportivos (Verao + Inverno)\n");
        printf(" 8. Ranking de Paises em um Esporte\n");
        printf(" 9. Evolucao Numero de Atletas (2004-2022)\n");
        printf(" 10. Eficiencia de Medalhas (10 Paises)\n");
        printf(" 11. Primeira Medalha da Historia (Pais)\n");
        printf(" 12. Ranking Total de Medalhas (10 Paises)\n");
        printf(" 13. Top 10 Atletas (Maiores Medalhistas)\n");
        printf(" 14. Ranking Participacao (10 Paises)\n");
        printf(" 15. Ranking Medalhas (Ultimas Edicoes)\n");
        printf(" 16. Ranking Atletas por Esporte (Ano)\n");
        printf(" 17. Evolucao Participacao Feminina\n");
        printf(" 18. Ranking Atletas Mais Velhos\n");
        printf(" 19. Ranking Medalhistas Mais Velhos\n");
        printf(" 20. Atletas com Mais Participacoes\n");
        printf(" 21. Peso Medio Medalhistas de Ouro\n");
        printf(" 22. Ranking Paises (Medalhistas Unicos)\n");
        printf(" 0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao_usuario);

        // Direcionamento para a funcao correspondente
        switch(opcao_usuario) {
            case 1: q1_total_medalhas_por_estacao(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            case 2: q2_genero_atletas_por_pais(atletas, total_atletas, resultados, total_resultados); break;
            case 3: q3_medalhas_vs_populacao(atletas, total_atletas, resultados, total_resultados, populacao, total_populacao); break;
            case 4: q4_media_idade_medalhistas(atletas, total_atletas, resultados, total_resultados); break;
            case 5: q5_media_peso_medalhistas(atletas, total_atletas, resultados, total_resultados); break;
            case 6: q6_media_altura_medalhistas(atletas, total_atletas, resultados, total_resultados); break;
            case 7: q7_atletas_verao_e_inverno(atletas, total_atletas, resultados, total_resultados); break;
            case 8: q8_ranking_pais_por_esporte(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            case 9: q9_evolucao_numero_atletas(atletas, total_atletas, resultados, total_resultados); break;
            case 10: q10_eficiencia_pais_medalhas(atletas, total_atletas, resultados, total_resultados); break;
            case 11: q11_primeira_medalha_pais(atletas, total_atletas, resultados, total_resultados); break;
            case 12: q12_ranking_paises_total_medalhas(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            case 13: q13_top_atletas_maiores_medalhistas(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            case 14: q14_ranking_paises_participacao(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            case 15: q15_ranking_ultimas_olimpiadas(resultados, total_resultados); break;
            case 16: q16_atletas_por_esporte_ano(resultados, total_resultados); break;
            case 17: q17_evolucao_participacao_feminina(atletas, total_atletas, resultados, total_resultados); break;
            case 18: q18_ranking_atletas_mais_velhos(atletas, total_atletas, resultados, total_resultados); break;
            case 19: q19_ranking_medalhistas_mais_velhos(atletas, total_atletas, resultados, total_resultados); break;
            case 20: q20_atletas_com_mais_participacoes(atletas, total_atletas, resultados, total_resultados); break;
            case 21: q21_peso_medio_medalhistas_ouro(atletas, total_atletas, resultados, total_resultados); break;
            case 22: q22_ranking_paises_medalhistas_unicos(atletas, total_atletas, resultados, total_resultados, regioes, total_regioes); break;
            
            case 0: break;
            default: printf("Opcao invalida. Tente novamente.\n");
        }
    } while(opcao_usuario != 0);

    // Liberacao de memoria alocada dinamicamente
    liberar_memoria(atletas); 
    liberar_memoria(resultados); 
    liberar_memoria(regioes); 
    liberar_memoria(populacao);
    
    return 0;
}
