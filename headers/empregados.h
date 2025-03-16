#ifndef EMPREGADOS_H
#define EMPREGADOS_H

typedef struct {
    int id;
    char nome[51];
    int cpf[11];
    bool documento; // false para CNPJ e true para CPF
    int cnpj[14];
    int celular[11];
    char email[51];
    char dataNascimento[11];
    char endereco[51];
    char dataAdmissao[11];
    double salario;
    char setor[30];
    char status[15];
} Empregado;

void cadastrarEmpregado(Empregado *empregado);
void verEmpregados();
void editarEmpregado();

#endif // EMPREGADOS_H