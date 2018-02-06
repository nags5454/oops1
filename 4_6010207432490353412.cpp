#include <iostream>
using namespace std;
class cl {
	int i;
	public:
	cl(int j) { i=j; }
	int show_i() { return i; }
};

int main()
{
	cl ob[3] = {1, 2, 3}; // initializers
//	cl ob[3] = { cl(1), cl(2), cl(3) };
	cl *p;
	p = ob; // get start of array
	int i;
	for(i=0; i<3; i++)
	{
		cout << p->show_i() << "\n";
		p++;
	}
	
	return 0;
}
