#define NOGDI
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/empregados.h"
#include "../headers/utils.h"
#include "../headers/clientes.h"

void cadastrarEmpregado(Empregado *empregado) {
    FILE *datFile = fopen("data/empregados.dat", "a+b");
    FILE *idxFile = fopen("data/empregados.idx", "a+b");

    if (!datFile || !idxFile) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    system("cls");
    printf("\n\033[79C╔════════════════════════╗\n");
    printf("\033[79C║ Cadastro de Empregados ║\n");
    printf("\033[79C╚════════════════════════╝\n\n");

    fseek(datFile, 0, SEEK_END); // Move ao final do arquivo
    long size = ftell(datFile); // Puxa posição atual

    if (size > 0) { // Caso já existam funcionários
        fseek(datFile, size - sizeof(Empregado), SEEK_SET);
        fread(empregado, sizeof(Empregado), 1, datFile);
        empregado->id++;
    } else {
        empregado->id = 1; // Inicia pelo id 1 caso não existam funcionários
    }

    long long cpfLong, cnpjLong, celularLong;
    int op;

    printf("\033[62C╔══════════════════════════════════════════════════════════╗\n");
    printf("\033[62C║ Nome completo do funcionário (Máximo 50 caracteres)      ║\n");
    printf("\033[62C╠══════════════════════════════════════════════════════════╣\n");
    printf("\033[62C║ Nome: \033[30;46m                                                  \033[30;47m ║\n");
    printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");

    printf("\033[52C╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("\033[52C║ Documento do funcionário (Sem simbolos. Ex: . ou - | Digite 0 caso seja CNPJ) ║\n");
    printf("\033[52C╠═══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("\033[52C║ CPF: \033[30;46m           \033[30;47m                                                              ║\n");
    printf("\033[52C║ CNPJ: \033[30;46m              \033[30;47m                                                          ║\n");
    printf("\033[52C╚═══════════════════════════════════════════════════════════════════════════════╝\n\n");

    printf("\033[59C╔════════════════════════════════════════════════════════════════╗\n");
    printf("\033[59C║ Número de contato do funcionário. Inclua o DDD. Ex: 1899736969 ║\n");
    printf("\033[59C║               (Sem simbolos. Ex: (), - ou espaço)              ║\n");
    printf("\033[59C╠════════════════════════════════════════════════════════════════╣\n");
    printf("\033[59C║ Celular: \033[30;46m           \033[30;47m                                           ║\n");
    printf("\033[59C╚════════════════════════════════════════════════════════════════╝\n\n");

    printf("\033[62C╔═══════════════════════════════════════════════════════════╗\n");
    printf("\033[62C║  E-mail de contato do funcionário (Máximo 50 caracteres)  ║\n");
    printf("\033[62C╠═══════════════════════════════════════════════════════════╣\n");
    printf("\033[62C║ Email: \033[30;46m                                                  \033[30;47m ║\n");
    printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n\n");

    printf("\033[60C╔══════════════════════════════════════════════════════════════╗\n");
    printf("\033[60C║        Endereço do funcionário (Máximo 50 caracteres)        ║\n");
    printf("\033[60C╠══════════════════════════════════════════════════════════════╣\n");
    printf("\033[60C║ Endereço: \033[30;46m                                                  \033[30;47m ║\n");
    printf("\033[60C╚══════════════════════════════════════════════════════════════╝\n\n");
    
    printf("\033[72C╔══════════════════════════════════════╗\n");
    printf("\033[72C║   Data de nascimento do funcionário  ║\n");
    printf("\033[72C║ Utilizar de barras (/) para separar. ║\n");
    printf("\033[72C╠══════════════════════════════════════╣\n");
    printf("\033[72C║ Data de Nascimento: \033[30;46m          \033[30;47m       ║\n");
    printf("\033[72C╚══════════════════════════════════════╝\n\n");

    printf("\n\n\033[87CPágina 1/2\n");

    // NOME
    printf("\033[9;1H\033[71G\033[30;46m");
    fgets(empregado->nome, sizeof(empregado->nome), stdin);
    empregado->nome[strcspn(empregado->nome, "\n")] = 0;
    printf("\033[5B\033[60G");

    // DOCUMENTO
    scanf("%lld", &cpfLong);
    if (cpfLong == 0) {
        empregado->documento = false;
        printf("\033[61G");
        scanf("%lld", &cnpjLong);
        for (int i = 13; i >= 0; i--) {
            empregado->cnpj[i] = cnpjLong % 10;
            cnpjLong /= 10;
        }
        memset(empregado->cpf, 0, sizeof(empregado->cpf));
        printf("\033[6B\033[71G");
    } else {
        empregado->documento = true;
        for (int i = 10; i >= 0; i--) {
            empregado->cpf[i] = cpfLong % 10;
            cpfLong /= 10;
        }
        memset(empregado->cnpj, 0, sizeof(empregado->cnpj));
        printf("\033[7B\033[71G");
    }
    clrBuffer();

    // CELULAR
    scanf("%lld", &celularLong);
    for (int i = 10; i >= 0; i--) {
        empregado->celular[i] = celularLong % 10;
        celularLong /= 10;
    }
    printf("\033[5B\033[72G");
    clrBuffer();

    // EMAIL
    fgets(empregado->email, sizeof(empregado->email), stdin);
    empregado->email[strcspn(empregado->email, "\n")] = 0;
    printf("\033[5B\033[73G");

    // ENDEREÇO
    fgets(empregado->endereco, sizeof(empregado->endereco), stdin);
    empregado->endereco[strcspn(empregado->endereco, "\n")] = 0;
    printf("\033[6B\033[95G");

    // DATA NASC
    fgets(empregado->dataNascimento, sizeof(empregado->dataNascimento), stdin);
    empregado->dataNascimento[strcspn(empregado->dataNascimento, "\n")] = 0;
    printf("\033[30;47m");

    printf("\n\n\033[70C\033[34mAperte Enter para prosseguir a próxima página\033[30m");
    getchar();

    system("cls");
    printf("\n\033[79C╔════════════════════════╗\n");
    printf("\033[79C║ Cadastro de Empregados ║\n");
    printf("\033[79C╚════════════════════════╝\n\n");

    printf("\033[76C╔═══════════════════════════════╗\n");
    printf("\033[76C║      Setor do funcionário     ║\n");
    printf("\033[76C╠═══════════════════════════════╣\n");
    printf("\033[76C║ 1) Gestão                     ║\n");
    printf("\033[76C║ 2) Supervisão                 ║\n");
    printf("\033[76C║ 3) Caixa                      ║\n");
    printf("\033[76C║ 4) Atendimento ao Cliente     ║\n");
    printf("\033[76C║ 5) Reabastecimento de Estoque ║\n");
    printf("\033[76C║ 6) Almoxarifado               ║\n");
    printf("\033[76C║ 7) Limpeza                    ║\n");
    printf("\033[76C╠═══════════════════════════════╣\n");
    printf("\033[76C║ Selecione o setor:            ║\n");
    printf("\033[76C╚═══════════════════════════════╝\n\n");

    printf("\033[69C╔════════════════════════════════════════════╗\n");
    printf("\033[69C║           Salário do funcionário           ║\n");
    printf("\033[69C║ Somente digitos e vírgula (,). Ex: 1860,40 ║\n");
    printf("\033[69C╠════════════════════════════════════════════╣\n");
    printf("\033[69C║ Salário: \033[30;46m          \033[30;47m                        ║\n");
    printf("\033[69C╚════════════════════════════════════════════╝\n\n");

    printf("\033[74C╔═════════════════════════════════╗\n");
    printf("\033[74C║         Data de Admissão        ║\n");
    printf("\033[74C╠═════════════════════════════════╣\n");
    printf("\033[74C║ 1) Data atual (Hoje)            ║\n");
    printf("\033[74C║ 2) Data específica              ║\n");
    printf("\033[74C╠═════════════════════════════════╣\n");
    printf("\033[74C║ Selecione a data de admissão:   ║\n");
    printf("\033[74C╚═════════════════════════════════╝\n\n");

    printf("\n\n\n\n\033[87CPágina 2/2");

    // SETOR
    printf("\033[17;1H\033[98G");
    scanf("%d", &op);
    clrBuffer();

    switch(op) {
        case 1:
            strcpy(empregado->setor, "Gestão");
            break;
        case 2:
            strcpy(empregado->setor, "Supervisão");
            break;
        case 3:
            strcpy(empregado->setor, "Caixa");
            break;
        case 4:
            strcpy(empregado->setor, "Atendimento ao Cliente");
            break;
        case 5:
            strcpy(empregado->setor, "Reabastecimento de Estoque");
            break;
        case 6:
            strcpy(empregado->setor, "Almoxarifado");
            break;
        case 7:
            strcpy(empregado->setor, "Limpeza");
            break;
        default:
            printf("\a\n\033[73C\033[31mOpção inválida! Tente novamente.\033[30m\n");
            return;
    }

    // SALÁRIO
    printf("\033[6B\033[81G\033[30;46m");
    scanf("%lf", &empregado->salario);
    clrBuffer();

    // DATA ADMISSÃO
    printf("\033[30;47m\033[8B\033[107G");
    scanf("%d", &op);
    clrBuffer();

    switch(op) {
        case 1:
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            strftime(empregado->dataAdmissao, sizeof(empregado->dataAdmissao), "%d/%m/%Y", &tm);
            break;
        case 2:
            printf("\033[75G╠═════════════════════════════════╣\n");
            printf("\033[75G║   Data de Admissão Específica   ║\n");
            printf("\033[75G║ Data de Admissão: \033[30;46m          \033[30;47m    ║\n");
            printf("\033[75G╚═════════════════════════════════╝\n");
            printf("\033[2A\033[95G\033[30;46m");
            fgets(empregado->dataAdmissao, sizeof(empregado->dataAdmissao), stdin);
            empregado->dataAdmissao[strcspn(empregado->dataAdmissao, "\n")] = 0;
            printf("\033[30;47m");
            break;
        default:
            printf("\a\n\033[76C\033[31mOpção inválida. Tente novamente.\033[30m\n");
            break;
    }

    strcpy(empregado->status, "Ativo");
    

    // Salva o empregado para os arquivos .dat e .idx
    fseek(datFile, 0, SEEK_END); // Vai para o final do arquivo .dat
    long position = ftell(datFile); // Puxa posição atual
    printf("\n\n\033[54C\033[33mSalvando empregado: ID# %d, Nome: %s, Setor: %s, Salário: %.2lf\033[30m\n", empregado->id, empregado->nome, empregado->setor, empregado->salario);
    fwrite(empregado, sizeof(Empregado), 1, datFile); // Anota funcionário ao .dat
    fflush(datFile); // Força a escrita ao arquivo
    fwrite(&position, sizeof(long), 1, idxFile); // Anota posição ao .idx
    fflush(idxFile); // Força a escrita ao arquivo

    printf("\n\033[66C\033[32mO empregado %s foi registrado com sucesso!\033[30m\n", empregado->nome);
    tocarSomSucesso();
    sleep(2);
    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

void verEmpregados() {
    FILE *datFile = fopen("data/empregados.dat", "a+b");
    FILE *idxFile = fopen("data/empregados.idx", "a+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    system("cls");
    printf("\n\033[69C╔════════════════════════════════════════════╗\n");
    printf("\033[69C║ Exibindo todos os funcionários registrados ║\n");
    printf("\033[69C╚════════════════════════════════════════════╝\n\n");

    Empregado empregado;
    long position;
    long nextPosition;

    rewind(idxFile); // Inicia a leitura do .idx pelo começo

    // Checa se o arquivo idxFile está aberto e é um arquivo válido
    if (idxFile == NULL) {
        printf("\a\033[31mErro ao abrir o arquivo idx.\033[30m\n");
        return;
    }

    printf("\033[18C╔══════════╦═════════════════════════════════════╦══════════════════════════════════════════════════════════════╦═══════════════════════════════════╗\n");
    printf("\033[18C║ ID       ║ Funcionário                         ║ Dados Pessoais                                               ║ Dados Funcionais                  ║\n");
    printf("\033[18C╠══════════╬═════════════════════════════════════╬══════════════════════════════════════════════════════════════╬═══════════════════════════════════╣\n");

    // Lê o index para puxar as posições
    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET); // Procura a posição no .dat
        if (fread(&empregado, sizeof(Empregado), 1, datFile) == 1) {// Lê a info do funcionário
            if (empregado.id != -1) {
                printf("\033[18C║ ID#%02d    ║ %-35s ║ ", empregado.id, empregado.nome);
                if (empregado.documento == true) {
                    printCPF(empregado.cpf);
                } else if (empregado.documento == false) {
                    if (empregado.cnpj[0] != 0) {
                        printCNPJ(empregado.cnpj);
                    }
                }
                printf("\033[1A\033[131G║ Status: %s \033[167G║\n", empregado.status);
                printf("\033[18C║\033[30G║\033[68G║ Telefone: "); printTelefone(empregado.celular); printf("\033[1A\033[131G║ Setor: %s \033[167G║\n", empregado.setor);
                printf("\033[18C║\033[30G║\033[68G║ Email: %s \033[131G║ Data Admissão: %s \033[167G║\n", empregado.email, empregado.dataAdmissao);
                printf("\033[18C║\033[30G║\033[68G║ Endereço: %s \033[131G║ Salário: R$%.2f \033[167G║\n", empregado.endereco, empregado.salario);
                printf("\033[18C║\033[30G║\033[68G║ Data Nasc: %s \033[131G║\033[167G║\n", empregado.dataNascimento);
                
                long currentPosition = ftell(idxFile);
                if (fread(&nextPosition, sizeof(long), 1, idxFile)) {
                    printf("\033[18C╠══════════╬═════════════════════════════════════╬══════════════════════════════════════════════════════════════╬═══════════════════════════════════╣\n");
                }
                fseek(idxFile, currentPosition, SEEK_SET);
            }
        } else {
            printf("\a\033[18C\033[31m╠> Erro ao ler dados do empregado. \033[68G║\033[131G║\033[167G║\033[30m\n");
        }
    }

    printf("\033[18C╚══════════╩═════════════════════════════════════╩══════════════════════════════════════════════════════════════╩═══════════════════════════════════╝\n");
    printf("\n\033[63CDigite qualquer outra tecla para voltar ao menu principal.\n");
    getchar();
    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

void editarEmpregado() {
    FILE *datFile = fopen("data/empregados.dat", "r+b");
    FILE *idxFile = fopen("data/empregados.idx", "r+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    long position;
    long nextPosition;
    Empregado empregado;
    int empregadoId;
    int encontrado = 0;
    long empregadoOffset = -1;

    system("cls");
    
    printf("\n\033[77C╔═════════════════════════════╗\n");
    printf("\033[77C║ Atualização de Funcionários ║\n");
    printf("\033[77C╚═════════════════════════════╝\n\n");

    printf("\033[71C════╣ Listando todos os funcionários ╠════\n\n");
    rewind(idxFile);
    printf("\033[18C╔══════════╦═════════════════════════════════════╦══════════════════════════════════════════════════════════════╦═══════════════════════════════════╗\n");
    printf("\033[18C║ ID       ║ Funcionário                         ║ Dados Pessoais                                               ║ Dados Funcionais                  ║\n");
    printf("\033[18C╠══════════╬═════════════════════════════════════╬══════════════════════════════════════════════════════════════╬═══════════════════════════════════╣\n");

    // Lê o index para puxar as posições
    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET); // Procura a posição no .dat
        if (fread(&empregado, sizeof(Empregado), 1, datFile) == 1) {// Lê a info do funcionário
            if (empregado.id != -1) {
                printf("\033[18C║ ID#%02d    ║ %-35s ║ ", empregado.id, empregado.nome);
                if (empregado.documento == true) {
                    printCPF(empregado.cpf);
                } else if (empregado.documento == false) {
                    if (empregado.cnpj[0] != 0) {
                        printCNPJ(empregado.cnpj);
                    }
                }
                printf("\033[1A\033[131G║ Status: %s \033[167G║\n", empregado.status);
                printf("\033[18C║\033[30G║\033[68G║ Telefone: "); printTelefone(empregado.celular); printf("\033[1A\033[131G║ Setor: %s \033[167G║\n", empregado.setor);
                printf("\033[18C║\033[30G║\033[68G║ Email: %s \033[131G║ Data Admissão: %s \033[167G║\n", empregado.email, empregado.dataAdmissao);
                printf("\033[18C║\033[30G║\033[68G║ Endereço: %s \033[131G║ Salário: R$%.2f \033[167G║\n", empregado.endereco, empregado.salario);
                printf("\033[18C║\033[30G║\033[68G║ Data Nasc: %s \033[131G║\033[167G║\n", empregado.dataNascimento);
                
                long currentPosition = ftell(idxFile);
                if (fread(&nextPosition, sizeof(long), 1, idxFile)) {

                printf("\033[18C╠══════════╬═════════════════════════════════════╬══════════════════════════════════════════════════════════════╬═══════════════════════════════════╣\n");
                }
                fseek(idxFile, currentPosition, SEEK_SET);
            }
        } else {
            printf("\a\033[18C\033[31m╠> Erro ao ler dados do empregado. \033[68G║\033[131G║\033[167G║\033[30m\n");
        }
    }

    printf("\033[18C╚══════════╩═════════════════════════════════════╩══════════════════════════════════════════════════════════════╩═══════════════════════════════════╝\n");

    printf("\n\033[70CDigite o ID do empregado que deseja editar: ");
    scanf("%d", &empregadoId);
    clrBuffer();

    rewind(datFile);
    rewind(idxFile);

    while (fread(&position, sizeof(long), 1, idxFile)) {
        empregadoOffset++;
        fseek(datFile, position, SEEK_SET);
        if (fread(&empregado, sizeof(Empregado), 1, datFile) == 1) {
            if (empregado.id == empregadoId) {
                encontrado = 1;
                break;
            }
        }
    }

    if (!encontrado) {
        printf("\a\n\033[80C\033[31mEmpregado não encontrado.\033[30m\n");
        fclose(datFile);
        fclose(idxFile);
        sleep(2);
        system("cls");
        return;
    } else {
        tocarSomSucesso();
        printf("\n\033[58C\033[32mEmpregado encontrado: %s com ID %d e setor %s\033[30m\n", empregado.nome, empregado.id, empregado.setor);
        printf("\033[69CDocumento encontrado: ");
        if (empregado.documento == true) {
            printCPF(empregado.cpf);
        } else if (empregado.documento == false && empregado.cnpj[0] != 0) {
            printCNPJ(empregado.cnpj);
        }
    }

    int op;
    int opSub;
    char confirmar;

    printf("\n\033[70C╔═══════════════════════════════════════════╗\n");
    printf("\033[70C║           Edição de Funcionário           ║\n");
    printf("\033[70C╠═══════════════════════════════════════════╣\n");
    printf("\033[70C║ 1) Dados pessoais                         ║\n");
    printf("\033[70C║ 2) Dados funcionais                       ║\n");
    printf("\033[70C║ 3) Desligar o funcionário                 ║\n");
    printf("\033[70C║ -1) Cancelar e retornar ao menu principal ║\n");
    printf("\033[70C╚═══════════════════════════════════════════╝\n");
    printf("\033[83CEscolha uma opção: ");
    scanf("%d", &op);
    clrBuffer();

    switch(op) {
        case 1:
            printf("\n\033[70C╔═══════════════════════════════════════════╗\n");
            printf("\033[70C║          Edição de Dados Pessoais         ║\n");
            printf("\033[70C╠═══════════════════════════════════════════╣\n");
            printf("\033[70C║ 1) Nome                                   ║\n");
            printf("\033[70C║ 2) Documento                              ║\n");
            printf("\033[70C║ 3) Telefone                               ║\n");
            printf("\033[70C║ 4) Email                                  ║\n");
            printf("\033[70C║ 5) Endereço                               ║\n");
            printf("\033[70C║ 6) Data de Nascimento                     ║\n");
            printf("\033[70C║ -1) Cancelar e retornar ao menu principal ║\n");
            printf("\033[70C╚═══════════════════════════════════════════╝\n");
            printf("\033[83CEscolha uma opção: ");

            scanf("%d", &opSub);
            clrBuffer();

            char novoNome[50];
            long long novoCPF;
            long long novoCNPJ;
            long long novoCelular;
            char novoEmail[50];
            char novoEndereco[50];
            char novoNascimento[11];

            switch(opSub) {
                case 1:
                    printf("\n\033[61C╔═════════════════════════════════════════════════════════════════╗\n");
                    printf("\033[61C║ Novo nome para o funcionário selecionado (Máximo 50 caracteres) ║\n");
                    printf("\033[61C║ Nome: \033[30;46m                                                  \033[30;47m        ║\n");
                    printf("\033[61C╠═════════════════════════════════════════════════════════════════╣\n");
                    printf("\033[61C║ Deseja confirmar o novo nome para o funcionário? (s/n):         ║\n");
                    printf("\033[61C╚═════════════════════════════════════════════════════════════════╝\n\n");
                    
                    printf("\033[5A\033[70G\033[30;46m");
                    fgets(novoNome, sizeof(novoNome), stdin);
                    novoNome[strcspn(novoNome, "\n")] = 0;
                    printf("\033[30;47m\033[1B\033[120G");

                    scanf(" %c", &confirmar);
                    clrBuffer();

                    if (confirmar == 's' || confirmar == 'S') {
                        strcpy(empregado.nome, novoNome);
                        printf("\n\033[72C\033[32mNome do funcionário editado com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        sleep(2);
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[54C\033[31mEdição cancelada. Nome do funcionário continua sem alteração.\033[30m\n");
                        sleep(2);
                        break;
                    } else {
                        printf("\n\a\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        sleep(2);
                        return;
                    }
                    break;
                case 2:
                    int opDoc;
                    printf("\n\033[73C╔════════════════════════════════════╗\n");
                    printf("\033[73C║        Edição de Documento         ║\n");
                    printf("\033[73C╠════════════════════════════════════╣\n");
                    printf("\033[73C║ 1) Alterar numeração de documento  ║\n");
                    printf("\033[73C║ 2) Alterar tipo de documento       ║\n");
                    printf("\033[73C║ -1) Cancelar e retornar            ║\n");
                    printf("\033[73C╚════════════════════════════════════╝\n");
                    printf("\033[73CEscolha a opção que deseja alterar: ");
                    scanf("%d", &opDoc);
                    clrBuffer();

                    switch(opDoc) {
                        case 1:
                            printf("\n\033[65C╔═════════════════════════════════════════════════════╗\n");
                            printf("\033[65C║           Alterar numeração de documento            ║\n");
                            if (empregado.documento == true) {
                                printf("\033[65C║ CPF: \033[30;46m           \033[30;47m                                    ║\n");
                                printf("\033[65C╠═════════════════════════════════════════════════════╣\n");
                                printf("\033[65C║ Deseja confirmar a nova numeração do CPF? (s/n):    ║\n");
                                printf("\033[65C╚═════════════════════════════════════════════════════╝\n");
                                printf("\033[4A\033[73G\033[30;46m");
                                scanf("%lld", &novoCPF);
                                printf("\033[30;47m\033[1B\033[117G");

                                scanf(" %c", &confirmar);
                                if (confirmar == 's' || confirmar == 'S') {
                                    for (int i = 10; i >= 0; i--) {
                                        empregado.cpf[i] = novoCPF % 10;
                                        novoCPF /= 10;
                                    }
                                    printf("\n\033[72C\033[32mCPF do funcionário editado com sucesso.\033[30m\n");
                                    tocarSomSucesso();
                                    break;
                                    sleep(2);
                                } else if (confirmar == 'n' || confirmar == 'N') {
                                    printf("\n\033[62C\033[31mEdição cancelada. CPF do funcionário continua sem alteração.\033[30m\n");
                                    sleep(2);
                                    break;
                                } else {
                                    printf("\a\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                    sleep(2);
                                    return;
                                }
                            } else {
                                printf("\033[65C║ CNPJ: \033[30;46m              \033[30;47m                                ║\n");
                                printf("\033[65C╠═════════════════════════════════════════════════════╣\n");
                                printf("\033[65C║ Deseja confirmar a nova numeração do CNPJ? (s/n):   ║\n");
                                printf("\033[65C╚═════════════════════════════════════════════════════╝\n");
                                printf("\033[4A\033[74G\033[30;46m");
                                scanf("%lld", &novoCNPJ);
                                printf("\033[30;47m\033[1B\033[118G");
                                clrBuffer();

                                scanf(" %c", &confirmar);
                                if (confirmar == 's' || confirmar == 'S') {
                                    for (int i = 13; i >= 0; i--) {
                                        empregado.cnpj[i] = novoCNPJ % 10;
                                        novoCNPJ /= 10;
                                    }
                                    printf("\n\033[71C\033[32mCNPJ do funcionário editado com sucesso.\033[30m\n");
                                    tocarSomSucesso();
                                    sleep(2);
                                    break;
                                } else if (confirmar == 'n' || confirmar == 'N') {
                                    printf("\n\033[61C\033[31mEdição cancelada. CNPJ do funcionário continua sem alteração.\033[30m\n");
                                    sleep(2);
                                    break;
                                } else {
                                    printf("\a\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                    sleep(2);
                                    return;
                                }
                            }
                            break;

                        case 2:
                            printf("\n\033[66C╔══════════════════════════════════════════════════╗\n");
                            printf("\033[66C║             Alterar tipo de documento            ║\n"); 
                            printf("\033[66C║ Documento atual: ");
                            if (empregado.documento == true) {
                                printCPF(empregado.cpf); printf("\033[1A\033[105G             ║\n");
                                printf("\033[66C╠══════════════════════════════════════════════════╣\n");
                                printf("\033[66C║ Deseja confirmar a alteração para CNPJ? (s/n):   ║\n");
                                printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                                printf("\033[2A\033[116G");

                                scanf(" %c", &confirmar);
                                if (confirmar == 's' || confirmar == 'S') {
                                    printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                                    printf("\033[66C║ CNPJ: \033[30;46m              \033[30;47m                             ║\n");
                                    printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                                    empregado.documento = false;
                                    memset(empregado.cpf, 0, sizeof(empregado.cpf));
                                    printf("\033[2A\033[75G\033[30;46m");
                                    scanf("%lld", &novoCNPJ);
                                    printf("\033[30;47m");
                                    for (int i = 13; i >= 0 ; i--) {
                                        empregado.cnpj[i] = novoCNPJ % 10;
                                        novoCNPJ /= 10;
                                    }
                                    printf("\n\033[71C\033[32mDocumento alterado para CNPJ com sucesso!\033[30m\n");
                                    tocarSomSucesso();
                                    sleep(2);
                                    break;
                                } else if (confirmar == 'n' || confirmar == 'N') {
                                    printf("\n\033[64C\033[31mEdição cancelada. CPF do funcionário continua sem alteração.\033[30m\n");
                                    sleep(2);
                                    break;
                                } else {
                                    printf("\a\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                    sleep(2);
                                    return;
                                }
                            } else if (empregado.documento == false) {
                                printCNPJ(empregado.cnpj); printf("\033[1A\033[106G            ║");
                                printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                                printf("\033[66C║ Deseja confirmar a alteração para CPF? (s/n):    ║\n");
                                printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                                printf("\033[2A\033[115G");;
                                scanf(" %c", &confirmar);
                                if (confirmar == 's' || confirmar == 'S') {
                                    printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                                    printf("\033[66C║ CPF: \033[30;46m           \033[30;47m                                 ║\n");
                                    printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                                    empregado.documento = true;
                                    memset(empregado.cnpj, 0, sizeof(empregado.cnpj));
                                    printf("\033[2A\033[74G\033[30;46m");
                                    scanf("%lld", &novoCPF);
                                    printf("\033[03;47m");
                                    for (int i = 10; i >= 0 ; i--) {
                                        empregado.cpf[i] = novoCPF % 10;
                                        novoCPF /= 10;
                                    }
                                    printf("\n\033[72C\033[32mDocumento alterado para CPF com sucesso!\033[30m\n");
                                    tocarSomSucesso();
                                    sleep(2);
                                    break;
                                } else if (confirmar == 'n' || confirmar == 'N') {
                                    printf("\n\033[63C\033[31mEdição cancelada. CNPJ do cliente continua sem alteração.\033[30m\n");
                                    sleep(2);
                                    break;
                                } else {
                                    printf("\a\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                    sleep(2);
                                    return;
                                }
                            }
                            break;
                        case -1:
                            printf("\033[76C\033[31mCancelando edição de documento.\033[30m\n");
                            sleep(2);
                            break;
                        default:
                            printf("\a\033[76c\033[31mOpção inválida. Tente novamente.\033[30m\n");
                            sleep(2);
                            return;
                    }
                    break;

                case 3:
                    printf("\n\033[65C╔══════════════════════════════════════════════════════════════╗\n");
                    printf("\033[65C║                   Alterar número de celular                  ║\n");
                    printf("\033[65C║ Celular: \033[30;46m           \033[30;47m                                         ║\n");
                    printf("\033[65C╠══════════════════════════════════════════════════════════════╣\n");
                    printf("\033[65C║ Deseja confirmar o novo celular para o funcionário? (s/n):   ║\n");
                    printf("\033[65C╚══════════════════════════════════════════════════════════════╝\n");
                    printf("\033[4A\033[77G\033[30;46m");
                    scanf("%lld", &novoCelular);
                    printf("\033[30;47m");
                    clrBuffer();

                    printf("\033[1B\033[123G");
                    scanf(" %c", &confirmar);
                    if (confirmar == 's' || confirmar == 'S') {
                        for (int i = 10; i >= 0; i--) {
                            empregado.celular[i] = novoCelular % 10;
                            novoCelular /= 10;
                        }
                        printf("\n\033[70C\033[32mCelular do cliente alterado com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        sleep(2);
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[60C\033[31mEdição cancelada. Celular do cliente continua sem alteração.\033[30m\n");
                        sleep(2);
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        sleep(2);
                        return;
                    }
                    break;

                case 4:
                    printf("\n\033[62C╔════════════════════════════════════════════════════════════════╗\n");
                    printf("\033[62C║ Novo email para o funcionário selecionado (Máximo 50 caracteres) ║\n");
                    printf("\033[62C║ Email: \033[30;46m                                                  \033[30;47m       ║\n");
                    printf("\033[62C╠════════════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ Deseja confirmar o novo email para o funcionário? (s/n):       ║\n");
                    printf("\033[62C╚════════════════════════════════════════════════════════════════╝\n\n");
                    
                    printf("\033[5A\033[72G\033[30;46m");
                    fgets(novoEmail, sizeof(novoEmail), stdin);
                    novoEmail[strcspn(novoEmail, "\n")] = 0;
                    printf("\033[30;47m\033[1B\033[122G");

                    scanf(" %c", &confirmar);
                    clrBuffer();

                    if (confirmar == 's' || confirmar == 'S') {
                        strcpy(empregado.email, novoEmail);
                        printf("\n\033[72C\033[32mEmail do funcionário editado com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[61C\033[31mEdição cancelada. Email do funcionário continua sem alteração.\033[30m\n");
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        return;
                    }
                    break;
                case 5:
                    printf("\n\033[62C╔═════════════════════════════════════════════════════════════════════╗\n");
                    printf("\033[62C║ Novo endereço para o funcionário selecionado (Máximo 50 caracteres) ║\n");
                    printf("\033[62C║ Endereço: \033[30;46m                                                  \033[30;47m        ║\n");
                    printf("\033[62C╠═════════════════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ Deseja confirmar o novo endereço para o funcionário? (s/n):         ║\n");
                    printf("\033[62C╚═════════════════════════════════════════════════════════════════════╝\n\n");

                    printf("\033[5A\033[75G\033[30;46m");
                    fgets(novoEndereco, sizeof(novoEndereco), stdin);
                    novoEndereco[strcspn(novoEndereco, "\n")] = 0;
                    printf("\033[30;47m\033[1B\033[125G");

                    scanf(" %c", &confirmar);
                    clrBuffer();

                    if (confirmar == 's' || confirmar == 'S') {
                        strcpy(empregado.endereco, novoEndereco);
                        printf("\n\033[70C\033[32mEndereço do funcionário editado com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[60C\033[31mEdição cancelada. Endereço do funcionário continua sem alteração.\033[30m\n");
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        return;
                    }
                    break;
                case 6:
                    printf("\n\033[55C╔═════════════════════════════════════════════════════════════════════════╗\n");
                    printf("\033[55C║                 Nova data de nascimento do funcionário                  ║\n");
                    printf("\033[55C║                  Utilizar de barras (/) para separar.                   ║\n");
                    printf("\033[55C╠═════════════════════════════════════════════════════════════════════════╣\n");
                    printf("\033[55C║ Nova Data de Nascimento: \033[30;46m          \033[30;47m               ║\n");
                    printf("\033[55C╠═════════════════════════════════════════════════════════════════════════╣\n");
                    printf("\033[55C║ Deseja confirmar a nova data de nascimento para o funcionário? (s/n):   ║\n");
                    printf("\033[55C╚═════════════════════════════════════════════════════════════════════════╝\n\n");

                    printf("\033[5A\033[83G\033[30;46m");
                    fgets(novoNascimento, sizeof(novoNascimento), stdin);
                    novoNascimento[strcspn(novoNascimento, "\n")] = 0;
                    printf("\033[30;47m\033[1B\033[127G");

                    scanf(" %c", &confirmar);
                    clrBuffer();

                    if (confirmar == 's' || confirmar == 'S') {
                        strcpy(empregado.dataNascimento, novoNascimento);
                        printf("\n\033[66C\033[32mData de nascimento do empregado editada com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        sleep(2);
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[56C\033[31mEdição cancelada. Data de nascimento do empregado continua sem alteração.\033[30m\n");
                        sleep(2);
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        sleep(2);
                        return;
                    }
                    break;
                case -1:
                    printf("\n\033[72C\033[31mCancelando e retornado ao menu.\033[30m\n");
                    sleep(2);
                    break;
                default:
                    printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
                    sleep(2);
            }
            break;
        case 2:
            char novoSetor[30];
            char novoAdmissao[11];
            double novoSalario;
            int opFuncional;

            printf("\n\033[69C╔═════════════════════════════════════════════╗\n");
            printf("\033[69C║          Edição de Dados Funcionais         ║\n");
            printf("\033[69C╠═════════════════════════════════════════════╣\n");
            printf("\033[69C║ 1) Setor                                    ║\n");
            printf("\033[69C║ 2) Salário                                  ║\n");
            printf("\033[69C║ 3) Data de Admissão                         ║\n");
            printf("\033[69C║ 4) Status                                   ║\n");
            printf("\033[69C║ -1) Cancelar e retornar ao menu principal   ║\n");
            printf("\033[69C╚═════════════════════════════════════════════╝\n");
            printf("\033[83CEscolha uma opção: ");

            scanf("%d", &opFuncional);
            clrBuffer();

            switch(opFuncional) {
                case 1:
                    int opSetor;

                    printf("\n\033[62C╔══════════════════════════════════════════════════════════╗\n");
                    printf("\033[62C║               Alterar setor do funcionário               ║\n");
                    printf("\033[62C╠══════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ 1) Gestão                                                ║\n");
                    printf("\033[62C║ 2) Supervisão                                            ║\n");
                    printf("\033[62C║ 3) Caixa                                                 ║\n");
                    printf("\033[62C║ 4) Atendimento ao Cliente                                ║\n");
                    printf("\033[62C║ 5) Reabastecimento de Estoque                            ║\n");
                    printf("\033[62C║ 6) Almoxarifado                                          ║\n");
                    printf("\033[62C║ 7) Limpeza                                               ║\n");
                    printf("\033[62C╠══════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ Selecione o novo setor:                                  ║\n");
                    printf("\033[62C╚══════════════════════════════════════════════════════════╝\n");

                    printf("\033[2A\033[103G");
                    scanf("%d", &opSetor);
                    clrBuffer();

                    switch(opSetor) {
                        case 1:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Gestão");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 2:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Supervisão");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 3:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Caixa");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 4:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Atendimento ao Cliente");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 5:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Reabastecimento de Estoque");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 6:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Almoxarifado");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 7:
                            printf("\033[63G╠══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo setor para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[134G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.setor, "Limpeza");
                                printf("\n\033[73C\033[32mSetor do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Setor do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case -1:
                            printf("\n\033[72C\033[31mCancelando e retornando ao menu.\033[30m\n");
                            sleep(2);
                            break;
                        default:
                            printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente!\033[30m\n");
                            sleep(2);
                    }
                    break;
                case 2:
                    printf("\n\033[69C╔════════════════════════════════════════════╗\n");
                    printf("\033[69C║       Alterar salário do funcionário       ║\n");
                    printf("\033[69C║ Somente digitos e vírgula (,). Ex: 1860,40 ║\n");
                    printf("\033[69C╠════════════════════════════════════════════╣\n");
                    printf("\033[69C║ Novo salário: \033[30;46m          \033[30;47m                   ║\n");
                    printf("\033[69C╠════════════════════════════════════════════╣\n");
                    printf("\033[69C║ Deseja confirmar o novo salário? (s/n):    ║\n");
                    printf("\033[69C╚════════════════════════════════════════════╝\n\n");
                    printf("\033[5A\033[86G\033[30;46m");
                    scanf("%lf", &novoSalario);
                    printf("\033[30;47m\033[1B\033[112G");
                    clrBuffer();

                    scanf(" %c", &confirmar);
                    if (confirmar == 's' || confirmar == 'S') {
                        empregado.salario = novoSalario;
                        printf("\n\033[71C\033[32mSalário do empregado alterado com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[61C\033[31mEdição cancelada. Salário do empregado continua sem alteração.\033[30m\n");
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        return;
                    }
                    break;
                case 3:
                    printf("\n\033[74C╔════════════════════════════════════════════════════╗\n");
                    printf("\033[74C║               Alterar data de Admissão             ║\n");
                    printf("\033[74C╠════════════════════════════════════════════════════╣\n");
                    printf("\033[74C║ Nova data de admissão: \033[30;46m          \033[30;47m                  ║\n");
                    printf("\033[74C╠════════════════════════════════════════════════════╣\n");
                    printf("\033[74C║ Deseja confirmar a nova data de admissão? (s/n):   ║\n");
                    printf("\033[74C╚════════════════════════════════════════════════════╝\n\n");
                    printf("\033[5A\033[100G\033[30;46m");
                    fgets(novoAdmissao, sizeof(novoAdmissao), stdin);
                    novoAdmissao[strcspn(novoAdmissao, "\n")] = 0;

                    printf("\033[30;47m\033[1B\033[126G");
                    scanf(" %c", &confirmar);
                    clrBuffer();

                    if (confirmar == 's' || confirmar == 'S') {
                        strcpy(empregado.dataAdmissao, novoAdmissao);
                        printf("\n\033[67C\033[32mData de admissão do empregado editada com sucesso.\033[30m\n");
                        tocarSomSucesso();
                        sleep(2);
                        break;
                    } else if (confirmar == 'n' || confirmar == 'N') {
                        printf("\n\033[57C\033[31mEdição cancelada. Data de admissão do empregado continua sem alteração.\033[30m\n");
                        sleep(2);
                        break;
                    } else {
                        printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                        sleep(2);
                        return;
                    }
                    break;
                case 4:
                    int opStatus;

                    printf("\n\033[62C╔═══════════════════════════════════════════════════════════╗\n");
                    printf("\033[62C║               Alterar status do funcionário               ║\n");
                    printf("\033[62C╠═══════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ 1) Ativo                                                  ║\n");
                    printf("\033[62C║ 2) Afastado                                               ║\n");
                    printf("\033[62C║ 3) Férias                                                 ║\n");
                    printf("\033[62C║ 4) Em Treinamento                                         ║\n");
                    printf("\033[62C╠═══════════════════════════════════════════════════════════╣\n");
                    printf("\033[62C║ Selecione o novo status:                                  ║\n");
                    printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n");
                    printf("\033[2A\033[90G");
                    scanf("%d", &opStatus);
                    clrBuffer();

                    switch(opStatus) {
                        case 1:
                            printf("\033[63G╠═══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo status para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[121G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.status, "Ativo");
                                printf("\n\033[72C\033[32mStatus do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Status do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 2:
                            printf("\033[63G╠═══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo status para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[121G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.status, "Afastado");
                                printf("\n\033[72C\033[32mStatus do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Status do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 3:
                            printf("\033[63G╠═══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo status para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[121G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.status, "Férias");
                                printf("\n\033[72C\033[32mStatus do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Status do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case 4:
                            printf("\033[63G╠═══════════════════════════════════════════════════════════╣\n");
                            printf("\033[62C║ Deseja confirmar o novo status para o empregado? (s/n):   ║\n");
                            printf("\033[62C╚═══════════════════════════════════════════════════════════╝\n\n");
                            printf("\033[3A\033[121G");
                            scanf(" %c", &confirmar);
                            if (confirmar == 's' || confirmar == 'S') {
                                strcpy(empregado.status, "Em Treinamento");
                                printf("\n\033[72C\033[32mStatus do empregado editado com sucesso.\033[30m\n");
                                tocarSomSucesso();
                                sleep(2);
                                break;
                            } else if (confirmar == 'n' || confirmar == 'N') {
                                printf("\n\033[62C\033[31mEdição cancelada. Status do empregado continua sem alteração.\033[30m\n");
                                sleep(2);
                                break;
                            } else {
                                printf("\a\n\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                                sleep(2);
                                return;
                            }
                            break;
                        case -1:
                            printf("\n\033[73C\033[31mCancelando e retornando ao menu.\033[30m\n");
                            break;
                        default:
                            printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
                    }
                    break;
            }
            break;
        case 3:
            printf("\a\n\033[67C╔═════════════════════════════════════════════════╗\n");
            printf("\033[67C║     Deseja realmente \033[31mDESLIGAR\033[30m o funcionário?    ║\n");
            printf("\033[67C║           \033[31mEsta operação não tem volta\033[30m           ║\n");
            printf("\033[67C╠═════════════════════════════════════════════════╣\n");
            printf("\033[67C║ Desejo desligar o funcionário (s/n):            ║\n");
            printf("\033[67C╚═════════════════════════════════════════════════╝\n");
            printf("\033[2A\033[107G");
            scanf(" %c", &confirmar);
            clrBuffer();
            if (confirmar == 's' || confirmar == 'S') {
                empregado.id = -1;
                printf("\n\033[75C\033[32mFuncionário desligado com sucesso.\033[30m\n");
                tocarSomSucesso();
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[62C\033[31mOperação cancelada. Cadastro do funcionário continua efetivo.\033[30m\n");
            }
            sleep(2);
            break;
        case -1:
            printf("\n\033[76C\033[31mCancelando e retornando ao menu.\033[30m\n");
            sleep(2);
            break;
        default:
            printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
            sleep(2);
    }
    clrBuffer();

    fseek(datFile, empregadoOffset * sizeof(Empregado), SEEK_SET);
    fwrite(&empregado, sizeof(Empregado), 1, datFile);

    system("cls");

    fclose(datFile);
    fclose(idxFile);
}