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


//라디안
double rad(double r)
{
	return r * PI / 180;
}

char *input_p; // 문자열의 위치
char number_s[MAX_NUMBER]; // 읽어들인 숫자저장
char function_s[MAX_FUNCTION]; // 읽어들인 숫자저장
char operand;

char *function[] = { "sin", "cos", "tan", "rad" ,
					 "asin", "acos", "atan",
					 "sinh", "cosh", "tanh",
					 "exp", "log" };
// 함수이름

double(*function_p[])(double) = { sin, cos, tan, rad ,
								asin, acos, atan,
								sinh, cosh, tanh,
								exp, log };
// 함수 포인터
//double function(double x)의 형태로 쓰이기 때문

double cal(char *input);
double PlusMinus();
double MultiDivide();
double Check();
double(*getfunction(char *fstr))(double);
bool is_op(char c);

//메인함수
int main()
{
	char input[MAX_ARRAY];
	double result;
	int select;


	while (1) {
		printf("\n------------------------------------------\n");
		printf("1. 연산 수행\n");
		printf("2. 사용 가능 연산자 목록\n");
		printf(" : ");
		scanf("%d", &select);

		switch (select) {
		case 1:
			while (1) {
				printf("(back을 입력하면 이전 메뉴로 돌아갑니다.)\n");
				printf("수식을 입력하세요 \n");
				printf(" : ");
				scanf("%s", input);

				if (input[0] == 'b' && input[1] == 'a' && input[2] == 'c' && input[3] == 'k')
					break;

				result = cal(input);
				printf("= %f\n", result);

			}
			break;

		case 2:
			printf("\n+(덧셈)                -(뺄셈)                  *(곱셈)                    /(나눗셈)\n");
			printf("()(괄호)               sin(사인함수)            cos(코사인함수)            tan(탄젠트함수)\n");
			printf("rad(라디안)            asin(역사인함수)         acos(역코사인함수)         atan(역탄젠트함수)\n");
			printf("sinh(쌍곡선사인함수)   cosh(쌍곡선코사인함수)   tanh(쌍곡선젠트함수)\n");
			printf("exp(지수함수)          log(로그함수)            ^(제곱)\n");
			printf("(back을 입력하면 이전 메뉴로 돌아갑니다.)\n");
			printf(" : ");
			scanf("%s", input);

			if (input[0] == 'b' && input[1] == 'a' && input[2] == 'c' && input[3] == 'k')
				break;

			break;
		default:
			printf("재입력 하세요.\n");
		}
	}
	return 0;
}

// 문자열로 받은 값을 실수형으로
double cal(char *input)
{
	input_p = input; // 계산할 문자열의 초기화
	
	//덧셈 뺄셈이 우선순위가 가장 낮음
	return PlusMinus();
}

// 덧셈 뺄셈 
double PlusMinus()
{
	double num1, num2;
	char op;
	num1 = MultiDivide();

	while (operand == '+' || operand == '-') {
		op = operand;
		
		//곱셈과 나눗셈의 연산순위가 더 높으므로 함수내에서 곱셈,나눗셈 함수를 호출
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

// 곱셈 나눗셈 
double MultiDivide()
{
	double num1, num2, tmp;
	char op;
	int i;

	//연산과정 이전에 check함수를 통해 앞의 피연산자를 찾음
	num1 = Check();

	//연산자 찾기
	while (operand == '*' || operand == '/' || operand == '^') {
		op = operand;

		//연산과정 이전에 check함수를 통해 뒤의 피연산자를 찾음
		num2 = Check();

		switch (op) {

		case '*':
			num1 = num1 * num2;
			break;

		case '/':
			if (num2 == 0) {
				printf("0 으로 나눌 수 없습니다.\n");
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

// 소수점, 부호, 괄호, 이외의 문자열
double Check()
{
	double n = 0;

	//숫자와 소수점찾기
	if (isdigit(*input_p) || *input_p == '.') { 

		bool dot;
		char *input = number_s;

		do {
			dot = (*input_p == '.');
			*input++ = *input_p++;

		} while (*input_p && (isdigit(*input_p) || (!dot && *input_p == '.')));
		//숫자와 소수점이 아닐때 까지의 문자열을 찾음.

		*input = '\0';
		n = atof(number_s);
		//문자열을 실수로
	}

	//덧셈 뺄셈이 아닌 부호로서의 연산자
	else if (*input_p == '-' || *input_p == '+') {   

		bool min = (*input_p == '-');
		input_p++;

		//check함수를 재귀호출하여 부호 뒤의 숫자를 찾음
		n = Check();

		if (min) 
			n = -n;

		return n; // 다음 연산자는 읽어진 상태이므로 바로 리턴
	}

	else if (*input_p == '(') {   // 괄호
		input_p++;

		//괄호가 시작되면 다시 덧셈 뺄셈 연산자부터 시작하여 괄호안에서 새로운 연산 수행
		n = PlusMinus();

		if (operand != ')') {
			printf("잘못 입력되었습니다.(괄호)\n");
		}
	}

	//math함수를 통한 다양한 연산자들 (문자)
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
			printf("잘못입력되었습니다.(연산자)\n");
		}
	}

	//지정하지않은 연산자들이 있을 경우 error
	if (*input_p) {
		if (is_op(*input_p)) operand = *input_p++;
		else {
			printf("잘못된 수식입니다.\n");
		}
	}
	else operand = 0;   // 문자열 끝, 다음 연산자 없음

	return n;
}


//math헤더의 지정된 다양한 연산자들 찾기 
double(*getfunction(char *fstr))(double)
{
	int n = sizeof(function) / sizeof(function[0]);
	int i;

	for (i = 0; i < n; i++) {
		if (strcmpi(function[i], fstr) == 0)
			//대소문자 구별x

			return function_p[i];
	}

	return 0;
}


//한 자리 연산자들
bool is_op(char c)
{
	//문자열에서 특정 문자를 검색
	return strchr("+-*/^()", c) != 0;
}