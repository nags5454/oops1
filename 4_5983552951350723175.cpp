#include <iostream>
using namespace std;

void repeatChar(char = '*', int = 10);

int main(void)
{

	repeatChar('r',7);			//	rrrrrrr

	repeatChar('%');			//	%%%%%%%%%%

	repeatChar();				//	**********
	
	return 0;
}

void repeatChar(char ch, int num)
{
	int i;
	if(num < 0)
	{
		return;
	}
	for(i=0; i<num; i++)
	{
		cout << ch;
	}
}
