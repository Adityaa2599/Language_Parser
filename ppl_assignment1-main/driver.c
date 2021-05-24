/* driver.c
 Saransh Jindal,    Siddharth Awasthi,  Aditya Sharma
 2018A7PS0221P      2018A7PS0287P       2018A7PS0367P 
 */

#include "gram_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){

    int two=0;
    int three=0;
    int four=0;
    initializeTokenMap();
    char*filename = "sourcecode.txt";
    tokenStream *s = (tokenStream*)malloc(sizeof(tokenStream));
    *s = NULL;
    tokeniseSourcecode(filename,s);
    printf("Tokenized\n");
    printf("Reading Grammar...");
    Grammar g;
    char *gram_filename = "grammar.txt";
    g = read_grammar(gram_filename,g);
    printf("Done\n");
    TreeNode*parsetree;
    typeExpressionTable*tetable = createTETable();
    while(1){
        printf("Option 0: Exit\n");
        printf("Option 1: Create parse tree\n");
        printf("Option 2: Traverse the parse tree\n");
        printf("Option 3: Print parse tree\n");
        printf("Option 4: Print typeExpressionTable\n");
        int option;
        printf("Enter Choice: ");
        scanf("%d", &option);
        printf("\n");
        if(option==0){
            break;
        }
        switch(option){
            case 1:
                printf("Creating parse tree...");
                parsetree = createParseTree(g,s);
                printf(" Done\n");
                //two = 1;
                break;
            case 2:
                printf("Creating parse tree...");
                parsetree = createParseTree(g,s);
                printf(" Done\n");
                printf("Traversing Tree... ");
                traverseTree(parsetree,tetable);
                printf("Done\n");
                three=1;
                break;
                
                
                
            case 3:
                printf("Creating parse tree...");
                parsetree = createParseTree(g,s);
                printf(" Done\n");
                printf("Printing Parse Tree:\n\n");
                print_ParseTree(parsetree);
                printf("\n");
        
                break;
            case 4:
                printf("Creating parse tree...");
                parsetree = createParseTree(g,s);
                printf(" Done\n");
                printf("Traversing Tree... \n");
                traverseTree(parsetree,tetable);
                printf("Done\n");
                printf("Printing Type Expression Table:\n\n");
                printTETable(*tetable);
             
            break;
            case 5:
                printf("Invalid choice, try again\n");
        }
    }
}