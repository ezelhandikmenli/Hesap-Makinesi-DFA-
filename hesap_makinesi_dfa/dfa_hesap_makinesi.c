
//Implementation of a Lexical Analyser for Simple Mathematical Expressions.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define nStates 7
#define nSymbols 6

#define nMaxTokens 20
#define nLenghtNameTokens 20


void deleteBlankSpaces(char* str);
int isItNumber(char n);
int isItOperador(char n);
void DFARecognizer(char* exp, char tokens[][nLenghtNameTokens], int *numTokens);


int main(void)
{
	FILE* input;
	FILE* output;
	char buff[35];
	char tokens[nMaxTokens][nLenghtNameTokens];
	int numTokens = -1;
	int i = 0;


	//// Okunacak bir dosya a�ar (input)..
	if ((input = fopen("input.txt", "r")) == NULL)
		printf("It was not possible to open the file to read!");
		
	//Yaz�lacak bir dosya a�ar (output).
	if ((output = fopen("output.txt", "w+")) == NULL)
			printf("It was not possible to open the file to write!");	
			

	//Girdi dosyas�ndan sat�r sat�r okur.
	while (fgets(buff, sizeof(buff), input))
	{		
		numTokens = -1;
		
		printf("\n------------------------------------------------------\n");
		printf("G�R�LEN �FADELER: %s", buff); //Orjinal giri�i ekrana yazd�r�r.
		fprintf(output, "\n------------------------------------------------------\n"); 
		fprintf(output, "G�R�LEN �FADELER: %s\n", buff);  //prints the original input to the output file.		
		
		deleteBlankSpaces(buff);
		
		fprintf(output, "\nTANIMLANAN �FADELER: \n");
		printf("\nTan�mlanan �FADELER: \n");
		
		
		DFARecognizer(buff, tokens, &numTokens);
		
		for (i = 0; i <= numTokens; i++)
		{
			printf("%s  ", tokens[i]);
			fprintf(output, "%s  ", tokens[i]);
		}
	}


	fclose(output);
	fclose(input);
	
	return 0;
}




//Bo� alanlar� bir dizeden siler.
void deleteBlankSpaces(char* str)
{
	const char* d = str;
	int i = 0;

	do
	{
		while (*d == ' ')
		{
			++d;
			i++;
		}
	} while (*str++ = *d++);
}



//Ba��ms�z de�i�ken bir say� ise '1' de�erini d�nd�r�r.
int isItNumber(char n) 
{

	if( n >= '0' && n <= '9' )
	{
		return 1;	
	}

	return 0;
}



//Ba��ms�z de�i�ken bir i�le� ise '1' de�erini d�nd�r�r.
int isItOperator(char n) 
{
	if ((n == '+') || (n == '-') || (n == '*')
		|| (n == '/'))
	{
		return 1;		
	}

	return 0;
}



//TAMSAYI(�nteger) m� yoksa kayan nokta (Float) m� oldu�unu kontrol eder.
void DFARecognizer(char* exp, char tokens[][nLenghtNameTokens], int *numTokens)
{
	
  // �lk s�tun, bir i�leci t�keten ge�i�lerle ilgilidir (+, -, / veya *).
 // �kinci s�tun, sol parantez kullanan ge�i�lerle ilgilidir.
 // ���nc� s�tun, sa� parantez kullanan ge�i�lerle ilgilidir.
 // D�rd�nc� s�tun, bir say�y� t�keten ge�i�lerle ilgilidir.
 // Be�inci s�tun, bir noktay� t�keten ge�i�lerle ilgilidir.
 // Alt�nc� s�tun, ba�ka herhangi bir karakteri t�keten ge�i�lerle ilgilidir.
	
	int transitionTable[nStates][nSymbols] = { {1, 2, 3, 4, -1, -1}, //durum(state) 0.
											   {-1, -1, -1, -1, -1, -1}, //durum(state) 1.
											   {-1, -1, -1, -1, -1, -1}, //durum(state) 2.
											   {-1, -1, -1, -1, -1, -1}, //...
											   {-1, -1, -1, 4, 5, -1},
											   {-1, -1, -1, 6, -1, -1},
											   {-1, -1, -1, 6, -1, -1} };
	

	//Hangi ��e, bir durumun kabul durumu olup olmad���n� g�sterir.
	int acceptanceStates[nStates] = {-1, 1, 1, 1, 1, -1, 1};										  

	int presentState = 0;
	int lastAcceptedState = 0;
	int inputPosition = 0;
	int flagInteger = 0, flagFloat = 0;


	
	while ((exp[inputPosition] != '\n'))
	{
		flagInteger = 0;
		flagFloat = 0;
		presentState = 0;
		lastAcceptedState = 0;
				
		if (isItOperator(exp[inputPosition]))
		{	
			if (transitionTable[presentState][0] != -1)
			{
				presentState = transitionTable[presentState][0];
			}	
		}
		else if (exp[inputPosition] == '(')
		{	
			if (transitionTable[presentState][1] != -1)
			{
				presentState = transitionTable[presentState][1];
			}	
		}
		else if (exp[inputPosition] == ')')
		{	
			if (transitionTable[presentState][2] != -1)
			{
				presentState = transitionTable[presentState][2];
			}	
		}
		else if (isItNumber(exp[inputPosition]))
		{
			while(1)
			{
				if (isItNumber(exp[inputPosition]))
				{
					if (transitionTable[presentState][3] != -1)
					{
						flagInteger = 1;
			
						presentState = transitionTable[presentState][3];
					}	
				}	
				else if (exp[inputPosition] == '.')
				{
					if (transitionTable[presentState][4] != -1)
					{
						flagFloat = 1;
		
						presentState = transitionTable[presentState][4];
					}	
				}
				else
				{
					break;
				}
				
				inputPosition++;	
			}
		}
		else
		{
			if (transitionTable[presentState][5] != -1)
			{
				presentState = transitionTable[presentState][5];
			}	
		}
		
		
		if (flagInteger != 1 && flagFloat != 1)
		{
			inputPosition++;	
		}
		
		
		//Ge�i�ten sonraki yeni durumun bir kabul durumu olup olmad���n� do�rular.
		if (acceptanceStates[presentState] == 1)
		{
			lastAcceptedState = presentState;
		}
		
		(*numTokens)++;
		
		switch(lastAcceptedState)
		{
			case 1:
			{
				strcpy(tokens[*numTokens], "Operator");
				break;
			}
			case 2:
			{
				strcpy(tokens[*numTokens], "Sol-Parantez");
				break;
			}
			case 3:
			{
				strcpy(tokens[*numTokens], "Sa�-Parantez");
				break;
			}
			case 4:
			{
				strcpy(tokens[*numTokens], "Integer");
				break;
			}
			case 6:
			{
				strcpy(tokens[*numTokens], "Float");
				break;
			}
			case -1:
			case 0:
			default:
			{
				strcpy(tokens[*numTokens], "Ge�ersiz");
				break;
			}	
			}
		}
	}
