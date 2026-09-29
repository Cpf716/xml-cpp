//
//  util.cpp
//  xml
//
//  Created by Corey Ferguson on 9/29/26.
//

#include "util.h"

std::vector<std::string> copy(const std::vector<std::string_view> value)
{
    std::vector<std::string> result;

    result.reserve(value.size());

    for (std::string_view sv : value)
        result.push_back(std::string(sv));

    return result;
}

std::vector<int> digits(const double number)
{
    std::vector<int> result;

    int whole = number;

    while (whole)
    {
        result.push_back(whole % 10);

        whole /= 10;
    }

    if (result.empty())
        result.push_back(0);

    result.insert(result.begin(), INT_MAX);

    double part = number;

    while (part - (int)part)
    {
        result.insert(result.begin(), (
                                          (int)(part * 10)) %
                                          10);

        part *= 10;
    }

    // Reverse
    for (int i = 0; i < result.size() / 2; i++)
        std::swap(result[i], result[result.size() - i - 1]);

    return result;
}

std::string escape(const std::string string)
{
    size_t len = string.length() + 1;
    char *str = new char[pow((int)len + 2)];

    strcpy(str, string.c_str());

    // Insert leading double quotations
    str[len] = '\"';

    for (size_t i = len; i > 0; i--)
        std::swap(str[i], str[i - 1]);

    len++;

    // Escape double quotations
    for (size_t i = 1; i < len - 1; i++)
    {
        if (str[i] == '\"')
        {
            // resize, if required
            if (is_pow(len + 2, 2))
            {
                char *tmp = new char[pow((int)(len + 2) * 2)];

                for (size_t j = 0; j < len; j++)
                    tmp[j] = str[j];

                delete[] str;

                str = tmp;
            }

            // Insert escape character
            str[len] = '\\';

            for (size_t j = len; j > i; j--)
                std::swap(str[j], str[j - 1]);

            len++;
            i++;
        }
    }

    // Insert trailing double quotoations
    str[len] = '\"';

    std::swap(str[len], str[len - 1]);

    len++;

    std::string result = std::string(str);

    delete[] str;

    return result;
}

// 1. (\+|-)?
// 2. (\+|-)?[0-9]+(\.[0-9]+)?
// 3. (\+|-)?([0-9]+(\.[0-9]+)?|[0-9]*\.[0-9]+)((E|e)(\+|-)?[0-9]+)?
bool is_number(const std::string value)
{
    if (value.empty())
        return false;

    int i = 0;

    // Leading positive (+) or negative (-) sign
    if (value[i] == '+' || value[i] == '-')
        i++;

    // Find decimal point
    int j = i;

    while (j < value.length() && value[j] != '.')
        j++;

    // If no decimal point is found, start at the beginning (after the sign, if applicable)
    // Find exponent
    int k = j == value.length() ? i : j;

    while (k < value.length() && !(value[k] == 'E' || value[k] == 'e'))
        k++;

    // Stop at the decimal point, if applicable; otherwise stop at the exponent, if applicable
    int l = j < k ? j : k,
        m = i;

    for (; m < l; m++)
        if (!isdigit(value[m]))
            return false;

    // Count the number of digits between the beginning (after sign, if applicable) and the decimal point (if applicable)
    // and the decimal point (if applicable) and the exponent (if applicable)
    size_t n = l - i;

    //  After decimal (if applicable) and before exponent (if applicable)
    if (j != value.length())
    {
        for (m = j + 1; m < k; m++)
            if (!isdigit(value[m]))
                return false;

        n += k - j - 1;
    }

    // There are no digits between sign (if applicable) and decimal point (if applicable)
    // and/or decimal point (if applicable) and exponent (if applicable)
    if (n == 0)
        return false;

    // After exponent (if applicable)
    if (k != value.length())
    {
        size_t l = k + 1;

        if (l == value.length())
            return false;

        // Leading positive (+) or negative (-) sign
        if (value[l] == '+' || value[l] == '-')
            l++;

        if (l == value.length())
            return false;

        for (; l < value.length(); l++)
            if (!isdigit(value[l]))
                return false;
        // Single digit
    }

    return true;
}

bool is_pow(const size_t b, const size_t n)
{
    if (b == 0)
        return false;

    if (n == 0)
        return b < 2;

    int result = log(b) / log(n);

    return (int)result - result == 0;
}

std::string join(std::vector<std::string> values, std::string delimeter)
{
    std::ostringstream ss;

    if (values.size())
    {
        for (size_t i = 0; i < values.size() - 1; i++)
            ss << values[i] << delimeter;

        ss << values[values.size() - 1];
    }

    return ss.str();
}

void merge(std::vector<std::string> &values, const std::string delimiter)
{
    for (int i = 0; i < values.size() - 1; i++)
    {
        // Find opening double quotations
        size_t l = 0;

        while (l < values[i].length() && values[i][l] != '\"')
            l++;

        // Double quotations found
        if (l != values[i].length())
        {
            // find closing double quotations
            size_t j = l + 1;

            while (j < values[i].length())
            {
                if (values[i][j] == '\"')
                {
                    size_t r = j + 1;

                    while (r < values[i].length() && values[i][r] == '\"')
                        r++;

                    if ((r - j) % 2 == 0)
                        j = r;
                    else
                        break;
                }
                else
                    j++;
            }

            // None found in the same token
            if (j == values[i].length())
            {
                bool flag = true;

                // Find closing double quotations in subsequent tokens
                while (flag && i < values.size() - 1)
                {
                    size_t j = 0;

                    while (j < values[i + 1].length())
                    {
                        if (values[i + 1][j] == '\"')
                        {
                            size_t r = j + 1;

                            while (r < values[i + 1].length() && values[i + 1][r] == '\"')
                                r++;

                            if ((r - j) % 2 == 0)
                                j = r;
                            else
                            {
                                // Break nested loop
                                flag = false;
                                break;
                            }
                        }
                        else
                            j++;
                    }

                    // Merge tokens
                    values[i] += delimiter + values[i + 1];

                    values.erase(values.begin() + i + 1);
                }
            }
        }
    }
}

double parse_number(const std::string value)
{
    return is_number(value) ? stod(value) : NAN;
}

int pow(const int b, int n)
{
    return b == 0 ? 1 : pow(n, ceil(log(b) / log(n)));
}

std::vector<std::string_view> split(const std::string_view string, const std::string delimeter)
{
    int start = 0;

    std::vector<std::string_view> result;

    result.reserve(string.length() + 1);

    for (int end = 0; end <= (int)string.length() - (int)delimeter.length(); end++)
    {
        int index = 0;

        while (index < delimeter.length() && string[end + index] == delimeter[index])
            index++;

        if (index == delimeter.length())
        {
            result.push_back(string.substr(start, end - start));

            start = end + index;
        }
    }

    result.push_back(string.substr(start));

    return result;
}

std::vector<std::string> split(const std::string string, const std::string delimeter)
{
    int start = 0;

    std::vector<std::string> result;

    result.reserve(string.length() + 1);

    for (int end = 0; end <= (int)string.length() - (int)delimeter.length(); end++)
    {
        int index = 0;

        while (index < delimeter.length() && string[end + index] == delimeter[index])
            index++;

        if (index == delimeter.length())
        {
            result.push_back(string.substr(start, end - start));

            start = end + index;
        }
    }

    result.push_back(string.substr(start));

    return result;
}

void split(std::vector<std::string> &target, const std::string source, const std::string delimeter)
{
    int start = 0;

    for (int end = 0; end <= (int)source.length() - (int)delimeter.length(); end++)
    {
        int index = 0;

        while (index < delimeter.length() && source[end + index] == delimeter[index])
            index++;

        if (index == delimeter.length())
        {
            target.push_back(source.substr(start, end - start));

            start = end + index;
        }
    }

    target.push_back(source.substr(start));
}

std::vector<std::string_view> tokens(const std::string_view string)
{
    std::vector<std::string_view> result;

    result.reserve(string.length() + 1);

    for (size_t start = 0, end = 0; end < string.length(); end++)
    {
        while (end < string.length() && isspace(string[end]))
            end++;

        start = end;

        while (end < string.length() && !isspace(string[end]))
            end++;

        if (start != end)
            result.push_back(string.substr(start, end - start));
    }

    return result;
}

std::vector<std::string> tokens(const std::string string)
{
    std::vector<std::string> result;

    result.reserve(string.length() + 1);

    for (size_t start = 0, end = 0; end < string.length(); end++)
    {
        while (end < string.length() && isspace(string[end]))
            end++;

        start = end;

        while (end < string.length() && !isspace(string[end]))
            end++;

        if (start != end)
            result.push_back(string.substr(start, end - start));
    }

    return result;
}

void tokens(std::vector<std::string> &target, const std::string source)
{
    target.reserve(source.length());

    for (size_t start = 0, end = 0; end < source.length(); end++)
    {
        while (end < source.length() && isspace(source[end]))
            end++;

        start = end;

        while (end < source.length() && !isspace(source[end]))
            end++;

        if (start != end)
            target.push_back(source.substr(start, end - start));
    }
}

std::string truncate_d(const double number, const int min_prec)
{
    std::vector<int> d = digits(number);

    int i = (int)d.size() - 1;

    // Find trailing nonzero digit
    while (i >= 0 && d[i] == 0)
        i--;

    int j = i;

    // Find decimal point
    while (j >= 0 && d[j] != INT_MAX)
        j--;

    if (j == -1)
        j = i;

    std::vector<std::string> result;

    for (int k = 0; k < j; k++)
        result.push_back(std::to_string(d[k]));

    if (j != i)
    {
        result.push_back(".");

        int k;

        for (k = j + 1; k <= i; k++)
            result.push_back(std::to_string(d[k]));

        for (; k <= j + min_prec; k++)
            result.push_back(std::to_string(0));
    }
    else if (d[i] == -1)
    {
        result.push_back(".");

        for (int k = 0; k < min_prec; k++)
            result.push_back(std::to_string(0));
    }

    return join(result, "");
}

std::string unescape(const std::string string)
{
    if (string.empty())
        return string;

    // Find opening double quotations
    int l = 0;

    while (l < string.length() && string[l] != '\"')
        l++;

    // None found; return identity
    if (l == string.length())
        return string;

    // Copy string
    int len = (int)string.length() + 1;
    char *str = new char[len];

    strcpy(str, string.c_str());

    // Erase opening double quotations
    for (int i = l; i < len - 1; i++)
        std::swap(str[i], str[i + 1]);

    len--;

    // Find closing double quotations
    int r = l;

    while (r < len - 2 && (str[r] == '\\' || str[r + 1] != '\"'))
        r++;

    if (r < len - 2 && str[r + 1] == '\"')
        r++;

    if (r < len - 1)
    {
        // Erase closing double quotations
        for (int i = r; i < len - 1; i++)
            std::swap(str[i], str[i + 1]);

        len--;

        // Escape applicable double quotations
        for (int i = l; i < r - 1; i++)
        {
            if (str[i] == '\\' && str[i + 1] == '\"')
            {
                for (int j = i; j < len - 1; j++)
                    std::swap(str[j], str[j + 1]);

                len--;
                r--;
            }
        }

        // Erase extranous double quotations
        while (r < len - 2)
        {
            if (str[r] == '\\' && str[r + 1] == '\"')
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = r; k < len - 1; k++)
                        std::swap(str[k], str[k + 1]);

                    len--;
                }
            }
            else
                r++;
        }
        // None found
    }
    else
    {
        // Escape double quotations
        for (int i = l; i < len - 2; i++)
        {
            if (str[i] == '\\' && str[i + 1] == '\"')
            {
                for (int j = i; j < len - 1; j++)
                    std::swap(str[j], str[j + 1]);

                len--;
                i++;
            }
        }
    }

    std::string result = std::string(str);

    delete[] str;

    return result;
}
