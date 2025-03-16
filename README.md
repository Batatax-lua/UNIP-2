# UNIP-2
Programa em C projetado para o segundo semestre do curso superior técnico de ADS na UNIP

Contém uma razoável quantidade de funcionalidades, incluindo, porém não limitadas à:
- Tela de boot detalhada e com música;
- Sistema de Login;
- PDV com função de carrinho de compras e emissão de notas fiscais;
- Estoque de produtos com aviso de baixo estoque;
- Gerenciamento de produtos, funcionários;
- Registro de clientes;
- Administração de funcionários, clientes, gastos e mais;
- Visualização de gastos e lucros mensais, por meio de um gráfico de barras através do API pbPlots;
- Música de fundo customizável através do API Microsoft PowerShell;
- Código QR redirecionável para um PDF do código de defesa do consumidor, através do API Microsoft PowerShell;

Para compilação, utilize o comando através do bash, na pasta raís:
gcc src/*.c iconPrograma.o include/pbPlots.c include/pbPlots.h include/supportLib.c include/supportLib.h -o hortifruti.exe -lwinmm

Acesso aos perfis:
Usuários: vendas1, vendas2
Senha: venda

Usuários: gerente1, gerente2
Senha: gerente

Usuário: admin
Senha: senhadmin