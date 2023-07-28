#include <bits/stdc++.h>
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
string ltrim(string &s) {
    return regex_replace(s, regex("^\\s+"), string("")); //to remove the empty lines from file
}
string rtrim(string &s) {
    return regex_replace(s, regex("\\s+$"), string(""));
}
map<string,pair<int,int>> instructions;
map<string,pair<string,string>> labelvalues;
map<string,int> labellineno;
map<int,char> hexval;
vector<string> opcodes,operands,pcs;
set<string> uniquelabels;
int pc=0,errorflag=0,checkhalt=0;
string addtopc="00000000";
int bintodec(string x){
    int res=0,mul=1;
    for(int i=x.size()-1;i>=2;i--){  //converting binary num input to decimal
        res+=(x[i]-'0')*mul;
        mul*=2;
    }
    return res;
}
// custom functions for conversion of decimal to required bits of binary
string decto2bin(int n){
    string temp="",res="";
    while(n){
        temp+=to_string(n%2);
        n/=2;
    }
    reverse(temp.begin(),temp.end());
    for(int i=0;i<8-temp.size();i++){
        res+='0';
    }
    res+=temp;
    return res;
}
string decto6bin(int n){
    string temp="",res="";
    if(n<0){
        n*=-1;
        while(n){
            temp+=to_string(n%2);
            n/=2;
        }
        reverse(temp.begin(),temp.end());
        for(int i=0;i<24-temp.size();i++){
            res+='0';
        }
        res+=temp;
        for(int i=0;i<res.size();i++){
            if(res[i]=='0'){
                res[i]='1';
            }
            else{
                res[i]='0';
            }
        }
        int carry=1;
        for(int i=res.size()-1;i>=0;i--){
            if(carry==1){
                if(res[i]=='1'){
                    res[i]='0';
                }
                else{
                    res[i]='1';
                    carry='0';
                }
            }
        }
        return res;
    }
    while(n){
        temp+=to_string(n%2);
        n/=2;
    }
    reverse(temp.begin(),temp.end());
    for(int i=0;i<24-temp.size();i++){
        res+='0';
    }
    res+=temp;
    return res;
}
string decto8bin(int n){
    string temp="",res="";
    if(n<0){
        n*=-1;
        while(n){
            temp+=to_string(n%2);
            n/=2;
        }
        reverse(temp.begin(),temp.end());
        for(int i=0;i<32-temp.size();i++){
            res+='0';
        }
        res+=temp;
        for(int i=0;i<res.size();i++){
            if(res[i]=='0'){
                res[i]='1';
            }
            else{
                res[i]='0';
            }
        }
        int carry=1;
        for(int i=res.size()-1;i>=0;i--){
            if(carry==1){
                if(res[i]=='1'){
                    res[i]='0';
                }
                else{
                    res[i]='1';
                    carry='0';
                }
            }
        }
        return res;
    }
    while(n){
        temp+=to_string(n%2);
        n/=2;
    }
    reverse(temp.begin(),temp.end());
    for(int i=0;i<32-temp.size();i++){
        res+='0';
    }
    res+=temp;
    return res;
}
//custom functions to convert decimal to required bits of hex
string valto6hex(int x){
    if(x<0){
        string res="";
        stringstream ss;
        ss<<hex<<(-x);
        string temp=ss.str();
        for(int i=0;i<6-temp.size();i++){
            res+="f";
        }
        for(int i=0;i<temp.size();i++){
            if(temp[i]-'0'>9){
                res+=hexval['f'-temp[i]];
            }
            else{
                res+=hexval[15-temp[i]+'0'];
            }
        }
        int carry=1;
        for(int i=5;i>=0;i--){
            if(res[i]!='f'){
                res[i]=res[i]+carry;
                carry=0;
            }
            else{
                if(carry==1){
                    res[i]='0';
                }
            }
        }
        return res;
    }
    stringstream ss;
    ss<<hex<<x;
    string temp=ss.str();
    string res="";
    for(int i=0;i<6-temp.size();i++){
        res+="0";
    }
    res+=temp;
    return res;
}
string valto8hex(int x){
    if(x<0){
        string res="";
        stringstream ss;
        ss<<hex<<(-x);
        string temp=ss.str();
        for(int i=0;i<8-temp.size();i++){
            res+="f";
        }
        for(int i=0;i<temp.size();i++){
            if(temp[i]-'0'>9){
                res+=hexval['f'-temp[i]];
            }
            else{
                res+=hexval[15-temp[i]+'0'];
            }
        }
        int carry=1;
        for(int i=7;i>=0;i--){
            if(res[i]!='f'){
                res[i]=res[i]+carry;
                carry=0;
            }
            else{
                if(carry==1){
                    res[i]='0';
                }
            }
        }
        return res;
    }
    stringstream ss;
    ss<<hex<<x;
    string temp=ss.str();
    string res="";
    for(int i=0;i<8-temp.size();i++){
        res+="0";
    }
    res+=temp;
    return res;
}
int octtodec(string x){
    int res=0,mul=1;
    for(int i=x.size()-1;i>=2;i--){
        res+=(x[i]-'0')*mul;
        mul*=8;
    }
    return res;
}
//function to check if a given input is number
int hextodec(string x){
    int res=0,mul=1;
    for(int i=x.size()-1;i>=2;i--){
        if(x[i]-'0'>9){
            res+=(x[i]-'a'+10)*mul;
        }
        else{
            res+=(x[i]-'0')*mul;
        }
        mul*=16;
    }
    return res;
}
bool checkifnum(string s){
    bool flag=1;
    int i=0;
    for(auto ch:s){
        if(i==0 && ch=='-'){
            i++;
            continue;
        }
        if(ch-'0'>=0 && ch-'0'<=9){
            continue;
        }
        else{
            flag=0;
            break;
        }
    }
    return flag;
}
void lines(vector<string> &filelines, string fname){  //getting the non-empty lines from file
    fstream fobj;
    fobj.open(fname);
    string line;
    while(getline(fobj,line)){
        line=ltrim(line);
        line=rtrim(line);
        if(line!=""){
            filelines.push_back(line);
        }
    }
}
void opinfo(map<string,pair<int,int>> &instructions){ //classifying the type of instruction for convenience
    //{common name, {opcode,operand}}
    // 0 = No Value/Offset
    // 1 = Value
    // 2 = Offset
    instructions["data"]={-1, 1};
    instructions["SET"]={-2, 1};
    instructions["ldc"]={0, 1};
    instructions["adc"]={1, 1};
    instructions["ldl"]={2, 2};
    instructions["stl"]={3, 2};
    instructions["ldnl"]={4, 2};
    instructions["stnl"]={5, 2};
    instructions["add"]={6, 0};
    instructions["sub"]={7, 0};
    instructions["shl"]={8, 0};
    instructions["shr"]={9, 0};
    instructions["adj"]={10, 1};
    instructions["a2sp"]={11, 0};
    instructions["sp2a"]={12, 0};
    instructions["call"]={13, 2};
    instructions["return"]={14, 0};
    instructions["brz"]={15, 2};
    instructions["brlz"]={16, 2};
    instructions["br"]={17, 2};
    instructions["HALT"]={18, 0};
}
void reading(string line,int lineno, vector<vector<string>> &errors, vector<string> &extras, vector<string> &labels){ //reading the lines from the file
    string opcode="",operand="",labelname="";
    int indexofcolon=-1,flag2=1;
    extras[lineno]="NO";
    if(find(line.begin(),line.end(),';')==line.end()){
        if(find(line.begin(),line.end(),':')!=line.end()){
            for(int i=0;i<line.size();i++){  //checking the condition for labels and comments
                if(line[i]!=':'){
                    if(line[i]==' '){
                        continue;
                    }
                    labelname+=line[i];
                }
                else{
                    indexofcolon=i;
                    break;
                }
            }
        }
    }
    else{
        int integer1=find(line.begin(),line.end(),':')-line.begin();
        int integer2=find(line.begin(),line.end(),';')-line.begin();
        if(integer2>integer1){
            for(int i=0;i<line.size();i++){
                if(line[i]!=':'){
                    if(line[i]==' '){
                        continue;
                    }
                    labelname+=line[i];
                }
                else{
                    indexofcolon=i;
                    break;
                }
            }
        }
    }
    labels[lineno]=labelname;
    if(labelname!=""){  //handling the label errors
        for(int i=0;i<labelname.size();i++){
            if(!isalnum(labelname[i]) && labelname[i]!='_'){
                flag2=0;
                break;
            }
        }
        if(((labelname[0]-'0'>=0 && labelname[0]-'0'<=9) || flag2==0) && line[0]!=';'){
            errors[lineno].push_back("Bogus label name");
            errorflag=1;
        }
        else{
            if(find(uniquelabels.begin(),uniquelabels.end(),labelname)==uniquelabels.end()){
                uniquelabels.insert(labelname);
            }
            else{
                errors[lineno].push_back("Duplicate label definition");
                errorflag=1;
            }
        }
    }
    int end=line.size()-1;
    if(find(line.begin(),line.end(),';')!=line.end()){
        end=find(line.begin(),line.end(),';')-line.begin();
    }
    //storing the operand and opcode and extra char on each line
    for(int i=indexofcolon+1;i<=end;i++){
        if(line[i]!=' ' && opcode==""){
            while(line[i]!=' '){
                if(line[i]==';'){
                    break;
                }
                if(i==end+1){
                    break;
                }
                opcode+=line[i];
                i++;
            }
        }
        else if(line[i]!=' ' && opcode!="" && operand==""){
            while(line[i]!=' '){
                if(line[i]==';'){
                    break;
                }
                if(i==end+1){
                    break;
                }
                operand+=line[i];
                i++;
            }
        }
        else if(line[i]!=' ' && opcode!="" && operand!="" && line[i]!=';'){
            extras[lineno]="YES";
        }
    }
    if(errors[lineno].size()==0 && operand!=""){
        labelvalues[labelname]={operand,opcode}; //storing the labelvalues to specially handle for SET
    }
    labellineno[labelname]=lineno;
    opcodes.push_back(opcode);
    operands.push_back(operand);
}
void checkerror(vector<vector<string>> &errors, vector<string> &extras, vector<string> &labels){ //storing errors in each line
    for(int i=0;i<errors.size();i++){
        string pcstring=valto8hex(pc); //storing the program counter
        pcs.push_back(pcstring);
        if(opcodes[i]!=""){
            pc++;
        }
        if(instructions.find(opcodes[i])==instructions.end() && opcodes[i]!=""){
            errors[i].push_back("Bogus mnemonic"); //if not valid opcode
            errorflag=1;
        }
        else{
            if(instructions[opcodes[i]].second==0){
                if(operands[i]!=""){
                    errors[i].push_back("Unexpected operand"); //extra operand
                    errorflag=1;
                }
            }
            else if(instructions[opcodes[i]].second==1){
                if(operands[i]==""){
                    errors[i].push_back("Missing operand"); //operand not given
                    errorflag=1;
                    continue;
                }
                if(extras[i]=="YES"){
                    errors[i].push_back("Extra on end of line"); //extra char on line
                    errorflag=1;
                }
                if(labelvalues.find(operands[i])!=labelvalues.end()){
                    if(labelvalues[operands[i]].first[0]=='0' && labelvalues[operands[i]].second[0]=='b'){
                        labelvalues[operands[i]].first=to_string(bintodec(labelvalues[operands[i]].first)); //converting the labelvalues to integer in SET
                    }
                    else if(labelvalues[operands[i]].first[0]=='0' && labelvalues[operands[i]].second[0]=='x'){
                        labelvalues[operands[i]].first=to_string(hextodec(labelvalues[operands[i]].first));
                    }
                    else if(labelvalues[operands[i]].first[0]=='0' && labelvalues[operands[i]].second[0]=='o'){
                        labelvalues[operands[i]].first=to_string(octtodec(labelvalues[operands[i]].first));
                    }
                }
                else{
                    if(operands[i][0]=='0' && operands[i][1]=='b'){
                        operands[i]=to_string(bintodec(operands[i]));
                    }
                    else if(operands[i][0]=='0' && operands[i][1]=='x'){ //chekcing if operand is a valid number if not SET
                        operands[i]=to_string(hextodec(operands[i]));
                    }
                    else if(operands[i][0]=='0' && operands[i][1]=='o'){
                        operands[i]=to_string(octtodec(operands[i]));
                    }
                    if(checkifnum(operands[i])==false){
                        errors[i].push_back("Not a number");
                        errorflag=1;
                    }
                }
            }
            else if(instructions[opcodes[i]].second==2){
                if(operands[i]==""){
                    errors[i].push_back("Missing operand");
                    errorflag=1;
                }
                if(operands[i][0]=='0' && operands[i][1]=='b'){
                    operands[i]=to_string(bintodec(operands[i]));
                }
                else if(operands[i][0]=='0' && operands[i][1]=='x'){
                    operands[i]=to_string(hextodec(operands[i])); //checking if number is given
                }
                else if(operands[i][0]=='0' && operands[i][1]=='o'){
                    operands[i]=to_string(octtodec(operands[i]));
                }
                if(checkifnum(operands[i])==0){
                    if(find(uniquelabels.begin(),uniquelabels.end(),operands[i])==uniquelabels.end()){ //checking if valid label given in offset
                        errors[i].push_back("No such label");
                        errorflag=1;
                    }
                }
            }
        }
    }
}
string opcodetohex(int x){
    stringstream ss;
    ss<<hex<<x;
    string res=ss.str(); //function to get opcode in hex
    if(x>=16){
        return res;
    }
    else{
        return "0"+res;
    }
}
void printerror(string errorfile, vector<vector<string>> errors){
    fstream fobj;
    fobj.open(errorfile,fstream::out);
    for(int i=0;i<errors.size();i++){ //printing error in the log file
        if(errors[i].size()==0){
            fobj<<"No error"<<endl;
        }
        else{
            fobj<<errors[i]<<endl;
        }
    }
}
void listingfile(string filename, vector<string> &filelines, vector<string> labels, vector<vector<string>> errors){ //making the listing file
    if(errorflag==1){ //calling the error file if errors are there
        filename+=".log";
        printerror(filename,errors);
        return;
    }
    string binfile=filename+".o";
    string logfile=filename+".log";
    filename+=".lst";
    fstream fobj,fobj2,fobj3;
    fobj.open(filename,fstream::out);
    fobj2.open(binfile,fstream::out);
    fobj3.open(logfile,fstream::out);
    fobj3<<"No errors found"<<endl;
    for(int i=0;i<filelines.size();i++){
        if(labels[i]=="" && opcodes[i]==""){
            fobj<<pcs[i]<<" "<<"        "<<" "<<filelines[i]<<" Just a comment"<<endl; //giving required warnings
        }
        else if(labels[i]!="" && opcodes[i]==""){
            fobj<<pcs[i]<<" "<<"        "<<" "<<filelines[i]<<" WARNING: unused label"<<endl;
        }
        else if(opcodes[i]!=""){
            if(instructions[opcodes[i]].second==0){
                if(opcodes[i]=="HALT"){
                    checkhalt=1;
                }
                fobj<<pcs[i]<<" "<<"000000"<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //writing into the files as required by converison into hex by the custom functions
                fobj2<<"000000000000000000000000"<<decto2bin(instructions[opcodes[i]].first)<<"00";
            }
            else if(instructions[opcodes[i]].second==1){
                if(opcodes[i]=="SET" || opcodes[i]=="data"){ //handling SET and data separately
                    fobj<<pcs[i]<<" ";
                    int conver=stoi(operands[i]);
                    fobj<<valto8hex(conver)<<" "<<filelines[i]<<endl;
                    fobj2<<decto8bin(conver);
                    if(opcodes[i]=="SET"){
                        fobj2<<"10"; //additional bits passed to binary file to differentiate between SET and data and other instructions
                    }
                    else{
                        fobj2<<"11";
                    }
                }
                else{
                    if(labelvalues[operands[i]].second=="SET"){
                        fobj<<pcs[i]<<" ";
                        int conver=stoi(labelvalues[operands[i]].first);
                        fobj<<valto6hex(conver)<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //substituting the value if SET
                        fobj2<<decto6bin(conver)<<decto2bin(instructions[opcodes[i]].first)<<"00";
                    }
                    else{
                        if(checkifnum(operands[i])){
                            int conver=stoi(operands[i]);
                            fobj<<pcs[i]<<" "<<valto6hex(conver)<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //if number then convert to hex
                            fobj2<<decto6bin(conver)<<decto2bin(instructions[opcodes[i]].first)<<"00";
                        }
                        else{
                            string temp="";
                            for(int j=2;j<8;j++){
                                temp+=pcs[labellineno[operands[i]]][j];
                            }
                            fobj<<pcs[i]<<" "<<temp<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //else take the address as hex
                            int conver=hextodec(pcs[labellineno[operands[i]]]);
                            fobj2<<decto6bin(conver)<<decto2bin(instructions[opcodes[i]].first)<<"00";
                        }
                    }
                }
            }
            else{
                if(checkifnum(operands[i])==1){
                    fobj<<pcs[i]<<" ";
                    int conver=stoi(operands[i]);
                    fobj<<valto6hex(conver)<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //if number take hex
                    fobj2<<decto6bin(conver)<<decto2bin(instructions[opcodes[i]].first)<<"00";
                }
                else{
                    fobj<<pcs[i]<<" ";
                    int con=hextodec(pcs[i]);
                    int conv=hextodec(pcs[labellineno[operands[i]]]);
                    int conver=conv-con-1;
                    fobj<<valto6hex(conver)<<opcodetohex(instructions[opcodes[i]].first)<<" "<<filelines[i]<<endl; //taking the difference in prog counters
                    fobj2<<decto6bin(conver)<<decto2bin(instructions[opcodes[i]].first)<<"00";
                }
            }
        }
    }
    if(checkhalt==0){
        fobj<<"WARNING: No HALT mnemonic found to terminate the emulator"<<endl;
    }
}
int main(){
    string fname,filename="";
    cout<<"Enter your file name: ";
    cin>>fname;
    for(int i=0;i<fname.size();i++){ //opening the file
        if(fname[i]=='.'){
            break;
        }
        else{
            filename+=fname[i];
        }
    }
    vector<string> filelines;
    lines(filelines,fname);
    opinfo(instructions);
    vector<vector<string>> errors(filelines.size()); 
    vector<string> extras(filelines.size()),labels(filelines.size());
    for(int i=0;i<filelines.size();i++){
        reading(filelines[i],i,errors,extras,labels);
    }
    char digit='0',charac='a';
    for(int i=0;i<15;i++){ //hex map for easy conversion
        if(i<10){
            hexval[i]=digit++;
        }
        else{
            hexval[i]=charac++;
        }
    }
    checkerror(errors,extras,labels); //calling the functions
    listingfile(filename,filelines,labels,errors);
}