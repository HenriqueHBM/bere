
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h> // Para função toupper()

/* Defines ---------------------------*/

#define max_caracter 100               // max de caracter para a descrição
#define ARQUIVO "cadastrar&vendas.txt" // renomeando o arquivo que vai ser valvo
#define COMPRAS "vendas.txt"           // criando um novo arquivo para salvar as compras de cada cliente;
#define TEMP "temp.txt"
#define DADOSCLIENTES "DadosClientes.txt"

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
    char nome[100];
    char nome_social[100];
    char cpf[12];
    char rua_num[50];
    char bairro[50];
    char celular[50];
} Cliente;

/* Fim Structs ----------------------------*/

/* Variaveis Globais ----------------------*/

Carrinho *clientes = NULL;
int qtd_clientes = 0;
int idx;

/* Fim Variaveis Globais ------------------*/

/* Inicializando Funcoes ------------------*/

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void salvar_Produtos_Arquivo(Cadastrar_Produtos *pt, FILE *arquivo);
void listar_Produtos();
void mostrar_Produtos_Compra();
void venda_de_Produtos();
void atualizarEstoque(int idProduto, int quantidadeComprada);
void nota_E_Desconto();
void nota_fiscal();
void salvar_Compra();
void pagamento();
void cadastrarCliente(Cliente *);
int contadorCaracterFile(char *, char);
void limparBuffer();
int verificaArquivo(FILE *arquivo);
void removeEnterTexto(char *);
int exibirMenu();
void exibirVendas();
void exibirCadastros();

void abrirCaixa();
void fecharCaixa();
void menuRelatorios();
void listarClientes();

/* Fim Inicializando Funcoes --------------*/

void pular_Linha()
{
    printf("\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int escolha;

    do // Pelo menos uma vez vai ser mostrado a menu principal
    {

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
            //
            abrirCaixa();
            break;
        case 4:
            fecharCaixa();
            break;
        case 5:
            menuRelatorios();
            break;
        case 6:
            //
            printf("\n\n\t\tSaindo do programa\n\n");
            break;
        default:
            printf(" \n          Opção invalida\n\n");
            break;
        }

    } while (escolha != 7); // Se a escolha for diferente de 7 vai repetir se não vai parar o programa
    return 0;
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

int exibirMenu()
{
    char opcao[10]; // alterado para string, pois caso o usuario inserisse um caracter, ele bugava o codigo
    printf("\n");
    printf("%50s\n", "=======================================");
    printf("%50s\n", "|            MENU PRINCIPAL           |");
    printf("%50s\n", "=======================================");
    printf("%50s\n", "| 1) Cadastros                        |");
    printf("%50s\n", "| 2) Vendas                           |");
    printf("%50s\n", "| 3) Abertura de caixa                |");
    printf("%50s\n", "| 4) Fechamento de caixa              |");
    printf("%50s\n", "| 5) Relatórios                       |");
    printf("%51s\n", "| 6) Sair                             |");
    printf("%50s\n", "======================================");
    printf("%32s", "Escolha uma opção: ");
    fgets(opcao, sizeof(opcao), stdin);
    return atoi(opcao); // Para transformar um valor de texto em um inteiro em C, a função atoi() da biblioteca stdlib.h pode ser usada (Google)
}

void exibirCadastros()
{
    char opcao[10];
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;
    FILE *arquivo;
    while (1)
    {
        printf("1. Menu Cadastros\n");
        printf("\t\t1 - Cadastro de Clientes\n");
        printf("\t\t2 - Cadastro de Produtos\n");
        printf("\t\t3 - Retornar ao Menu Principal\n");
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
        case 3:
            printf("Voltando ao menu principal...\n");
            return;
        default:
            printf("Opcao Invalida!!");
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

int verificaArquivo(FILE *arquivo)
{
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo"); // mostra erro mais detalhado
        return 1;                          // indica erro
    }
    return 0; // tudo certo
}

void removeEnterTexto(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0'; // remove o "enter/ \n" do texto
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

void exibirVendas()
{
    int opcao;
    do
    {

        printf("\tÁREA  DE VENDAS\n");
        printf("========================\n");
        printf("21 - NOVA VENDA\n22 - RETIRANDA DE CAIXA(SANGUIA)\n23 - PAGAMENTO\n24 - RETORNAR OA MENU PRÍNCIPAL\n");
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
            // free(carrinho);
            break;

        case 24:
            return;
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
    idx = qtd_clientes; // aqui da para alterar para o id do cliente
    Cadastrar_Produtos temp;

    clientes = realloc(clientes, (qtd_clientes + 1) * sizeof(Carrinho));
    if (clientes == NULL)
    {
        printf("Erro ao alocar memória para carrinhos!\n");
    }

    clientes[qtd_clientes].itens = NULL;
    clientes[qtd_clientes].qtd_itens = 0;
    clientes[qtd_clientes].ID_Cliente = qtd_clientes + 1; // ou outro ID único
    clientes[qtd_clientes].total_final = 0.0;

    do
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        mostrar_Produtos_Compra();

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

        // Abrindo o arquivo que está nosso aetoque para poder verificar os dados dos produtos;
        FILE *arquivo = fopen(ARQUIVO, "r");
        if (arquivo == NULL)
        {
            printf("\nErroo abrir o arquivo de produtos!!!");
            fclose(arquivo);
            return;
        }

        printf("\nCarrinho de compras: ");

        printf("\nInforme o codigo do produto a ser comprado: ");
        scanf("%d", &codigo);

        int produto_encontrado = 0;

        while (fgets(linha, sizeof(linha), arquivo) != NULL)
        {
            int campo_lido = sscanf(linha, "%d;%[^;];%d;%f;%f;%d;%d",
                                    &temp.id,
                                    temp.descricao_Produto,
                                    &temp.categoria_Produto,
                                    &temp.Preco_De_Compra,
                                    &temp.Preco_De_Venda,
                                    &temp.Quantidade_em_Estoque,
                                    &temp.estoque_Minimo);

            if (campo_lido == 7 && codigo == temp.id)
            {
                produto_encontrado = 1;

                printf("\nProduto encontrado: %s", temp.descricao_Produto);
                printf("\nQuantidade em estoque: %d", temp.Quantidade_em_Estoque);
                printf("\nInforme a quantidade desejada: ");
                scanf("%d", &quantidade);

                if (quantidade <= temp.Quantidade_em_Estoque)
                {
                    float valor = temp.Quantidade_em_Estoque - quantidade;
                    if (valor <= temp.estoque_Minimo)
                        printf("\nAviso: estoque no limite mínimo!!!!");

                    atualizarEstoque(codigo, quantidade);

                    clientes[idx].itens = realloc(clientes[idx].itens, (clientes[idx].qtd_itens + 1) * (sizeof(ItemCarrinho)));
                    if (clientes[idx].itens == NULL)
                    {
                        printf("Erro ao alocar memória!\n");
                        fclose(arquivo);
                        return;
                    }

                    ItemCarrinho *item = &clientes[idx].itens[clientes[idx].qtd_itens];

                    item->id_produto = temp.id;
                    strcpy(item->descricao, temp.descricao_Produto);
                    item->preco_unitario = temp.Preco_De_Venda;
                    item->quantidade = quantidade;
                    item->subtotal = quantidade * temp.Preco_De_Venda;

                    clientes[idx].total_final += item->subtotal;
                    clientes[idx].qtd_itens++;

                    printf("\nItem adicionado ao carrinho!");
                }
                else
                    printf("\nQuantidade no estoque insuficiente!");

                break; // produto encontrado, não precisa continuar lendo
            }
        }

        if (!produto_encontrado)
            printf("\nCódigo do produto inválido ou não encontrado.");

        fclose(arquivo);

        do
        {
            printf("\nNovo item no carrinho de compra (s/n): ");
            scanf(" %c", &escolha_continuar_Comprando);
            if (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n')
                printf("\nOpção inválida!");

        } while (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n');

    } while (escolha_continuar_Comprando == 's');
}

void mostrar_Produtos_Compra()
{
    FILE *arquivo = fopen(ARQUIVO, "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Cadastrar_Produtos produto;

    printf("\nProdutos disponíveis:\n");
    printf("ID\tDescrição\t\tEstoque\t\tPreço de Venda\n");
    printf("---------------------------------------------------------------\n");

    while (fscanf(arquivo, "%d;%[^;];%d;%f;%f;%d;%d\n",
                &produto.id,
                produto.descricao_Produto,
                &produto.categoria_Produto,
                &produto.Preco_De_Compra,
                &produto.Preco_De_Venda,
                &produto.Quantidade_em_Estoque,
                &produto.estoque_Minimo) != EOF)
    {
        printf("%d\t%-20s\t%d\t\tR$ %.2f\n",
            produto.id,
            produto.descricao_Produto,
            produto.Quantidade_em_Estoque,
            produto.Preco_De_Venda);
    }

    fclose(arquivo);
}

void atualizarEstoque(int idProduto, int quantidadeComprada)
{
    FILE *arquivoOriginal = fopen(ARQUIVO, "r");
    FILE *arquivoTemp = fopen(TEMP, "w");

    if (!arquivoOriginal || !arquivoTemp)
    {
        printf("Erro ao abrir arquivos para atualizar estoque.\n");
        if (arquivoOriginal)
            fclose(arquivoOriginal);
        if (arquivoTemp)
            fclose(arquivoTemp);
        return;
    }

    Cadastrar_Produtos temp;

    while (fscanf(arquivoOriginal, "%d;%[^;];%d;%f;%f;%d;%d\n",
                &temp.id,
                temp.descricao_Produto,
                &temp.categoria_Produto,
                &temp.Preco_De_Compra,
                &temp.Preco_De_Venda,
                &temp.Quantidade_em_Estoque,
                &temp.estoque_Minimo) != EOF)
    {
        if (temp.id == idProduto)
        {
            temp.Quantidade_em_Estoque -= quantidadeComprada;
            if (temp.Quantidade_em_Estoque < 0)
                temp.Quantidade_em_Estoque = 0;
        }

        fprintf(arquivoTemp, "%d;%s;%d;%.2f;%.2f;%d;%d\n",
                temp.id,
                temp.descricao_Produto,
                temp.categoria_Produto,
                temp.Preco_De_Compra,
                temp.Preco_De_Venda,
                temp.Quantidade_em_Estoque,
                temp.estoque_Minimo);
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // Substitui o arquivo original pelo atualizado
    remove(ARQUIVO);
    rename(TEMP, ARQUIVO);
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

    printf("\nTotal compra: \n\t\tR$ %.2f", total_compra);

    printf("\nHá desconto (informe 0 (para não) ou porcentos concedido: ");
    if((scanf("%f", &desconto_no_totalC)) != 0)
    {
        desconto_no_totalC = desconto_no_totalC/100;
        total_compra = total_compra * desconto_no_totalC;
    }

    printf("\nTotal final: R$ %.2f", total_compra);

}


void pagamento()
{
    int opcao, aprovacao_maquina = 0, x=0;
    float valor_QFalta, valor_Recebido;
    char y;

    if(clientes[idx].total_final == 0)
    {
        printf("\nNem um produto foi comprado...");
        return;
    }
    else
    {
        printf("\n1 - Cartao\t2 - dinheiro\t3 - retornar ao menu principal");
        scanf("%d", &opcao);

        do
        {
           switch (opcao)
            {
            case 1:
                nota_fiscal();
                clientes[idx].categoria_Pagamento = 'c';

                do
                {
                    printf("\nInforme: \n(1) – Pagamento na maquinha Ok\n(0) – Pagamento no cartão não Ok");
                    scanf("%d", &aprovacao_maquina);
                    if(x > 0)
                        printf("\nOpcao invalida");
                    x++;
                }while(aprovacao_maquina !=  1 && aprovacao_maquina != 0);

                if(aprovacao_maquina == 1)
                {
                    clientes[idx].situacao_Do_pagamento = 'f';
                    clientes[idx].valor_Recebido = clientes[idx].total_final;
                    salvar_Compra();
                    printf("\nPagamento efetuado com sucesso <3");
                }

                break;
                case 2:
                    nota_fiscal();
                    clientes[idx].categoria_Pagamento = 'd';

                    printf("\nValor recebido: ");
                    scanf("%f", &valor_Recebido);
                    clientes[idx].valor_Recebido = valor_Recebido;

                    //Aqui vai o troco, fazer o calculo de quanto temos no caixa e devolter determinada quantia
                    if(clientes[idx].valor_Recebido < clientes[idx].total_final)
                    {
                        do
                        {
                            printf("\nValor insuficiente! deseja pagar o resto no cartao (s/n)");
                            scanf("%c", &y);

                        } while (y != 's' && y != 'n');

                        if(y == 's')
                        {
                            printf("\nPagamento efetuado com sucesso <3");
                            clientes[idx].categoria_Pagamento = 'dc';
                            clientes[idx].situacao_Do_pagamento = 'f';
                        }
                        else
                        {
                            do
                            {
                                valor_QFalta = (clientes[idx].total_final) - (clientes[idx].valor_Recebido);
                                printf("\nA quantia que falta é: R$ %.2f", valor_QFalta);
                                printf("\nInforme o valor pago: ");
                                scanf("%f", &valor_Recebido);
                                clientes[idx].valor_Recebido = valor_Recebido;
                            } while (clientes[idx].valor_Recebido < clientes[idx].total_final);

                            clientes[idx].situacao_Do_pagamento = 'f';
                            salvar_Compra();
                            printf("\nPagamento efetuado com sucesso <3");
                        }
                    }
                    else
                    {
                        printf("\nPagamento efetuado com sucesso <3");
                        clientes[idx].situacao_Do_pagamento = 'f';
                        salvar_Compra();
                    }

                break;

                default:
                    printf("\nOpcao invalida");
                break;
            }
            if(aprovacao_maquina == 0)
                opcao = 3;
        }while(opcao != 3 && aprovacao_maquina != 0);


    }

}

void nota_fiscal()
{
    float total_compra;

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

    printf("\nTotal compra: \n\t\tR$ %.2f", total_compra);
}


void salvar_Compra()
{
    FILE *file = fopen("venda.txt", "a+");
    if(file == NULL)
        printf("\nErro ao abrir o arquivo de compras");

    fprintf(file, "%d;%d;%c;%c;%f\n",
            clientes[idx].ID_Cliente,
            clientes[idx].num_Venda,
            clientes[idx].categoria_Pagamento,
            clientes[idx].situacao_Do_pagamento,
            clientes[idx].valor_Recebido);

    for(int i =0; i < clientes[idx].qtd_itens; i++)
    {
        fprintf(file, "%d;%s;%f;%f\n",
            clientes[idx].itens[i].id_produto,
            clientes[idx].itens[i].descricao,
            clientes[idx].itens[i].preco_unitario,
            clientes[idx].itens[i].subtotal);
    }

    for (int i = 0; i < qtd_clientes; i++)
    {
        free(clientes[i].itens);
    }
    free(clientes);
}

/*Teste ---------*/

typedef struct {
    float valor;
    char tipo; // 'a' - abertura, 's' - sangria, 'd' - dinheiro, 'c' - cartão
    int num_venda; // 0 se for abertura/sangria
} MovimentoCaixa;

typedef struct {
    int num_venda;
    float valor_total;
    char forma_pagamento; // 'd'-dinheiro, 'c'-cartão, 'dc'-ambos
    char situacao; // 'a'-aberto, 'p'-pago
} Venda;

MovimentoCaixa *movimentos = NULL;
int qtd_movimentos = 0;
Venda *vendas = NULL;
int qtd_vendas = 0;
float saldo_caixa = 0.0;


void abrirCaixa() {
    float valor;
    // Solicita o valor inicial do caixa
    printf("Informe o valor de abertura do caixa: ");
    scanf("%f", &valor);
    getchar();// Limpa o buffer do teclado
    
      // Verifica se o valor é válido (não negativo)
    if (valor >= 0) {
         // Aloca espaço para um novo movimento no caixa
        movimentos = realloc(movimentos, (qtd_movimentos + 1) * sizeof(MovimentoCaixa));
        movimentos[qtd_movimentos].valor = valor;
        movimentos[qtd_movimentos].tipo = 'a'; // 'a' para abertura
        movimentos[qtd_movimentos].num_venda = 0;// 0 pois não é associado a venda
        qtd_movimentos++;
        
        saldo_caixa = valor;  // Atualiza o saldo do caixa
        printf("Caixa aberto com R$ %.2f\n", valor);
    } else {
        printf("Valor inválido para abertura de caixa!\n");
    }
}


void fecharCaixa() {
      // Variáveis para armazenar os totais
    float vl_abertura = 0.0;
    float vl_pago_d = 0.0;
    float vl_pago_c = 0.0;
    float vl_pago_dc = 0.0;
    float faturamento = 0.0;
    int qtd_vendas_realizadas = 0;
    
    // Calcula os totais percorrendo todos os movimentos
    for (int i = 0; i < qtd_movimentos; i++) {
        if (movimentos[i].tipo == 'a') {
            vl_abertura = movimentos[i].valor; // Pega o valor de abertura
        } else if (movimentos[i].tipo == 'd') {
            vl_pago_d += movimentos[i].valor; // Soma pagamentos em dinheiro
        } else if (movimentos[i].tipo == 'c') {
            vl_pago_c += movimentos[i].valor; // Soma pagamentos em cartão
        }
    }

    
     // Exibe o resumo do caixa
    printf("• Quantidade de vendas realizadas: %d\n", qtd_vendas_realizadas);
    printf("• Total do faturamento no dia: R$ %.2f\n", faturamento);
    printf("• Valor da abertura de caixa: R$ %.2f\n", vl_abertura);
    printf("• Valor pago em dinheiro: R$ %.2f\n", vl_pago_d);
    printf("• Valor pago em cartão: R$ %.2f\n", vl_pago_c);
    printf("• Valor pago em dinheiro/cartão: R$ %.2f\n", vl_pago_dc);
    
     // Verifica se o caixa fecha sem divergências
    float diferenca = faturamento - vl_abertura - vl_pago_d - vl_pago_c - vl_pago_dc;
    
    if (diferenca == 0) {// Se houver divergência, solicita ajuste
        printf("Caixa fechado corretamente!\n");
    } else {
        printf("ATENÇÃO: O caixa não pode ser fechado porque há divergência de R$ %.2f\n", diferenca);
        printf("Informe o valor de ajuste (positivo para sobra, negativo para falta): ");
        float ajuste;
        scanf("%f", &ajuste);
        getchar();
        
        if (ajuste == diferenca) {
            printf("Caixa ajustado e fechado!\n");
        } else {
            printf("Ajuste incorreto! Caixa não fechado!\n");
            return;
        }
    }
    
    // Zerar caixa
    saldo_caixa = 0.0;
}



void menuRelatorios() {
    char opcao[10];
    while (1) {
        printf("\n5. Relatórios\n");
        printf("\t\t1 - Listagem dos Clientes\n");
        printf("\t\t2 - Listagem dos Produtos\n");
        printf("\t\t3 - Listagem das Vendas\n");
        printf("\t\t4 - Retornar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        
        fgets(opcao, sizeof(opcao), stdin);
        
        switch(atoi(opcao)) {
            case 1:
                listarClientes();
                break;
            case 2:
                listar_Produtos();
                break;
            case 3:
                //listarVendas();
                break;
            case 4:
                return;
                break;
            default:
                printf("Opção inválida!\n");
        }
    }
}


void listar_Produtos()
{
    FILE *arquivo = fopen(ARQUIVO, "r");
    if(arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!!!\n");
        return ;
    }

    printf("\n-- Produtos Cadastrados --\n");

    Cadastrar_Produtos temp;
    // Lê cada produto do arquivo e exibe suas informações
    while (fscanf(arquivo, "%d;%[^;];%d;%f;%f;%d;%d\n", 
                &temp.id,
                temp.descricao_Produto,
                &temp.categoria_Produto,
                &temp.Preco_De_Compra,
                &temp.Preco_De_Venda,
                &temp.Quantidade_em_Estoque,
                &temp.estoque_Minimo) != EOF)
    {
 // Exibe os detalhes de cada produto
        printf("ID: %d\n", temp.id);
        printf("Descrição: %s\n", temp.descricao_Produto);
        printf("Categoria: %d\n", temp.categoria_Produto);
        printf("Compra: R$ %.2f | Venda: R$ %.2f\n", temp.Preco_De_Compra, temp.Preco_De_Venda);
        printf("Estoque: %d | Mínimo: %d\n", temp.Quantidade_em_Estoque, temp.estoque_Minimo);
        printf("-------------------------\n");
    }

    fclose(arquivo);
}

void listarClientes(){
        FILE *arquivo = fopen(DADOSCLIENTES, "r");
    if(arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!!!\n");
        return ;
    }

    printf("\n-- Clientes Cadastrados --\n");

    Cliente temp;
    while (fscanf(arquivo, "%d,%s,%s,%s,%s,%s,%s\n",
                &temp.cod,
                &temp.nome,
                &temp.nome_social,
                &temp.cpf,
                &temp.rua_num,
                &temp.bairro,
                &temp.celular) != EOF)
    {

        printf("ID: %d\n", temp.cod);
        printf("Nome: %s\n", temp.nome);
        printf("Nome Social: %d\n", temp.nome_social);
        printf("CPF: %d\n", temp.cpf);
        printf("Rua: %d\n", temp.rua_num);
        printf("Bairro: %d\n", temp.bairro);
        printf("Telefone: %d\n", temp.celular);
        printf("-------------------------\n");
    }

    fclose(arquivo);
}