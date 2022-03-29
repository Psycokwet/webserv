#include <iostream>
#include <string>

int main ()
{
  std::string str="We think in generalities, but we live in details.";
                                           // (quoting Alfred N. Whitehead)

  std::string str2 = str.substr (3,5);     // "think"

  std::string a_word = "live";
  std::size_t pos = str.find(a_word);      // position of "live" in str

  std::string str3 = str.substr (pos + a_word.size());     // get from "live" to the end

  std::cout << str3 << '\n';

  return 0;
}