#include <iostream>
using namespace std;

struct MyStruct
{
   int a;
   int b;
};

int GetVariable(int MyStruct::* field)
{
   MyStruct *_myStruct = new MyStruct();
   _myStruct ->a = 1;
   _myStruct ->b = 2;

   return _myStruct->*field;
}

int main( )
{
   int my_variable = GetVariable(&MyStruct::a); 
   std::cout << my_variable << std::endl;
   return 0;   
}