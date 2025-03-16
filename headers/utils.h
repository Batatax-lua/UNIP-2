#ifndef UTILS_H
#define UTILS_H
#include <windows.h>

void clrBuffer();
void setarTamanhoFonte(int tamanhoFonte);
void setarTamanhoLayout(int tamX, int tamY);
void desenharLogo();
void desenharBoot();
void desenharLogoIntro();
void printCredito();
void toLowerStr(char* str);
long converterParaLong(const char* str);
void printCPF(const int cpf[]);
void printCNPJ(const int cnpj[]);
void printTelefone(const int celular[]);
void abrirImg(const char *localImg);
void tocarSom(const char* localSom);
void tocarSomSucesso();
void tocarMusica(const char* Musica);
void pularMusica();
DWORD WINAPI puxarKeybind(LPVOID lpParam);
void limparDadosClientes(const char *pastaDados);
void limparDadosProdutos(const char *pastaDados);
void limparDadosRecibos(const char *pastaDados);
void limparDadosEmpregados(const char *pastaDados);
void limparDadosGerais(const char *pastaDados);

#endif // UTILS_H