#include <iostream>
#include <string>
#include <map>

#include <sstream>

#include <iostream>

using namespace std;

int main() {
    string s = "999s";

    stringstream degree(s);

    int x = 0;
    degree >> x;

    cout << "Value of x: " << x;
}