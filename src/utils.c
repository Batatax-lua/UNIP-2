#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <windows.h>
#include <shellapi.h>
#include "../headers/utils.h"

void clrBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void setarTamanhoFonte(int tamanhoFonte) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;

    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    cfi.dwFontSize.X = 0;        
    cfi.dwFontSize.Y = tamanhoFonte; 
    cfi.FontWeight = 400;
    wcscpy(cfi.FaceName, L"Consolas"); 

    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

void setarTamanhoLayout(int tamX, int tamY) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { tamX, tamY };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = {0, 0, tamX - 1, tamY - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void desenharLogo() { 
    printf("\033[32;47m\n\
    \033[51C██╗  ██╗ ██████╗ ██████╗ ████████╗██╗███████╗██████╗ ██╗   ██╗████████╗██╗ \n\
    \033[51C██║  ██║██╔═══██╗██╔══██╗╚══██╔══╝██║██╔════╝██╔══██╗██║   ██║╚══██╔══╝██║ \n\
    \033[51C███████║██║   ██║██████╔╝   ██║   ██║█████╗  ██████╔╝██║   ██║   ██║   ██║ \n\
    \033[51C██╔══██║██║   ██║██╔══██╗   ██║   ██║██╔══╝  ██╔══██╗██║   ██║   ██║   ██║ \n\
    \033[51C██║  ██║╚██████╔╝██║  ██║   ██║   ██║██║     ██║  ██║╚██████╔╝   ██║   ██║ \n\
    \033[51C╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝     ╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝ ");
    printf("\033[32m\n\033[72CBoas vindas ao seu sistema de hortifruti!\033[30m\n");
    
    time_t t;
    time(&t);
    struct tm *local_time = localtime(&t);
    char buffer[40];
    strftime(buffer, sizeof(buffer), "%A, %d/%m/%Y %H:%M", local_time);
    printf("\033[90m\033[76C%s\033[30m\n", buffer);

    printf("\033[48;2H\033[32mAperte Ctrl + P para ligar/desligar a música de fundo.\033[30m\033[10;92H");
}

void desenharBoot() {
    printf("\033[12B\033[84C\033[94mNexaByte Sistemas\n");
    printf("\033[2B");
    printf("\033[70C               .,-:;//;:=, \n");
    printf("\033[70C           . :H@@@MM@M#H/.,+%%;, \n");
    printf("\033[70C        ,/X+ +M@@M@MM%%=,-%%HMMM@X/, \n");
    printf("\033[70C      -+@MM; $M@@MH+-,;XMMMM@MMMM@+- \n");
    printf("\033[70C     ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/. \n");
    printf("\033[70C   ,%%MM@@MH ,@%%=             .---=-=:=,. \n");
    printf("\033[70C   =@#@@@MX.,                -%%HX$$%%%%%%:; \n");
    printf("\033[70C  =-./@M@M$                   .;@MMMM@MM: \n");
    printf("\033[70C  X@/ -$MM/                    . +MM@@@M$ \n");
    printf("\033[70C ,@M@H: :@:                    . =X#@@@@- \n");
    printf("\033[70C ,@@@MMX, .                    /H- ;@M@M= \n");
    printf("\033[70C .H@@@@M@+,                    %%MM+..%%#$. \n");
    printf("\033[70C  /MMMM@MMH/.                  XM@MH; =; \n");
    printf("\033[70C   /%%+%%$XHH@$=              , .H@@@@MX, \n");
    printf("\033[70C    .=--------.           -%%H.,@@@@@MX, \n");
    printf("\033[70C    .%%MM@@@HHHXX$$$%%+- .:$MMX =M@@MM%%. \n");
    printf("\033[70C      =XMMM@MM@MM#H;,-+HMM@M+ /MMMX= \n");
    printf("\033[70C        =%%@M@M#@$-.=$@MM@@@M; %%M%%= \n");
    printf("\033[70C          ,:+$+-,/H#MMMMMMM@= =, \n");
    printf("\033[70C                =++%%%%+/:-. \n");
    printf("\n\n\033[73C\033[94mCarregando seu sistema de hortifruti...");    
}

void desenharLogoIntro() {
    printf("\033[12B\033[83C\033[92mDa Terra Hortifruti\n");
    printf("\033[2B");
    printf("\033[32m\033[58C       :      \033[31m                 ..,,..    ...,,.. \n");
    printf("\033[32m\033[58C      ,%%,    \033[31m            .. ,#########::#########:, \n");
    printf("\033[32m\033[58C      :#%%,   \033[31m        ,,:',####%%%%%%##:`::%%%%####, \n");
    printf("\033[32m\033[58C     ,##%%%%,  \033[31m    ,##%% ,##%%%%:::::''%%' `::::%%####, \n");
    printf("\033[32m\033[58C     %###%%%;;,\033[31m   ,###%%:,##%%:::''    '  . .`:::%%###, \n");
    printf("\033[32m\033[58C    :####%%;;::\033[31m ,##%%:' ,#%%::''   .,,,..    . .`::%%%%##, \n");
    printf("\033[32m\033[58C    %####%;;::,\033[31m##%%:' ,##%''  ,%%########%     . `:::%%##, \n");
    printf("\033[32m\033[58C    ######:::,\033[31m##%%:',####:  ,##%%:''     `%%,     .`::%%##, \n");
    printf("\033[32m\033[58C    :#####%%:'\033[31m##%%:',#####' ,###%' ,%%%%,%%%,'%%,     . ::%%###,,.. \n");
    printf("\033[32m\033[58C     #####%%:,\033[31m#%%:'#######  %%:'%  %'  `%% %% %%,.     '::%%#######, \n");
    printf("\033[32m\033[58C     `####%%,\033[31m#%%:',####### ::' %   ' ,%%%%%%%, ::%%.    . '::%%###### \n");
    printf("\033[32m\033[58C      `###'##%%:\033[31m ######## ,.   %%  %%,   ':: `:%%%%  :  . .:::%%###' \n");
    printf("\033[32m\033[58C      ,,::,### \033[31m %%%%%%### ::  % %% '%%%,.::: .:%%%%   #.  . ::%%%%#' \n");
    printf("\033[32m\033[58C,,,:::%%##:\033[31m;#   `%%%%%## :% ,%%, %   ':%%%:'  #%%%%' ,.:##.  ::%%#' \n");
    printf("\033[32m\033[58C::%%#####% %%%\033[31m:::  :::%%%% `%%,'%%     ..,,%####' :%# `::##, '' \n");
    printf("\033[32m\033[58C###%%::'###\033[31m%%::: .   `:::, `::,,%%%######%%%'',::%%##' ,:::%%## \n");
    printf("\033[32m\033[58C''''   ,###\033[31m#%%:::. .  `::%%,     '':%%::' .,::%%%%#'   :::%%%%##, \n");
    printf("\033[32m\033[58C      :#%%'\033[31m##%%:::.  . . '%%::,,.. ..,,,,::%%%%###'  ,:%%%%####' \n");
    printf("\033[32m\033[58C     ,###%%'\033[31m###%%:::: . . `::::::::::%%%%#####'   ,::%%####:' \n");
    printf("\033[32m\033[58C     %###%%%;\033[31m'###%%::::.   .`::%%%%%%%%#####:'  ,,::%%##:' \n");
    printf("\033[32m\033[58C     ####%%;:\033[31m;'####%%:::::.   `:%%######::'  ,,:::%%### \n");
    printf("\033[32m\033[58C     %%####;:\033[31m;'######%%::::.           ,::::%%%%####' \n");
    printf("\033[32m\033[60C     `####%%;:\033[31m'`#########%%:::....,,:::%%%%#######' \n");
    printf("\033[32m\033[58C        ;#####;;\033[31m'..;;:::#########::%%#########:'' \n");
    printf("\033[32m\033[58C                     \033[31m  ~~~~``````''''~~~ ");
}

void printCredito() {
    system("cls");
    printf("\n\033[73C╔═════════════════════════════════════╗\n");
    printf("\033[73C║             Créditos do             ║\n");
    printf("\033[73C║ Projeto Integrador Multidisciplinar ║\n");
    printf("\033[73C║           Semestre 2/2024           ║\n");
    printf("\033[73C╚═════════════════════════════════════╝\n\n");

    printf("\033[14;1H\033[68C╔═══════════════════════════════════════════════╗\n");
    printf("\033[68C║                     \033[31mLíder\033[30m                     ║\n");
    printf("\033[68C║          Gustavo Bianchi de Oliveira          ║\n");
    printf("\033[68C╠═══════════════════════════════════════════════╣\n");
    printf("\033[68C╠> Fundamentos de Redes de Dados de Comunicação ║\n");
    printf("\033[68C╠> Metodologia Científica                       ║\n");
    printf("\033[68C╚═══════════════════════════════════════════════╝\n");

    printf("\033[38C╔═══════════════════════════════════════════════╗\n");
    printf("\033[38C║                  \033[35mProgramador\033[30m                  ║     ╔═══════════════════════════════════════════════╗\n");
    printf("\033[38C║        Luiz Augusto Gonçalves Bazzegio        ║     ║              Victor Heitor Roldam             ║\n");
    printf("\033[38C╠═══════════════════════════════════════════════╣     ╠═══════════════════════════════════════════════╣\n");
    printf("\033[38C╠> Linguagens e Técnicas de Programação         ║     ╠> Engenharia de Software                       ║\n");
    printf("\033[38C╠> Matemática para Computação                   ║     ╚═══════════════════════════════════════════════╝\n");
    printf("\033[38C╚═══════════════════════════════════════════════╝\n");

    printf("\033[38C╔═══════════════════════════════════════════════╗     ╔═══════════════════════════════════════════════╗\n");
    printf("\033[38C║        Lilyan Maria Moreira de Oliveira       ║     ║        Miguel Henrique de Souza Manarin       ║\n");
    printf("\033[38C╠═══════════════════════════════════════════════╣     ╠═══════════════════════════════════════════════╣\n");
    printf("\033[38C╠> Ética e Legislação Profissional              ║     ╠> Educação Ambiental                           ║\n");
    printf("\033[38C╚═══════════════════════════════════════════════╝     ╚═══════════════════════════════════════════════╝\n");

    printf("\033[5B\033[74CPressione enter para retornar ao menu");
    getchar();
    system("cls");
}

void toLowerStr(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

long converterParaLong(const char* str) {
    long result = 0;
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        }
        str++;
    }
    return result;
}

void printCPF(const int cpf[]) {
    printf("CPF: %d%d%d.%d%d%d.%d%d%d-%d%d\n",
        cpf[0], cpf[1], cpf[2], // 3 primeiros digitos
        cpf[3], cpf[4], cpf[5], // 3 digitos do meio
        cpf[6], cpf[7], cpf[8], // 3 ultimos digitos
        cpf[9], cpf[10]);       // 2 digitos finals depois do traço
}

void printCNPJ(const int cnpj[]) {
    printf("CNPJ: %d%d.%d%d%d.%d%d%d/%d%d%d%d-%d%d\n",
        cnpj[0], cnpj[1],                       // 2 primeiros digitos
        cnpj[2], cnpj[3], cnpj[4],              // 3 digitos do meio pt. 1
        cnpj[5], cnpj[6], cnpj[7],              // 3 digitos do meio pt. 2
        cnpj[8], cnpj[9], cnpj[10], cnpj[11],   // 4 digitos depois da barra
        cnpj[12], cnpj[13]);                    // 2 digitos depois do traço
}

void printTelefone(const int celular[]) {
    printf("(%d%d) %d%d%d%d%d-%d%d%d%d\n",
        celular[0], celular[1],                                     // 2 digitos do DDD
        celular[2], celular[3], celular[4], celular[5], celular[6], // 5 primeiros digitos
        celular[7], celular[8], celular[9], celular[10]);           // 4 ultimos digitos
}

void abrirImg(const char *localImg) {
    FILE *img = fopen(localImg, "r");
    if (!img) {
        printf("\n\a\033[60C\033[31mErro ao abrir %s, verifique-se que a imagem existe!\n\033[30m", localImg);
    } else {
        ShellExecute(0, "open", localImg, NULL, NULL, SW_SHOW);
        fclose(img);
    }
}

void tocarSom(const char* localSom) {
    PlaySound(localSom, NULL, SND_FILENAME | SND_ASYNC);
}

void tocarSomSucesso() {
    PlaySound("audio/SomSucesso.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void tocarMusica(const char* Musica) {
    static int isTocando = 0;
    
    const char* pastaMusicas = "audio/";
    char fullPath[260];

    snprintf(fullPath, sizeof(fullPath), "%s%s", pastaMusicas, Musica);
    
    if (isTocando == 1) {
        PlaySound(NULL, NULL, SND_ASYNC);
    } else {
        PlaySound(fullPath, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    isTocando = !isTocando;
}

DWORD WINAPI puxarKeybind(LPVOID lpParam) {
    while (1) {
        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('P') & 0x8000)) {
            tocarMusica("MusicaFundo.wav");
            sleep(5);
        }
        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('U') & 0x8000)) {
            tocarMusica("MusicaFundoAlt.wav");
            sleep(5);
        }
        Sleep(500);
    }
    return 0;
}

void limparDadosClientes(const char *pastaDados) {
    struct dirent *entry;
    DIR *dir = opendir(pastaDados);

    char caminhoDados[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "clientes.dat") || strstr(entry->d_name, "clientes.idx")) {
            snprintf(caminhoDados, sizeof(caminhoDados), "%s/%s", pastaDados, entry->d_name);

            if (unlink(caminhoDados) == 0) {
                printf("\t\t═> Arquivo deletado: %s\n", caminhoDados);

                FILE *file = fopen(caminhoDados, "w");
                if (file != NULL) {
                    printf("\t\t═> Arquivo criado: %s\n", caminhoDados);
                    fclose(file);
                }
            }
        }
    }

    closedir(dir);
}

void limparDadosProdutos(const char *pastaDados) {
    struct dirent *entry;
    DIR *dir = opendir(pastaDados);

    char caminhoDados[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "produtos.dat") || strstr(entry->d_name, "produtos.idx")) {
            snprintf(caminhoDados, sizeof(caminhoDados), "%s/%s", pastaDados, entry->d_name);

            if (unlink(caminhoDados) == 0) {
                printf("\t\t═> Arquivo deletado: %s\n", caminhoDados);

                FILE *file = fopen(caminhoDados, "w");
                if (file != NULL) {
                    printf("\t\t═> Arquivo criado: %s\n", caminhoDados);
                    fclose(file);
                }
            }
        }
    }

    closedir(dir);
}

void limparDadosRecibos(const char *pastaDados) {
    struct dirent *entry;
    DIR *dir = opendir(pastaDados);

    char caminhoDados[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "recibos.dat") || strstr(entry->d_name, "recibos.idx")) {
            snprintf(caminhoDados, sizeof(caminhoDados), "%s/%s", pastaDados, entry->d_name);

            if (unlink(caminhoDados) == 0) {
                printf("\t\t═> Arquivo deletado: %s\n", caminhoDados);

                FILE *file = fopen(caminhoDados, "w");
                if (file != NULL) {
                    printf("\t\t═> Arquivo criado: %s\n", caminhoDados);
                    fclose(file);
                }
            }
        }
    }

    closedir(dir);
}

void limparDadosEmpregados(const char *pastaDados) {
    struct dirent *entry;
    DIR *dir = opendir(pastaDados);

    char caminhoDados[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "empregados.dat") || strstr(entry->d_name, "empregados.idx")) {
            snprintf(caminhoDados, sizeof(caminhoDados), "%s/%s", pastaDados, entry->d_name);

            if (unlink(caminhoDados) == 0) {
                printf("\t\t═> Arquivo deletado: %s\n", caminhoDados);

                FILE *file = fopen(caminhoDados, "w");
                if (file != NULL) {
                    printf("\t\t═> Arquivo criado: %s\n", caminhoDados);
                    fclose(file);
                }
            }
        }
    }

    closedir(dir);
}

void limparDadosGerais(const char *pastaDados) {
    struct dirent *entry;
    DIR *dir = opendir(pastaDados);

    char caminhoDados[256];

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".dat") || strstr(entry->d_name, ".idx")) {
            snprintf(caminhoDados, sizeof(caminhoDados), "%s/%s", pastaDados, entry->d_name);

            if (unlink(caminhoDados) == 0) {
                printf("\t\t═> Arquivo deletado: %s\n", caminhoDados);

                FILE *file = fopen(caminhoDados, "w");
                if (file != NULL) {
                    printf("\t\t═> Arquivo criado: %s\n", caminhoDados);
                    fclose(file);
                }
            }
        }
    }

    closedir(dir);
}