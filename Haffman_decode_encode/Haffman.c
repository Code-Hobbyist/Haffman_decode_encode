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
	用链表的方式创建哈夫曼树

==================================
	定义哈夫曼树的节点信息 ：
		1.权值
		2.字符
		3.位串(存储对应字符的哈夫曼编码)
		4.定义左右孩子指针
*/
typedef struct Tree_Node
{
	double weight;	//权值
	char ch;	//字符
	char bit[16];	//位串
	struct Tree_Node *next, *lChild, *rChild;	//指向下一个节点的指针，左右孩子指针
}Stack_list;
/*
	创建栈，遍历哈夫曼树查找编码时，临时存放对应字符的哈夫曼编码
*/
typedef struct Code_Stack {
	char code[MAXSIZE];		//存放临时对应字符的哈夫曼编码
	int Stack_top;			//指向栈顶的元素
}Stack_Code;

//初始化栈
void init(Stack_Code* Stack)
{
	Stack->Stack_top = 0;
}
/*
	判断栈是否为空 ：
		Stack->Stack_top为空时，返回1
		Stack->Stack_top不为空时，返回0
*/
int isEmpty(Stack_Code* Stack)
{
	return (Stack->Stack_top ? 0 : 1);
}
//入栈
void Stack_Push(Stack_Code* Stack, char data)
{
	if (Stack->Stack_top == MAXSIZE)
	{
		printf("The stack is full!\n");
	}
	Stack->code[Stack->Stack_top] = data;
	Stack->Stack_top++;
}
//	出栈
void Stack_Pop(Stack_Code* Stack)
{

	if (isEmpty(Stack))
	{
		printf("This stack is empty!\n");
	}
	Stack->code[Stack->Stack_top] = '\0';
	Stack->Stack_top--;
}

//哈夫曼编码函数
void Haffuman_encode(Stack_list* root, Stack_Code* Stack)
{
	if (root == NULL)
	{
		return;
	}
	/*
		当检索到的节点左右孩子为NULL是，说明它是叶子节点
	*/
	if (root->lChild == NULL && root->rChild == NULL)
	{
		int i = 0;
		/*
			为了方便拷贝操作，将栈底定义在数组下标为0的位置，
			这样可以直接将栈中的所有元素拷贝到对应叶子节点的bit数组中
		*/
		strcpy_s(root->bit, sizeof(root->bit) / sizeof(root->bit[0]), Stack->code);

	}

	/*
		哈夫曼编码定义为：左0，右1
		如果左孩子不为空，将'0'字符入栈，继续递归。
		如果右孩子不为空，将'1'字符入栈，继续递归。
		当遍历到对应的叶子节点时，左右孩子为空，当前节点出栈，返回上次的节点，继续递归遍历。
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
	哈夫曼解码：
		将传进来的str('0','1')字符串，挨个遍历，查找对应的字符(按照左0，右1的方式遍历)，
		每次循环用Temp_root指向根节点，保证可以按照编码遍历到对于的字符。
		当左右孩子为NULL，说明已经找到了目标字符所在的叶子节点。
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
	创建有序链表(从小到大)：
		当链表为空，插进来的节点即为头结点
		当链表不为空，定义pCurrent，pPrevious两个指针，以便找到合适的插入位置
		找到位置后修改指针即可
*/
Stack_list* Insert_list(Stack_list* root, Stack_list* insert_node)
{
	Stack_list* pCurrent;		//指向当前节点的指针
	Stack_list* pPrevious;		//指向当前节点的前驱节点的指针
	memset(insert_node->bit, '\0', sizeof(insert_node->bit));	//格式化bit数组空间
	if (root == NULL)
	{
		root = insert_node;
	}
	else
	{
		pCurrent = root;		//当前节点指向头结点
		pPrevious = NULL;
		while (pCurrent && pCurrent->weight < insert_node->weight)	//找到对应的插入位置
		{
			pPrevious = pCurrent;
			pCurrent = pCurrent->next;
		}
		insert_node->next = pCurrent;
		if (pPrevious == NULL)		//前驱节点为NULL说明，其本身就是头结点
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
	创建哈夫曼树：
		规定：较小的书放左节点，较大的数放右节点
		根据创建哈夫曼的规则建立哈夫曼树
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
		root = Insert_list(root, newNode);		//将新的节点插入有序链表
	}
	return root;
}

//中序遍历打印哈夫曼树
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
//打印哈夫曼树叶子节点的权值
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
	int num[27] = { 0 };	//存放字符出现的次数
	char ahp[27] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ' };
	errno_t err_read;
	errno_t err_write;
	errno_t err_write_read;
	FILE *fp_read;
	FILE *fp_write;
	FILE *fp_write_read;
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_result.txt", "w+")) != 0)
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
	if ((err_write_read = fopen_s(&fp_write_read, "G:\\信息论\\text1_result.txt", "r+")) != 0)
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


	//计算每个字符出现的概率
	double probability_arr[27] = { 0.0 };
	for (int i = 0; i < sizeof(num) / sizeof(num[0]); i++)
	{
		probability_arr[i] = num[i] / (double)sum;
	}
	/*for (int i = 0; i < sizeof(probability_arr) / sizeof(probability_arr[0]); i++)
	{
		printf("%f\n", probability_arr[i]);
	}*/
	//计算信源熵
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

	//读取文件进行编码
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1_result.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_encode.txt", "w+")) != 0)
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

	//读取文件，解码
	int ch;
	Stack_list* temp_pCurrent;
	Stack_list* temp_root;
	temp_root = root;
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1_encode.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_decode.txt", "w+")) != 0)
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
	printf("信源字符   权值       码字\n");
	Print_HT_Tree(root);
	printf("哈夫曼编码信源熵：%f\n", source_entropy);
	//计算编码效率
	double efficiency = 0; //初始化编码效率
	efficiency = source_entropy / aver_code_length;
	printf("哈夫曼编码效率  ：%0.3f%%\n", efficiency * 100);
	//printf("%f\n", aver_code_length);
	//Haffuman_decode(root, Haffuman_code);
	//printf("\n%s\n", Haffuman_code);
	system("pause");


}