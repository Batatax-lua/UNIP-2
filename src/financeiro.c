#define NOGDI
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "../headers/empregados.h"
#include "../headers/pdv.h"
#include "../headers/utils.h"
#include "../include/pbPlots.h"
#include "../include/supportLib.h"

double calcularTotalSalarios() {
    FILE *datFile = fopen("data/empregados.dat", "rb");
    if (!datFile) {
        printf("\a\033[31mErro ao abrir empregados.dat. Garanta que já existe ao menos um empregado registrado.\033[30m");
        return 0.0;
    }

    double salarioTotal = 0.0;
    Empregado empregado;
    while (fread(&empregado, sizeof(Empregado), 1, datFile) == 1) {
        salarioTotal += empregado.salario;
    }

    fclose(datFile);
    return salarioTotal;
}

double calcularTotalVendas() {
    FILE *datFile = fopen("data/recibos.dat", "rb");
    if (!datFile) {
        printf("\a\033[31mErro ao abrir recibos.dat. Garanta que já existe ao menos um recibo registrado.\033[30m");
        return 0.0;
    }

    double vendasTotal = 0.0;
    Recibo recibo;
    while (fread(&recibo, sizeof(Recibo), 1, datFile) == 1) {
            vendasTotal += recibo.total;
    }

    fclose(datFile);
    return vendasTotal;
}

void calcularDadosMensais(double *lucros, double *gastos, int meses) {
    // Data pré-montada para setembro e outubro
    // setembro
    lucros[0] = 4500;
    gastos[0] = 4600;

    // outubro
    lucros[1] = 7700;
    gastos[1] = 4870;

    // atual (novembro)
    lucros[2] = calcularTotalVendas();
    gastos[2] = calcularTotalSalarios();
}

bool criarGrafico(double *lucros, double *gastos, int meses, const char *localImg) {
    _Bool sucesso;
    StringReference *msgErro = CreateStringReference(L"", 0);
    RGBABitmapImageReference *imgRef = CreateRGBABitmapImageReference();

    // Define as cores do gráfico
    BarPlotSettings *config = GetDefaultBarPlotSettings();
    config->width = 800;
    config->height = 600;
    config->autoLabels = false;
    config->title = L"Lucros e Gastos mensais";
    config->titleLength = wcslen(config->title);
    config->yLabel = L"Valores (R$)";
    config->yLabelLength = wcslen(config->yLabel);
    config->yMin = 0;
    config->yMax = 10000;
    config->showGrid = true;
    config->autoColor = false;

    config->xLabels = (StringReference **)calloc(sizeof(StringReference *) * meses, 1);
    config->xLabelsLength = 3;
    config->xLabels[0] = CreateStringReference(strparam(L"Set 24"));
    config->xLabels[1] = CreateStringReference(strparam(L"Out 24"));
    config->xLabels[2] = CreateStringReference(strparam(L"Nov 24"));

    BarPlotSeries *seriesLucros = GetDefaultBarPlotSeriesSettings();
    seriesLucros->ys = lucros;
    seriesLucros->ysLength = meses;
    seriesLucros->color = CreateRGBColor(0, 1, 0);

    BarPlotSeries *seriesGastos = GetDefaultBarPlotSeriesSettings();
    seriesGastos->ys = gastos;
    seriesGastos->ysLength = meses;
    seriesGastos->color = CreateRGBColor(1, 0, 0);

    config->barPlotSeries = (BarPlotSeries **)malloc(2 * sizeof(BarPlotSeries *));
    config->barPlotSeries[0] = seriesLucros;
    config->barPlotSeries[1] = seriesGastos;
    config->barPlotSeriesLength = 2;

    sucesso = DrawBarPlotFromSettings(imgRef, config, msgErro);
    if (sucesso) {
        size_t pngLength;
        double *pngData = ConvertToPNG(&pngLength, imgRef->image);
        WriteToFile(pngData, pngLength, (char *)localImg);
        free(pngData);
        free(imgRef->image);
    }

    free(config);
    free(imgRef);
    return sucesso;
}

void criarAbrirGrafico() {
    int numMeses = 3;
    double lucros[numMeses];
    double gastos[numMeses];

    calcularDadosMensais(lucros, gastos, numMeses);

    const char *localImg = "GraficoLucroGasto.png";
    criarGrafico(lucros, gastos, numMeses, localImg);
    abrirImg(localImg);

    const char *meses[] = {
        "Setembro 2024",
        "Outubro 2024",
        "Novembro 2024"
    };

    for (int i = 0; i < numMeses; i++) {
        printf("\033[76C\033[32m Lucros %s: R$%.2f \n", meses[i], lucros[i]);
        printf("\033[76C\033[31m Gastos %s: R$%.2f \033[30m\n\n", meses[i], gastos[i]);
    }
}