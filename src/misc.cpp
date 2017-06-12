#include <misc.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

string num2str(int num)
{
	stringstream s;
	s << num;
	return s.str();
}
