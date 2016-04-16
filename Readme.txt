Para responder aos requisitos anunciados para este projeto de SOPE, implementamos uma poss�vel solu��o, em que desenvolvemos 3 aplica��es, que visa o desenvolvimento de um programa simples, eficiente, elegante, e de f�cil compreens�o.
As aplica��es que desenvolvemos foram as seguintes: 

lsdir:
Tem a funcionalidade de abrir um diret�rio, recolher as informa��es do mesmo e escrev�-las - por defeito, para a sa�da standard de output. 
A escrita pretende guardar as informa��es chave do ficheiro, que, por defeito, s�o guardadas com um espa�o. Por�m, para separar o nome do ficheiro da informa��o seguinte, utilizou-se uma barra '/', pois � um s�mbolo que n�o pode ser usado para nomear um ficheiro, garantido na totalidade a sua correta leitura posterior.
Por cada diret�rio encontrado � gerado uma nova inst�ncia deste programa, que vai percorrer esse subdiret�rio.


lssort:
Nesta aplica��o, recorreu-se ao uso de uma pipe e do utilit�rio 'sort', para ordenar o conte�do que adv�m de 'lsdir'. Estando 'lsdir' a escrever para a standard output, foi poss�vel redirecionar esta sa�da para o canal de escrita do pipe.
Por outro lado, em execu��o paralela, o 'sort' vai receber o conte�do atrav�s do pipe (redirecionando a standard input), e escrever para o ficheiro "files.txt" (tamb�m por duplica��o de descritores).
A utiliza��o deste m�todo tem duas vantagens:
N�o � necess�rio um ficheiro auxiliar para escrever o conte�do produzido por 'lsdir', pois � escrito diretamente no pipe.
A liga��o entre a escrita do conte�do de �lsdir� no pipe, e a leitura do mesmo pelo �sort� em execu��o paralela, torna o processamento de ordena��o mais r�pido, pois o �sort� n�o tem que esperar que todo o conte�do dos ficheiros seja escrito (� medida que �lsdir� escreve, o �sort� recebe a informa��o).



rmdup:
� respons�vel por invocar �lssort� e implementa a principal fun��o deste projeto - tornar os ficheiros duplicados em hardlinks para o mais antigo destes.
Pelo facto de �files.txt� estar ordenado, a compara��o de linhas � feita com um �ficheiro pivot� e um �ficheiro candidato�: 
para nomes iguais, o ficheiro que se mant�m inalterado encontra-se em primeiro (mais antigo), em �files.txt�, pelo que � o nosso pivot.
ficheiros com o mesmo nome, s�o colocados sempre em linhas adjacentes, em �files.txt�, pelo que, quando o ficheiro candidato tiver um nome diferente do nome do ficheiro pivot, podemos deixar de processar o pivot atual, passando agora o atual candidato a ter esse estatuto.
	Deste modo, � poss�vel processar todos os ficheiros em �files.txt�, sem ser necess�rio compar�-los todos.
