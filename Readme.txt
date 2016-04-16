Para responder aos requisitos anunciados para este projeto de SOPE, implementamos uma possível solução, em que desenvolvemos 3 aplicações, que visa o desenvolvimento de um programa simples, eficiente, elegante, e de fácil compreensão.
As aplicações que desenvolvemos foram as seguintes: 

lsdir:
Tem a funcionalidade de abrir um diretório, recolher as informações do mesmo e escrevê-las - por defeito, para a saída standard de output. 
A escrita pretende guardar as informações chave do ficheiro, que, por defeito, são guardadas com um espaço. Porém, para separar o nome do ficheiro da informação seguinte, utilizou-se uma barra '/', pois é um símbolo que não pode ser usado para nomear um ficheiro, garantido na totalidade a sua correta leitura posterior.
Por cada diretório encontrado é gerado uma nova instância deste programa, que vai percorrer esse subdiretório.


lssort:
Nesta aplicação, recorreu-se ao uso de uma pipe e do utilitário 'sort', para ordenar o conteúdo que advém de 'lsdir'. Estando 'lsdir' a escrever para a standard output, foi possível redirecionar esta saída para o canal de escrita do pipe.
Por outro lado, em execução paralela, o 'sort' vai receber o conteúdo através do pipe (redirecionando a standard input), e escrever para o ficheiro "files.txt" (também por duplicação de descritores).
A utilização deste método tem duas vantagens:
Não é necessário um ficheiro auxiliar para escrever o conteúdo produzido por 'lsdir', pois é escrito diretamente no pipe.
A ligação entre a escrita do conteúdo de ‘lsdir’ no pipe, e a leitura do mesmo pelo ‘sort’ em execução paralela, torna o processamento de ordenação mais rápido, pois o ‘sort’ não tem que esperar que todo o conteúdo dos ficheiros seja escrito (à medida que ‘lsdir’ escreve, o ‘sort’ recebe a informação).



rmdup:
É responsável por invocar ‘lssort’ e implementa a principal função deste projeto - tornar os ficheiros duplicados em hardlinks para o mais antigo destes.
Pelo facto de “files.txt” estar ordenado, a comparação de linhas é feita com um “ficheiro pivot” e um “ficheiro candidato”: 
para nomes iguais, o ficheiro que se mantém inalterado encontra-se em primeiro (mais antigo), em “files.txt”, pelo que é o nosso pivot.
ficheiros com o mesmo nome, são colocados sempre em linhas adjacentes, em “files.txt”, pelo que, quando o ficheiro candidato tiver um nome diferente do nome do ficheiro pivot, podemos deixar de processar o pivot atual, passando agora o atual candidato a ter esse estatuto.
	Deste modo, é possível processar todos os ficheiros em “files.txt”, sem ser necessário compará-los todos.
