#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines ------------------
#define USUARIOS "Usuarios.dat" // nome do arquivo binario para salvar os usuarios
#define DADOSCLIENTES "DadosClientes.dat"
#define max_caracter 100               // max de caracter para a descrição
#define ARQUIVO "cadastrar&vendas.dat" // renomeando o arquivo que vai ser valvo
#define CATEGORIAS "Categorias.dat"


// structs------------------
typedef struct
{
    int id;
    char usuario[15];
    char password[10];
    int tipo_acesso;
} Usuarios;

typedef struct
{
    int cod;
    char nome[100];
    char nome_social[100];
    char cpf[12];
    char rua_num[50];
    char bairro[50];
    char celular[50];
} Cliente;

typedef struct
{
    int id;
    char descricao_Produto[max_caracter];
    int categoria_Produto;
    float Preco_De_Compra;
    float Preco_De_Venda;
    int Quantidade_em_Estoque;
    int estoque_Minimo;
} Cadastrar_Produtos;

typedef struct{
    int id; 
    char categoria[30];
} Categorias;

// declaracao das funcoes ------------
void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void salvar_Produtos_Arquivo(Cadastrar_Produtos *pt, FILE *arquivo);
int menu_principal();
int menu_cadastros();
void main_cadastros();
void removeEnterTexto(char *);
int contadorCaracterFile(char *, char);
void cadastrar_usuario();
void show_usuarios();
void cadastrarCliente();
int verificaArquivo(FILE *arquivo);
void limparBuffer();
void cadastrarCategorias();

void pular_Linha()
{
    printf("\n");
}

// funcao principal
int main()
{
    int opcao;

    do
    {
        opcao = menu_principal();

        switch ((opcao))
        {
        case 1:
            main_cadastros();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            printf("Obrigado por usar o nosso sistema!!!\n");
            break;
        default:
            printf("Opcao Invalida!!!\n");
            break;
        }

    } while ((opcao) != 6);
}

int menu_principal()
{
    char opcao[3];
    printf(
        "Menu Principal: \n"
        "\t[1]- Cadastros \n"
        "\t[2]- Vendas \n"
        "\t[3]- Abertura de Caixa \n"
        "\t[4]- Fechamento de Caixa \n"
        "\t[5]- Relatorios \n"
        "\t[6]- Sair \n");
    fgets(opcao, sizeof(opcao), stdin);
    return atoi(opcao);
}
int menu_cadastros()
{
    char opcao[3];
    printf(
        "Menu Cadastros\n"
        "\t[1] - Cadastro de usuarios\n"
        "\t[2] - Cadastro de Clientes\n"
        "\t[3] - Cadastro de Produtos\n"
        "\t[4] - Cadastro de Categoria\n"
        "\t[5] - Retornar ao Menu Principa\n");
    fgets(opcao, sizeof(opcao), stdin);
    return atoi(opcao);
}

void main_cadastros()
{
    int opcao;

    Cliente cliente;
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;
    FILE *arquivo;
    do
    {
        opcao = menu_cadastros();
        switch ((opcao))
        {
        case 1:
            cadastrar_usuario();
            printf("---------------------------");
            show_usuarios();
            break;
        case 2:
            cadastrarCliente();
            break;
        case 3:
             // Aqui usei realloc para sempre termos memória e espaço no "deposito" e não passar por cima do estoque antigo
            pt_Produtos = realloc(pt_Produtos, (quantidade + 1) * sizeof(Cadastrar_Produtos));
            if (pt_Produtos == NULL)
            {
                printf("erro ao alocar memória!!!\n");
                return;
            }

            cadastrar_Informacao_Produtos(&pt_Produtos[quantidade]);
            FILE *arquivo = fopen(ARQUIVO, "a+");
            if (arquivo == NULL)
            {
                printf("ERRO AO ABRIR O ARQUIVO!!!\n");
                return;
            }

            salvar_Produtos_Arquivo(&pt_Produtos[quantidade], arquivo);
            fclose(arquivo);

            quantidade++;
            break;
        case 4:
            cadastrarCategorias();
            break;
        case 5:
            printf("Obrigado por usar o nosso sistema!!!\n");
            break;
        default:
            printf("Opcao Invalida!!!\n");
            break;
        }

    } while ((opcao) != 5);
}

// funcoes utilitarios
void removeEnterTexto(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0'; // remove o "enter/ \n" do texto
}

void open_create_file(char *arquivo, FILE **cftPtr)
{
    // printf(arquivo, cftPtr);
    if ((*cftPtr = fopen(arquivo, "a+")) == NULL)
    {
        printf("O arquivo nao pode ser aberto");
        return;
    }
}

void cadastrar_usuario()
{
    FILE *arq;
    char usuario[15], password[12];
    int tipo_user = 2;
    int invalido = 0;

    printf("----Cadastro de usuario----\n");
    do
    { // nome de usuario;
        printf("Insira o nome de usuario [8] ~ [12] caracteres\n");
        fgets(usuario, sizeof(usuario), stdin);
        removeEnterTexto(usuario);
        invalido = 1;

        if (strlen(usuario) < 8 || strlen(usuario) > 12)
        {
            printf("Nome de usuario invalido\n");
            invalido = 0;
        }

    } while (invalido == 0);

    do
    { // senha do usuario;
        printf("Insira a senha de usuario [6] ~ [8] caracteres\n");
        fgets(password, sizeof(password), stdin);
        removeEnterTexto(password);
        invalido = 1;

        if (strlen(password) < 6 || strlen(password) > 8)
        {
            printf("Senha de usuario invalido\n");
            invalido = 0;
        }
    } while (invalido == 0);

    do
    { // tipo de usuario;
        printf("Insira o tipo de usuario [1] - Administrador, [2] - Usuario padrao \n");
        scanf("%d", &tipo_user);
        invalido = 1;

        if (tipo_user != 1 && tipo_user != 2)
        {
            printf("Tipo Invalido\n");
            invalido = 0;
        }

    } while (invalido == 0);

    int id = contadorCaracterFile(USUARIOS, '\n') + 1;

    open_create_file(USUARIOS, &arq); // abrindo o arquivo e verificando

    fprintf(arq, "%d %s %s %d \n", id, usuario, password, tipo_user); // salvando as informacoes
    fclose(arq);                                                      // fechando o arquivo

    printf("Usuario cadastrado com sucesso\n");
}

int contadorCaracterFile(char *nome_arquivo, char caracter_desejado)
{ // funcao para contar quantos caracteres especificos possuem em um arquivo
    int contador = 0;
    char caracter;
    FILE *arquivo = fopen(nome_arquivo, "r"); // abrindo o arquivo

    if (arquivo == NULL)
    { // caso de um erro ao abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    // fgetc le um caracter por vez do arquivo, o caracter lido e armazenado na variavel
    // o loop continua ate o final do arquivo
    while ((caracter = fgetc(arquivo)) != EOF)
    {
        if (caracter == caracter_desejado)
        {
            contador++; // incrementando o contador
        }
    }

    fclose(arquivo);

    return contador;
}

void cadastrarCliente()
{
    Cliente new_cliente;
    char *nome_arquivo = DADOSCLIENTES; // declarando como ponteiro, para usar dentro do fopen da funcao a seguir
    int contador = 0;
    FILE *arquivo;

    // Verifica se o arquivo existe e conta as linhas
    arquivo = fopen(nome_arquivo, "a+");
    if (verificaArquivo(arquivo))
        return;
    fclose(arquivo);

    contador = contadorCaracterFile(nome_arquivo, '\n');
    new_cliente.cod = contador + 1;

    limparBuffer(); // verifica a entrada/limpa buffer // ajuste da funcao do Wesley

    printf("Insira o Nome: \t");
    fgets(new_cliente.nome, sizeof(new_cliente.nome), stdin);
    removeEnterTexto(new_cliente.nome); // removendo o ("enter",\n) da string

    printf("Insira um nome social: \t");
    fgets(new_cliente.nome_social, sizeof(new_cliente.nome_social), stdin);
    removeEnterTexto(new_cliente.nome_social);

    do
    { // verificacao simples para o tamanho do cpf
        printf("Insira o cpf: \t");
        fgets(new_cliente.cpf, sizeof(new_cliente.cpf), stdin);
        removeEnterTexto(new_cliente.cpf);

    } while ((strlen(new_cliente.cpf) != 11));
    limparBuffer();

    printf("Insira o nome da rua ou o numero da residencia: \t");
    fgets(new_cliente.rua_num, sizeof(new_cliente.rua_num), stdin);
    removeEnterTexto(new_cliente.rua_num);

    printf("Insira o bairro: \t");
    fgets(new_cliente.bairro, sizeof(new_cliente.bairro), stdin);
    removeEnterTexto(new_cliente.bairro);

    printf("Insira o numero de telefone: \t");
    fgets(new_cliente.celular, sizeof(new_cliente.celular), stdin);
    removeEnterTexto(new_cliente.celular);

    arquivo = fopen(nome_arquivo, "a");
    if (verificaArquivo(arquivo))
    {
        return;
    }

    fprintf(arquivo, "%d,%s,%s,%s,%s,%s,%s\n", // salvando as informacoes no arquivo
            new_cliente.cod, new_cliente.nome, new_cliente.nome_social,
            new_cliente.cpf, new_cliente.rua_num, new_cliente.bairro, new_cliente.celular);

    fclose(arquivo); // fechando o arquivo
}

int verificaArquivo(FILE *arquivo)
{
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo"); // mostra erro mais detalhado
        return 1;                          // indica erro
    }
    return 0; // tudo certo
}


void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt)
{
    printf("\n-- Cadastrar Produto --\n");
    int validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("ID: ");
        scanf("%d", &pt->id);
        getchar();
        validacao_de_Resposta++;
    } while (pt->id <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    printf("Descrição: ");
    fgets(pt->descricao_Produto, max_caracter, stdin);
    strtok(pt->descricao_Produto, "\n"); // Remove o \n do final
    pular_Linha();

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado não é 1 ou 2 ou 3! Digite um valor valido...\n");

        printf("Categoria (1-Alimentos, 2-Limpeza, 3-Panificação): ");
        scanf("%d", &pt->categoria_Produto);
        validacao_de_Resposta++;
    } while (pt->categoria_Produto != 1 && pt->categoria_Produto != 2 && pt->categoria_Produto != 3);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("VALOR DE COMPRA: ");
        scanf("%f", &pt->Preco_De_Compra);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Preco_De_Compra <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("PREÇO DE VENDA: ");
        scanf("%f", &pt->Preco_De_Venda);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Preco_De_Venda <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("QUANTIDADE ESTOQUE: ");
        scanf("%d", &pt->Quantidade_em_Estoque);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Quantidade_em_Estoque <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("ESTOQUE MINÍMO: ");
        scanf("%d", &pt->estoque_Minimo);
        getchar();
        validacao_de_Resposta++;
    } while (pt->estoque_Minimo <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;
}


void salvar_Produtos_Arquivo(Cadastrar_Produtos *pt, FILE *arquivo)
{
    fprintf(arquivo, "%d;%s;%d;%.2f;%.2f;%d;%d\n",
            pt->id,
            pt->descricao_Produto,
            pt->categoria_Produto,

            pt->Preco_De_Compra,
            pt->Preco_De_Venda,
            pt->Quantidade_em_Estoque,
            pt->estoque_Minimo);
}

void cadastrarCategorias(){
    FILE *arq;
    int invalido;
    char categoria[30];
    
    printf("----Cadastro de Categoria----\n");
    do{
        printf("Insira a Categoria do produto\n");
        fgets(categoria, sizeof(categoria), stdin);
         removeEnterTexto(categoria); // removendo o ("enter",\n) da string
        invalido = 1;
        
        if(strlen(categoria) < 2){
            printf("Categoria invalida\n");
            invalido = 0;
        }
    }while(invalido == 0);

    int id = contadorCaracterFile(CATEGORIAS, '\n') + 1; // funcao para contar os "enter || \n" do arquivo, add + 1 pq ele inicia em 0

    open_create_file(CATEGORIAS, &arq); // abrindo o arquivo e verificando

    fprintf(arq, "%d %s", id, categoria);
    fclose(arq);

    printf("Categoria cadastrado com sucesso");
}


void show_usuarios()
{
    char usuario[15], password[12];
    int tipo_user;
    int id;
    FILE *arq;

    if ((arq = fopen(USUARIOS, "r")) == NULL)
    {
        printf("O arquivo nao pode ser aberto.\n");
    }
    else
    {
        printf("%-10s%-13s%-10s%-5s\n", "ID", "Nome", "Senha", "Tipo Usuario");

        // Lê os dados enquanto fscanf retornar 4 itens corretamente lidos
        while (fscanf(arq, "%d %s %s %d", &id, usuario, password, &tipo_user) == 4)
        {
            printf("%-10d%-13s%-10s%-6d\n", id, usuario, password, tipo_user);
        }

        fclose(arq);
    }
}

void limparBuffer()
{
    int ch;
    // descarte todos os caracter que ainda estao no buffer, como \n
    // garantindo que o próximo comando de entrada comece em uma entrada limpa
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}


