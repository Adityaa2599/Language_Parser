/* gram_module.c
 contains the following functions - 
    - tokenising sourcecode
    - traversing the parse tree
    - printing the parse tree
    - printing type expression table
 Saransh Jindal,    Siddharth Awasthi,  Aditya Sharma
 2018A7PS0221P      2018A7PS0287P       2018A7PS0367P 
 */

#include "gram_module.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char*placeholdertoken = "a";
tokenMap TK_MAP = NULL;
char*ID_TK = "ID_TK";
char*NUM_TK = "NUM_TK";
void initializeTokenMap(){
    FILE*f = fopen("init.txt","r");
    char*lex_buf = (char*)malloc(sizeof(char)*21);
    char*tk_buf = (char*)malloc(sizeof(char)*21);
    int is_term;
    TK_map_node* tail = TK_MAP;
    while(fscanf(f,"%s %s %d",lex_buf,tk_buf,&is_term)!=EOF){
        TK_map_node*new = (TK_map_node*)malloc(sizeof(TK_map_node));
        new->lexeme = (char*)malloc(sizeof(char)*(strlen(lex_buf)+1));
        strcpy(new->lexeme,lex_buf);
        new->tk_name = (char*)malloc(sizeof(char)*(strlen(tk_buf)+1));
        strcpy(new->tk_name,tk_buf);
        new->is_terminal = is_term;
        if(!TK_MAP){
            TK_MAP = new;
            tail = TK_MAP;
        }
        else{
            tail->next = new;
            tail = tail->next;
        }
    }

}

char*getTokenName(char*lex){
    TK_map_node*temp = TK_MAP;
    while(temp){
        if(strcmp(lex,temp->lexeme)==0){
            return temp->tk_name;
        }
        temp=temp->next;
    }
    if(isdigit(lex[0])){
        for(int i=0; i<strlen(lex);i++){
            if(!isdigit(lex[i])){
                return NULL;
            }
        }
        return NUM_TK;
    }
    else{
        return ID_TK;
    }
    return NULL;
    // return placeholdertoken;
}

TK_node*create_token(char*lex, int line_num){
    TK_node*node;
    node = (TK_node*)malloc(sizeof(TK_node));
    node->lexeme = (char*)malloc(sizeof(char)*(strlen(lex)+1));
    strcpy(node->lexeme, lex);
    node->tk_name = getTokenName(lex);
    node->line_num = line_num;
    node->next = NULL;
    return node;
}

void tokeniseSourcecode(char*filename, tokenStream *s){

    TK_node*s_tail = *s;
    FILE*f;
    f = fopen(filename,"r");
    if(!f){
        printf("error opening file\n");
        return;
    }
    char*buffer = (char*)malloc(sizeof(char)*1024);
    char*restore = buffer;
    char*temp = (char*)malloc(sizeof(char)*21);
    int line_num = 1;
    printf("Tokeinzing...\n");
    while(fgets(buffer,1024,f)!=NULL){
        while(buffer[0]==' '||buffer[0]=='\t'){
            buffer++;
        }
        int buf_len = strlen(buffer);
        while(buffer[buf_len-1]=='\n'||buffer[buf_len-1]=='\r'||buffer[buf_len-1]==' '||buffer[buf_len-1]=='\t'){
            buffer[buf_len-1] = '\0';
            buf_len--;
        }
        if(strlen(buffer)<1){
            continue;
        }
        // printf("%d %d\n",buf_len, strlen(buffer));
        //
        temp = strtok(buffer, " ");
        while(temp){
            // if(strlen(temp<1)){
            //     temp = strtok(NULL," ");
            //     continue;
            // }
            
            if(strcmp(temp,"..")==0){
                TK_node*new1 = create_token(".", line_num);
                TK_node*new2 = create_token(".",line_num);
                new1->next = new2;
                s_tail->next = new1;
                s_tail = new2;
                temp = strtok(NULL," ");
                continue;
            }

            TK_node*new = create_token(temp,line_num);
            if(!*s){
                *s = new;
                s_tail = *s;
            }
            else{
                s_tail->next = new;
                s_tail = s_tail->next;
            }
            temp = strtok(NULL," ");
        }
        line_num++;
        buffer = restore;
    }
    free(buffer);
    fclose(f);
}

/* helper funtions for debugging and testing*/
void printTokenList(tokenStream s){
    while(s){
        printf("name=%s, token=%s line_num=%d\n",s->lexeme,s->tk_name,s->line_num);
        s = s->next;
    }
}
void printTokenMap(){
    TK_map_node*temp = TK_MAP;
    while(temp){
        printf("name=%s, token=%s, term=%d\n",temp->lexeme, temp->tk_name,temp->is_terminal);
        temp = temp->next;
    }
}
table_element create_table_element(char*id, num_type nm_type, stat_dynam_ds stat_dynam, type_exp_ds type_exp) { 
  table_element temp;
//   temp = (table_element*)malloc(sizeof(table_element));
  strcpy(temp.identifier,id);

  temp.nm_type = nm_type;
  temp.stat_dynam = stat_dynam;
  temp.type_exp = type_exp;
  return temp;
}

void add_to_table(typeExpressionTable* tbl, table_element new){
    if(tbl->count<tbl->cap){
        tbl->rows[tbl->count++] = new;
        return;
    }
    tbl->cap+=10;
    tbl->rows = (table_element*)realloc(tbl->rows, sizeof(table_element)*tbl->cap);
    tbl->rows[tbl->count++] = new;
}

typeExpressionTable* createTETable(){
    typeExpressionTable*newtable = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
    newtable->cap =  10;
    newtable->count = 0;
    newtable->rows = (table_element*)malloc(sizeof(table_element)*newtable->cap);
    return newtable;
}

int Rect_Range(TreeNode *Node,rect_array_exp * exp, int i)
{  int ret=0;
  if(Node->num_children==7)
     {
      ret=Rect_Range(Node->children[6],exp,i+1);
     }
  
  if(Node->num_children<7)
    {   exp->range_rect_low=(char **)malloc(sizeof(char*)*(i+1));
        exp->range_rect_high=(char **)malloc(sizeof(char*)*(i+1));
        exp->dimension_rect=i+1;
        //printf("%d \n",i+1);
     }  
   TreeNode *L=Node->children[1];
   TreeNode *H=Node->children[4];
    char *l;
    if(L->children[0]->symbol_name[0]=='<')
     {
        ret=1;
        L=L->children[0];
        l=L->children[0]->lex_token->lexeme;
     }
    else
     {
       l=L->children[0]->lex_token->lexeme; 
     }
    char *h;
    if(H->children[0]->symbol_name[0]=='<')
     {
        ret=1;
        H=H->children[0];
        h=H->children[0]->lex_token->lexeme;
     }
    else
     {
       h=H->children[0]->lex_token->lexeme; 
     }
   
   
   (exp->range_rect_low)[i]=l;
   (exp->range_rect_high)[i]=h;
  return ret;
}

int count_indices(TreeNode*index_list){
    int count=1;
    while(index_list->num_children==2){
        count++;
        index_list = index_list->children[1];
    }
    return count;
}

table_element getTEfromID(char*id,typeExpressionTable*teteable){
    table_element te;
    for(int i=0;i<teteable->count;i++){
        if(strcmp(teteable->rows[i].identifier,id)==0){
            te = teteable->rows[i];
           // printf("%s \n",te.identifier);
           
            break;
        }
    }
    return te;
}

int check_term(TreeNode*term, typeExpressionTable*tetable){
    // 0 int, 1 real, 2 bool
    // -1 for mismatch
    
    if(strcmp(term->symbol_name,"<term>")==0){
        int l;
       
        TreeNode*factor = term->children[0];
        // printf("ya\n");
        //printf("%d \n",factor->num_children);
        if(strcmp(factor->children[0]->symbol_name, "NUM_TK")==0){
            //printf("NUMtk\n");
            l = 0;
            //printf("%s \n",factor->children[0]->lex_token->lexeme);
            //printf("L=-1\n");
        }
        else{//printf("yy\n");
            TreeNode*var_node = factor->children[0];
            //printf("%s\n",var_node->children[0]->symbol_name);
            if(var_node->children[0]->symbol_name[2]=='a'){
                TreeNode*id_tk_node = var_node->children[0]->children[0];
                //printf("%s \n",id_tk_node->lex_token->lexeme);
                table_element id_tk_te = getTEfromID(id_tk_node->lex_token->lexeme,tetable);
                if(id_tk_te.nm_type==PRIMITIVE){
                    if(id_tk_te.type_exp.primitive_id==INTEGER){
                        l=0;
                    }
                    else if(id_tk_te.type_exp.primitive_id==REAL){
                        l=1;
                    }
                    else{
                        //printf("yy\n");
                        return -1;
                    }
                }
                else{
                    //printf("returning -1 \n");
                    return -1;
                }
            }
            else{
                //printf("array in term\n");
                //printf("%s\n",var_node->children[0]->children[0]->children[0]->lex_token->lexeme);
                table_element arr_var_te = getTEfromID(var_node->children[0]->children[0]->children[0]->lex_token->lexeme,tetable);
                int count = count_indices(var_node->children[0]->children[2]);
                //printf("yes\n");
                if(arr_var_te.nm_type==RECTANGULAR){
                    if(count != arr_var_te.type_exp.rect_array_id.dimension_rect){
                        //raise error
                       // printf("raising error\n");
                        TreeNode* arr_tk_node= var_node->children[0]->children[0]->children[0];
                        print_error(arr_tk_node->lex_token->line_num, arr_var_te.stat_dynam,"***",arr_tk_node->lex_token->lexeme,INTEGER,"***",INTEGER,arr_tk_node->depth,"INVALID ARRAY ASSIGNMENT");
                        return -1;
                    }
                }
                else{
                    if(count != arr_var_te.type_exp.jagged_array_id.dimension_jagged){
                        TreeNode* arr_tk_node= var_node->children[0]->children[0]->children[0];
                        print_error(arr_tk_node->lex_token->line_num, arr_var_te.stat_dynam,"***",arr_tk_node->lex_token->lexeme,INTEGER,"***",INTEGER,arr_tk_node->depth,"INVALID ARRAY ASSIGNMENT");
                        return -1;
                    }
                }
                l=0;
            }

        }
        //printf("Yes\n");
        if(term->num_children>1){
           
            int r = check_term(term->children[2],tetable);
            //printf("%d \n",r);
            char*operator = term->children[1]->children[0]->symbol_name;
            if(l!=r){
                TreeNode*last = term;
                while(last->num_children>0){
                    last = last->children[0];
                }
                print_error(last->lex_token->line_num, 1,term->children[1]->symbol_name,last->lex_token->lexeme,l,"***",r,term->depth,"DATATYPE MISMATCH");
                //printf("Yo");
                return -1;
            }
            if(strcmp(operator,"OP_DIV")==0){
                return 1;
            }
        }
       // printf("returning l = %d ",l);
       
        return l;
    }
    // else its a boolean term
    else{
        int l=2;
        char*l_id = term->children[0]->children[0]->children[0]->lex_token->lexeme;
        table_element l_te = getTEfromID(l_id,tetable);
        //printf("%s \n",term->children[1]->symbol_name);
        if(l_te.nm_type!=PRIMITIVE || l_te.type_exp.primitive_id!=BOOLEAN){
            print_error(term->children[0]->children[0]->children[0]->lex_token->line_num,DYNAMIC,term->children[1]->lex_token->lexeme,l_id,l_te.type_exp.primitive_id,"***",BOOLEAN,term->children[0]->children[0]->children[0]->depth,"INVALID OPERAND TYPE MISMATCH");
            return -1;
        }
        if(term->num_children>1){
            int r = check_term(term->children[2],tetable);
            if(l!=r){
                            print_error(term->children[0]->children[0]->children[0]->lex_token->line_num,DYNAMIC,term->children[1]->lex_token->lexeme,l_id,l_te.type_exp.primitive_id,"***",BOOLEAN,term->children[0]->children[0]->children[0]->depth,"INVALID EXPRESSION");

                return -1;
            }
        }
        return l;
    }
    return -1;
}

int check_expression(TreeNode*exp, typeExpressionTable*tetable){
    
    if(strcmp(exp->symbol_name, "<Arithmetic_expression>")==0){
        //printf("arithmetic expression\n");
        if(exp->num_children>1){
            //printf("Yo\n");
            int l = check_term(exp->children[0],tetable);
            //printf("l = %d \n",l);
            int r = check_expression(exp->children[2],tetable);
            //
            if(l==r&&l!=-1){
                //printf("%d %d \n",l,r);
                return l;
            }
            else{
                TreeNode*last = exp;
                while(last->children>0){
                    last = last->children[0];
                }
                print_error(last->lex_token->line_num, 1,exp->children[1]->children[0]->symbol_name, last->lex_token->lexeme,l,"***",r,exp->depth,"INVALID EXPRESSION ASSIGNMENT");
            }
        }
        else{
            //printf("%s\n",exp->children[0]->symbol_name);
            
            //printf("%d \n",x);
            //printf("final yes\n");
            return check_term(exp->children[0],tetable);
        }
    }
    else{
        //printf("%d \n",exp->num_children);
        if(exp->num_children>1){
            
            int l = check_term(exp->children[0],tetable);
            int r = check_expression(exp->children[2],tetable);
            if(l!=r){
                //printf("\n\nmismatch %d %d \n",l,r);
                TreeNode*last = exp;
                while(last->children>0){
                    last = last->children[0];
                }
                print_error(last->lex_token->line_num, 1,exp->children[1]->children[0]->symbol_name, last->lex_token->lexeme,l,"***",r,exp->depth,"INVALID EXPRESSION ASSIGNMENT");
            }
            else{
                return l;
            }
        }
        else{
            return check_term(exp->children[0],tetable);
        }
    }
    return -1;
}


int checkTypeExpression(TreeNode*lhs,TreeNode*rhs, typeExpressionTable* teteable){
    //printf("checking type expression\n");
    table_element te_lhs;
    char* lhs_id;
    
     if(strcmp(lhs->children[0]->children[0]->symbol_name,"<Array_element>")!=0)
    lhs_id=lhs->children[0]->children[0]->children[0]->lex_token->lexeme;
    else
    {  
      lhs_id=lhs->children[0]->children[0]->children[0]->children[0]->lex_token->lexeme;
    }
    // lhs->variable/arrayelement->var_name->ID_TK->tokennode->lexeme
    int type_lhs;
    //get TE for LHS
    //printf("Yes");
    te_lhs = getTEfromID(lhs_id,teteable);
    /* lhs->te = te_lhs;*************INSERT LINE TO ASSIGN TYPE EXPRESSION TO NODE************* */
    // check if element access indices are equal to dimensions
       //printf("%s\n",lhs->children[0]->);
    if(strcmp(lhs->children[0]->children[0]->symbol_name,"<Array_element>")==0){
       
        int count = count_indices(lhs->children[0]->children[0]->children[2]);
        
        if(te_lhs.nm_type==RECTANGULAR){
            
            if(count != te_lhs.type_exp.rect_array_id.dimension_rect){
                TreeNode*lhstk = lhs->children[0]->children[0]->children[0]->children[0];
                print_error(lhstk->lex_token->line_num,1,"***",lhs_id,RECTANGULAR,"***",INTEGER,lhs->depth,"INVALID ARRAY ACCESS DIMENSION MISMATCH");
                
                return 0;
            }
        }
        else{
            
            if(count != te_lhs.type_exp.jagged_array_id.dimension_jagged){
                TreeNode*lhstk = lhs->children[0]->children[0]->children[0];
                print_error(lhstk->lex_token->line_num,1,"***",lhs_id,JAGGED,"***",INTEGER,lhs->depth,"INVALID ARRAY ACCESS AND ASSIGNMENT");
                return 0;
            }
        }
        type_lhs = 0;
    }
    else{ 
        //printf(" not array element\n");
        if(te_lhs.type_exp.primitive_id==INTEGER){
            type_lhs = 0;
            //printf(" yes");
        }
        else if(te_lhs.type_exp.primitive_id==REAL){
            type_lhs = 1;
        }
        else{
            type_lhs=2;
        }
    }

    TreeNode*exp = rhs->children[0];
    int exp_type = check_expression(exp,teteable);
     //printf("exp =%d \n",type_lhs);
    //printf("%d %d exp type\n",exp_type,type_lhs);
     //printf("%d \n",exp_type);
    
    if(exp_type!=type_lhs){

        TreeNode*lhstk = lhs->children[0]->children[0]->children[0];
        if(strcmp(lhs->children[0]->children[0]->symbol_name,"<Array_element>")!=0)
        lhstk=lhs->children[0]->children[0]->children[0];
    else
    {  
      lhstk=lhs->children[0]->children[0]->children[0]->children[0];
    }
        //printf("printing this block \n\n");
        print_error(lhstk->lex_token->line_num, 1,"=",lhs_id,te_lhs.type_exp.primitive_id,"***",te_lhs.type_exp.primitive_id, lhs->depth,"INVALID ASSIGMNENT");
        return 0;
    }
    return 1;

}

void traverseTree(TreeNode* tree, typeExpressionTable*tetable){
    // if declare - use to populate
    // else if non terminal - traverse over each child
    if(!tree){
        return ;
    }
      //populate_table(tree->children[0]->children[0],tetable);
    if(tree->symbol_name[0]=='<'){
        if(strcmp(tree->symbol_name,"<Declare>")==0){
            populate_table(tree,tetable);
        }
        else if(strcmp(tree->symbol_name,"<Assignment_Statements>")==0){
            TreeNode*lhs = tree->children[0];
            TreeNode*rhs = tree->children[2];
            int valid = 0;
            valid = checkTypeExpression(lhs,rhs,tetable);
            //printf("%d \n",valid);
            if(!valid){
                //raise error
                
                //printf("type mismatch at line %d \n",tree->children[1]->lex_token->line_num);
            }
            if(tree->num_children==4){
                traverseTree(tree->children[3], tetable);
            }   
        }
        else{
            for(int i=0; i<tree->num_children;i++){
                traverseTree(tree->children[i], tetable);
            }
        }
    }
    return;
}
int check_row(TreeNode *Node)
{
 if(Node->num_children==1)
  return 1;
  else
  {
      return 0;
  }
  
}
int Row_2d(TreeNode *Node)
{
  int ret;
  ret=atoi(Node->children[6]->lex_token->lexeme);
  TreeNode *var=Node->children[10];
  int count=0;
  while(var&&count<=ret)
  {
      ++count;
      if(check_row(var->children[0])==0)
      {     print_error(Node->children[0]->lex_token->line_num,0,"***","***",INTEGER,"***",INTEGER,0,"2D JA Type definition error");
         // printf("Type definition error line %d ",Node->children[0]->lex_token->line_num);
          return -1;
      }
      if(var->num_children>1)
      var=var->children[2];
      else
      {
          var=NULL;
      }
      

  }
  if(count!=ret)
  {
       print_error(Node->children[0]->lex_token->line_num,0,"***","***",INTEGER,"***",INTEGER,Node->depth,"2D JA Type definition error");
          return -1;
  }
  else
  {
      return ret;
  }
  
  
}
int check_row_3d(TreeNode *Node )
{ int ret=0;
 while(Node)
 {
     ++ret;
     if(Node->num_children>1)
     Node=Node->children[1];
     else
     {
         Node=NULL;
     }
     
 }
 return ret;
}
int Row_3d(TreeNode * Node,int **row_ar)
{
  int ret;
  ret=atoi(Node->children[6]->lex_token->lexeme);
  (*row_ar)=(int*)malloc(sizeof(int)*ret);
  TreeNode *var=Node->children[10];
  int count=0;
  while(var&&count<=ret)
  {   //printf("RUN ");
      
      int val=check_row_3d(var->children[0]);
      ++count;
      (*row_ar)[count-1]=val;
      if(var->num_children>1)
      {var=var->children[2];}
      else
      {
          var=NULL;
      }
  }
  //printf("\n");
  //printf("%d \n",count);
  if(count!=ret)
  {
       print_error(Node->children[0]->lex_token->line_num,0,"***","***",INTEGER,"***",INTEGER,0,"3D JA Size Miss Match");
          //return -1;
  }
  
      return ret;
  
}

void populate_table(TreeNode*dec_node, typeExpressionTable* tetable){
    //traverse node
    //fill relevant fields
    TreeNode*dec_node_copy = dec_node;
    table_element new_row;
    if(strcmp(dec_node->children[0]->symbol_name,"<Primitive_Declaration>")==0){
        new_row.nm_type = PRIMITIVE;
        new_row.stat_dynam = NOT_APPLICABLE;
        dec_node = dec_node->children[0];
        char*type_tk = dec_node->children[2]->children[0]->symbol_name;
        if(strcmp(type_tk,"TK_INT")==0){
            new_row.type_exp.primitive_id = INTEGER;
        }
        else if(strcmp(type_tk,"TK_BOOL")==0){
            new_row.type_exp.primitive_id = BOOLEAN;
        }
        else{
            new_row.type_exp.primitive_id = REAL;
        }
        dec_node = dec_node->children[0];
        if(strcmp(dec_node->children[1]->symbol_name,"<Variable_Name>")==0){
            strcpy(new_row.identifier,dec_node->children[1]->children[0]->lex_token->lexeme);
            dec_node->type_exp_tn = new_row.type_exp;
            add_to_table(tetable,new_row);
            dec_node_copy->type_exp_tn = new_row.type_exp;
            return;
        }
        else{
            dec_node = dec_node->children[4];
            while(dec_node){
                TreeNode* var = dec_node->children[0];
                char *var_name = var->children[0]->lex_token->lexeme;
                table_element next_list_var = create_table_element(var_name,PRIMITIVE,NOT_APPLICABLE,new_row.type_exp);
                dec_node->type_exp_tn = new_row.type_exp;
                add_to_table(tetable,next_list_var);
                if(dec_node->num_children>1){
                    dec_node = dec_node->children[1];
                }
                else{
                    dec_node = NULL;
                }
            }
            dec_node_copy->type_exp_tn = new_row.type_exp;
            return;
        }
    }
    if(strcmp(dec_node->children[0]->symbol_name,"<Array_Declaration>")==0){
       dec_node=dec_node->children[0];

      if(strcmp(dec_node->children[0]->symbol_name,"<Rectangular_Array>")==0){
          dec_node=dec_node->children[0];
           TreeNode *var=dec_node->children[3];
           rect_array_exp * exp;
           exp=(rect_array_exp *)malloc(sizeof(rect_array_exp));

            int check=Rect_Range(var,exp,0);
            if(check==1)
            new_row.stat_dynam=DYNAMIC;
            else
            {
                new_row.stat_dynam=STATIC;
            }
            
           new_row.type_exp.rect_array_id=*exp;
          
          
          
          
          
          //traversing variable ID
          dec_node=dec_node->children[0];
          
          if(strcmp(dec_node->children[1]->symbol_name,"TK_LIST")==0){
              dec_node = dec_node->children[4];
              while(dec_node->num_children == 2){
                  table_element next_list_var = create_table_element(dec_node->children[0]->children[0]->lex_token->lexeme, RECTANGULAR, new_row.stat_dynam,new_row.type_exp );
                  dec_node->type_exp_tn = new_row.type_exp;
                  add_to_table(tetable,next_list_var);
                  dec_node = dec_node->children[1];
              }
              table_element next_list_var = create_table_element(dec_node->children[0]->children[0]->lex_token->lexeme, RECTANGULAR, new_row.stat_dynam,new_row.type_exp  );
              dec_node->type_exp_tn = new_row.type_exp;
              add_to_table(tetable,next_list_var);  
          }
          else{
              dec_node = dec_node->children[1];
              table_element next_list_var = create_table_element(dec_node->children[0]->children[0]->lex_token->lexeme, RECTANGULAR,new_row.stat_dynam,new_row.type_exp  );
              dec_node->type_exp_tn = new_row.type_exp;
              add_to_table(tetable,next_list_var);  
          }
        dec_node_copy->type_exp_tn = new_row.type_exp;
      }
      else
      { //printf("inside jagged");
           dec_node=dec_node->children[0];
          new_row.nm_type=JAGGED;
          new_row.stat_dynam=STATIC;
           //printf("%s \n",dec_node->symbol_name);
           if(strcmp(dec_node->children[15]->symbol_name,"<ROW_LIST2d>")==0)
             {  new_row.type_exp.jagged_array_id.dimension_jagged=2;  

                char* t=dec_node->children[5]->lex_token->lexeme;
                 new_row.type_exp.jagged_array_id.r1_low=atoi(t);
                  
                 t=dec_node->children[8]->lex_token->lexeme;
                 new_row.type_exp.jagged_array_id.r1_high=atoi(t);
                  //printf("%s \n",t);
                 TreeNode *var=dec_node->children[15];
                  int size_of_2D=new_row.type_exp.jagged_array_id.r1_high-new_row.type_exp.jagged_array_id.r1_low+1;
                  int num_rows=0;
                  
                 new_row.type_exp.jagged_array_id.r2=(int *)malloc(sizeof(int)*size_of_2D);
               while(var&&num_rows<size_of_2D)
              {   ++num_rows;
              
                 int row_size=Row_2d(var->children[0]);
                 //printf("%d \n",num_rows);
                 if(row_size==-1)
                  {  new_row.error=1;
                      break;//means there was an error in one of the rows TODO add 
                  }
                  new_row.type_exp.jagged_array_id.r2[num_rows-1]=row_size;
                   if(var->num_children>1)
                    var=var->children[1];
                   else
                   {
                       var=NULL;
                   }
                   
              }
              if(num_rows!=size_of_2D)
              {
                  print_error(dec_node->children[1]->lex_token->line_num,0,"***","***",INTEGER,"***",INTEGER,0,"2D JA declaration Error");
              }
                dec_node = dec_node->children[0];
                 if(strcmp(dec_node->children[1]->symbol_name,"<Variable_Name>")==0){
                      strcpy(new_row.identifier,dec_node->children[1]->children[0]->lex_token->lexeme);
                     dec_node->type_exp_tn = new_row.type_exp;
                     add_to_table(tetable,new_row);
                     dec_node_copy->type_exp_tn = new_row.type_exp;
                     return;
                 }
             else{
              dec_node = dec_node->children[4];
              while(dec_node){
                  TreeNode* var = dec_node->children[0];
                 char *var_name = var->children[0]->lex_token->lexeme;
                 table_element next_list_var = create_table_element(var_name,JAGGED,STATIC,new_row.type_exp);
                 dec_node->type_exp_tn = new_row.type_exp;
                 add_to_table(tetable,next_list_var);
                 if(dec_node->num_children>1){
                     dec_node = dec_node->children[1];
                     }    
                 else{
                    dec_node = NULL;
                    }
                 }
                 dec_node_copy->type_exp_tn = new_row.type_exp;
                 return;
             }  
           }
           else//3d jaddeg array
           {
                new_row.type_exp.jagged_array_id.dimension_jagged=3;  

                char* t=dec_node->children[5]->lex_token->lexeme;
                new_row.type_exp.jagged_array_id.r1_low=atoi(t);
                  
                t=dec_node->children[8]->lex_token->lexeme;
                new_row.type_exp.jagged_array_id.r1_high=atoi(t);

                int size_of_2D=new_row.type_exp.jagged_array_id.r1_high-new_row.type_exp.jagged_array_id.r1_low+1;
                new_row.type_exp.jagged_array_id.r2=(int *)malloc(sizeof(int)*size_of_2D);
                new_row.type_exp.jagged_array_id.r3=(int **)malloc(sizeof(int*)*size_of_2D);
                 int **row_ar=new_row.type_exp.jagged_array_id.r3;
                TreeNode *var=dec_node->children[17];
                int num_rows=0;
                while(var&&num_rows<size_of_2D)
                 {++num_rows;
                  int row_size=Row_3d(var->children[0],row_ar+num_rows-1);
                   if(row_size==-1)
                   {   new_row.error=1;
                       break;//means there was an error in one of the rows TODO add 
                   }
                   new_row.type_exp.jagged_array_id.r2[num_rows-1]=row_size;
                   if(var->num_children>1)
                    var=var->children[1];
                   else
                   {
                       var=NULL;
                   }
                 }
                dec_node_copy->type_exp_tn = new_row.type_exp;
              if(num_rows!=size_of_2D)
               {
                  print_error(dec_node->children[1]->lex_token->line_num,0,"***","***",INTEGER,"***",INTEGER,0,"3D JA declaration Error");
               }
               dec_node = dec_node->children[0];
                 if(strcmp(dec_node->children[1]->symbol_name,"<Variable_Name>")==0){
                      strcpy(new_row.identifier,dec_node->children[1]->children[0]->lex_token->lexeme);
                     dec_node->type_exp_tn = new_row.type_exp;
                     add_to_table(tetable,new_row);
                     return;
                    }

                  else{
                       dec_node = dec_node->children[4];
                     while(dec_node){
                     TreeNode* var = dec_node->children[0];
                     char *var_name = var->children[0]->lex_token->lexeme;
                     table_element next_list_var = create_table_element(var_name,JAGGED,STATIC,new_row.type_exp);
                     dec_node->type_exp_tn = next_list_var.type_exp;
                     add_to_table(tetable,next_list_var);
                     if(dec_node->num_children>1){
                            dec_node = dec_node->children[1];
                       }    
                     else{
                            dec_node = NULL;
                        }
                 }
                 dec_node_copy->type_exp_tn = new_row.type_exp;
                 return;
              }
            }
           
        }
      
    }
    
}

void print_tree_node(TreeNode *Node){

    printf("%s %s %s %s %s \t\t%s \t\t%s \n","Symbol Name","Terminal/Non Terminal","Name of Lexeme","Line Number","Grammar Rule","Depth of Node","Type Expression");

    if(Node->lex_token==NULL){      //for non terminal
        printf("%s %s %s %s %d \t",Node->symbol_name,"Non Terminal","Not a leaf node","Not a leaf node",Node->depth); 
        int i=0;
        while(i<Node->num_children){        
            printf("%s ",Node->children[i]->symbol_name); ++i; 
        }
    }
    else{       //for terminal
        printf("%s %s %s %d %d \t%s ",Node->symbol_name,"Terminal",Node->lex_token->lexeme,Node->lex_token->line_num,Node->depth,"Not a non terminal"); 
    }
    printf("\t");

    if(strcmp(Node->symbol_name,"<Declare>")==0){       //type expression for <Declare> node(optional)
        
        if(strcmp(Node->children[0]->symbol_name,"<Primitve_Declaration>")==0){     //type expression for primitive data type
            printf("<type = %s > ",Node->type_exp_tn.primitive_id);
        }

        else if(strcmp(Node->children[0]->children[0]->symbol_name,"<Rectangular_Array>")==0){       //type expression for retangular data type

            printf("<type=rectangularArray,dimensions= %d,",Node->type_exp_tn.rect_array_id.dimension_rect);
            for(int j=0;j<Node->type_exp_tn.rect_array_id.dimension_rect;j++){
                printf("range_R%d=(%s,%s), ",j+1,Node->type_exp_tn.rect_array_id.range_rect_low[j],Node->type_exp_tn.rect_array_id.range_rect_high[j]);
            }
            printf("basicElementType = integer> ");

        }

        else{       //type expression for jagged array
            int jagged_dim = Node->type_exp_tn.jagged_array_id.dimension_jagged;
            int dim_row2 = Node->type_exp_tn.jagged_array_id.r1_high - Node->type_exp_tn.jagged_array_id.r1_low + 1;
            int* r2_arr = Node->type_exp_tn.jagged_array_id.r2;
            int** r3_arr = Node->type_exp_tn.jagged_array_id.r3;

            printf("<type=jaggedArray,dimensions= %d,range_R1=(%d,%d), range_R2 =(",jagged_dim,Node->type_exp_tn.jagged_array_id.r1_low,Node->type_exp_tn.jagged_array_id.r1_high);
            
            if(jagged_dim==2){
                for(int k=0;k<dim_row2;k++){
                    if(k!=0)
                        printf(", ");
                    printf("%d",r2_arr[k]);
                }
            }

            if(jagged_dim==3){
                for(int j=0;j<dim_row2;j++){
                    if(j!=0)
                        printf(", ");
                    printf("%d[",r2_arr[j]);
                    for(int k=0; k<r2_arr[j]; k++){
                        if(k!=0)
                            printf(", ");
                        printf("%d",r3_arr[j][k]);
                    }
                    printf("]");
                }
            }
            printf("), basicElementType = integer> ");       
        }
    }
    else{       //if the node is not a <Declare> node
        printf("Not Applicable ");
    }

    printf("\n");
}

void print_ParseTree(TreeNode * Node){
    
    TreeNode* temp = Node;
     
    print_tree_node(temp); 

    // then recur from left to right subtree
    int i=0;
    while(i<temp->num_children){
        print_ParseTree(temp->children[i]); ++i; 
    }
}

void printTETable(typeExpressionTable T){
    char* data_type_arr[] = {"Integer", "Real", "Boolean"};
    char* field3_arr[] = {"Static", "Dynamic", "Not Applicable"};

    printf("%s %s %s %s \n","FIELD1","FIELD2","FIELD3   ","FIELD4");
    
    int i=0;
    int count_rows = T.count;
    table_element* temp_table_element=T.rows;

    while(i<count_rows){
        char range_exp_rect[50];

        if(temp_table_element[i].nm_type==0){
            printf("%s %s %s \t\t <type = %s > \n",temp_table_element[i].identifier,"Primitive","Not Applicable",data_type_arr[temp_table_element[i].type_exp.primitive_id]);
        }

        if(temp_table_element[i].nm_type==1){
            
            printf("%s %s %s \t\t <type=rectangularArray,dimensions= %d,",temp_table_element[i].identifier,"Rectangular",field3_arr[temp_table_element->stat_dynam],temp_table_element[i].type_exp.rect_array_id.dimension_rect);
            
            for(int j=0;j<temp_table_element[i].type_exp.rect_array_id.dimension_rect;j++){
                printf("range_R%d=(%s,%s), ",j+1,temp_table_element[i].type_exp.rect_array_id.range_rect_low[j],temp_table_element[i].type_exp.rect_array_id.range_rect_high[j]);
            }
            printf("basicElementType = integer> \n");
        }
        if(temp_table_element[i].nm_type==2){
            int jagged_dim = temp_table_element[i].type_exp.jagged_array_id.dimension_jagged;
            int dim_row2 = temp_table_element[i].type_exp.jagged_array_id.r1_high - temp_table_element[i].type_exp.jagged_array_id.r1_low + 1;
            int* r2_arr = temp_table_element[i].type_exp.jagged_array_id.r2;
            int** r3_arr = temp_table_element[i].type_exp.jagged_array_id.r3;
            printf("%s %s %s \t\t <type=jaggedArray,dimensions= %d,range_R1=(%d,%d), range_R2 =(",temp_table_element[i].identifier,"Jagged","Not Applicable",jagged_dim,temp_table_element[i].type_exp.jagged_array_id.r1_low,temp_table_element[i].type_exp.jagged_array_id.r1_high);   
            if(jagged_dim==2){
                for(int k=0;k<dim_row2;k++){
                    if(k!=0)
                        printf(", ");
                    printf("%d",r2_arr[k]);
                }
            }
            if(jagged_dim==3){
                for(int j=0;j<dim_row2;j++){
                    if(j!=0)
                        printf(", ");
                    printf("%d[",r2_arr[j]);
                    for(int k=0; k<r2_arr[j]; k++){
                        if(k!=0)
                            printf(", ");
                        printf("%d",r3_arr[j][k]);
                    }
                    printf("]");
                }
            }
            printf("), basicElementType = integer> \n");
        }
        i++;
    }
}

void print_error(int line_num,int stat_type,char *operator,char *fop,data_type f,char *soperand,data_type s,int depth,char *message)
{
  printf("line number = %d ;",line_num);
  if(stat_type==0)
  {
      printf(" type = Declarative Statement ;");
  }
  else{
      printf(" type = Assignment Statement ;");
   printf(" operator = %s ;",operator);
   printf(" First Operand = %s ;",fop);
   printf(" First Operand type = ");
    switch (f)
    {
    case INTEGER:
         printf("INTEGER ;");
        break;
    case REAL :
         printf("REAL ;");
         break;
    case BOOLEAN :
         printf("BOOLEAN ;");
         break;
    }
    printf(" Second Operand = %s ;",fop);
   printf(" Second Operand type = ");
    switch (s){
    case INTEGER:
         printf("INTEGER ;");
        break;
    case REAL :
         printf("REAL ;");
         break;
    case BOOLEAN :
         printf("BOOLEAN ;");
         break;
    }
    }
    printf(" Depth = %d ;",depth);
    printf(" %s \n",message);
}
