#ifndef PRODUTOS_H
#define PRODUTOS_H

typedef struct {
    int id;
    char nome[21];
    char tipo[16];
    float preco;
    float quantidade;
    char fornecedor[41];
} Produto;

void cadastrarProduto(Produto *produto);
void verProdutos();
void editarProduto();

#endif // PRODUTOS_H