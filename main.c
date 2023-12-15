#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VENDAS 100 // Adjust the maximum number of vendas as needed
// Estrutura para representar informações sobre um Produto
typedef struct {
  int id_produto;
  char nome_produto[50];
  float valor_produto;
} Produto;

// Estrutura para representar informações sobre uma Venda
typedef struct {
  int id_venda;
  int id_cliente;
  char nome_cliente[50];
  int id_fornecedor;
  int id_produto;
  int quantidade_produto;
  float valor_produto;
  float valor_final;
  float valor_final_com_acrescimo;
  char data[20];
  int pago;
  char nome_produto[50];
} Venda;

Venda *alocarMemoriaVendas(int numVendas) {
    Venda *vendas = malloc(numVendas * sizeof(Venda));
    if (vendas == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    return vendas;
}

// Função para obter o valor do produto pelo ID
float obterValorProdutoPorID(Produto *produtos, int numProdutos,
                             int id_produto) {
  for (int i = 0; i < numProdutos; i++) {
    if (produtos[i].id_produto == id_produto) {
      return produtos[i].valor_produto;
    }
  }
  return 0.0; // Valor padrão caso o produto não seja encontrado (pode ser
              // ajustado conforme necessário)
}

// Função para obter o nome do produto pelo ID
char *obterNomeProdutoPorID(Produto *produtos, int numProdutos, int id_produto) {
    for (int i = 0; i < numProdutos; i++) {
        printf("Checking product: ID(%d) - Target ID(%d)\n", produtos[i].id_produto, id_produto);
        if (produtos[i].id_produto == id_produto) {
            return produtos[i].nome_produto;
        }
    }
    printf("Product not found!\n");
    return NULL; // Return NULL if the product is not found
}


void inserirVenda(FILE *ficheiro, Venda *novaVenda, Produto *produtos, int numProdutos, float valorProduto) {
    // Atribui manualmente o valor do produto
    novaVenda->valor_produto = valorProduto;

    // Calcula os valores finais
    novaVenda->valor_final = novaVenda->quantidade_produto * novaVenda->valor_produto;
    novaVenda->valor_final_com_acrescimo = novaVenda->valor_final + (novaVenda->valor_final * 0.23);

    // Restante do código para pagamento, data, etc.

    // Move o cursor para o final do arquivo
    fseek(ficheiro, 0, SEEK_END);

    // Escreve a nova venda no arquivo
    if (fwrite(novaVenda, sizeof(Venda), 1, ficheiro) != 1) {
        printf("Error writing to the file.\n");
    }

    // Atualiza o arquivo imediatamente
    fflush(ficheiro);
}




// Função para consultar artigos vendidos para um cliente num período específico
// Correct function signature
void consultarArtigosVendidos(Venda *vendas, int numVendas) {
  int id_cliente;
  char data_inicial[20];
  char data_final[20];

  printf("ID Cliente: ");
  scanf("%d", &id_cliente);
  printf("Data Inicial: ");
  scanf("%s", data_inicial);
  printf("Data Final: ");
  scanf("%s", data_final);

  printf("Artigos vendidos para o cliente %d no período de %s a %s:\n",
         id_cliente, data_inicial, data_final);
  printf("ID Venda\tQuantidade\tValor Produto\tData\n");

  for (int i = 0; i < numVendas; i++) {
    struct tm data_Inicial, dataFinal, vendaData;
    strptime(data_inicial, "%d/%m/%Y", &data_Inicial);
    strptime(data_final, "%d/%m/%Y", &dataFinal);
    strptime(vendas[i].data, "%d/%m/%Y", &vendaData);
    time_t data_InicialTime = mktime(&data_Inicial);
    time_t dataFinalTime = mktime(&dataFinal);
    time_t vendaDataTime = mktime(&vendaData);

    if (vendaDataTime >= data_InicialTime && vendaDataTime <= dataFinalTime) {
      printf("%d\t\t%d\t\t%.2f\t\t%s\n", vendas[i].id_venda,
             vendas[i].quantidade_produto, vendas[i].valor_produto,
             vendas[i].data);
    }
  }
}


// Função para consultar informações gerais sobre vendas
void consultarVendasGerais(Venda *vendas, int numVendas) {
  printf("Consulta de Vendas Gerais:\n");
  for (int i = 0; i < numVendas; i++) {
    printf("ID Venda: %d, ID Cliente: %d, Data: %s, Pago: %s\n",
     vendas[i].id_venda, vendas[i].id_cliente, vendas[i].data,
     (vendas[i].pago == 1) ? "Sim" : "Não");
  }
}

// Função para consultar vendas para um cliente específico
void consultarVendasPorIDCliente(Venda *vendas, int numVendas) {
  int id_cliente;
  printf("Insira o ID do cliente para consultar vendas: ");
  scanf("%d", &id_cliente);

  printf("Consulta de Vendas para o Cliente %d:\n", id_cliente);
  for (int i = 0; i < numVendas; i++) {
    if (vendas[i].id_cliente == id_cliente) {
      printf("ID Venda: %d, Data: %s, Pago: %s\n", vendas[i].id_venda,
             vendas[i].data, (vendas[i].pago == 1) ? "Sim" : "Não");
    }
  }
}

// Função para consultar detalhes de uma venda específica pelo ID da venda
void consultarVendasPorIDVenda(Venda *vendas, int numVendas) {
  int id_venda;
  printf("Insira o ID da venda para consultar detalhes: ");
  scanf("%d", &id_venda);

  printf("Consulta Detalhada de Venda (ID Venda %d):\n", id_venda);
  for (int i = 0; i < numVendas; i++) {
    if (vendas[i].id_venda == id_venda) {
      printf("ID Cliente: %d, Data: %s, Pago: %s\n", vendas[i].id_cliente,
             vendas[i].data, (vendas[i].pago == 1) ? "Sim" : "Não");
    }
  }
}

// Função para consultar vendas para uma data específica
void consultarVendasPorData(Venda *vendas, int numVendas) {
  char data_consulta[20];
  printf("Insira a data para consultar vendas (formato dd/mm/aaaa): ");
  scanf("%s", data_consulta);

  printf("Consulta de Vendas para a Data %s:\n", data_consulta);
  for (int i = 0; i < numVendas; i++) {
    if (strcmp(vendas[i].data, data_consulta) == 0) {
      printf("ID Venda: %d, ID Cliente: %d, Pago: %s\n", vendas[i].id_venda,
             vendas[i].id_cliente, (vendas[i].pago == 1) ? "Sim" : "Não");
    }
  }
}

// Função para consultar vendas com base no estado de pagamento
void consultarVendasPorPagamento(Venda *vendas, int numVendas) {
  int pago_consulta;
  printf("Insira o status de pagamento para consultar vendas (1 - Pago, 0 - "
         "Não Pago): ");
  scanf("%d", &pago_consulta);

  printf("Consulta de Vendas para Pagamento %d:\n", pago_consulta);
  for (int i = 0; i < numVendas; i++) {
    if (vendas[i].pago == pago_consulta) {
      printf("ID Venda: %d, ID Cliente: %d, Data: %s\n", vendas[i].id_venda,
             vendas[i].id_cliente, vendas[i].data);
    }
  }
}
// Função para o menu de consulta de vendas
void consultarVendas(Venda *vendas, int numVendas) {
  int opcaoConsulta;
  do {
    printf("\nMenu de Consulta de Vendas:\n");
    printf("1. Consultar Vendas Gerais\n");
    printf("2. Consultar por ID Cliente\n");
    printf("3. Consultar por ID Venda\n");
    printf("4. Consultar por Data\n");
    printf("5. Consultar por Pagamento\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("Escolha a opcao: ");
    scanf("%d", &opcaoConsulta);

    switch (opcaoConsulta) {
    case 1:
      // Lógica para consultar vendas gerais
      consultarVendasGerais(vendas, numVendas);
      break;
    case 2:
      // Lógica para consultar por ID Cliente
      consultarVendasPorIDCliente(vendas, numVendas);
      break;
    case 3:
      // Lógica para consultar por ID Venda
      consultarVendasPorIDVenda(vendas, numVendas);
      break;
    case 4:
      // Lógica para consultar por Data
      consultarVendasPorData(vendas, numVendas);
      break;
    case 5:
      // Lógica para consultar por Pagamento
      consultarVendasPorPagamento(vendas, numVendas);
      break;
    case 0:
      // Sair
      break;
    default:
      printf("Opcao invalida.\n");
    }
  } while (opcaoConsulta != 0);
}

void alterarVenda(FILE *ficheiro, Venda *vendas, int numVendas, Produto *produtos, int numProdutos) {
  int id_venda;
  printf("Insira o ID da venda a ser alterada: ");
  if (scanf("%d", &id_venda) != 1) {
    printf("Entrada inválida. Certifique-se de inserir um número.\n");
    return;
  }

  int encontrada = 0;
  for (int i = 0; i < numVendas; i++) {
    if (vendas[i].id_venda == id_venda) {
      encontrada = 1;

      printf("Novos dados para a venda:\n");
      printf("ID Cliente: ");
      scanf("%d", &vendas[i].id_cliente);
      printf("Nome Cliente: ");
      scanf("%s", vendas[i].nome_cliente);
      printf("ID Fornecedor: ");
      scanf("%d", &vendas[i].id_fornecedor);

      // Obtém automaticamente o nome e o valor do produto pelo ID
      printf("ID Produto: ");
      if (scanf("%d", &vendas[i].id_produto) != 1) {
          printf("Entrada inválida. Certifique-se de inserir um número.\n");
          return;
      }


      int indiceProduto = -1;
      for (int j = 0; j < numProdutos; j++) {
        if (produtos[j].id_produto == vendas[i].id_produto) {
          indiceProduto = j;
          break;
        }
      }

      if (indiceProduto != -1) {
        strcpy(vendas[i].nome_produto, produtos[indiceProduto].nome_produto);
        vendas[i].valor_produto = produtos[indiceProduto].valor_produto;
      } else {
        printf("Produto com ID %d não encontrado.\n", vendas[i].id_produto);
        // Se o produto não for encontrado, você pode optar por tratar de alguma
        // forma. Neste exemplo, estamos definindo um valor padrão.
        strcpy(vendas[i].nome_produto, "Produto Desconhecido");
        vendas[i].valor_produto = 0.0;
      }

      printf("Quantidade Produto: ");
      if (scanf("%d", &vendas[i].quantidade_produto) != 1) {
        printf("Entrada inválida. Certifique-se de inserir um número.\n");
        return;
      }

      // Cálculo do valor final sem acréscimo
      vendas[i].valor_final = vendas[i].quantidade_produto * vendas[i].valor_produto;
      vendas[i].valor_final_com_acrescimo = vendas[i].valor_final + (vendas[i].valor_final * 0.23); // 23%

      // Obtém a data atual
      time_t t;
      struct tm *dataAtual;
      time(&t);
      dataAtual = localtime(&t);

      // Formata a data como dd/mm/aaaa
      strftime(vendas[i].data, sizeof(vendas[i].data), "%d/%m/%Y", dataAtual);

      // Lê o status de pagamento
      printf("ID Cliente: %d, Data: %s, Pago: %s\n", vendas[i].id_cliente,
         vendas[i].data, (vendas[i].pago == 1) ? "Sim" : "Não");
      if (scanf("%d", &vendas[i].pago) != 1) {
        printf("Entrada inválida. Certifique-se de inserir 0 ou 1.\n");
        return;
      }

      // Verifica se a venda está paga
      if (vendas[i].pago == 1) {
        printf("Venda paga.\n");
      } else {
        printf("Venda não paga.\n");
      }

      // Atualiza no ficheiro apenas se a venda foi encontrada e alterada
      fseek(ficheiro, i * sizeof(Venda), SEEK_SET);
      fwrite(&vendas[i], sizeof(Venda), 1, ficheiro);

      // Sai do loop, pois a venda foi encontrada e alterada
      break;
    }
  }

  if (encontrada) {
    printf("Venda alterada com sucesso.\n");
  } else {
    printf("Venda com ID %d não encontrada.\n", id_venda);
  }
}


// Declaração de uma variável estática para o ID da venda
static int proximoIdVenda = 1;

// Função para obter o próximo ID da venda
// Função para obter e salvar o próximo ID da venda
// Função para obter e salvar o próximo ID da venda
int obterProximoIdVenda() {
  static int proximoIdVenda = 1;

  // Tenta abrir o arquivo para leitura
  FILE *arquivoId = fopen("proximoId.txt", "r");

  // Se conseguir abrir, lê o próximo ID salvo
  if (arquivoId != NULL) {
    fscanf(arquivoId, "%d", &proximoIdVenda);
    fclose(arquivoId);
  } else {
    // Se não conseguir abrir, cria o arquivo com o próximo ID
    arquivoId = fopen("proximoId.txt", "w");
    fprintf(arquivoId, "%d", proximoIdVenda);
    fclose(arquivoId);
  }

  // Abre o arquivo para escrita (ou cria se não existir)
  arquivoId = fopen("proximoId.txt", "w");

  // Salva o próximo ID + 1 para a próxima vez
  fprintf(arquivoId, "%d", proximoIdVenda + 1);

  fclose(arquivoId);

  return proximoIdVenda++;
}

void mostrarSubMenuInserirVenda(FILE *ficheiro, Venda *vendas, int *numVendas,
                                Produto *produtos, int *numProdutos) {
    Venda novaVenda;

    // Obtain the next ID for the venda
    novaVenda.id_venda = obterProximoIdVenda();

    printf("Inserir Venda:\n");
    printf("ID Cliente: ");
    scanf("%d", &novaVenda.id_cliente);
    printf("Nome Cliente: ");
    scanf("%s", novaVenda.nome_cliente);
    printf("ID Fornecedor: ");
    scanf("%d", &novaVenda.id_fornecedor);
    printf("ID Produto: ");
    if (scanf("%d", &novaVenda.id_produto) != 1) {
        printf("Entrada inválida. Certifique-se de inserir um número.\n");
        return;
    }

    // Obtain the name and value of the product by ID
    char *nomeProduto = obterNomeProdutoPorID(produtos, *numProdutos, novaVenda.id_produto);
    if (nomeProduto == NULL) {
        printf("Produto não encontrado.\n");
        return;
    }

    strcpy(novaVenda.nome_produto, nomeProduto);
    novaVenda.valor_produto = obterValorProdutoPorID(produtos, *numProdutos, novaVenda.id_produto);

    // Read the quantity of the product
    printf("Quantidade Produto: ");
    scanf("%d", &novaVenda.quantidade_produto);

    // Calculate the final value without increment
    novaVenda.valor_final = novaVenda.quantidade_produto * novaVenda.valor_produto;
    novaVenda.valor_final_com_acrescimo = novaVenda.valor_final + (novaVenda.valor_final * 0.23); // 23%

    // Obtain the current date
    time_t t;
    struct tm *dataAtual;
    time(&t);
    dataAtual = localtime(&t);

    // Format the date as dd/mm/yyyy
    strftime(novaVenda.data, sizeof(novaVenda.data), "%d/%m/%Y", dataAtual);

    // Read the remaining user information
    printf("Pago (0 - Não, 1 - Sim): ");
    scanf("%d", &novaVenda.pago);

    // Add to the array in memory
    vendas[*numVendas] = novaVenda;

    // Update in the file
    fseek(ficheiro, 0, SEEK_END);
    fwrite(&novaVenda, sizeof(Venda), 1, ficheiro);

    FILE *arquivo_saida;
    arquivo_saida = fopen("vendas.txt", "a");

    if (arquivo_saida != NULL) {
        // Print the table to the console and the file
        fprintf(arquivo_saida,
                "\n\n\n\n%-10d | %-16s | %-14d | %-10s | %-10d | %-13.2f | %-11.2f "
                "| %-11.2f | %-11.2f | %-26s\n",
                novaVenda.id_venda, novaVenda.nome_cliente, novaVenda.id_fornecedor,
                novaVenda.nome_produto, novaVenda.quantidade_produto,
                novaVenda.valor_produto, novaVenda.valor_final,
                novaVenda.valor_final_com_acrescimo,
                (novaVenda.pago == 1) ? "Sim" : "Não", novaVenda.data);

        printf("\n\n\n\n%-10s | %-16s | %-14s | %-10s | %-10s | %-13s | %-11s | "
                "%-11s | %-11s | %-26s\n",
                "ID Venda", "Nome Cliente", "ID Fornecedor", "Nome Produto",
                "Quantidade", "Valor Produto", "Valor Final",
                "Valor Final com Acréscimo", "Pago", "Data");

        printf("%-10d | %-16s | %-14d | %-10s | %-10d | %-13.2f | %-11.2f | "
                "%-11.2f | %-11s | %-26s\n",
                novaVenda.id_venda, novaVenda.nome_cliente, novaVenda.id_fornecedor,
                novaVenda.nome_produto, novaVenda.quantidade_produto,
                novaVenda.valor_produto, novaVenda.valor_final,
                novaVenda.valor_final_com_acrescimo,
                (novaVenda.pago == 1) ? "Sim" : "Não", novaVenda.data);

        fclose(arquivo_saida);
    } else {
        printf("Erro ao abrir o arquivo de saída.\n");
    }
}




// Função para mostrar o submenu de consulta de artigos vendidos
void mostrarSubMenuConsultarArtigosVendidos(Venda *vendas, int numVendas) {
  consultarArtigosVendidos(vendas, numVendas);
}
// Função para mostrar o submenu de consulta de vendas
void mostrarSubMenuConsultarVendas(Venda *vendas, int numVendas) {
  consultarVendas(vendas, numVendas);
}
// Função para mostrar o submenu de alteração de venda
void mostrarSubMenuAlterarVenda(FILE *ficheiro, Venda *vendas, int *numVendas,
                                Produto *produtos, int *numProdutos) {
  alterarVenda(ficheiro, vendas, *numVendas, produtos, *numProdutos);
}

// Função que exibe o menu principal e chama as funções correspondentes com base
// na escolha do usuário
int mostrarMenuPrincipal(FILE *ficheiro, Venda *vendas, int *numVendas,
                         Produto *produtos, int *numProdutos) {
  int opcao;
  printf("\nMenu Principal:\n");
  printf("1. Inserir Venda\n");
  printf("2. Consultar Artigos Vendidos\n");
  printf("3. Consultar Vendas\n");
  printf("4. Alterar Venda\n");
  printf("0. Sair\n");
  printf("Escolha a opcao: ");
  scanf("%d", &opcao);

  switch (opcao) {
  case 1:
    mostrarSubMenuInserirVenda(ficheiro, vendas, numVendas, produtos,
                               numProdutos);
    break;
  case 2:
    mostrarSubMenuConsultarArtigosVendidos(vendas, *numVendas);
    break;
  case 3:
    mostrarSubMenuConsultarVendas(vendas, *numVendas);
    break;
  case 4:
    mostrarSubMenuAlterarVenda(ficheiro, vendas, &numVendas, produtos,
                               &numProdutos);

    break;
  case 0:
    break;
  default:
    printf("Opcao invalida. Tente novamente.\n");
  }

  return opcao;
}
int main() {
    FILE *ficheiro = fopen("vendas.txt", "r+");
    if (ficheiro == NULL) {
        printf("Erro ao abrir o ficheiro.\n");
        return 1;
    }

    // Allocate memory for vendas array
    Venda *vendas = alocarMemoriaVendas(MAX_VENDAS);

    Produto produtos[] = {
        {1, "Produto1", 10.0},
        {2, "Produto2", 15.0},
        // Add more products as needed
    };
    int numProdutos = sizeof(produtos) / sizeof(produtos[0]);

    int numVendas = 0; // Initialize the number of vendas

    int opcaoPrincipal;
    do {
        opcaoPrincipal = mostrarMenuPrincipal(ficheiro, vendas, &numVendas,
                                              produtos, &numProdutos);
    } while (opcaoPrincipal != 0);

    // Free dynamically allocated memory
    free(vendas);
    fclose(ficheiro);
    return 0;
}

// Consultar Artigos Vendidos n funciona, provavelmente n esta a salvar
// Id da venda tambem nao consegues usar, sque n ta a salvar
// Alterar venda n esta testado, de resto o code está ok
