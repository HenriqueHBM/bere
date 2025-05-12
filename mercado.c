#include <stdio.h>
#include <stdlib.h>
#include <locale.h> //Gramática Portugues
#include <time.h>   // tempo do computador
#include <string.h> // bibliotecas de string

// Estrutura struct para formato dos dados dos produtos
typedef struct
{
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto; // Mudando o nome struct para "Produto" facilitar chama-lá

// Com a estrutura struct cada iten da variável, são 4 a 5, vai ter seu dados
Produto limpeza[] =
    {
        // A condição da função para chamalá será limpeza[]
        {11, "Detergente", 1.99, 50},
        {12, "Sabão em pó", 8.99, 30},
        {13, "Esponja", 1.50, 0},
        {14, "Amaciante(1Lt)", 15.00, 20},
        {15, "Bucha de Pia(c/3)", 4.99, 100},
        {16, "Desinfetante", 7.99, 15},
        {17, "Sabão em barra", 1.00, 60},
};

Produto alimentos[] =
    {
        {21, "Café", 19.99, 10},
        {22, "Leite(cx)", 5.90, 15},
        {23, "Arroz(1Kl)", 4.50, 10},
        {24, "Feijão preto", 8.00, 5},
        {25, "Açucar(1Kl)", 5.00, 20},
        {26, "Sal(1Kl)", 2.00, 6},
        {27, "Farinha de Trigo (1Kl)", 5.00, 15},
};

Produto padaria[] =
    {
        // A condição da função para chamalá será padaria[]
        {31, "Pão de forma", 9.50, 0},
        {32, "Pão integral", 12.50, 0},
        {33, "Pão frânces(uni)", 1.90, 0},
        {34, "Sonho", 8.50, 0},
        {35, "Biscoito", 12.50, 0},
        {36, "Pão doce", 2.50, 0},
        {37, "Salgado", 17.50, 0},
};

typedef struct
{
    int  cod;
    char nome[100];
    char nome_social[100];
    char cpf[12];
    char rua_num[50];
    char bairro[50];
    char celular[50];
} Cliente;

// Variáveis que recebem o total, utilizando *total para alterar o valor, de cada categoria
float totalLimpeza = 0, totalAlimentos = 0, totalPadaria = 0, totalVendas = 0, totalAbertura = 0, abertura, fechamentoLipeza, fechamentoAlimento, fechamentoPadaria;
int caixa_aberto = 0, qtde_clientes = 0; // 0 para false e 1 para true;

// Pode ser usado para pular linha
int exibirMenu();
void exibirProdutos(Produto produtos[], int tamanho, float *total);
void realizarPagamento();
void limparComEspera();
void calculoDesconto(float valorFinal, float recebido, float totalCompra);
void aberturaCaixa(Produto produtos[], int tamanho);
void fechamentoCaixa();
void pularLinha();
void maior_que_zero(int valor);
void exibirCadastros();
// void cadastrarCliente(Cliente clientes[], int *qtde_clientes);
void cadastrarCliente(FILE *, Cliente *);
int contadorCaracterFile( char nome_arquivo, char caracter_desejado);

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int escolha;
    int teste = 0;

    const char *arquivo = "Dados.txt";
    char c = '\n';

    teste =  contadorCaracterFile(&arquivo, c);
    return teste;
    exibirCadastros();

    do // Pelo menos uma vez vai ser mostrado a menu principal
    {

        escolha = exibirMenu();
        if (caixa_aberto != 1 && escolha != 5)
        {
            // if( ){ // realizar a abertura do caixa antes de dar continuidade no programa
            printf("Realiza a abertura do caixa primeiramente \n");
            //}
        }
        else if (escolha == 7)
        { // ver se tem que ajustar
            printf("\n\n\t\tSaindo do programa\n\n");
        }
        else if (caixa_aberto == 1 && escolha == 5)
        { // nao deixar reabrir o caixa
            printf("\n\n\t\\Caixa já foi aberto\n\n");
        }
        else
        {
            switch (escolha)
            {
            case 1:
                exibirCadastros();
                int tamanho_limpeza = sizeof(limpeza) / sizeof(limpeza[0]); //  Divide o tamanho total pelo tamanho de um elemento para obter o número de elementos no array. (Google)
                exibirProdutos(limpeza, tamanho_limpeza, &totalLimpeza);    // chamamos a função e nos () colocamos as condições
                break;
            case 2:
                int tamanho_alimento = sizeof(alimentos) / sizeof(alimentos[0]);
                exibirProdutos(alimentos, tamanho_alimento, &totalAlimentos);
                break;
            case 3:
                int tamanho_padaria = sizeof(padaria) / sizeof(padaria[0]);
                exibirProdutos(padaria, tamanho_padaria, &totalPadaria);
                break;
            case 4:
                realizarPagamento(); // chamamos a fução pagar, mas ela não precisa de condição
                break;
            case 5:
                aberturaCaixa(padaria, 7);
                break;
            case 6:
                fechamentoCaixa();
                break;
            case 7:
                printf("\n\n\t\tSaindo do programa\n\n");
                break;
            default:
                printf(" \n          Opção invalida\n\n");
                break;
            }
        }

    } while (escolha != 7); // Se a escolha for diferente de 7 vai repetir se não vai parar o programa
    return 0;
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
    printf("%50s\n", "| 3) Padaria                          |");
    printf("%50s\n", "| 4) Abertura de caixa                |");
    printf("%50s\n", "| 5) Fechamento de caixa              |");
    printf("%51s\n", "| 6) Relatórios                       |");
    printf("%50s\n", "| 7) Sair                             |");
    printf("%50s\n", "======================================");
    printf("%38s", "Escolha uma opção: ");
    fgets(opcao, sizeof(opcao), stdin);
    return atoi(opcao); // Para transformar um valor de texto em um inteiro em C, a função atoi() da biblioteca stdlib.h pode ser usada (Google)
}

void aberturaCaixa(Produto produtos[], int tamanho)
{
    caixa_aberto = 1;
    printf("\t\t\t\t\tABERTURA DE CAIXA\n");
    printf("\t\t\t\t--------------------------------------\n\n\n");
    printf("\t\t\t\t\tInforme o estoque da Padaria\n\n");
    for (int i = 0; i < tamanho; i++)
    {
        int estoquePadaria = 0;
        do
        {
            printf("\t\t\t\t%2d - %25s - R$%4.2f - %3d \n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque);
            printf("\t\t\t\tQuantidade de %s: ", produtos[i].nome);
            scanf("%d", &estoquePadaria);
            maior_que_zero(estoquePadaria); // funcao para printar mensagem
            pularLinha();
        } while (estoquePadaria < 0);

        produtos[i].estoque = estoquePadaria;
    }
    do
    {
        printf("\n\n\t\t\t\tQuantia de dinheiro no caixa: ");
        scanf("%f", &totalAbertura);
        maior_que_zero(totalAbertura); // funcao para printar mensagem
        pularLinha();
    } while (totalAbertura < 0);

    printf("\t\t\t\tSaldo do caixa: R$%.2f\n", totalAbertura);
    abertura = totalAbertura;
    limparComEspera();
}

// Agora dissemos o que essas funções vão fazer
void exibirProdutos(Produto produtos[], int tamanho, float *total)
{ // Função vai precisar de condições qual variavel na estrutura struct, tamanho e total
    int codigo, quantidade;
    printf("\t\t\t\t--------------------------------------\n\n\n"); // trocar por uma funcao
    printf("\t\t\t\tSelecione o codigo do produto (precione 0 para voltar0):\n\n");
    printf("\t\t\t\tCódigo\t\t\tNome\tPreço\tEstoque\n");
    for (int i = 0; i < tamanho; i++)
    {
        printf("\t\t\t\t%2d - %25s - R$%.2f - %3d \n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque);
    }
    while (1)
    {
        printf("\n\n\t\t\t\tCÓDIGO:");
        scanf("%d", &codigo);
        // se for 0 vai parar essa parte e vai voltar a return 0
        if (codigo == 0)
            break;
        for (int i = 0; i < tamanho; i++)
        { // vai fazer um loop até encontrar o codigo na variavel struct, se o codigo existir então vai entrar no if
            if (codigo == produtos[i].codigo)
            {
                if (produtos[i].estoque > 0)
                {
                    printf("\n\t\t\t\tQUANTIDADE:"); // Aquui entra o estoque
                    scanf("%d", &quantidade);
                    if (quantidade > produtos[i].estoque)
                    {
                        printf("\n\n\t\t\t\t\tQUANTIDADE MAIOR QUE TEM NO ESTOQUE.");
                        break;
                    }
                    *total += produtos[i].preco * quantidade; // Aqui que modificamos o ponteiro total
                    printf("\n\t\t\t\t%d DE %s FOI ADICIONADO AO CARRINHO. \n", quantidade, produtos[i].nome);
                    produtos[i].estoque = produtos[i].estoque - quantidade;
                }
                else
                    printf("\n\n\t\t\t\t\tESTOQUE INSUFICIENTE\n");
            }
            else
                printf("Código invalido\n");
        }
    }
    system("cls"); // limpa a tela
}

// Função para o pagamento
void realizarPagamento()
{ // Soma todas as caegorias
    float totalCompra = totalAlimentos + totalLimpeza + totalPadaria;
    if (totalCompra == 0)
    {
        printf("Nao foi realizado nem uma compra");
        return;
    }

    // nossa nota fiscal que vai ter o total de cade categoria
    printf("         Resumo da Compra:\n");
    printf("         - Materiais de Limpeza - R$%.2f \n", totalLimpeza);
    printf("         - Vendas de Alimentos  - R$%.2f \n", totalAlimentos);
    printf("         -      Padaria         - R$%.2f \n\n", totalPadaria);
    printf("         -   Total a pagar      - R$%.2f \n", totalCompra);

    int opcao;
    int cartao_Ok;
    float recebido, valorFinal = totalCompra;
    printf("\t\tEscolha a forma de pagamento:\n\t\t1) Dinheiro (Desconto)\n\t\t2) Cartao\n\t\tOpsão:");
    scanf("%d", &opcao);
    pularLinha();
    if (opcao == 1)
    {
        printf("\t\tQuantia recebida: ");
        scanf("%f", &recebido);
        pularLinha();
    }

    // opção 1 será a que terá os descontos, logo, os if e else
    switch (opcao)
    {
    case 1:
        if (totalCompra <= 50)
        {
            valorFinal = totalCompra * 0.95;
            calculoDesconto(valorFinal, recebido, totalCompra); // faz a verificação do troco
        }
        else if (totalCompra > 50 && totalCompra < 200)
        {
            valorFinal = totalCompra * 0.90;
            calculoDesconto(valorFinal, recebido, totalCompra);
        }
        else if (totalCompra >= 200)
        {
            int desconto;
            printf("\t\tEsse valor passa os R$200, informe a porcetagem de desconto\n");
            scanf("%d", &desconto);
            valorFinal = totalCompra * (desconto / 100);
            calculoDesconto(valorFinal, recebido, totalCompra);
        }
        break;
    case 2:
        printf("\t\tO pagamento no cartão foi: \\(1\\) pagamneto OK\n\t\t \\(2\\) pagamento não OK\n");
        scanf("%d", &cartao_Ok);
        if (cartao_Ok == 1)
        {
            printf("\t\tPagamento no catão efetuado com sucesso\n");
            limparComEspera();
        }
        else if (cartao_Ok == 2)
        {
            printf("\t\tPagamento no catão não foi efetuado com sucesso\n");
            printf("\t\tEscolha outra forma de pagamento ou repita o processo\n");
            return realizarPagamento();
        }
        else
            printf("\t\tEscolha invalida\n");
        break;
    default:
        printf("\t\tEscolha invalida\n");
        break;
    }
    totalVendas += totalCompra;
    fechamentoLipeza += totalLimpeza;
    fechamentoAlimento += totalAlimentos;
    fechamentoPadaria += totalPadaria;
    totalCompra = totalAlimentos = totalLimpeza = totalPadaria = 0;
}

void calculoDesconto(float valorFinal, float recebido, float totalCompra)
{
    float resto_valor = 0;
    if (recebido == valorFinal)
    {
        printf("\nPagamento com dinheiro no valor R$%.2f ficou por %.2f\n", totalCompra, valorFinal);
        totalCompra = valorFinal;
        limparComEspera();
    }
    else if (recebido > valorFinal)
    {
        printf("Pagamento em dinheiro: R$%.2f com desconto. \n\t\tTotal final: R$%.2f\n", totalCompra, valorFinal);
        printf("\t\tTroco: R$%.2f\n", recebido - valorFinal);
        totalAbertura -= (recebido - valorFinal);
        limparComEspera();
    }
    else
    {
        printf("Valor insuficiente!\n");
        printf("Valor Inserido %.2f\n", recebido);
        printf("Valor Restante %.2f\n", totalCompra - (recebido + resto_valor));
        printf("Insira o restante do valor\n");
        scanf("%f", &resto_valor);

        return calculoDesconto(valorFinal, recebido + resto_valor, totalCompra);
    }
}

void limparComEspera()
{
    printf("\nPressione qualquer tecla para continuar...");
    getchar();     // consome o \n deixado pelo scanf
    getchar();     // espera a tecla do usuário
    system("cls"); // limpa a tela
}

void fechamentoCaixa()
{
    printf("\t\tO valor de Abertura de caixa foi: %.2f\n", abertura);
    printf("\t\tFaturamento total: %.2f\n", totalVendas - totalAbertura);
    printf("\t\tFaturamento das categorias\n");
    printf("\t\tMateriais de Limpeza: %.2f\n", fechamentoLipeza);
    printf("\t\tVenda de Alimento: %.2f\n", fechamentoAlimento);
    printf("\t\tPadaria: %.2f\n", fechamentoPadaria);
}

void pularLinha()
{
    printf("\n");
}

void maior_que_zero(int valor)
{ // caso insira um valor menor que 0
    if (valor < 0)
    {
        printf("\nQuantidade invalida, insira um valor maior ou igual a zero");
    }
}

void exibirCadastros()
{
    char opcao[2];
    // Cliente clientes[100]; //trocar por max
    FILE *arquivo;
    while (1)
    {
        printf("1. Menu Cadastros\n");
        printf("\t\t1 - Cadastro de Clientes\n");
        printf("\t\t2 - Cadastro de Produtos\n");
        printf("\t\t3 - Retornar ao Menu Principal\n");
        printf("Escolha uma opção: ");

        fgets(opcao, sizeof(opcao), stdin);

        Cliente cliente;
        arquivo = fopen("DadosClientes.txt", "a+"); //caso nao exista cria, se nao so atualiza;

        switch (atoi(opcao))
        {
        case 1:
            // cadastrarCliente(&arquivo, &cliente);
            break;
        case 2:
            printf("Cadastro de produtos ainda não implementado.\n");
            break;
        case 3:
            printf("Voltando ao menu principal...\n");
            return;
        default:
        printf("Voltando ao menu principal");
        }

        printf("\n");
    }
}

// void cadastrarCliente(Cliente clientes[], int *qtde_clientes){
//     Cliente novoCliente;

//     novoCliente.cod = *qtde_clientes + 1;

//     printf("%d", novoCliente.cod);

//     clientes[*qtde_clientes] = novoCliente;
//     (*qtde_clientes)++; // incrementando o ponteiro, funciona igual o +=, e o ponteiro nao entendo o ++ pois em c deixa o ponteiro confuso
// }


void cadastrarCliente(FILE *arquivo, Cliente *new_cliente){
    int ch; 
    //if(*contagem == 0) printf("Pressione Enter Para Cadastrar\n");
    while ((ch = getchar()) != '\n' && ch != EOF); //verifica a entrada/limpar buffer // ajuste da funcao do Wesley

    printf("Insira o Nome: \n");
    fgets(new_cliente->nome, sizeof(new_cliente->nome), stdin);
    new_cliente->nome[strcspn(new_cliente->nome, "\n")] = '\0';

    printf("Insira um nome social:");
    fgets(new_cliente->nome_social, sizeof(new_cliente->nome_social), stdin);
    new_cliente->nome_social[strcspn(new_cliente->nome_social, "\n")] = '\0';

    printf("Insira o cpf:");
    fgets(new_cliente->cpf, sizeof(new_cliente->cpf), stdin);
    new_cliente->cpf[strcspn(new_cliente->cpf, "\n")] = '\0';

    printf("Insira o nome da rua ou o numero da residencia:");
    fgets(new_cliente->rua_num, sizeof(new_cliente->rua_num), stdin);
    new_cliente->rua_num[strcspn(new_cliente->rua_num, "\n")] = '\0';

    printf("Insira o bairro:");
    fgets(new_cliente->bairro, sizeof(new_cliente->bairro), stdin);
    new_cliente->bairro[strcspn(new_cliente->bairro, "\n")] = '\0';

    printf("Insira o numero de telefone:");
    fgets(new_cliente->celular, sizeof(new_cliente->celular), stdin);
    new_cliente->celular[strcspn(new_cliente->celular, "\n")] = '\0';

    // add o codigo tbm
    fprintf(arquivo, "%d,%s,%s,%s,%s,%s,%s\n");
}

int contadorCaracterFile(const char *nome_arquivo, char caracter_desejado){
    int contador = 0;
    char caracter;
    FILE *arquivo = fopen("Dados.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    while ((caracter = fgetc(arquivo)) != EOF){
        if(caracter == caracter_desejado){
            contador++;
        }
    }

    fclose(arquivo);

    printf("Quantidade de %s e de %d", caracter_desejado, contador);

    return contador;
}