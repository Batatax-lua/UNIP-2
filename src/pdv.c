#define NOGDI
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "../headers/pdv.h"
#include "../headers/produtos.h"
#include "../headers/utils.h"
#include "../headers/clientes.h"

// Inicializa o cart com valor de contagem 0
void iniciarCart(Cart* cart) {
    cart->count = 0;
}

// Retira a quantidade do produto inserido do estoque
void retirarEstoque(int id, float quantity) {
    FILE *produtosFile = fopen("data/produtos.dat", "rb+");
    if (!produtosFile) {
        perror("\a\n\033[79C\033[31mErro ao abrir produtos.dat\033[30m");
        return;
    }

    Produto produto;
    while (fread(&produto, sizeof(Produto), 1, produtosFile)) {
        if (produto.id == id) {
            produto.quantidade -= quantity; // Subtrai do estoque
            fseek(produtosFile, - (long)sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, produtosFile);
            break;
        }
    }
    fclose(produtosFile);
}

// Função que devolve uma quantidade de um produto ao estoque
void devolverEstoque(int id, float quantity) {
    // Abre o arquivo produtos.dat em modo de leitura e escrita binária
    FILE *produtosFile = fopen("data/produtos.dat", "rb+");
    if (!produtosFile) {
        // Exibe uma mensagem de erro caso o arquivo não possa ser aberto
        perror("\a\n\033[79C\033[31mErro ao abrir produtos.dat\033[30m");
        return;
    }

    Produto produto;
    // Lê o arquivo produto por produto até encontrar o correspondente ao ID fornecido
    while (fread(&produto, sizeof(Produto), 1, produtosFile)) {
        if (produto.id == id) {
            // Adiciona a quantidade devolvida ao estoque do produto
            produto.quantidade += quantity;
            // Retorna o ponteiro do arquivo para a posição correta para sobrescrever o produto
            fseek(produtosFile, - (long)sizeof(Produto), SEEK_CUR);
            // Atualiza os dados do produto no arquivo
            fwrite(&produto, sizeof(Produto), 1, produtosFile);
            break;
        }
    }
    // Fecha o arquivo
    fclose(produtosFile);
}

// Função que adiciona um produto ao carrinho
int adicionarProdutoCart(Cart* cart, Produto* produto, float quantidade) {
    FILE *produtosFile = fopen("data/produtos.dat", "rb+"); // Abre o arquivo de produtos
    // Verifica se o carrinho atingiu o tamanho máximo permitido
    if (cart->count >= MAX_CART_SIZE) {
        printf("\a\n\033[81C\033[31mO carrinho está cheio!\033[30m\n");
        return 0;
    }

    // Verifica se há estoque suficiente do produto
    if (produto->quantidade < quantidade) {
        printf("\a\n\033[63C\033[31mQuantidade insuficiente em estoque para o produto %s\033[30m\n", produto->nome);
        return 0;
    }

    // Procura o produto no carrinho
    for (int i = 0; i < cart->count; i++) {
        if (cart->produtos[i].id == produto->id) {
            if (produto->quantidade >= quantidade) {
                // Adiciona a quantidade ao carrinho e atualiza o estoque
                cart->quantidade[i] += quantidade;
                strcpy(cart->tipoProduto[i], produto->tipo);
                retirarEstoque(produto->id, quantidade);
                printf("\n\033[35C\033[32m%s adicionado ao carrinho. Quantidade total: %.1f un., Quantidade restante no estoque: %.1f un.\n", produto->nome, cart->quantidade[i], produto->quantidade);
            } else {
                printf("\a\n\033[66C\033[31mQuantidade insufience em estoque para adicionar mais.\033[30m\n");
            }
            return 1;
        }
    }
    
    // Adiciona o produto ao carrinho
    cart->produtos[cart->count] = *produto;
    cart->quantidade[cart->count] = quantidade;
    strcpy(cart->tipoProduto[cart->count], produto->tipo);
    cart->count++;
    retirarEstoque(produto->id, quantidade);
    printf("\n\033[35C\033[32m%s adicionado ao carrinho. Quantidade total: %.1f un., Quantidade restante no estoque: %.1f un.\033[30m\n", produto->nome, quantidade, produto->quantidade);
   
    fclose(produtosFile); // Fecha o arquivo
    return 1;
}

// Função que remove um produto do carrinho
int removerProdutoCart(Cart* cart, int produtoId) {
    // Procura o produto pelo ID no carrinho
    for (int i = 0; i < cart->count; i++) {
        if (cart->produtos[i].id == produtoId) {
            Produto* produto = &cart->produtos[i];
            devolverEstoque(produto->id, cart->quantidade[i]); // Devolve ao estoque
            cart->count--; // Reduz a contagem de itens no carrinho
            printf("\033[78C\033[32mProduto removido do carrinho.\033[30m\n");
            return 1;
        } else {
            printf("\033[75C\033[31mProduto não encontrado no carrinho.\033[30m\n");
        }
    }
    return 0;
}

// Função que cancela uma compra e devolve os itens ao estoque
void cancelarCompra(Cart* cart) {
    // Percorre o carrinho devolvendo os itens ao estoque
    for (int i = 0; i < cart->count; i++) {
        Produto* produto = &cart->produtos[i];
        printf("\n\033[71C\033[32mQuantidade de %s devolvida ao estoque: %.1f\033[30m\n", produto->nome, cart->quantidade[i]);
        devolverEstoque(produto->id, cart->quantidade[i]);
    }
    cart->count = 0; // Zera o carrinho
}

// Função que busca um produto pelo ID no arquivo
Produto* buscarProdutoPorID(int id) {
    FILE *produtosFile = fopen("data/produtos.dat", "rb");
    if (!produtosFile) {
        perror("\a\n\033[79C\033[31mErro ao abrir produtos.dat\033[30m");
        return NULL;
    }

    Produto produto;
    while (fread(&produto, sizeof(Produto), 1, produtosFile)) {
        if (produto.id == id) {
            fclose(produtosFile);
            Produto* result = malloc(sizeof(Produto));
            *result = produto;
            return result; // Retorna o produto encontrado
        }
    }
    fclose(produtosFile);
    printf("\a\n\033[75C\033[31mProduto com o ID %d não encontrado.\033[30m\n", id);
    return NULL;
}

// Função que busca um produto pelo nome no arquivo
Produto* buscarProdutoPorNome(const char* name) {
    FILE *produtosFile = fopen("data/produtos.dat", "rb");
    if (!produtosFile) {
        perror("\a\n\033[79C\033[31mErro ao abrir produtos.dat\033[30m");
        return NULL;
    }

    char lowerNome[50];
    strncpy(lowerNome, name, sizeof(lowerNome));
    lowerNome[sizeof(lowerNome) - 1] = '\0';
    toLowerStr(lowerNome); // Converte para letras minúsculas

    Produto produto;
    char lowerProdutoNome[50];

    while (fread(&produto, sizeof(Produto), 1, produtosFile)) {
        strncpy(lowerProdutoNome, produto.nome, sizeof(lowerProdutoNome));
        lowerProdutoNome[sizeof(lowerProdutoNome) - 1] = '\0';
        toLowerStr(lowerProdutoNome);

        if (strcmp(lowerProdutoNome, lowerNome) == 0) {
            fclose(produtosFile);
            Produto* result = malloc(sizeof(Produto));
            *result = produto;
            return result; // Retorna o produto encontrado
        }
    }
    fclose(produtosFile);
    printf("\a\n\033[74C\033[31mProduto com o nome %s não encontrado.\033[30m\n", name);
    return NULL;
}

void salvarNota(Cart* cart) {
    FILE *datFile = fopen("data/recibos.dat", "a+b");
    FILE *idxFile = fopen("data/recibos.idx", "a+b");
    if (!datFile) {
        printf("\a\n\033[79C\033[31mErro ao abrir recibos.dat\n\033[30m");
        return;
    }

    Recibo recibo;
    double produtoTotal = 0;
    int cpf[11] = {0};
    int cnpj[14] = {0};
    double total = 0;
    char op;
    Cliente* clientePtr = NULL;

    fseek(datFile, 0, SEEK_END);
    long size = ftell(datFile);

    if (size > 0) {
        fseek(datFile, size - sizeof(Recibo), SEEK_SET);
        fread(&recibo, sizeof(Recibo), 1, datFile);
        recibo.id++;
    } else {
        recibo.id = 1;
    }

    printf("\n\033[73C╔═════════════════════════════════════╗\n");
    printf("\033[73C║          Registro de Compra         ║\n");
    printf("\033[73C╠═════════════════════════════════════╣\n");
    printf("\033[73C║ O cliente possui cadastro? (s/n):   ║\n");
    printf("\033[73C╚═════════════════════════════════════╝\n\n");
    printf("\033[3A\033[110G");
    scanf(" %c", &op);
    clrBuffer();

    char opSemCadastro;
    char opMsg;
    long long cpfLong;
    long long cnpjLong;

    memset(recibo.cliente, 0, sizeof(recibo.cliente));
    memset(recibo.cpf, 0, sizeof(recibo.cpf));
    memset(recibo.cnpj, 0, sizeof(recibo.cnpj));

    if (op == 's' || op == 'S') {
        recibo.documentoNaNota  =  true;
        printf("\033[2B");
        printf("\033[63C╔══════════════════════════════════════════════════════════╗\n");
        printf("\033[63C║                    Cadastro de Cliente                   ║\n");
        printf("\033[63C╠══════════════════════════════════════════════════════════╣\n");
        printf("\033[63C║  Nome completo do cliente conforme registrado no sistema ║\n");
        printf("\033[63C║ Nome: \033[30;46m                                                  \033[30;47m ║\n");
        printf("\033[63C╠══════════════════════════════════════════════════════════╣\n");
        printf("\033[63C║    Documento do cliente conforme registrado no sistema   ║\n");
        printf("\033[63C║              Insira 0 no CPF caso seja CNPJ              ║\n");
        printf("\033[63C║ CPF: \033[30;46m           \033[30;47m                                         ║\n");
        printf("\033[63C║ CNPJ: \033[30;46m              \033[30;47m                                     ║\n");
        printf("\033[63C╚══════════════════════════════════════════════════════════╝\n\n");

        printf("\033[8A\033[72G\033[30;46m");
        fgets(recibo.cliente, sizeof(recibo.cliente), stdin);
        recibo.cliente[strcspn(recibo.cliente, "\n")] = 0;

        printf("\033[3B\033[71G");
        scanf("%lld", &cpfLong);
        clrBuffer();
        if (cpfLong == 0) {
            memset(recibo.cpf, 0, sizeof(recibo.cpf));
            printf("\033[72G");
            scanf("%lld", &cnpjLong);
            clrBuffer();
            for (int i = 13; i >= 0; i--) {
                recibo.cnpj[i] = cnpjLong % 10;
                cnpjLong /= 10;
            }
        } else if (cpfLong != 0) {
            for (int i = 10; i >= 0; i--) {
                recibo.cpf[i] = cpfLong % 10;
                cpfLong /= 10;
            }
        }
        printf("\033[30;47m\033[4B");
    
        clientePtr = buscarCliente(recibo.cliente, recibo.cpf, recibo.cnpj);
        if (clientePtr != NULL) {
            strcpy(recibo.cliente, clientePtr->nome);
        }
    } else if (op == 'n' || op == 'N') {
        recibo.cliente[0] = '\0';
        memset(recibo.cliente, 0, sizeof(recibo.cliente));
        printf("\033[2B");
        printf("\033[62C╔══════════════════════════════════════════════════════════╗\n");
        printf("\033[62C║                     Documento na nota                    ║\n");
        printf("\033[62C╠══════════════════════════════════════════════════════════╣\n");
        printf("\033[62C║ Cliente deseja documento na nota? (s/n):                 ║\n");
        printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
        
        printf("\033[3A\033[106G");
        scanf(" %c", &opSemCadastro);
        clrBuffer();
        if (opSemCadastro == 's' || opSemCadastro == 'S') {
            recibo.documentoNaNota = true;
            printf("\033[2A");
            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
            printf("\033[62C║    Documento do cliente conforme registrado no sistema   ║\n");
            printf("\033[62C║              Insira 0 no CPF caso seja CNPJ              ║\n");
            printf("\033[62C║ CPF: \033[30;46m           \033[30;47m                                         ║\n");
            printf("\033[62C║ CNPJ: \033[30;46m              \033[30;47m                                     ║\n");
            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
            printf("\033[4A\033[70G\033[30;46m");
            scanf("%lld", &cpfLong);
            clrBuffer();
            if (cpfLong == 0) {
                memset(recibo.cpf, 0, sizeof(recibo.cpf));
                printf("\033[71G");
                scanf("%lld", &cnpjLong);
                clrBuffer();
                for (int i = 13; i >= 0; i--) {
                    recibo.cnpj[i] = cnpjLong % 10;
                    cnpjLong /= 10;
                }
            } else if (cpfLong != 0) {
                for (int i = 10; i >= 0; i--) {
                    recibo.cpf[i] = cpfLong % 10;
                    cpfLong /= 10;
                }
            }
            printf("\033[30;47m");
            printf("\n");
        }
    }

    printf("\033[2B");
    printf("\033[64C╔══════════════════════════════════════════════════════╗\n");
    printf("\033[64C║                   Mensagem na Nota                   ║\n");
    printf("\033[64C╠══════════════════════════════════════════════════════╣\n");
    printf("\033[64C║ Deseja inserir uma mensagem/frase na nota? (s/n):    ║\n");
    printf("\033[64C╚══════════════════════════════════════════════════════╝\n\n");
    printf("\033[3A\033[117G");
    scanf(" %c", &opMsg);
    clrBuffer();
    if (opMsg == 's' || opMsg == 'S') {
        printf("\033[2A");
        printf("\033[65G╠══════════════════════════════════════════════════════╣\n");
        printf("\033[64C║        Mensagem na Nota (Máximo 30 caracteres)       ║\n");
        printf("\033[64C║ Mensagem: \033[30;46m                              \033[30;47m             ║\n");
        printf("\033[64C╚══════════════════════════════════════════════════════╝\n");
        printf("\033[2A\033[77G\033[30;46m");
        fgets(recibo.mensagemCustom, sizeof(recibo.mensagemCustom), stdin);
        recibo.mensagemCustom[strcspn(recibo.mensagemCustom, "\n")] = 0;
        printf("\033[30;47m\n\033[84C\033[33mGerando recibo...\033[30m\n");
    } else {
        recibo.mensagemCustom[0] = '\0';
    }
    
    recibo.count = cart->count;
    for (int i = 0; i < cart->count; i++) {
        strcpy(recibo.tipoProduto[i], cart->tipoProduto[i]);
        recibo.cart[i] = cart->produtos[i].id;
        recibo.quantidade[i] = cart->quantidade[i];
        produtoTotal += cart->produtos[i].preco * cart->quantidade[i];
        total += produtoTotal;
    }

    if (recibo.cliente != NULL) {
        if (clientePtr != NULL) {
            printf("\n\n\033[66C\033[32mCliente encontrado no cadastro: ID %d - %s\n", clientePtr->id, clientePtr->nome);
            printf("\033[72CAplicando desconto de 10%% por cadastro...\033[30m\n");
            total *= 0.90;
            free(clientePtr);
        }
    }
    recibo.total = total;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    strftime(recibo.dataEmitido, sizeof(recibo.dataEmitido), "%d/%m/%Y %H:%M:%S", &tm);

    fseek(datFile, 0, SEEK_END);
    long position = ftell(datFile);
    fwrite(&recibo, sizeof(Recibo), 1, datFile);
    fflush(datFile);
    fwrite(&position, sizeof(long), 1, idxFile);
    fflush(idxFile);

    printNota();
    cart->count = 0;

    fclose(datFile);
    fclose(idxFile);
    system("cls");
}

void printNota() {
    FILE *datFile = fopen("data/recibos.dat", "rb");
    if (datFile == NULL) {
        printf("\a\n\033[79C\033[31mErro ao abrir recibos.dat\033[30m\n");
        return;
    }

    Recibo recibo;
    fseek(datFile, -(long)sizeof(Recibo), SEEK_END);
    fread(&recibo, sizeof(Recibo), 1, datFile);
    fclose(datFile);
    long position;

    system("cls");
    printf("\033[30;103m");
    printf("\033[2J");
    printf("\033[H");
    printf("\033[46C------------------------------------- Recibo de Compra --------------------------------------\n");
    printf("\033[82CCOMERCIO DE ALIMENTOS\n\033[80CDA TERRA HORTIFRUTI LTDA\n");
    printf("\033[75CAv. Inês Zistente, 69 - ASSIS - SP\n");
    printf("\033[80CCNPJ: 43.283.175/0001-82\n");
    printf("\033[84CIE: 13.000.312-0\n");
    if (recibo.mensagemCustom[0] != '\0') {
        printf("\33[75C%s\n", recibo.mensagemCustom);
    }
    printf("\033[46C---------------------------------------------------------------------------------------------\n");
    printf("\033[48C%s\n", recibo.dataEmitido);
    printf("\033[48CCCF: %02d\tCOO: %02d", recibo.id, recibo.id);
    printf("\n\033[86CCUPOM FISCAL\n");
    printf("\033[46CITEM   | CÓDIGO       | PRODUTO              | QTD. UN    | VL UNIT(R$)  | VL UNIT TOTAL (R$)\n");

    double total = 0;
    for (int i = 0; i < recibo.count; i++) {
        if (recibo.cart[i] != 0) {
            Produto *produtoPtr = buscarProdutoPorID(recibo.cart[i]);
            if (produtoPtr != NULL) {
                double itemTotal = produtoPtr->preco * recibo.quantidade[i];
                total += itemTotal;
                printf("\033[46C%02d     | %-12d | %-20s | %-10.1f X %-13.2f| %-10.2f\n",
                        i + 1,
                        produtoPtr->id,
                        produtoPtr->nome,
                        recibo.quantidade[i],
                        produtoPtr->preco,
                        itemTotal);
                free(produtoPtr);
            }
        }
    }

    printf("\033[46C---------------------------------------------------------------------------------------------\n");
    Cliente* cliente = buscarCliente(recibo.cliente, recibo.cpf, recibo.cnpj);
    if (cliente != NULL) {
        printf("\n\033[48C*Desconto de 10%% aplicado*\n");
        printf("\033[48CCliente cadastrado: %s", recibo.cliente);
        total *= 0.90;
        (cliente);
    }
    printf("\n\033[48CDocumento do consumidor: \n\033[48C");
    if (cliente != NULL) {
        if (cliente->documento == true) {
            printCPF(cliente->cpf);
        } else if (cliente->documento == false) {
            printCNPJ(cliente->cnpj);
        }
    } else if (cliente == NULL) {
        if (recibo.documentoNaNota == true) {
            if (recibo.cpf[0] != 0) {
                printCPF(recibo.cpf);
            } else if (recibo.cpf[0] == 0) {
                if (recibo.cnpj[0] != 0) {
                    printCNPJ(recibo.cnpj);
                } else {
                    printf("N/A\n");
                }
            }
        } else {
            printf("N/A\n");
        }
    }
    printf("\033[48CTOTAL  R$\033[39C%.2f\n", total);
    printf("\033[48CDinheiro\033[40C%.2f\n", total);

    printf("\033[48CIMPOSTOS  Valor=R$0.00\t Media=0%%\n");
    printf("\033[46C=============================================================================================\n");

    printf("\n\033[64CDigite qualquer tecla para confirmar e retornar ao menu.");
    getchar();
    printf("\033[30;47m");
    printf("\033[2J");
    printf("\033[H");
    system("cls");
}

void verRecibos() {
    FILE *datFile = fopen("data/recibos.dat", "a+b");
    FILE *idxFile = fopen("data/recibos.idx", "a+b");
    if (datFile == NULL || idxFile == NULL) {
        printf("\a\n\033[79C\033[31mErro ao abrir recibos.dat e/ou recibos.idx\033[30m\n");
        return;
    }

    system("cls");


    printf("\n\033[66C╔══════════════════════════════════════════════════╗\n");
    printf("\033[66C║ Exibindo todos os recibos de compras registrados ║\n");
    printf("\033[66C╚══════════════════════════════════════════════════╝\n\n");

    Recibo recibo;
    Cliente cliente;
    Produto produto;
    long position;

    rewind(idxFile);

    if (idxFile == NULL) {
        printf("\a\n\033[79C\033[31mErro ao abrir o arquivo .idx\033[30m\n");
        return;
    }

    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET);
        if (fread(&recibo, sizeof(Recibo), 1, datFile) == 1) {
            printf("\033[63C╔════════════════════╣ Recibo Nº %02d ╠═════════════════════╗\n", recibo.id);
            printf("\033[63C║ Data e Hora: %s \033[122G║\n", recibo.dataEmitido);

            printf("\033[63C║ Documento consumidor: \033[122G║\n");
            if (recibo.documentoNaNota == true) {
                if (recibo.cpf[0] != 0) {
                    printf("\033[63C║ "); printCPF(recibo.cpf); printf("\033[1A\033[122G║\n");
                } else if (recibo.cpf[0] == 0) {
                    if (recibo.cnpj[0] != 0) {
                        printf("\033[63C║ "); printCNPJ(recibo.cnpj); printf("\033[1A\033[122G║\n");
                    } else {
                        printf("\033[63C║ N/A\033[122G║\n");
                    }
                }
            } else {
                printf("\033[63C║ N/A\033[122G║\n");
            }

            Cliente* cliente = buscarCliente(recibo.cliente, recibo.cpf, recibo.cnpj);
            if (cliente != NULL) {
                printf("\033[63C║ Cliente: %s\033[122G║\n", recibo.cliente);
                printf("\033[63C║ Cliente cadastrado. \033[32mDesconto de 10%% aplicado.\033[30m\033[122G║\n");
            }

            printf("\033[63C║ Produtos:\033[122G║\n");
            for (int i = 0; i < recibo.count; i++) {
                if (recibo.cart[i] != 0) {
                    Produto *produtoPtr = buscarProdutoPorID(recibo.cart[i]);
                    if (produtoPtr != NULL) {
                        if (strcmp(recibo.tipoProduto[i], "fruta") == 0 || strcmp(recibo.tipoProduto[i], "Fruta") == 0 || strcmp(recibo.tipoProduto[i], "verdura") == 0 || strcmp(recibo.tipoProduto[i], "Verdura") == 0 || strcmp(recibo.tipoProduto[i], "legume") == 0 || strcmp(recibo.tipoProduto[i], "Legume") == 0) {
                            printf("\033[63C║ ID#%02d      %-20s Quantidade: %-10.1f\033[122G║\n", recibo.cart[i], produtoPtr->nome, recibo.quantidade[i]);
                        } else {
                             printf("\033[63C║ ID#%02d      %-20s Quantidade: %-10.0f\033[122G║\n", recibo.cart[i], produtoPtr->nome, recibo.quantidade[i]);
                        }
                        free(produtoPtr);
                    } else {
                        printf("\033[63C║ ID#%-6d Produto não encontrado\tQuantidade: %-10.1f\033[122G║\n", recibo.cart[i], recibo.quantidade[i]);
                    }
                }
            }
            if (recibo.mensagemCustom[0] != '\0') {
                printf("\033[63C║Mensagem do Vendedor: %s \033[122G║\n", recibo.mensagemCustom);
            }
            printf("\033[63C╠═════════════════════════════════════════════════════════╣\n");
            printf("\033[63C║ \033[12CSubtotal do recibo ID# %d: R$%.2f\033[122G║\n", recibo.id, recibo.total);
            printf("\033[63C╚═════════════════════════════════════════════════════════╝\n\n");
        }
    }

    printf("\n\033[63CDigite qualquer outra tecla para voltar ao menu principal.\n");
    getchar();
    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

void listarProdutosDisponiveis() {
    system("cls");
    FILE *datFile = fopen("data/produtos.dat", "r+b");
    if (datFile == NULL) {
        printf("\a\n\033[79C\033[31mErro ao abrir o produtos.dat\033[30m");
    }

    printf("\n\033[80C╔══════════════════════╗\n");
    printf("\033[80C║ Registro de Produtos ║\n");
    printf("\033[80C╚══════════════════════╝\n\n");

    long nextPosition;
    Produto produto;
    printf("\033[67C════╣ Listando todos os produtos disponíveis ╠════\n\n");

    printf("  ╔══════════╦═══════════════════════════════╦═════════════════════════╦════════════════════════════╦═════════════════════════╦══════════════════════════════════════════════════════╗\n");
    printf("  ║ ID       ║ Produto                       ║ Preço                   ║ Tipo                       ║ Estoque                 ║ Fornecedor                                           ║\n");
    printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");

    // Lê o index para exibir todas as entradas
    while (fread(&produto, sizeof(Produto), 1, datFile) == 1) {
        if (produto.quantidade >= 0.1) {
            if (strcmp(produto.tipo, "fruta") == 0 || strcmp(produto.tipo, "Fruta") == 0 || strcmp(produto.tipo, "verdura") == 0 || strcmp(produto.tipo, "Verdura") == 0 || strcmp(produto.tipo, "legume") == 0 || strcmp(produto.tipo, "Legume") == 0) {
                printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/kg: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.1f Kg. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
            } else {
                printf("  ║ ID#%02d    ║ Produto: %-20s ║ Preço/un: R$%-11.2f ║ Tipo: %-20s ║ Estoque: %-10.0f un. ║ Fornecedor: %-20s\033[182G║\n", produto.id, produto.nome, produto.preco, produto.tipo, produto.quantidade, produto.fornecedor);
            }
            if (produto.quantidade <= 20.0) {
                printf("  ║\033[31m          ╠> Aviso: Pouco estoque!\033[30m\033[46G║\033[72G║\033[101G\033[31m╠> Pouca qtd. em estoque\033[30m\033[127G║\033[182G║\n");
            }
            long currentPosition = ftell(datFile);
            if (fread(&nextPosition, sizeof(long), 1, datFile)) {
                printf("  ╠══════════╬═══════════════════════════════╬═════════════════════════╬════════════════════════════╬═════════════════════════╬══════════════════════════════════════════════════════╣\n");
            }
            fseek(datFile, currentPosition, SEEK_SET);
        }
    }
    printf("  ╚══════════╩═══════════════════════════════╩═════════════════════════╩════════════════════════════╩═════════════════════════╩══════════════════════════════════════════════════════╝\n");

    fclose(datFile);
}

void verCart(Cart *cart) {
    system("cls");
    float total = 0.0;

    printf("\n\033[81C╔═════════════════════╗\n");
    printf("\033[81C║ Carrinho de Compras ║\n");
    printf("\033[81C╚═════════════════════╝\n\n");

    if (cart->count == 0) {
        printf("\033[81CO carrinho está vazio.\n");
        return;
    }

    printf("\033[63C╔═══════╦══════════════════════╦════════════╦════════════╗\n");
    printf("\033[63C║ ID    ║ Produto              ║ Quantidade ║ Preço Qtd. ║\n");
    printf("\033[63C╠═══════╬══════════════════════╬════════════╬════════════╣\n");

    for (int i = 0; i < cart->count; i++) {
        Produto produto = cart->produtos[i];
        float quantidade = cart->quantidade[i];
        printf("\033[63C║ %02d    ║ %-20s ║ %-10.1f ║ R$%-10.2f \033[121G║\n", produto.id, produto.nome, quantidade, produto.preco);
        total += quantidade * produto.preco;
    }

    printf("\033[63C╠═══════╩══════════════════════╩════════════╩════════════╣\n");
    printf("\033[63C║ \033[19CSubtotal: R$%.2f \033[121G║\n", total);
    printf("\033[63C╚════════════════════════════════════════════════════════╝\n");
}