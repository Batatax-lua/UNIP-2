#define NOGDI // Exclui as definições da interface gráfica do dispositivo (GDI) para não haver conflito com o pbPlots
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include "../headers/clientes.h"
#include "../headers/produtos.h"
#include "../headers/empregados.h"
#include "../headers/utils.h"
#include "../headers/pdv.h"
#include "../headers/financeiro.h"
#include "../include/pbPlots.h"
#include "../include/supportLib.h"

// Estruturas

typedef struct {
    char usuario[20];
    char senha[20];
    int cargo;
} User;

// Tables
User users[] = {
    {"vendas1", "venda", 0},
    {"vendas2", "venda", 0},
    {"gerente1", "gerente", 1},
    {"gerente2", "gerente", 1},
    {"admin", "senhadmin", 2},
};

// Constantes
const int userCount = sizeof(users) / sizeof(users[0]);

const char* titulosCargos[] = {
    "Vendas", //cargo 0
    "Gerenciamento", //cargo 1
    "Admin" //cargo 2
};

User userLogado;

// Funções
unsigned int sleep(unsigned int seconds); // Função para pausar o programaa pelo tempo definido

void escondeSenha(char *senha) { // Função para esconder a senha e trocar por asteriscos
    int index = 0;
    int maxLen = 15;
    char ch;
    
    while (index < maxLen - 1) {
        ch = getch();

        if (ch == '\n' || ch == '\r') {
            break;
        } else if (ch == '\b' && index > 0) {
            printf("\b \b");
            index--;
        } else if (ch != '\b') {
            senha[index++] = ch;
            printf("*");
        }
    }

    senha[index] = '\0';
    printf("\n");
}

int login(char* inputUsuario, char* inputSenha) { // Função para realizar o login baseado na lista de usuário e senha
    for(int i = 0; i < userCount; i++) {
        if (strcmp(users[i].usuario, inputUsuario) == 0 &&
            strcmp(users[i].senha, inputSenha) == 0) {
                return users[i].cargo;
            }
    }
    return -1;
}

// Menu do Ponto de Vendas
void menuPDV() {
    int vendaOp; // Valor para utilizar no switch-case
    Cart cart; // Ponteiro para a struct Cart
    iniciarCart(&cart); // Inicializa o cart localizado em pdv.c
    system("cls");
    do {
        desenharLogo();
        printf("\n\033[74C╔══════════════════════════════════╗\n");
        printf("\033[74C║          Ponto de Vendas         ║\n");
        printf("\033[74C╠══════════════════════════════════╣\n");
        printf("\033[74C║ 1) Adicionar produto ao carrinho ║\n");
        printf("\033[74C║ 2) Remover produto do carrinho   ║\n");
        printf("\033[74C║ 3) Visualizar carrinho           ║\n");
        printf("\033[74C║ 4) Pesquisar e adicionar produto ║\n");
        printf("\033[74C║ 5) Finalizar pedido              ║\n");
        printf("\033[74C║ 6) Cancelar pedido               ║\n");
        printf("\033[74C║ -1) Voltar ao menu principal     ║\n");
        printf("\033[74C╚══════════════════════════════════╝\n");
        printf("\033[83CEscolha uma opção: ");
        scanf("%d", &vendaOp);
        clrBuffer(); // Função para limpar o buffer da entrada, garantindo que não haja erros por newline ou caracteres irrelevantes. Localizado em utils.c

        switch(vendaOp) {
            case 1:
                Produto *produtoPtr; // Ponteiro para a struct Produto
                float quantidade; // Float para definir a quantidade de um produto
                int produtoId; // Int para definir o ID de um produto
                listarProdutosDisponiveis(); // Lista todos os produtos cuja quantidade é maior que 0.1, localizado em pdv.c
                printf("\n\033[65CDigite o ID produto que deseja adicionar ao carrinho: \n");
                printf("\033[81CDigite a quantidade: ");
                printf("\033[1A\033[120G"); // Move o ponteiro acima 1 linha (#A) e para a coluna 120 (#G)
                scanf("%d", &produtoId);
                clrBuffer();
                printf("\033[103G");
                scanf("%f", &quantidade);
                clrBuffer();
                produtoPtr = buscarProdutoPorID(produtoId); // Seleciona o produto e verifica se existe, localizado em pdv.c
                if (produtoPtr == NULL) {
                    sleep(2);
                    system("cls");
                    break;
                } else {
                    adicionarProdutoCart(&cart, produtoPtr, quantidade); // Adiciona o produto ao cart com a quantidade
                    sleep(2);
                }
                free(produtoPtr); // Libera a memória do ponteiro do produto (Alocada em buscarProdutoPorID)
                system("cls");
                break;
            case 2:
                char op;
                verCart(&cart);
                printf("\n\033[64CDigite o ID do produto que deseja remover do carrinho: ");
                scanf("%d", &produtoId);
                clrBuffer();
                printf("\a\033[64CDeseja realmente \033[31mremover\033[30m o produto do carrinho? (s/n): ");
                scanf(" %c", &op);

                if (op == 's' || op == 'S') {
                    removerProdutoCart(&cart, produtoId); // Remove o produto do cart e devolve sua quantia ao estoque, localizado em pdv.c
                    printf("\033[70C\033[32mO produto de ID %d foi removido do carrinho.\033[30m\n", produtoId);
                    tocarSomSucesso(); // Função para tocar um som de sucesso, localizado em utils.c
                    sleep(2);
                    system("cls");
                    break;
                } else {
                    printf("\033[75C\033[31mCancelando a remoção do produto...\033[30m\n");
                    system("cls");
                    break;
                }
            case 3:
                verCart(&cart); // Exibe o carrinho na tela, localizado em pdv.c
                printf("\n\033[63CDigite qualquer outra tecla para voltar ao menu principal.\n");
                getchar();
                system("cls");
                break;
            case 4:
                char produtoNome[50];
                char buscaOp;
                Produto *result = NULL;

                printf("\n\033[66C╔══════════════════════════════════════════════════╗\n");
                printf("\033[66C║                 Busca de Produto                 ║\n");
                printf("\033[66C╠══════════════════════════════════════════════════╣\n");
                printf("\033[66C║ i) Busca por ID                                  ║\n");
                printf("\033[66C║ n) Busca por nome                                ║\n");
                printf("\033[66C╠══════════════════════════════════════════════════╣\n");
                printf("\033[66C║ Selecione seu método de busca:                   ║\n");
                printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                printf("\033[2A\033[100G");
                scanf(" %c", &buscaOp);
                getchar();
                if (buscaOp == 'i' || buscaOp == 'I') {
                    printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                    printf("\033[66C║ Digite o ID do produto que deseja buscar: \033[30;46m    \033[30;47m   ║\n");
                    printf("\033[66C╚══════════════════════════════════════════════════╝\n\n");
                    printf("\033[3A\033[111G\033[30;46m");
                    scanf("%d", &produtoId);
                    printf("\033[30;47m");
                    clrBuffer();
                    result = buscarProdutoPorID(produtoId); // Busca o produto do ID inserido e o retorna ao ponteiro result, localizado em pdv.c
                    if (result != NULL) { // Se o ponteiro não for nulo (existe)
                        printf("\n\033[77C\033[32mProduto encontrado: %s\033[30m\n", result->nome);
                        printf("\033[69CDeseja adicionar o produto ao carrinho? (s/n): ");
                        scanf(" %c", &buscaOp);
                        getchar();
                        if (buscaOp == 's' || buscaOp == 'S') {
                            printf("\033[68CDigite a quantidade para adicionar ao carrinho: ");
                            scanf("%f", &quantidade);
                            getchar();
                            adicionarProdutoCart(&cart, result, quantidade);
                            system("cls");
                        } else {
                            system("cls");
                            break;
                        }
                    } else { // Se o ponteiro for nulo (não existe)
                        printf("\a\033[60C\033[31mProduto de ID %02d não encontrado. Tente novamente.\033[30m\n", produtoId);
                        sleep(2);
                        system("cls");
                        break;
                    }
                } else if (buscaOp == 'n' || buscaOp == 'N') {
                    printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                    printf("\033[66C║    Digite o nome do produto que deseja buscar    ║\n");
                    printf("\033[66C║ Nome: \033[30;46m                    \033[30;47m                       ║\n");
                    printf("\033[66C╚══════════════════════════════════════════════════╝\n\n");
                    printf("\033[3A\033[75G\033[30;46m");
                    fgets(produtoNome, sizeof(produtoNome), stdin); // Função para ler a entrada do teclado como uma string
                    produtoNome[strcspn(produtoNome, "\n")] = 0; // Remove a newline para não gerar conflitos em leitura de entradas
                    printf("\033[30;47m");
                    result = buscarProdutoPorNome(produtoNome);
                    if (result != NULL) {
                        printf("\n\033[77C\033[32mProduto encontrado: %s\033[30m\n", result->nome);
                        printf("\033[69CDeseja adicionar o produto ao carrinho? (s/n): ");
                        scanf(" %c", &buscaOp);
                        getchar();
                        if (buscaOp == 's' || buscaOp == 'S') {
                            printf("\033[68CDigite a quantidade para adicionar ao carrinho: ");
                            scanf("%f", &quantidade);
                            getchar();
                            adicionarProdutoCart(&cart, result, quantidade);
                            system("cls");
                        } else {
                            system("cls");
                            break;
                        }
                    } else {
                        printf("\a\033[60C\033[31mProduto de nome %s não encontrado. Tente novamente.\033[30m\n", produtoNome);
                        sleep(2);
                        system("cls");
                        break;
                    }
                }
                break;
            case 5:
                salvarNota(&cart); // Inicia o processo de finalizar a compra, localizado em pdv.c
                for (int i = 0; i < cart.count; i++) {
                    retirarEstoque(cart.produtos[i].id, cart.quantidade[i]); // Retira a quantia de produtos no carrinho do estoque, localizado em pdv.c
                }
                break;
            case 6:
                char cancelOp;
                printf("\033[70CDeseja realmente cancelar o pedido? (s/n): ");
                scanf(" %c", &cancelOp);
                if (cancelOp == 's' || cancelOp == 'S') {
                    cancelarCompra(&cart); // Cancela a venda e retorna todos os produtos ao estoque, localizado em pdv.c
                    printf("\033[84C\033[31mCompra cancelada.\033[30m\n");
                    system("cls");
                } else {
                    printf("\033[85C\033[32mCompra retomada.\033[30m\n");
                    system("cls");
                }
                break;
            case -1:
                printf("\n\033[78C\033[31mVoltando ao menu de vendas...\033[30m\n");
                system("cls");
                break;
            default:
                system("cls");
                printf("\a\033[74C\033[31mOpção inválida! Tente novamente.\033[30m\n");
        } 
    } while (vendaOp != -1);
}

void funcVendas() {
    int opcao;
    int numProdutos;
    Cliente cliente;
    Produto produto;

    system("cls");
    do {
        desenharLogo();
        printf("\n\033[78C╔══════════════════════════╗\n");
        printf("\033[78C║      Menu de Vendas      ║\n");
        printf("\033[78C╠══════════════════════════╣\n");
        printf("\033[78C║ 1) Cadastrar Clientes    ║\n");
        printf("\033[78C║ 2) Ver Clientes          ║\n");
        printf("\033[78C║ 3) Registrar Pedido      ║\n");
        printf("\033[78C║ 4) Visualizar Pedidos    ║\n");
        printf("\033[78C║ 5) Defesa do Consumidor  ║\n");
        printf("\033[78C║ -1) Sair                 ║\n");
        printf("\033[78C╚══════════════════════════╝\n");
        printf("\033[83CEscolha uma opção: ");
        scanf("%d", &opcao);
        clrBuffer();

        switch(opcao) {
            case 1:
                cadastrarCliente(&cliente); // Inicia o processo de cadastro do cliente e envia ponteiro à função, localizado em clientes.c
                break;
            case 2:
                verClientes(); // Visualiza todos os clientes cadastrados, localizado em pdv.c
                break;
            case 3:
                menuPDV(); // Transfere o usuário para o menu do ponto de vendas, localizado logo acima dessa função
                break;
            case 4:
                verRecibos(); // Visualiza todos os recibos registrados, localizado em pdv.c
                break;
            case 5: 
                printf("\n\033[64C\033[33mAbrindo o QR Code para o Código de Defesa do Consumidor..\033[30m\n");
                abrirImg("QR_CodigoDefesaConsumidor.png"); // Usa a API do powershell do windows para abrir a foto do código QR, localizado em utils.c
                printf("\n\033[54CAlternativamente, link (Para copiar, selecione e aperte: CTRL + SHIFT + C): \n\033[47C\t\033[34mhttps://www.gov.br/mj/pt-br/assuntos/seus-direitos/consumidor/Anexos/cdc-portugues-2013.pdf \033[30m\n");
                printf("\n\n\033[73CPressione qualquer tecla para retornar.\n");
                getchar();
                system("cls");
                break;
            case -1:
                printf("\n\033[79CSaindo do menu de vendas...\n");
                system("cls");
                break;
            default:
                printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
                sleep(1);
                system("cls");
        }
    } while (opcao != -1);
}

void funcGestao() {
    int opcao;
    FILE *datProdutos = fopen("./data/produtos.dat", "a+b"); // Abre os arquivos idx e dat dos produtos no modo de acrescentar
    if (datProdutos == NULL) {
        perror("Erro ao abrir o arquivo .dat");
    }
    FILE *idxProdutos = fopen("./data/produtos.idx", "a+b");
    if (idxProdutos == NULL) {
        perror("Erro ao abrir o arquivo .idx");
    }

    system("cls");
    do {
        desenharLogo();
        printf("\n\033[75C╔═════════════════════════════════╗\n");
        printf("\033[75C║          Menu de Gestão         ║\n");
        printf("\033[75C╠═════════════════════════════════╣\n");
        printf("\033[75C║ 1) Cadastrar Cliente            ║\n");
        printf("\033[75C║ 2) Visualizar Clientes          ║\n");
        printf("\033[75C║ 3) Editar Cliente               ║\n");
        printf("\033[75C║ 4) Cadastrar Produto            ║\n");
        printf("\033[75C║ 5) Visualizar Produtos          ║\n");
        printf("\033[75C║ 6) Editar Produto               ║\n");
        printf("\033[75C║ 7) Visualizar Pedidos           ║\n");
        printf("\033[75C║ 8) Visualizar Fornecedores      ║\n");
        printf("\033[75C║ 9) Visualizar Lucro/Gasto       ║\n");
        printf("\033[75C║ 10) Visualizar Orçamento        ║\n");
        printf("\033[75C║ 11) Cadastrar Empregado         ║\n");
        printf("\033[75C║ 12) Visualizar Empregados       ║\n");
        printf("\033[75C║ 13) Defesa do Consumidor        ║\n");
        printf("\033[75C║ -1) Voltar ao menu principal    ║\n");
        printf("\033[75C╚═════════════════════════════════╝\n");
        printf("\033[83CEscolha uma opção: ");
        scanf("%d", &opcao);
        clrBuffer();

        switch(opcao) {
            case 1:
                Cliente cliente;
                cadastrarCliente(&cliente); // Inicia o processo de cadastro do cliente e envia ponteiro à função, localizado em clientes.c
                break;
            case 2:
                verClientes(); // Visualiza todos os clientes cadastrados, localizado em clientes.c
                break;
            case 3:
                editarCliente(); // Transporta o usuário ao menu de edição de clientes, localizado em clientes.c
                break;
            case 4:
                Produto produto; // Cria um ponteiro para a struct Produto
                cadastrarProduto(&produto); // Inicia o processo de cadastro do produto e envia ponteiro à função, localizado em produtos.c
                break;
            case 5:
                verProdutos(); // Visualiza todos os produtos cadastrados, localizado em produtos.c
                break;
            case 6:
                editarProduto(); // Transporta o usuário ao menu de edição de produtos, localizado em produtos.c
                break;
            case 7:
                verRecibos(); // Visualiza todos os recibos registrados, localizado em pdv.c
                break;
            case 8:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 9:
                printf("\033[74C\033[33mAbrindo gráfico de lucros e gastos..\033[30m\n");
                criarAbrirGrafico(); // Gera e abre o gráfico de lucro e gasto dos últimos 3 mêses, localizado em financeiro.c
                printf("// \n\033[73CPressione qualquer tecla para retornar.\n");
                getchar();
                system("cls");
                break;
            case 10:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 11:
                Empregado empregado; // Cria um ponteiro para a struct Empregado
                cadastrarEmpregado(&empregado); // Inicia o processo do cadastro de um empregado e passa o ponteiro à função, localizado em empregados.c
                break;
            case 12:
                verEmpregados(); // Visualiza todos os empregados registrados no sistema, localizado em empregados.c
                break;
            case 13:
                printf("\n\033[64C\033[33mAbrindo o QR Code para o Código de Defesa do Consumidor..\033[30m\n");
                abrirImg("QR_CodigoDefesaConsumidor.png");
                printf("\n\033[54CAlternativamente, link (Para copiar, selecione e aperte: CTRL + SHIFT + C): \n\033[47C\t\033[34mhttps://www.gov.br/mj/pt-br/assuntos/seus-direitos/consumidor/Anexos/cdc-portugues-2013.pdf \033[30m\n");
                printf("\n\n\033[73CPressione qualquer tecla para retornar.\n");
                getchar();
                system("cls");
                break;
            case -1:
                printf("\n\033[79C\033[31mSaindo do menu de gestão...\033[30m\n");
                break;
            default:
                printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
                sleep(2);
                system("cls");
        }
    } while (opcao != -1);
}

void funcAdmin() {
    int opcao;

    do {
        system("cls");
        desenharLogo();
        printf("\n\033[61C╔════════════════════════════════════════════════════════════╗\n");
        printf("\033[61C║                    Menu de Administração                   ║\n");
        printf("\033[61C╠══════════════════════════════╦═════════════════════════════╣\n");
        printf("\033[61C║ 1) Cadastrar Cliente         ║ 10) Visualizar Fornecedores ║\n");
        printf("\033[61C║ 2) Visualizar Clientes       ║ 11) Visualizar Lucro/Gasto  ║\n");
        printf("\033[61C║ 3) Editar Cliente            ║ 12) Gerenciar Orçamento     ║\n");
        printf("\033[61C║ 4) Cadastrar Produto         ║ 13) Visualizar Orçamento    ║\n");
        printf("\033[61C║ 5) Visualizar Produtos       ║ 14) Cadastrar Empregado     ║\n");
        printf("\033[61C║ 6) Editar Produto            ║ 15) Gerenciar Empregado     ║\n");
        printf("\033[61C║ 7) Registrar Pedido          ║ 16) Visualizar Empregados   ║\n");
        printf("\033[61C║ 8) Visualizar Pedidos        ║ 17) Apagar Registros        ║\n");
        printf("\033[61C║ 9) Gerenciar Fornecedores    ║ 18) Defesa do Consumidor    ║\n");
        printf("\033[61C║ -1) Voltar ao menu principal ║                             ║\n");
        printf("\033[61C╚══════════════════════════════╩═════════════════════════════╝\n");
        printf("\033[83CEscolha uma opção: ");
        scanf("%d", &opcao);
        clrBuffer();

        switch(opcao) {
            case 1:
                Cliente cliente;
                cadastrarCliente(&cliente); // Inicia o processo de cadastro do cliente e envia ponteiro à função, localizado em clientes.c
                break;
            case 2:
                verClientes(); // Visualiza todos os clientes cadastrados, localizado em clientes.c
                break;
            case 3:
                editarCliente(); // Transporta o usuário ao menu de edição de clientes, localizado em clientes.c
                break;
            case 4:
                Produto produto; // Cria um ponteiro para a struct Produto
                cadastrarProduto(&produto); // Inicia o processo de cadastro do produto e envia ponteiro à função, localizado em produtos.c
                break;
            case 5:
                verProdutos(); // Visualiza todos os produtos cadastrados, localizado em produtos.c
                break;
            case 6:
                editarProduto(); // Transporta o usuário ao menu de edição de produtos, localizado em produtos.c
                break; 
            case 7:
                menuPDV(); // Abre o menu do ponto de vendas
                break;
            case 8:
                verRecibos(); // Visualiza todos os recibos registrados, localizado em pdv.c
                break;
            case 9:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 10:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 11:
                printf("\033[74C\033[33mAbrindo gráfico de lucros e gastos...\033[30m\n");
                criarAbrirGrafico(); // Gera e abre o gráfico de lucro e gasto dos últimos 3 mêses, localizado em financeiro.c
                printf("\n\033[73CPressione qualquer tecla para retornar.\n");
                getchar();
                system("cls");
                break;
            case 12:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 13:
                printf("\n\033[58C\033[34mPrograma de demonstração para o Projeto Integrador Multidisciplinar.\n");
                printf("\033[51CEsta função não foi implementada. Serve apenas para servir como prova de conceito.\033[30m\n");
                sleep(2);
                system("cls");
                break;
            case 14:
                Empregado empregado; // Cria um ponteiro para a struct Empregado
                cadastrarEmpregado(&empregado); // Inicia o processo do cadastro de um empregado e passa o ponteiro à função, localizado em empregados.c
                break;
            case 15:
                editarEmpregado(); // Transporta o usuário ao menu de edição de empregados, localizado em empregados.c
                break;
            case 16:
                verEmpregados(); // Visualiza todos os empregados registrados no sistema, localizado em empregados.c
                break;
            case 17:
                int opDelReg;
                char confirmar;

                system("cls");
                desenharLogo();
                printf("\n\033[70C╔═══════════════════════════════════════════╗\n");
                printf("\033[70C║            Deleção de Registros           ║\n");
                printf("\033[70C╠═══════════════════════════════════════════╣\n");
                printf("\033[70C║ 1) Clientes                               ║\n");
                printf("\033[70C║ 2) Produtos                               ║\n");
                printf("\033[70C║ 3) Recibos                                ║\n");
                printf("\033[70C║ 4) Empregados                             ║\n");
                printf("\033[70C║ 5) Todos os registros                     ║\n");
                printf("\033[70C║ -1) Cancelar e retornar ao menu principal ║\n");
                printf("\033[70C╚═══════════════════════════════════════════╝\n");
                printf("\033[70CSelecione os registros que deseja deletar: ");
                scanf("%d", &opDelReg);
                clrBuffer();

                if (opDelReg != -1 && opDelReg < 6) {
                    printf("\a\n\033[67C╔═════════════════════════════════════════════════╗\n");
                    printf("\033[67C║      Deseja realmente \033[31mEXCLUIR\033[30m os registros?     ║\n");
                    printf("\033[67C║           \033[31mEsta operação não tem volta\033[30m           ║\n");
                    printf("\033[67C╠═════════════════════════════════════════════════╣\n");
                    printf("\033[67C║ Desejo excluir os registros (s/n):              ║\n");
                    printf("\033[67C╚═════════════════════════════════════════════════╝\n");
                    printf("\033[2A\033[105G");
                }
                
                switch (opDelReg) {
                    case 1:
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            limparDadosClientes("data/"); // Apaga os registros dos clientes utilizando o powershell para excluir e recriar os arquivos dat e idx, o argumento da função é o local de onde estão os arquivos, localizaado em utils.c
                            printf("\n\033[77C\033[31mRegistros apagados com sucesso.\033[30m\n");
                            sleep(2);
                            system("cls");
                            break;
                        }
                        break;
                    case 2:
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            limparDadosProdutos("data/"); // Apaga os registros dos produtos utilizando o powershell para excluir e recriar os arquivos dat e idx, o argumento da função é o local de onde estão os arquivos, localizaado em utils.c
                            printf("\n\033[77C\033[31mRegistros apagados com sucesso.\033[30m\n");
                            sleep(2);
                            system("cls");
                            break;
                        }
                        break;
                    case 3:
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            limparDadosRecibos("data/"); // Apaga os registros dos recibos utilizando o powershell para excluir e recriar os arquivos dat e idx, o argumento da função é o local de onde estão os arquivos, localizaado em utils.c
                            printf("\n\033[77C\033[31mRegistros apagados com sucesso.\033[30m\n");
                            sleep(2);
                            system("cls");
                            break;
                        }
                        break;
                    case 4:
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            limparDadosEmpregados("data/"); // Apaga os registros dos empregados utilizando o powershell para excluir e recriar os arquivos dat e idx, o argumento da função é o local de onde estão os arquivos, localizaado em utils.c
                            printf("\n\033[77C\033[31mRegistros apagados com sucesso.\033[30m\n");
                            sleep(2);
                            system("cls");
                            break;
                        }
                        break;
                    case 5:
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            limparDadosGerais("data/"); // Apaga todos os registros utilizando o powershell para excluir e recriar os arquivos dat e idx, o argumento da função é o local de onde estão os arquivos, localizaado em utils.c
                            printf("\n\033[77C\033[31mRegistros apagados com sucesso.\033[30m\n");
                            sleep(2);
                            system("cls");
                            break;
                        }
                        break;
                    case -1:
                        printf("\n\033[82C\033[32mRetornando ao menu..\033[30m\n");
                        system("cls");
                        break;
                    default:
                        printf("\a\n\033[74C\033[31mOpção inválida. Tente novmamente.\033[30m\n");
                }
                break;
            case 18:
                printf("\n\033[64C\033[33mAbrindo o QR Code para o Código de Defesa do Consumidor..\033[30m\n");
                abrirImg("QR_CodigoDefesaConsumidor.png");
                printf("\n\033[54CAlternativamente, link (Para copiar, selecione e aperte: CTRL + SHIFT + C): \n\033[47C\t\033[34mhttps://www.gov.br/mj/pt-br/assuntos/seus-direitos/consumidor/Anexos/cdc-portugues-2013.pdf \033[30m\n");
                printf("\n\n\033[73CPressione qualquer tecla para retornar.\n");
                getchar();
                system("cls");
                break;
            case 19:
                printCredito();
                break;
            case -1:
                printf("\033[31mSaindo do menu de gestão...\033[30m\n");
                break;
            default:
                system("cls");
                printf("\a\033[31mOpção inválida. Tente novamente.\033[30m\n");
        }
    } while (opcao != -1);
}

// Main
int main() {
    // "Pré-inicio", Basicamente prepara o console para deixar tudo certinho para o programa
    setlocale(LC_ALL, "pt-BR.UTF-8"); // Seta o código para aceitar caracteres especiais e acentos
    HWND console = GetConsoleWindow(); // Cria um ponteiro para o console
    HANDLE threadKeybind = CreateThread(NULL, 0, puxarKeybind, NULL, 0, NULL); // Cria uma thread para a função puxarKeybind localizada em utils.c, esperando uma keybind
    ShowWindow(console, SW_MAXIMIZE); // Maximiza a tela (sem deixar em tela cheia)

    SetConsoleTitle("Sistemas Da Terra Hortifruti"); // Seta o titulo do programa
    system("cls"); // Limpa a tela

    char usuario[20];
    char senha[20];
    int cargo, tamF, tamL1, tamL2;
    int ativo = 1;

    printf("1680x1050 (16:10): Fonte 20, Layout 184x49\n");
    printf("1920x1080 (16:9): Fonte 18, Layout 207x54\n\n");
    printf("Insira o tamanho da fonte (Normal 20): ");
    scanf("%d", &tamF);
    clrBuffer();
    setarTamanhoFonte(tamF); // Seta o tamanho da fonte do console para o valor, localizado em utils.c
    printf("\nInsira o tamanho do layout (Normal 184/49): \nAltura: ");
    scanf("%d", &tamL1);
    clrBuffer();
    printf("Largura: ");
    scanf("%d", &tamL2);
    clrBuffer();
    setarTamanhoLayout(tamL1, tamL2); // Seta o tamanho do layout do console para os valores, localizado em utils.c

    // Parte do "boot" do programa, apenas para destaque visual
    Beep(2000, 100); // Toca um beep na frequência e pelo tempo específicado, função nativa do windows
    system("cls");
    desenharBoot(); // Desenha a logo do boot na tela, localizado em utils.c
    tocarSom("audio/MusicaBoot.wav"); // Toca um som utilizando o powershell, o argumento da função é o diretório onde está o audio e seu nome, localizado em utils.c
    sleep(2);
    system("cls");
    sleep(1);
    desenharLogoIntro(); // Desenha a logo da intro na tela, localizado em utils.c
    sleep(3);
    tocarSom("C:/Windows/Media/Windows Logon.wav"); // Toca o som de login padrão do windows, localizado no diretório da instalação do windows

    // Sistema de login que vai "segurar" todas as outras partes do programa
    do {
        // Prepara as cores
        printf("\033[30;47m");
        printf("\033[2J");
        printf("\033[H");
        system("cls");


        // Inicia o programa
        desenharLogo();
        printf("\n\033[78C╔══════════════════════════╗\n");
        printf("\033[78C║     Sistema de Login     ║\n");
        printf("\033[78C╠══════════════════════════╣\n");
        printf("\033[78C║ Usuário:                 ║\n");
        printf("\033[78C║ Senha:                   ║\n");
        printf("\033[78C╚══════════════════════════╝\n");
        printf("\033[67CInsira X no usuário ou senha para sair do programa.\n");

        // Escaneia o nome de usuário inserido pelo usuário
        printf("\033[4A\033[90G"); scanf("%s", usuario);

        // Finaliza o programa caso o usuário seja X
        if (strcmp(usuario, "x") == 0 || strcmp(usuario, "X") == 0) {
            ativo = 0;
            break;
        }

        // Escaneia e esconde a senha inserida pelo usuário
        printf("\033[88G"); escondeSenha(senha);

        // Finaliza o programa caso a senha seja X
        if (strcmp(senha, "x") == 0 || strcmp(senha, "X") == 0) {
            ativo = 0;
            break;
        }

        // Seta o cargo como o número retornado pela função de login
        cargo = login(usuario, senha);

        if (cargo == -1) {
            printf("\033[2B\033[70C\t\033[31m\aDados incorretos. Por favor tente novamente!\033[30m\n");
            sleep(2);
        } else {
            userLogado.cargo = cargo;
            if (userLogado.cargo == 0) {
                funcVendas();
            } else if (userLogado.cargo == 1) {
                funcGestao();
            } else {
                funcAdmin();
            }
        }

        system("cls");
    } while (ativo == 1);
    CloseHandle(threadKeybind); // Termina a thread da keybind

    return 0;
}
