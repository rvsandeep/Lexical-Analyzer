/*
 * lexer.c
 * The lexical analyzer is the first phase of compiler.
 * Its main task is to read the input characters and produces
 * output a sequence of tokens that the parser uses for syntax analysis.
 *
 *  	Created on: Dec-2012
 *      Authors: Sandeep R V, Sandeep Venkatesh
 *      email: rvsandeep792@gmail.com, kvsandy22@gmail.com
 *      Contact No: +91-90194-34240 , +91-88612-64334
 */

//including headers
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>


// keywords_tree is a self-balancing tree of keywords
struct keywords_tree {
	char *keyword;						// keyword string
	int id;								// unique id assigned to each keyword
	int height;							// height of the node down the tree (i.e max (height of left subtree , height of right subtree)+1) 
	struct keywords_tree *left;			
	struct keywords_tree *right;
};

// keeps count of number of lines in the source program
int lineCount;

// keywords pointer to structure keywords_tree
typedef struct keywords_tree *keywords;

/*
 * getnode returns block of space for structure of type keywords_tree
 * exits with error message if space was not allocated
 */
keywords getnode() {
	keywords word = (keywords) malloc(sizeof(struct keywords_tree));
	if (word == NULL ) {
		printf("Heap space not sufficient");
		exit(1);
	} else {
		word->left = NULL;
		word->right = NULL;
		word->height = 0;
		return word;
	}
}
// returns height of the node down the tree (-1 if node is null)
int height(keywords root) {
	return root == NULL ? -1 : root->height;
}
// returns maximum of two numbers
int max(int a, int b) {
	return a > b ? a : b;
}

/*
 * Used by the self balancing tree to balance such a condition by rotating left once
 *
 * a
 *  \		left rotation
 *   b			=>			     b
 *    \						   / \
 *     c					  a   c
 */
keywords left_rotation(keywords root) {
	keywords temp = root->right;
	root->right = temp->left;
	temp->left = root;
	root->height = max(height(root->left), height(root->right)) + 1;
	temp->height = max(height(temp->right), root->height) + 1;
	return temp;
}

/*
 * Used by the self balancing tree to balance such a condition by rotating right once
 *
 *      c
 *     /	right rotation
 *    b			=>		     b
 *   /					   / \
 *  a					  a   c
 */
keywords right_rotation(keywords root) {
	keywords temp = root->left;
	root->left = temp->right;
	temp->right = root;
	root->height = max(height(root->left), height(root->right)) + 1;
	temp->height = max(height(temp->left), root->height) + 1;
	return temp;
}

/*
 * Used by the self balancing tree to balance such a condition by right and left rotation
 *
 *  a					i.e  a
 *   \	  right rotation	      right   \		   left
 *    c			=>		      c     =>	      b		=> 	  		 b
 *   /		on right		     /	 rotation      \	 rotation		/ \
 *  b		sub tree	            b	    	   	c   			       a   c
 */
keywords left_right_rotation(keywords root) {

	root->left = left_rotation(root->left);
	return right_rotation(root);
}

/*
 * Used by the self balancing tree to balance such a condition by rotating left and right rotation
 *
 *  	 c		            c
 *   	/  left rotation	   /	   right rotation
 *    a			=>        b   	  		 =>				     b
 *  	\		         / 							    / \
 *  	 b		        a		   					   a   c
 */
keywords right_left_rotation(keywords root) {

	root->right = right_rotation(root->right);
	return left_rotation(root);
}
/*
 * inserts an keyword with unique id assigned into the tree
 */
keywords insert_keyword(char *word, int id, keywords root) {
	if (root == NULL ) {
		root = getnode();
		root->keyword = word;
		root->id = id;
	} else {
		if (strcmp(word, root->keyword) < 0) {

			// insert in left sub-tree
			root->left = insert_keyword(word, id, root->left);

			// balance tree
			if (height(root->left) - height(root->right) == 2) {
				if (strcmp(word, root->left->keyword) < 0)
					root = right_rotation(root);
				else
					root = left_right_rotation(root);
			}
		} else if (strcmp(word, root->keyword) > 0) {

			// insert in right sub-tree
			root->right = insert_keyword(word, id, root->right);

			// balance tree
			if (height(root->right) - height(root->left) == 2)
				if (strcmp(word, root->right->keyword) > 0)
					//if( x.compareTo( t.right.element ) > 0 )
					root = left_rotation(root);
				else
					root = right_left_rotation(root);
		} else
			; //duplicate
	}
	// update height of the node down the tree
	root->height = max(height(root->left), height(root->right)) + 1;
	return root;
}
/*
 * constructs the keyword tree by calling insert_keyword repeatedly
 */
keywords construct_tree(keywords root) {
	root = insert_keyword("auto", 1, root);
	root = insert_keyword("break", 2, root);
	root = insert_keyword("case", 3, root);
	root = insert_keyword("char", 4, root);
	root = insert_keyword("const", 5, root);
	root = insert_keyword("continue", 6, root);
	root = insert_keyword("default", 7, root);
	root = insert_keyword("do", 8, root);
	root = insert_keyword("double", 9, root);
	root = insert_keyword("else", 10, root);
	root = insert_keyword("enum", 11, root);
	root = insert_keyword("extrn", 12, root);
	root = insert_keyword("file", 13, root);
	root = insert_keyword("float", 14, root);
	root = insert_keyword("for", 15, root);
	root = insert_keyword("goto", 16, root);
	root = insert_keyword("if", 17, root);
	root = insert_keyword("int", 18, root);
	root = insert_keyword("long", 19, root);
	root = insert_keyword("main", 20, root);
	root = insert_keyword("register", 21, root);
	root = insert_keyword("return", 22, root);
	root = insert_keyword("short", 23, root);
	root = insert_keyword("signed", 24, root);
	root = insert_keyword("sizeof", 25, root);
	root = insert_keyword("static", 26, root);
	root = insert_keyword("typedef", 27, root);
	root = insert_keyword("union", 28, root);
	root = insert_keyword("unsigned", 29, root);
	root = insert_keyword("void", 30, root);
	root = insert_keyword("volatile", 31, root);
	root = insert_keyword("while", 32, root);
	root = insert_keyword("struct", 33, root);
	root = insert_keyword("NULL", 34, root);
	return root;
}
/*
 * looks in the tree for the specified keyword in O(log n) time complexity
 */
int look_up(keywords root, char *token) {
	if (root == NULL ) {
		return -1;
	} else {
		if (strcmp(token, root->keyword) == 0) {
			return root->id;
		} else if (strcmp(token, root->keyword) < 0) {
			return look_up(root->left, token);
		} else
			return look_up(root->right, token);
	}
}
/*
 * returns 1 (true) if character is not space delimiters (' ', '\t' , '\n')
 * else return 0 (false)
 */
int if_not_space(char c) {
	if (c != ' ' && c != '\t' && c != '\n')
		return 1;
	return 0;
}
/*
 * returns 0 (false) if the word is not a constant
 * else prints it has constant to the screen and returns 1 (true)
 */
int check_constant(char *word) {
	int i, constant;
	for (i = 0; i < strlen(word); i++) {
		if (!isdigit(word[i]))
			return 0;
	}
	printf("%s\t\t:\tConstant\n", word);
	return 1;
}
/*
 * does look-up the for the token in keywords_tree
 * and if exists prints as keyword and returns as true
 * else checks if constant
 */
int compare_keywords(char *token, keywords root) {

	int id = look_up(root, token);
	if (id >= 0) {
		printf("%s\t\t:\tKeyword id:%d\n", token, id);
		return 1;
	}
	return 0;
}
/*
 * classifies the token to a keyword , identifier , constant and operator
 */
void classify_word(char *token, keywords root) {

	// if token is not keyword and constant then it is identifier
	if (! ( compare_keywords(token, root) || check_constant(token) )) {
		printf("%s\t\t:\tIdentifier\n", token);
	}
}

/*
 * returns 1 (true) if the character is not a special character
 * else prints special character and returns 0 (false)
 */
int if_not_special_char(char c) {
	if (c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')'
			|| c == '@' || c == '#' || c == '$' || c == ';') {

		printf("%c\t\t:\tSpecial Character\n", c);
		return 0;

	}
	return 1;
}
/* returns 1 (true) if  character is not an operator
 * else outputs as operator and returns 0 (false)
 */
int if_not_arith_op(char c) {
	if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/') {

		printf("%c\t\t:\tArithmetic Operator\n", c);
		return 0;
	}
	return 1;
}

/*
 *  returns 1 (true) if  character is not an relational operator
 *  else outputs as relational operator and returns 0 (false)
 */
int if_not_relational_op(char c) {
	if (c == '<' || c == '>' || c == '!') {

		printf("%c\t\t:\tRelational Operator\n", c);
		return 0;
	}
	return 1;
}
/*
 *  returns 1 (true) if  character is not an a quote
 *  else scans till the next quote (printf and scanf statements)
 *  outputs it as string constant
 */

int if_not_qoutes(char line[], int *j) {
	int k;
	k = *j;
	if (line[*j] == '\"') {
		printf("%c", line[*j]);
		while (line[++k] != '\"')
			printf("%c", line[k]);
		printf("%c\t\t:\t", line[*j]);
		printf("String Constant\n");
		*j = k;
		return 0;
	}else if (line[*j] == '\'') {
		printf("%c", line[*j]);
		while (line[++k] != '\'')
			printf("%c", line[k]);
		printf("%c\t\t:\t", line[*j]);
		printf("Character Constant\n");
		*j = k;
		return 0;
	}
	return 1;
}

/*
 *  returns 1 (true) if  character is a not a bit operator or logical operator
 *  else outputs as bitwise operator or logical operator and returns 0 (false)
 */

int if_not_bit(char line[], int *j) {
	int k;
	if (line[*j] == '&' || line[*j] == '|') {
		k = *j + 1;
		if (line[*j] == line[k]) {
			*j = *j + 1;
			printf("%c%c\t\t:\tLogical operator\n", line[k - 1], line[k]);
			return 0;
		} else {
			printf("%c\t\t:\tBit-wise Operator\n", line[k - 1]);
			return 0;
		}
	}
	return 1;
}
/*
 * reads the file and returns a line
 */
void get_line(FILE * file, char line[50]) {
	int i = 0;
	char c = getc(file);
	char next;
	while(!if_not_space(c)) {
		if (c=='\n') ++lineCount;
		c = getc(file) ;
	}
	
	printf("\nLine %d:\t",++lineCount);

	while (c != '\n' && !feof(file)) {
	
		if (c=='/') {
			next = getc(file);
			if (next == '/') {
				while(c!='\n')
					c = getc(file);
				line[i] = '\0';
				return;
			} else if ( next == '*') {
				c = getc(file);
				while ( 1 ){
					if (c=='\n')
						++lineCount;
					next  = getc(file);
					if (c == '*' && next == '/' &&  !feof(file))
						break;
					c = next;
				}
			}else {
				line[i++] = c;
			}
			
		} else {
			line[i++] = c;
		}
		c = getc(file);
	}
	line[i] = '\0';
	return;
}

// Main method of the project
int main(int argc, char *argv[]) {
	int i = 0, j = 0, stringlength;
	FILE *file;

	//source file
	file = fopen("input.c", "r");

	char c, buffer[500];
	char line[500];
	int aConstant = 0;

	keywords root = NULL;

	// initializing tree
	root = construct_tree(root);

	//tokenizing the words
	while (!feof(file)) {

		// read a line
		get_line(file, line);
		j = 0;
		stringlength = strlen(line);

		// checking preprocessor directives
		if (line[0] == '#') {
			printf("%s\t\t:\tPreprocessor	Directive\n\n", line);
			continue;
		} else
			printf("%s\n\n", line);
			
		if (stringlength!=0)
			printf("Lexeme\t\t:\tType\n\n");			
		// tokenizes the line
		while (j != stringlength) {

			while ( (!(line[j]=='\0')) && if_not_relational_op(line[j])
					&& (if_not_special_char(line[j])) && (if_not_bit(line, &j))
					&& (if_not_arith_op(line[j])) && (if_not_qoutes(line, &j)) && (if_not_space(line[j])) ) {
				buffer[i++] = line[j++];
			}
			buffer[i] = '\0';
			j++;
			if (i != 0) {
				classify_word(buffer, root);
			}
			i = 0;
			if (j > stringlength)
				break;
		}
	}
}
