#ifndef CLIENTES_H
#define CLIENTES_H

typedef struct {
    int id;
    char nome[51];
    int cpf[11];
    int cnpj[14];
    bool documento; // Usando a bool true para o CPF e false para o CNPJ
    int celular[11];
} Cliente;

void cadastrarCliente(Cliente *cliente);
void verClientes();
void editarCliente();
Cliente* buscarCliente(const char* nome, const int cpf[], const int cnpj[]);

#endif // CLIENTES_H