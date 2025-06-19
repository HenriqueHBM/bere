#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines ------------------
#define USUARIOS "Usuarios.dat" // nome do arquivo binario para salvar os usuarios
#define DADOSCLIENTES "DadosClientes.dat"
#define max_caracter 100               // max de caracter para a descrição
#define ARQUIVO "cadastrar&vendas.dat" // renomeando o arquivo que vai ser valvo
#define CATEGORIAS "Categorias.dat"
#define PRODUTOS "Produtos.dat"


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

typedef struct
{
    int id;
    char produto[max_caracter];
    char categoria_produto[max_caracter];
    float preco_compra;
    float margem_lucro;
    float preco_venda;
    int qtde_estoque;
    int estoque_minimo;
} Produtos;

typedef struct{
    int id; 
    char categoria[30];
} Categorias;

// declaracao das funcoes ------------
int menu_principal();
int menu_cadastros();
void main_cadastros();
void removeEnterTexto(char *);
int contadorCaracterFile(char *, char);
void cadastrar_usuario();
void show_usuarios();
void cadastrar_cliente();
int verificaArquivo(FILE *arquivo);
void limparBuffer();
void cadastrar_categorias();
void cadastrar_produto();
void open_create_file(char *arquivo, FILE **cftPtr);

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
            break;
        case 2:
            cadastrar_cliente();
            break;
        case 3:
            cadastrar_produto();
            break;
        case 4:
            int count_categorias = contadorCaracterFile(CATEGORIAS, '\n');
            if(count_categorias == 0 ){
                printf("Sem categoria cadastrada, realizar o cadastro de uma antes\n");
            }else{
                cadastrar_categorias();
            }
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

void cadastrar_cliente()
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

void cadastrar_produto(){
    FILE *arq;
    Produtos prod;
    Categorias cat;
    int opcao ;
    int invalido = 0;

    printf("---------Cadastrar Produto---------\n");

    printf("Produto\n");
    fgets(prod.produto, sizeof(Produtos), stdin);
    removeEnterTexto(prod.produto);

    if((arq = fopen(CATEGORIAS, "r")) == NULL){
        printf("deu PT");
    }

    int count_categorias = contadorCaracterFile(CATEGORIAS, '\n') + 1;
    printf("Categorias:\n");
    while(fscanf(arq, "%d %s", &cat.id, cat.categoria)!= EOF ){
        printf("[%d] - %s\n", cat.id, cat.categoria);
    }
    fclose(arq);
    do{
        printf("Escolha uma opcao: \n");
        scanf("%d", &opcao);
        invalido = 1;

        if((opcao <= 0 || opcao > count_categorias)){
            invalido = 0;
        }
        
    }while( invalido == 0);

    if((arq = fopen(CATEGORIAS, "r")) == NULL){
        printf("deu PT");
    }

    while(fscanf(arq, "%d %s", &cat.id, cat.categoria)!= EOF ){
        if(opcao == cat.id){
            strcpy(prod.categoria_produto, cat.categoria);
        }
    }
    fclose(arq);

    do
    {
        printf("Valor de compra\n");
        scanf("%f", &prod.preco_compra);
    } while (prod.preco_compra <= 0);
    
    do
    {
        printf("Valor de venda\n");
        scanf("%f", &prod.preco_venda);
    } while (prod.preco_venda <= 0);

    do
    {
        printf("Quantidade em estoque\n");
        scanf("%d", &prod.qtde_estoque);
    } while (prod.qtde_estoque <= 0);

    do
    {
        printf("Estoque minimo\n");
        scanf("%d", &prod.estoque_minimo);
    } while (prod.estoque_minimo <= 0);

    int id = contadorCaracterFile(PRODUTOS, '\n') + 1;

    open_create_file(PRODUTOS, &arq); // abrindo o arquivo e verificando

    fprintf(arq, "%d %s %s %.2f %.2f %.2f %d %d \n", 
        id,
        prod.produto,
        prod.categoria_produto,
        prod.preco_compra,
        prod.margem_lucro,
        prod.preco_venda,
        prod.qtde_estoque,
        prod.estoque_minimo    
    ); // salvando as informacoes
    fclose(arq);                                                      // fechando o arquivo

    printf("Produto cadastrado com sucesso\n");
}

void cadastrar_categorias(){
    FILE *arq;
    char categoria[30];
    
    printf("----Cadastro de Categoria----\n");
    printf("Insira a Categoria do produto\n");
    fgets(categoria, sizeof(categoria), stdin);

    int id = contadorCaracterFile(CATEGORIAS, '\n') + 1; // funcao para contar os "enter || \n" do arquivo, add + 1 pq ele inicia em 0

    open_create_file(CATEGORIAS, &arq); // abrindo o arquivo e verificando

    fprintf(arq, "%d %s", id, categoria);
    fclose(arq);

    printf("Categoria cadastrado com sucesso\n");
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

void limparBuffer()
{
    int ch;
    // descarte todos os caracter que ainda estao no buffer, como \n
    // garantindo que o próximo comando de entrada comece em uma entrada limpa
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
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
        printf("O arquivo nao pode ser aberto\n");
        return;
    }
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

int verificaArquivo(FILE *arquivo)
{
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo\n"); // mostra erro mais detalhado
        return 1;                          // indica erro
    }
    return 0; // tudo certo
}