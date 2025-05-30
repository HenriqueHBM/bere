#include <stdio.h>
#include <stdlib.h>  // Funções utilitárias (malloc, free, exit, etc.)
#include <string.h>   // Manipulação de strings
#include <locale.h> // Para configurar o idioma e acentuação (setlocale)
#include <ctype.h> // Para função toupper()
#include <time.h>  // Trabalhar com data e hora
#include <math.h> // Operações matemáticas como pow, sqrt
//#include <windows.h> //usar a funcao do sleep no windows
#include <unistd.h> //usar a funcao do sleep no linux

/* Defines ---------------------------*/

#define max_caracter 100               // max de caracter para a descrição
#define max_cpf 12
#define max_inf 50
#define ARQUIVO "estoqueBere.bin"          // renomeando o arquivo que vai ser valvo
#define COMPRAS "vendasBere.bin"           // criando um novo arquivo para salvar as compras de cada cliente;
#define DADOSCLIENTES "DadosClientesBere.txt"

/* Fim Defines ----------------------*/

/* Structs --------------------------*/

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
    int id_produto;
    char descricao[max_caracter];
    float preco_unitario;
    int quantidade;
    float subtotal;
} ItemCarrinho;

typedef struct
{
    ItemCarrinho *itens;
    int qtd_itens;
    int ID_Cliente;
    float total_final;
    char categoria_Pagamento;
    int num_Venda;
    char situacao_Do_pagamento;
    float valor_Recebido;
} Carrinho;

typedef struct
{
    int cod;
    char nome[max_caracter];
    char nome_social[max_caracter];
    char cpf[max_cpf];
    char rua_num[max_inf];
    char bairro[max_inf];
    char celular[max_inf];
} Cliente;

/* Fim Structs ----------------------------*/

/* Variaveis Globais ----------------------*/

Carrinho *clientes = NULL; //realloc para cada cliente
int qtd_clientes = 0;
int idx; //numero do cliente, carrinho do cliente 1 = idx 0
float saldo_caixa = 0.0; // abertura de caixa
int movimentos;
int qtd_movimentos;

/* Fim Variaveis Globais ------------------*/

/* Inicializando Funcoes ------------------*/

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void mostrar_Produtos_Compra_Binario();
void venda_de_Produtos();
void atualizar_Estoque(int idProduto, int quantidadeComprada);
void nota_E_Desconto();
void nota_fiscal();
void salvar_Compra();
void pagamento();
void cadastrarCliente(Cliente *);
int contadorCaracterFile(char *, char);
void limparBuffer(); //Limpa a entrada do teclado, removendo o ENTER pendente.
int verificaArquivo(FILE *arquivo); // verifica se conseguiu abrir um arquivo. 
void removeEnterTexto(char *); //Remove o ENTER (\n) no final das strings.
int exibirMenu();
void exibirVendas();
void exibirCadastros();
void pular_Linha();
void limpar_Terminal();
void abrirCaixa();
void fecharCaixa();
void menuRelatorios();
void relatorioProdutos();
void relatorioVendas();
void relatorioClientes();

/* Fim Inicializando Funcoes --------------*/

int main() {
    setlocale(LC_ALL, "portuguese");
    int escolha;

    do {
        limpar_Terminal();
        escolha = exibirMenu();

        switch (escolha)
        {
            case 1:
                exibirCadastros();
            break;

            case 2:
                exibirVendas();
            break;

            case 3:
                abrirCaixa();
            break;

            case 4:
                fecharCaixa();
            break;

            case 5:
                menuRelatorios();
            break;

            case 6:
                printf("\n\t\tSistema encerrado. Volte sempre!\n\n");
            break;

            default:
                printf("\nERRO: Opção inválida! Tente novamente.\n");
                sleep(1); // Pausa para visualização do erro
            break;
        }
    } while (escolha != 6);

    return 0;
}

int exibirMenu() {
    char opcao[10];

    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║           MERCADO BERE               ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║ 1. Cadastros                         ║\n");
    printf("║ 2. Vendas                            ║\n");
    printf("║ 3. Abertura de caixa                 ║\n");
    printf("║ 4. Fechamento de caixa               ║\n");
    printf("║ 5. Relatórios                        ║\n");
    printf("║ 6. Sair                              ║\n");
    printf("╚══════════════════════════════════════╝\n");

    printf("\nDigite sua opção → ");
    fgets(opcao, sizeof(opcao), stdin);

    return atoi(opcao);
}

void pular_Linha()
{
    printf("\n");
}

void limpar_Terminal()
{
    //vai verificar qual sistema operacional o user esta usando
    #ifdef _WIN32           //windows
        system("cls");      //limpa o terminal
    #else
        system("clear");
    #endif
}

void exibirCadastros()
{
    char opcao[10];
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;
    FILE *arquivo;
    while (1)
    {
        printf("%50s\n", "----------- CADASTROS -----------");
        printf("%50s\n", "=================================");
        printf("%50s\n", "| [1] CLIENTES                  |");
        printf("%50s\n", "---------------------------------");
        printf("%50s\n", "| [2] PRODUTOS                  |");
        printf("%50s\n" ,"---------------------------------");
        printf("%50s\n", "| [3] VOLTAR AO MENU PRINCIPAL  |");
        printf("%50s\n", "=================================");

        printf("Escolha uma opção: ");
        fgets(opcao, sizeof(opcao), stdin);

        // caso nao exista cria, se nao so atualiza;
        Cliente cliente;
        switch (atoi(opcao))
        {
            case 1:
                cadastrarCliente(&cliente);
            break;

            case 2:
                //atribui ao ponteiro a função REALLOC que almenta nosso espaço na memória de modo que não ficamos sem "espaço"
                //a cada cadastro ele vai realocar memoria do tamanho da strucct na memoria
                pt_Produtos = realloc(pt_Produtos, (quantidade + 1) * sizeof(Cadastrar_Produtos));
                if (pt_Produtos == NULL)
                {
                    printf("erro ao alocar memória!!!\n");
                    return;
                }

                //chama a função para cadastrar os produtos em nosso arquivo binário
                cadastrar_Informacao_Produtos(&pt_Produtos[quantidade]);

                FILE *arquivo = fopen(ARQUIVO, "ab"); // adicionar ao final
                if (arquivo == NULL)
                {
                    printf("ERRO AO ABRIR O ARQUIVO!!!\n");
                    return;
                }

                fwrite(&pt_Produtos[quantidade], sizeof(Cadastrar_Produtos), 1, arquivo);
                fclose(arquivo);

                quantidade++;
            break;

            case 3:
                printf("Voltando ao menu principal...\n");
                return;
            break;

            default:
                printf("Opcao Invalida!!");
            break;
        }

        printf("\n");
    }
}

void cadastrarCliente(Cliente *new_cliente)
{

    char *nome_arquivo = DADOSCLIENTES; // declarando como ponteiro, para usar dentro do fopen da funcao a seguir
    int contador = 0;
    FILE *arquivo;

    // Verifica se o arquivo existe e conta as linhas
    arquivo = fopen(nome_arquivo, "a+");
    if (verificaArquivo(arquivo))
        return;
    fclose(arquivo);

    contador = contadorCaracterFile(nome_arquivo, '\n');
    new_cliente->cod = contador + 1;

    limparBuffer(); // verifica a entrada/limpa buffer // ajuste da funcao do Wesley

    printf("Insira o Nome: \t");
    fgets(new_cliente->nome, sizeof(new_cliente->nome), stdin);
    removeEnterTexto(new_cliente->nome); // removendo o ("enter",\n) da string

    printf("Insira um nome social: \t");
    fgets(new_cliente->nome_social, sizeof(new_cliente->nome_social), stdin);
    removeEnterTexto(new_cliente->nome_social);

    do
    { // verificacao simples para o tamanho do cpf
        printf("Insira o cpf: \t");
        fgets(new_cliente->cpf, sizeof(new_cliente->cpf), stdin);
        removeEnterTexto(new_cliente->cpf);

    } while ((strlen(new_cliente->cpf) != 11));
    limparBuffer();

    printf("Insira o nome da rua ou o numero da residencia: \t");
    fgets(new_cliente->rua_num, sizeof(new_cliente->rua_num), stdin);
    removeEnterTexto(new_cliente->rua_num);

    printf("Insira o bairro: \t");
    fgets(new_cliente->bairro, sizeof(new_cliente->bairro), stdin);
    removeEnterTexto(new_cliente->bairro);

    printf("Insira o numero de telefone: \t");
    fgets(new_cliente->celular, sizeof(new_cliente->celular), stdin);
    removeEnterTexto(new_cliente->celular);

    arquivo = fopen(nome_arquivo, "a");
    if (verificaArquivo(arquivo))
    {
        return;
    }

    fprintf(arquivo, "%d,%s,%s,%s,%s,%s,%s\n", // salvando as informacoes no arquivo
            new_cliente->cod, new_cliente->nome, new_cliente->nome_social,
            new_cliente->cpf, new_cliente->rua_num, new_cliente->bairro, new_cliente->celular);

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

void removeEnterTexto(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0'; // remove o "enter/ \n" do texto
}

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt)
{
    printf("\n=== CADASTRO DE PRODUTO ===\n");

    printf("\n-- Cadastrar Produto --\n");
    int validacao_de_Resposta = 0;

    // Validação de ID
    do {
        printf("▶ ID do produto (número positivo): ");
        if (scanf("%d", &pt->id) != 1 || pt->id <= 0) {
            printf("ERRO: ID inválido!\n");
            limparBuffer();
        }
    } while (pt->id <= 0);

    limparBuffer();
    printf("▶ Descrição: ");
    fgets(pt->descricao_Produto, max_caracter, stdin);
    strtok(pt->descricao_Produto, "\n");

    // Validação de categoria
    do {
        printf("▶ Categoria (1-Alimentos | 2-Limpeza | 3-Padaria): ");
        scanf("%d", &pt->categoria_Produto);
        if (pt->categoria_Produto < 1 || pt->categoria_Produto > 3) {
            printf("ERRO: Categoria inexistente!\n");
        }
    } while (pt->categoria_Produto < 1 || pt->categoria_Produto > 3);

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

void exibirVendas()
{
    int opcao;
    do
    {

        printf("\tÁREA  DE VENDAS\n");
        printf("========================\n");
        printf("21 - NOVA VENDA\n22 - RETIRANDA DE CAIXA(SANGUIA)\n23 - PAGAMENTO\n24 - RETORNAR AO MENU PRÍNCIPAL\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
            case 21:
                venda_de_Produtos();
                nota_E_Desconto();
            break;

            case 22:
                // retirada de caixa
            break;

            case 23:
                pagamento();
            break;

            case 24:
                printf("\n\tVoltando ao menu principal...");
            break;

        default:
            printf("\tOpcao Invalida!!\n");
            break;
        }
    } while (opcao != 24);
}

void venda_de_Produtos()
{
    int codigo, quantidade;
    char linha[256], escolha_continuar_Comprando;
    idx = qtd_clientes; //define qual e o índice atual do carrinho do cliente/ serve para saber em qual posição da lista de clientes estamos trabalhando
    Cadastrar_Produtos temp;

    if (qtd_clientes == 0) { // verifica se existe carrinho criado, então  cria um espaço na memória para um carrinho
        clientes = malloc(sizeof(Carrinho));
    } else {
        clientes = realloc(clientes, (qtd_clientes + 1) * sizeof(Carrinho)); //  se existem carrinhos anteriores, então o programa aumenta o espaço na memória
    }

    clientes[qtd_clientes].itens = NULL;
    clientes[qtd_clientes].qtd_itens = 0;
    clientes[qtd_clientes].ID_Cliente = qtd_clientes + 1; // ou outro ID único
    clientes[qtd_clientes].total_final = 0.0;


    do
    {
        #ifdef _WIN32 // limpa o terminal dependendo do sistema operacional
            system("cls");
        #else
            system("clear");
        #endif

        mostrar_Produtos_Compra_Binario();

        // Mostrar resumo da compra
        printf("\n\n============= NOTA FISCAL (Prévia) =============\n");
        printf("ITEM\tQTD\tUNITÁRIO\tSUBTOTAL\n");

        for (int i = 0; i < clientes[idx].qtd_itens; i++)
        {
            printf("%s\t%d\tR$ %.2f\t\tR$ %.2f\n",
                clientes[idx].itens[i].descricao,
                clientes[idx].itens[i].quantidade,
                clientes[idx].itens[i].preco_unitario,
                clientes[idx].itens[i].subtotal);
        }

        //Abrindo o arquivo que está nosso aetoque para poder verificar os dados dos produtos;
        FILE *arquivo = fopen(ARQUIVO, "rb"); // abrindo binário
        if (arquivo == NULL)
        {
            printf("\nErro ao abrir o arquivo de produtos!!!\n");
            return;
        }

        printf("\nCarrinho de compras: ");

        printf("\nInforme o codigo do produto a ser comprado: ");
        scanf("%d", &codigo);

        int produto_encontrado = 0;

        //vai ler produto por produto dentro do nosso arquivo binario, até o final, e vai retornar 1 se leu 0 não e armezenado na variavel temp
        while (fread(&temp, sizeof(Cadastrar_Produtos), 1, arquivo) == 1)
        {
            if (codigo == temp.id)
            {
                produto_encontrado = 1; //true

                printf("\nProduto encontrado: %s", temp.descricao_Produto);
                printf("\nQuantidade em estoque: %d", temp.Quantidade_em_Estoque);
                printf("\nInforme a quantidade desejada: ");
                scanf("%d", &quantidade);

                if (quantidade <= temp.Quantidade_em_Estoque) { //  quantidade suficiente no estoque ?
                    float valor = temp.Quantidade_em_Estoque - quantidade; // veririca se o estoque nao vai ficar negativo com a compra
                    if (valor <= temp.estoque_Minimo)
                        printf("\nAviso: estoque no limite mínimo!!!!");

                    atualizar_Estoque(codigo, quantidade); // essa função também precisa estar adaptada ao binário

                    clientes[idx].itens = realloc(clientes[idx].itens, // criando espaço na memória para adicionar mais um item no carrinho do cliente atual
                                                (clientes[idx].qtd_itens + 1) * sizeof(ItemCarrinho));
                    if (clientes[idx].itens == NULL) {
                        printf("Erro ao alocar memória!\n");
                        fclose(arquivo);
                        return;
                    }

                    ItemCarrinho *item = &clientes[idx].itens[clientes[idx].qtd_itens]; // variavel para preencher os dados a serguir

                    item->id_produto = temp.id;
                    strcpy(item->descricao, temp.descricao_Produto);
                    item->preco_unitario = temp.Preco_De_Venda;
                    item->quantidade = quantidade;
                    item->subtotal = quantidade * temp.Preco_De_Venda;

                    clientes[idx].total_final += item->subtotal; //atualiza o total da compra e aumenta o contador de itens
                    clientes[idx].qtd_itens++;

                    printf("\nItem adicionado ao carrinho!");
                } else {
                    printf("\nQuantidade no estoque insuficiente!"); 
                }

                break; // já encontrou o produto
            }
        }

        if (!produto_encontrado) // caso nao encontre o produto
            printf("\nCódigo do produto inválido ou não encontrado.");

        fclose(arquivo);

        do {
            printf("\nNovo item no carrinho de compra (s/n): ");
            scanf(" %c", &escolha_continuar_Comprando);
            if (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n')
                printf("\nOpção inválida!");

        } while (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n');

    } while (escolha_continuar_Comprando == 's'); // enquanto a opcao for s, vai poder add mais itens ao carrinho

}

void mostrar_Produtos_Compra_Binario() {
    FILE *arquivo = fopen(ARQUIVO, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Cadastrar_Produtos produto;

    printf("\nProdutos disponíveis:\n");
    printf("ID\tDescrição\t\tEstoque\t\tPreço de Venda\n");
    printf("---------------------------------------------------------------\n");

    //vai ler produto por produto dentro do nosso arquivo binario, até o final, e vai retornar 1 se leu 0 não
    while (fread(&produto, sizeof(Cadastrar_Produtos), 1, arquivo) == 1) {
        printf("%-5d %-25s %-10d R$ %.2f\n",
               produto.id,
               produto.descricao_Produto,
               produto.Quantidade_em_Estoque,
               produto.Preco_De_Venda);
    }

    fclose(arquivo);
}

void atualizar_Estoque(int idProduto, int quantidadeComprada) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (!arquivo) {
        printf("\nERRO: Arquivo de estoque inacessível!\n");
        return;
    }

    Cadastrar_Produtos produto;
    int encontrado = 0;

    while (fread(&produto, sizeof(Cadastrar_Produtos), 1, arquivo)) {
        if (produto.id == idProduto) {
            produto.Quantidade_em_Estoque -= quantidadeComprada;

            // Proteção contra estoque negativo
            if (produto.Quantidade_em_Estoque < 0) {
                produto.Quantidade_em_Estoque = 0;
            }

            // Volta o ponteiro para reescrever o produto atualizado
            //FSEEK - posicionando o ponteiro de posição no arquivo onde achou o ID
            //FWRITE - grava - o tamanho da struct, no exato ligar onde paraou o ponteiro
            fseek(arquivo, -sizeof(Cadastrar_Produtos), SEEK_CUR);
            fwrite(&produto, sizeof(Cadastrar_Produtos), 1, arquivo);
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nAVISO: Produto ID %d não encontrado!\n", idProduto);
    }
}

void nota_E_Desconto()
{
    float total_compra = 0, desconto_no_totalC;

    printf("\n\n============= NOTA FISCAL =============\n");
    printf("ITEM\tQTD\tUNITÁRIO\tSUBTOTAL\n");

    for (int i = 0; i < clientes[idx].qtd_itens; i++)
    {
        printf("%s\t%d\tR$ %.2f\t\tR$ %.2f\n",
            clientes[idx].itens[i].descricao,
            clientes[idx].itens[i].quantidade,
            clientes[idx].itens[i].preco_unitario,
            clientes[idx].itens[i].subtotal);

        total_compra += clientes[idx].itens[i].subtotal;
    }

    printf("\nTotal da compra: R$ %.2f", total_compra);

    printf("\nHá desconto? Informe a porcentagem (ou 0 para nenhum): ");
    if (scanf("%f", &desconto_no_totalC) == 1 && desconto_no_totalC > 0)
    {
        float valor_desconto = total_compra * (desconto_no_totalC / 100.0);
        total_compra -= valor_desconto;
        printf("\nDesconto aplicado: R$ %.2f (%.2f%%)", valor_desconto, desconto_no_totalC);
    }

    printf("\nTotal final com desconto: R$ %.2f\n", total_compra);
}

//          SANGUIA

void pagamento()
{
    if (clientes[idx].total_final == 0)
    {
        printf("\nNenhum produto foi comprado...\n");
        return;
    }

    int opcao;
    int aprovacao_maquina;
    float valor_Recebido = 0, valor_restante;
    char opcao_parcial;

    do
    {
        printf("\n===== Forma de Pagamento =====\n");
        printf("1 - Cartão\n");
        printf("2 - Dinheiro\n");
        printf("3 - Retornar ao menu principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // limpar buffer
            continue;
        }
        while (getchar() != '\n'); // limpar buffer

        switch (opcao)
        {
            case 1: // Cartão
                //NOTA é todos os produtos comprados
                nota_fiscal();
                clientes[idx].categoria_Pagamento = 'c';

                do
                {
                    printf("\nPagamento na maquininha (1 = OK, 0 = Não OK): ");
                    if (scanf("%d", &aprovacao_maquina) != 1 || (aprovacao_maquina != 0 && aprovacao_maquina != 1))
                    {
                        printf("Opção inválida! Tente novamente.\n");
                        while (getchar() != '\n');
                        aprovacao_maquina = -1; // força repetir o loop
                        continue;
                    }
                    while (getchar() != '\n');
                } while (aprovacao_maquina == -1);

                if (aprovacao_maquina == 1)
                {
                    clientes[idx].situacao_Do_pagamento = 'f';
                    clientes[idx].valor_Recebido = clientes[idx].total_final;
                    //salva no arquivo vendasBere
                    salvar_Compra();
                    printf("\nPagamento efetuado com sucesso! Obrigado pela preferência.\n");
                }
                else
                {
                    printf("\nPagamento no cartão não aprovado.\n");
                }
                break;

            case 2: // Dinheiro
                nota_fiscal();
                clientes[idx].categoria_Pagamento = 'd';

                valor_Recebido = 0;
                valor_restante = clientes[idx].total_final;

                while (valor_Recebido < clientes[idx].total_final) // repete ate pagar tudo
                {
                    printf("\nTotal a pagar: R$ %.2f", clientes[idx].total_final);
                    printf("\nInforme o valor recebido (dinheiro): R$ ");
                    if (scanf("%f", &valor_Recebido) != 1 || valor_Recebido <= 0)
                    {
                        printf("Valor inválido. Tente novamente.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');

                    if (valor_Recebido < valor_restante) //caso valor recebido menor que valore restante
                    {
                        valor_restante -= valor_Recebido; //desconta o valor restante 

                        printf("\nValor insuficiente. Faltam R$ %.2f", valor_restante);
                        printf("\nDeseja pagar o restante com cartão? (s/n): ");
                        scanf(" %c", &opcao_parcial); 
                        while (getchar() != '\n');

                        if (opcao_parcial == 's' || opcao_parcial == 'S')
                        {
                            clientes[idx].categoria_Pagamento = 'd'; // ou 'dc' para dinheiro + cartão
                            clientes[idx].situacao_Do_pagamento = 'f'; //f finalizado
                            clientes[idx].valor_Recebido = clientes[idx].total_final;
                            salvar_Compra();
                            printf("\nPagamento efetuado com sucesso! Obrigado.\n");
                            break;
                        }
                        else if (opcao_parcial == 'n' || opcao_parcial == 'N')
                        {
                            printf("\nPor favor, informe o valor restante em dinheiro.\n");
                            // valor_Recebido continuará acumulando no próximo loop
                            valor_Recebido = clientes[idx].total_final - valor_restante;
                        }
                        else
                        {
                            printf("Opção inválida, assumindo 'n'.\n");
                        }
                    }
                    else
                    {
                        float troco = valor_Recebido - clientes[idx].total_final; // verificnado se possui troco
                        if (troco > 0)
                            printf("\nTroco: R$ %.2f\n", troco);

                        clientes[idx].situacao_Do_pagamento = 'f';
                        clientes[idx].valor_Recebido = valor_Recebido;
                        salvar_Compra();
                        printf("\nPagamento efetuado com sucesso! Obrigado.\n");
                        break;
                    }
                }
                break;

            case 3:
                printf("\nRetornando ao menu principal...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 3 && !(opcao == 1 && aprovacao_maquina == 1));
}

void nota_fiscal() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║           NOTA FISCAL - MERCADO BERE        ║\n");
    printf("╠══════════════════╦══════╦═════════╦════════╣\n");
    printf("║ PRODUTO          ║ QTD  ║ PREÇO   ║ TOTAL  ║\n");
    printf("╠══════════════════╬══════╬═════════╬════════╣\n");

    for (int i = 0; i < clientes[idx].qtd_itens; i++) {
        printf("║ %-16s ║ %-4d ║ %-7.2f ║ %-6.2f ║\n",
            clientes[idx].itens[i].descricao,
            clientes[idx].itens[i].quantidade,
            clientes[idx].itens[i].preco_unitario,
            clientes[idx].itens[i].subtotal);
    }

    printf("╠══════════════════╩══════╩═════════╩════════╣\n");
    printf("║ TOTAL: R$ %-34.2f ║\n", clientes[idx].total_final);
    printf("╚════════════════════════════════════════════╝\n");
}

void salvar_Compra()
{
    // Verifica se há itens no carrinho
    if (clientes[idx].qtd_itens == 0) {
        printf("\nAVISO: Nenhum item para salvar!\n");
        return;
    }

    FILE *file = fopen(COMPRAS, "ab"); // modo binário append
    if (file == NULL) {
        printf("\nERRO CRÍTICO: Não foi possível abrir o arquivo de vendas!\n");
        return;
    }

    // Salva os dados do cliente/venda
    fwrite(&clientes[idx].ID_Cliente, sizeof(int), 1, file);
    fwrite(&clientes[idx].num_Venda, sizeof(int), 1, file);
    fwrite(&clientes[idx].categoria_Pagamento, sizeof(char), 1, file);
    fwrite(&clientes[idx].situacao_Do_pagamento, sizeof(char), 1, file);
    fwrite(&clientes[idx].valor_Recebido, sizeof(float), 1, file);

    // Salva a quantidade de itens no carrinho para poder ler depois
    fwrite(&clientes[idx].qtd_itens, sizeof(int), 1, file);

    // Salva os itens
    for(int i = 0; i < clientes[idx].qtd_itens; i++)
    {
        fwrite(&clientes[idx].itens[i].id_produto, sizeof(int), 1, file);
        fwrite(clientes[idx].itens[i].descricao, sizeof(char), max_caracter, file);
        fwrite(&clientes[idx].itens[i].preco_unitario, sizeof(float), 1, file);
        fwrite(&clientes[idx].itens[i].subtotal, sizeof(float), 1, file);
    }

    fclose(file);

    // Libera memória após salvar
    for (int i = 0; i < qtd_clientes; i++)
    {
        free(clientes[i].itens);
    }
    free(clientes);
}

void abrirCaixa() {
    // Verifica se o caixa já está aberto
    if (saldo_caixa > 0) {
        printf("\nAVISO: O caixa já está aberto com R$ %.2f\n", saldo_caixa);
        printf("Pressione Enter para continuar...");
        limparBuffer();
        getchar();
        return;
    }

    float valor;
    do {
        printf("\nABERTURA DE CAIXA\n");
        printf("Informe o valor inicial em dinheiro: R$ ");

        if (scanf("%f", &valor) != 1) {
            printf("ERRO: Valor inválido!\n");
            limparBuffer();
            continue;
        }

        if (valor < 0) {
            printf("ERRO: O valor não pode ser negativo!\n");
        }
    } while (valor < 0);

    saldo_caixa = valor;
    movimentos = 0; // Reseta contador de movimentos

    // Registra no arquivo de log
    FILE *log = fopen("caixa.log", "a");
    if (log) {
        time_t now = time(NULL);
        fprintf(log, "ABERTURA | %.2f | %s", valor, ctime(&now));
        fclose(log);
    }

    printf("\nCaixa aberto com R$ %.2f\n", valor);
    printf("Pressione Enter para continuar...");
    limparBuffer();
    getchar();
}


/* Funções para adicionar ao primeiro código */

void fecharCaixa() {
    // Verifica se o caixa está aberto
    if (saldo_caixa <= 0) {
        printf("\nERRO: O caixa não está aberto!\n");
        printf("Pressione Enter para continuar...");
        limparBuffer();
        getchar();
        return;
    }

    float total_dinheiro = 0;
    float total_cartao = 0;
    float total_misto = 0;
    int qtd_vendas = 0;

    FILE *arquivo = fopen(COMPRAS, "rb");
    if (arquivo) {
        Carrinho venda;
        while (fread(&venda, sizeof(Carrinho), 1, arquivo) == 1) {
            if (venda.situacao_Do_pagamento == 'f') { // Só conta vendas finalizadas
                qtd_vendas++;

                switch(venda.categoria_Pagamento) { // Soma os valores conforme a forma de pagamento
                    case 'd': total_dinheiro += venda.total_final; break; //dindin
                    case 'c': total_cartao += venda.total_final; break; //cartao
                    case 'b': total_misto += venda.total_final; break; //misto
                }
            }
        }
        fclose(arquivo);
    }

    // Cálculo do saldo teórico
    float saldo_teorico = saldo_caixa + total_dinheiro + total_misto;
    float saldo_real = saldo_teorico; // Em sistema real, aqui viria um input do valor físico

    // Exibe relatório
    printf("\n═╦══════════════════════════════════════╗\n");
    printf(" ║       FECHAMENTO DE CAIXA            ║\n");
    printf(" ╠══════════════════════════════════════╣\n");
    printf(" ║ %-20s R$ %-10.2f ║\n", "Saldo Inicial:", saldo_caixa);
    printf(" ║ %-20s R$ %-10.2f ║\n", "Vendas (Dinheiro):", total_dinheiro);
    printf(" ║ %-20s R$ %-10.2f ║\n", "Vendas (Cartão):", total_cartao);
    printf(" ║ %-20s R$ %-10.2f ║\n", "Vendas (Mistas):", total_misto);
    printf(" ╠══════════════════════════════════════╣\n");
    printf(" ║ %-20s R$ %-10.2f ║\n", "Saldo Teórico:", saldo_teorico);
    printf(" ║ %-20s R$ %-10.2f ║\n", "Saldo Real:", saldo_real);
    printf(" ╠══════════════════════════════════════╣\n");

    // Verifica divergência
    float diferenca = saldo_real - saldo_teorico;
    if (fabs(diferenca) > 0.01) { // Considera margem de erro de 1 centavo / valor absoluto de um número de ponto flutuante
        printf(" ║ ATENÇÃO: Divergência de R$ %-8.2f ║\n", diferenca);
    } else {
        printf(" ║ Caixa Fechado Corretamente       ║\n");
    }
    printf(" ╚══════════════════════════════════════╝\n");

    // Registra no arquivo
    FILE *log = fopen("caixa.log", "a");
    if (log) {
        time_t now = time(NULL); //  variável now do tipo time_t (que armazena tempo)/ time(NULL) pega o tempo atual do sistema
        fprintf(log, "FECHAMENTO | Teórico: %.2f | Real: %.2f | %s",
                saldo_teorico, saldo_real, ctime(&now)); // ctime(&now)  transforma o (timestamp) em uma string bonitinha
        fclose(log);
    }

    // Zera o caixa
    saldo_caixa = 0;

    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}

void menuRelatorios() {
    int opcaoRelat = 0;

    do {
        limpar_Terminal();
        printf("\n-------- RELATÓRIOS --------\n\n");
        printf("- [1] CLIENTES\n");
        printf("- [2] PRODUTOS\n");
        printf("- [3] VENDAS\n");
        printf("- [4] RETORNAR AO MENU\n");
        printf("\nSelecione a opção desejada: ");
        scanf("%d", &opcaoRelat);
        getchar();

        switch(opcaoRelat) {
            case 1:
                relatorioClientes();
                break;
            case 2:
                relatorioProdutos();
                break;
            case 3:
                relatorioVendas();
                break;
            case 4:
                printf("Retornando ao menu...\n\n");
                break;
            default:
                printf("Opção inválida, tente novamente.\n");
                break;
        }
    } while (opcaoRelat != 4);
}

void relatorioProdutos() {
    limpar_Terminal();
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("\nERRO: Arquivo de produtos não encontrado!\n");
        printf("Verifique se existem produtos cadastrados.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\n═══════════════════════════════════════════════════════════════\n");
    printf("                  RELATÓRIO DE PRODUTOS CADASTRADOS               \n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("| %-4s | %-20s | %-6s | %-9s | %-7s | %-7s |\n",
           "ID", "Descrição", "Categ.", "Preço", "Estoque", "Mínimo");
    printf("────────────────────────────────────────────────────────────────\n");

    Cadastrar_Produtos produto;
    int totalProdutos = 0;

    while (fread(&produto, sizeof(Cadastrar_Produtos), 1, arquivo) == 1) {
        // Formata categoria para texto
        char categoria[20];
        switch(produto.categoria_Produto) { // a categoria e separada por num, e faz um switch para colocar um nome para o usuario
            case 1: strcpy(categoria, "Alim."); break;
            case 2: strcpy(categoria, "Limpeza"); break;
            case 3: strcpy(categoria, "Padaria"); break;
            default: strcpy(categoria, "Inválida"); break;
        }

        // Verifica estoque baixo (destaque vermelho se abaixo do mínimo)
        if (produto.Quantidade_em_Estoque < produto.estoque_Minimo) {
            printf("| %-4d | %-20.20s | %-6s | R$ %-6.2f | %-7d | %-7d |\n",
                produto.id, produto.descricao_Produto, categoria,
                produto.Preco_De_Venda, produto.Quantidade_em_Estoque,
                produto.estoque_Minimo);
        } else {
            printf("| %-4d | %-20.20s | %-6s | R$ %-6.2f | %-7d | %-7d |\n",
                produto.id, produto.descricao_Produto, categoria,
                produto.Preco_De_Venda, produto.Quantidade_em_Estoque,
                produto.estoque_Minimo);
        }
        totalProdutos++;
    }

    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado no sistema.\n");
    }

    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Total de produtos: %d\n", totalProdutos);
    fclose(arquivo);

    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}

void relatorioVendas() {
    limpar_Terminal();
    FILE *arquivo = fopen(COMPRAS, "rb");
    if (arquivo == NULL) {
        printf("\nERRO: Nenhum dado de vendas encontrado!\n");
        printf("Realize vendas antes de gerar relatórios.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\n══════════════════════════════════════════════════════════\n");
    printf("                   RELATÓRIO DE VENDAS                     \n");
    printf("══════════════════════════════════════════════════════════\n");

    Carrinho venda;
    float totalGeral = 0;
    int totalVendas = 0;

    while (fread(&venda, sizeof(Carrinho), 1, arquivo) == 1) {
        printf("\nVENDA Nº: %d", venda.num_Venda);
        printf(" | Cliente: %d", venda.ID_Cliente);
        printf(" | Total: R$ %.2f\n", venda.total_final);

        // Formata tipo de pagamento
        char tipoPagamento[20];
        char status[20];

        switch(venda.categoria_Pagamento) {
            case 'd': strcpy(tipoPagamento, "Dinheiro"); break;
            case 'c': strcpy(tipoPagamento, "Cartão"); break;
            case 'b': strcpy(tipoPagamento, "Misto"); break;
            default: strcpy(tipoPagamento, "Desconhecido"); break;
        }

        switch(venda.situacao_Do_pagamento) {
            case 'f': strcpy(status, "Finalizado"); break;
            case 'a': strcpy(status, "Aberto"); break;
            case 'x': strcpy(status, "Cancelado"); break;
            default: strcpy(status, "Indefinido"); break;
        }

        printf("Pagamento: %s | Status: %s\n", tipoPagamento, status);
        printf("Itens:\n");

        for (int i = 0; i < venda.qtd_itens; i++) {
            printf("  %-15s x%d R$ %.2f (R$ %.2f unit.)\n",
                venda.itens[i].descricao,
                venda.itens[i].quantidade,
                venda.itens[i].subtotal,
                venda.itens[i].preco_unitario);
        }

        totalGeral += venda.total_final;
        totalVendas++;
    }

    printf("\n══════════════════════════════════════════════════════════\n");
    printf("Total de vendas: %d | Faturamento geral: R$ %.2f\n",
        totalVendas, totalGeral);
    fclose(arquivo);

    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}

void relatorioClientes() {
    limpar_Terminal();
    FILE *arquivo = fopen(DADOSCLIENTES, "r");
    if (arquivo == NULL) {
        printf("\nERRO: Nenhum cliente cadastrado!\n");
        printf("Cadastre clientes antes de gerar relatórios.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\n═══════════════════════════════════════════════════════════════════════════════\n");
    printf("                            RELATÓRIO DE CLIENTES                               \n");
    printf("═══════════════════════════════════════════════════════════════════════════════\n");
    printf("| %-3s | %-20s | %-15s | %-11s | %-20s |\n",
            "ID", "Nome", "Nome Social", "CPF", "Celular");
    printf("────────────────────────────────────────────────────────────────────────────────\n");

    char linha[256];
    int totalClientes = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        Cliente cli;
        char *token = strtok(linha, ","); // strtok(linha, ","): pega a primeira parte da linha até a primeira vírgula e se encontrar, converte de texto para inteiro usando atoi() e coloca em cli.cod.
        if (token) cli.cod = atoi(token); 

        token = strtok(NULL, ","); 
        if (token) strncpy(cli.nome, token, max_caracter); // basicamente estamos copiando partes do arquivo para uma variavel struct para printar

        token = strtok(NULL, ",");
        if (token) strncpy(cli.nome_social, token, max_caracter);

        token = strtok(NULL, ",");
        if (token) strncpy(cli.cpf, token, max_cpf);

        // Ignora rua e bairro (não mostrados neste relatório)
        strtok(NULL, ",");
        strtok(NULL, ",");

        token = strtok(NULL, "\n");
        if (token) strncpy(cli.celular, token, max_inf);

        printf("| %-3d | %-20.20s | %-15.15s | %-11.11s | %-20.20s |\n",
            cli.cod, cli.nome, cli.nome_social, cli.cpf, cli.celular);
        totalClientes++;
    }

    printf("═══════════════════════════════════════════════════════════════════════════════\n");
    printf("Total de clientes: %d\n", totalClientes);
    fclose(arquivo);

    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}