#include "calculator.hpp"
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

bool is_valid_double(const string &s) {
    if (s.empty()) return false;
    size_t index = 0;
    if (s[index] == '+' || s[index] == '-') {
        index++;
        if (index >= s.size()) return false;
    }
    size_t decimal_pos = s.find('.', index);
    if (decimal_pos == string::npos) {
        for (size_t i = index; i < s.size(); ++i) {
            if (!isdigit(s[i])) return false;
        }
        return true;
    } else {
        if (s.find('.', decimal_pos + 1) != string::npos) return false;
        string before = s.substr(index, decimal_pos - index);
        string after = s.substr(decimal_pos + 1);
        if (before.empty() && after.empty()) return false;
        if (!before.empty()) {
            for (char c : before) {
                if (!isdigit(c)) return false;
            }
        } else {
            return false;
        }
        if (after.empty()) return false;
        for (char c : after) {
            if (!isdigit(c)) return false;
        }
        return true;
    }
}

double parse_number(const string &expression) {
    return stod(expression);
}

struct Number {
    bool is_negative;
    string integer;
    string fractional;
};

Number parse_components(const string &s) {
    Number num;
    size_t start = 0;
    if (s[start] == '+' || s[start] == '-') {
        num.is_negative = (s[start] == '-');
        start++;
    } else {
        num.is_negative = false;
    }
    size_t decimal_pos = s.find('.', start);
    if (decimal_pos == string::npos) {
        num.integer = s.substr(start);
        num.fractional = "0";
    } else {
        num.integer = s.substr(start, decimal_pos - start);
        num.fractional = s.substr(decimal_pos + 1);
    }
    if (num.integer.empty()) num.integer = "0";
    size_t first_non_zero = num.integer.find_first_not_of('0');
    if (first_non_zero != string::npos) {
        num.integer = num.integer.substr(first_non_zero);
    } else {
        num.integer = "0";
    }
    size_t last_non_zero = num.fractional.find_last_not_of('0');
    if (last_non_zero != string::npos) {
        num.fractional = num.fractional.substr(0, last_non_zero + 1);
    } else {
        num.fractional = "0";
    }
    return num;
}

int compare_positive_numbers(const string &a_int, const string &a_frac, const string &b_int, const string &b_frac) {
    if (a_int.length() > b_int.length()) return 1;
    if (a_int.length() < b_int.length()) return -1;
    if (a_int != b_int) return a_int > b_int ? 1 : -1;
    size_t max_frac = max(a_frac.length(), b_frac.length());
    string a_padded = a_frac + string(max_frac - a_frac.length(), '0');
    string b_padded = b_frac + string(max_frac - b_frac.length(), '0');
    if (a_padded > b_padded) return 1;
    if (a_padded < b_padded) return -1;
    return 0;
}

string add_integer_strings(const string &a, const string &b) {
    string result;
    int i = a.length() - 1, j = b.length() - 1, carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    reverse(result.begin(), result.end());
    size_t first_non_zero = result.find_first_not_of('0');
    return (first_non_zero != string::npos) ? result.substr(first_non_zero) : "0";
}

string subtract_integer_strings(const string &a, const string &b) {
    string result;
    int i = a.length() - 1, j = b.length() - 1, borrow = 0;
    while (i >= 0 || j >= 0) {
        int a_digit = (i >= 0) ? (a[i--] - '0') : 0;
        int b_digit = (j >= 0) ? (b[j--] - '0') : 0;
        int sub = a_digit - b_digit - borrow;
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(sub + '0');
    }
    reverse(result.begin(), result.end());
    size_t first_non_zero = result.find_first_not_of('0');
    return (first_non_zero != string::npos) ? result.substr(first_non_zero) : "0";
}

string add_positive_abs(const Number &a, const Number &b) {
    size_t max_frac = max(a.fractional.length(), b.fractional.length());
    string a_frac = a.fractional + string(max_frac - a.fractional.length(), '0');
    string b_frac = b.fractional + string(max_frac - b.fractional.length(), '0');
    string a_num = a.integer + a_frac;
    string b_num = b.integer + b_frac;
    string sum = add_integer_strings(a_num, b_num);
    size_t decimal_pos = sum.length() - max_frac;
    string integer_part = (decimal_pos > 0) ? sum.substr(0, decimal_pos) : "0";
    string fractional_part = (decimal_pos < sum.length()) ? sum.substr(decimal_pos) : "0";
    size_t last_non_zero = fractional_part.find_last_not_of('0');
    fractional_part = (last_non_zero != string::npos) ? fractional_part.substr(0, last_non_zero + 1) : "0";
    return (fractional_part == "0") ? integer_part : integer_part + "." + fractional_part;
}

string subtract_positive_abs(const Number &a, const Number &b) {
    size_t max_frac = max(a.fractional.length(), b.fractional.length());
    string a_frac = a.fractional + string(max_frac - a.fractional.length(), '0');
    string b_frac = b.fractional + string(max_frac - b.fractional.length(), '0');
    string a_num = a.integer + a_frac;
    string b_num = b.integer + b_frac;
    string diff = subtract_integer_strings(a_num, b_num);
    size_t decimal_pos = diff.length() - max_frac;
    if (decimal_pos > diff.length()) {
        diff = string(decimal_pos - diff.length(), '0') + diff;
        decimal_pos = 0;
    }
    string integer_part = (decimal_pos > 0) ? diff.substr(0, decimal_pos) : "0";
    string fractional_part = (decimal_pos < diff.length()) ? diff.substr(decimal_pos) : "0";
    size_t last_non_zero = fractional_part.find_last_not_of('0');
    fractional_part = (last_non_zero != string::npos) ? fractional_part.substr(0, last_non_zero + 1) : "0";
    return (fractional_part == "0") ? integer_part : integer_part + "." + fractional_part;
}

string add_string_numbers(const string &a, const string &b) {
    Number num_a = parse_components(a);
    Number num_b = parse_components(b);
    if (num_a.is_negative == num_b.is_negative) {
        string sum = add_positive_abs(num_a, num_b);
        return (num_a.is_negative && sum != "0") ? "-" + sum : sum;
    } else {
        int cmp = compare_positive_numbers(num_a.integer, num_a.fractional, num_b.integer, num_b.fractional);
        if (cmp == 0) return "0";
        if (cmp > 0) {
            string diff = subtract_positive_abs(num_a, num_b);
            return (num_a.is_negative) ? "-" + diff : diff;
        } else {
            string diff = subtract_positive_abs(num_b, num_a);
            return (num_b.is_negative) ? "-" + diff : diff;
        }
    }
}