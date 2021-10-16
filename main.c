#include<stdio.h>
#include<stdlib.h>
#include"dec.h"
#include"assert.h"
#include"syntax.tab.h"


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

    //printf("Start parsing\n");

    //parse grammer
    yyparse();
    assert(root->children);
    assert(!(root->next));
    print_tree(root, 0);

    return 0;
}
