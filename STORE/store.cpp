#include "./store.h"

bool STORE::ifExist(string el) {
    this->IT = this->declarations.find(el);
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
    std::cout << IT->first << " => " << IT->second << '\n';
}
