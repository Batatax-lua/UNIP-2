#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS 100

// Definição da estrutura Produto
struct Produto {
    char nome[100];
    float preco_por_kg;
    float peso;
};

// Função para calcular o preço total de um produto
float calcular_preco_total(struct Produto p) {
    return p.peso * p.preco_por_kg;
}

int main() {
    struct Produto produtos[MAX_PRODUTOS];
    int num_produtos = 0;
    char escolha;
    float preco_final = 0.0; 

    do {
        // Entrada do produto
        printf("Digite o nome do produto que deseja: ");
        scanf("%s", produtos[num_produtos].nome);

        printf("Digite o preço por kg de %s: ", produtos[num_produtos].nome);
        scanf("%f", &produtos[num_produtos].preco_por_kg);

        printf("Digite o peso de %s (em kg): ", produtos[num_produtos].nome);
        scanf("%f", &produtos[num_produtos].peso);

        // Cálculo do preço total
        float preco_total = calcular_preco_total(produtos[num_produtos]);
        printf("Preço total de %s: R$ %.2f\n", produtos[num_produtos].nome, preco_total);

        //Calcular todos os preços para um valor final
        preco_final += preco_total;

        num_produtos++;

        // Verificar se o usuário deseja adicionar mais produtos
        printf("Deseja adicionar outro produto? (s/n): ");
        scanf(" %c", &escolha);  // Removido o espaço a mais antes do %c

    } while (escolha == 's' && num_produtos < MAX_PRODUTOS);

    // Exibe todos os produtos e seus preços
    printf("\nResumo da compra:\n");
    for (int i = 0; i < num_produtos; i++) {
        printf("%s: Peso = %.2f kg, Preço/kg = R$ %.2f, Preço total = R$ %.2f\n\n", 
            produtos[i].nome, 
            produtos[i].peso, 
            produtos[i].preco_por_kg, 
            calcular_preco_total(produtos[i]));
    }

    printf ("O preço final de sua compra é de: R$ %.2f\n\n", preco_final);

    return 0;
}

    
