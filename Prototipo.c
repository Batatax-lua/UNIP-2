#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#define MAX_PRODUTOS 100
#define MAX_NOME_PROD 50
#define MAX_PEDIDOS 99

typedef struct {
    char Nome[50];
    float Preco;
    int Quantidade;
} Produto;

typedef struct {
    int Id;
    Produto Produtos[99];
    float Total;
} Pedido;

char mensagemDeBoasVindas[] = "\nBoas vindas ao Sistema de Hortifruti";
char listaDosProdutos[MAX_PRODUTOS][MAX_NOME_PROD];
int numeroDeProdutos = 3;
int id = 0;
Pedido pedidos[MAX_PEDIDOS];

// ... (rest of your functions remain the same)
void ExibirLogo() {
    printf("\n\
    __   __  _______  ______    _______  ___   _______  ______    __   __  _______  ___    \n\
   |  | |  ||       ||    _ |  |       ||   | |       ||    _ |  |  | |  ||       ||   |   \n\
   |  |_|  ||   _   ||   | ||  |_     _||   | |    ___||   | ||  |  | |  ||_     _||   |   \n\
   |       ||  | |  ||   |_||_   |   |  |   | |   |___ |   |_||_ |  |_|  |  |   |  |   |   \n\
   |       ||  |_|  ||    __  |  |   |  |   | |    ___||    __  ||       |  |   |  |   |   \n\
   |   _   ||       ||   |  | |  |   |  |   | |   |    |   |  | ||       |  |   |  |   |   \n\
   |__| |__||_______||___|  |_|  |___|  |___| |___|    |___|  |_||_______|  |___|  |___|   ");

    printf("%s\n", mensagemDeBoasVindas);
}

void RegistrarProduto();
void MostrarProdutosRegistrados();
int RegistrarPedido();
int ExibirPedidos();
void ExibirOpcoesDoMenu();

void ExibirOpcoesDoMenu() {
    while (1) {
        system("clear");
        ExibirLogo();
        printf("\nDigite 1 para registrar um produto\n");
        printf("Digite 2 para mostrar todos os produtos\n");
        printf("Digite 3 para alterar um produto\n");
        printf("Digite 4 para criar um pedido\n");
        printf("Digite 5 para visualizar pedidos\n");
        printf("Digite 6 para exibir o preço médio de um produto\n");
        printf("Digite -1 para sair\n");

        printf("\nDigite a sua opção: ");
        int opcaoEscolhida;
        scanf("%d", &opcaoEscolhida);
        getchar();  // Consume newline

        switch (opcaoEscolhida) {
            case 1:
                RegistrarProduto();
                break;
            case 2:
                MostrarProdutosRegistrados();
                break;
            case 3:
                break;
            case 4:
                RegistrarPedido();
                break;
            case 5:
                ExibirPedidos();
                break;
            case 6:
                printf("Você escolheu a opção %d\n", opcaoEscolhida);
                sleep(2);
                system("clear");
                break;
            case -1:
                printf("Obrigado por utilizar nosso sistema.\n");
                return;
            default:
                printf("Opção inválida\n");
                sleep(2);
                system("clear");
        }
    }
}

void RegistrarProduto() {
    system("clear");
    printf("**********************\n");
    printf("Registro de Produtos\n");
    printf("**********************\n\n");
    printf("Digite o nome do produto que deseja registrar: ");

    char nomeDoProduto[MAX_NOME_PROD];
    fgets(nomeDoProduto, MAX_NOME_PROD, stdin);
    nomeDoProduto[strcspn(nomeDoProduto, "\n")] = 0;  // Remove newline

    if (numeroDeProdutos < MAX_PRODUTOS) {
        strcpy(listaDosProdutos[numeroDeProdutos], nomeDoProduto);
        numeroDeProdutos++;
        printf("O produto %s foi registrado com sucesso!\n", nomeDoProduto);
    } else {
        printf("Limite máximo de produtos atingido!\n");
    }

    sleep(2);
    system("clear");
    ExibirOpcoesDoMenu();
}

void MostrarProdutosRegistrados() {
    system("clear");
    printf("************************************\n");
    printf("Exibindo todos os produtos registrados\n");
    printf("************************************\n\n");

    for (int i = 0; i < numeroDeProdutos; i++) {
        printf("Banda: %s\n", listaDosProdutos[i]);
    }

    printf("\nDigite uma tecla para voltar ao menu principal\n");
    getchar();
    system("clear");
    ExibirOpcoesDoMenu();
}


int RegistrarPedido() {
    system("clear");
    pedidos[id].Id = id;
    printf("ID de Pedido: %d\n", id + 1);
    float total = 0;

    for(int i = 0; i < 99; i++){
        char nom[50];
        float prc;
        int qtd;
        char novoProd;

        printf("Insira o nome do produto: ");
        fgets(nom, sizeof(nom), stdin);
        nom[strcspn(nom, "\n")] = 0;  // Remove newline

        printf("Insira o preço do produto: ");
        scanf("%f", &prc);

        printf("Insira a quantidade do produto: ");
        scanf("%d", &qtd);

        Produto produto = {.Preco = prc, .Quantidade = qtd};
        strcpy(produto.Nome, nom);

        pedidos[id].Produtos[i] = produto;

        total += prc * qtd;

        printf("Deseja adicionar outro produto? [y/n] ");
        scanf(" %c", &novoProd);
        getchar();  // Consume newline

        if (novoProd != 'y'){
            break;
        }
    }
    pedidos[id].Total = total;
    id++;
}

int ExibirPedidos() {
    system("clear");
    for(int i = 0; i < id; i++){
        printf("ID de Pedido: %d\n", i + 1);

        for(int j = 0; j < 99; j++){
            if(pedidos[i].Produtos[j].Quantidade == 0){
                break;
            }
            printf("\nProduto #%d\n", j + 1);
            printf("Produto: %s\n", pedidos[i].Produtos[j].Nome);
            printf("Preço: %.2f\n", pedidos[i].Produtos[j].Preco);
            printf("Quantidade: %d\n", pedidos[i].Produtos[j].Quantidade);
        }
        printf("-------------------------\n");
        printf("[ Subtotal: %.2f ]\n", pedidos[i].Total);
        printf("**************************************\n");
    }
    printf("Aperte qualquer tecla para retornar...");
    getchar();
}

int main() {
    initscr();
    strcpy(listaDosProdutos[0], "Banana");
    strcpy(listaDosProdutos[1], "Melância");
    strcpy(listaDosProdutos[2], "Maçã");

    ExibirOpcoesDoMenu();
    mvprintw(0, 0, "hello world!");
    return 0;
}
