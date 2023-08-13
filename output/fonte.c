#include<stdio.h>
typedef char literal[256];

int main(){
	/*----Variaveis temporarias----*/
	int T_0;
	int T_1;
	int T_2;
	int T_3;
	int T_4;
	/*------------------------------*/
	literal A;
	int D, B;
	double C;
	/*------------------------------*/
	printf("%s", "Digite B");
	scanf("%d", &B);
	printf("%s", "Digite A:");
	scanf("%s", A);
	T_0 = B > 2;
	if( T_0 ){
		T_1 = B <= 4;
		if( T_1 ){
			printf("%s", "B esta entre 2 e 4");
		}
	}
	T_2 = B + 1;
	B = T_2;
	T_3 = B + 2;
	B = T_3;
	T_4 = B + 3;
	B = T_4;
	D = B;
	C = 5.0;
	printf("%s", "\nB=\n");
	printf("%d", D);
	printf("%s", "\n");
	printf("%lf", C);
	printf("%s", "\n");
	printf("%s", A);

	return 0;
}