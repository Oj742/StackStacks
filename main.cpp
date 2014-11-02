/*
Oj742
StackStacks Interpreter
*/
#include "Stack.h"

using namespace std;

void test_stack()
{
    program test;
    char in[128];

    cout << "StackStacks testing thing\n" << "  type 'quit' to quit\n  '...' means there's only empty stacks past that point\n";
    cout << "   stack_id(stack_size)\n\n\n";
    do
    {
        cin.getline(in, 255);

        if(!strcmp(in, "push")) test.sc_push();
        else if(!strcmp(in, "pop")) test.sc_pop();
        else if(!strcmp(in, "tuck")) test.sc_tuck();
        else if(!strcmp(in, "nip")) test.sc_nip();
        else if(!strcmp(in, "\\down")) test.sc_Mdown();
        else if(!strcmp(in, "\\up")) test.sc_Mup();
        else if(!strcmp(in, "\\root")) test.sc_Mroot();
        else if(!strcmp(in, "swap")) test.sc_swap();
        else if(!strcmp(in, "dup")) test.sc_dup();
        else if(!strcmp(in, "cdup")) test.sc_cdup();
        else if(!strcmp(in, "clear")) test.sc_clear();
        else if(!strcmp(in, ".size")) test.sc_Psize();
        else if(!strcmp(in, ".level")) test.sc_Plevel();
        else if(!strcmp(in, "pack")) test.sc_pack();
        else if(!strcmp(in, "unpack")) test.sc_unpack();
        else if(!strcmp(in, "\\leaf")) test.sc_Mleaf();
        else if(!strcmp(in, "-pack")) test.sc_pack();
        else if(!strcmp(in, ".fail")) test.sc_Pfail();
        else if(!strcmp(in, ".test")) test.sc_Ptest();
        else if(!strcmp(in, "add")) test.sc_add();
        else if(!strcmp(in, "cat")) test.sc_cat();
        else if(!strcmp(in, "dup2")) test.sc_dup2();
        else if(!strcmp(in, "inc")) test.sc_inc();
        else if(!strcmp(in, "dec")) test.sc_dec();
        else if(!strcmp(in, "\\goto")) test.sc_Mgoto();
        else if(!strcmp(in, "geta")) test.sc_geta();
        else if(!strcmp(in, "gets")) test.sc_gets();
        else if(!strcmp(in, "outa")) test.sc_outa();
        else if(!strcmp(in, "outi")) test.sc_outi();
        else if(!strcmp(in, "outc")) test.sc_outc();
        else if(!strcmp(in, "outs")) test.sc_outs();
        else if(!strcmp(in, "not")) test.sc_not();
        else if(!strcmp(in, "test")) test.sc_test();
        else if(!strcmp(in, "pop2")) test.sc_pop2();
        else if(!strcmp(in, "eq")) test.sc_eq();
        else if(!strcmp(in, "ls")) test.sc_ls();
        else if(!strcmp(in, "grt")) test.sc_grt();
        else if(!strcmp(in, "neq")) test.sc_neq();
        else if(!strcmp(in, "lseq")) test.sc_lseq();
        else if(!strcmp(in, "grteq")) test.sc_grteq();
        else if(!strcmp(in, "swap2")) test.sc_swap2();
        else if(!strcmp(in, "over")) test.sc_over();
        else if(!strcmp(in, "-over")) test.sc_Rover();
        else if(!strcmp(in, "cycle")) test.sc_cycle();
        else if(!strcmp(in, "-cycle")) test.sc_Rcycle();
        else if(!strcmp(in, "cover")) test.sc_cover();
        else if(!strcmp(in, "-cover")) test.sc_Rcover();
        else if(!strcmp(in, "over2")) test.sc_over2();
        else if(!strcmp(in, "-over2")) test.sc_Rover2();
        else if(!strcmp(in, "shftl")) test.sc_shftl();
        else if(!strcmp(in, "shftr")) test.sc_shftr();
        else if(!strcmp(in, "xchg")) test.sc_xchg();
        else if(!strcmp(in, "take")) test.sc_take();
        else if(!strcmp(in, ".leaf")) test.sc_Pleaf();
        else if(!strcmp(in, "rot")) test.sc_rot();
        else if(!strcmp(in, "-rot")) test.sc_Rrot();
        else if(!strcmp(in, "debug")) test.sc_debug();
        else if(!strcmp(in, "debuge")) test.sc_debuge();
        else if(!strcmp(in, "debuga")) test.sc_debuga();
        else if(!strcmp(in, "or")) test.sc_or();
        else if(!strcmp(in, "and")) test.sc_and();
        else if(!strcmp(in, "xor")) test.sc_xor();

        else if(*in == '\"') test.sc_push(compilestr(in));
        else if(*in == '\'') test.sc_pushx(compilestr(in));
        else if(isnum(in)) test.sc_push(strtonum(in));
        else if(!strcmp(in, "quit"));
        else cout << "Unknown command \'" << in << "\'";

        test.sc_debuge();
        test.sc_debuga();
        cout << endl;
    }
    while(strcmp(in, "quit"));
}

void manual_load()
{
    program p;
    ifstream file;
    char in[128];

    cout << "Enter filename to load: ";
    cin.getline(in, 128);
    if(!strcmp(in, "test"))
    {
        test_stack();
    }
    else
    {
        file.open(in);
        if(file.good())
        {
            p.compile(file);
            cout << endl << endl;
            p.execute();
        }
        else cout << "Unable to load file.\n";
    }
}

int main()
{
    manual_load();
    return 0;
}
