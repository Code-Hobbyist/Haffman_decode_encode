#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXSIZE 27
int sum = 0;
double aver_code_length = 0;
int index = 0;
char Haffuman_code[16] = { '\0' };
int Haffuman_ch = 0;
/*
	������ķ�ʽ������������

==================================
	������������Ľڵ���Ϣ ��
		1.Ȩֵ
		2.�ַ�
		3.λ��(�洢��Ӧ�ַ��Ĺ���������)
		4.�������Һ���ָ��
*/
typedef struct Tree_Node
{
	double weight;	//Ȩֵ
	char ch;	//�ַ�
	char bit[16];	//λ��
	struct Tree_Node *next, *lChild, *rChild;	//ָ����һ���ڵ��ָ�룬���Һ���ָ��
}Stack_list;
/*
	����ջ�����������������ұ���ʱ����ʱ��Ŷ�Ӧ�ַ��Ĺ���������
*/
typedef struct Code_Stack {
	char code[MAXSIZE];		//�����ʱ��Ӧ�ַ��Ĺ���������
	int Stack_top;			//ָ��ջ����Ԫ��
}Stack_Code;

//��ʼ��ջ
void init(Stack_Code* Stack)
{
	Stack->Stack_top = 0;
}
/*
	�ж�ջ�Ƿ�Ϊ�� ��
		Stack->Stack_topΪ��ʱ������1
		Stack->Stack_top��Ϊ��ʱ������0
*/
int isEmpty(Stack_Code* Stack)
{
	return (Stack->Stack_top ? 0 : 1);
}
//��ջ
void Stack_Push(Stack_Code* Stack, char data)
{
	if (Stack->Stack_top == MAXSIZE)
	{
		printf("The stack is full!\n");
	}
	Stack->code[Stack->Stack_top] = data;
	Stack->Stack_top++;
}
//	��ջ
void Stack_Pop(Stack_Code* Stack)
{

	if (isEmpty(Stack))
	{
		printf("This stack is empty!\n");
	}
	Stack->code[Stack->Stack_top] = '\0';
	Stack->Stack_top--;
}

//���������뺯��
void Haffuman_encode(Stack_list* root, Stack_Code* Stack)
{
	if (root == NULL)
	{
		return;
	}
	/*
		���������Ľڵ����Һ���ΪNULL�ǣ�˵������Ҷ�ӽڵ�
	*/
	if (root->lChild == NULL && root->rChild == NULL)
	{
		int i = 0;
		/*
			Ϊ�˷��㿽����������ջ�׶����������±�Ϊ0��λ�ã�
			��������ֱ�ӽ�ջ�е�����Ԫ�ؿ�������ӦҶ�ӽڵ��bit������
		*/
		strcpy_s(root->bit, sizeof(root->bit) / sizeof(root->bit[0]), Stack->code);

	}

	/*
		���������붨��Ϊ����0����1
		������Ӳ�Ϊ�գ���'0'�ַ���ջ�������ݹ顣
		����Һ��Ӳ�Ϊ�գ���'1'�ַ���ջ�������ݹ顣
		����������Ӧ��Ҷ�ӽڵ�ʱ�����Һ���Ϊ�գ���ǰ�ڵ��ջ�������ϴεĽڵ㣬�����ݹ������
	*/
	if (root->lChild != NULL)
	{
		Stack_Push(Stack, '0');
		Haffuman_encode(root->lChild, Stack);
	}
	if (root->rChild != NULL)
	{
		Stack_Push(Stack, '1');
		Haffuman_encode(root->rChild, Stack);
	}
	Stack_Pop(Stack);
}
/*
	���������룺
		����������str('0','1')�ַ������������������Ҷ�Ӧ���ַ�(������0����1�ķ�ʽ����)��
		ÿ��ѭ����Temp_rootָ����ڵ㣬��֤���԰��ձ�����������ڵ��ַ���
		�����Һ���ΪNULL��˵���Ѿ��ҵ���Ŀ���ַ����ڵ�Ҷ�ӽڵ㡣
*/

Stack_list* Haffuman_decode(Stack_list* pCurrent, int str)
{

	if (pCurrent->lChild != NULL && pCurrent->rChild != NULL)
	{
		if (str == 48)
		{
			pCurrent = pCurrent->lChild;
		}
		else
		{
			pCurrent = pCurrent->rChild;
		}
	}
	if (pCurrent->lChild == NULL && pCurrent->rChild == NULL)
	{
		Haffuman_ch = pCurrent->ch;
	}
	return pCurrent;
}

/*
	������������(��С����)��
		������Ϊ�գ�������Ľڵ㼴Ϊͷ���
		������Ϊ�գ�����pCurrent��pPrevious����ָ�룬�Ա��ҵ����ʵĲ���λ��
		�ҵ�λ�ú��޸�ָ�뼴��
*/
Stack_list* Insert_list(Stack_list* root, Stack_list* insert_node)
{
	Stack_list* pCurrent;		//ָ��ǰ�ڵ��ָ��
	Stack_list* pPrevious;		//ָ��ǰ�ڵ��ǰ���ڵ��ָ��
	memset(insert_node->bit, '\0', sizeof(insert_node->bit));	//��ʽ��bit����ռ�
	if (root == NULL)
	{
		root = insert_node;
	}
	else
	{
		pCurrent = root;		//��ǰ�ڵ�ָ��ͷ���
		pPrevious = NULL;
		while (pCurrent && pCurrent->weight < insert_node->weight)	//�ҵ���Ӧ�Ĳ���λ��
		{
			pPrevious = pCurrent;
			pCurrent = pCurrent->next;
		}
		insert_node->next = pCurrent;
		if (pPrevious == NULL)		//ǰ���ڵ�ΪNULL˵�����䱾�����ͷ���
		{
			root = insert_node;
		}
		else
		{
			pPrevious->next = insert_node;
		}
	}

	return root;
}

/*
	��������������
		�涨����С�������ڵ㣬�ϴ�������ҽڵ�
		���ݴ����������Ĺ�������������
*/
Stack_list* Create_Huffman_Tree(Stack_list* root)
{
	Stack_list* L_Child;
	Stack_list* R_Child;
	//printf("Parent	lChild	rChild\n");
	while (root && root->next)
	{
		Stack_list* newNode;
		L_Child = root;
		//printf("%d", L_Child->date);
		R_Child = root->next;
		//printf("%d", R_Child->date);
		root = R_Child->next;

		newNode = (Stack_list*)malloc(sizeof(Stack_list));
		newNode->weight = L_Child->weight + R_Child->weight;
		//printf("%4d,%6d,%6d\n",newNode->date,L_Child->date, R_Child->date);
		newNode->lChild = L_Child;
		newNode->rChild = R_Child;
		newNode->next = NULL;
		L_Child->next = NULL;
		//L_Child->lChild = L_Child->rChild = NULL;
		R_Child->next = NULL;
		//R_Child->lChild = R_Child->rChild = NULL;
		root = Insert_list(root, newNode);		//���µĽڵ������������
	}
	return root;
}

//���������ӡ��������
void Print_HT_Tree(Stack_list* root)
{
	//printf("%d", root->date);
	if (root == NULL)
	{
		return;
	}

	Print_HT_Tree(root->lChild);
	if (root->lChild == NULL || root->rChild == NULL)
	{
		aver_code_length += root->weight * strlen(root->bit);
		printf("  %c       %f   %s\n", root->ch, root->weight, root->bit);
	}
	Print_HT_Tree(root->rChild);

}
//��ӡ��������Ҷ�ӽڵ��Ȩֵ
void Print(Stack_list* root)
{
	Stack_list* p = root;

	if (!p)
	{
		return;
	}
	else {
		while (p)
		{
			printf("%f", p->weight);
			p = p->next;
		}

	}
}
void encode_tofile(Stack_list* root, char str)
{

	if (root == NULL)
	{
		return;
	}
	encode_tofile(root->lChild, str);
	if (root->lChild == NULL && root->rChild == NULL)
	{
		if (str == root->ch)
		{
			strcpy_s(Haffuman_code, sizeof(Haffuman_code) / sizeof(Haffuman_code[0]), root->bit);
		}
	}
	encode_tofile(root->rChild, str);
}
int main()
{
	int num[27] = { 0 };	//����ַ����ֵĴ���
	char ahp[27] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ' };
	errno_t err_read;
	errno_t err_write;
	errno_t err_write_read;
	FILE *fp_read;
	FILE *fp_write;
	FILE *fp_write_read;
	if ((err_read = fopen_s(&fp_read, "G:\\��Ϣ��\\text1.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\��Ϣ��\\text1_result.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	char buf[1024] = { '\0' };
	char temp_buf[1024] = { '\0' };
	int fp_index = 0;
	while (fgets(buf, (1024), fp_read) != NULL)
	{
		for (int i = 0; i < sizeof(buf) / sizeof(buf[0]); i++)
		{
			if (buf[i] >= 'A' && buf[i] <= 'Z')
			{
				temp_buf[fp_index++] = buf[i] + 32;
			}
			if ((buf[i] >= 'a' && buf[i] <= 'z'))
			{
				temp_buf[fp_index++] = buf[i];
			}
			if (buf[i] == ' ')
			{
				temp_buf[fp_index++] = buf[i];
			}
		}
		fputs(temp_buf, fp_write);
		fp_index = 0;
	}
	fclose(fp_read);
	fclose(fp_write);
	if ((err_write_read = fopen_s(&fp_write_read, "G:\\��Ϣ��\\text1_result.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	memset(buf, '\0', sizeof(buf));
	while (fgets(buf, (1024), fp_write_read) != NULL)
	{
		for (int i = 0; i < 27; i++)
		{
			for (int j = 0; j < (sizeof(buf) / sizeof(buf[0])); j++)
			{
				if (buf[j] == ahp[i])
				{
					num[i]++;
					sum++;
				}
			}
		}
		memset(buf, '\0', sizeof(buf));
	}
	/*for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
	{
		printf("%d\n", num[i]);
	}
	printf("%d\n", sum);*/
	fclose(fp_write_read);


	//����ÿ���ַ����ֵĸ���
	double probability_arr[27] = { 0.0 };
	for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
	{
		probability_arr[i] = num[i] / (double)sum;
	}
	/*for (int i = 0; i < sizeof(probability_arr) / sizeof(probability_arr[0]); i++)
	{
		printf("%f\n", probability_arr[i]);
	}*/
	//������Դ��
	double source_entropy = 0;
	for (int i = 0; i < sizeof(probability_arr) / sizeof(probability_arr[0]); i++)
	{
		source_entropy += (probability_arr[i] * (-log10(probability_arr[i]) / log10(2)));
	}
	Stack_Code* Stack = (Stack_Code*)malloc(sizeof(Stack_Code));
	memset(Stack->code, '\0', sizeof(Stack->code));
	init(Stack);
	int result[16] = { 0 };
	//int arr_weight[] = { 9,1,3,5,7,0 };
	//char arr_ch[] = { 'A','B','C','D','E','F' };
	Stack_list* root = NULL;
	Stack_list* result_root = NULL;
	for (int i = 0; i < sizeof(ahp) / sizeof(ahp[0]); i++)
	{
		Stack_list* insert_node = (Stack_list*)malloc(sizeof(Stack_list));
		insert_node->ch = ahp[i];
		insert_node->weight = probability_arr[i];
		insert_node->lChild = NULL;
		insert_node->next = NULL;
		insert_node->rChild = NULL;
		Stack_list* result_root = Insert_list(root, insert_node);
		root = result_root;
	}
	root = Create_Huffman_Tree(root);
	Haffuman_encode(root, Stack);

	//��ȡ�ļ����б���
	if ((err_read = fopen_s(&fp_read, "G:\\��Ϣ��\\text1_result.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\��Ϣ��\\text1_encode.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	while (fgets(buf, 1024, fp_read) != NULL)
	{
		for (int i = 0; i < sizeof(buf) / sizeof(buf[0]); i++)
		{
			encode_tofile(root, buf[i]);
			fputs(Haffuman_code, fp_write);
			memset(Haffuman_code, '\0', sizeof(Haffuman_code));
		}
		memset(buf, '\0', sizeof(buf));
	}
	fclose(fp_read);
	fclose(fp_write);

	//��ȡ�ļ�������
	int ch;
	Stack_list* temp_pCurrent;
	Stack_list* temp_root;
	temp_root = root;
	if ((err_read = fopen_s(&fp_read, "G:\\��Ϣ��\\text1_encode.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\��Ϣ��\\text1_decode.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	while ((ch = fgetc(fp_read)) != EOF)
	{
		temp_pCurrent = Haffuman_decode(temp_root, ch);
		temp_root = temp_pCurrent;
		if (temp_pCurrent->lChild == NULL && temp_pCurrent->rChild == NULL)
		{
			fputc(Haffuman_ch, fp_write);
			//memset(Haffuman_ch, '\0', sizeof(Haffuman_ch));
			temp_root = root;
		}

	}
	fclose(fp_read);
	fclose(fp_write);
	printf("��Դ�ַ�   Ȩֵ       ����\n");
	Print_HT_Tree(root);
	printf("������������Դ�أ�%f\n", source_entropy);
	//�������Ч��
	double efficiency = 0; //��ʼ������Ч��
	efficiency = source_entropy / aver_code_length;
	printf("����������Ч��  ��%0.3f%%\n", efficiency * 100);
	//printf("%f\n", aver_code_length);
	//Haffuman_decode(root, Haffuman_code);
	//printf("\n%s\n", Haffuman_code);
	system("pause");


}