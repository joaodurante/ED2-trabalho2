/***************************************************************************************************
* @title Trabalho Aula 6 ED2
* @version 1.0
* @author Guilherme Krambeck
* @author João Pedro Durante
 ***************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>

#define REGISTERS_FILENAME "registers.bin"
#define PRIMARY_INDEX_FILENAME "primary_indice.bin"
#define NAME_LIST_FILENAME "name_list.bin"
#define ISBN_LIST_FILENAME "isbn_list.bin"
#define BIBLIOTECA_FILENAME "resources/biblioteca.bin"
#define BUSCA_PRIMARY_FILENAME "resources/busca_p.bin"
#define BUSCA_SECONDARY_FILENAME "resources/busca_s.bin"
#define WRITE_MODE "r+b"
#define READ_MODE "rb"
#define VECTOR_SIZE 100

struct book {
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
};
typedef struct book Book;

struct primaryKey {
    char key[14]; //ISBN
    int offset;
};
typedef struct primaryKey PrimaryKey;

struct nameList {
    char key[50]; //Author
    int offset;
};
typedef struct nameList NameList;

struct isbnList {
    char isbn[14];
    int next;
};
typedef struct isbnList IsbnList;

/**
 * Responsável pela exibicao e distribuicao das operacoes
 */
void menu();

/**
 * Insercao de novos registros no arquivo, quando este deslocamento for -1 quer
 */
void insert();

/**
 * Encontra o registro atraves do arquivo de busca_p
 */
void findByISBN();

/**
 * Busca e exibe um registro do arquivo principal utilizando como indice para busca um ISBN informado
 * 
 * @param isbn ISBN do livro a ser buscado
 */
void findByISBN(const char* isbn);

/**
 * Aguarda o usuário pressionar qualquer tecla para
 * continuar a execução do programa
 */
void waitForUserResponse();

/**
 * Efetua a limpeza da tela
 */
void clearScreen();

/**
 * Efetua a abertua do arquivo no modo de abertura informado
 *
 * @param file ponteiro que receberá o arquivo aberto
 * @param filename nome do arquivo a ser aberto
 * @param openMode modo de abertura do arquivo
 * @param createIfNotExists flag para criar o arquivo caso o mesmo não esteja criado ainda
 * @return sucesso ou falha na abertura do arquivo
 */
bool openFile(FILE** file, const char* filename, const char* openMode, bool createIfNotExists);

/**
 * Incrementa o indice de registros no arquivo principal
 * @param file arquivo principal de registros
 * @para index índice atual dos registros
 */
void incrementNumberOfRegisters(FILE* file, int index);

/**
 * Funcao que pega a quantidade de registros que estão na biblioteca de registros
 * @param regLibrary ponteiro do arquivo da biblioteca de registros
 * @return inteiro com a quantidade de registros da biblioteca
 */
int getNumberOfInserts(FILE* regLibrary);

/**
 * Funcao que pega a quantidade de registros escritos no arquivo principal
 * @param file arquivo principal com os registros
 * @return inteiro com a quantidade de registros  (indice)
 */
int getIndex(FILE* file);

/**
 * Funcao que faz o dump do arquivo principal
 */
void dumpPrincipalFile();

/**
 * Função que faz a escolha do nome do arquivo para ser feito o dump
 */
void dumpFile();

/**
 * Função que faz o dump de um arquivo de indice
 *
 * @param filename nome do arquivo de indice
 * @param keyRegister struct que receberá os registros de indice durante a leitura do arquivo
 */
template <typename T>
void dumpIndexFile(const char* filename, T keyRegister);

/**
 * Buscar uma chave dentro de um arquivo usando busca sequencial
 *
 * @param key chave que será utilizada na busca
 * @param filename nome do arquivo onde será feita a busca
 * @param keyRegister struct que receberá o registro de indice encontrada, caso não seja encontrado ficará NULL
 */
template <typename T>
T* findByKey(const char* key, const char* filename, T *keyRegister);

/**
 * Funcao que insere o registro na variavel *string
 *
 * @param file arquivo principal
 * @param string arquivo que receberá o registro
 * @param bytes numero de bytes a ser lido do arquivo
 */
void selectRegister(FILE *file, char *string, int bytes);

/**
 * Retorna o tamanho do registro
 *
 * @param file arquivo principal
 * @return tamanho do registro
 */
int getRegisterSize(FILE *file);

/**
 * Printa os valores em hexadecimal
 * @param s string com valores do registro
 */
void printHex(const char* s);

/**
 * Insere o novo elemento na lista de chaves primarias
 * @param isbn isbn do novo livro
 * @param index quantidade de registros ja cadastrados no arquivo
 * @param offset posicao do novo registro no arquivo principal
 */
void insertPrimaryKey(char *isbn, int index, int offset);

/**
 * Ordena as chaves primarias
 * @param primaryKey[] vetor com todas as chaves primarias
 * @param index quantidade de registros ja cadastrados no arquivo
 */
void sortPrimaryKey(PrimaryKey primaryKey[], int index);
/**
 * Insere os dados nos arquivos de chave secundaria
 * @param isbn[] string com o isbn do novo elemento
 * @param autor[] string com o autor do novo elemento
 */
void insertSecondaryKey(char isbn[], char autor[]);

/**
 * Insere os dados no arquivo de isbn's
 * @param isbn[] string com o isbn do novo elemento
 * @param nextOffset e o offset do proximo elemento da 'lista'
 * @return retorna a posicao do elemento inserido
 */
int insertIsbnList(char isbn[], int nextOffset);

/**
 * Percorre a lista de nomes de autor para ver se ja existe o nome na lista
 * @param nameList[] vetor da lista de nomes
 * @param authorQtt quantidade de elementos na lista
 * @return retorna -1 caso nao existe ou o offset do mesmo caso exista
 */
int findAuthor(NameList nameList[], int authorQtt, char autor[]);

/**
 * Ordena a lista de nomes em ordem alfabetica
 * @param nameList[] vetor com todos os elementos da lista de nomes
 * @param authorQtt quantidade de elementos na lista
 */
void sortSecondaryKey(NameList nameList[], int authorQtt);

/**
 * Busca no arquivo principal e exibe os dados do registro contido no offset informado
 * @param offset offset do registro no arquivo principal
 */
void findByOffset(int offset);

/**
 * Encontra o registro atraves do arquivo de busca_s
 */
void findByAuthor();

/**
 * Busca e exibe um registro do arquivo principal utiizando como indice para busca o autor informado
 */
void findByAuthor(const char* autor, FILE* isbnFile, NameList* nameList, int total);
int main() {
	menu();
}

void menu(){
    int resp;
    do {
    	clearScreen();
		printf("Menu");
		printf("\n1.Insercao");
		printf("\n2.Procurar por ISBN");
		printf("\n3.Procurar por Autor");
		printf("\n4.Dump Arquivos");
		printf("\n0.Sair");
		printf("\nEscolha: ");
		scanf("%d", &resp);
		fflush(stdin);
		switch(resp){
			case 1:
				insert();
				break;
			case 2:
				findByISBN();
				break;
			case 3:
				findByAuthor();
				break;
			case 4:
				dumpFile();
				break;
			case 0:
				return;
			default: printf("\nEscolha Invalida");
      	}
	    waitForUserResponse();
    } while(resp != 0);
}

bool openFile(FILE** file, const char* filename, const char* openMode, bool createIfNotExists){
	*file = fopen(filename, openMode);

	if (*file == NULL && createIfNotExists){
		*file = fopen(filename, "w+b");
	}
	if (*file == NULL){
		printf("\nNão foi possível abrir o arquivo %s", filename);
		return false;
	}
	return true;
}

void waitForUserResponse(){
    printf("\nPressione qualquer tecla para continuar...");
    //getch();
	getchar();
	getchar();
}

void clearScreen(){
   //system("cls");
   system("clear");
}

void insert(){
    int index, length, regPosition;
    Book registry;
    char buffer[123];
	
    //Abre o arquivo como r+b para ler e escrever
    FILE *file;
    if (!openFile(&file, REGISTERS_FILENAME, WRITE_MODE, true))
		return;

	FILE* regLibrary;
	if (!openFile(&regLibrary, BIBLIOTECA_FILENAME, READ_MODE, false))
		return;

	//Pega a quantidade de registros inseridos no arquivo principal
	index = getIndex(file);

	int quantity = getNumberOfInserts(regLibrary) - 1; //Numero de registros da biblioteca -1 para pegar a posicao correta
	if(quantity >= index){
		//index * sizeof(Book) = posicao do registro que ira ser inserido
		fseek(regLibrary, index * sizeof(Book), SEEK_SET);
		fread(&registry, sizeof(Book), 1, regLibrary);
		fclose(regLibrary);

		sprintf(buffer, "%s#%s#%s#%s", registry.isbn, registry.titulo, registry.autor, registry.ano);
		length = strlen(buffer);

		//Inserindo o registro no arquivo principal
		fseek(file, 0, SEEK_END);
		fwrite(&length, sizeof(int), 1, file);
		fwrite(buffer, sizeof(char), length, file);

		//Pega a posicao do registro (ftell - (length + sizeof(int)))
		regPosition = ftell(file) - (length + sizeof(int));

		insertPrimaryKey(registry.isbn, index, regPosition);
		insertSecondaryKey(registry.isbn, registry.autor);

		//Incrementa o numero de registros no arquivo
		incrementNumberOfRegisters(file, index);
		printf("Registro inserido: %s", buffer);
	}else{
		printf("\nNão há mais registros para serem inseridos!");
	}
	fclose(file);
}

void findByISBN(){
	clearScreen();
	FILE* busca_p;
    if (!openFile(&busca_p, BUSCA_PRIMARY_FILENAME, READ_MODE, false))
		return;

	FILE* primaryIndexFile;
    if (!openFile(&primaryIndexFile, PRIMARY_INDEX_FILENAME, WRITE_MODE, true))
		return;

	int index = getIndex(primaryIndexFile);

	char isbn[14];
	int i=0;
	while(fread(&isbn, sizeof(isbn), 1, busca_p)){
		if (i == index){
			findByISBN(isbn);
			incrementNumberOfRegisters(primaryIndexFile, index);
		}
		i++;
	}
	fclose(busca_p);
	fclose(primaryIndexFile);
}

void findByISBN(const char* isbn){
	PrimaryKey* primaryKey = (PrimaryKey*) malloc(sizeof(primaryKey));
	primaryKey = findByKey(isbn, PRIMARY_INDEX_FILENAME, primaryKey);
	if (primaryKey != NULL){
		printf("\n#############################");
		printf("\nDados do registro %s encontrado:", primaryKey->key);
		findByOffset(primaryKey->offset);
	}else{
		printf("\n#############################");
		printf("\nRegistro não encontrado: %s\n\n", isbn);
	}
	
}

template <typename T>
T* findByKey(const char* key, const char* filename, T *keyRegister){
	FILE* file;
    if (!openFile(&file, filename, READ_MODE, false))
		return NULL;
	
	fseek(file, sizeof(int), SEEK_SET);

	while(fread(keyRegister, sizeof(*keyRegister), 1, file)){
		if(strcmp(key, keyRegister->key) == 0){
			return keyRegister;
		}
	}
	fclose(file);
	return NULL;
}

void findByOffset(int offset){
	FILE* file;
	if (!openFile(&file, REGISTERS_FILENAME, READ_MODE, false))
		return;

	fseek(file, offset, SEEK_SET);
	
	char string[120], *pch, regSize;
	regSize = getRegisterSize(file);
	selectRegister(file, string, regSize);
	if (regSize > 0){
		printf("\n\n");
		pch = strtok(string, "#");
		while(pch != NULL){
			printHex(pch);
			printf("\n%s\n\n", pch);
			pch = strtok(NULL, "#");
		}
		printf("\n");
	}
}

int getIndex(FILE* file){
	int index;
	rewind(file);
	if(!fread(&index, sizeof(int), 1, file)){
		rewind(file);
		index = 0;
		fwrite(&index, sizeof(int), 1, file);
	}
	return index;
}

void incrementNumberOfRegisters(FILE* file, int index){
    rewind(file);
	index++;
    fwrite(&index, sizeof(int), 1, file);
}

int getNumberOfInserts(FILE* regLibrary){
    int quantity;
    fseek(regLibrary, 0, SEEK_END);
    quantity = ftell(regLibrary) / sizeof(Book);
    return quantity;
}

void insertPrimaryKey(char *isbn, int index, int offset){
    int i;
    PrimaryKey primaryKey[VECTOR_SIZE];

    FILE *file;
	if (!openFile(&file, PRIMARY_INDEX_FILENAME, WRITE_MODE, true))
		return;

	fseek(file, sizeof(int), SEEK_SET);
    //le o arquivo para a memoria
    for(i=0; i<index; i++){
        fread(&primaryKey[i], sizeof(PrimaryKey), 1, file);
    }
    //insere os novos dados na memoria
    strcpy(primaryKey[i].key, isbn);
    primaryKey[i].offset = offset;
    index++;

    sortPrimaryKey(primaryKey, index);

    //insere as chaves ordenadas para o arquivo
	fseek(file, sizeof(int), SEEK_SET);
    for(i=0; i<index; i++){
        fwrite(&primaryKey[i], sizeof(PrimaryKey), 1, file);
    }
    fclose(file);
}

void sortPrimaryKey(PrimaryKey primaryKey[], int index){
    PrimaryKey temp;

    //bubblesort
    for(int i = 0; i<index-1; i++){
        for(int j = i+1; j<index; j++){
            if(atof(primaryKey[i].key) > atof(primaryKey[j].key)){
                temp = primaryKey[i];
                primaryKey[i] = primaryKey[j];
                primaryKey[j] = temp;
            }
        }
    }
}

void insertSecondaryKey(char isbn[], char autor[]){
    int authorPosition, i=0;
    NameList nameList[VECTOR_SIZE];
    FILE* nameFile;
	if (!openFile(&nameFile, NAME_LIST_FILENAME, WRITE_MODE, true))
		return;
    
	fseek(nameFile, sizeof(int), SEEK_SET);

    //le o arquivo para a memoria
    while(fread(&nameList[i], sizeof(NameList), 1, nameFile))
        i++;
    
    //busca se ja existe o nome do autor na lista
    authorPosition = findAuthor(nameList, i, autor);
    if(authorPosition == -1){
        //caso nao exista inserir o novo autor nas duas listas
        strcpy(nameList[i].key, autor);
        nameList[i].offset = insertIsbnList(isbn, -1);
        sortSecondaryKey(nameList, i);
        i++;
    }else{
        //caso exista inserir o autor apenas na lista de isbn e atualizar os ponteiros
        nameList[authorPosition].offset = insertIsbnList(isbn, nameList[authorPosition].offset);
    }
    //reescreve o arquivo de nomes
    fseek(nameFile, sizeof(int), SEEK_SET);
    for(int j=0; j<i; j++){
        fwrite(&nameList[j], sizeof(NameList), 1, nameFile);
    }
    fclose(nameFile);
}

//Insere no final novo registro e retorna a posicao do mesmo
int insertIsbnList(char isbn[], int nextOffset){
    int position;
    IsbnList temp;
    FILE *isbnFile;
    if(!openFile(&isbnFile, ISBN_LIST_FILENAME, WRITE_MODE, true))
		return -1;
    
    strcpy(temp.isbn, isbn);
    temp.next = nextOffset; //Insere -1 como proximo elemento da lista caso seja o primeiro ou o offset do proximo
    
    fseek(isbnFile, 0, SEEK_END);
    position = ftell(isbnFile);
   	fwrite(&temp, sizeof(IsbnList), 1, isbnFile); //Escreve no final do arquivo
   	fclose(isbnFile);
    return position; //Retorna posicao do novo registro
}

void sortSecondaryKey(NameList nameList[], int authorQtt){
    NameList temp;
    for(int i=0; i<authorQtt-1; i++){
        for(int j=i+1; j<authorQtt; j++){
            if(strcmp(nameList[i].key, nameList[j].key) > 0){
                temp = nameList[i];
                nameList[i] = nameList[j];
                nameList[j] = temp;
            }
        }
    }
}

//Retorna a posicao do autor existente no vetor da memoria ou -1 caso nao exista
int findAuthor(NameList nameList[], int authorQtt, char autor[]){
    for(int i=0; i<authorQtt; i++){
        if(!strcmp(nameList[i].key, autor))
            return i;
    }
    return -1;
}

void findByAuthor(){
	char autor[50], isbn[14];
	int i=0, j=0, compare;
	NameList nameList[VECTOR_SIZE];
	FILE *searchFile, *isbnFile, *nameFile;
	
	
	if (!openFile(&searchFile, BUSCA_SECONDARY_FILENAME, READ_MODE, false))
		return;
	if (!openFile(&isbnFile, ISBN_LIST_FILENAME, READ_MODE, false))
		return;
	if (!openFile(&nameFile, NAME_LIST_FILENAME, WRITE_MODE, true))
		return;
        
	int index = getIndex(nameFile);
	
	//Carregando o arquivo para a memoria
	while(fread(&nameList[i], sizeof(NameList), 1, nameFile))
		i++;
		

	//Seleciona um autor do arquivo de busca e busca para ver se existe
	while(fread(autor, sizeof(char), 50, searchFile)){
		if(j == index){
			findByAuthor(autor, isbnFile, nameList, i);
			incrementNumberOfRegisters(nameFile, index);
		}
		j++;
	}
	fclose(isbnFile);
	fclose(nameFile);
	fclose(searchFile);
}

void findByAuthor(const char* author, FILE* isbnFile, NameList* nameList, int total){
	IsbnList isbnAux;
	int j=0, compare=1;
	clearScreen();
	printf("BUSCA PELO AUTOR: %s\n\n", author);
	while(j < total && compare != 0){
		compare = strcmp(nameList[j].key, author);
		if(compare == 0){
			isbnAux.next = nameList[j].offset; //Pega a posicao para dar fseek para o primeiro elemento da lista
			do{
				fseek(isbnFile, isbnAux.next, SEEK_SET); //Aponta para o primeiro elemento da lista do autor
				fread(&isbnAux, sizeof(IsbnList), 1, isbnFile); // Recupera o isbn
				findByISBN(isbnAux.isbn); //printa
				printf("\n\n");
			}while(isbnAux.next != -1);
		}
		j++;
	}
}

void dumpFile(){
	clearScreen();
	int resp;
	printf("Escolha o arquivo para ser feito o dump:");
	printf("\n1.Arquivo Principal");
	printf("\n2.Indice Primario");
	printf("\n3.Indice Secundario");
	printf("\nEscolha: ");
	scanf("%d", &resp);
	fflush(stdin);
	switch(resp){
		case 1:
			dumpPrincipalFile();
			break;
		case 2:
			PrimaryKey primaryKey;
			dumpIndexFile(PRIMARY_INDEX_FILENAME, primaryKey);
			break;
		case 3:
			NameList nameList;
			dumpIndexFile(NAME_LIST_FILENAME, nameList);
			break;
		//case 4:
		//	filename = INVERTED_LIST_FILENAME;
		//	break;
		default:
			printf("\nEscolha Invalida");
			return;
	}
}

void dumpPrincipalFile(){
	FILE* file;
	if (!openFile(&file, REGISTERS_FILENAME, READ_MODE, false))
		return;
	
  	clearScreen();
  	printf("REGISTROS:\n\n");
    char string[120], *pch, regSize;

    //fseek para pular o indice de registros
    fseek(file, sizeof(int), SEEK_SET);

    regSize = getRegisterSize(file);
    selectRegister(file, string, regSize);
    int i =0;
    while(regSize > 0){
        i++;
		printf("### %do registro ###\n", i);
		pch = strtok(string, "#");
		while(pch != NULL){
			printHex(pch);
			printf("\n%s\n\n", pch);
			pch = strtok(NULL, "#");
		}
		printf("\n");
        regSize = getRegisterSize(file);
        selectRegister(file, string, regSize);
    }
	fclose(file);
}

template <typename T>
void dumpIndexFile(const char* filename, T keyRegister){
	FILE* file;
	if (!openFile(&file, filename, READ_MODE, false))
		return;

	fseek(file, sizeof(int), SEEK_SET);
	
  	clearScreen();
  	printf("INDÍCES:\n");

	while(fread(&keyRegister, sizeof(keyRegister), 1, file)){
		printf("\nOffset: %d\n", keyRegister.offset);
		printHex(keyRegister.key);
		printf("\n%s\n", keyRegister.key);
	}
	fclose(file);
}

void printHex(const char* s){
    int i = 0;
    while(s[i] != '\0'){
        printf("%x ", s[i]);
        i++;
    }
}

void selectRegister(FILE *file, char *string, int bytes){
    if(bytes > 0){
    	fread(string, bytes, 1, file);
    	string[bytes] = '\0';
	}
}

int getRegisterSize(FILE *file){
  	int bytes;
  	if(fread(&bytes, sizeof(int), 1, file))
  		return bytes;
    else
    	return 0;
}
