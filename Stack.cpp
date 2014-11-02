/*
Oj742
StackStacks Interpreter
*/
#include "Stack.h"
using namespace std;

qword stack::amount = 0;

qword stack::getsize(){return s_size;}
stack* stack::gethead(){return head;}
stack* stack::getup(){return up;}
stack* stack::getnext(){return next;}

stack::stack()
{
    id = amount;
    ++amount;
    next = NULL;
    up = NULL;
    head = NULL;
    s_size = 0;
}

stack::stack(stack * above, stack * after)
{
    id = amount;
    ++amount;
    up = above;
    next = after;
    head = NULL;
    s_size = 0;
}

stack::stack(qword i)
{
    id = amount;
    ++amount;
    next = NULL;
    up = NULL;
    head = NULL;
    s_size = i;
    for(; i > 0; --i)
    {
        head = new stack(this, head);
    }
}

stack::stack(char * str)
{
    stack * temp = NULL;
    id = amount;
    ++amount;
    next = NULL;
    up = NULL;
    s_size = strlen(str);
    for(long i = s_size - 1; i >= 0; --i)
    {
        head = new stack((qword)str[i]);
        head->next = temp;
        temp = head;
    }
}

stack::stack(bool f)
{
    id = amount;
    ++amount;
    next = NULL;
    up = NULL;
    if(f){
        s_size = 1;
        head = new stack(this, NULL);
    } else {
        s_size = 0;
        head = NULL;
    }
}

stack::~stack()
{
    stack * temp;
    while(head)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
}

bool stack::pop()
{
    if(!head) return true;

    stack * temp;

    --s_size;
    temp = head->next;
    delete head;
    head = temp;
    return false;
}

bool stack::nip()
{
    if(!head || !head->next) return true;

    stack * temp;

    --s_size;
    temp = head->next;
    head->next = temp->next;
    delete temp;
    return false;
}

void stack::push(stack * s)
{
    ++s_size;
    s->up = this;
    s->next = NULL;
    if(!head) head = s;
    else
    {
        stack * temp = head;
        head = s;
        head->next = temp;
    }
}

void stack::tuck(stack * s)
{
    ++s_size;
    s->up = this;
    s->next = NULL;
    if(!head) head = s;
    else
    {
        stack * temp = head->next;
        head->next = s;
        head->next->next = temp;
    }
}

void stack::cat(stack * s)
{
    ++s_size;
    if(!head) head = s;
    else{
        stack * temp = head;
        while(temp->next) temp = temp->next;
        temp->next = s;
    }
    s->next = NULL;
    s->up = this;
}

void stack::display(qword level, stack * work)
{
    for(qword n = 0; n < level; ++n) cout << "  ";
    if(this == work) cout << ">";
    else cout << " ";
    if(empty() && level) {cout << "...\n"; return;}
    else cout << hex << id << "(" << dec << s_size << ")\n";

    if(head) head->display(level+1, work);
    if(next) next->display(level, work);
}

stack* stack::copy(stack * source)
{
    stack * temp = new stack;

    if(source){
        source = source->head;
        while(source){
            temp->cat(copy(source));
            source = source->next;
        }
    }
    return temp;
}

stack* stack::take()
{
    if(!head) return NULL;
    stack * temp;

    --s_size;
    temp = head;
    head = head->next;
    temp->next = NULL;
    return temp;
}

stack* stack::take(qword loc)
{
    if(!head || loc >= s_size) return NULL;
    stack * temp = head, * prev;
    --s_size;
    for(qword i = 0; i < loc; ++i){
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    temp->next = NULL;
    return temp;
}

stack* stack::peek(qword loc)
{
    if(!head || loc >= s_size) return NULL;
    stack * temp = head;
    for(qword i = 0; i < loc; ++i) temp = temp->next;
    return temp;
}

void stack::add(stack * s)
{
    stack * temp = s->head;
    if(temp)
        while(temp->next) temp = temp->next;
    temp->next = head;
    head = s->head;
    s_size += s->s_size;
}

bool stack::empty()
{
    if(s_size) return false;
    if(next) return next->empty();
    return true;
}

bool stack::insert(qword loc, stack * s)
{
    if(s_size < loc) {
        if(s->up == NULL) delete s; //don't want memory leaks
        return true;
    }
    s->up = this;
    if(!loc) {
        head = s;
        head->next = NULL;
    } else {
        stack * temp = head, * prev;
        for(qword i = 0; i < loc; ++i) {
            prev = temp;
            temp = temp->next;
        }
        prev->next = s;
        s->next = temp;
    }
    ++s_size;
    return false;
}
/*
void stack::split()
{
    if(!head || head->s_size < 2) {failure = true; return;}

    failure = false;

    stack * temp = head;
    stack * temp2;

    for(int i = 0; i < head->s_size/2; ++i){
        temp = temp->next;
    }
    temp2 = temp->next;
    temp->next = NULL;
    temp2->next = head->next;
    head->next = temp2;
    ++s_size;
}*/

void program::sc_push(){working->push(new stack());}

void program::sc_push(qword num){working->push(new stack(num));}

void program::sc_push(char * str){working->push(new stack(str));}

void program::sc_pushx(char * str){working->add(new stack(str));}

void program::sc_pop(){flag_fail = working->pop();}

void program::sc_pop2(){
    if(working->getsize() > 1){
        working->pop();
        flag_fail = working->pop();
    } else flag_fail = true;
}

void program::sc_nip(){flag_fail = working->nip();}

void program::sc_tuck(){working->tuck(new stack());}

void program::sc_dup(){
    if(working->getsize()){
        flag_fail = false;
        working->push(new stack(working->gethead()->getsize()));
    } else flag_fail = false;
}

void program::sc_cdup(){
    if(working->getsize()){
        flag_fail = false;
        working->push(stack::copy(working->gethead()));
    } else flag_fail = true;
}

void program::sc_dup2(){
    if(working->getsize() > 1){
        flag_fail = false;
        working->push(new stack(working->gethead()->getnext()->getsize()));
        working->push(new stack(working->gethead()->getnext()->getsize()));
    } else flag_fail = true;
}

void program::sc_Psize(){working->push(new stack(working->getsize()));}

void program::sc_Plevel(){working->push(new stack(level));}

void program::sc_Pfail(){working->push(new stack(flag_fail));}

void program::sc_Ptest(){working->push(new stack(flag_test));}

void program::sc_clear(){flag_fail = true; while(working->getsize()){flag_fail = false; working->pop();}}

void program::sc_outi(){
    if(working->getsize()){
        cout << working->gethead()->getsize();
        flag_fail = working->pop();
    } else flag_fail = true;
}

void program::sc_outc(){
    if(working->getsize()){
        cout << (char)working->gethead()->getsize();
        flag_fail = working->pop();
    } else flag_fail = true;
}

void program::sc_outa(){
    if(working->getsize()){
        flag_fail = false;
        while(working->getsize()){
            cout << (char)working->gethead()->getsize();
            working->pop();
        }
    } else flag_fail = true;
}

void program::sc_outs(){
    if(working->getsize() && working->gethead()->getsize()){
        flag_fail = false;
        while(working->gethead()->getsize()){
            cout << (char)working->gethead()->gethead()->getsize();
            working->gethead()->pop();
        }
        working->pop();
    } else flag_fail = true;
}

void program::sc_gets(){
    static char b[255];
    cin.getline(b, 255);
    working->push(new stack(b));
}

void program::sc_geta(){
    static char b[255];
    cin.getline(b, 255);
    working->add(new stack(b));
}

void program::sc_inc(){
    if(working->getsize()){
        flag_fail = false;
        working->gethead()->push(new stack());
    } else flag_fail = true;
}

void program::sc_dec(){
    if(working->getsize()){
        flag_fail = working->gethead()->pop();
    } else flag_fail = true;
}

void program::sc_pack(){
    if(working->getsize() > 1){
        stack * temp = working->take();
        working->gethead()->push(temp);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Rpack(){
    if(working->getsize() > 1){
        stack * temp = working->take();
        temp->push(working->take());
        working->push(temp);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_unpack(){
    if(working->getsize() && working->gethead()->getsize()){
        working->push(working->gethead()->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_add(){
    if(working->getsize() > 1){
        stack * temp = working->take();
        working->gethead()->add(temp);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_cat(){
    if(working->getsize() > 1){
        stack * temp = working->take();
        temp->add(working->take());
        working->push(temp);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Mup(){
    if(level){
        working = working->getup();
        --level;
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Mdown(){
    if(working->getsize()){
        working = working->gethead();
        ++level;
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Mroot(){
    working = root;
    level = 0;
}

void program::sc_Mleaf(){
    while(working->getsize()) {working = working->gethead(); ++level;}
}

void program::sc_Mgoto(){
    if(working->getsize()){
        qword target = working->gethead()->getsize();
        qword i = 0;
        stack * temp = root, * temp2 = working->take();
        for(; i < target && temp->getsize(); ++i)temp = temp->gethead();
        if(i == target){
            flag_fail = false;
            working = temp;
            level = target;
            delete temp2;
        } else{
            flag_fail = true;
            working->push(temp2);
        }
    }else flag_fail = true;
}

void program::sc_swap(){
    if(working->getsize() > 1){
        working->tuck(working->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_test(){
    if(working->getsize()){
        flag_test = bool(working->gethead()->getsize());
        flag_fail = working->pop();
    } else flag_fail = true;
}

void program::sc_not(){
    if(working->getsize()){
        if(working->gethead()->getsize()) while(working->gethead()->getsize()) working->gethead()->pop();
        else working->gethead()->push(new stack());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_eq(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() == working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_ls(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() < working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_grt(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() > working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_neq(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() != working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_lseq(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() <= working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_grteq(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getsize() >= working->gethead()->getnext()->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_swap2(){
    if(working->getsize() > 3){
        working->push(working->take(3));
        working->push(working->take(3));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_over(){
    if(working->getsize() > 1){
        working->push(new stack(working->gethead()->getnext()->getsize()));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Rover(){flag_fail = working->insert(2, new stack(working->gethead()->getsize()));}

void program::sc_cycle(){flag_fail = working->insert(working->getsize(), working->take());}

void program::sc_Rcycle(){
    stack * temp = working->take(working->getsize()-1);
    if(temp){
        working->push(temp);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_cover(){
    if(working->getsize() > 1){
        working->push(stack::copy(working->gethead()->getnext()));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Rcover(){flag_fail = working->insert(2, stack::copy(working->gethead()));}

void program::sc_over2(){
    if(working->getsize() > 3){
        working->push(new stack(working->peek(3)->getsize()));
        working->push(new stack(working->peek(3)->getsize()));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Rover2(){
    if(working->getsize() > 3){
        working->insert(4, new stack(working->peek(0)->getsize()));
        flag_fail = working->insert(5, new stack(working->peek(1)->getsize()));
    } else flag_fail = true;
}

void program::sc_resize(){
    if(working->getsize()){
        qword n = working->gethead()->getsize();
        working->pop();
        while(working->getsize() < n) working->cat(new stack());
        while(working->getsize() > n) delete working->take(working->getsize()-1);
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_shftr(){
    if(working->getsize() > 1 && working->gethead()->getsize()){
        working->peek(1)->push(working->gethead()->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_shftl(){
    if(working->getsize() > 1 && working->peek(1)->getsize()){
        working->gethead()->push(working->peek(1)->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_xchg(){
    if(working->getsize() > 1 && working->gethead()->getsize() && working->peek(1)->getsize()){
        working->peek(1)->push(working->gethead()->take());
        working->gethead()->push(working->peek(1)->take(1));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_take(){
    if(working->getsize() > 1 && working->gethead()->getsize() && working->gethead()->getsize() < working->peek(1)->getsize()){
        qword n = working->gethead()->getsize();
        working->pop();
        working->push(new stack());
        while(working->gethead()->getsize() < n) working->gethead()->cat(working->peek(1)->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Pleaf(){
    qword i = level;
    stack * temp = working;
    while(temp->getsize()) {
        temp = temp->gethead();
        ++i;
    }
    working->push(new stack(i));
}

void program::sc_rot(){
    if(working->getsize() > 2){
        working->insert(2, working->take());
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_Rrot(){
    if(working->getsize() > 2){
        working->push(working->take(2));
        flag_fail = false;
    } else flag_fail = true;
}

void program::sc_or(){
    if(working->getsize() > 1){
        working->push(new stack(working->peek(0)->getsize() || working->peek(1)->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_and(){
    if(working->getsize() > 1){
        working->push(new stack(working->peek(0)->getsize() && working->peek(1)->getsize()));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}

void program::sc_xor(){
    if(working->getsize() > 1){
        working->push(new stack((working->peek(0)->getsize() || working->peek(1)->getsize()) && !(working->peek(0)->getsize() && working->peek(1)->getsize())));
        working->nip();
        flag_fail = working->nip();
    } else flag_fail = true;
}



void program::sc_debug(){cout << endl; working->display(level, working);}

void program::sc_debuga(){cout << endl; root->display(0, working);}

void program::sc_debuge(){cout << "\nFAIL: " << flag_fail << "  TEST: " << flag_test << "  level: " << level << endl;}

void program::sc_debugc()
{
    call_stack * temp = callstack;
    cout << "\nCall Stack:\n";
    while(temp)
    {
        cout << temp->current->name << "(" << temp->i << ")\n";
        temp = temp->prev;
    }
}
/*
void program::sc_debugx()
{
    block * temp = chead;
    while(temp)
    {
        cout << temp->name << "(" << temp->size << ")\n";
        temp = temp->next;
    }
}*/

