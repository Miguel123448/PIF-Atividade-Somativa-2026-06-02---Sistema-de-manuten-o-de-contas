#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_CLIENTES "clientes.dat"
#define TAM_NOME 60

typedef struct {
    int ativo;
    int numeroConta;
    char nome[TAM_NOME];
    double saldo;
} Cliente;

void limparEntrada() {  //eliminando lixo na entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


FILE* abrirArquivo() {
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "rb+");

    if (arquivo == NULL) {
        arquivo = fopen(ARQUIVO_CLIENTES, "wb+");
    }

    if (arquivo == NULL) {
        printf("Erro ao abrir/criar o arquivo.\n");
        exit(1);
    }

    return arquivo;
}


int contaJaExiste(FILE *arquivo, int numeroConta) {
    Cliente cliente;

    fseek(arquivo, 0, SEEK_SET);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo && cliente.numeroConta == numeroConta) {
            return 1;
        }
    }

    return 0;
}

void cadastrarCliente(FILE *arquivo) {
    int posicao;
    Cliente cliente;
    Cliente existente;

    printf("\n--- Cadastrar novo cliente ---\n");
    printf("Informe a posicao do registro, iniciando em 0: ");
    scanf("%d", &posicao);
    limparEntrada();

    if (posicao < 0) {
        printf("Posicao invalida.\n");
        return;
    }

    fseek(arquivo, posicao * sizeof(Cliente), SEEK_SET);

    if (fread(&existente, sizeof(Cliente), 1, arquivo) == 1 && existente.ativo) {
        printf("Essa posicao ja possui um cliente ativo.\n");
        return;
    }

    printf("Numero da conta: ");
    scanf("%d", &cliente.numeroConta);
    limparEntrada();

    if (cliente.numeroConta <= 0) {
        printf("Numero de conta invalido.\n");
        return;
    }

    if (contaJaExiste(arquivo, cliente.numeroConta)) {
        printf("Ja existe um cliente cadastrado com esse numero de conta.\n");
        return;
    }

    printf("Nome do cliente: ");
    fgets(cliente.nome, TAM_NOME, stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0';   // removendo o /n no final

    printf("Saldo inicial: R$ ");
    scanf("%lf", &cliente.saldo);
    limparEntrada();

    cliente.ativo = 1;

    fseek(arquivo, posicao * sizeof(Cliente), SEEK_SET);
    fwrite(&cliente, sizeof(Cliente), 1, arquivo);
    fflush(arquivo);    //forcando o salvamento

    printf("Cliente cadastrado com sucesso na posicao %d.\n", posicao);
}

void consultarCliente(FILE *arquivo) {
    int numeroConta;
    Cliente cliente;
    int encontrou = 0;

    printf("\n--- Consultar cliente ---\n");
    printf("Informe o numero da conta: ");
    scanf("%d", &numeroConta);
    limparEntrada();

    fseek(arquivo, 0, SEEK_SET);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo && cliente.numeroConta == numeroConta) {
            printf("\nCliente encontrado:\n");
            printf("Conta: %d\n", cliente.numeroConta);
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo: R$ %.2f\n", cliente.saldo);
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Cliente nao encontrado.\n");
    }
}

void atualizarSaldo(FILE *arquivo) {
    int numeroConta;
    Cliente cliente;
    int encontrou = 0;

    printf("\n--- Atualizar saldo ---\n");
    printf("Informe o numero da conta: ");
    scanf("%d", &numeroConta);
    limparEntrada();

    fseek(arquivo, 0, SEEK_SET);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo && cliente.numeroConta == numeroConta) {
            printf("Saldo atual: R$ %.2f\n", cliente.saldo);
            printf("Novo saldo: R$ ");
            scanf("%lf", &cliente.saldo);
            limparEntrada();

            fseek(arquivo, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);    //forcando o salvamento

            printf("Saldo atualizado com sucesso.\n");
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Cliente nao encontrado.\n");
    }
}

void encerrarConta(FILE *arquivo) {
    int numeroConta;
    Cliente cliente;
    int encontrou = 0;

    printf("\n--- Encerrar conta ---\n");
    printf("Informe o numero da conta: ");
    scanf("%d", &numeroConta);
    limparEntrada();

    fseek(arquivo, 0, SEEK_SET);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo && cliente.numeroConta == numeroConta) {
            cliente.ativo = 0;

            fseek(arquivo, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);    //forcando o salvamento

            printf("Conta encerrada com sucesso.\n");
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Cliente nao encontrado.\n");
    }
}

void listarAPartirDaPosicaoAtual(FILE *arquivo) {
    Cliente cliente;
    int posicao = 0;
    int encontrou = 0;

    printf("\n--- Lista de clientes ---\n");

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo) {
            printf("\nPosicao: %d\n", posicao);
            printf("Conta: %d\n", cliente.numeroConta);
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo: R$ %.2f\n", cliente.saldo);
            encontrou = 1;
        }
        posicao++;
    }

    if (!encontrou) {
        printf("Nenhum cliente ativo cadastrado.\n");
    }
}

void listarTodosClientes(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_SET);
    listarAPartirDaPosicaoAtual(arquivo);
}

void restaurarLeitura(FILE *arquivo) {
    printf("\nLeitura restaurada para o inicio do arquivo usando rewind().\n");
    rewind(arquivo);
    listarAPartirDaPosicaoAtual(arquivo);
}

void exibirMenu() {
    printf("\n==============================\n");
    printf(" SISTEMA DE MANUTENCAO DE CONTAS\n");
    printf("==============================\n");
    printf("1. Cadastrar um novo cliente em uma posicao especifica\n");
    printf("2. Consultar um cliente pelo numero da conta\n");
    printf("3. Atualizar o saldo de um cliente\n");
    printf("4. Encerrar conta\n");
    printf("5. Listar todos os clientes\n");
    printf("6. Restaurar leitura com rewind() e repetir listagem\n");
    printf("7. Encerrar\n");
    printf("Escolha uma opcao: ");
}

int main() {
    FILE *arquivo = abrirArquivo();
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparEntrada();

        switch (opcao) {
            case 1: //1. Cadastrar um novo cliente em uma posição específica.
                cadastrarCliente(arquivo);
                break;
            case 2: //2. Consultar um cliente pelo número da conta.
                consultarCliente(arquivo);
                break;
            case 3: //3. Atualizar o saldo de um cliente.
                atualizarSaldo(arquivo);
                break;
            case 4: //4. Encerra conta (remover cliente).
                encerrarConta(arquivo);
                break;
            case 5: //5. Listar todos os clientes.
                listarTodosClientes(arquivo);
                break;
            case 6: //6. Restaurar a leitura do arquivo do início com `rewind()` para repetir a listagem.
                restaurarLeitura(arquivo);
                break;
            case 7:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 7);

    fclose(arquivo);
    return 0;
}
