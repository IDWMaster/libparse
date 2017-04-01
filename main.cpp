#include "parser.h"
#include <stdio.h>
using namespace libparse;
int main(int argc, char** argv) {
  ParseTree tree("Hello world. End.");
  StringRef ref;
  tree.expectWhitespace(ref);
  printf("-%s-\n",((std::string)ref).data());
  tree.expectWhitespace(ref);
  printf("-%s-\n",((std::string)ref).data());
  if(tree.expectWhitespace(ref)) {
    printf("Error\n");
  }
  return 0;
}