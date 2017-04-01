#include "header.h"
typedef unsigned long long int UINT64;
////подсчёт количества встречаемости каждого символа
//typedef struct treeS
//{
//	struct tree_s* parent;
//	struct tree_s* left;
//	struct tree_s* right;
//	unsigned char symbol;
//	unsigned int weight;
//} Tree;
//
//
//void tableOfFrequency(char* fileName)
//{
//	UINT64 k = 0;//счётчик различных букв
//	char curChar;//текущий прочитанный символ
//	FILE* file = NULL;
//	Tree symbols[256] = { 0 };
//	Tree *psymbols;
//	psymbols = (Tree*)malloc(256 * sizeof(Tree));
//	if ((file = fopen(fileName, "rb")) == NULL)
//		OPEN_ERR
//		////////////
//		int size = 15;
//	for (int i = 0; i < 256; i++)
//	{
//		symbols[i].symbol = (unsigned char)i;
//	}
//	for (int i = 0; i < size; i++)
//	{
//		curChar = fgetc(file);
//		for (int j = 0; j < 256; j++)
//		{
//			if (symbols[j].symbol==curChar)//если совпадают
//			{
//				symbols[j].weight++;
//				break;
//			}
//		}
//		//связь с указателями
//		for (int i = 0; i < 256; i++)
//			psymbols[i] = (*&symbols[i]);
//		//сортировка по убыванию
//		Tree temp;
//		for (int i = 1; i < 256; i++)
//			for (int j = 0; j < 256 - 1; j++)
//				if (symbols[j].weight < symbols[j + 1].weight) {
//					temp = symbols[j];
//					symbols[j] = symbols[j + 1];
//					symbols[j + 1] = temp;
//				}
//	}
//	//вывод
//	for (int j = 0; j < 256; j++)
//	{
//		printf("%c %d\n", symbols[j].symbol, symbols[j].weight);
//	}
//
//
//
//	fclose(file);
//}
//typedef struct Tree {
//	unsigned char symbol;
//	struct Tree *left;
//	struct Tree *right;
//} Tree;
//typedef struct List {
//	UINT64 count;
//	struct List *next;
//	struct Tree *tree;
//}List;

typedef struct Tree
{
	int symbol;
	UINT64 count;
	struct Tree *next, *left, *right;
} Tree;
/*подсчёт встречающихся символов*/
void createFrequencyArr(FILE *file, UINT64 *arr)
{
	UINT64 size = 15;//заменить на функцию
	for (UINT64 i = 0; i < size; i++)
	{
		arr[fgetc(file)]++;
	}
	return;
}
/*Функция записи бит в файл*/
int writeBits(FILE *out, int *position, char *buffer, char *value)
{
	int bit;
	while (*value)
	{
		bit = *value - '0';
		if (bit)
		{
			*buffer = (*buffer | 1 << (7 - *position));
		}
		*position++;
		if (*position == 8)
		{
			fwrite(buffer, sizeof(char), 1, out);
			*position = 0;
			*buffer = 0;
		}
		*value++;
	}
}
void insert(Tree **head, UINT64 count, int symbol,char mode)
{
	Tree* tmp =(Tree*)malloc(sizeof(Tree));
	Tree* cur = NULL;
	int flag = 0;
	if (mode == 0)
	{
		//если список пуст
		if (*head == NULL)
		{
			tmp->left = tmp->right = NULL;
			tmp->symbol = symbol;
			tmp->count = count;
			tmp->next = *head;
			*head = tmp;
			return;
		}
		if (count <= (*head)->count) /* вставить перед первым */
		{
			tmp->count = count;
			tmp->symbol = symbol;
			tmp->left = tmp->right = NULL;
			tmp->next = (*head);
			*head = tmp;
		}
		else
		{
			cur = (*head);
			/* ищем позицию для вставки */
			while ((flag == 0) && (cur->next != NULL))
			{
				if ((cur->count < count) && (cur->next->count >= count)) flag = 1;
				cur = cur->next;
			}
			if (flag == 0) /* позиция не найдена - вставить в конец */
			{
				tmp->count = count;
				tmp->symbol = symbol;
				tmp->left = tmp->right = NULL;
				tmp->next = NULL;
				cur->next = tmp;
			}
			else /* позиция в середине найдена */
			{
				tmp->count = count;
				tmp->symbol = symbol;
				tmp->left = tmp->right = NULL;
				tmp->next = cur->next;
				cur->next = tmp;
			}
		}
	}
	//при создании дерева
	else 
	{
		if (count <= (*head)->count) /* вставить перед первым */
		{
			tmp->count = (*head)->count + (*head)->next->count;
			tmp->symbol = NODE;
			tmp->left = *head;
			tmp->right = (*head)->next;
			tmp->next = (*head);
			*head = tmp;
		}
		else
		{
			cur = (*head);
			/* ищем позицию для вставки */
			while ((flag == 0) && (cur->next != NULL))
			{
				if ((cur->count < count) && (cur->next->count >= count)) flag = 1;
				cur = cur->next;
			}
			if (flag == 0) /* позиция не найдена - вставить в конец */
			{
				tmp->count = (*head)->count + (*head)->next->count;
				tmp->symbol = NODE;
				tmp->left = *head;
				tmp->right = (*head)->next;
				tmp->next = NULL;
				cur->next = tmp;
				(*head) = (*head)->next->next;
			}
			else /* позиция в середине найдена */
			{
				tmp->count = (*head)->count + (*head)->next->count;
				tmp->symbol = NODE;
				tmp->left = *head;
				tmp->right = (*head)->next;
				tmp->next = cur->next;
				cur->next = tmp;
				(*head) = (*head)->next->next;
			}
		}
	}
}
void makeHuffmanTree(Tree **head)
{
	UINT64 count;
	while ((*head)->next)
	{
		count = (*head)->count + (*head)->next->count;
		insert(head, count, NODE, MAKETREE);
	}
}
void buildCodeArray(Tree* root, UINT64 code_array[][2], UINT64 code, int length)
{
	if (root == NULL) return;
	if (root->left == NULL) {
		code_array[root->symbol][0] = code;
		code_array[root->symbol][1] = (length == 0 ? 1 : length);
		return;
	}
	buildCodeArray(root->left, code_array, code << 1, length + 1);
	buildCodeArray(root->right, code_array, (code << 1) | 1, length + 1);
}
/*******************************DEBUGER FUNCTIONS**********************************************************/
void printLinkedList(Tree *head) {
	if (!head) {
		printf("Spisok pust");
	}
	while (head) {
		printf("%d ", head->count);
		printf("%c\n",head->symbol);
		head = head->next;
	}
}
void printTree(Tree* root) {
	if (root == NULL)
	{
		//printf("Tree is empty");
		return;
	}
	printf("%d ", root->count);
	if (root->symbol < 0) printf("%d\n", root->symbol);
	else printf("%c\n", root->symbol);
	printTree(root->left);
	printTree(root->right);
}
void deleteNode(Tree* root) {
	if (root) {
		deleteNode(root->left);
		deleteNode(root->right);
		free(root);

	}
}
Tree* deleteTree(Tree* root) {
	deleteNode(root);
	root = NULL;
	return root;
}
void printCodeArray(const UINT64 array[][2]) // prints to "codes.txt"
{
	FILE* file = fopen("codes.txt", "w");
	for (int i = 0; i < 256; i++)
	{
		if (array[i][1] > 0) {
			fprintf(file, "%c: ", i);
			for (int j = array[i][1] - 1; j >= 0; j--)
				fprintf(file, "%lld", (array[i][0] >> j) & 1);
			fprintf(file, "\n");
		}
	}
	fclose(file);
}
/*********************************************************************************************************/
int main(int argc, char **argv)
{
	UINT64 arr[256] = { 0 };
	FILE *file = NULL;
	Tree *head = NULL;
	if ((file = fopen("test.txt", "rb")) == NULL)
		OPEN_ERR
	createFrequencyArr(file, arr);
	fclose(file);
	for (int i = 96; i < 100; i++)
	{
		if (arr[i] != 0)
		{
			insert(&head, arr[i], i,MAKELIST);
		}
	}
	printLinkedList(head);
	printf("\n");
	makeHuffmanTree(&head);
	printTree(head);
	UINT64 codeArray[256][2] = { { 0ULL } }; // [[code, code length], ...]
	buildCodeArray(head, codeArray, 1ULL, 0);
	printCodeArray(codeArray);


	

}
