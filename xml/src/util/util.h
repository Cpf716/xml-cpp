//
//  util.h
//  xml
//
//  Created by Corey Ferguson on 9/29/26.
//

#ifndef util_h
#define util_h

#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <sstream>

// Non-Member Functions

// Return string_views copied to strings
std::vector<std::string> copy(const std::vector<std::string_view> value);

// Return the digits comprising a floating-point number; the decimal point is represented by INT_MAX
std::vector<int> digits(const double number);

/**
 * Return string escaped by double quotations
 */
std::string escape(const std::string string);

/**
 * Return true if value can be parsed into a floating-point number, otherwise return false
 */
bool is_number(const std::string value);

// Return true if b is a power of n
bool is_pow(const size_t b, const size_t n);

// Return values joined by delimiter
std::string join(std::vector<std::string> values, std::string delimeter);

/**
 * Merge double quotation-escaped tokens
 */
void merge(std::vector<std::string> &values, const std::string delimiter = "");

/**
 * Return value parsed into a floating-point number
 */
double parse_number(const std::string value);

/**
 * Return the next power of n for b
 * I.e. pow(15, 2) = 16
 */
int pow(const int b, const int n = 2);

// Return string split by delimiter; reference only
std::vector<std::string_view> split(const std::string_view string, const std::string delimeter);

// Return string split by delimiter; copy
std::vector<std::string> split(const std::string string, const std::string delimeter);

// Return string split by delimiter (legacy); copy
void split(std::vector<std::string> &target, const std::string source, const std::string delimeter);

// Return string split by whitespace; reference only
std::vector<std::string_view> tokens(const std::string_view string);

// Return string split by whitespace; copy
std::vector<std::string> tokens(const std::string string);

// Return string split by whitespace (legacy); reference only
void tokens(std::vector<std::string> &target, const std::string source);

// Return floating-point number formatted to min precision
std::string truncate_d(const double number, const int min_prec = 0);

/**
 * Unescape double quotation-escaped string
 */
std::string unescape(const std::string string);

#endif /* util_h */
