#include "./store.h"

bool STORE::ifExist(string el) {
    // cout<<this->declarations.find(el)->second<<endl;
    this->IT = this->declarations.find(el);
    // cout<<IT->second<<endl;
    if(this->IT == this->declarations.end()){
        return false;
    } else {
        return true;
    }
}
bool STORE::insert(string key , string val){
    this->ret = this->declarations.insert (pair<string,string>(key,val));
    if (ret.second==false) {
        cout << "element " << key <<" already existed"<<endl;
        return false;
    } else {
        return true;
    }  
}
void STORE::print(){
     for (IT=declarations.begin(); IT!=declarations.end(); ++IT)
    std::cout <<"| "<<IT->first <<" | "<< " => " <<"| " <<IT->second <<" |"<< '\n';
}

string STORE::ifType(string el){
    this->IT = this->declarations.find(el);
    if(this->IT == this->declarations.end()){
        return "";
    } else {
        return this->IT->second;
    }
}
