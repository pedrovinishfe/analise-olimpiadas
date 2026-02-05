#include <stdio.h>
#include <stdlib.h>
#include "../include/olimpiadas.h"

int main() {
    int total_atletas=0, total_resultados=0, total_regioes=0, total_populacao=0;

    printf("--- SISTEMA OLIMPICO UFS ---\n");
    printf("Carregando bases de dados...\n");

    Atleta* atletas = ler_arquivo_bios("bios.csv", &total_atletas);
    Resultado* resultados = ler_arquivo_resultados("results.csv", &total_resultados);
    NocRegiao* regioes = ler_arquivo_noc("noc_regions.csv", &total_regioes);
    Populacao* populacao = ler_arquivo_populacao("populations.csv", &total_populacao);

    if(!atletas || !resultados || !regioes) {
        printf("[ERRO] Falha ao carregar CSVs. Verifique se estao na mesma pasta.\n");
        return 1;
    }
    printf("[SUCESSO] Dados carregados.\n");

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf(" 1. Comparar Verao/Inverno (10 Paises)\n 2. Genero (Pais)\n 3. Medalhas vs Pop (Pais/Ano)\n");
        printf(" 4-6. Medias (Idade/Peso/Altura)\n 7. Atletas Verao+Inverno\n 8. Ranking Pais/Esporte\n");
        printf(" 9. Evolucao Historica (2004-2022)\n 10. Eficiencia (10 Paises)\n 11. Prim Medalha (Pais)\n");
        printf(" 12. Ranking 10 Paises (Medalhas)\n 13. Top 10 Atletas\n 14. Ranking 10 Paises (Participacao)\n");
        printf(" 15. Top 10 Edicoes\n 16. Ranking 10 Esportes\n 17. Evolucao Mulheres\n");
        printf(" 18-20. Rankings Idade/Part\n 21. Peso Ouro\n 22. Medalhistas Unicos\n 0. Sair\n>> ");
        scanf("%d", &opcao);

        switch(opcao) {
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
            default: printf("Opcao invalida.\n");
        }
    } while(opcao!=0);

    liberar_memoria(atletas); liberar_memoria(resultados); liberar_memoria(regioes); liberar_memoria(populacao);
    return 0;
}
