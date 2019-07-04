#ifndef __LOGGINGSTRINGNODE_H__
#define __LOGGINGSTRINGNODE_H__

// TODO
/**
 * struct definition of class and external definition of class table
 */

struct LoggingStringNode_class {
    struct StringNode_class* super;
    int (*compareTo) (void*,void*);
    void (*printNode) (void*);
    void (*insert) (void*,void*);
    void (*print) (void*);
    void (*delete) (void*);
};
extern struct LoggingStringNode_class LoggingStringNode_class_table;

/**
 * struct definition of object
 */

struct LoggingStringNode;
struct LoggingStringNode {
    struct LoggingStringNode_class* class;

    // instance variables defined in super class(es)
    struct LoggingStringNode* left;
    struct LoggingStringNode* right;

    // instance variables defined in this class

    char* s;
};

/**
*definition of methods implemented by this class
*/
void LoggingStringNode_ctor(void*,char*);
void LoggingStringNode_insert(void*,void*);

/**
*definition of new for class
*/
void* new_LoggingStringNode(char*);

#endif /*__LOGGINGSTRINGNODE_H__*/
