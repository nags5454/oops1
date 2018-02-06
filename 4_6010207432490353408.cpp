#include <iostream>
using namespace std;
class shared {
	static int a;	//does not allocate storage
	int b;
	public:
	void set(int i, int j) {a=i; b=j;}
	void show();
};

int shared::a; // define a
void shared::show()
{
	cout << "This is static a: " << a;
	cout << "\nThis is non-static b: " << b;
	cout << "\n";
}
int main()
{
	shared x, y;
	x.show();	
	x.set(1, 1); // set a to 1
	x.show();
	y.set(2, 2); // change a to 2
	y.show();

	x.show(); 
	return 0;
}

/*
When you precede a member variable's declaration with static, you are telling the
compiler that only one copy of that variable will exist and that all objects of the class
will share that variable.
All static variables are initialized to zero before the first object is created.
*/
