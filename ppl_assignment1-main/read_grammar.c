/*contains functions 
fill_Gnode = Fills the particular element of the grammar array with the rule of the grammar in the form of
            Linked List where each node is defined by Gnode
print_list =self explanatory

read_grammar= the actual function which opens the file and reads each rule and calls fill_Gnode to put put it
              in the form of linked list in the array
              
 Saransh Jindal,    Siddharth Awasthi,  Aditya Sharma
 2018A7PS0221P      2018A7PS0287P       2018A7PS0367P 
 */

#include "gram_module.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void fill_Gnode(Grammar g,char *s)// utility function for filling the array element with the rule in the form of Linked List
{
    if(strlen(s)==0||s==NULL)
    return;
    
    char *temp=strtok(s," ");
   // printf("%s ",temp);
   g->nt=1;
   Gnode *x;
    x=g;
    x->next=NULL;
    x->symbol_name=(char*)malloc(strlen(temp)+1);
    strcpy(x->symbol_name,temp);
    temp=strtok(NULL," ");
    int i=0;
    while(temp)
    {  ++i;
      Gnode *t=(Gnode *)malloc(sizeof(Gnode));
      t->next=NULL;
      t->symbol_name=(char*)malloc(strlen(temp)+1);
      strcpy(t->symbol_name,temp);
      if(is_NT(t->symbol_name))
       {
           t->nt=1;
       }
       else
       {
           t->nt=0;
       }
       t->num_children=0;
      x->next=t;
      x=x->next;
      temp=strtok(NULL," ");
    }
    g->num_children=i;
    return;
}
void print_list(Gnode *t)
{   Gnode *l;
    l=t;
    while(l!=NULL)
    {
        printf("%s %d %d\n",l->symbol_name,l->nt,l->num_children);
        l=l->next;
        //printf(" 1 ");
    }
    //printf("list printed \n");
}
Grammar read_grammar(char * file_name,Grammar g)
{
    FILE *f=fopen(file_name,"r");
    if(!f)
    {
        printf("file not found\n");
        return NULL;
    }
        //printf("YES\n");

        g=(Gnode *)malloc(sizeof(Gnode)*70);
    
    char temp[1000];
    int i=0;
while(fgets(temp,sizeof(temp),f)!=NULL)
    {   int n=strlen(temp);
    //printf("%s \n",temp);
    temp[n-1]='\0';
    if(n<=1)
    continue;
      
    fill_Gnode(g+i,temp);
    //print_list(g+i);
    
    ++i;}
    //printf("out of read \n");
return g;
}
void add_empty(tokenStream s)
{
  while(s->next)
  {
      if(s->lexeme==";"&&s->next->lexeme[0]==';')
      { 
        TK_node *new;
        new=(TK_node *)malloc(sizeof(TK_node));
        new->lexeme="epsilon";
        new->line_num=s->line_num;
        new->next=s->next;
        s->next=new;
        new->tk_name="EMPTY_TK";
        break;
      }
      s=s->next;
  }
  return;
}