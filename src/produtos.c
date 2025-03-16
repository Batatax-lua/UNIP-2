#define NOGDI
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../headers/produtos.h"
#include "../headers/utils.h"

void cadastrarProduto(Produto *produto) {
    FILE *datProdutos = fopen("data/produtos.dat", "a+b");
    FILE *idxProdutos = fopen("data/produtos.idx", "a+b");
    system("cls");
    printf("\n\033[80C╔══════════════════════╗\n");
    printf("\033[80C║ Registro de Produtos ║\n");
    printf("\033[80C╚══════════════════════╝\n\n");

    // Vai ao final da lista
    fseek(datProdutos, 0, SEEK_END);
    long size = ftell(datProdutos); // Puxa o tamanho

    if (size > 0) { // Verifica se existem produtos existentes
        // Puxa a posição do ultimo produto
        fseek(datProdutos, size - sizeof(Produto), SEEK_SET);
        fread(produto, sizeof(Produto), 1, datProdutos); // Lê o ultimo produto
        produto->id++; // Aumenta o ID
    } else {
        produto->id = 1; // Se não existe um produto inicia pelo 1
    }

    // Cadastra o nome do produto
    printf("\033[71C╔════════════════════════════════════════╗\n");
    printf("\033[71C║ Nome do produto (Máximo 20 caracteres) ║\n");
    printf("\033[71C╠════════════════════════════════════════╣\n");
    printf("\033[71C║ Nome: \033[30;46m                   \033[30;47m              ║\n");
    printf("\033[71C╚════════════════════════════════════════╝\n\n");
    
    printf("\033[71C╔════════════════════════════════════════╗\n");
    printf("\033[71C║ Tipo do produto (Máximo 15 caracteres) ║\n");
    printf("\033[71C╠════════════════════════════════════════╣\n");
    printf("\033[71C║ Tipo: \033[30;46m               \033[30;47m                  ║\n");
    printf("\033[71C╚════════════════════════════════════════╝\n\n");

    printf("\033[70G╔════════════════════════════════════════════╗\n");
    printf("\033[70G║ Preço do produto (Preço por unidade/quilo) ║\n");
    printf("\033[70G║   Use a vírgula (,) para separar o preço   ║\n");
    printf("\033[70G╠════════════════════════════════════════════╣\n");
    printf("\033[70G║ Preço: \033[30;46m      \033[30;47m                              ║\n");
    printf("\033[70G╚════════════════════════════════════════════╝\n\n");

    printf("\033[67G╔═══════════════════════════════════════════════════╗\n");
    printf("\033[67G║                 Estoque do produto                ║\n");
    printf("\033[67G║ Use a vírgula (,) para produtos medidos por quilo ║\n");
    printf("\033[67G╠═══════════════════════════════════════════════════╣\n");
    printf("\033[67G║ Estoque: \033[30;46m          \033[30;47m                               ║\n");
    printf("\033[67G╚═══════════════════════════════════════════════════╝\n\n");

    printf("\033[66G╔══════════════════════════════════════════════════════╗\n");
    printf("\033[66G║     Fornecedor do Produto (Máximo 40 caracteres)     ║\n");
    printf("\033[66G╠══════════════════════════════════════════════════════╣\n");
    printf("\033[66G║ Fornecedor: \033[30;46m                                        \033[30;47m ║\n");
    printf("\033[66G╚══════════════════════════════════════════════════════╝\n\n");

    // Cadastra o nome do produto
    printf("\033[30;46m\033[29A\033[80G");
    fgets(produto->nome, sizeof(produto->nome), stdin);
    produto->nome[strcspn(produto->nome, "\n")] = 0; // Tira a newline
    printf("\033[5B\033[80G");

    // Cadastra o tipo do produto
    

    fgets(produto->tipo, sizeof(produto->tipo), stdin);
    produto->tipo[strcspn(produto->tipo, "\n")] = 0;
    printf("\n\033[5B\033[79G");

    // Cadastra o preço do produto
    scanf("%f", &produto->preco);
    printf("\033[6B\033[78G");
    clrBuffer();
    
    // Cadastra a quantia para o estoque
    scanf("%f", &produto->quantidade);
    printf("\033[5B\033[80G");
    clrBuffer();

    // Cadastra o fornecedor
    fgets(produto->fornecedor, sizeof(produto->fornecedor), stdin);
    produto->fornecedor[strcspn(produto->fornecedor, "\n")] = 0;
    printf("\033[30;47m");

    // Salva o produto no .dat
    fseek(datProdutos, 0, SEEK_END); // Vai ao final do .dat
    long position = ftell(datProdutos); // Puxa a posição atual
    printf("\n\033[36C\033[33mSalvando produto: ID# %d, Nome: %s, Tipo: %s, Preço: %.2f, Estoque: %.1f un., Fornecedor: %s\033[30m\n", produto->id, produto->nome, produto->tipo, produto->preco, produto->quantidade, produto->fornecedor);
    fwrite(produto, sizeof(Produto), 1, datProdutos); // Anota a posição no .dat
    fflush(datProdutos); // Força anotação no arquivo

    // Salva o produto no .idx
    fwrite(&position, sizeof(long), 1, idxProdutos); // Anota a posição no .idx
    fflush(idxProdutos); // Força anotação no arquivo

    printf("\033[69C\033[32mO produto %s foi registrado com sucesso!\033[30m\n", produto->nome);
    tocarSomSucesso();
    sleep(2);
    system("cls");

    fclose(datProdutos);
    fclose(idxProdutos);
    return;
}

void verProdutos() {
    FILE *datProdutos = fopen("data/produtos.dat", "a+b");
    FILE *idxProdutos = fopen("data/produtos.idx", "a+b");

    system("cls");
    printf("\n\033[71C╔════════════════════════════════════════╗\n");
    printf("\033[71C║ Exibindo todos os produtos registrados ║\n");
    printf("\033[71C╚════════════════════════════════════════╝\n\n");

    long position;
    long nextPosition;
    Produto produto;

    rewind(idxProdutos);

    printf("  ╔══════════╦═══════════════════════════════╦═════════════════════════╦════════════════════════════╦═════════════════════════╦══════════════════════════════════════════════════════╗\n");
    printf("  ║ ID       ║ Produto                       ║ Preço                   ║ Tipo                       ║ Estoque                 ║ Fornecedor                                           ║\n");
    printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");

    // Lê o index para exibir todas as entradas
    while (fread(&position, sizeof(long), 1, idxProdutos)) {
        fseek(datProdutos, position, SEEK_SET); // Vai ao final do .dat
        fread(&produto, sizeof(Produto), 1, datProdutos); // Puxa as informações do produto
        if (strcmp(produto.tipo, "fruta") == 0 || strcmp(produto.tipo, "Fruta") == 0 || strcmp(produto.tipo, "verdura") == 0 || strcmp(produto.tipo, "Verdura") == 0 || strcmp(produto.tipo, "legume") == 0 || strcmp(produto.tipo, "Legume") == 0) {
            printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/kg: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.1f Kg. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
        } else {
            printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/un: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.0f un. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
        }
        if (produto.quantidade <= 20.0) {
            printf("  ║\033[31m          ╠> Aviso: Pouco estoque!\033[30m\033[46G║\033[72G║\033[101G\033[31m╠> Pouca qtd. em estoque\033[30m\033[127G║\033[182G║\n");
        }
        long currentPosition = ftell(idxProdutos);
        if (fread(&nextPosition, sizeof(long), 1, idxProdutos)) {
            printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");
        }
        fseek(idxProdutos, currentPosition, SEEK_SET);
    }
    printf("  ╚══════════╩═══════════════════════════════╩═════════════════════════╩════════════════════════════╩═════════════════════════╩══════════════════════════════════════════════════════╝\n");
    printf("\n\033[67CAperte qualquer tecla para voltar ao menu principal\n");
    getchar();
    system("cls");

    fclose(datProdutos);
    fclose(idxProdutos);
}

void editarProduto() {
    FILE *datFile = fopen("data/produtos.dat", "r+b");
    FILE *idxFile = fopen("data/produtos.idx", "r+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    long position;
    long nextPosition;
    Produto produto;
    int produtoId;
    int encontrado = 0;
    long produtoOffset = -1;

    system("cls");
    
    printf("\n\033[79C╔═════════════════════════╗\n");
    printf("\033[79C║ Atualização de produtos ║\n");
    printf("\033[79C╚═════════════════════════╝\n\n");

    printf("\033[74C════╣ Listando todos os produtos ╠════\n");
    printf("  ╔══════════╦═══════════════════════════════╦═════════════════════════╦════════════════════════════╦═════════════════════════╦══════════════════════════════════════════════════════╗\n");
    printf("  ║ ID       ║ Produto                       ║ Preço                   ║ Tipo                       ║ Estoque                 ║ Fornecedor                                           ║\n");
    printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");
    rewind(idxFile);
    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET);
        if (fread(&produto, sizeof(Produto), 1, datFile) == 1) {
            if (strcmp(produto.tipo, "fruta") == 0 || strcmp(produto.tipo, "Fruta") == 0 || strcmp(produto.tipo, "verdura") == 0 || strcmp(produto.tipo, "Verdura") == 0 || strcmp(produto.tipo, "legume") == 0 || strcmp(produto.tipo, "Legume") == 0) {
                printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/kg: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.1f Kg. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
            } else {
                printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/un: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.0f un. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
            }
            long currentPosition = ftell(idxFile);
            if (fread(&nextPosition, sizeof(long), 1, idxFile)) {
                printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");
            }
            fseek(idxFile, currentPosition, SEEK_SET);
        } else {
            printf("\a\033[31m  ╠> Erro ao ler dados do produto. \033[182G║\033[30m\n");
        }
    }

    printf("  ╚══════════╩═══════════════════════════════╩═════════════════════════╩════════════════════════════╩═════════════════════════╩══════════════════════════════════════════════════════╝\n");
    printf("\n\033[71CDigite o ID do produto que deseja editar: ");
    scanf("%d", &produtoId);
    clrBuffer();

    rewind(datFile);
    rewind(idxFile);

    while (fread(&position, sizeof(long), 1, idxFile)) {
        produtoOffset++;
        fseek(datFile, position, SEEK_SET);
        if (fread(&produto, sizeof(Produto), 1, datFile) == 1) {
            if (produto.id == produtoId) {
                encontrado = 1;
                break;
            }
        }
    }

    if (!encontrado) {
        printf("\a\033[81C\033[31mProduto não encontrado.\033[30m\n");
        fclose(datFile);
        fclose(idxFile);
        sleep(2);
        system("cls");
        return;
    } else {
        printf("\033[29C\033[32mProduto encontrado: %s com ID %02d, preço %.2f, tipo %s e fornecedor %s com %.0f unidade(s) em estoque.\033[30m\n", produto.nome, produto.id, produto.preco, produto.tipo, produto.fornecedor, produto.quantidade);
    }

    int op;

    printf("\n\033[70C╔═══════════════════════════════════════════╗\n");
    printf("\033[70C║             Edição de Produto             ║\n");
    printf("\033[70C╠═══════════════════════════════════════════╣\n");
    printf("\033[70C║ 1) Nome                                   ║\n");
    printf("\033[70C║ 2) Preço                                  ║\n");
    printf("\033[70C║ 3) Tipo                                   ║\n");
    printf("\033[70C║ 4) Fornecedor                             ║\n");
    printf("\033[70C║ 5) Estoque                                ║\n");
    printf("\033[70C║ -1) Cancelar e retornar ao menu principal ║\n");
    printf("\033[70C╚═══════════════════════════════════════════╝\n");
    printf("\033[82CEscolha uma opção: ");
    scanf("%d", &op);
    clrBuffer();

    switch(op) {
        char novoNome[20];
        float novoPreco;
        char novoTipo[15];
        char novoFornecedor[50];
        float novoEstoque;
        char confirmar;

        case 1:
            printf("\n\033[62C╔═════════════════════════════════════════════════════════════╗\n");
            printf("\033[62C║ Novo nome para o produto selecionado (Máximo 20 caracteres) ║\n");
            printf("\033[62C║ Nome: \033[30;46m                    \033[30;47m                                  ║\n");
            printf("\033[62C╠═════════════════════════════════════════════════════════════╣\n");
            printf("\033[62C║ Deseja confirmar o novo nome para o produto? (s/n):         ║\n");
            printf("\033[62C╚═════════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[71G\033[30;46m");
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = 0;
            printf("\033[30;47m\033[1B\033[117G");

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                strcpy(produto.nome, novoNome);
                printf("\n\033[74C\033[32mNome do produto editado com sucesso.\033[30m\n");
                tocarSomSucesso();
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[56C\033[31mEdição cancelada. Nome do produto continua sem alteração.\033[30m\n");
                break;
            } else {
                printf("\a\n\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                return;
            }

        case 2:
            printf("\033[61C╔═════════════════════════════════════════════════════════════╗\n");
            printf("\033[61C║   Novo preço para o produto selecionado (Preço por un/kg)   ║\n");
            printf("\033[61C║            Use a vírgula (,) para separar o preço           ║\n");
            printf("\033[61C║ Preço: \033[30;46m      \033[30;47m                                               ║\n");
            printf("\033[61C╠═════════════════════════════════════════════════════════════╣\n");
            printf("\033[61C║ Deseja confirmar o novo preço para o produto? (s/n):        ║\n");
            printf("\033[61C╚═════════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[71G\033[30;46m");
            scanf("%f", &novoPreco);
            printf("\033[30;47m\033[1B\033[117G");
            clrBuffer();

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                produto.preco = novoPreco;
                printf("\n\033[73C\033[32mPreço do produto editado com sucesso.\033[30m\n");
                tocarSomSucesso();
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[63C\033[31mEdição cancelada. Preço do produto continua sem alteração.\033[30m\n");
                break;
            } else {
                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                return;
            }
            break;

        case 3:
            printf("\n\033[62C╔═════════════════════════════════════════════════════════════╗\n");
            printf("\033[62C║ Novo tipo para o produto selecionado (Máximo 15 caracteres) ║\n");
            printf("\033[62C║ Tipo: \033[30;46m               \033[30;47m                                       ║\n");
            printf("\033[62C╠═════════════════════════════════════════════════════════════╣\n");
            printf("\033[62C║ Deseja confirmar o novo tipo para o produto? (s/n):         ║\n");
            printf("\033[62C╚═════════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[71G\033[30;46m");
            fgets(novoTipo, sizeof(novoTipo), stdin);
            novoTipo[strcspn(novoTipo, "\n")] = 0;
            printf("\033[30;47m\033[1B\033[117G");

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                strcpy(produto.tipo, novoTipo);
                printf("\n\033[74C\033[32mTipo do produto editado com sucesso.\033[30m\n");
                tocarSomSucesso();
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[56C\033[31mEdição cancelada. Tipo do produto continua sem alteração.\033[30m\n");
                break;
            } else {
                printf("\a\n\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                return;
            }
            break;

        case 4:
            printf("\n\033[62C╔═══════════════════════════════════════════════════════════════════╗\n");
            printf("\033[62C║ Novo fornecedor para o produto selecionado (Máximo 20 caracteres) ║\n");
            printf("\033[62C║ Fornecedor: \033[30;46m                    \033[30;47m                                  ║\n");
            printf("\033[62C╠═══════════════════════════════════════════════════════════════════╣\n");
            printf("\033[62C║ Deseja confirmar o novo fornecedor para o produto? (s/n):         ║\n");
            printf("\033[62C╚═══════════════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[77G\033[30;46m");
            fgets(novoFornecedor, sizeof(novoFornecedor), stdin);
            novoFornecedor[strcspn(novoFornecedor, "\n")] = 0;
            printf("\033[30;47m\033[1B\033[123G");

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                strcpy(produto.fornecedor, novoFornecedor);
                printf("\n\033[71C\033[32mFornecedor do produto editado com sucesso.\033[30m\n");
                tocarSomSucesso();
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[61C\033[31mEdição cancelada. Fornecedor do produto continua sem alteração.\033[30m\n");
                break;
            } else {
                printf("\a\n\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                return;
            }
            break;

        case 5:
            printf("\033[61C╔════════════════════════════════════════════════════════════════════════╗\n");
            printf("\033[61C║                 Novo estoque para o produto selecionado                ║\n");
            printf("\033[61C║            Use a vírgula (,) para produtos medidos por quilo           ║\n");
            printf("\033[61C║ Estoque: \033[30;46m          \033[30;47m                                                    ║\n");
            printf("\033[61C╠════════════════════════════════════════════════════════════════════════╣\n");
            printf("\033[61C║ Deseja confirmar o novo estoque para o produto? (s/n):                 ║\n");
            printf("\033[61C╚════════════════════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[73G\033[30;46m");
            scanf("%f", &novoEstoque);
            printf("\033[30;47m\033[1B\033[119G");
            clrBuffer();

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                produto.quantidade = novoEstoque;
                printf("\n\033[65C\033[32mQuantidade do produto em estoque alterada com sucesso.\033[30m\n");
                tocarSomSucesso();
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[62C\033[31mEdição cancelada. Estoque do produto continua sem alteração.\033[30m\n");
                break;
            } else {
                printf("\a\n\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                return;
            }
            break;

        case -1:
            printf("\n\033[78C\033[31mRetornando ao menu principal.\033[30m\n");
            break;
        default:
            printf("\a\n\033[76C\033[31mOpção inválida. Tente novamente.\033[30m\n");
            return;
    }

    fseek(datFile, produtoOffset * sizeof(Produto), SEEK_SET);
    fwrite(&produto, sizeof(Produto), 1, datFile);

    system("cls");
    
    fclose(datFile);
    fclose(idxFile);
}