/* parse.c
 contains the following functions:
    - create parse tree
 Saransh Jindal,    Siddharth Awasthi,  Aditya Sharma
 2018A7PS0221P      2018A7PS0287P       2018A7PS0367P 
 */
#include "gram_module.h"
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
int assignment_line;
int is_NT(char *s)
{
    if(s[0]=='<')
     return 1;
     else
     {
         return 0;
     }
     
}
TreeNode * createNode(tokenStream *s,Grammar g,Gnode *Node,int depth)
{ 
    if(*s==NULL) return NULL;
    if(Node->nt==0){
      // printf("terminal %s ts l=%s tkn=%s ln=%d\n",Node->symbol_name,(*s)->lexeme,(*s)->tk_name,(*s)->line_num);
      if(strcmp(Node->symbol_name,(*s)->tk_name)==0){
        //printf("exiting create Node \n");
        TreeNode *ret=(TreeNode *)malloc(sizeof(TreeNode));
        ret->num_children=0;
        ret->children=NULL;
        ret->symbol_name= Node->symbol_name;
        ret->lex_token=*s;
        ret->depth=depth;
        *s=(*s)->next;
          //printf("Matched\n");
        return ret;
      }
      else{  
          //printf("did not match \n");
        return NULL;
      }  
    }
    //printf("Non terminal %s ts l=%s tkn=%s ln=%d\n",Node->symbol_name,(*s)->lexeme,(*s)->tk_name,(*s)->line_num);

    TreeNode *ret;
    ret=(TreeNode *)malloc(sizeof(TreeNode));
    ret->symbol_name=Node->symbol_name;
    ret->lex_token=NULL;
    ret->depth=depth;
    for(int i=0;i<65;++i){
      if(strcmp(Node->symbol_name,g[i].symbol_name)==0){
        int num_child=g[i].num_children;
          ret->num_children=g[i].num_children;
        int flag=1; 
        ret->children=(TreeNode **)malloc(sizeof(TreeNode*)*num_child);
        Gnode *temp_Gnode=g[i].next;
        //tokenStream *t=s;
        tokenStream t=*s;
        for(int j=0;j<num_child;++j){
          TreeNode *temp=createNode(s,g,temp_Gnode,depth+1);
          if(temp==NULL)
          {
            flag=0;
            break;
          }
          (ret->children)[j]=temp;
          temp_Gnode=temp_Gnode->next;
        }
        if(flag==0)
        {   
          *s=t;
          free(ret->children);
        }
        else{
          //printf("found node for %s %d\n",Node->symbol_name,ret->num_children); 
          
          return ret;
        }
      }    
    }
    free(ret);
    return NULL;
    }

    TreeNode* createParseTree(Grammar g,tokenStream *s)
    {

    tokenStream *t=s;

    Gnode * start=(Gnode *)malloc(sizeof(Gnode));
    start->next=NULL;
    start->symbol_name=(char *)malloc(8);
    start->num_children=1;
    start->nt=1;
    strcpy(start->symbol_name,"<Start>");
    //printf("done \n");
    TreeNode *ret;
    //printf("%s \n",(*s)->lexeme);
    //assignment_line=find_assignment(s);
    ret=createNode(t,g,start,0);

    return ret;
}
