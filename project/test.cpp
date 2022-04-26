#include <iostream>
#include <string>
#include <map>

class GrandParent 
{
  virtal void print()
  {
    std::cout << "Grand Parent\n";
  }

};

class Parent: public GrandParent
{
  void print()
  {
    std::cout << "Parent\n";
  }
}

class Kid: public Parent
{
  void print()
  {
    std::cout << "Kid\n";
  }

}

int main ()
{
  Parents parent;
  GrandParent *gp= dynamic_cast<GrandParent*>(&parent);
  
  
}