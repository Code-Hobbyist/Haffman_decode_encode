#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXSIZE 16
#define LENGTH 27
typedef struct Shannon_node		//存储概率，码字，码长，信源符号，概率和
{
	double sum_probability;
	double probability;
	int code_length;
	char ch;
	char bit[MAXSIZE];
}Shannon_node;
int Shannon_ch;
double Pro_sum = 0;		//概率累加和
int sum = 0;
char Shannon_code[16] = { '\0' };		//存储香农编码的码字
char flag = 0;
double aver_code_length = 0;
void encode_tofile(Shannon_node bit_code[], char str)		//对文献进行编码
{
	for (int i = 0; i < LENGTH; i++)
	{
		if (bit_code[i].ch == str)
		{
			strcpy_s(Shannon_code, sizeof(Shannon_code) / sizeof(Shannon_code[0]), bit_code[i].bit);
			flag = bit_code[i].code_length;
		}
	}
}
void sort(Shannon_node bit_code[])		//从达到消进行排序
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH - i; j++)
		{
			if (bit_code[j].probability < bit_code[j + 1].probability)
			{
				Shannon_node temp = bit_code[j + 1];
				bit_code[j + 1] = bit_code[j];
				bit_code[j] = temp;
			}
		}
	}
}

int main()
{
	printf("=================香农编码====================\n");
	//初始化结构体数组
	Shannon_node bit_code[LENGTH];
	for (int i = 0; i < LENGTH; i++)
	{
		memset(bit_code[i].bit, '\0', sizeof(bit_code[i].bit));
		bit_code[i].code_length = 0;
		bit_code[i].probability = 0.0;
		bit_code[i].sum_probability = 0.0;
		bit_code[i].ch = 'A';
	}
	int code_length[27] = { 0 };	//存储码长
	int num[27] = { 0 };	//存放字符出现的次数
	char ahp[27] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ' };
	errno_t err_read;
	errno_t err_write;
	errno_t err_write_read;
	FILE *fp_read;			//读文件指针
	FILE *fp_write;			//写文件指针
	FILE *fp_write_read;
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1.txt", "r+")) != 0)		//如果打开失败，打印失败信息
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_result.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	char buf[1024] = { '\0' };		//定义数据缓冲区
	char temp_buf[1024] = { '\0' };		//定义临时存储数据缓冲区
	int fp_index = 0;
	while (fgets(buf, (1024), fp_read) != NULL)		//一次读取1024长度的数据
	{
		//对数据进行数据处理
		for (int i = 0; i < sizeof(buf) / sizeof(buf[0]); i++)
		{
			//将大写改成小写
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
		//将temp_buf数据写入文件
		fputs(temp_buf, fp_write);
		fp_index = 0;
	}
	fclose(fp_read);
	fclose(fp_write);
	//预处理后进行概率统计
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
	fclose(fp_write_read);


	//计算每个字符出现的概率
	double probability_arr[27] = { 0.0 };
	for (int i = 0; i < LENGTH; i++)
	{
		probability_arr[i] = num[i] / (double)sum;
		bit_code[i].probability = probability_arr[i];
		bit_code[i].ch = ahp[i];
	}
	//计算码长
	for (int i = 0; i < sizeof(ahp) / sizeof(ahp[0]); i++)
	{
		code_length[i] = (int)((-log10(probability_arr[i]) / log10(2)) + 1);
		bit_code[i].code_length = code_length[i];
	}
	sort(bit_code);


	//计算累加概率
	//double sum_pro[27] = { 0.0 };
	for (int i = 0; i < LENGTH - 1; i++)
	{
		Pro_sum += bit_code[i].probability;
		bit_code[i + 1].sum_probability = Pro_sum;
	}
	double result_pro;
	for (int i = 0; i < LENGTH; i++)
	{
		result_pro = bit_code[i].sum_probability;
		for (int j = 0; j < bit_code[i].code_length; j++)
		{

			int temp = (int)(result_pro * 2);
			bit_code[i].bit[j] = temp + 48;
			result_pro = (result_pro * 2) - (int)(result_pro * 2);

		}
	}


	//对文件进行编码
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1_result.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_encode.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	memset(buf, '\0', sizeof(buf));
	while (fgets(buf, 1024, fp_read) != NULL)
	{
		for (int i = 0; i < sizeof(buf) / sizeof(buf[0]); i++)
		{
			encode_tofile(bit_code, buf[i]);
			fputs(Shannon_code, fp_write);
			//fputc(flag, fp_write);
			memset(Shannon_code, '\0', sizeof(Shannon_code));

		}
		memset(buf, '\0', sizeof(buf));
	}
	fclose(fp_read);
	fclose(fp_write);

	//读取文件解码
	if ((err_read = fopen_s(&fp_read, "G:\\信息论\\text1_encode.txt", "r+")) != 0)
	{
		printf("error\n");
	}
	if ((err_write = fopen_s(&fp_write, "G:\\信息论\\text1_decode.txt", "w+")) != 0)
	{
		printf("error\n");
	}
	int i = 0, j = 0, k = 0, flag_count = 0;
	int count = 0;
	char buffer[80000] = { '\0' };
	while (fgets(buffer, 80000, fp_read) != NULL)
	{
		i = 0;
		while (buffer[i] != '\0')
		{

			if (bit_code[k].bit[j] == '\0')
			{
				fputc(bit_code[k].ch, fp_write);
				j = 0;
				k++;
				if (k > LENGTH - 1)
				{
					k = 0;
				}
			}
			if (buffer[i] == bit_code[k].bit[j])
			{
				if (buffer[i + 1] == '\0')
				{
					j++;
					break;
				}

				i++;
				j++;
			}
			else
			{


				i = i - j;
				k++;
				if (k > LENGTH - 1)
				{
					k = 0;
				}
				j = 0;
			}
		}
		count++;
		memset(buf, '\0', sizeof(buf));
	}
	fclose(fp_read);
	fclose(fp_write);
	//计算信源熵
	double source_entropy = 0;
	for (int i = 0; i < sizeof(probability_arr) / sizeof(probability_arr[0]); i++)
	{
		source_entropy += (probability_arr[i] * (-log10(probability_arr[i]) / log10(2)));
	}
	//平均码长
	for (int i = 0; i < sizeof(probability_arr) / sizeof(probability_arr[0]); i++)
	{
		aver_code_length += (code_length[i] * probability_arr[i]);
	}
	//编码效率
	double efficiency = 0; //初始化编码效率
	efficiency = source_entropy / aver_code_length;
	//打印信息
	for (int i = 0; i < LENGTH; i++)
	{
		printf("%c,%f,%d,%f,%s\n", bit_code[i].ch, bit_code[i].probability, bit_code[i].code_length, bit_code[i].sum_probability, bit_code[i].bit);
	}
	printf("香农编码信源熵  ：%f\n", source_entropy);
	printf("香农编码编码效率：%0.3f%%\n", efficiency * 100);
	//printf("%s", Shannon_code);
	system("pause");
}