#define NOGDI
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/clientes.h"
#include "../headers/utils.h"

void cadastrarCliente(Cliente *cliente) {
    FILE *datFile = fopen("data/clientes.dat", "a+b");
    FILE *idxFile = fopen("data/clientes.idx", "a+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("Erro ao abrir os arquivos de dados.\n");
        return;
    }

    int cpf[11] = {0};
    int cnpj[14] = {0};

    system("cls");
    printf("\n\033[79C╔══════════════════════╗\n");
    printf("\033[79C║ Registro de Clientes ║\n");
    printf("\033[79C╚══════════════════════╝\n\n");
    // Enviar ao .dat
    fseek(datFile, 0, SEEK_END); // Move ao final do arquivo
    long size = ftell(datFile); // Puxa posição atual

    if (size > 0) { // Caso já existam clientes
        fseek(datFile, size - sizeof(Cliente), SEEK_SET);
        fread(cliente, sizeof(Cliente), 1, datFile);
        cliente->id++;
    } else {
        cliente->id = 1; // Inicia pelo id 1 caso não existam clientes
    }

    printf("\033[61C╔══════════════════════════════════════════════════════════╗\n");
    printf("\033[61C║ Nome completo do cliente (Máximo 50 caracteres)          ║\n");
    printf("\033[61C╠══════════════════════════════════════════════════════════╣\n");
    printf("\033[61C║ Nome: \033[30;46m                                                  \033[30;47m ║\n");
    printf("\033[61C╚══════════════════════════════════════════════════════════╝\n\n");

    printf("\033[55C╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("\033[55C║ Documento do Cliente (Sem simbolos. Ex: . ou - | Digite 0 caso seja CNPJ) ║\n");
    printf("\033[55C╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("\033[55C║ CPF: \033[30;46m           \033[30;47m                                                          ║\n");
    printf("\033[55C║ CNPJ: \033[30;46m              \033[30;47m                                                      ║\n");
    printf("\033[55C╚═══════════════════════════════════════════════════════════════════════════╝\n\n");

    printf("\033[60C╔═════════════════════════════════════════════════════════════╗\n");
    printf("\033[60C║ Número de contato do cliente. Inclua o DDD. Ex: 1899736969  ║\n");
    printf("\033[60C║             (Sem simbolos. Ex: (), - ou espaço)             ║\n");
    printf("\033[60C╠═════════════════════════════════════════════════════════════╣\n");
    printf("\033[60C║ Celular: \033[30;46m           \033[30;47m                                        ║\n");
    printf("\033[60C╚═════════════════════════════════════════════════════════════╝");

    printf("\033[15A\033[70G\033[30;46m");
    fgets(cliente->nome, sizeof(cliente->nome), stdin);
    cliente->nome[strcspn(cliente->nome, "\n")] = 0;

    int op;
    long long cpfLong;
    printf("\033[5B\033[63G");
    scanf("%lld", &cpfLong);
    if (cpfLong == 0) {
        cliente->documento = false;
        printf("\033[64G");
        long long cnpjLong;
        scanf("%lld", &cnpjLong);
        for (int i = 13; i >= 0; i--) {
            cliente->cnpj[i] = cnpjLong % 10;
            cnpjLong /= 10;
        }
        memset(cliente->cpf, 0, sizeof(cliente->cpf));
        printf("\033[6B\033[72G");
    } else if (cpfLong != 0) {
        cliente->documento = true;
        for (int i = 10; i >= 0; i--) {
            cliente->cpf[i] = cpfLong % 10;
            cpfLong /= 10;
        }
        memset(cliente->cnpj, 0, sizeof(cliente->cnpj));
        printf("\033[7B\033[72G");
    }
    clrBuffer();

    long long celularLong;
    scanf("%lld", &celularLong);
    for (int i = 10; i >= 0; i--) {
        cliente->celular[i] = celularLong % 10;
        celularLong /= 10;
    }
    printf("\033[30;47m");
    clrBuffer();

    // Salva o cliente para os arquivos .dat e .idx
    fseek(datFile, 0, SEEK_END); // Vai para o final do arquivo .dat
    long position = ftell(datFile); // Puxa posição atual
    printf("\n\033[55C\033[33mSalvando cliente: ID# %d, Nome: %s\n", cliente->id, cliente->nome);
    fwrite(cliente, sizeof(Cliente), 1, datFile); // Anota cliente ao .dat
    fflush(datFile); // Força a escrita ao arquivo
    fwrite(&position, sizeof(long), 1, idxFile); // Anota posição ao .idx
    fflush(idxFile); // Força a escrita ao arquivo

    printf("\n\033[50C\033[32mO cliente %s foi registrado com sucesso!\033[30m\n", cliente->nome);
    tocarSomSucesso();
    sleep(2);
    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

void verClientes() {
    FILE *datFile = fopen("data/clientes.dat", "a+b");
    FILE *idxFile = fopen("data/clientes.idx", "a+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    system("cls");
    printf("\n\033[72C╔════════════════════════════════════════╗\n");
    printf("\033[72C║ Exibindo todos os clientes registrados ║\n");
    printf("\033[72C╚════════════════════════════════════════╝\n\n");

    Cliente cliente;
    long position;
    long nextPosition;

    rewind(idxFile); // Inicia a leitura do .idx pelo começo

    printf("\033[39C╔══════════╦═════════════════════════════════════╦══════════════════════════╦════════════════════════════╗\n");
    printf("\033[39C║ ID       ║ Cliente                             ║ Documento                ║ Contato                    ║\n");
    printf("\033[39C╠══════════╬═════════════════════════════════════╬══════════════════════════╬════════════════════════════╣\n");

    // Lê o index para puxar as posições
    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET); // Procura a posição no .dat
        if (fread(&cliente, sizeof(Cliente), 1, datFile) == 1) {// Lê a info do cliente
            if (cliente.id != -1) {
                printf("\033[39C║ ID#%02d    ║ %-35s ║ ", cliente.id, cliente.nome);
                if (cliente.documento == false) {
                    if (cliente.cnpj[0] != 0) {
                        printCNPJ(cliente.cnpj); 
                    }
                } else if (cliente.documento == true) {
                        printCPF(cliente.cpf);
                }
                printf("\033[1A\033[116G║ "); printTelefone(cliente.celular); printf("\033[1A\033[145G║\n");
                long currentPosition = ftell(idxFile);
                if (fread(&nextPosition, sizeof(long), 1, idxFile)) {
                    printf("\033[39C╠══════════╬═════════════════════════════════════╬══════════════════════════╬════════════════════════════╣\n");
                }
                fseek(idxFile, currentPosition, SEEK_SET);
            }
        } else {
            printf("\a\033[39C\033[31m╠> Erro ao ler dados do cliente. \033[145G║\033[30m\n");
        }
    }

    printf("\033[39C╚══════════╩═════════════════════════════════════╩══════════════════════════╩════════════════════════════╝\n");
    printf("\n\033[63CDigite qualquer outra tecla para voltar ao menu principal.\n");
    getchar();
    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

void editarCliente() {
    FILE *datFile = fopen("data/clientes.dat", "r+b");
    FILE *idxFile = fopen("data/clientes.idx", "r+b");

    if (datFile == NULL || idxFile == NULL) {
        printf("\a\033[31mErro ao abrir os arquivos de dados.\033[30m\n");
        return;
    }

    long position;
    long nextPosition;
    Cliente cliente;
    int clienteId;
    int encontrado = 0;
    long clienteOffset = -1;

    system("cls");
    
    printf("\n\033[79C╔═════════════════════════╗\n");
    printf("\033[79C║ Atualização de Clientes ║\n");
    printf("\033[79C╚═════════════════════════╝\n\n");

    printf("\033[73C════╣ Listando todos os clientes ╠════\n\n");
    rewind(idxFile);
    printf("\033[39C╔══════════╦═════════════════════════════════════╦══════════════════════════╦════════════════════════════╗\n");
    printf("\033[39C║ ID       ║ Cliente                             ║ Documento                ║ Contato                    ║\n");
    printf("\033[39C╠══════════╬═════════════════════════════════════╬══════════════════════════╬════════════════════════════╣\n");

    // Lê o index para puxar as posições
    while (fread(&position, sizeof(long), 1, idxFile)) {
        fseek(datFile, position, SEEK_SET); // Procura a posição no .dat
        if (fread(&cliente, sizeof(Cliente), 1, datFile) == 1) {// Lê a info do cliente
            if (cliente.id != -1) {
                printf("\033[39C║ ID#%02d    ║ %-35s ║ ", cliente.id, cliente.nome);
                if (cliente.documento == false) {
                    if (cliente.cnpj[0] != 0) {
                        printCNPJ(cliente.cnpj); 
                    }
                } else if (cliente.documento == true) {
                        printCPF(cliente.cpf);
                }
                printf("\033[1A\033[116G║ "); printTelefone(cliente.celular); printf("\033[1A\033[145G║\n");
                long currentPosition = ftell(idxFile);
                if (fread(&nextPosition, sizeof(long), 1, idxFile)) {
                    printf("\033[39C╠══════════╬═════════════════════════════════════╬══════════════════════════╬════════════════════════════╣\n");
                }
                fseek(idxFile, currentPosition, SEEK_SET);
            }
        } else {
            printf("\a\033[39C\033[31m╠> Erro ao ler dados do produto. \033[145G║\033[30m\n");
        }
    }

    printf("\033[39C╚══════════╩═════════════════════════════════════╩══════════════════════════╩════════════════════════════╝\n");
    printf("\n\033[71CDigite o ID do cliente que deseja editar: ");
    scanf("%d", &clienteId);
    clrBuffer();

    rewind(datFile);
    rewind(idxFile);

    while (fread(&position, sizeof(long), 1, idxFile)) {
        clienteOffset++;
        fseek(datFile, position, SEEK_SET);
        if (fread(&cliente, sizeof(Cliente), 1, datFile) == 1) {
            if (cliente.id == clienteId) {
                encontrado = 1;
                break;
            }
        }
    }

    if (!encontrado) {
        printf("\a\033[31mCliente não encontrado.\033[30m\n");
        fclose(datFile);
        fclose(idxFile);
        sleep(2);
        system("cls");
        return;
    } else {
        printf("\033[32mCliente encontrado: %s com ID %d e celular ", cliente.nome, cliente.id); printTelefone(cliente.celular); printf("\033[30m");
        printf("Documento encontrado: ");
        if (cliente.documento == true) {
            printCPF(cliente.cpf);
        } else if (cliente.documento == false && cliente.cnpj[0] != 0) {
            printCNPJ(cliente.cnpj);
        }
    }

    int op;

    printf("\n\033[70C╔═══════════════════════════════════════════╗\n");
    printf("\033[70C║             Edição de Cliente             ║\n");
    printf("\033[70C╠═══════════════════════════════════════════╣\n");
    printf("\033[70C║ 1) Nome                                   ║\n");
    printf("\033[70C║ 2) Documento                              ║\n");
    printf("\033[70C║ 3) Celular                                ║\n");
    printf("\033[70C║ 4) Finalizar Cadastro                     ║\n");
    printf("\033[70C║ -1) Cancelar e retornar ao menu principal ║\n");
    printf("\033[70C╚═══════════════════════════════════════════╝\n");
    printf("\033[78CEscolha uma opção: ");
    scanf("%d", &op);
    clrBuffer();

    switch(op) {
        char novoNome[50];
        long long novoCPF;
        long long novoCNPJ;
        long long novoCelular;
        char confirmar;
        case 1:
            printf("\n\033[62C╔═════════════════════════════════════════════════════════════╗\n");
            printf("\033[62C║ Novo nome para o cliente selecionado (Máximo 50 caracteres) ║\n");
            printf("\033[62C║ Nome: \033[30;46m                                                  \033[30;47m    ║\n");
            printf("\033[62C╠═════════════════════════════════════════════════════════════╣\n");
            printf("\033[62C║ Deseja confirmar o novo nome para o cliente? (s/n):         ║\n");
            printf("\033[62C╚═════════════════════════════════════════════════════════════╝\n\n");
            printf("\033[5A\033[71G\033[30;46m");
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = 0;
            printf("\033[30;47m\033[1B\033[117G");

            scanf(" %c", &confirmar);
            clrBuffer();

            if (confirmar == 's' || confirmar == 'S') {
                strcpy(cliente.nome, novoNome);
                printf("\n\033[74C\033[32mNome do cliente editado com sucesso.\033[30m\n");
                tocarSomSucesso();
                sleep(2);
                break;
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[56C\033[31mEdição cancelada. Nome do cliente continua sem alteração.\033[30m\n");
                sleep(2);
                break;
            } else {
                printf("\n\a\033[72C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                sleep(2);
                return;
            }
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
                    if (cliente.documento == true) {
                        printf("\033[65C║ CPF: \033[30;46m           \033[30;47m                                    ║\n");
                        printf("\033[65C╠═════════════════════════════════════════════════════╣\n");
                        printf("\033[65C║ Deseja confirmar a nova numeração do CPF? (s/n):    ║\n");
                        printf("\033[65C╚═════════════════════════════════════════════════════╝\n");
                        printf("\033[4A\033[73G\033[30;46m");
                        scanf("%lld", &novoCPF);
                        printf("\033[30;47m\033[1B\033[117G");
                        clrBuffer();
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            for (int i = 10; i >= 0; i--) {
                                cliente.cpf[i] = novoCPF % 10;
                                novoCPF /= 10;
                            }
                            printf("\n\033[74C\033[32mCPF do cliente editado com sucesso.\033[30m\n");
                            tocarSomSucesso();
                            sleep(2);
                            break;
                        } else if (confirmar == 'n' || confirmar == 'N') {
                            printf("\n\033[64C\033[31mEdição cancelada. CPF do cliente continua sem alteração.\033[30m\n");
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
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            for (int i = 13; i >= 0; i--) {
                                cliente.cnpj[i] = novoCNPJ % 10;
                                novoCNPJ /= 10;
                            }
                            printf("\n\033[73C\033[32mCNPJ do cliente editado com sucesso.\033[30m\n");
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

                case 2:
                    printf("\n\033[66C╔══════════════════════════════════════════════════╗\n");
                    printf("\033[66C║             Alterar tipo de documento            ║\n"); 
                    printf("\033[66C║ Documento atual: ");
                    if (cliente.documento == true) {
                        printCPF(cliente.cpf); printf("\033[1A\033[105G             ║\n");
                        printf("\033[66C╠══════════════════════════════════════════════════╣\n");
                        printf("\033[66C║ Deseja confirmar a alteração para CNPJ? (s/n):   ║\n");
                        printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                        printf("\033[2A\033[116G");
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                            printf("\033[66C║ CNPJ: \033[30;46m              \033[30;47m                             ║\n");
                            printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                            cliente.documento = false;
                            memset(cliente.cpf, 0, sizeof(cliente.cpf));
                            printf("\033[2A\033[75G\033[30;46m");
                            scanf("%lld", &novoCNPJ);
                            clrBuffer();
                            printf("\033[30;47m");
                            for (int i = 13; i >= 0 ; i--) {
                                cliente.cnpj[i] = novoCNPJ % 10;
                                novoCNPJ /= 10;
                            }
                            printf("\n\033[71C\033[32mDocumento alterado para CNPJ com sucesso!\033[30m\n");
                            tocarSomSucesso();
                            sleep(2);
                            break;
                        } else if (confirmar == 'n' || confirmar == 'N') {
                            printf("\n\033[64C\033[31mEdição cancelada. CPF do cliente continua sem alteração.\033[30m\n");
                            sleep(2);
                            break;
                        } else {
                            printf("\a\033[74C\033[31mOperador inválido. Tente novamente.\033[30m\n");
                            sleep(2);
                            return;
                        }
                    } else if (cliente.documento == false) {
                        printCNPJ(cliente.cnpj); printf("\033[1A\033[106G            ║");
                        printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                        printf("\033[66C║ Deseja confirmar a alteração para CPF? (s/n):    ║\n");
                        printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                        printf("\033[2A\033[115G");
                        scanf(" %c", &confirmar);
                        clrBuffer();
                        if (confirmar == 's' || confirmar == 'S') {
                            printf("\033[67G╠══════════════════════════════════════════════════╣\n");
                            printf("\033[66C║ CPF: \033[30;46m           \033[30;47m                                 ║\n");
                            printf("\033[66C╚══════════════════════════════════════════════════╝\n");
                            cliente.documento = true;
                            memset(cliente.cnpj, 0, sizeof(cliente.cnpj));
                            printf("\033[2A\033[74G\033[30;46m");
                            scanf("%lld", &novoCPF);
                            clrBuffer();
                            printf("\033[03;47m");
                            for (int i = 10; i >= 0 ; i--) {
                                cliente.cpf[i] = novoCPF % 10;
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
            printf("\033[65C╔══════════════════════════════════════════════════════════╗\n");
            printf("\033[65C║                 Alterar número de celular                ║\n");
            printf("\033[65C║ Celular: \033[30;46m           \033[30;47m                                     ║\n");
            printf("\033[65C╠══════════════════════════════════════════════════════════╣\n");
            printf("\033[65C║ Deseja confirmar o novo celular para o cliente? (s/n):   ║\n");
            printf("\033[65C╚══════════════════════════════════════════════════════════╝\n");
            printf("\033[4A\033[77G\033[30;46m");
            scanf("%lld", &novoCelular);
            printf("\033[30;47m");
            clrBuffer();

            printf("\033[1B\033[123G");
            scanf(" %c", &confirmar);
            clrBuffer();
            if (confirmar == 's' || confirmar == 'S') {
                for (int i = 10; i >= 0; i--) {
                    cliente.celular[i] = novoCelular % 10;
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
            printf("\a\n\033[66C╔═════════════════════════════════════════════════╗\n");
            printf("\033[66C║ Deseja realmente \033[31mEXCLUIR\033[30m o cadastro do cliente? ║\n");
            printf("\033[66C║           \033[31mEsta operação não tem volta\033[30m           ║\n");
            printf("\033[66C╠═════════════════════════════════════════════════╣\n");
            printf("\033[66C║ Desejo excluir o cadastro do cliente (s/n):     ║\n");
            printf("\033[66C╚═════════════════════════════════════════════════╝\n");
            printf("\033[2A\033[113G");
            scanf(" %c", &confirmar);
            clrBuffer();
            if (confirmar == 's' || confirmar == 'S') {
                cliente.id = -1;
                printf("\n\033[70C\033[32mCadastro do cliente finalizado com sucesso.\033[30m\n");
                tocarSomSucesso();
                sleep(2);
            } else if (confirmar == 'n' || confirmar == 'N') {
                printf("\n\033[57C\033[31mOperação cancelada. Cadastro do cliente continua efetivo.\033[30m\n");
                sleep(2);
            }
            break;
        case -1:
            printf("\n\033[73C\033[31mCancelando e retornando ao menu.\033[30m\n");
            break;
        default:
            printf("\a\n\033[74C\033[31mOpção inválida. Tente novamente.\033[30m\n");
            sleep(2);
            fclose(datFile);
            return;
    }

    fseek(datFile, clienteOffset * sizeof(Cliente), SEEK_SET);
    fwrite(&cliente, sizeof(Cliente), 1, datFile);

    system("cls");

    fclose(datFile);
    fclose(idxFile);
}

Cliente* buscarCliente(const char* nome, const int cpf[], const int cnpj[]) {
    FILE *datFile = fopen("data/clientes.dat", "r+b");
    if (!datFile) {
        printf("\a\033[31mErro ao abrir clientes.dat\033[30m");
        return NULL;
    }

    Cliente* cliente = malloc(sizeof(Cliente));
    if (!cliente) {
        printf("\a\033[31mErro ao alocar memória para o Cliente.\033[30m\n");
        fclose(datFile);
        return NULL;
    }

    while (fread(cliente, sizeof(Cliente), 1, datFile)) {
        if (cliente->documento == true && memcmp(cliente->cpf, cpf, sizeof(cliente->cpf)) == 0) {
            fclose(datFile);
            return cliente;
        } else if (cliente->documento == false && memcmp(cliente->cnpj, cnpj, sizeof(cliente->cnpj)) == 0) {
            fclose(datFile);
            return cliente;
        }
    }

    free(cliente);
    fclose(datFile);
    return NULL;
}