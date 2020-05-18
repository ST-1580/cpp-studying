#include "big_integer.h"

#include <cstring>
#include <stdexcept>

big_integer::big_integer() : value(1), sign(false) {

}

big_integer::big_integer(big_integer const& other){

}

big_integer::big_integer(int a) : value(1), sign(a < 0){
    value[0] = (a > 0) ? a : -a;
}

big_integer::big_integer(std::string const& str) : value(1), sign(false) {
    if (str.empty() || str == "0" || str == "-0") {
        return;
    }
    size_t curr_pos = (str[0] == '-' ? 1 : 0);
    while (curr_pos < str.size()) {
        *this *= 10;
        *this += (uint32_t) (str[curr_pos] - '0');
        curr_pos++;
    }
    sign = (str[0] == '-');
}

big_integer::~big_integer() {

}

big_integer& big_integer::operator=(big_integer const& other) {
    big_integer to_swap (other);
    std::swap(sign, to_swap.sign);
    std::swap(value, to_swap.value);
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    *this = *this + rhs;
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    *this = *this - rhs;
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    *this = *this * rhs;
    return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    *this = *this / rhs;
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    *this = *this % rhs;
    return *this;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
    *this = *this & rhs;
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
    *this = *this | rhs;
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
    *this = *this ^ rhs;
    return *this;
}

big_integer& big_integer::operator<<=(int rhs) {
    *this = *this << rhs;
    return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
    *this = *this >> rhs;
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    if (value.size() == 1 && value[0] == 0) {
        return *this;
    }
    big_integer to_swap (*this);
    to_swap.sign = !sign;
    return to_swap;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer& big_integer::operator++() {
    *this = *this + 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer old_value = *this;
    *this = *this + 1;
    return old_value;
}

big_integer& big_integer::operator--() {
    *this = *this - 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer old_value = *this;
    *this = *this - 1;
    return old_value;
}

big_integer operator+(big_integer a, big_integer const& b) {

}

big_integer operator-(big_integer a, big_integer const& b) {

}

big_integer operator*(big_integer a, big_integer const& b) {

}

big_integer operator/(big_integer a, big_integer const& b) {

}

big_integer operator%(big_integer a, big_integer const& b) {

}

big_integer operator&(big_integer a, big_integer const& b) {

}

big_integer operator|(big_integer a, big_integer const& b) {

}

big_integer operator^(big_integer a, big_integer const& b) {

}

big_integer operator<<(big_integer a, int b) {

}

big_integer operator>>(big_integer a, int b) {

}

bool operator==(big_integer const& a, big_integer const& b) {

}

bool operator!=(big_integer const& a, big_integer const& b) {

}

bool operator<(big_integer const& a, big_integer const& b) {

}

bool operator>(big_integer const& a, big_integer const& b) {

}

bool operator<=(big_integer const& a, big_integer const& b) {

}

bool operator>=(big_integer const& a, big_integer const& b) {

}

std::string to_string(big_integer const& a) {

}

std::ostream& operator<<(std::ostream& s, big_integer const& a) {

}
