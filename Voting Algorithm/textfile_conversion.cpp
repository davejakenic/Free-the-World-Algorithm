#include"textfile_conversion.hpp"
#include<fstream>
#include<bitset>
#include<limits>
#include <sstream>

void stream_from_bits(std::ostream& out, std::vector<bool> const& v){
    unsigned long int const n=v.size();
    std::string bitstr(n,'x');
    for(unsigned long int i=0;i<n;i++){ if( v[i] ){ bitstr[i] = '1'; }else{ bitstr[i] = '0'; } }
    for(unsigned long int i=0;i<bitstr.size();i+=std::numeric_limits<unsigned char>::digits)
    {unsigned char b = std::bitset<std::numeric_limits<unsigned char>::digits>(bitstr.substr(i,std::numeric_limits<unsigned char>::digits)).to_ulong();out<<b; }
}

void bits_from_stream(std::vector<bool>& v, std::istream& in){
    std::string bitstring;
    char c;
    while(in.get(c)){ bitstring+=std::bitset<std::numeric_limits<unsigned char>::digits>((unsigned char)(c)).to_string(); }
    unsigned long int const n=bitstring.size();
    v.clear(); v.reserve(n);
    for(unsigned long int i=0;i<n;i++){ v.push_back( bitstring.compare(i,1,"0") ); }
}

void bits_from_string(std::vector<bool>& v, std::string const& str){
    std::stringstream s( str, std::ios::binary );
    bits_from_stream(v,s);
}

void bits_from_file(std::vector<bool>& v, std::string const& path_to_file){
    std::ifstream s( path_to_file, std::ios::binary );
    bits_from_stream(v,s);
}

void string_from_bits(std::string& str, std::vector<bool> const& v){
    std::stringstream s;
    stream_from_bits(s,v);
    str=s.str();
}

void file_from_bits(std::string const& path_to_file, std::vector<bool> const& v){
    std::ofstream s( path_to_file, std::ios::binary );
    stream_from_bits(s,v);
    s.close();
}