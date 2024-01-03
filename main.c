#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VENDAS 100
#define MAX_STRING 50
#define PAGO 1
#define NAO_PAGO 0
#define PAGO_STRING "Sim"
#define NAO_PAGO_STRING "Não"

typedef struct {
    int id_venda;
    int id_cliente;
    char nome_cliente[MAX_STRING];
    int id_fornecedor;
    int id_produto;
    char nome_produto[MAX_STRING];
    int quantidade_produto;
    float valor_produto;
    float valor_final;
    char data[11];
    bool pago;
} Venda;

typedef struct {
    int id_produto;
    char nome_produto[MAX_STRING];
    float valor_produto;
} Produto;

void inicializarProximoIdVenda() {
    FILE *arquivoProximoId = fopen("proximoIdVenda.txt", "r");
    if (arquivoProximoId == NULL) {
        // O arquivo não existe, então vamos criar um arquivo inicial
        arquivoProximoId = fopen("proximoIdVenda.txt", "w");
        if (arquivoProximoId != NULL) {
            // Escrever um valor inicial no arquivo (por exemplo, 1)
            fprintf(arquivoProximoId, "1");
            fclose(arquivoProximoId);
        } else {
            printf("Erro ao criar o arquivo de próximo ID.\n");
        }
    } else {
        fclose(arquivoProximoId);
    }
}
void salvarProximoIdVenda(int proximoId) {
    FILE *arquivoProximoId = fopen("proximoIdVenda.txt", "w");
    if (arquivoProximoId == NULL) {
        printf("Erro ao abrir o arquivo de próximo ID para escrita.\n");
        return;
    }

    fprintf(arquivoProximoId, "%d", proximoId);

    fclose(arquivoProximoId);
}

char* obterNomeProduto(int id_produto) {
    FILE *arquivoProdutos = fopen("produtos.txt", "r");
    if (arquivoProdutos == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return NULL;
    }

    Produto produto;
    while (fscanf(arquivoProdutos, "%d %s %f", &produto.id_produto, produto.nome_produto, &produto.valor_produto) == 3) {
        if (produto.id_produto == id_produto) {
            fclose(arquivoProdutos);
            // Aloca memória suficiente para armazenar o nome do produto
            char *nomeProduto = malloc(strlen(produto.nome_produto) + 1);
            strcpy(nomeProduto, produto.nome_produto);
            return nomeProduto;
        }
    }

    fclose(arquivoProdutos);
    return NULL; // Retorna NULL se o produto não for encontrado
}

// Função para liberar a memória alocada para o nome do produto
void liberarNomeProduto(char *nomeProduto) {
    free(nomeProduto);
}


int lerProximoIdVenda() {
    FILE *arquivoProximoId = fopen("proximoIdVenda.txt", "r");
    if (arquivoProximoId == NULL) {
        printf("Erro ao abrir o arquivo de próximo ID para leitura. Usando ID padrão.\n");
        return 1;  // Retorna 1 como ID padrão se o arquivo não existir
    }

    int proximoId;
    fscanf(arquivoProximoId, "%d", &proximoId);

    fclose(arquivoProximoId);

    return proximoId;
}

float obterValorProduto(int id_produto) {
    FILE *arquivoProdutos = fopen("produtos.txt", "r");
    if (arquivoProdutos == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return 0.0; // Retorna um valor padrão se o arquivo não puder ser aberto
    }

    Produto produto;
    while (fscanf(arquivoProdutos, "%d %s %f", &produto.id_produto, produto.nome_produto, &produto.valor_produto) == 3) {
        if (produto.id_produto == id_produto) {
            fclose(arquivoProdutos);
            return produto.valor_produto;
        }
    }

    fclose(arquivoProdutos);
    return 0.0;
}

void inicializarProdutos() {
    FILE *arquivoProdutos = fopen("produtos.txt", "r");
    if (arquivoProdutos == NULL) {
        // O arquivo não existe, então vamos criar um arquivo inicial
        arquivoProdutos = fopen("produtos.txt", "w");
        if (arquivoProdutos != NULL) {
            // Escrever alguns produtos iniciais no arquivo
            fprintf(arquivoProdutos, "1 Produto1 10.0\n");
            fprintf(arquivoProdutos, "2 Produto2 15.0\n");
            fprintf(arquivoProdutos, "3 Produto3 20.0\n");
            fclose(arquivoProdutos);
        } else {
            printf("Erro ao criar o arquivo de produtos.\n");
        }
    } else {
        fclose(arquivoProdutos);
    }
}

int verificarExistenciaProduto(int id_produto) {
    FILE *arquivoProdutos = fopen("produtos.txt", "r");
    if (arquivoProdutos == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return 0; // Retorna 0 se o arquivo não puder ser aberto
    }

    Produto produto;
    while (fscanf(arquivoProdutos, "%d %*s %*f", &produto.id_produto) == 1) {
        if (produto.id_produto == id_produto) {
            fclose(arquivoProdutos);
            return 1; // Retorna 1 se o produto existir
        }
    }

    fclose(arquivoProdutos);
    return 0; // Retorna 0 se o produto não existir
}

void inserirVenda(Venda vendas[], int *totalVendas) {
    int proximoId = lerProximoIdVenda();
    int ultimoId = 0;

    for (int i = 0; i < *totalVendas; i++) {
        if (vendas[i].id_venda > ultimoId) {
            ultimoId = vendas[i].id_venda;
        }
    }

    vendas[*totalVendas].id_venda = (ultimoId >= proximoId) ? ultimoId + 1 : proximoId;

    printf("ID Cliente: ");
    scanf("%d", &vendas[*totalVendas].id_cliente);

    printf("Nome Cliente: ");
    scanf("%s", vendas[*totalVendas].nome_cliente);

    printf("ID Fornecedor: ");
    scanf("%d", &vendas[*totalVendas].id_fornecedor);

    do {
        printf("ID Produto: ");
        scanf("%d", &vendas[*totalVendas].id_produto);

        if (!verificarExistenciaProduto(vendas[*totalVendas].id_produto)) {
            printf("Produto inexistente. Por favor, insira um ID de produto válido.\n");
        }
    } while (!verificarExistenciaProduto(vendas[*totalVendas].id_produto));

    vendas[*totalVendas].valor_produto = obterValorProduto(vendas[*totalVendas].id_produto);

    printf("Quantidade Produto: ");
    scanf("%d", &vendas[*totalVendas].quantidade_produto);

    vendas[*totalVendas].valor_final = vendas[*totalVendas].quantidade_produto * vendas[*totalVendas].valor_produto * 1.23;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(vendas[*totalVendas].data, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // Leitura do estado de pagamento
    printf("Está Pago? (Digite 0 para 'Não' ou 1 para 'Sim'): ");
    int pagamento;
    scanf("%d", &pagamento);

    vendas[*totalVendas].pago = (pagamento == 1) ? true : false;

    // Exibição do estado de pagamento
    printf("Está Pago? %s\n", vendas[*totalVendas].pago ? "Sim" : "Não");

    printf("Valor do Produto: %.2f\n", vendas[*totalVendas].valor_produto);
    printf("Valor do Produto (Quantidade): %.2f\n", vendas[*totalVendas].quantidade_produto * vendas[*totalVendas].valor_produto);
    printf("Valor Final (com IVA): %.2f\n", vendas[*totalVendas].valor_final);

    (*totalVendas)++;

    FILE *arquivoVendas = fopen("vendas.txt", "a");
    if (arquivoVendas == NULL) {
        printf("Erro ao abrir o arquivo de vendas para escrita.\n");
        return;
    }

    fprintf(arquivoVendas, "%d %d %s %d %d %s %d %.2f %.2f %s %d\n",
        vendas[*totalVendas - 1].id_venda,
        vendas[*totalVendas - 1].id_cliente,
        vendas[*totalVendas - 1].nome_cliente,
        vendas[*totalVendas - 1].id_fornecedor,
        vendas[*totalVendas - 1].id_produto,
        obterNomeProduto(vendas[*totalVendas - 1].id_produto),
        vendas[*totalVendas - 1].quantidade_produto,
        vendas[*totalVendas - 1].valor_produto,
        vendas[*totalVendas - 1].valor_final,
        vendas[*totalVendas - 1].data,
        vendas[*totalVendas - 1].pago);

    salvarProximoIdVenda(vendas[*totalVendas - 1].id_venda + 1);

    fclose(arquivoVendas);
}



// Função para consultar artigos vendidos
void consultarArtigosVendidos() {
    int idProdutoConsulta;
    char dataConsulta[MAX_STRING];

    printf("Informe o ID do produto: ");
    scanf("%d", &idProdutoConsulta);

    printf("Informe a data no formato DD-MM-AAAA: ");
    scanf("%s", dataConsulta);

    int dia, mes, ano;
    // Extrair dia, mês e ano da data inserida pelo utilizador
    if (sscanf(dataConsulta, "%d-%d-%d", &dia, &mes, &ano) != 3) {
        printf("Formato de data inválido. Use DD-MM-AAAA.\n");
        return;
    }

    // Validar a data (considerando anos bissextos)
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1 || dia > 31 ||
        (mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30 ||
        (mes == 2 && (dia > 29 || (dia == 29 && (ano % 4 != 0 || (ano % 100 == 0 && ano % 400 != 0)))))) {
        printf("Data inválida.\n");
        return;
    }


    FILE *arquivoVendas = fopen("vendas.txt", "r");
    if (arquivoVendas == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    // Variáveis para armazenar os dados da venda
    int idProdutoVenda, quantidadeVenda;
    char nomeClienteVenda[MAX_STRING], dataVenda[MAX_STRING];

    // Variável para armazenar a quantidade total de artigos vendidos
    int quantidadeTotal = 0;

    // Loop para percorrer todas as vendas no arquivo
    while (fscanf(arquivoVendas, "%*d %*d %s %*d %d %*s %d %*f %*f %s %*d", nomeClienteVenda, &idProdutoVenda, &quantidadeVenda, dataVenda) == 4) {
        // Verificar se a venda corresponde aos critérios de consulta
        if (idProdutoConsulta == idProdutoVenda && sscanf(dataVenda, "%d-%d-%d", &dia, &mes, &ano) == 3 &&
            dia == dia && mes == mes && ano == ano) {
            printf("Venda ID: %d, Cliente: %s, Quantidade: %d\n", idProdutoVenda, nomeClienteVenda, quantidadeVenda);

            // Atualizar a quantidade total de artigos vendidos
            quantidadeTotal += quantidadeVenda;
        }
    }

    // Imprimir a quantidade total de artigos vendidos
    printf("Quantidade Total de Artigos Vendidos: %d\n", quantidadeTotal);

    fclose(arquivoVendas);
}

void consultarVendas() {
    FILE *arquivoVendas = fopen("vendas.txt", "r");

    if (arquivoVendas == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    int escolhaConsulta;
    printf("===== Opções de Consulta =====\n");
    printf("1. Por ID do Cliente\n");
    printf("2. Por ID do Produto\n");
    printf("3. Por Data\n");
    printf("Escolha a opção de consulta: ");
    scanf("%d", &escolhaConsulta);

    switch (escolhaConsulta) {
        case 1: {
            int idClienteConsulta;
            printf("Informe o ID do cliente: ");
            scanf("%d", &idClienteConsulta);

            printf("===== Vendas para o Cliente ID %d =====\n", idClienteConsulta);

            Venda venda;
            while (fscanf(arquivoVendas, "%d %d %s %d %d %s %d %f %f %s %d",
                           &venda.id_venda,
                           &venda.id_cliente,
                           venda.nome_cliente,
                           &venda.id_fornecedor,
                           &venda.id_produto,
                           venda.nome_produto,
                           &venda.quantidade_produto,
                           &venda.valor_produto,
                           &venda.valor_final,
                           venda.data,
                           &venda.pago) == 11) {
                if (venda.id_cliente == idClienteConsulta) {
                    printf("ID Venda: %d, Nome Cliente: %s, Valor Final: %.2f\n",
                           venda.id_venda,
                           venda.nome_cliente,
                           venda.valor_final);
                }
            }
            break;
        }
        case 2: {
            int idProdutoConsulta;
            printf("Informe o ID do produto: ");
            scanf("%d", &idProdutoConsulta);

            printf("===== Vendas para o Produto ID %d =====\n", idProdutoConsulta);

            Venda venda;
            while (fscanf(arquivoVendas, "%d %d %s %d %d %s %d %f %f %s %d",
                           &venda.id_venda,
                           &venda.id_cliente,
                           venda.nome_cliente,
                           &venda.id_fornecedor,
                           &venda.id_produto,
                           venda.nome_produto,
                           &venda.quantidade_produto,
                           &venda.valor_produto,
                           &venda.valor_final,
                           venda.data,
                           &venda.pago) == 11) {
                if (venda.id_produto == idProdutoConsulta) {
                    printf("ID Venda: %d, Nome Produto: %s, Valor Final: %.2f\n",
                           venda.id_venda,
                           venda.nome_produto,
                           venda.valor_final);
                }
            }
            break;
        }
        case 3: {
            char dataConsulta[MAX_STRING];
            int dia, mes, ano;
            printf("Informe a data no formato DD-MM-AAAA: ");
            scanf("%s", dataConsulta);

            printf("===== Vendas na Data %s =====\n", dataConsulta);

            Venda venda;
            while (fscanf(arquivoVendas, "%d %d %s %d %d %s %d %f %f %s %d",
                           &venda.id_venda,
                           &venda.id_cliente,
                           venda.nome_cliente,
                           &venda.id_fornecedor,
                           &venda.id_produto,
                           venda.nome_produto,
                           &venda.quantidade_produto,
                           &venda.valor_produto,
                           &venda.valor_final,
                           venda.data,
                           &venda.pago) == 11) {

                // Convertendo o formato da data para "DD-MM-AAAA" para comparação
                char vendaDataFormatada[MAX_STRING];
                sscanf(venda.data, "%4d-%2d-%2d", &ano, &mes, &dia);
                sprintf(vendaDataFormatada, "%02d-%02d-%04d", dia, mes, ano);

                if (strcmp(vendaDataFormatada, dataConsulta) == 0) {
                    printf("ID Venda: %d, Data: %s, Valor Final: %.2f\n",
                           venda.id_venda,
                           venda.data,
                           venda.valor_final);
                }
            }
            break;
        }
        default:
            printf("Opção de consulta inválida.\n");
            break;
    }

    fclose(arquivoVendas);
}

void alterarVenda() {
    int idVendaAltera;
    printf("Informe o ID da venda que deseja alterar: ");
    scanf("%d", &idVendaAltera);

    FILE *arquivoVendas = fopen("vendas.txt", "r");
    FILE *arquivoTemp = fopen("temp.txt", "w");
    FILE *arquivoProdutos = fopen("produtos.txt", "r");

    if (arquivoVendas == NULL || arquivoTemp == NULL || arquivoProdutos == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int encontrado = 0; // Flag para indicar se a venda foi encontrada

    Venda venda;
    while (fscanf(arquivoVendas, "%d %d %s %d %d %s %d %f %f %s %d",
                   &venda.id_venda,
                   &venda.id_cliente,
                   venda.nome_cliente,
                   &venda.id_fornecedor,
                   &venda.id_produto,
                   venda.nome_produto,
                   &venda.quantidade_produto,
                   &venda.valor_produto,
                   &venda.valor_final,
                   venda.data,
                   &venda.pago) == 11) {
        // Verifica se o ID da venda corresponde ao ID informado pelo utilizador
        if (venda.id_venda == idVendaAltera) {
            encontrado = 1;

            // Verifica se a venda já está paga
            if (venda.pago) {
                printf("Não é possível alterar uma venda já paga.\n");
                fclose(arquivoVendas);
                fclose(arquivoTemp);
                fclose(arquivoProdutos);
                remove("temp.txt");
                return;
            }

            // Exibe os detalhes da venda antes da alteração
            printf("Detalhes da Venda (ID: %d):\n", venda.id_venda);
            printf("Nome Cliente: %s\n", venda.nome_cliente);
            printf("Valor Final: %.2f\n", venda.valor_final);
            printf("Está Pago? (%s - Sim, %s - Não): %s\n", PAGO_STRING, NAO_PAGO_STRING, venda.pago ? PAGO_STRING : NAO_PAGO_STRING);

            // Solicita a alteração dos campos
            printf("Novo ID Cliente: ");
            scanf("%d", &venda.id_cliente);

            printf("Novo Nome Cliente: ");
            scanf("%s", venda.nome_cliente);

            printf("Novo ID Fornecedor: ");
            scanf("%d", &venda.id_fornecedor);

            // Corrigir o campo ID Produto e obter o valor do produto do arquivo produtos.txt
            printf("Novo ID Produto: ");
            scanf("%d", &venda.id_produto);

            // Atualizar o valor do produto
            venda.valor_produto = obterValorProduto(venda.id_produto);

            printf("Nova Quantidade Produto: ");
            scanf("%d", &venda.quantidade_produto);

            // Atualiza o valor final
            venda.valor_final = venda.quantidade_produto * venda.valor_produto * 1.23;

            // Exibe os detalhes atualizados
            printf("Detalhes Atualizados:\n");
            printf("Nome Cliente: %s\n", venda.nome_cliente);
            printf("Valor Final: %.2f\n", venda.valor_final);
            printf("Está Pago? (%s)\n", venda.pago ? PAGO_STRING : NAO_PAGO_STRING);

            // Escreve a venda alterada no arquivo temporário
            fprintf(arquivoTemp, "%d %d %s %d %d %s %d %.2f %.2f %s %d\n",
                    venda.id_venda,
                    venda.id_cliente,
                    venda.nome_cliente,
                    venda.id_fornecedor,
                    venda.id_produto,
                    venda.nome_produto,
                    venda.quantidade_produto,
                    venda.valor_produto,
                    venda.valor_final,
                    venda.data,
                    venda.pago);

            printf("Venda alterada com sucesso.\n");
        } else {
            // Se não for a venda a ser alterada, copia para o arquivo temporário
            fprintf(arquivoTemp, "%d %d %s %d %d %s %d %.2f %.2f %s %d\n",
                    venda.id_venda,
                    venda.id_cliente,
                    venda.nome_cliente,
                    venda.id_fornecedor,
                    venda.id_produto,
                    venda.nome_produto,
                    venda.quantidade_produto,
                    venda.valor_produto,
                    venda.valor_final,
                    venda.data,
                    venda.pago);
        }
    }

    if (!encontrado) {
        printf("Venda com ID %d não encontrada.\n", idVendaAltera);
    }

    fclose(arquivoVendas);
    fclose(arquivoTemp);
    fclose(arquivoProdutos);

    // Copia o conteúdo original de volta para o arquivo vendas.txt
    FILE *arquivoTempOriginal = fopen("temp.txt", "r");
    FILE *arquivoVendasOriginal = fopen("vendas.txt", "w");

    if (arquivoTempOriginal == NULL || arquivoVendasOriginal == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int c;
    while ((c = fgetc(arquivoTempOriginal)) != EOF) {
        fputc(c, arquivoVendasOriginal);
    }

    fclose(arquivoTempOriginal);
    fclose(arquivoVendasOriginal);

    // Remove o arquivo temporário
    remove("temp.txt");
}


int main() {
    // Inicializar o arquivo de produtos se necessário
    inicializarProdutos();
    inicializarProximoIdVenda();
    Venda vendas[MAX_VENDAS];
    int totalVendas = 0;

    int escolha;
    do {
        printf("\n===== Menu Principal =====\n");
        printf("1. Inserir Venda\n");
        printf("2. Consultar Artigos Vendidos\n");
        printf("3. Consultar Vendas\n");
        printf("4. Alterar Venda\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                inserirVenda(vendas, &totalVendas);
                break;
            case 2:
                consultarArtigosVendidos(vendas, totalVendas);
                break;
            case 3:
                consultarVendas();
                break;
            case 4:
                alterarVenda();
                break;
            case 0:
                // Salvar as vendas no arquivo antes de sair
                printf("Saindo do programa. Obrigado!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (escolha != 0);

    return 0;
}
