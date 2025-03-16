#include <stdio.h>
#include <string.h>

#define EMPLOYEE_FILE "employees.dat"
#define INDEX_FILE "index.dat"

// Estrutura para armazenar os dados de um empregado
typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

// Estrutura para armazenar a posição de um empregado no arquivo (índice)
typedef struct {
    int id;
    long position;
} IndexEntry;

// Função para adicionar um empregado ao arquivo e atualizar o índice
void add_employee(Employee emp) {
    FILE *data_file = fopen(EMPLOYEE_FILE, "ab");  // Abrir o arquivo de dados no modo de adição binária
    FILE *index_file = fopen(INDEX_FILE, "ab");    // Abrir o arquivo de índice no modo de adição binária

    if (!data_file || !index_file) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    // Obter a posição atual no arquivo (onde o registro será adicionado)
    long position = ftell(data_file);

    // Gravar o empregado no arquivo de dados
    fwrite(&emp, sizeof(Employee), 1, data_file);

    // Criar uma entrada de índice e associar o ID à posição
    IndexEntry entry = { emp.id, position };
    fwrite(&entry, sizeof(IndexEntry), 1, index_file);

    fclose(data_file);
    fclose(index_file);
}

// Função para buscar um empregado pelo ID
void find_employee_by_id(int id) {
    FILE *data_file = fopen(EMPLOYEE_FILE, "rb");
    FILE *index_file = fopen(INDEX_FILE, "rb");

    if (!data_file || !index_file) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    IndexEntry entry;
    int found = 0;

    // Procurar o ID no arquivo de índice
    while (fread(&entry, sizeof(IndexEntry), 1, index_file)) {
        if (entry.id == id) {
            found = 1;
            // Se encontrado, usar fseek para acessar a posição correta no arquivo de dados
            fseek(data_file, entry.position, SEEK_SET);
            Employee emp;
            fread(&emp, sizeof(Employee), 1, data_file);
            printf("Empregado encontrado: ID = %d, Nome = %s, Salário = %.2f\n", emp.id, emp.name, emp.salary);
            break;
        }
    }

    if (!found) {
        printf("Empregado com ID %d não encontrado.\n", id);
    }

    fclose(data_file);
    fclose(index_file);
}

int main() {
    // Adicionando alguns empregados
    Employee emp1 = { 1, "Alice", 50000.0 };
    Employee emp2 = { 2, "Bob", 55000.0 };
    Employee emp3 = { 3, "Charlie", 60000.0 };

    add_employee(emp1);
    add_employee(emp2);
    add_employee(emp3);

    // Buscando um empregado por ID
    find_employee_by_id(2);  // Deve encontrar o Bob
    find_employee_by_id(4);  // Deve exibir "Empregado com ID 4 não encontrado."

    return 0;
}
