# Sistema de Manutenção de Contas

Atividade em linguagem C para manutenção de contas de clientes usando arquivo binário com registros de tamanho fixo.

## Funcionalidades

O sistema possui um menu com as seguintes opções:

1. Cadastrar um novo cliente em uma posição específica do arquivo.
2. Consultar um cliente pelo número da conta.
3. Atualizar o saldo de um cliente.
4. Encerrar uma conta, removendo logicamente o cliente.
5. Listar todos os clientes ativos.
6. Restaurar a leitura do arquivo desde o início usando `rewind()` e repetir a listagem.
7. Encerrar o programa.

## Como funciona

O arquivo utilizado é binário e se chama `clientes.dat`.

Cada cliente é salvo em um registro de tamanho fixo usando uma `struct`:

```c
typedef struct {
    int ativo;
    int numeroConta;
    char nome[60];
    double saldo;
} Cliente;
```

O campo `ativo` indica se o registro está sendo usado ou se a conta foi encerrada.

Para acessar uma posição específica no arquivo, o programa usa `fseek()`:

```c
fseek(arquivo, posicao * sizeof(Cliente), SEEK_SET);
```

Para gravar e ler os registros, são utilizadas as funções `fwrite()` e `fread()`.

A função `rewind()` é usada na opção 6 para retornar a leitura ao início do arquivo e repetir a listagem dos clientes.

## Observações

A remoção do cliente é lógica, ou seja, o registro continua existindo no arquivo, mas é marcado como inativo. Isso mantém o tamanho fixo dos registros e permite o acesso direto por posição.
