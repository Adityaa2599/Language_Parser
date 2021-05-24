/* gram_module.h
 contains all function prototypes and data structures declaration
 Saransh Jindal,    Siddharth Awasthi,  Aditya Sharma
 2018A7PS0221P      2018A7PS0287P       2018A7PS0367P 
 */

typedef struct token_node{
    char*lexeme;
    char*tk_name;
    int line_num;
    struct token_node*next;
}TK_node;

typedef struct gram_node{
    char *symbol_name;
    int nt;
    int num_children;
    struct gram_node *next;
} Gnode;


typedef struct token_map_node{
    char*lexeme;
    char*tk_name;
    int is_terminal;
    struct token_map_node*next;
}TK_map_node;
typedef struct token_map_node* tokenMap;
typedef struct gram_node* Grammar;
typedef struct token_node* tokenStream;
void initializeTokenMap();
void tokeniseSourcecode(char*filename, tokenStream *s);
void printTokenList(tokenStream s);
Grammar read_grammar(char * file_name,Grammar g);
void print_list(Gnode *t);

typedef enum{INTEGER, REAL, BOOLEAN} data_type;
typedef enum{PRIMITIVE, RECTANGULAR, JAGGED} num_type;
typedef struct{
  int dimension_rect;
  char **range_rect_low;
  char **range_rect_high;
}rect_array_exp;


typedef struct{
  int dimension_jagged;

  int r1_low;
  int r1_high;

  int *r2; //size would be r1_high-r1_low 
  
  //only if 3D
  int **r3; //jagged array of [r1_high-r1_low][]

}jagged_array_exp;


typedef union {
    rect_array_exp rect_array_id;
    jagged_array_exp jagged_array_id;
    data_type primitive_id;
}type_exp_ds;

typedef enum{STATIC, DYNAMIC, NOT_APPLICABLE} stat_dynam_ds;

typedef struct {
    
    char identifier[30];
    num_type nm_type; // datatype - primitive/rect/jagged
    stat_dynam_ds stat_dynam;
    type_exp_ds type_exp;
    int error;
}table_element; 

typedef struct TABLELEMENT{
 table_element* rows;
 int count;
 int cap;
}typeExpressionTable;

table_element create_table_element(char*id, num_type nm_type, stat_dynam_ds stat_dynam, type_exp_ds type_exp);
void add_to_table(typeExpressionTable* tbl, table_element new);
typeExpressionTable* createTETable();

typedef struct tn{
    char *symbol_name;
    tokenStream lex_token ;
    int num_children;
    struct tn **children;
    int depth;
    type_exp_ds type_exp_tn;
}TreeNode;
void printTokenMap();

TreeNode* createParseTree(Grammar g,tokenStream *s);
void print_ParseTree(TreeNode * Node);
int is_NT(char *s);

void populate_table(TreeNode*node, typeExpressionTable*tetable);
void traverseTree(TreeNode* tree, typeExpressionTable*tetable);
void populate_table(TreeNode*node, typeExpressionTable*tetable);
int  checkTypeExpression(TreeNode*lhs,TreeNode*rhs, typeExpressionTable* tetable);
void print_error(int line_num,int stat_type,char *operator,char *fop,data_type f,char *soperand,data_type s,int depth,char *message);
void printTETable(typeExpressionTable T);