#ifndef FINANCEIRO_H
#define FINANCEIRO_H

#include "empregados.h"
#include "pdv.h"
#include "utils.h"

double calcularTotalSalarios();
double calcularTotalVendas();
void calcularDadosMensais(double *lucros, double *gastos, int meses);
bool criarGrafico(double *lucros, double *gastos, int meses, const char *localImg);
void criarAbrirGrafico();

#endif // FINANCEIRO_H