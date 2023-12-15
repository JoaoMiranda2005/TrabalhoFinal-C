#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
char *obterNomeProdutoPorID(Produto *produtos, int numProdutos,
                            int id_produto) {
  for (int i = 0; i < numProdutos; i++) {
    if (produtos[i].id_produto == id_produto) {
      return produtos[i].nome_produto;
    }
  }
  return "Produto Desconhecido";
}

// Função para inserir uma nova venda no arquivo
void inserirVenda(FILE *ficheiro, Venda *novaVenda) {
  novaVenda->valor_final =
      novaVenda->quantidade_produto * novaVenda->valor_produto;
  novaVenda->valor_final_com_acrescimo =
      novaVenda->valor_final + (novaVenda->valor_final * 0.23);

  fseek(ficheiro, 0, SEEK_END);
  fwrite(novaVenda, sizeof(Venda), 1, ficheiro);
}

// Função para consultar artigos vendidos para um cliente num período específico
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
    if (vendas[i].id_cliente == id_cliente &&
        strcmp(vendas[i].data, data_inicial) >= 0 &&
        strcmp(vendas[i].data, data_final) <= 0) {
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

  printf("Consulta de Vendas para Pagamento %s:\n",
         (pago_consulta == 1) ? "Pago" : "Não Pago");
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

// Função para alterar informações de uma venda existente
void alterarVenda(FILE *ficheiro, Venda *vendas, int numVendas) {
  int id_venda;
  printf("Insira o ID da venda a ser alterada: ");
  scanf("%d", &id_venda);

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
      printf("Quantidade Produto: ");
      scanf("%d", &vendas[i].quantidade_produto);
      printf("Valor Produto: ");
      scanf("%f", &vendas[i].valor_produto);
      printf("Valor Final: ");
      scanf("%f", &vendas[i].valor_final);
      printf("Data: ");
      scanf("%s", vendas[i].data);
      printf("Pago (0 - Não, 1 - Sim): ");
      scanf("%d", &vendas[i].pago);

      // Atualiza no ficheiro
      fseek(ficheiro, i * sizeof(Venda), SEEK_SET);
      fwrite(&vendas[i], sizeof(Venda), 1, ficheiro);

      printf("Venda alterada com sucesso.\n");
      break;
    }
  }

  if (!encontrada) {
    printf("Venda com ID %d não encontrada.\n", id_venda);
  }
}

// Declaração de uma variável estática para o ID da venda
static int proximoIdVenda = 1;

// Função para obter o próximo ID da venda
int obterProximoIdVenda() {
  static int proximoIdVenda = 1;
  return proximoIdVenda++;
}

// Função para mostrar o submenu de inserção de venda
void mostrarSubMenuInserirVenda(FILE *ficheiro, Venda *vendas, int *numVendas,
                                Produto *produtos, int *numProdutos) {
  Venda novaVenda;

  // Obtém automaticamente o próximo ID da venda
  novaVenda.id_venda = obterProximoIdVenda();

  printf("Inserir Venda:\n");
  printf("ID Cliente: ");
  scanf("%d", &novaVenda.id_cliente);
  printf("Nome Cliente: ");
  scanf("%s", novaVenda.nome_cliente);
  printf("ID Fornecedor: ");
  scanf("%d", &novaVenda.id_fornecedor);
  printf("ID Produto: ");
  scanf("%d", &novaVenda.id_produto);

  // Obter o nome e o valor do produto pelo ID
  strcpy(novaVenda.nome_produto,
         obterNomeProdutoPorID(produtos, *numProdutos, novaVenda.id_produto));
  novaVenda.valor_produto =
      obterValorProdutoPorID(produtos, *numProdutos, novaVenda.id_produto);

  // Lê a quantidade do produto
  printf("Quantidade Produto: ");
  scanf("%d", &novaVenda.quantidade_produto);

  // Cálculo do valor final sem acréscimo
  novaVenda.valor_final =
      novaVenda.quantidade_produto * novaVenda.valor_produto;
  novaVenda.valor_final_com_acrescimo =
      novaVenda.valor_final + (novaVenda.valor_final * 0.23); // 23%

  // Obtém a data atual
  time_t t;
  struct tm *dataAtual;
  time(&t);
  dataAtual = localtime(&t);

  // Formata a data como dd/mm/aaaa
  strftime(novaVenda.data, sizeof(novaVenda.data), "%d/%m/%Y", dataAtual);

  // Lê o restante das informações do usuário
  printf("Pago (0 - Não, 1 - Sim): ");
  scanf("%d", &novaVenda.pago);

  // Adiciona ao array em memória
  vendas[*numVendas] = novaVenda;
  (*numVendas)++;

  // Atualiza no ficheiro
  fseek(ficheiro, 0, SEEK_END);
  fwrite(&novaVenda, sizeof(Venda), 1, ficheiro);

  FILE *arquivo_saida;
  arquivo_saida = fopen("vendas.txt", "a");

  if (arquivo_saida != NULL) {
    // Imprime a tabela no console e no arquivo
    fprintf(arquivo_saida,
            "\n\n\n\n%-10d | %-16s | %-14d | %-10s | %-10d | %-13.2f | %-11.2f "
            "| %-11.2f | %-26.2f\n",
            novaVenda.id_venda, novaVenda.nome_cliente, novaVenda.id_fornecedor,
            novaVenda.nome_produto, novaVenda.quantidade_produto,
            novaVenda.valor_produto, novaVenda.valor_final,
            novaVenda.valor_final_com_acrescimo,
            novaVenda.valor_final_com_acrescimo);

    printf("\n\n\n\n%-10s | %-16s | %-14s | %-10s | %-10s | %-13s | %-11s | "
           "%-11s | %-26s\n",
           "ID Venda", "Nome Cliente", "ID Fornecedor", "Nome Produto",
           "Quantidade", "Valor Produto", "Valor Final",
           "Valor Final com Acréscimo", "Data");

    printf("%-10d | %-16s | %-14d | %-10s | %-10d | %-13.2f | %-11.2f | "
           "%-11.2f | %-26.2f\n",
           novaVenda.id_venda, novaVenda.nome_cliente, novaVenda.id_fornecedor,
           novaVenda.nome_produto, novaVenda.quantidade_produto,
           novaVenda.valor_produto, novaVenda.valor_final,
           novaVenda.valor_final_com_acrescimo,
           novaVenda.valor_final_com_acrescimo);

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
void mostrarSubMenuAlterarVenda(FILE *ficheiro, Venda *vendas, int numVendas) {
  alterarVenda(ficheiro, vendas, numVendas);
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
    mostrarSubMenuAlterarVenda(ficheiro, vendas, *numVendas);
    break;
  case 0:
    break;
  default:
    printf("Opcao invalida. Tente novamente.\n");
  }

  return opcao;
}
// Função principal do programa
int main() {
  FILE *ficheiro = fopen("vendas.txt", "r+");
  if (ficheiro == NULL) {
    printf("Erro ao abrir o ficheiro.\n");
    return 1;
  }

  // Initialize products and number of products
  Produto produtos[] = {
      {1, "Produto1", 10.0}, // Exemplo de valor: 10.0
      {2, "Produto2", 15.0}, // Exemplo de valor: 15.0
                             // Adicione mais produtos conforme necessário
  };
  int numProdutos = sizeof(produtos) / sizeof(produtos[0]);

  fseek(ficheiro, 0, SEEK_SET);

  Venda vendas[100] = {0}; // Initialize array elements with zeros
  int numVendas = 0;

  // Carregar vendas do ficheiro
  while (fread(&vendas[numVendas], sizeof(Venda), 1, ficheiro) == 1) {
    numVendas++;
  }

  int opcaoPrincipal;
  do {
    opcaoPrincipal = mostrarMenuPrincipal(ficheiro, vendas, &numVendas,
                                          produtos, &numProdutos);

  } while (opcaoPrincipal != 0);

  fclose(ficheiro);
  return 0;
}