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
	cl ob(88), *p;
	p = &ob; // get address of ob
	cout << p->show_i() << endl; // use -> to call show_i()
	
	return 0;
}
