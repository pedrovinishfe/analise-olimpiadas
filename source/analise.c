#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

// Estruturas auxiliares
typedef struct { char chave[100]; int valor; } ParChaveValor;
int comparar_decrescente(const void* a, const void* b) { return ((ParChaveValor*)b)->valor - ((ParChaveValor*)a)->valor; }

typedef struct { int id_atleta; int idade; } ParIdade;
int comparar_idade_decrescente(const void* a, const void* b) { return ((ParIdade*)b)->idade - ((ParIdade*)a)->idade; }

// --- Implementação das questões com variáveis mnemônicas ---

// Q1:
void q1_total_medalhas_por_estacao(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, NocRegiao* regioes, int n_regioes) {
    char siglas_paises[10][10];
    int contagem_medalhas[10][2]; // [0]=Verao, [1]=Inverno

    // Inicializa a matriz de contagem
    for(int i=0; i<10; i++){ 
      contagem_medalhas[i][0]=0; 
      contagem_medalhas[i][1]=0;
    }

    printf("\nQ1: Comparativo 10 paises \n");
    printf("Digite as siglas de 10 paises (ex: BRA USA CHN):\n");
    
    // Input dos 10 paises
    for(int i=0; i<10; i++) { 
      printf("Pais %d: ", i+1); 
      scanf("%s", siglas_paises[i]); 
    }

    // Loop percorrendo todos os resultados
    for(int i=0; i<n_resultados; i++) {
       // Checa se houve medalha (ignora NA e vazios)
       if(strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0) {
            
            // Verifica se pertence a um dos 10 paises escolhidos
            for(int indice_pais=0; indice_pais<10; indice_pais++) {
                if(strcmp(resultados[i].noc, siglas_paises[indice_pais])==0) {
                    // Define indice (0=Summer, 1=Winter) e incrementa
                    int indice_estacao = (strcmp(resultados[i].estacao, "Summer")==0) ? 0 : 1;
                    contagem_medalhas[indice_pais][indice_estacao]++;
                }
            }
        }
    }

    // Exibicao da tabela e preparacao para grafico
    printf("\n| %-25s | %-8s | %-8s |\n", "REGIAO", "VERAO", "INVERNO");
    char* labels_grafico[10]; double valores_grafico[10];
    
    for(int i=0; i<10; i++) {
        char* nome_pais = buscar_nome_pais_por_noc(regioes, n_regioes, siglas_paises[i]);
        char nome_exibicao[26]; strncpy(nome_exibicao, nome_pais, 25); nome_exibicao[25]='\0';
        
        printf("| %-25s | %-8d | %-8d |\n", nome_exibicao, contagem_medalhas[i][0], contagem_medalhas[i][1]);
        
        // Prepara dados para o grafico (Soma total)
        labels_grafico[i] = siglas_paises[i]; 
        valores_grafico[i] = (double)(contagem_medalhas[i][0] + contagem_medalhas[i][1]);
    }
    gerar_grafico_barra("q1_comparativo.svg", labels_grafico, valores_grafico, 10, "Total V+I", "Qtd");
}

// Q2:
void q2_genero_atletas_por_pais(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    char codigo_pais[10];
    printf("Digite a sigla do Pais: "); scanf("%s", codigo_pais);

    printf("\nQ2: Genero (%s)\n", codigo_pais);
    
    // Arrays auxiliares para nao contar o mesmo atleta duas vezes
    char* visitados_masculino = (char*)calloc(MAXIMO_ATLETAS,1); 
    char* visitados_feminino = (char*)calloc(MAXIMO_ATLETAS,1);
    int total_homens=0, total_mulheres=0;

    // Loop principal
    for(int i=0; i<n_resultados; i++) {
        int id_atleta = resultados[i].id_atleta;
        
        // Filtra pelo pais e valida ID
        if(strcmp(resultados[i].noc, codigo_pais)==0 && id_atleta>=0 && id_atleta<MAXIMO_ATLETAS) {
            
            // Verifica evento feminino ou masculino e incrementa se inedito
            if(verifica_evento_feminino(resultados[i].evento)) { 
                if(!visitados_feminino[id_atleta]) { visitados_feminino[id_atleta]=1; total_mulheres++; } 
            }
            else { 
                if(!visitados_masculino[id_atleta]) { visitados_masculino[id_atleta]=1; total_homens++; } 
            }
        }
    }
    
    printf("   Homens: %d\n   Mulheres: %d\n", total_homens, total_mulheres);
    
    // Gera grafico comparativo
    char* labels_grafico[]={"Masc","Fem"}; 
    double valores_grafico[]={(double)total_homens,(double)total_mulheres};
    gerar_grafico_barra("q2_genero.svg", labels_grafico, valores_grafico, 2, "Genero", "Qtd");
    
    free(visitados_masculino); free(visitados_feminino);
}

// Q3:
void q3_medalhas_vs_populacao(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, Populacao* populacoes, int n_populacoes) {
    char codigo_pais[10]; int ano_busca;
    
    printf("Digite a sigla do Pais: "); scanf("%s", codigo_pais);
    printf("Digite o Ano: "); scanf("%d", &ano_busca);

    printf("\nQ3: Medalhas/populacao(%s - %d)\n", codigo_pais, ano_busca);
    
    int total_medalhas = 0;
    
    // Conta medalhas do pais naquele ano
    for(int i=0; i<n_resultados; i++)
        if(resultados[i].ano_olimpiada==ano_busca && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && strcmp(resultados[i].noc,codigo_pais)==0) 
            total_medalhas++;

    // Busca populacao correspondente
    double populacao_estimada = buscar_populacao_por_ano(populacoes, n_populacoes, codigo_pais, ano_busca);
    
    // Calcula razao (medalhas por milhao)
    printf("   Medalhas: %d | Pop: %.0f\n   Razao: %.4f med/milhao\n", total_medalhas, populacao_estimada, (populacao_estimada>0)?total_medalhas/(populacao_estimada/1000000.0):0);
}

// Q4: Idade media dos medalhistas de um ano
void q4_media_idade_medalhistas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    int ano_busca; printf("Digite o Ano da Edicao: "); scanf("%d", &ano_busca);
   
    printf("\n Q4: Idade média medalhistas (%d) \n", ano_busca);
    
    char* atletas_visitados = (char*)calloc(MAXIMO_ATLETAS,1);
    double soma_idades=0; int total_contabilizados=0;

    // Loop pelos resultados
    for(int i=0; i<n_resultados; i++) {
        int id_atleta = resultados[i].id_atleta;
        
        // Filtra: Ano correto, ganhou medalha e atleta valido
        if(resultados[i].ano_olimpiada==ano_busca && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && id_atleta>=0 && id_atleta<MAXIMO_ATLETAS && !atletas_visitados[id_atleta]) {
            
            // Busca dados pessoais do atleta pra calcular idade
            int indice_vetor_atleta = buscar_indice_atleta_por_id(atletas, n_atletas, id_atleta);
            if(indice_vetor_atleta!=-1 && atletas[indice_vetor_atleta].ano_nascimento>0) { 
                soma_idades += (ano_busca - atletas[indice_vetor_atleta].ano_nascimento); 
                total_contabilizados++; 
                atletas_visitados[id_atleta]=1; // Marca como visitado para nao somar mesmo atleta 2x
            }
        }
    }
    
    if(total_contabilizados > 0) printf("   -> Idade Media: %.2f anos\n", soma_idades/total_contabilizados);
    else printf("Nenhum dado encontrado para %d.\n", ano_busca);
    free(atletas_visitados);
}

// Q5:
void q5_media_peso_medalhistas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    int ano_busca; printf("Digite o Ano da Edicao: "); scanf("%d", &ano_busca);
  
    printf("\nQ5: Peso medio dos medalhistas (%d)\n", ano_busca);
    
    char* atletas_visitados = (char*)calloc(MAXIMO_ATLETAS,1);
    double soma_pesos=0; int total_contabilizados=0;

    for(int i=0; i<n_resultados; i++) {
        int id_atleta = resultados[i].id_atleta;
        // Filtra medalhistas do ano
        if(resultados[i].ano_olimpiada==ano_busca && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && id_atleta>=0 && id_atleta<MAXIMO_ATLETAS && !atletas_visitados[id_atleta]) {
            int indice_vetor_atleta = buscar_indice_atleta_por_id(atletas, n_atletas, id_atleta);
            // Soma peso se disponivel
            if(indice_vetor_atleta!=-1 && atletas[indice_vetor_atleta].peso>0) { 
                soma_pesos += atletas[indice_vetor_atleta].peso; 
                total_contabilizados++; 
                atletas_visitados[id_atleta]=1; 
            }
        }
    }
    if(total_contabilizados > 0) printf("   -> Peso Medio: %.2f kg\n", soma_pesos/total_contabilizados);
    else printf("Nenhum dado encontrado para %d.\n", ano_busca);
    free(atletas_visitados);
}

// Q6:
void q6_media_altura_medalhistas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    int ano_busca; printf("Digite o Ano da Edicao: "); scanf("%d", &ano_busca);
    
    printf("\nQ6: Altura media medalhistas (%d) \n", ano_busca);
    
    char* atletas_visitados = (char*)calloc(MAXIMO_ATLETAS,1);
    double soma_alturas=0; int total_contabilizados=0;

    for(int i=0; i<n_resultados; i++) {
        int id_atleta = resultados[i].id_atleta;
        // Filtra medalhistas do ano
        if(resultados[i].ano_olimpiada==ano_busca && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && id_atleta>=0 && id_atleta<MAXIMO_ATLETAS && !atletas_visitados[id_atleta]) {
            int indice_vetor_atleta = buscar_indice_atleta_por_id(atletas, n_atletas, id_atleta);
            // Soma altura se disponivel
            if(indice_vetor_atleta!=-1 && atletas[indice_vetor_atleta].altura>0) { 
                soma_alturas += atletas[indice_vetor_atleta].altura; 
                total_contabilizados++; 
                atletas_visitados[id_atleta]=1; 
            }
        }
    }
    if(total_contabilizados > 0) printf("   -> Altura Media: %.2f cm\n", soma_alturas/total_contabilizados);
    else printf("Nenhum dado encontrado para %d.\n", ano_busca);
    free(atletas_visitados);
}

// Q7: Atletas "Multiesportivos" que competiram no Verao E no Inverno
void q7_atletas_verao_e_inverno(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    printf("\nQ7: Atletas multiesportivos (Verao e inverno)\n");
    printf("Processando..\n");

    char* participou_verao = (char*)calloc(MAXIMO_ATLETAS,1);
    char* participou_inverno = (char*)calloc(MAXIMO_ATLETAS,1);

    // 1. Mapeamento: Marca quais atletas participaram de qual estacao
    for(int i=0; i<n_resultados; i++) {
        if(resultados[i].id_atleta>=0 && resultados[i].id_atleta<MAXIMO_ATLETAS) {
            if(strcmp(resultados[i].estacao,"Summer")==0) participou_verao[resultados[i].id_atleta]=1;
            else participou_inverno[resultados[i].id_atleta]=1;
        }
    }

    int total_multiesportivos = 0;
    printf("\n Lista de atletas\n");
    
    // 2. Verificacao: Percorre atletas e checa interseccao (Verao AND Inverno)
    for(int i=0; i<n_atletas; i++) {
        int id_atleta = atletas[i].id;
        if(id_atleta>=0 && id_atleta<MAXIMO_ATLETAS && participou_verao[id_atleta] && participou_inverno[id_atleta]) {
            printf("[%d] %s (%s): ", total_multiesportivos+1, atletas[i].nome, atletas[i].noc);

            // Imprime os anos de participacao
            int eh_primeiro_print = 1;
            int anos_ja_listados[30]; int qtd_anos_listados=0;

            for(int k=0; k<n_resultados; k++) {
                if(resultados[k].id_atleta == id_atleta) {
                    // Evita duplicar o mesmo ano na exibicao
                    int ano_ja_foi_impresso = 0;
                    for(int z=0; z<qtd_anos_listados; z++) if(anos_ja_listados[z] == resultados[k].ano_olimpiada) ano_ja_foi_impresso=1;

                    if(!ano_ja_foi_impresso) {
                        if(!eh_primeiro_print) printf(", ");
                        printf("%d %s", resultados[k].ano_olimpiada, resultados[k].estacao);
                        eh_primeiro_print = 0;
                        anos_ja_listados[qtd_anos_listados++] = resultados[k].ano_olimpiada;
                    }
                }
            }
            printf("\n");
            total_multiesportivos++;
        }
    }
    printf("\n-> Total atletas encontrados:: %d\n", total_multiesportivos);
    free(participou_verao); free(participou_inverno);
}

// Q8: Ranking de paises (NOCs) com mais medalhas em um esporte especifico
void q8_ranking_pais_por_esporte(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, NocRegiao* regioes, int n_regioes) {
    char nome_esporte[100];
    printf("Digite o esporte (Ingles, ex: Judo): "); scanf(" %99[^\n]", nome_esporte);
    printf("\n Q8: Ranking em %s \n", nome_esporte);

    ParChaveValor lista_paises[200]; int qtd_paises_encontrados=0;
    
    // Loop para contar medalhas por pais no esporte
    for(int i=0; i<n_resultados; i++) {
        if(strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && strstr(resultados[i].esporte, nome_esporte)) {
            char* sigla_pais = resultados[i].noc; 
            int indice_pais_na_lista = -1;
            
            // Busca se pais ja esta na lista
            for(int k=0; k<qtd_paises_encontrados; k++) if(strcmp(lista_paises[k].chave, sigla_pais)==0) indice_pais_na_lista=k;
            
            // Incrementa ou adiciona novo pais
            if(indice_pais_na_lista!=-1) lista_paises[indice_pais_na_lista].valor++; 
            else { 
                strcpy(lista_paises[qtd_paises_encontrados].chave, sigla_pais); 
                lista_paises[qtd_paises_encontrados].valor=1; 
                qtd_paises_encontrados++; 
            }
        }
    }
    
    // Ordena decrescente
    qsort(lista_paises, qtd_paises_encontrados, sizeof(ParChaveValor), comparar_decrescente);
    
    double valores_grafico[10]; char* labels_grafico[10];
    // Exibe top 10
    for(int i=0; i<10 && i<qtd_paises_encontrados; i++) {
        printf("#%d %s: %d\n", i+1, lista_paises[i].chave, lista_paises[i].valor);
        valores_grafico[i] = lista_paises[i].valor; 
        labels_grafico[i] = lista_paises[i].chave;
    }
    gerar_grafico_barra("q8_ranking.svg", labels_grafico, valores_grafico, (qtd_paises_encontrados<10?qtd_paises_encontrados:10), nome_esporte, "Med");
}

// Q9: Evolucao do numero de atletas unicos ao longo dos anos
void q9_evolucao_numero_atletas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    printf("\n Q9: Evolucao 2004-2022 \n");
    int anos_alvo[] = {2004, 2006, 2008, 2010, 2012, 2014, 2016, 2018, 2020, 2022};
    int qtd_anos_analisados = 10;

    char* visitado_global = (char*)calloc(MAXIMO_ATLETAS, 1);
    char* visitado_no_ano = (char*)malloc(MAXIMO_ATLETAS);

    int total_unicos_periodo = 0;
    double eixo_x_anos[10], eixo_y_qtd[10];

    // Loop por cada ano definido
    for(int i=0; i<qtd_anos_analisados; i++) {
        int ano_atual = anos_alvo[i];
        int total_atletas_ano = 0;
        
        // Limpa vetor de visita para o ano atual
        memset(visitado_no_ano, 0, MAXIMO_ATLETAS);

        // Conta atletas no ano
        for(int k=0; k<n_resultados; k++) {
            int id_atleta = resultados[k].id_atleta;
            if(resultados[k].ano_olimpiada == ano_atual && id_atleta >= 0 && id_atleta < MAXIMO_ATLETAS) {
                if(!visitado_no_ano[id_atleta]) { visitado_no_ano[id_atleta] = 1; total_atletas_ano++; }
                // Conta para o total global unico
                if(!visitado_global[id_atleta]) { visitado_global[id_atleta] = 1; total_unicos_periodo++; }
            }
        }
        printf("   Ano %d: %d atletas\n", ano_atual, total_atletas_ano);
        eixo_x_anos[i] = (double)ano_atual;
        eixo_y_qtd[i] = (double)total_atletas_ano;
    }

    printf("\n   -> Total de atletas unicos (2004-2022): %d\n", total_unicos_periodo);
    gerar_grafico_linha("q9_evolucao.svg", eixo_x_anos, eixo_y_qtd, qtd_anos_analisados, "Evolucao 2004-2022", "Ano", "Atletas");
    free(visitado_global); free(visitado_no_ano);
}

// Q10: Eficiencia (Medalhas / Atletas Participantes) de 10 paises
void q10_eficiencia_pais_medalhas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    char siglas_paises[10][10]; int ano_busca;
    printf("\n Q10: Eficiencia de 10 paises\n");
    printf("Digite o Ano: "); scanf("%d", &ano_busca);
    printf("Digite 10 siglas de paises:\n");
    
    // Input
    for(int i=0; i<10; i++) {
        printf("Pais %d: ", i+1); scanf("%s", siglas_paises[i]); }

    char* atletas_visitados = (char*)calloc(MAXIMO_ATLETAS, 1);
    char* labels_grafico[10]; double valores_grafico[10];

    printf("\n| PAIS | MED | ATL | EFIC |\n");
    // Loop para cada pais escolhido
    for(int i=0; i<10; i++) {
        int total_medalhas=0, total_atletas=0; 
        memset(atletas_visitados, 0, MAXIMO_ATLETAS); // Reseta visitados para o proximo pais

        // Conta medalhas e atletas unicos do pais
        for(int k=0; k<n_resultados; k++) {
            if(resultados[k].ano_olimpiada==ano_busca && strcmp(resultados[k].noc, siglas_paises[i])==0) {
                if(strlen(resultados[k].medalha)>0 && strcmp(resultados[k].medalha,"NA")!=0) total_medalhas++;
                
                int id_atleta = resultados[k].id_atleta;
                if(id_atleta>=0 && id_atleta<MAXIMO_ATLETAS && !atletas_visitados[id_atleta]) { 
                    atletas_visitados[id_atleta]=1; 
                    total_atletas++; 
                }
            }
        }
        
        // Calcula e exibe eficiencia
        double eficiencia = (total_atletas>0) ? (double)total_medalhas/total_atletas : 0.0;
        printf("| %-4s | %-3d | %-3d | %.3f |\n", siglas_paises[i], total_medalhas, total_atletas, eficiencia);
        labels_grafico[i] = siglas_paises[i]; 
        valores_grafico[i] = eficiencia;
    }
    gerar_grafico_barra("q10_eficiencia.svg", labels_grafico, valores_grafico, 10, "Eficiencia", "Ind"); free(atletas_visitados);
}

// Q11: Ano e tipo da primeira medalha de um pais
void q11_primeira_medalha_pais(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    char codigo_pais[10];
    
    printf("Digite a sigla do Pais: "); scanf("%s", codigo_pais);
    printf("\nQ11: Primeira medalha (%s) \n", codigo_pais);
    
    int menor_ano = 9999; char tipo_medalha[50]="";
    
    // Varre resultados buscando o ano minimo com medalha
    for(int i=0; i<n_resultados; i++) 
        if(strcmp(resultados[i].noc, codigo_pais)==0 && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0) {
            if(resultados[i].ano_olimpiada < menor_ano) { 
                menor_ano = resultados[i].ano_olimpiada; 
                strcpy(tipo_medalha, resultados[i].medalha); 
            }
        }
        
    if(menor_ano != 9999) printf("   Ano: %d (%s)\n", menor_ano, tipo_medalha); else printf("   Nenhuma.\n");
}

// Q12: Comparativo e ranking simples de medalhas entre 10 paises
void q12_ranking_paises_total_medalhas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, NocRegiao* regioes, int n_regioes) {
    char siglas_paises[10][10]; int contagem_medalhas[10];
    printf("\n Q12: Ranking 10 paises escolhidos(total de medalhas)\n");
    printf("Digite as siglas de 10 paises:\n");
    
    // Input e reset contadores
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", siglas_paises[i]); contagem_medalhas[i]=0; }

    // Conta medalhas para cada um dos 10 paises
    for(int i=0; i<n_resultados; i++) if(strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0) {
        for(int p=0; p<10; p++) if(strcmp(resultados[i].noc, siglas_paises[p])==0) contagem_medalhas[p]++;
    }
    
    // Ordenacao simples (Bubble Sort)
    for(int i=0; i<9; i++) for(int j=i+1; j<10; j++) if(contagem_medalhas[j]>contagem_medalhas[i]) {
        // Troca valor
        int temp_valor = contagem_medalhas[i]; 
        contagem_medalhas[i] = contagem_medalhas[j]; 
        contagem_medalhas[j] = temp_valor;
        
        // Troca nome/sigla
        char temp_sigla[10]; 
        strcpy(temp_sigla, siglas_paises[i]); 
        strcpy(siglas_paises[i], siglas_paises[j]); 
        strcpy(siglas_paises[j], temp_sigla);
    }
    
    // Exibe e plota
    double valores_grafico[10]; char* labels_grafico[10];
    for(int i=0; i<10; i++) { 
        printf("#%d %s: %d\n", i+1, siglas_paises[i], contagem_medalhas[i]); 
        valores_grafico[i] = (double)contagem_medalhas[i]; 
        labels_grafico[i] = siglas_paises[i]; 
    }
    gerar_grafico_barra("q12_ranking.svg", labels_grafico, valores_grafico, 10, "Ranking Paises", "Total");
}
