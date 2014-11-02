/*
Oj742
StackStacks Interpreter
*/
typedef unsigned long long qword;
#include "Code.h"

class stack
{
public:
    stack();
    stack(stack * above, stack * after);
    stack(qword i);
    stack(char * str);
    stack(bool f);
    ~stack();
    bool pop();
    void clear();
    bool nip();
    void push(stack * s);
    void tuck(stack * s);
    void cat(stack * s);
    stack* take(); //pop and return
    stack* take(qword loc);
    stack* peek(qword loc);
    void add(stack * s);

    static stack * copy(stack * source);
    void display(qword level, stack * work);
    bool empty();
    bool insert(qword, stack*);

    qword getsize();
    stack* gethead();
    stack* getup();
    stack* getnext();
private:
    stack * next; //the stack behind this one on the same stack
    stack * up; //the stack containing this stack
    stack * head; //the first stack this stack is carrying
    /*
    up
      this
        head
        ...
      next
      ...
    ...
    */
    qword s_size; //this is for speed/ease of access
    qword id; //this is only for debug purposes
    static qword amount; //paired with id
};


class program
{
public:
    program();
    ~program();
    void sc_push();
    void sc_push(qword num);
    void sc_push(char * str);
    void sc_pushx(char * str);
    void sc_pop();
    void sc_pop2();
    void sc_tuck();
    void sc_nip();
    void sc_dup();
    void sc_cdup();
    void sc_dup2();
    void sc_clear();
    void sc_Pfail();
    void sc_Plevel();
    void sc_Psize();
    void sc_Ptest();
    void sc_Pleaf();
    void sc_outi();
    void sc_outc();
    void sc_outa();
    void sc_outs();
    void sc_gets();
    void sc_geta();
    void sc_inc();
    void sc_dec();
    void sc_pack();
    void sc_unpack();
    void sc_Rpack();
    void sc_add();
    void sc_cat();
    void sc_Mup();
    void sc_Mdown();
    void sc_Mroot();
    void sc_Mgoto();
    void sc_Mleaf();
    void sc_swap();
    void sc_not();
    void sc_test();
    void sc_eq();
    void sc_ls();
    void sc_grt();
    void sc_neq();
    void sc_lseq();
    void sc_grteq();
    void sc_swap2();
    void sc_over();
    void sc_Rover();
    void sc_cover();
    void sc_Rcover();
    void sc_cycle();
    void sc_Rcycle();
    void sc_over2();
    void sc_Rover2();
    void sc_resize();
    void sc_take();
    void sc_shftl();
    void sc_shftr();
    void sc_xchg();
    void sc_rot();
    void sc_Rrot();
    void sc_and();
    void sc_or();
    void sc_xor();


    void sc_debug();
    void sc_debuga();
    void sc_debuge();
    void sc_debugc();
    void sc_debugx();
    void compile(ifstream & file);
    scode interpret(char* word, ifstream & file, char * id);
    block* newblock(char * id, ifstream & file);
    block* searchforblock(char * id);
    void finishcompile();
    void execute();
private:
    stack * root;
    stack * working;
    qword level;
    bool flag_fail;
    bool flag_test;

    strtree * names;
    struct block * chead;
    struct call_stack * callstack;
    ofstream log;
};
