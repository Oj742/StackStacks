/*
Oj742
StackStacks Interpreter
*/
#include "Stack.h"

program::program()
{
    flag_fail = false;
    flag_test = false;
    level = 0;
    root = new stack();
    working = root;
    chead = NULL;
    names = new strtree();
    names->add("main");
    callstack = NULL;
}

program::~program()
{
    delete root;
    delete chead;
    log.close();
}

void program::compile(ifstream & file)
{
    char * temp;
    char saved[65];
    *saved = '\0';
    log.open("StackStacksLog.txt");
    log << "Beginning compiling...\n";
    while(!file.eof())
    {
        temp = getword(file);
        if(*saved && !strcmp(temp, "{"))
        {
            newblock(saved, file);
            *saved = '\0';
        }
        else
        {
            if(*saved && !isspace(*temp)) log << "Unused name \'" << saved << "\'\n";
            strcpy(saved, temp);
        }
    }
    finishcompile();
}


block* program::newblock(char * id, ifstream & file)
{
    memstore temp;
    block * ret = new block;
    scode buf;
    if(*id) ret->name = names->add(id);
    else ret->name = NULL;
    log << "Compiling function \'" << id << "\'...\n";
    do
    {
        buf = interpret(getword(file), file, ret->name);
        if(buf) temp.add(buf);
    }
    while(buf != SC_END);
    ret->c = temp.getarray();
    ret->size = temp.getsize();
    ret->next = chead;
    chead = ret;
    log << "Finished compiling function \'" << id << "\'\n";
    return ret;
}

scode program::interpret(char * word, ifstream & file, char * id)
{
    scode ret;

    if(!strcmp(word, "}")) ret = SC_END;
    else if(!strcmp(word, "push")) ret = SC_PUSH;
    else if(!strcmp(word, "pop")) ret = SC_POP;
    else if(!strcmp(word, "tuck")) ret = SC_TUCK;
    else if(!strcmp(word, "nip")) ret = SC_NIP;
    else if(!strcmp(word, "\\down")) ret = SC_MDOWN;
    else if(!strcmp(word, "\\up")) ret = SC_MUP;
    else if(!strcmp(word, "\\root")) ret = SC_MROOT;
    else if(!strcmp(word, "swap")) ret = SC_SWAP;
    else if(!strcmp(word, "dup")) ret = SC_DUP;
    else if(!strcmp(word, "cdup")) ret = SC_CDUP;
    else if(!strcmp(word, "clear")) ret = SC_CLEAR;
    else if(!strcmp(word, ".size")) ret = SC_SIZE;
    else if(!strcmp(word, ".level")) ret = SC_LEVEL;
    else if(!strcmp(word, "pack")) ret = SC_PACK;
    else if(!strcmp(word, "unpack")) ret = SC_UNPACK;
    else if(!strcmp(word, "\\leaf")) ret = SC_MLEAF;
    else if(!strcmp(word, "-pack")) ret = SC_RPACK;
    else if(!strcmp(word, ".fail")) ret = SC_GETFAIL;
    else if(!strcmp(word, ".test")) ret = SC_GETTEST;
    else if(!strcmp(word, "add")) ret = SC_ADD;
    else if(!strcmp(word, "cat")) ret = SC_CAT;
    else if(!strcmp(word, "dup2")) ret = SC_DUP2;
    else if(!strcmp(word, "inc")) ret = SC_INC;
    else if(!strcmp(word, "dec")) ret = SC_DEC;
    else if(!strcmp(word, "\\goto")) ret = SC_MGOTO;
    else if(!strcmp(word, "geta")) ret = SC_GETA;
    else if(!strcmp(word, "gets")) ret = SC_GETS;
    else if(!strcmp(word, "outa")) ret = SC_OUTA;
    else if(!strcmp(word, "outi")) ret = SC_OUTI;
    else if(!strcmp(word, "outc")) ret = SC_OUTC;
    else if(!strcmp(word, "outs")) ret = SC_OUTS;
    else if(!strcmp(word, "not")) ret = SC_NOT;
    else if(!strcmp(word, "test")) ret = SC_TEST;
    else if(!strcmp(word, "pop2")) ret = SC_POP2;
    else if(!strcmp(word, "eq")) ret = SC_EQ;
    else if(!strcmp(word, "ls")) ret = SC_LS;
    else if(!strcmp(word, "grt")) ret = SC_GRT;
    else if(!strcmp(word, "neq")) ret = SC_NEQ;
    else if(!strcmp(word, "lseq")) ret = SC_LSEQ;
    else if(!strcmp(word, "grteq")) ret = SC_GRTEQ;
    else if(!strcmp(word, "swap2")) ret = SC_SWAP2;
    else if(!strcmp(word, "over")) ret = SC_OVER;
    else if(!strcmp(word, "-over")) ret = SC_ROVER;
    else if(!strcmp(word, "cycle")) ret = SC_CYCLE;
    else if(!strcmp(word, "-cycle")) ret = SC_RCYCLE;
    else if(!strcmp(word, "cover")) ret = SC_COVER;
    else if(!strcmp(word, "-cover")) ret = SC_RCOVER;
    else if(!strcmp(word, "over2")) ret = SC_OVER2;
    else if(!strcmp(word, "-over2")) ret = SC_ROVER2;
    else if(!strcmp(word, "shftl")) ret = SC_SHFTL;
    else if(!strcmp(word, "shftr")) ret = SC_SHFTR;
    else if(!strcmp(word, "xchg")) ret = SC_XCHG;
    else if(!strcmp(word, "take")) ret = SC_TAKE;
    else if(!strcmp(word, ".leaf")) ret = SC_GETLEAF;
    else if(!strcmp(word, "rot")) ret = SC_ROT;
    else if(!strcmp(word, "-rot")) ret = SC_RROT;
    else if(!strcmp(word, "debug")) ret = SC_DEBUG;
    else if(!strcmp(word, "debuge")) ret = SC_DEBUGE;
    else if(!strcmp(word, "debuga")) ret = SC_DEBUGA;
    else if(!strcmp(word, "debugc")) ret = SC_DEBUGC;
    else if(!strcmp(word, "debugx")) ret = SC_DEBUGX;
    else if(!strcmp(word, "?skip")) ret = SC_SKIP;
    else if(!strcmp(word, "?do")) ret = SC_DO;
    else if(!strcmp(word, "?loop")) ret = SC_LOOP;
    else if(!strcmp(word, "?exit")) ret = SC_EXIT;
    else if(!strcmp(word, "or")) ret = SC_OR;
    else if(!strcmp(word, "and")) ret = SC_AND;
    else if(!strcmp(word, "xor")) ret = SC_XOR;
    else if(!strcmp(word, "endl")) ret = SC_ENDL;

    else if(isspace(*word) || !*word) ret = NULL;
    else if(!strcmp(word, "{"))
    {
        ret = new xcode;
        ((xcode*)ret)->i = SC_BLOCK;
        ((xcode*)ret)->blk = newblock(names->add(anonfunc(id)), file);
    }
    else if(*word == '\"')
    {
        ret = new xcode;
        ((xcode*)ret)->i = SC_STRING;
        ((xcode*)ret)->str = names->add(compilestr(word));
    }
    else if(*word == '\'')
    {
        ret = new xcode;
        ((xcode*)ret)->i = SC_STRINGX;
        ((xcode*)ret)->str = names->add(compilestr(word));
    }
    else if(*word == '@')
    {
        ret = new xcode;
        ((xcode*)ret)->i = SC_CALL;
        ((xcode*)ret)->str = names->add(++word);
    }
    else if(isnum(word))
    {
        ret = new xcode;
        ((xcode*)ret)->i = SC_NUM;
        ((xcode*)ret)->num = strtonum(word);
    }
    else
    {
        ret = NULL;
        log << "Unknown command \'" << word << "\' in \'" << id << "\'\n";
    }

    return ret;
}

block* program::searchforblock(char * id)
{
    block * temp = chead;
    while(temp)
    {
        if(temp->name == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

void program::finishcompile() //link all function calls
{
    block * temp = chead;
    char * tc;
    log << "Linking...\n";
    while(temp)
    {
        for(qword i = 0; i < temp->size; ++i)
        {
            if((temp->c[i] < SC_PUSH || temp->c[i] > SC_IDLAST) && temp->c[i] && ((xcode*)temp->c[i])->i == SC_CALL)
            {
                ((xcode*)temp->c[i])->blk = searchforblock(tc = ((xcode*)temp->c[i])->str);
                if(!((xcode*)temp->c[i])->blk)
                {
                    log << "Tried to call nonexisting function \'" << tc << "\' in \'" << temp->name << "\'\n";
                    ((xcode*)temp->c[i])->i = SC_NOP;
                }
                else log << "Call to \'" << tc << "\' linked\n";
            }
        }
        temp = temp->next;
    }

}

void program::execute()
{
    block * tblk = searchforblock(names->add("main"));
    call_stack * temp;
    long ip = 0;
    log.close();
    log.open("StackStackslog.txt", ios::app);
    if(!tblk)
    {
        log << "No \'main\' function found; execution aborted...\n";
        return;
    }
    log << "Started execution\n";
    callstack = new call_stack;
    callstack->current = tblk;
    callstack->i = 0;
    callstack->prev = NULL;
    do
    {
        tblk = callstack->current;
        ip = callstack->i;
        while(ip < tblk->size)
        {
            //decode(tblk->c[ip]); cout << endl;
            if(!tblk->c[ip] || tblk->c[ip] == SC_END);
            else if(tblk->c[ip] == SC_PUSH) sc_push();
            else if(tblk->c[ip] == SC_POP) sc_pop();
            else if(tblk->c[ip] == SC_POP2) sc_pop2();
            else if(tblk->c[ip] == SC_SIZE) sc_Psize();
            else if(tblk->c[ip] == SC_LEVEL) sc_Plevel();
            else if(tblk->c[ip] == SC_NIP) sc_nip();
            else if(tblk->c[ip] == SC_TUCK) sc_tuck();
            else if(tblk->c[ip] == SC_CLEAR) sc_clear();
            else if(tblk->c[ip] == SC_DUP) sc_dup();
            else if(tblk->c[ip] == SC_CDUP) sc_cdup();
            else if(tblk->c[ip] == SC_SWAP) sc_swap();
            else if(tblk->c[ip] == SC_INC) sc_inc();
            else if(tblk->c[ip] == SC_DEC) sc_dec();
            else if(tblk->c[ip] == SC_PACK) sc_pack();
            else if(tblk->c[ip] == SC_UNPACK) sc_unpack();
            else if(tblk->c[ip] == SC_GETFAIL) sc_Pfail();
            else if(tblk->c[ip] == SC_GETTEST) sc_Ptest();
            else if(tblk->c[ip] == SC_OUTI) sc_outi();
            else if(tblk->c[ip] == SC_OUTC) sc_outc();
            else if(tblk->c[ip] == SC_OUTA) sc_outa();
            else if(tblk->c[ip] == SC_OUTS) sc_outs();
            else if(tblk->c[ip] == SC_GETA) sc_geta();
            else if(tblk->c[ip] == SC_GETS) sc_gets();
            else if(tblk->c[ip] == SC_DUP2) sc_dup2();
            else if(tblk->c[ip] == SC_SWAP2) sc_swap2();
            else if(tblk->c[ip] == SC_OVER) sc_over();
            else if(tblk->c[ip] == SC_ROVER) sc_Rover();
            else if(tblk->c[ip] == SC_COVER) sc_cover();
            else if(tblk->c[ip] == SC_RCOVER) sc_Rcover();
            else if(tblk->c[ip] == SC_OVER2) sc_over2();
            else if(tblk->c[ip] == SC_ROVER2) sc_Rover2();
            else if(tblk->c[ip] == SC_ROT) sc_rot();
            else if(tblk->c[ip] == SC_RROT) sc_Rrot();
            else if(tblk->c[ip] == SC_CYCLE) sc_cycle();
            else if(tblk->c[ip] == SC_RCYCLE) sc_Rcycle();
            else if(tblk->c[ip] == SC_ADD) sc_add();
            else if(tblk->c[ip] == SC_TAKE) sc_take();
            else if(tblk->c[ip] == SC_CAT) sc_cat();
            else if(tblk->c[ip] == SC_MUP) sc_Mup();
            else if(tblk->c[ip] == SC_MDOWN) sc_Mdown();
            else if(tblk->c[ip] == SC_MROOT) sc_Mroot();
            else if(tblk->c[ip] == SC_MLEAF) sc_Mleaf();
            else if(tblk->c[ip] == SC_MGOTO) sc_Mgoto();
            else if(tblk->c[ip] == SC_NOT) sc_not();
            else if(tblk->c[ip] == SC_EQ) sc_eq();
            else if(tblk->c[ip] == SC_LS) sc_ls();
            else if(tblk->c[ip] == SC_GRT) sc_grt();
            else if(tblk->c[ip] == SC_NEQ) sc_neq();
            else if(tblk->c[ip] == SC_LSEQ) sc_lseq();
            else if(tblk->c[ip] == SC_GRTEQ) sc_grteq();
            else if(tblk->c[ip] == SC_OR) sc_or();
            else if(tblk->c[ip] == SC_AND) sc_and();
            else if(tblk->c[ip] == SC_XOR) sc_xor();
            else if(tblk->c[ip] == SC_TEST) sc_test();
            else if(tblk->c[ip] == SC_SKIP)
            {
                if(flag_test) ++ip;
            }
            else if(tblk->c[ip] == SC_DO)
            {
                if(!flag_test) ++ip;
            }
            else if(tblk->c[ip] == SC_EXIT)
            {
                if(flag_test) ip = tblk->size;
            }
            else if(tblk->c[ip] == SC_LOOP)
            {
                if(flag_test) ip = -1;
            }
            else if(tblk->c[ip] == SC_GETLEAF) sc_Pleaf();
            else if(tblk->c[ip] == SC_RPACK) sc_Rpack();
            else if(tblk->c[ip] == SC_SHFTL) sc_shftl();
            else if(tblk->c[ip] == SC_SHFTR) sc_shftr();
            else if(tblk->c[ip] == SC_XCHG) sc_xchg();
            else if(tblk->c[ip] == SC_DEBUG) sc_debug();
            else if(tblk->c[ip] == SC_DEBUGA) sc_debuga();
            else if(tblk->c[ip] == SC_DEBUGC)
            {
                callstack->i = ip;
                sc_debugc();
            }
            else if(tblk->c[ip] == SC_DEBUGE) sc_debuge();
            else if(tblk->c[ip] == SC_ENDL) cout << endl;
            else
            {
                switch(((xcode*)tblk->c[ip])->i)
                {
                case SC_NUM:
                    sc_push(((xcode*)tblk->c[ip])->num);
                    break;
                case SC_STRING:
                    sc_push(((xcode*)tblk->c[ip])->str);
                    break;
                case SC_STRINGX:
                    sc_pushx(((xcode*)tblk->c[ip])->str);
                    break;
                case SC_CALL:
                case SC_BLOCK:
                    callstack->i = ip+1;
                    temp = new call_stack;
                    temp->prev = callstack;
                    temp->i = 0;
                    temp->current = ((xcode*)tblk->c[ip])->blk;
                    callstack = temp;
                    ip = -1;
                    tblk = callstack->current;
                    break;
                }
            }
            ++ip;
        }
        temp = callstack;
        callstack = callstack->prev;
        delete temp;
    }
    while(callstack);
    log << "Finshed execution\n";
}

/*
void decode(scode c)
{
    if(!c) cout << "nop";
    else if(c == SC_END) cout <<  "}";
    else if(c == SC_PUSH) cout <<  "push";
    else if(c == SC_POP) cout <<  "pop";
    else if(c == SC_POP2) cout <<  "pop2";
    else if(c == SC_SIZE) cout <<  ".size";
    else if(c == SC_LEVEL) cout <<  ".level";
    else if(c == SC_NIP) cout <<  "nip";
    else if(c == SC_TUCK) cout <<  "tuck";
    else if(c == SC_CLEAR) cout <<  "clear";
    else if(c == SC_DUP) cout <<  "dup";
    else if(c == SC_CDUP) cout <<  "cdup";
    else if(c == SC_SWAP) cout <<  "swap";
    else if(c == SC_INC) cout <<  "inc";
    else if(c == SC_DEC) cout <<  "dec";
    else if(c == SC_PACK) cout <<  "pack";
    else if(c == SC_UNPACK) cout <<  "unpack";
    else if(c == SC_GETFAIL) cout <<  ".fail";
    else if(c == SC_GETTEST) cout <<  ".test";
    else if(c == SC_OUTI) cout <<  "outi";
    else if(c == SC_OUTC) cout <<  "outc";
    else if(c == SC_OUTA) cout <<  "outa";
    else if(c == SC_OUTS) cout <<  "outs";
    else if(c == SC_GETA) cout <<  "geta";
    else if(c == SC_GETS) cout <<  "gets";
    else if(c == SC_DUP2) cout <<  "dup2";
    else if(c == SC_SWAP2) cout <<  "swap2";
    else if(c == SC_OVER) cout <<  "over";
    else if(c == SC_ROVER) cout <<  "-over";
    else if(c == SC_COVER) cout <<  "cover";
    else if(c == SC_RCOVER) cout <<  "-cover";
    else if(c == SC_OVER2) cout <<  "over2";
    else if(c == SC_ROVER2) cout <<  "-over2";
    else if(c == SC_ROT) cout <<  "rot";
    else if(c == SC_RROT) cout <<  "-rot";
    else if(c == SC_CYCLE) cout <<  "cycle";
    else if(c == SC_RCYCLE) cout <<  "-cycle";
    else if(c == SC_ADD) cout <<  "add";
    else if(c == SC_TAKE) cout <<  "take";
    else if(c == SC_CAT) cout <<  "cat";
    else if(c == SC_MUP) cout <<  "\\up";
    else if(c == SC_MDOWN) cout <<  "\\down";
    else if(c == SC_MROOT) cout <<  "\\root";
    else if(c == SC_MLEAF) cout <<  "\\leaf";
    else if(c == SC_MGOTO) cout <<  "\\goto";
    else if(c == SC_NOT) cout <<  "not";
    else if(c == SC_EQ) cout <<  "eq";
    else if(c == SC_LS) cout <<  "ls";
    else if(c == SC_GRT) cout <<  "grt";
    else if(c == SC_NEQ) cout <<  "neq";
    else if(c == SC_LSEQ) cout <<  "lseq";
    else if(c == SC_GRTEQ) cout <<  "grteq";
    else if(c == SC_OR) cout <<  "or";
    else if(c == SC_AND) cout <<  "and";
    else if(c == SC_XOR) cout <<  "xor";
    else if(c == SC_TEST) cout <<  "test";
    else if(c == SC_SKIP) cout <<  "?skip";
    else if(c == SC_DO) cout <<  "?do";
    else if(c == SC_EXIT) cout <<  "?exit";
    else if(c == SC_LOOP) cout <<  "?loop";
    else if(c == SC_GETLEAF) cout <<  ".leaf";
    else if(c == SC_RPACK) cout <<  "-pack";
    else if(c == SC_SHFTL) cout <<  "shftl";
    else if(c == SC_SHFTR) cout <<  "shftr";
    else if(c == SC_XCHG) cout <<  "xchg";
    else if(c == SC_DEBUG) cout <<  "debug";
    else if(c == SC_DEBUGA) cout <<  "debuga";
    else if(c == SC_DEBUGC) cout <<  "debugc";
    else if(c == SC_DEBUGE) cout <<  "debuge";
    else if(c == SC_ENDL) cout <<  "endl";
    else
    {
        switch(((xcode*)c)->i)
        {
        case SC_NUM:
            cout << ((xcode*)c)->num;
            break;
        case SC_STRING:
            cout <<  "\"" << ((xcode*)c)->str << "\"";
            break;
        case SC_STRINGX:
            cout <<  "\'" << ((xcode*)c)->str << "\'";
            break;
        case SC_CALL:
            cout <<  "@" << ((xcode*)c)->blk->name;
            break;
        case SC_BLOCK:
            cout <<  "{";
            break;
        }
    }
}
*/
