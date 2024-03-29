#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "integernode.h"
#include "stringnode.h"
#include "reversestringnode.h"
#include "loggingstringnode.h"

int main(int argc, char** argv) {
  if (argc > 2) {
    struct Node* tree = NULL;
    for (int i=2; i< argc; i++) {
      struct Node* node = NULL;
      if (strcmp(argv[1], "i") == 0)
        node = new_IntegerNode(atoi(argv[i])); // TODO integer value of argument atoi(argv[i])
      else if (strcmp(argv[1], "s") == 0)
        node = new_StringNode(argv[i]);
      else if (strcmp(argv[1], "r") == 0)
        node = new_ReverseStringNode(argv[i]); // TODO
      else if (strcmp(argv[1], "l") == 0)
        node = new_LoggingStringNode(argv[i]);// TODO
      if (node != NULL) {
        if (tree == NULL)
          tree = node;
        else
          tree->class->insert(tree, node);
      }
    }
    if (tree != NULL) {
      tree->class->print(tree);
      if (strcmp(argv[1], "i") == 0) {
        // TODO print sum
        struct IntegerNode* this = (struct IntegerNode*) tree;
        printf("%d\n", this->class->sum(this));
        }
        // TODO node->delete()
        Node_delete(tree);
    }
  }
}
