#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/olimpiadas.h"

// Algumas estruturas auxiliares
typedef struct { char chave[100]; int valor; } ParChaveValor;
int comparar_decrescente(const void* a, const void* b) { return ((ParChaveValor*)b)->valor - ((ParChaveValor*)a)->valor; }

typedef struct { int id_atleta; int idade; } ParIdade;
int comparar_idade_decrescente(const void* a, const void* b) { return ((ParIdade*)b)->idade - ((ParIdade*)a)->idade; }
