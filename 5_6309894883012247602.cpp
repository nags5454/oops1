#include <iostream>

using namespace std;
int m=30;
int main()
{
      int m=20;
      {
            int m=10;
            cout<<endl<<m;
            cout<<endl<<::m;
            cout<<endl<<::m<<endl;
      }
      return 0;
}
