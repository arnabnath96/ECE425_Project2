#ifndef STACK_H
#define STACK_H

/*************************************************************************************************
 Stack Node
*************************************************************************************************/

typedef struct StackNode_type
{
	int data;
	struct StackNode_type* below;
} StackNode;

StackNode* create_stacknode(int);
void delete_stacknode(StackNode*);

/*************************************************************************************************
 Stack
*************************************************************************************************/

typedef struct Stack_type
{
	struct StackNode_type* top;
} Stack;

Stack* create_stack();
Stack* copy_stack(Stack*);
void copy_stacknode(StackNode*, Stack*);
void delete_stack(Stack*);
void push_stack(Stack*, int);
int pop_stack(Stack*);
void print_stack(Stack*);
int isempty_stack(Stack*);

#endif // STACK_H
