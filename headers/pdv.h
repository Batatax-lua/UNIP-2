#ifndef PDV_H
#define PDV_H

#include "produtos.h"

#define MAX_CART_SIZE 100

typedef struct {
    Produto produto;
    float quantidade;
} itemCart;

typedef struct {
    Produto produtos[MAX_CART_SIZE];
    char tipoProduto[MAX_CART_SIZE][15];
    float quantidade[MAX_CART_SIZE];
    int count; // Número de produtos no carrinho
} Cart;

typedef struct {
    int id;
    char cliente[51];
    int cart[MAX_CART_SIZE];
    float quantidade[MAX_CART_SIZE];
    int cpf[11];
    int cnpj[14];
    bool documentoNaNota;
    int count;
    char tipoProduto[MAX_CART_SIZE][15];
    double total;
    char dataEmitido[100];
    char mensagemCustom[51];
} Recibo;

void iniciarCart(Cart* cart);
void retirarEstoque(int id, float quantity);
void devolverEstoque(int id, float quantity);
int adicionarProdutoCart(Cart* cart, Produto* produto, float quantidade);
int removerProdutoCart(Cart* cart, int produtoId);
void cancelarCompra(Cart* cart);
void printNota();
void salvarNota(Cart* cart);
Produto* buscarProdutoPorID(int id);
Produto* buscarProdutoPorNome(const char* name);
void verRecibos();
void listarProdutosDisponiveis();
void verCart(Cart *cart);

#endif // PDV_H
