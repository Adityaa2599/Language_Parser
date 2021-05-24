#include "gram_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
 {
    initializeTokenMap();
    char*filename = "sourcecode.txt";
    tokenStream *s = (tokenStream*)malloc(sizeof(tokenStream));
    *s = NULL;
    tokeniseSourcecode(filename,s);
     //printTokenList(*s);
     //printTokenMap();
     printf("Source code read \nNow reading Grammer \n");
     Grammar g;
     char *grammer_file="grammar.txt";
     g=read_grammar(grammer_file,g);
    printf("Grammer read \n");
     TreeNode *ret=createParseTree(g,s);
    // print_list(g+15);printf("\n");
     
     if(ret!=NULL)
     { printf("parse successfull \n");
      
      /*for(int i=0;i<ret->num_childern;++i)
      {
          printf("%s ",ret->children[i]->symbol_name);
      }*/
     //print_ParseTree(ret);
     }
     else
     {
         printf("unable to parse maybe syntax error \n");
         //printTokenList( *s);
     }
   typeExpressionTable * tb;
     tb=(typeExpressionTable * )malloc(sizeof(typeExpressionTable));
     traverseTree(ret,tb);
     
    /*// printf("Yes \n");
    for(int i=0;i<tb->count;++i)
     {   if(tb->rows[i].type_exp.primitive_id==INTEGER)
          printf("Integer \n");
          else
          {
              printf("Not Integer");
          }
          
        
     }*/
      return 0;
}