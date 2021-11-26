#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"./include/syntax.h"
#include"./syntax.tab.h"
#include"semantic/semantic.h"
#include"symtab/symtab.h"
#include"intercode/ir.h"

void yyrestart(FILE *);



int main(int argc, char** argv){

    if(argc <= 1){
        printf("Too few argument\n");
        return 1;
    }

    error_cnt = 0;

    FILE *f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }

    //restart in file f
    yyrestart(f);

    

    //parse grammer
    yyparse();
    assert(root->children);
    assert(!(root->next));
    //print_tree(root, 0);

    
    Program0(root);
    IRProgram0(root);
    printIC();

    return 0;
}
