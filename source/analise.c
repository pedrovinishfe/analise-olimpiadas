#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

typedef struct { char chave[100]; int valor; } ParChaveValor;
int comparar_decrescente(const void* a, const void* b) { return ((ParChaveValor*)b)->valor - ((ParChaveValor*)a)->valor; }

typedef struct { int id_atleta; int idade; } ParIdade;
int comparar_idade_decrescente(const void* a, const void* b) { return ((ParIdade*)b)->idade - ((ParIdade*)a)->idade; }

// Q1
void q1_total_medalhas_por_estacao(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, NocRegiao* regioes, int n_regioes) {
    char paises[10][10];
    int contagem[10][2];
    for(int i=0; i<10; i++) { contagem[i][0]=0; contagem[i][1]=0; }

    printf("\n=== Q1: COMPARATIVO 10 PAISES ===\n");
    printf("Digite as siglas de 10 paises (ex: BRA USA CHN):\n");
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", paises[i]); }

    for(int i=0; i<n_resultados; i++) {
        if(strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0) {
            for(int p=0; p<10; p++) {
                if(strcmp(resultados[i].noc, paises[p])==0) {
                    int idx = (strcmp(resultados[i].estacao, "Summer")==0) ? 0 : 1;
                    contagem[p][idx]++;
                }
            }
        }
    }

    printf("\n| %-25s | %-8s | %-8s |\n", "REGIAO", "VERAO", "INVERNO");
    char* labels[10]; double vals[10];
    for(int p=0; p<10; p++) {
        char* nome = buscar_nome_pais_por_noc(regioes, n_regioes, paises[p]);
        char display[26]; strncpy(display, nome, 25); display[25]='\0';
        printf("| %-25s | %-8d | %-8d |\n", display, contagem[p][0], contagem[p][1]);
        labels[p] = paises[p]; vals[p] = (double)(contagem[p][0] + contagem[p][1]);
    }
    gerar_grafico_barra("q1_comparativo.svg", labels, vals, 10, "Total V+I", "Qtd");
}

// Q2
void q2_genero_atletas_por_pais(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    char codigo_pais[10];
    printf("Digite a sigla do Pais: "); scanf("%s", codigo_pais);

    printf("\n=== Q2: GENERO (%s) ===\n", codigo_pais);
    char* masc=(char*)calloc(MAXIMO_ATLETAS,1); char* fem=(char*)calloc(MAXIMO_ATLETAS,1);
    int tm=0, tf=0;
    for(int i=0; i<n_resultados; i++) {
        int id=resultados[i].id_atleta;
        if(strcmp(resultados[i].noc, codigo_pais)==0 && id>=0 && id<MAXIMO_ATLETAS) {
            if(verifica_evento_feminino(resultados[i].evento)) { if(!fem[id]) { fem[id]=1; tf++; } }
            else { if(!masc[id]) { masc[id]=1; tm++; } }
        }
    }
    printf("   Homens: %d\n   Mulheres: %d\n", tm, tf);
    char* l[]={"Masc","Fem"}; double v[]={(double)tm,(double)tf};
    gerar_grafico_barra("q2_genero.svg", l, v, 2, "Genero", "Qtd");
    free(masc); free(fem);
}

// Q3
void q3_medalhas_vs_populacao(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados, Populacao* populacoes, int n_populacoes) {
    char codigo_pais[10]; int ano;
    printf("Digite a sigla do Pais: "); scanf("%s", codigo_pais);
    printf("Digite o Ano: "); scanf("%d", &ano);

    printf("\n=== Q3: MEDALHAS/POPULACAO (%s - %d) ===\n", codigo_pais, ano);
    int med = 0;
    for(int i=0; i<n_resultados; i++)
        if(resultados[i].ano_olimpiada==ano && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && strcmp(resultados[i].noc,codigo_pais)==0) med++;

        double pop = buscar_populacao_por_ano(populacoes, n_populacoes, codigo_pais, ano);
    printf("   Medalhas: %d | Pop: %.0f\n   Razao: %.4f med/milhao\n", med, pop, (pop>0)?med/(pop/1000000.0):0);

    char* labels[] = {"Medalhas", "Pop (M)"};
    double vals[] = {(double)med, pop/1000000.0};
    gerar_grafico_barra("q3_populacao.svg", labels, vals, 2, "Medalhas vs Pop", "Val");
}

// Q4
void q4_media_idade_medalhistas(Atleta* atletas, int n_atletas, Resultado* resultados, int n_resultados) {
    int ano; printf("Digite o Ano da Edicao: "); scanf("%d", &ano);
    printf("\n=== Q4: IDADE MEDIA MEDALHISTAS (%d) ===\n", ano);
    char* vis=(char*)calloc(MAXIMO_ATLETAS,1);
    double soma=0; int count=0;

    for(int i=0; i<n_resultados; i++) {
        int id=resultados[i].id_atleta;
        if(resultados[i].ano_olimpiada==ano && strlen(resultados[i].medalha)>0 && strcmp(resultados[i].medalha,"NA")!=0 && id>=0 && id<MAXIMO_ATLETAS && !vis[id]) {
            int idx=buscar_indice_atleta_por_id(atletas, n_atletas, id);
            if(idx!=-1 && atletas[idx].ano_nascimento>0) { soma += (ano - atletas[idx].ano_nascimento); count++; vis[id]=1; }
        }
    }
    double med = (count > 0) ? soma/count : 0;
    if(count > 0) printf("   -> Idade Media: %.2f anos\n", med); else printf("   [AVISO] Nenhum dado.\n");

    char* l[]={"Idade"}; double v[]={med};
    gerar_grafico_barra("q4_idade.svg", l, v, 1, "Idade Media", "Anos");
    free(vis);
}

// Q5
void q5_media_peso_medalhistas(Atleta* a, int na, Resultado* r, int nr) {
    int ano; printf("Digite o Ano da Edicao: "); scanf("%d", &ano);
    printf("\n=== Q5: PESO MEDIO MEDALHISTAS (%d) ===\n", ano);
    char* vis=(char*)calloc(MAXIMO_ATLETAS,1);
    double soma=0; int count=0;
    for(int i=0; i<nr; i++) {
        int id=r[i].id_atleta;
        if(r[i].ano_olimpiada==ano && strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0 && id>=0 && id<MAXIMO_ATLETAS && !vis[id]) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1 && a[idx].peso>0) { soma += a[idx].peso; count++; vis[id]=1; }
        }
    }
    double med = (count > 0) ? soma/count : 0;
    if(count > 0) printf("   -> Peso Medio: %.2f kg\n", med); else printf("   [AVISO] Nenhum dado.\n");

    char* l[]={"Peso"}; double v[]={med};
    gerar_grafico_barra("q5_peso.svg", l, v, 1, "Peso Medio", "kg");
    free(vis);
}

// Q6
void q6_media_altura_medalhistas(Atleta* a, int na, Resultado* r, int nr) {
    int ano; printf("Digite o Ano da Edicao: "); scanf("%d", &ano);
    printf("\n=== Q6: ALTURA MEDIA MEDALHISTAS (%d) ===\n", ano);
    char* vis=(char*)calloc(MAXIMO_ATLETAS,1);
    double soma=0; int count=0;
    for(int i=0; i<nr; i++) {
        int id=r[i].id_atleta;
        if(r[i].ano_olimpiada==ano && strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0 && id>=0 && id<MAXIMO_ATLETAS && !vis[id]) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1 && a[idx].altura>0) { soma += a[idx].altura; count++; vis[id]=1; }
        }
    }
    double med = (count > 0) ? soma/count : 0;
    if(count > 0) printf("   -> Altura Media: %.2f cm\n", med); else printf("   [AVISO] Nenhum dado.\n");

    char* l[]={"Altura"}; double v[]={med};
    gerar_grafico_barra("q6_altura.svg", l, v, 1, "Altura Media", "cm");
    free(vis);
}

// Q7
void q7_atletas_verao_e_inverno(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q7: ATLETAS MULTIESPORTIVOS (VERAO+INVERNO) ===\n");
    char* ver=(char*)calloc(MAXIMO_ATLETAS,1);
    char* inv=(char*)calloc(MAXIMO_ATLETAS,1);

    for(int k=0; k<nr; k++) {
        if(r[k].id_atleta>=0 && r[k].id_atleta<MAXIMO_ATLETAS) {
            if(strcmp(r[k].estacao,"Summer")==0) ver[r[k].id_atleta]=1;
            else inv[r[k].id_atleta]=1;
        }
    }

    int count = 0, tv=0, ti=0;
    printf("\n--- LISTA DE ATLETAS ---\n");
    for(int i=0; i<na; i++) {
        int id = a[i].id;
        if(id>=0 && id<MAXIMO_ATLETAS) {
            if(ver[id] && inv[id]) {
                printf("[%d] %s (%s)\n", count+1, a[i].nome, a[i].noc);
                count++;
            } else if(ver[id]) tv++; else if(inv[id]) ti++;
        }
    }
    printf("\n-> TOTAL ATLETAS ENCONTRADOS: %d\n", count);

    char* l[]={"So Verao", "So Inverno", "Ambos"}; double v[]={(double)tv, (double)ti, (double)count};
    gerar_grafico_barra("q7_multiesporte.svg", l, v, 3, "Tipos", "Qtd");
    free(ver); free(inv);
}

// Q8
void q8_ranking_pais_por_esporte(Atleta* a, int na, Resultado* r, int nr, NocRegiao* re, int nre) {
    char nome_esporte[100];
    printf("Digite o esporte (Ingles, ex: Judo): "); scanf(" %99[^\n]", nome_esporte);
    printf("\n=== Q8: RANKING EM %s ===\n", nome_esporte);

    ParChaveValor l[200]; int n=0;
    for(int i=0; i<nr; i++) {
        if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0 && strstr(r[i].esporte, nome_esporte)) {
            char* noc=r[i].noc; int p=-1;
            for(int k=0; k<n; k++) if(strcmp(l[k].chave,noc)==0) p=k;
            if(p!=-1) l[p].valor++; else { strcpy(l[n].chave,noc); l[n].valor=1; n++; }
        }
    }
    qsort(l, n, sizeof(ParChaveValor), comparar_decrescente);
    double v[10]; char* lb[10];
    for(int i=0; i<10 && i<n; i++) {
        printf("#%d %s: %d\n", i+1, l[i].chave, l[i].valor);
        v[i]=l[i].valor; lb[i]=l[i].chave;
    }
    gerar_grafico_barra("q8_ranking.svg", lb, v, (n<10?n:10), nome_esporte, "Med");
}

// Q9
void q9_evolucao_numero_atletas(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q9: EVOLUCAO 2004-2022 ===\n");
    int anos_alvo[] = {2004, 2006, 2008, 2010, 2012, 2014, 2016, 2018, 2020, 2022};
    char* visitado_global = (char*)calloc(MAXIMO_ATLETAS, 1);
    char* visitado_ano = (char*)malloc(MAXIMO_ATLETAS);
    int total_unicos_periodo = 0;
    double x[10], y[10];

    for(int i=0; i<10; i++) {
        int ano = anos_alvo[i];
        int count_ano = 0;
        memset(visitado_ano, 0, MAXIMO_ATLETAS);

        for(int k=0; k<nr; k++) {
            if(r[k].ano_olimpiada == ano && r[k].id_atleta >= 0 && r[k].id_atleta < MAXIMO_ATLETAS) {
                if(!visitado_ano[r[k].id_atleta]) { visitado_ano[r[k].id_atleta] = 1; count_ano++; }
                if(!visitado_global[r[k].id_atleta]) { visitado_global[r[k].id_atleta] = 1; total_unicos_periodo++; }
            }
        }
        printf("   Ano %d: %d atletas\n", ano, count_ano);
        x[i] = (double)ano; y[i] = (double)count_ano;
    }
    printf("\n   -> TOTAL DE ATLETAS UNICOS: %d\n", total_unicos_periodo);
    gerar_grafico_linha("q9_evolucao.svg", x, y, 10, "Evolucao", "Ano", "Atletas");
    free(visitado_global); free(visitado_ano);
}

// Q10
void q10_eficiencia_pais_medalhas(Atleta* a, int na, Resultado* r, int nr) {
    char paises[10][10]; int ano;
    printf("\n=== Q10: EFICIENCIA ===\n");
    printf("Digite o Ano: "); scanf("%d", &ano);
    printf("Digite 10 siglas:\n");
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", paises[i]); }

    char* vis=(char*)calloc(MAXIMO_ATLETAS, 1);
    char* labs[10]; double vals[10];

    printf("\n| PAIS | MED | ATL | EFIC |\n");
    for(int p=0; p<10; p++) {
        int med=0, atl=0; memset(vis, 0, MAXIMO_ATLETAS);
        for(int i=0; i<nr; i++) if(r[i].ano_olimpiada==ano && strcmp(r[i].noc, paises[p])==0) {
            if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) med++;
            if(r[i].id_atleta>=0 && r[i].id_atleta<MAXIMO_ATLETAS && !vis[r[i].id_atleta]) { vis[r[i].id_atleta]=1; atl++; }
        }
        double ef = (atl>0) ? (double)med/atl : 0.0;
        printf("| %-4s | %-3d | %-3d | %.3f |\n", paises[p], med, atl, ef);
        labs[p]=paises[p]; vals[p]=ef;
    }
    gerar_grafico_barra("q10_eficiencia.svg", labs, vals, 10, "Eficiencia", "Ind"); free(vis);
}

// Q11
void q11_primeira_medalha_pais(Atleta* a, int na, Resultado* r, int nr) {
    char codigo_pais[10];
    printf("Digite a sigla: "); scanf("%s", codigo_pais);
    printf("\n=== Q11: PRIMEIRA MEDALHA (%s) ===\n", codigo_pais);
    int min=9999; char tipo[50]="";
    for(int i=0; i<nr; i++) if(strcmp(r[i].noc, codigo_pais)==0 && strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) {
        if(r[i].ano_olimpiada < min) { min=r[i].ano_olimpiada; strcpy(tipo, r[i].medalha); }
    }
    double v=0;
    if(min!=9999) { printf("   Ano: %d (%s)\n", min, tipo); v=(double)min; }
    else printf("   Nenhuma.\n");

    char* l[]={"Ano"}; double val[]={v};
    gerar_grafico_barra("q11_primeira.svg", l, val, 1, codigo_pais, "Ano");
}

// Q12
void q12_ranking_paises_total_medalhas(Atleta* a, int na, Resultado* r, int nr, NocRegiao* re, int nre) {
    char paises[10][10]; int contagem[10];
    printf("\n=== Q12: RANKING 10 PAISES ===\n");
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", paises[i]); contagem[i]=0; }

    for(int i=0; i<nr; i++) if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) {
        for(int p=0; p<10; p++) if(strcmp(r[i].noc, paises[p])==0) contagem[p]++;
    }
    for(int i=0; i<9; i++) for(int j=i+1; j<10; j++) if(contagem[j]>contagem[i]) {
        int t=contagem[i]; contagem[i]=contagem[j]; contagem[j]=t;
        char ts[10]; strcpy(ts,paises[i]); strcpy(paises[i],paises[j]); strcpy(paises[j],ts);
    }
    double v[10]; char* l[10];
    for(int i=0; i<10; i++) { printf("#%d %s: %d\n", i+1, paises[i], contagem[i]); v[i]=(double)contagem[i]; l[i]=paises[i]; }
    gerar_grafico_barra("q12_ranking.svg", l, v, 10, "Ranking", "Total");
}

// Q13
void q13_top_atletas_maiores_medalhistas(Atleta* a, int na, Resultado* r, int nr, NocRegiao* re, int nre) {
    printf("\n=== Q13: TOP 10 ATLETAS ===\n");
    int* c=(int*)calloc(MAXIMO_ATLETAS,sizeof(int));
    for(int i=0; i<nr; i++) if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0 && r[i].id_atleta>=0) c[r[i].id_atleta]++;

    double v[10]; char* l[10];
    for(int k=0; k<10; k++) {
        int max=0, id=-1; for(int i=0; i<MAXIMO_ATLETAS; i++) if(c[i]>max) { max=c[i]; id=i; }
        if(max>0) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1) {
                printf("#%d %s (%s): %d\n", k+1, a[idx].nome, a[idx].noc, max);
                l[k]=(char*)malloc(30); strncpy(l[k], a[idx].nome, 10); l[k][10]=0; v[k]=max;
            }
            c[id]=-1;
        }
    }
    gerar_grafico_barra("q13_atletas.svg", l, v, 10, "Top Atletas", "Med"); free(c);
}

// Q14
void q14_ranking_paises_participacao(Atleta* a, int na, Resultado* r, int nr, NocRegiao* re, int nre) {
    char paises[10][10]; int contagem[10];
    printf("\n=== Q14: PARTICIPACAO ===\nDigite 10 siglas:\n");
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", paises[i]); contagem[i]=0; }

    for(int i=0; i<nr; i++) for(int p=0; p<10; p++) if(strcmp(r[i].noc, paises[p])==0) contagem[p]++;

    for(int i=0; i<9; i++) for(int j=i+1; j<10; j++) if(contagem[j]>contagem[i]) {
        int t=contagem[i]; contagem[i]=contagem[j]; contagem[j]=t;
        char ts[10]; strcpy(ts,paises[i]); strcpy(paises[i],paises[j]); strcpy(paises[j],ts);
    }
    double v[10]; char* l[10];
    for(int i=0; i<10; i++) { printf("#%d %s: %d\n", i+1, paises[i], contagem[i]); v[i]=(double)contagem[i]; l[i]=paises[i]; }
    gerar_grafico_barra("q14_part.svg", l, v, 10, "Part", "Qtd");
}

// Q15
void q15_ranking_ultimas_olimpiadas(Resultado* r, int nr) {
    printf("\n=== Q15: ULTIMAS 10 EDICOES ===\n");
    int anos[200], n_anos=0;
    for(int i=0; i<nr; i++) {
        int ex=0; for(int k=0;k<n_anos;k++) if(anos[k]==r[i].ano_olimpiada) ex=1;
        if(!ex && r[i].ano_olimpiada>0) anos[n_anos++]=r[i].ano_olimpiada;
    }
    for(int i=0; i<n_anos-1; i++) for(int j=i+1; j<n_anos; j++) if(anos[j]>anos[i]) { int t=anos[i]; anos[i]=anos[j]; anos[j]=t; }

    int lim = (n_anos<10)?n_anos:10; ParChaveValor l[10];
    for(int k=0; k<lim; k++) {
        int c=0; for(int i=0; i<nr; i++) if(r[i].ano_olimpiada==anos[k] && strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) c++;
        sprintf(l[k].chave, "%d", anos[k]); l[k].valor=c;
    }
    qsort(l, lim, sizeof(ParChaveValor), comparar_decrescente);
    double v[10]; char* lb[10];
    for(int i=0; i<lim; i++) { printf("#%d %s: %d\n", i+1, l[i].chave, l[i].valor); v[i]=l[i].valor; lb[i]=l[i].chave; }
    gerar_grafico_barra("q15_ranking.svg", lb, v, lim, "Edicoes", "Total");
}

// Q16
void q16_atletas_por_esporte_ano(Resultado* r, int nr) {
    char esp[10][100]; int c[10]; int ano;
    char* vis = (char*)calloc(MAXIMO_ATLETAS, 1);
    printf("\n=== Q16: 10 ESPORTES ===\nAno: "); scanf("%d", &ano);
    printf("10 Esportes:\n"); for(int i=0; i<10; i++) { printf("%d: ", i+1); scanf(" %99[^\n]", esp[i]); c[i]=0; }

    for(int e=0; e<10; e++) {
        memset(vis, 0, MAXIMO_ATLETAS);
        for(int i=0; i<nr; i++) if(r[i].ano_olimpiada==ano && strstr(r[i].esporte, esp[e]) && r[i].id_atleta>=0 && !vis[r[i].id_atleta]) {
            vis[r[i].id_atleta]=1; c[e]++;
        }
    }
    for(int i=0; i<9; i++) for(int j=i+1; j<10; j++) if(c[j]>c[i]) {
        int t=c[i]; c[i]=c[j]; c[j]=t;
        char ts[100]; strcpy(ts, esp[i]); strcpy(esp[i], esp[j]); strcpy(esp[j], ts);
    }
    double v[10]; char* lb[10];
    for(int i=0; i<10; i++) { printf("#%d %s: %d\n", i+1, esp[i], c[i]); v[i]=c[i]; lb[i]=esp[i]; }
    gerar_grafico_barra("q16_esporte.svg", lb, v, 10, "Esportes", "Atletas"); free(vis);
}

// Q17
void q17_evolucao_participacao_feminina(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q17: EVOLUCAO FEMININA ===\n");
    double x[200], y[200]; int p=0; char* vis=(char*)malloc(MAXIMO_ATLETAS);
    for(int ano=1896; ano<=2024; ano++) {
        int c=0, h=0; memset(vis,0,MAXIMO_ATLETAS);
        for(int i=0; i<nr; i++) if(r[i].ano_olimpiada==ano) {
            h=1;
            if(r[i].id_atleta>=0 && verifica_evento_feminino(r[i].evento) && !vis[r[i].id_atleta]) { vis[r[i].id_atleta]=1; c++; }
        }
        if(h) {
            printf("Ano %d: %d\n", ano, c);
            if(p<200) { x[p]=ano; y[p]=c; p++; }
        }
    }
    gerar_grafico_linha("q17_mulheres.svg", x, y, p, "Fem", "Ano", "Qtd"); free(vis);
}

// Q18
void q18_ranking_atletas_mais_velhos(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q18: 10 MAIS VELHOS ===\n");
    int* mm=(int*)calloc(MAXIMO_ATLETAS, sizeof(int));
    int* mf=(int*)calloc(MAXIMO_ATLETAS, sizeof(int));

    for(int i=0; i<nr; i++) {
        int id=r[i].id_atleta;
        if(id>=0 && id<MAXIMO_ATLETAS) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1 && a[idx].ano_nascimento>0) {
                int age=r[i].ano_olimpiada - a[idx].ano_nascimento;
                if(verifica_evento_feminino(r[i].evento)) { if(age>mf[id]) mf[id]=age; }
                else { if(age>mm[id]) mm[id]=age; }
            }
        }
    }

    ParIdade* lm = malloc(MAXIMO_ATLETAS * sizeof(ParIdade));
    ParIdade* lf = malloc(MAXIMO_ATLETAS * sizeof(ParIdade));
    int cm=0, cf=0;

    for(int i=0; i<MAXIMO_ATLETAS; i++) {
        if(mm[i]>0) { lm[cm].id_atleta=i; lm[cm].idade=mm[i]; cm++; }
        if(mf[i]>0) { lf[cf].id_atleta=i; lf[cf].idade=mf[i]; cf++; }
    }
    qsort(lm, cm, sizeof(ParIdade), comparar_idade_decrescente);
    qsort(lf, cf, sizeof(ParIdade), comparar_idade_decrescente);

    double v[10]; char* lbl[10];
    printf("\n--- HOMENS ---\n");
    for(int i=0; i<10 && i<cm; i++) {
        int idx=buscar_indice_atleta_por_id(a, na, lm[i].id_atleta);
        if(idx!=-1) {
            printf("%d. %s: %d anos\n", i+1, a[idx].nome, lm[i].idade);
            lbl[i]=(char*)malloc(30); strncpy(lbl[i], a[idx].nome, 10); lbl[i][10]=0; v[i]=lm[i].idade;
        }
    }
    gerar_grafico_barra("q18_velhos_masc.svg", lbl, v, 10, "Masc", "Idade");

    printf("\n--- MULHERES ---\n");
    for(int i=0; i<10 && i<cf; i++) {
        int idx=buscar_indice_atleta_por_id(a, na, lf[i].id_atleta);
        if(idx!=-1) printf("%d. %s: %d anos\n", i+1, a[idx].nome, lf[i].idade);
    }
    free(mm); free(mf); free(lm); free(lf);
}

// Q19
void q19_ranking_medalhistas_mais_velhos(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q19: MEDALHISTAS MAIS VELHOS ===\n");
    int* mm=(int*)calloc(MAXIMO_ATLETAS, sizeof(int));
    int* mf=(int*)calloc(MAXIMO_ATLETAS, sizeof(int));

    for(int i=0; i<nr; i++) if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) {
        int id=r[i].id_atleta;
        if(id>=0 && id<MAXIMO_ATLETAS) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1 && a[idx].ano_nascimento>0) {
                int age=r[i].ano_olimpiada - a[idx].ano_nascimento;
                if(verifica_evento_feminino(r[i].evento)) { if(age>mf[id]) mf[id]=age; }
                else { if(age>mm[id]) mm[id]=age; }
            }
        }
    }
    ParIdade* lm = malloc(MAXIMO_ATLETAS * sizeof(ParIdade));
    ParIdade* lf = malloc(MAXIMO_ATLETAS * sizeof(ParIdade));
    int cm=0, cf=0;

    for(int i=0; i<MAXIMO_ATLETAS; i++) {
        if(mm[i]>0) { lm[cm].id_atleta=i; lm[cm].idade=mm[i]; cm++; }
        if(mf[i]>0) { lf[cf].id_atleta=i; lf[cf].idade=mf[i]; cf++; }
    }
    qsort(lm, cm, sizeof(ParIdade), comparar_idade_decrescente);
    qsort(lf, cf, sizeof(ParIdade), comparar_idade_decrescente);

    double v[10]; char* lbl[10];
    printf("\n--- HOMENS ---\n");
    for(int i=0; i<10 && i<cm; i++) {
        int idx=buscar_indice_atleta_por_id(a, na, lm[i].id_atleta);
        if(idx!=-1) {
            printf("%d. %s: %d anos\n", i+1, a[idx].nome, lm[i].idade);
            lbl[i]=(char*)malloc(30); strncpy(lbl[i], a[idx].nome, 10); lbl[i][10]=0; v[i]=lm[i].idade;
        }
    }
    gerar_grafico_barra("q19_med_masc.svg", lbl, v, 10, "Med Masc", "Idade");

    printf("\n--- MULHERES ---\n");
    for(int i=0; i<10 && i<cf; i++) {
        int idx=buscar_indice_atleta_por_id(a, na, lf[i].id_atleta);
        if(idx!=-1) printf("%d. %s: %d anos\n", i+1, a[idx].nome, lf[i].idade);
    }
    free(mm); free(mf); free(lm); free(lf);
}

// Q20
void q20_atletas_com_mais_participacoes(Atleta* a, int na, Resultado* r, int nr) {
    printf("\n=== Q20: MAIS PARTICIPACOES ===\n");
    int* c=(int*)calloc(MAXIMO_ATLETAS, sizeof(int));
    for(int i=0; i<nr; i++) if(r[i].id_atleta>=0 && r[i].id_atleta<MAXIMO_ATLETAS) c[r[i].id_atleta]++;

    double v[10]; char* l[10];
    for(int k=0; k<10; k++) {
        int max=0, id=-1; for(int i=0; i<MAXIMO_ATLETAS; i++) if(c[i]>max) { max=c[i]; id=i; }
        if(max>0) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            printf("#%d %s: %d\n", k+1, a[idx].nome, max);
            l[k]=(char*)malloc(30); strncpy(l[k], a[idx].nome, 10); l[k][10]=0; v[k]=max; c[id]=-1;
        }
    }
    gerar_grafico_barra("q20_participacao.svg", l, v, 10, "Part", "Qtd"); free(c);
}

// Q21
void q21_peso_medio_medalhistas_ouro(Atleta* a, int na, Resultado* r, int nr) {
    int ano; printf("Digite o Ano: "); scanf("%d", &ano);
    printf("\n=== Q21: PESO MEDIO OURO (%d) ===\n", ano);
    char* vis=(char*)calloc(MAXIMO_ATLETAS,1); double s=0; int c=0;
    for(int i=0; i<nr; i++) {
        int id=r[i].id_atleta;
        if(r[i].ano_olimpiada==ano && strcmp(r[i].medalha,"Gold")==0 && id>=0 && id<MAXIMO_ATLETAS && !vis[id]) {
            int idx=buscar_indice_atleta_por_id(a, na, id);
            if(idx!=-1 && a[idx].peso>0) { s+=a[idx].peso; c++; vis[id]=1; }
        }
    }
    double med = (c>0) ? s/c : 0;
    if(c>0) printf("   Media: %.2f kg\n", med); else printf("   Sem dados.\n");

    char* l[]={"Peso Ouro"}; double v[]={med};
    gerar_grafico_barra("q21_peso_ouro.svg", l, v, 1, "Peso Ouro", "kg");
    free(vis);
}

// Q22
void q22_ranking_paises_medalhistas_unicos(Atleta* a, int na, Resultado* r, int nr, NocRegiao* re, int nre) {
    char p[10][10]; int c[10];

    printf("\n=== Q22: RANKING MEDALHISTAS UNICOS ===\nDigite 10 siglas:\n");
    for(int i=0; i<10; i++) { printf("Pais %d: ", i+1); scanf("%s", p[i]); c[i]=0; }

    char** vis = (char**)malloc(10 * sizeof(char*));
    for(int k=0; k<10; k++) vis[k] = (char*)calloc(MAXIMO_ATLETAS, 1);

    for(int i=0; i<nr; i++) {
        if(strlen(r[i].medalha)>0 && strcmp(r[i].medalha,"NA")!=0) {
            for(int k=0; k<10; k++) {
                if(strcmp(r[i].noc, p[k])==0) {
                    if(r[i].id_atleta>=0 && r[i].id_atleta<MAXIMO_ATLETAS) {
                        if(!vis[k][r[i].id_atleta]) {
                            vis[k][r[i].id_atleta] = 1;
                            c[k]++;
                        }
                    }
                    break;
                }
            }
        }
    }

    for(int i=0; i<9; i++) for(int j=i+1; j<10; j++) if(c[j]>c[i]) {
        int t=c[i]; c[i]=c[j]; c[j]=t;
        char ts[10]; strcpy(ts,p[i]); strcpy(p[i],p[j]); strcpy(p[j],ts);
    }

    double v[10]; char* l[10];
    for(int i=0; i<10; i++) {
        printf("#%d %s: %d\n", i+1, p[i], c[i]);
        v[i]=c[i]; l[i]=p[i];
    }
    gerar_grafico_barra("q22_rank_unicos.svg", l, v, 10, "Med Unicos", "Qtd");

    for(int k=0; k<10; k++) free(vis[k]);
    free(vis);
}
