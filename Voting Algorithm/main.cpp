/*

g++ main.cpp hash.cpp textfile_conversion.cpp
./a.exe

*/

#include<iostream>
#include<iomanip>
#include<set>
#include<vector>
#include<fstream>
#include<numeric>
#include"hash.hpp"
#include"textfile_conversion.hpp"

int main();

using std::pair;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::istream;
using std::to_string;
using std::cout;
using std::endl;

int PROOF_OF_WORK_DIFFICULTY    = 3;    // number of leading zeros in hash-digest in hex format
int RSA_KEY_LENGTH              = 50;   // bytes of e
int CHALLENGE_DIFFICULTY        = 2;    // number of bits to guess from AES key

// python interface
void python_function(string const& s, string const& args, ifstream& in){
    system(("python python\\"+s+".py "+args).c_str()); in.open("python\\tmp\\"+s+".txt"); // call python script and load output
}

// set compatibility
template<typename T>
bool does_set_contain(std::set<T> const& s, T const& x){
    return s.find(x)!=s.end();
}

// basic data types
struct hex{ string s="-1"; };
struct num{ string s="-1"; };
struct key{
    num  d,n;     // key for RSA asymmetric cryptography
    string to_string(void)const{ return d.s+"_"+n.s; }
};

// compare strings
bool     operator==(string const& a, string const& b){ 
    return a.size()==b.size() and a.compare(0,a.size(),b)==0; 
}
bool     operator!=(string const& a, string const& b){ 
    return not(a==b); 
}
bool     operator==(hex    const& a, hex    const& b){ 
    return a.s==b.s; 
}
bool     operator!=(hex    const& a, hex    const& b){ 
    return a.s!=b.s; 
}
bool     operator==(num    const& a, num    const& b){ 
    return a.s==b.s; 
}
bool     operator!=(num    const& a, num    const& b){ 
    return a.s!=b.s; 
}
ostream& operator<<(ostream     & o, num    const& x){ 
    o << x.s; return o; 
}
istream& operator>>(istream     & i, num         & x){ 
    i >> x.s; return i; 
}
ostream& operator<<(ostream     & o, hex    const& x){ 
    o << x.s; return o; 
}
istream& operator>>(istream     & i, hex         & x){ 
    i >> x.s; return i; 
}
bool     operator> (num    const& a, num    const& b){
    ifstream in; python_function("compare_num",a.s+" "+b.s,in); int i; in>>i; return (i==1);     // compare_num(a,b) is true when a>b
}
bool     operator< (num    const& a, num    const& b){
    return (a!=b) and (not (a>b));
}
bool     operator> (key    const& a, key    const& b){
    return a.n>b.n;     // the key with bigger n will conserve more information
}
num      operator+ (num    const& a, num    const& b){
    ifstream in; python_function(     "add_num",a.s+" "+b.s,in); num x; in>>x.s; return x;
}
num      operator- (num    const& a, num    const& b){
    ifstream in; python_function("subtract_num",a.s+" "+b.s,in); num x; in>>x.s; return x;
}

num    hex_to_num   (hex const& h){ 
    ifstream in; python_function("hex_to_num",h.s,in); num x; in>>x.s; return x;
}
string hex_to_string(hex const& h){
    return h.s; 
}
string num_to_string(num const& x){
    return x.s; 
}

num random_num(int L){
    ifstream in; python_function("random_num","1 "+to_string(L),in); num x; in >> x.s; return x; 
}
int random_int(int i0, int iE){
    ifstream in; python_function("random_int","1 "+to_string(i0)+" "+to_string(iE),in); int i; in >> i; return i;
}

void shuffle(vector<int>& p){
    int n=p.size(); vector<bool> vb(n,false); vector<int > q=p;
    for(int i=0;i<p.size();++i){
        int j=random_int(0,p.size()); while( vb[j] ){ j=(j+1)%n; }
        p[j]=q[i]; vb[j]=true; // copy each in a different unoccupied entry
    }
}

// basic cryptography
hex hash_of_string(string const& s       ){
    hex h; h.s = hash(s); return h; 
}
hex hash_of_file  (string const& filename){
    std::vector<bool> v; string s; bits_from_file(v,filename); string_from_bits(s,v); hex h; h.s = hash(s); return h; 
}

num        powk(hex const& h              ){
    ifstream in; python_function(      "proof_of_work",h.s+" "        +to_string(PROOF_OF_WORK_DIFFICULTY),in); num x; in>>x.s; return x; 
}
bool check_powk(hex const& h, num const& x){
    ifstream in; python_function("check_proof_of_work",h.s+" "+x.s+" "+to_string(PROOF_OF_WORK_DIFFICULTY),in); int i; in>>i  ; return (i==1); 
}

pair<key,key> generate_keys(void                      ){ 
    ifstream in; python_function("generate_keys",to_string(RSA_KEY_LENGTH),in); 
    key k1,k2; in >> k1.d.s; in >> k1.n.s; in >> k2.d.s; in >> k2.n.s; in.close(); return pair<key,key>(k1,k2);
}
num                   crypt(num const& x, key const& k){
    ifstream in; python_function("crypt",x.s+" "+k.d.s+" "+k.n.s,in); num y; in>>y.s; return y; 
}

string  make_challenge(num    const& x){
    //return x.s; // TODO
    ifstream in; python_function( "make_challenge",x.s+" "+to_string(CHALLENGE_DIFFICULTY),in); string s; in>>  s; return s;
}
num    solve_challenge(string const& s){
    //num x; x.s = s; return x;   // TODO
    ifstream in; python_function("solve_challenge","\""+s+"\" "+to_string(CHALLENGE_DIFFICULTY),in); num    y; in>>y.s; return y;
}

// immutable pool, listing all vote-eligible names with their public keys; can be issued by passport-maker or web-of-trust
class pool{
    private:
        vector<pair<string,key>>    v={};
        friend int main();
    public:
        vector<string> get_names_except(string name)const{
            vector<string> w;
            for(int i=0;i<v.size();++i)
                if(v[i].first!=name)
                    w.push_back(v[i].first);
            return w;
        }
        bool           find_id(string s, key& k)    const{
            for(int i=0;i<v.size();++i){
                if( v[i].first==s ){
                    k = v[i].second;
                    return true;
                }
            }
            return false;
        }
        string         hash (void)                  const{
            string s="";
            for(int i=0;i<v.size();++i) s +=v[i].first+v[i].second.to_string();
            return ::hash(s);
        }
        void           print(void)                  const{
            for(int i=0;i<v.size();++i) 
                cout << "   =============: " << i << "\n   name.........: " << v[i].first << "\n   key.d........: " 
                     << v[i].second.d << "\n   key.n........: " << v[i].second.n << "\n";
            cout << "   =============: " << v.size() << "\n";
        }
};

// messages that can be posted on the internet
class message{
    public:
        string              author,subject,body="-";
        num                 p1,p2,p3,p4;    // for pairing transfer
        vector<string>      vf = {};        // vector of filename; for attachments
        num                 pow,signature;
        mutable hex         digest;
    public:
        void      print(void)                       const{
            cout<< std::left << "     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n     ~ author   : " 
                << std::setw(40) << author << "  ~\n     ~ subject  : " << std::setw(40) << subject << "  ~\n"
                << "     ~ body     : " << std::setw(40) << body     << "  ~\n     ~ p1       : " << std::setw(40) 
                << p1 << "  ~\n     ~ p2       : " << std::setw(40) << p2 << "  ~\n     ~ p3       : " << std::setw(40) << p3 
                << "  ~\n     ~ p4       : " << std::setw(40) << p4<< "  ~\n";
            for(int i=0;i<vf.size();++i)
                cout<< "     ~ attached : " << std::setw(40) << vf[i]    << "  ~\n";
            cout<< "     ~ digest   : " << std::setw(40) << hex_to_string(digest)<< "\n     ~ sign     : " << std::setw(40) << signature
                << "  ~\n     ~ powk     : " << std::setw(40) << pow      << "  ~\n     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        }
        void       hash(void)                       const{
            string s = author+subject+body+to_string(vf.size())+num_to_string(p1)
            +num_to_string(p2)+num_to_string(p3)+num_to_string(p4); digest = hash_of_string(s);
            for(int i=0;i<vf.size();++i) digest = hash_of_string( hex_to_string(digest) + hex_to_string(hash_of_file(vf[i])) );
        }
        void       powk(void)                            {
            hash(); pow = ::powk(digest);
        }
        bool check_powk(void)                       const{
            hash(); return ::check_powk(digest,pow);
        }
        void       sign(key k)                           {
            hash(); signature = crypt(hex_to_num(digest),k);
        }
        bool check_sign(pool const& p)              const{
            hex d=digest; this->hash(); if(d!=digest) return false; // reject because digest is incorrect
            key k;   if( not( p.find_id(author,k) ) ) return false; // author does not exist in pool
            return ( hex_to_num(digest)==crypt(signature,k) );      // verify that public encryption of signature matches with digest
        }
        bool find_attachment(string const& filename)const{
            for(int i=0;i<vf.size();++i)
                if(vf[i]==filename)
                    return true;
            return false;
        }
};

// a public space on the internet
class messageBoard{
    public:
        pool const&         P;
    private:
        vector<message>     v = {};
    public:
        messageBoard  (pool    const& P)                          :P(P){}
        bool push_back(message const& m)                               {
            message e;
            if( ( m.check_sign(P) or m.check_powk() ) ){
                v.push_back(m); return true;
            }else{
                return false; // has neither signature nor pow
            }
        }
        bool find     (message      & m,                string subject){
            string a,s;
            for(int i=0;i<v.size();++i)
                if( v[i].subject==subject ){
                    m = v[i]; return true;
                }
            return false;
        }
        bool find     (message      & m, string author, string subject){
            string a,s;
            for(int i=0;i<v.size();++i)
                if( v[i].subject==subject and v[i].author==author ){
                    m = v[i]; return true;
                }
            return false;
        }
        void print    (void)                                      const{
            cout << "MessageBoard with " << v.size() << " messages:\n";
            for(int i=0;i<v.size();++i){ v[i].print(); cout << "\n"; }
        }
        message const& operator[](int i)const{
            if(i<0) i=0;
            if(i>v.size()) i=v.size()-1;
            return v[i];
        }
        int get_num_messages(void)const{return v.size();}
};

// recipe structures
class subRecipe{
    public:
        vector<num> const&              L_real;
        int                             n,k,L,ref,x;    // length,fakes,bits,index of L_real between fake lists
        vector<vector<num>>             v = {};         // fakes
        vector<int>                     p = {};         // permutation of L_real
        //
    public:
        subRecipe   (              vector<num>const& L_real, int n, int k, int L, int ref):L_real(L_real),n(n),k(k),L(L),ref(ref),p(n){
            for(int j=0;j<k;++j){
                vector<num> w;
                for(int i=0;i<n;++i)
                    w.push_back( random_num(L) );       // no check for redundants; trust in unlikeliness; requires suff. large L
                v.push_back(w);
            }
            std::iota(std::begin(p),std::end(p),0);     // fill p={0,1,2,...}
            shuffle(p); x=random_int(0,k+1);
        }
        subRecipe   (ifstream& in, vector<num>const& L_real)                              :L_real(L_real)                        {
            in >> n;
            in >> k;
            in >> L;
            in >> ref;
            in >> x;
            for(int j=0;j<k;++j){
                vector<num> w;
                for(int i=0;i<n;++i){
                    num t;
                    in >> t;
                    w.push_back(t);
                }
                v.push_back(w);
            }
            for(int i=0;i<n;++i){
                int t;
                in >> t;
                p.push_back(t);
            }
        }
        void to_file(ostream& out)                                                                                          const{
            out << n << "\n" << k << "\n" << L << "\n" << ref << "\n" << x << "\n";
            for(int j=0;j<k;++j)
                for(int i=0;i<n;++i)
                    out << v[j][i] << "\n";
            for(int i=0;i<n;++i)
                out << p[i] << "\n";
        }
        void   print(void)                                                                                                  const{
            for(int q=0;q<k;++q){
                cout << "fake list " << q << " : \n    ";
                for(int i=0;i<n;++i){
                    cout << v[q][i] << " ";
                }
                cout << "\n-----------------------------------------\n";
            }
            cout << "permutation : \n    ";
            for(int i=0;i<n;++i){
                cout << p[i] << " ";
            }
            cout << "\n";
        }
};
class    recipe{
    public:
        pool const&             P;
        int                     n,k,L,ref; // length,fakes,bits
        vector<string>          names;
        vector<string>          vKeyString;
        vector<num>             L_real = {};
        vector<subRecipe>       v = {};
        hex                     digest;
    public:
        recipe(pool const& P, int n, int k, int L, int ref, vector<string> const& names, vector<key> const& keys):P(P),n(n),k(k),L(L),ref(ref),names(names){
            for(int i=0;i<n;++i)
                L_real.push_back( random_num(L) );
            for(int i=0;i<names.size();++i)
                v.push_back( subRecipe(L_real,n,k,L,ref) );
            for(int i=0;i<keys.size();++i){
                vKeyString.push_back( keys[i].to_string() );
            }
            this->hash();
        }
        recipe(pool const& P, string filename)                                          :P(P)                                     {
            ifstream in(filename); int m; num q; string s;
            in >> n >> k >> L >> ref >> m;
            for(int i=0;i<m;++i){ in>>s; names     .push_back(s); }
            for(int i=0;i<m;++i){ in>>s; vKeyString.push_back(s); }
            for(int i=0;i<n;++i){ in>>q; L_real    .push_back(q); }
            for(int i=0;i<m;++i){        v         .push_back( subRecipe(in,L_real) ); }
            hash();
        }
        void to_file(string filename)const{
            ofstream out(filename); out << n << "\n" << k << "\n" << L << "\n" << ref << "\n" << names.size() << "\n";
            for(int i=0;i<names.size();++i) out <<      names[i] << "\n";
            for(int i=0;i<names.size();++i) out << vKeyString[i] << "\n";
            for(int i=0;i<n           ;++i) out << L_real[i] << "\n";
            for(int i=0;i<names.size();++i) v[i].to_file(out);
            out.close();
        }
        void   hash (void)                {
            to_file("hash_tmp\\tmp.txt"); digest = hash_of_file("hash_tmp\\tmp.txt");
        }
        void   print(void)           const{
            cout<< "###########RECIPE########################\ndimensions:\n"
                << "   list length...........: " << n <<"\n   number of fake lists..: " << k <<"\n"
                << "   bits per number.......: " << L <<"\n   voting reference......: " << ref <<"\n"
                << "===========POOL==========================\n";
            P.print(); cout    << "===========invited & engaging============\n";
            for(int i=0;i<names.size();++i) cout<< "   name["<<std::right<<std::setw(4)<<i<<"] : " << names[i] << "\n";
            cout<< std::left << "===========real List=====================\n    ";
            for(int i=0;i<n;++i) cout << L_real[i] << " ";
            cout    << "\n===========Sub===========================\n-----------------------------------------\n";
            for(int j=0;j<names.size();++j){ 
                cout<< "   for name=" << names[j] << "\n";
                v[j].print();
                cout<< "-----------------------------------------\n";
            }
            cout<< "#########################################\n\n";
        }
};

// bundle of lists for one voter
class package{
    public:
        string              name;
        int                 ref,n,k;
    public:
        //
        // for voter
        package(string const& filename, vector<num>& luckyNumbers, key const& keySec){
            // load one lucky number per list from package in filename
            ifstream in(filename); string s; luckyNumbers.clear(); in >> ref >> name >> k >> n;
            for(int j=0;j<=k;++j){
                int iP = random_int(0,n);
                for(int i= 0 ;i<iP;++i){ in>>s; }
                in>>s; luckyNumbers.push_back(    crypt( solve_challenge(s) ,keySec)    ); // only uncover one element per list
                for(int i=iP+1;i<n;++i){ in>>s; }
            }
        }
        //
        // for host
        package(recipe const& r):n(r.n),k(r.k){}
        void to_file(recipe const& r, int t, key const& keyPub, int ref, string& filename){
            subRecipe const& s(r.v[t]); this->ref=ref; this->name=r.names[t];
            filename    = "attachments\\package_for_"+name+"_for_vote_"+to_string(ref)+".txt"; ofstream out(filename);
            // write fake lists into file. put permuted real-list at random position x in-between the fake lists. each list element; first encrypt, then conceil (with challenge)
            out << ref << "\n" << name << "\n" << k << "\n" << n << "\n";
            for(int j=0;j<s.x;++j){
                for(int i=0;i<n;++i){
                    out << make_challenge( crypt(s.v[j][i]       ,keyPub) ) << "\n";
                }
            }
            for(int i=0;i<n;++i){
                    out << make_challenge( crypt(r.L_real[s.p[i]],keyPub) ) << "\n";
            }
            for(int j=s.x;j<k;++j){
                for(int i=0;i<n;++i){
                    out << make_challenge( crypt(s.v[j][i]       ,keyPub) ) << "\n";
                }
            }
            out.close();
        }
};

// anyone with an account
class node{
    public:
        string const            name;
    private:
        messageBoard&           W;
        pair<key,key>           autKeys;
        //
        // temporary data
        pair<key,key>           tmpKeys;
        //
        // temporary data for hosting
        vector<string>          participants;
        vector<key>             participantsKeys;
        recipe*                 r = NULL;
        int                     n=6, k=2, L=10;     // length of lists, number of fake lists, bits per list element
        //
        // temporary data for voting
        pair<string,key>        host;
        vector<pair<key,key>>   vKey = {};
        //
    public:
        node(string name, messageBoard& W):name(name),W(W){ autKeys = generate_keys(); }
        ~node(void){ if(r!=NULL) delete r; }
        pair<string,key> id(void)const{ return pair<string,key>(name,autKeys.first); }
        //
        bool invite(int ref){
            cout << "++++++ HOST["<<std::setw(20)<<name<<"] : invite ++++++\n";
            participants    = W.P.get_names_except(name);   // choose vote participants arbitrarily
            //
            tmpKeys = generate_keys();
            string filename = "attachments\\vote_invite_"+to_string(ref)+"_invited_participants.txt";
            ofstream out(filename); out << participants.size() << "\n"; for(int i=0;i<participants.size();++i) out << participants[i] << "\n"; out.close();// invitation list (attachment)
            message m; m.vf.push_back(filename);
            m.author    = name;
            m.subject   = "vote_invite_"+to_string(ref);
            m.body      = "sending my temporary public key: p1=e, p2=n";
            m.p1        = tmpKeys.first.d;
            m.p2        = tmpKeys.first.n;
            m.sign(autKeys.second);
            if( not W.push_back(m)){ cout << "Failure: Host " << name <<" fails at publishing invite ref="<<ref<<".\n"; return false; }// try sending
            return true;
        }
        bool engage(int ref){
            cout << "+++++ VOTER["<<std::setw(20)<<name<<"] : engage ++++++\n";
            message m_inv;
            if(not W  .find   (m_inv,"vote_invite_"+to_string(ref)) ){  // verify that invite message exists
                cout << "Voter "<< name << " received no invitation.\n"; return false;
            }
            if(not W.P.find_id(m_inv.author,host.second           ) ){  // verify host in pool
                cout << "Voter "<< name << " found that host "<<host.first<<" is no pool member.\n"; return false;
            }
            if(not             m_inv.check_sign(W.P)                ){  // verify the message's signature
                cout << "Voter "<<name<<" found that invite message has an invalid signature.\n"; return false;
            }
            if(                m_inv.vf.size()!=1                   ){  // verify the message's completeness (must have attachment)
                cout << "Voter "<<name<<" found that invite message lacks attachment.\n"; return false;
            }
            ifstream in(m_inv.vf[0]); string s; 
            while(std::getline(in,s)) if(s==name) break;                // search personal invite in attachment (line by line)
            if(s!=name){                                                // verify the host invited me personally
                cout << "Voter "<<name<<" finds no personal invitation in the attachment.\n"; return false;
            }
            tmpKeys = generate_keys(); host.first  = m_inv.author; host.second.d = m_inv.p1; host.second.n = m_inv.p2; // recv and send temp keys
            message m;
            m.author    = name;
            m.subject   = "vote_engage_"+to_string(ref);
            m.body      = "sending my temporary public key: p1=e, p2=n";
            m.p1        = tmpKeys.first.d;
            m.p2        = tmpKeys.first.n;
            m.sign(autKeys.second);
            if(not W.push_back(m)){ cout << "Failure: Voter " << name <<" fails at publishing engage ref="<<ref<<".\n"; return false; }// try sending reply
            return true;
        }
        bool spread(int ref){
            cout << "++++++ HOST["<<std::setw(20)<<name<<"] : spread ++++++\n";
            if(r!=NULL){                                                    // abort if there is already an open recipe
                cout << "Host "<<name<<" does already have an open recipe. -- While possible, this proof of concept code does not support parallel votes."; return false;
            }
            message m_eng; vector<string> engaging_participants;            // collect all voters' temporary public keys
            for(int i=0;i<participants.size();++i){
                if( W.find(m_eng,participants[i],"vote_engage_"+to_string(ref)) ){
                    key tk; engaging_participants.push_back(participants[i]); 
                    tk.d=m_eng.p1; tk.n=m_eng.p2; participantsKeys.push_back(tk);
                }
            }
            participants = engaging_participants;                           // forget those invited voters that did not respond
            r = new recipe(W.P,n,k,L,ref,participants,participantsKeys);    // generate recipe
            r->print();                                                     // for illustration purposes only! (the entire recipe is a secret of the host)
            message m_recipe;                                               // publish message with hash of recipe
            m_recipe.author     = name;
            m_recipe.subject    = "vote_spread_"+to_string(ref)+"_recipe_hash";
            m_recipe.body       = hex_to_string(r->digest);
            m_recipe.sign(autKeys.second);
            if(not W.push_back(m_recipe)){                                  // publish message with hash of recipe
                cout << "Failure: Host " << name <<" fails at publishing spread for ref="<<ref<<".\n"; return false;
            }
            message m_packages;                                             // create  message with all packages attached
            m_packages.author    = name;
            m_packages.subject   = "vote_spread_"+to_string(ref)+"_packages";
            string      filename;
            for(int i=0;i<participants.size();++i){                         // attach package for i-th voter
                package p(*r); p.to_file(*r,i,participantsKeys[i],ref,filename); m_packages.vf.push_back(filename);
            }
            m_packages.sign(autKeys.second);
            if(not W.push_back(m_packages)){                                // sign message with packages and publish
                cout << "Failure: Host " << name <<" fails at publishing package for ref="<<ref<<".\n"; return false;
            }
            return true;
        }               
        bool answer(int ref){
            cout << "+++++ VOTER["<<std::setw(20)<<name<<"] : answer ++++++\n";
            message m_spread; string filename = "attachments\\package_for_"+name+"_for_vote_"+to_string(ref)+".txt";
            if(not W.find(m_spread,"vote_spread_"+to_string(ref)+"_recipe_hash") ){                         // verify publication      of the recipe-hash
                cout << "Voter "<<name<<" does not see the recipe hash\n.";           return false;
            }
            if(not                       m_spread.check_sign(W.P)                ){                         // verify the signature under the recipe-hash
                cout << "Voter "<<name<<" found that recipe message has an invalid signature.\n"; return false;
            }
            if(not W.find(m_spread,"vote_spread_"+to_string(ref)+"_packages")    ){                         // verify publication      of the packages
                cout << "Voter "<<name<<" does not find the package message.\n";      return false;
            }
            if(not                       m_spread.check_sign(W.P)                ){                         // verify the signature under the packages
                cout << "Voter "<<name<<" found that packages message has an invalid signature.\n"; return false;
            }
            if(not m_spread.find_attachment(filename)                            ){                         // verify that packages are attached
                cout << "Voter "<<name<<" does not find his/her package attached.\n"; return false;
            }
            vector<num> luckyNumbers; package p(filename,luckyNumbers,tmpKeys.second);                      // load my package and exact one number per list into luckyNumbers
            k=luckyNumbers.size()-1; vKey.clear(); for(int j=0;j<=k;++j) vKey.push_back( generate_keys() ); // generate one key pair for each element in myNumbers[]
            vector<bool> v_sent(luckyNumbers.size(),false);                                                 // already published pairings
            for(int j=0;j<=k;++j){                                                                          // send all pairings separately in a random order
                int i = random_int(0,k+1); while( v_sent[i] ){ i = (i+1) % (k+1); }                         // random index whose lucky number has not been sent yet
                message m;
                m.author    = "anonymous";
                m.subject   = "pairing_for_vote_"+to_string(ref);
                m.p1        = vKey[i].first.d;                                                              // submit public vote key d
                m.p2        = vKey[i].first.n;                                                              // submit public vote key n
                m.p3        = random_num(10);                                                               // some salt to avoid that others read your lucky number
                m.p4        = crypt( luckyNumbers[i]+m.p3 , host.second );
                m.powk();
                if(not W.push_back(m) ){ cout << "Failure: Voter " << name <<" fails at publishing his/her answer to ref="<<ref<<" :\n"; m.print(); return false; }
                v_sent[i] = true;
            }
            return true;
        }
        bool reveal(int ref){
            cout << "++++++ HOST["<<std::setw(20)<<name<<"] : reveal ++++++\n";
            message m;
            m.author    = name;
            m.subject   = "reveal_for_"+to_string(ref);
            m.body      = "reveiling private temporary key as p1=d and p2=n"; // publish message with recipe and votekeys as attachment
            m.p1        = tmpKeys.second.d;
            m.p2        = tmpKeys.second.n;
            m.vf.push_back("attachments\\the_recipe.txt"); r->to_file("attachments\\the_recipe.txt");
            m.vf.push_back("attachments\\the_official_public_vote_keys.txt"); ofstream out("attachments\\the_official_public_vote_keys.txt");
            // traverse through all anonymous messages and decrypt each vote key; then check whether it is contained in L_real
            std::set<num> Set_Real(r->L_real.begin(),r->L_real.end());
            for(int q=0;q<W.get_num_messages();++q){
                if( W[q].author=="anonymous" and W[q].subject=="pairing_for_vote_"+to_string(ref) ){
                    num luckyX; key tk; tk.d=W[q].p1; tk.n=W[q].p2;
                    luckyX = crypt(W[q].p4,tmpKeys.second)-W[q].p3;
                    if( does_set_contain(Set_Real,luckyX) ){
                        out << "-----------------------------\nluckyX : " << luckyX << "\nkey.d  : " << tk.d << "\nkey.n  : " << tk.n << "\n";
                    }
                }
            }
            delete r; out.close();
            m.sign(autKeys.second); if(not W.push_back(m) ){cout << "Host "<<name<<" is unable to upload reveal message.\n"; return false; }
            return true;
        }
        //
        bool verify(int ref){
            bool acceptable_result = true;
            message m_spread, m_reveal; // load two messages from host
            cout<<"  xxxxxxxxxxx Verification xxxxxxxxxxxxxxxxxxx\n  x verifier : " << name << "\n";
            if(not W.find(m_spread,"vote_spread_"+to_string(ref)+"_recipe_hash") ){         // find spread message (we want to verify the hash of the recipe)
                cout<<"  x Message \'spread\' or \'reveal\' from host is missing.\n  x RESIGN: " << m_spread.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            if(not W.find(m_reveal,  "reveal_for_"+to_string(ref)              ) ){         // find reveal message (we want host's temporary private key, to unlock the pairings)
                cout<<"  x Message \'reveal\' or \'reveal\' from host is missing.\n  x RESIGN: " << m_spread.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            if(              m_spread.author!=m_reveal.author                    ){         // check that both messages have the same author (i.e., the host)
                cout<<"  x Message \'spread\' or \'reveal\' are from different authors.\n  x BAN: " << m_reveal.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            if(not m_spread.check_sign(W.P)                                      ){         // check signature of spread message
                cout<<"  x Message \'spread\' has invalid signature.\n  x RESIGN: " << m_reveal.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            if(not m_reveal.check_sign(W.P)                                      ){         // check signature of reveal message
                cout<<"  x Message \'reveal\' has invalid signature.\n  x RESIGN: " << m_reveal.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            r = new recipe(W.P,m_reveal.vf[0]);
            if( hex_to_string(r->digest)!=m_spread.body                          ){         // check hash of recipe with that in spread message
                cout<<"  x Wrong recipe hash.\n  x RESIGN: " << m_reveal.author << "\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; delete r; return false;
            }
            int m(r->names.size()), n(r->n), k(r->k), countReal=0;                          // number of: voters, list elements, fake lists; number of pairings from L_real
            key host_tmp_sec; host_tmp_sec.d=m_reveal.p1; host_tmp_sec.n=m_reveal.p2;       // obtain host's temporary private key
            message mi; vector<string> banNames; vector<vector<int>> countFake(m,vector<int>(k,0));
            // count number of votes; first compose sets
            std::set<num> Set_Real(r->L_real.begin(),r->L_real.end());                      // pack sets of real/fake list elements to channel lucky numbers faster
            vector<vector<std::set<num>>> Set_Fakes(m,vector<std::set<num>>(k,std::set<num>()));
            for(int i=0;i<m;++i) // for each paticipant
                for(int j=0;j<k;++j) // for each of his/her fake lists
                    Set_Fakes[i][j] = std::set<num>(r->v[i].v[j].begin(),r->v[i].v[j].end());
            // count number of votes; now traverse through all anonymous messages and decrypt each vote key
            for(int q=0;q<W.get_num_messages();++q)                                         // count each lucky number into "buckets" (aka, real and fakes)
                if( W[q].author=="anonymous" and W[q].subject=="pairing_for_vote_"+to_string(ref) ){
                    num luckyX; key tk; tk.d=W[q].p1; tk.n=W[q].p2; 
                    luckyX = crypt(W[q].p4,host_tmp_sec)-W[q].p3;
                    if( does_set_contain(Set_Real,luckyX) )
                        countReal++;
                    else
                        for(int i=0;i<m;++i)
                            for(int j=0;j<k;++j)
                                if( does_set_contain(Set_Fakes[i][j],luckyX) )
                                    countFake[i][j]++;
                }
            bool someone_tricked=false;                                                     // monitor results
            cout<<"  x table of number of pairings:\n  x   number of voters              : " << m << "\n  x   number of real-list pairings  : " << countReal << "\n";
            for(int j=0;j<m;++j){
                bool tricked=false; cout<<"  x   " << std::left << std::setw(29) << r->names[j] << " : "; 
                for(int q=0;q<k-1;++q){
                    if( countFake[j][q]>1 ) tricked = true; cout << std::setw(4) << countFake[j][q]   << ", ";
                }
                if( countFake[j][k-1]>1 )   tricked = true; cout << std::setw(4) << countFake[j][k-1] << ".";
                if(tricked){ someone_tricked = true; banNames.push_back(r->names[j]); cout << " reject\n"; }else{ cout << " accept\n"; }
            }
            delete r;
            if( banNames.size()>0 ){
                cout << "  x EXCLUDE: ";
                for(int q=0;q<banNames.size()-1;++q) cout << banNames[q] << ", ";
                cout<< banNames[banNames.size()-1] << ".\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            if( (not someone_tricked) and countReal>m ){
                cout<<"  x recipe was compromised.\n  x RESIGN: "<<m_reveal.author<<"\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"; return false;
            }
            cout<< "  x all numbers add up.\n  x SUCCESS\n  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n"; return true;
        }
};


// example
int main(){
    pool            P;      // voting pool
    messageBoard    W(P);   // public whiteboard (e.g. blockchain)
    node            A("Anton",W),B("Bobby",W),C("Carlo",W),G("Grace",W),O("Olive",W);               // voters male, host and observer female; all names unique
    P.v.push_back(A.id()); P.v.push_back(B.id()); P.v.push_back(C.id()); P.v.push_back(G.id());     // add voter and host to pool
    int ref = 33;   // voting reference
    
    // authentication anonymisation
    G.invite(ref);      A.engage(ref); B.engage(ref); C.engage(ref);
    G.spread(ref);      A.answer(ref); B.answer(ref); C.answer(ref);
    G.reveal(ref);      W.print();     O.verify(ref);
    
    return 0;
}

















