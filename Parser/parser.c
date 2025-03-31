/*
 * A top down operator precedence parser (Pratt parser).
 * Input: Token stream of the Python language generated by lexer.c/main.c.
 * Output: AST to be translate into lower-level IR via tree rewrite.
 */


#include "parser.h"
#include "../Lexer/lexer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


Parser
parser;


// This table is based on Nystrom's table, the only difference being the functions
// themselves do not produce bytecode, they instead weave a AST (like Crockford's
// nuds and leds).

// For more information on the table, refer to Robert Nystrom's Crafting Interpreters, Chapter 17

// For more inforation on the nuds and leds, refer to Crockford's
// blog below:
// https://www.crockford.com/javascript/tdop/tdop.html


Rule
table[] = {
	[TOKEN_INTEGER]		= {Prec_Integers,	nud_atom,	NULL},
	[TOKEN_IDENTIFIER]     	= {Prec_Identifiers,	nud_atom,	led_atom},
	[TOKEN_ADD]            	= {Prec_AddSub,		NULL,		led_binary},
	[TOKEN_MINUS]          	= {Prec_AddSub,		NULL,		led_binary},
	[TOKEN_EQUALS]         	= {Prec_Equals,		NULL,		led_binary},
	[TOKEN_MULTIPLICATION] 	= {Prec_MultDiv,	NULL,		led_binary},
	[TOKEN_DIVISION]       	= {Prec_MultDiv,	NULL,		led_binary},
	[TOKEN_EOF]            	= {Prec_EOF,		NULL,		NULL},
	[TOKEN_LEFT_PAREN]     	= {Prec_Paren,		nud_paren,	led_paren},
	[TOKEN_RIGHT_PAREN]    	= {Prec_Right_Paren,	NULL,		led_rparen},
	[TOKEN_NEWLINE]		= {Prec_Newline,	NULL,		led_binary},
	[TOKEN_PRINT]		= {Prec_Equals,		nud_print,	NULL},
	[TOKEN_COMMA]		= {Prec_Identifiers,	NULL,		led_comma},
	[TOKEN_FOR]		= {Prec_Identifiers,	nud_for,	NULL},
	[TOKEN_TAB]		= {Prec_Newline,	nud_tab,	NULL},
	[TOKEN_IN]		= {Prec_Keyword,	nud_in,		NULL},
	[TOKEN_COLON]		= {Prec_Keyword,	NULL,		led_colon},
	[TOKEN_RANGE]		= {Prec_Identifiers,	NULL,		led_range},
};


TreeNode *
nud_tab (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = spawn_node (parser.previous);
	return_node->left = spawn_node (spawn_token (TOKEN_RIGHT_PAREN));
	return_node->right = parse (table[parser.current.type].precedence,
			tree, was_newline);
	return return_node;
}


TreeNode *
led_colon (TreeNode *tree, bool was_newline)
{
	parser.previous = parser.current;
	parser.current = get_next_token (false);
	return tree;
}


TreeNode *
led_range(TreeNode *tree, bool was_newline)
{
	// prev should be keyword in.
	TreeNode *return_node = tree;
	// Curr should be range.
	return_node->left = spawn_node (parser.current);
	// Consume integer nested in parens.
	return_node->right = parse ((Prec_Right_Paren + 1), tree, was_newline);
	return return_node;
}


TreeNode *
nud_in (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = spawn_node (parser.previous);
	// We will let some other led handle weaving this correctly.
	// We will need to mimic iterator semantics.
	return return_node;
}


TreeNode *
nud_for (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = spawn_node (parser.previous);
	return return_node;
}


TreeNode *
led_atom (TreeNode *tree, bool was_newline)
{
	// Tree should be for or something like that.
	TreeNode *return_node = tree;
	// curr should be identifier.
	return_node->left = spawn_node (parser.current);
	return_node->right = parse (table[parser.current.type].precedence,
			tree, was_newline);
	return return_node;
}


TreeNode *
led_comma (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = spawn_node (parser.current);
	return_node->left = parse (table[parser.current.type].precedence,
			tree, was_newline);
	return_node->right = tree;
	return return_node;
}


TreeNode *
nud_print (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = spawn_node (parser.previous);
	return return_node;
}


TreeNode *
led_binary (TreeNode *tree, bool was_newline)
{
	TreeNode *new_node = spawn_node (parser.current);
	new_node->left = tree;
	new_node->right = parse (table[parser.current.type].precedence,
			tree, was_newline);
	return new_node;
}


TreeNode *
led_rparen (TreeNode *tree, bool was_newline)
{
	parser.previous = parser.current;
	parser.current = get_next_token (false);
	return tree;
}


TreeNode *
nud_paren (TreeNode *tree, bool was_newline)
{
	// +1 so it breaks on right parens.
	TreeNode *inner = parse ((Prec_Right_Paren + 1), tree, was_newline);
	return inner;
}


TreeNode *
led_paren (TreeNode *tree, bool was_newline)
{
	TreeNode *return_node = tree;
	return_node->left = spawn_node (spawn_token (TOKEN_RIGHT_PAREN));
	// Consume args.
	return_node->right = parse ((Prec_Right_Paren + 1), tree, was_newline);
	return return_node;
}


TreeNode *
nud_atom (TreeNode *tree, bool was_newline)
{
	TreeNode *tree_node = spawn_node (parser.previous);
	return tree_node;
}


TreeNode *
spawn_node (Token t)
{
	TreeNode *return_node = malloc (sizeof (*return_node));
	return_node->left = NULL;
	return_node->right = NULL;
	return_node->contents = t;
	return return_node;
}


StackNode *
spawn_stack_node (TreeNode *t)
{
	StackNode *return_node = malloc (sizeof (StackNode));
	return_node->contents = t;
	return_node->next = NULL;
	return return_node;
}


void
push (StackNode **stack, TreeNode *t)
{
	StackNode *push_node = spawn_stack_node (t);
	push_node->next = *stack;
	*stack = push_node;
}


TreeNode *
pop (StackNode **stack)
{
	StackNode *temp = *stack;
	TreeNode *return_t = temp->contents;
	*stack = (*stack)->next;
	free (temp);
	return return_t;
}


bool
is_stack_empty (StackNode **stack)
{
	if (*stack == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool
is_tree_node_empty (TreeNode *node)
{
	if ((node->left == NULL) && (node->right == NULL))
	{ // if leaf
		return true; // return true
	}
	else
	{
		return false;
	}
}


// Same thing as preorder but this one performs inorder traversal.
size_t
count_tree_nodes (TreeNode *tree)
{
	StackNode *stack = NULL;
	TreeNode *p = tree;
	size_t return_num = 0;
	for (;;)
	{
		if (is_tree_node_empty (p) == true)
		{
			return_num++;
			if (is_stack_empty (&stack) == true)
			{
				break;
			}
			else
			{
				p = pop (&stack);
				return_num++;
				p = p->right;
			}
		}
		else
		{
			push (&stack, p);
			p = p->left;
		}
	}
	return return_num;
}

void
enqueue (StackNode **queue, TreeNode *t)
{
	StackNode *queue_node = malloc (sizeof (StackNode));
	queue_node->contents = t;
	queue_node->next = *queue;
	*queue = queue_node;
}


TreeNode *
dequeue (StackNode **queue)
{
	StackNode *temp = *queue;
	StackNode *prev = NULL;
	while (temp->next != NULL)
	{
		prev = temp;
		temp = temp->next;
	}
	TreeNode *return_int = temp->contents;
	if (prev != NULL)
	{
		prev->next = NULL;
	}
	free (temp);
	return return_int;
}


TreeNode **
postorder (TreeNode *root, size_t size)
{
	TreeNode **return_array = malloc (size * sizeof (TreeNode));
	StackNode *stack = NULL;
	StackNode *stack2 = NULL;
	StackNode *queue = NULL;
	int count = 0;
	TreeNode *p = root;
	for (;;)
	{
		if (is_tree_node_empty (p) == true)
		{
			enqueue (&queue, p);
			if (is_stack_empty (&stack) == true)
			{
				break;
			}
			else
			{
				p = pop (&stack);
				p = p->right;
			}
		}
		else
		{
			push (&stack2, p);
			count++;
			push (&stack, p);
			p = p->left;
		}
	}
	for (int i = 0; i < count; i++)
	{
		TreeNode *x = pop (&stack2);
		enqueue (&queue, x);
	}
	for (size_t i = 0; i < size; i++)
	{
	    return_array[i] = dequeue (&queue);
	}
	return return_array;
}


// Equivilant to TAOCP VOL I, 3ed. Page 320, Algorithm T).
TreeNode **
preorder (TreeNode *root, size_t size)
{
	TreeNode **return_array = malloc (size * sizeof (TreeNode));
	StackNode *stack = NULL; // Stack needed to traverse tree
	StackNode *queue = NULL; // Queue stores our output,
		     // i.e. result of visiting each node
	TreeNode *p = root;
	for (;;)
	{
		if (is_tree_node_empty (p) == true)
		{
			enqueue (&queue, p); // Visit leaf
			if (is_stack_empty (&stack) == true)
			{
				break;
			}
			else
			{
				p = pop (&stack); // Backtrack
				p = p->right;
			}
		}
		else
		{
			enqueue (&queue, p); // Visit root
			push (&stack, p); // Store spot for backtrack
			p = p->left;
		}
	}
	for (size_t i = 0; i < size; i++)
	{
	    return_array[i] = dequeue (&queue);
	}
	return return_array;
}


TreeNode *
parse (Precedence rbp, TreeNode *tree, bool was_newline)
{
	// This boolean helps us to determine whether whitespace is acting as indentation.
	// (Since python standard is to use spaces instead of tabs).
	
	parser.previous = get_next_token (was_newline); // Initialize

	if (parser.previous.type == TOKEN_NEWLINE)
	{
		was_newline = true;
	}
	else
	{
		was_newline = false;
	}
	
	SemanticCode c = table[parser.previous.type].nud; // Get prefix code

	if (c == NULL)
	{
		printf ("error in parse()\n");
	}
	// c(tree) will return identifier or value itself
	TreeNode *left = c (tree, was_newline);
	// Slip past identifier or val and get to operator
	parser.current = get_next_token (was_newline);

	if (parser.current.type == TOKEN_NEWLINE)
	{
		was_newline = true;
	}
	else
	{
		was_newline = false;
	}

	while (rbp <= table[parser.current.type].precedence)
	{ // While rbp is ramping - 

		if (parser.current.type == TOKEN_NEWLINE)
		{
			was_newline = true;
		}
		else
		{
			was_newline = false;
		}
		if (parser.previous.type == TOKEN_EOF)
		{
			break;
		}
		c = table[parser.current.type].led; // Get infix code
		left = c (left, was_newline); // - run infix code
	}

	return left; // when rbp drops we break the loop and fold our tree
}
