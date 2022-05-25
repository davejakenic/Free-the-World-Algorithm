#ifndef TEXTFILE_CONVERSION_HPP
#define TEXTFILE_CONVERSION_HPP

#include<vector>
#include<string>

// convert path-to-text-file into vector<bool>
void bits_from_file(std::vector<bool>&, std::string const&);

// convert vector<bool> into path-to-text-file
void file_from_bits(std::string const&, std::vector<bool> const&);

// converts string into vector<bool>
void bits_from_string(std::vector<bool>&, std::string const&);

// convert vector<bool> into string
void string_from_bits(std::string&, std::vector<bool> const&);

#endif