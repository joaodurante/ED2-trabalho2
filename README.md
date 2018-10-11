# Trabalho Aula 6

O objetivo do exercício é gerenciar os livros de uma biblioteca. Para tanto, as seguintes informações são armazenadas: o ISBN do livro (chave primária), o título do livro, o autor, o ano e outras informações que você achar necessárias. O arquivo a ser criado deve ser de registros e campos de tamanho variável, com um inteiro (4 bytes) no início do registro indicando o tamanho do registro, e com campos separados pelo caractere ‘#’. 

| ISBN | Título | Autor | Ano |
| ---- | ------ | ----- | --- |
| 13 caracteres (fixo) | 50 caracteres (máximo) | 50 caracteres (máximo) | 4 caracteres (fixo) |

Ex.: 429788578610630#Estrutura de Dados#Folk#2000

As seguintes operações deverão estar disponíveis: 
1. Inserção 
2. Procurar por “ISBN” (índice primário)
3. Procurar por “Autor” (índice secundário / lista invertida) 
4. Carrega Arquivos 
5. Dump Arquivos 

__Inserção (1)__

Insere o registro no final do arquivo. Os dados a serem inseridos devem ser recuperados de um arquivo a ser fornecido no momento da execução do programa (vide Opção 4). 

__Pesquisa por chave primária (2)__

Vocês devem criar um arquivo de índice que contenha a lista dos “ISBN” presentes em seu arquivo de dados junto com o deslocamento necessário para acessar o registro de cada chave presente no arquivo. Assim, uma consulta deve primeiramente procurar a chave desejada neste novo arquivo e depois acessar diretamente o registro desejado no arquivo de dados. Os dados relacionados ao ISBN pesquisado devem ser exibidos. 

Observações: 
1. Agora, a inserção de um registro requer a manipulação de 2 arquivos (dados e índice). 
2. A busca por um código no índice primário pode ser feita sequencialmente ou por pesquisa binária. 
3. O índice deve ser mantido em memória principal e, em caso do programa ser interrompido inesperadamente, o índice deve ser recriado a partir do arquivo de dados. Desse modo, deve existir uma função que carrega o índice para a memória e uma que recria o índice quando necessário. Para criar/recriar o índice utilizem o Keysorting. 

__Pesquisa por chave secundária (3)__

Construa um índice secundário para acesso pelo “Autor”. Este índice consiste de 2 arquivos, formando uma lista invertida com o índice primário e o arquivo de dados. O primeiro arquivo do índice secundário contém a lista dos nomes dos autores, sem repetições, presentes no arquivo de dados junto com o deslocamento necessário para acessar uma lista de registros de cada nome no segundo arquivo. nome_autor1 offset1 nome_autor2 offset2 ... 

Já o segundo arquivo do índice secundário contém uma chave primária (“ISBN”) correspondente a uma publicação seguida de um novo deslocamento para este mesmo arquivo, formando uma lista ligada de chaves primárias. Quando este deslocamento for -1 quer dizer que a lista ligada acabou. ISBN1 offset1 ISBN2 offset2 ...

Observações: as mesmas da Opção 2.   

__Carrega Arquivos (4)__

A fim de facilitar os testes, serão fornecidos três arquivos: (a) “biblioteca.bin”, (b) “busca_p.bin” e (c) “busca_s.bin”. O primeiro (a) conterá os dados a serem inseridos durante os testes (não necessariamente todos os dados serão inseridos). Para tanto, uma sugestão é carregar o arquivo em memória (um vetor de struct) e ir acessando cada posição conforme as inserções vão ocorrendo. Note que é possível encerrar a execução e recomeçar a execução, sendo necessário marcar, de algum modo, quantos registros já forma utilizados do mesmo. 

Em relação a (b), o arquivo conterá uma lista de “ISBNs” a serem utilizados durante a pesquisa por chave primária (opção 2). A ideia é a mesma já descrita, ou seja, carregar o arquivo em memória (um vetor de struct) e ir acessando cada posição conforme as buscas vão ocorrendo. Note que é possível encerrar a execução e recomeçar a execução, sendo necessário marcar, de algum modo, quantos registros já forma utilizados do mesmo. Em relação a (c), considere as mesmas observações em relação ao item (b); porém, neste caso, o arquivo conterá uma lista de “autores” a serem utilizados durante a pesquisa por chave secundária (opção 3). 

__Dump Arquivo (5)__

Realiza o dump do arquivo. É necessário perguntar qual dos arquivos se deseja visualizar (principal, índice primário ou índice secundário). 

#### *__Observações Gerais:__*
1. Não criar os arquivos toda vez que o programa for aberto (fazer verificação). 
2. O arquivo principal deve ser manipulado totalmente em memória secundária!