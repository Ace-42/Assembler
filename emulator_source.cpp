#include <bits/stdc++.h>
#define int long long 
using namespace std;
template <typename S>
ostream& operator<<(ostream& os,
                    const vector<S>& vector)
{
    for (auto element : vector) {
        os << element << " ";
    }
    return os;
}
//custom function for binary to decimal
int bintodec(string x){
    if(x[0]=='1'){
        for(int i=0;i<x.size();i++){
            if(x[i]=='1'){
                x[i]='0';
            }
            else{
                x[i]='1';
            }
        }
        int carry=1;
        for(int i=x.size()-1;i>=0;i--){
            if(carry==1){
                if(x[i]=='1'){
                    x[i]='0';
                }
                else if(x[i]=='0'){
                    x[i]='1';
                    carry=0;
                }
            }
        }
        int res=0,mul=1;
        for(int i=x.size()-1;i>=0;i--){
            res+=(x[i]-'0')*mul;
            mul*=2;
        }
        return -1*res;
    }
    int res=0,mul=1;
    for(int i=x.size()-1;i>=0;i--){
        res+=(x[i]-'0')*mul;
        mul*=2;
    }
    return res;
}
int A=0,B=0,SP=0,PC=0,exitflag=1;
//declaring a huge array for memory
vector<int> memory(1<<24);
//to store the binary bits in groups of 34
vector<string> binlines;
vector<vector<int>> trace;
//custom functions to separate opcode and operands
int opcode(string x){
    string temp="";
    for(int i=24;i<=31;i++){
        temp+=x[i];
    }
    int res=bintodec(temp);
    return res;
}
int operand6(string x){
    string temp="";
    for(int i=0;i<=23;i++){
        temp+=x[i];
    }
    int res=bintodec(temp);
    return res;
}
int operand8(string x){
    string temp="";
    for(int i=0;i<=31;i++){
        temp+=x[i];
    }
    int res=bintodec(temp);
    return res;
}
//functions to implement after separation
void ldc(int value){                        
    B=A;
    A=value;
}
void adc(int value){
    A+=value;
}
void ldl(int offset){
    B=A;
    A=memory[SP+offset];
}
void stl(int offset){
    memory[SP+offset]=A;
    A=B;
}
void ldnl(int offset){
    A=memory[A+offset];
}
void stnl(int offset){
    memory[A+offset]=B;
}
void add(){
    A+=B;
}
void sub(){
    A=B-A;
}
void shl(){
    A=B<<A;
}
void shr(){ 
    A=B>>A;
}
void adj(int value){
    SP=SP+value;
}
void a2sp(){
    SP=A;
    A=B;
}
void sp2a(){
    B=A;
    A=SP;
}
void call(int offset){
    B=A;
    A=PC;
    PC+=offset;
}
void ret(){
    PC=A;
    A=B;
}
void brz(int offset){
    if(A==0)
        PC=PC+offset;
}
void brlz(int offset){
    if(A < 0)
        PC=PC+offset;
}
void br(int offset){ 
    PC=PC+offset;
}
void HALT(){
    exitflag=0;
}
void processing(){ //calling the function to carry out the operations 
    for(int i=0;i<binlines.size();i++){
        if(binlines[i][33]=='1' && binlines[i][32]=='1'){
            memory[i]=operand8(binlines[i]);
        }
    }
    while(PC<binlines.size()){
        if(exitflag==0){
            break;
        }
        if(binlines[PC][33]=='0' && binlines[PC][32]=='0'){
            int opcd=opcode(binlines[PC]);
            int oprnd=operand6(binlines[PC]);
            switch(opcd){
                case 0 :
                    ldc(oprnd);
                    break;
                
                case 1 :
                    adc(oprnd);
                    break;
                
                case 2 :
                    ldl(oprnd);
                    break;

                case 3 :
                    stl(oprnd);
                    break;
                case 4 :
                    ldnl(oprnd);
                    break;        
                
                case 5 :
                    stnl(oprnd);
                    break;

                case 6 :
                    add();
                    break;

                case 7 :
                    sub();
                    break;

                case 8 :
                    shl();
                    break;

                case 9 :
                    shr();
                    break;

                case 10 :
                    adj(oprnd);
                    break;

                case 11 :
                    a2sp();
                    break;

                case 12 :
                    sp2a();
                    break;

                case 13 :
                    call(oprnd);
                    break;

                case 14:
                    ret();
                    break;

                case 15 :
                    brz(oprnd);
                    break;

                case 16 :
                    brlz(oprnd);
                    break;

                case 17 :
                    br(oprnd);
                    break;

                case 18 :
                    HALT();
                    break;                                        
                default:
                    break;
            }
        }
        else if(binlines[PC][33]=='0' && binlines[PC][32]=='1'){
            PC++;
            continue;
        }
        else if(binlines[PC][33]=='1' && binlines[PC][32]=='1'){
            binlines[PC][33]=binlines[PC][33];
        }
        vector<int> tempo={A,B,PC,SP};
        trace.push_back(tempo);
        PC++;
    }
}
//for interacting with the emulator
void interaction(int n){
    int choice;
    cout<<"Enter your choice: \n";
    cout<<"Type 1 to see the trace line by line\n";
    cout<<"Type 2 to see memory dump\n";
    cout<<"Type 3 to exit the loop\n";
    cin>>choice;
    if(choice==3){
        return;
    }
    if(choice==1){
        for(int i=0;i<trace.size();i++){  //getting the trace
            cout<<"Line = "<<i<<" A = "<<trace[i][0]<<" B = "<<trace[i][1]<<" PC = "<<trace[i][2]<<" SP = "<<trace[i][3]<<endl;
        }
        interaction(1);
    }
    else if(choice==2){
        cout<<"The memory dump is:\n";  //getting the memory dump
        int end=trace.size()-1;
        cout<<" A = "<<trace[end][0]<<" B = "<<trace[end][1]<<" PC = "<<trace[end][2]<<" SP = "<<trace[end][3]<<endl;
        int indmem;
        cout<<"Enter till which index you want to see the memory storage\n";
        cin>>indmem;
        cout<<"The memory storage upto the specified index "<<indmem<<" is: \n";
        for(int i=0;i<=indmem;i++){
            cout<<"Memory at index "<<i<<" is "<<memory[i]<<endl;
        }
        interaction(2);
    }
}
signed main(){
    ifstream fobj;
    string filename;
    cout<<"Enter machine code file name: "; //opening the file 
    cin>>filename;
    fobj.open(filename,ios::binary);
    string line,temp;
    char x;
    int ct=0;
    while(fobj.read((char*)&x,sizeof(x))){
        ct++;                                 //storing in groups of 34
        temp.push_back(x);
        if(ct==34){
            ct=0;
            binlines.push_back(temp);
            temp.clear();
        }
    }
    processing();
    cout<<binlines.size()<<endl; //printing out the trace size and number of lines
    cout<<trace.size()<<endl;
    int choice=0;               //start of interaction initialised
    interaction(choice);
}