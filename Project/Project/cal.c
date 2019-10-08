#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef enum {false, true} bool;

#define PI 3.141592
#define MAX_ARRAY 200
#define MAX_NUMBER 100
#define MAX_FUNCTION 100


//����
double rad(double r)
{
	return r * PI / 180;
}

char *input_p; // ���ڿ��� ��ġ
char number_s[MAX_NUMBER]; // �о���� ��������
char function_s[MAX_FUNCTION]; // �о���� ��������
char operand;

char *function[] = { "sin", "cos", "tan", "rad" ,
					 "asin", "acos", "atan",
					 "sinh", "cosh", "tanh",
					 "exp", "log" };
// �Լ��̸�

double(*function_p[])(double) = { sin, cos, tan, rad ,
								asin, acos, atan,
								sinh, cosh, tanh,
								exp, log };
// �Լ� ������
//double function(double x)�� ���·� ���̱� ����

double cal(char *input);
double PlusMinus();
double MultiDivide();
double Check();
double(*getfunction(char *fstr))(double);
bool is_op(char c);

//�����Լ�
int main()
{
	char input[MAX_ARRAY];
	double result;
	int select;


	while (1) {
		printf("\n------------------------------------------\n");
		printf("1. ���� ����\n");
		printf("2. ��� ���� ������ ���\n");
		printf(" : ");
		scanf("%d", &select);

		switch (select) {
		case 1:
			while (1) {
				printf("(back�� �Է��ϸ� ���� �޴��� ���ư��ϴ�.)\n");
				printf("������ �Է��ϼ��� \n");
				printf(" : ");
				scanf("%s", input);

				if (input[0] == 'b' && input[1] == 'a' && input[2] == 'c' && input[3] == 'k')
					break;

				result = cal(input);
				printf("= %f\n", result);

			}
			break;

		case 2:
			printf("\n+(����)                -(����)                  *(����)                    /(������)\n");
			printf("()(��ȣ)               sin(�����Լ�)            cos(�ڻ����Լ�)            tan(ź��Ʈ�Լ�)\n");
			printf("rad(����)            asin(�������Լ�)         acos(���ڻ����Լ�)         atan(��ź��Ʈ�Լ�)\n");
			printf("sinh(�ְ�����Լ�)   cosh(�ְ�ڻ����Լ�)   tanh(�ְ��Ʈ�Լ�)\n");
			printf("exp(�����Լ�)          log(�α��Լ�)            ^(����)\n");
			printf("(back�� �Է��ϸ� ���� �޴��� ���ư��ϴ�.)\n");
			printf(" : ");
			scanf("%s", input);

			if (input[0] == 'b' && input[1] == 'a' && input[2] == 'c' && input[3] == 'k')
				break;

			break;
		default:
			printf("���Է� �ϼ���.\n");
		}
	}
	return 0;
}

// ���ڿ��� ���� ���� �Ǽ�������
double cal(char *input)
{
	input_p = input; // ����� ���ڿ��� �ʱ�ȭ
	
	//���� ������ �켱������ ���� ����
	return PlusMinus();
}

// ���� ���� 
double PlusMinus()
{
	double num1, num2;
	char op;
	num1 = MultiDivide();

	while (operand == '+' || operand == '-') {
		op = operand;
		
		//������ �������� ��������� �� �����Ƿ� �Լ������� ����,������ �Լ��� ȣ��
		num2 = MultiDivide();

		switch (op) {

		case '+':
			num1 = num1 + num2;
			break;

		case '-':
			num1 = num1 - num2;
			break;
		}
	}

	
	return num1;
}

// ���� ������ 
double MultiDivide()
{
	double num1, num2, tmp;
	char op;
	int i;

	//������� ������ check�Լ��� ���� ���� �ǿ����ڸ� ã��
	num1 = Check();

	//������ ã��
	while (operand == '*' || operand == '/' || operand == '^') {
		op = operand;

		//������� ������ check�Լ��� ���� ���� �ǿ����ڸ� ã��
		num2 = Check();

		switch (op) {

		case '*':
			num1 = num1 * num2;
			break;

		case '/':
			if (num2 == 0) {
				printf("0 ���� ���� �� �����ϴ�.\n");
			}

			num1 = num1 / num2;
			break;

		case '^':
			tmp = num1;
			for (i = 1; i < num2; i++) {
				tmp = tmp * num1;
			}
			num1 = tmp;
			break;
		}
	}

	return num1;
}

// �Ҽ���, ��ȣ, ��ȣ, �̿��� ���ڿ�
double Check()
{
	double n = 0;

	//���ڿ� �Ҽ���ã��
	if (isdigit(*input_p) || *input_p == '.') { 

		bool dot;
		char *input = number_s;

		do {
			dot = (*input_p == '.');
			*input++ = *input_p++;

		} while (*input_p && (isdigit(*input_p) || (!dot && *input_p == '.')));
		//���ڿ� �Ҽ����� �ƴҶ� ������ ���ڿ��� ã��.

		*input = '\0';
		n = atof(number_s);
		//���ڿ��� �Ǽ���
	}

	//���� ������ �ƴ� ��ȣ�μ��� ������
	else if (*input_p == '-' || *input_p == '+') {   

		bool min = (*input_p == '-');
		input_p++;

		//check�Լ��� ���ȣ���Ͽ� ��ȣ ���� ���ڸ� ã��
		n = Check();

		if (min) 
			n = -n;

		return n; // ���� �����ڴ� �о��� �����̹Ƿ� �ٷ� ����
	}

	else if (*input_p == '(') {   // ��ȣ
		input_p++;

		//��ȣ�� ���۵Ǹ� �ٽ� ���� ���� �����ں��� �����Ͽ� ��ȣ�ȿ��� ���ο� ���� ����
		n = PlusMinus();

		if (operand != ')') {
			printf("�߸� �ԷµǾ����ϴ�.(��ȣ)\n");
		}
	}

	//math�Լ��� ���� �پ��� �����ڵ� (����)
	else if (isalpha(*input_p)) {   

		char *f = function_s;
		double(*func)(double);

		do {
			*f++ = *input_p++;
		} while (isalpha(*input_p));

		*f = '\0';
		func = getfunction(function_s);

		if (func) {
			n = Check();
			n = func(n);

			return n;
		}
		else {
			printf("�߸��ԷµǾ����ϴ�.(������)\n");
		}
	}

	//������������ �����ڵ��� ���� ��� error
	if (*input_p) {
		if (is_op(*input_p)) operand = *input_p++;
		else {
			printf("�߸��� �����Դϴ�.\n");
		}
	}
	else operand = 0;   // ���ڿ� ��, ���� ������ ����

	return n;
}


//math����� ������ �پ��� �����ڵ� ã�� 
double(*getfunction(char *fstr))(double)
{
	int n = sizeof(function) / sizeof(function[0]);
	int i;

	for (i = 0; i < n; i++) {
		if (strcmpi(function[i], fstr) == 0)
			//��ҹ��� ����x

			return function_p[i];
	}

	return 0;
}


//�� �ڸ� �����ڵ�
bool is_op(char c)
{
	//���ڿ����� Ư�� ���ڸ� �˻�
	return strchr("+-*/^()", c) != 0;
}