#include "big_integer.h"

typedef unsigned long long int uint128_t __attribute__ ((mode (TI)));

big_integer::big_integer() : value(1), sign(false) {

}

big_integer::big_integer(big_integer const& other) {
    sign = other.sign;
    value = other.value;
}

big_integer::big_integer(int a) : value(1), sign(a < 0){
    if (a == std::numeric_limits<int>::min()) {
        value[0] = 2147483648;
    } else {
        value[0] = (a > 0) ? a : -a;
    }
}

big_integer::big_integer(uint32_t a) : value(1), sign(false){
    value[0] = a;
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
    value.clear();
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
    if (a.sign != b.sign) {
        return !a.sign ? a - (-b) : b - (-a);
    }
    size_t max_sz = std::max(a.value.size(), b.value.size());
    uint64_t sum = 0;
    uint64_t shift = 0; // 0 or 1
    big_integer res;
    for (size_t i = 0; i <= max_sz; i++) {
        res.value.push_back(0);
    }
    for (size_t i = 0; i < max_sz; i++) {
        sum = shift;
        if (i < a.value.size()) {
            sum += a.value[i];
        }
        if (i < b.value.size()) {
            sum += b.value[i];
        }
        shift = sum > UINT32_MAX;
        res.value[i] = (uint32_t) (sum & UINT32_MAX);
    }
    res.value[max_sz] = shift;
    res.sign = a.sign;
    res.delete_zero();
    return res;
}

big_integer operator-(big_integer a, big_integer const& b) {
    if (a.sign != b.sign) {
        return a.sign ? -(-a + b) : a + -b;
    }
    if (a.sign) {
        return (-b) - (-a);
    }
    if (a < b) {
        return -(b - a);
    }
    uint32_t shift = 0; // 0 or 1
    big_integer res;
    for (size_t i = 0; i < a.value.size(); i++) {
        res.value.push_back(0);
    }
    for (size_t i = 0; i < a.value.size(); i++) {
        if (i >= b.value.size()) {
            if (a.value[i] >= shift) {
                res.value[i] = a.value[i] - shift;
                shift = 0;
            } else {
                res.value[i] = UINT32_MAX;
                shift = 1;
            }
        } else if (a.value[i] >= b.value[i] + shift) {
            res.value[i] = a.value[i] - shift - b.value[i];
            shift = 0;
        } else {
            res.value[i] = UINT32_MAX - (b.value[i] - a.value[i]) - shift + 1;
            shift = 1;
        }
    }
    res.delete_zero();
    return res;
}

big_integer operator*(big_integer a, big_integer const& b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    big_integer res;
    for (size_t i = 0; i < a.value.size() + b.value.size(); i++) {
        res.value.push_back(0);
    }
    for (size_t i = 0; i < a.value.size(); i++) {
        uint64_t shift = 0; // every uint32_t number
        for (size_t j = 0; j < b.value.size(); j++) {
            uint64_t mul = (uint64_t) a.value[i] * b.value[j] + res.value[i + j] + shift;
            res.value[i + j] = (uint32_t) mul & UINT32_MAX;
            shift = (uint32_t) (mul >> 32);
        }
        res.value[i + b.value.size()] = shift;
    }
    res.sign = a.sign ^ b.sign;
    res.delete_zero();
    return res;
}

big_integer operator/(big_integer a, big_integer const& b) {
    if (b == 1) {
        return a;
    }
    if (b == -1) {
        return -a;
    }
    big_integer first = a;
    big_integer second = b;
    big_integer res;
    first.sign = second.sign = false;
    if (first < second) {
        return 0;
    }
    if (second.value.size() == 1) {
        uint32_t b_value = b.value[0];
        uint64_t rest = 0;
        uint64_t curr = 0;
        for (size_t i = 0; i < first.value.size(); i++) {
            curr = (rest << 32) | a.value[a.value.size() - 1 - i];
            res.value.push_back((uint32_t) (curr / b_value));
            rest = curr % b_value;
        }
        std::reverse(res.value.begin(), res.value.end());
        res.delete_zero();
        res.sign = a.sign ^ b.sign;
        return res;
    }
    // Algo from article: Multiple-Length Division Revisited: A Tour of the Minefield
    big_integer dq;
    first.value.push_back(0);
    size_t m = second.value.size() + 1;
    size_t n = first.value.size();
    res.value.resize(n - m + 1);
    uint32_t qt = 0;
    for (size_t i = m, j = res.value.size() - 1; i <= n; ++i, --j) {
        qt = trial(first, second);
        dq = second * qt;
        if (!smaller(first, dq, m)) {
            qt--;
            dq = second * qt;
        }
        res.value[j] = qt;
        difference(first, dq, m);
        if (!first.value.back()) {
            first.value.pop_back();
        }
    }
    res.delete_zero();
    res.sign = a.sign ^ b.sign;
    return res;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a - (a / b) * b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    big_integer first_num = a;
    big_integer second_num = b;
    big_integer res;
    size_t max_sz = std::max(a.value.size(), b.value.size());
    first_num.inverse(max_sz);
    second_num.inverse(max_sz);
    res.inverse(max_sz); // just push_back(0)
    for (size_t i = 0; i < max_sz; i++) {
        res.value[i] = first_num.value[i] & second_num.value[i];
    }
    if (a.sign & b.sign) {
        res = -res;
        res.inverse(max_sz);
        res = -res;
    }
    return res;
}

big_integer operator|(big_integer a, big_integer const& b) {
    big_integer first_num = a;
    big_integer second_num = b;
    big_integer res;
    size_t max_sz = std::max(a.value.size(), b.value.size());
    first_num.inverse(max_sz);
    second_num.inverse(max_sz);
    res.inverse(max_sz); // just push_back(0)
    for (size_t i = 0; i < max_sz; i++) {
        res.value[i] = first_num.value[i] | second_num.value[i];
    }
    if (a.sign | b.sign) {
        res = -res;
        res.inverse(max_sz);
        res = -res;
    }
    return res;
}

big_integer operator^(big_integer a, big_integer const& b) {
    big_integer first_num = a;
    big_integer second_num = b;
    big_integer res;
    size_t max_sz = std::max(a.value.size(), b.value.size());
    first_num.inverse(max_sz);
    second_num.inverse(max_sz);
    res.inverse(max_sz); // just push_back(0)
    for (size_t i = 0; i < max_sz; i++) {
        res.value[i] = first_num.value[i] ^ second_num.value[i];
    }
    if (a.sign ^ b.sign) {
        res = -res;
        res.inverse(max_sz);
        res = -res;
    }
    return res;
}

big_integer operator<<(big_integer a, int b) {
    uint32_t shift = 1 << b % 32;
    a *= shift;
    for (int i = 0; i < b / 32; i++) {
        a.value.push_back(0);
    }
    return a;
}

big_integer operator>>(big_integer a, int b) {
    for (int i = 0; i < b / 32; i++) {
        a.value.pop_back();
    }
    a -= a.sign ? (uint32_t) (1 << b % 32) - 1 : 0;
    uint32_t shift = 1 << b % 32;
    a /= shift;
    return a;
}

bool operator==(big_integer const& a, big_integer const& b) {
    if (a.value.size() != b.value.size()) {
        return false;
    }
    for (size_t i = 0; i < a.value.size(); i++) {
        if (a.value[i] != b.value[i]) {
            return false;
        }
    }
    return a.sign == b.sign;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a.sign != b.sign) {
        return a.sign;
    }
    if (a.sign) {
        return -a > -b;
    }
    if (a.value.size() != b.value.size()) {
        return a.value.size() < b.value.size();
    }
    size_t curr_pos = a.value.size();
    while (curr_pos > 0) {
        if (a.value[curr_pos - 1] != b.value[curr_pos - 1]) {
            return a.value[curr_pos - 1] < b.value[curr_pos - 1];
        }
        curr_pos--;
    }
    return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
    return !(a < b) && a != b;
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

std::string to_string(big_integer const& a) {
    if (a == 0) {
        return "0";
    }
    std::string str;
    big_integer curr = a;
    while (curr != 0) {
        str += char ('0' + (curr % 10).value[0]);
        curr /= 10;
    }
    str += a.sign ? "-" : "";
    std::reverse(str.begin(), str.end());
    return str;
}

/*std::ostream&operator<<(std::ostream& s, big_integer const& a) {

}*/

void big_integer::delete_zero() {
    while (value.size() > 1 && value.back() == 0) {
        value.pop_back();
    }
}

void big_integer::inverse(size_t sz) {
    for (size_t i = value.size(); i < sz; i++) {
        value.push_back(0);
    }
    if (sign) {
        sign = false;
        for (size_t i = 0; i < value.size(); i++) {
            value[i] = ~value[i];
        }
        *this += 1;
    }
}

uint32_t trial(big_integer &a, big_integer const &b) {
    uint128_t x = (((uint128_t) a.value[a.value.size() - 1] << 64) |
                   ((uint128_t) a.value[a.value.size() - 2] << 32) |
                   ((uint128_t) a.value[a.value.size() - 3]));
    uint128_t y = (((uint128_t) b.value[b.value.size() - 1] << 32) |
                   (uint128_t) b.value[b.value.size() - 2]);
    return std::min((uint32_t) (x / y), UINT32_MAX);
}

bool smaller(big_integer const &a, big_integer const &b, size_t index) {
    for (size_t i = 1; i <= a.value.size(); i++) {
        if (a.value[a.value.size() - i] != (index - i < b.value.size() ? b.value[index - i] : 0)) {
            return a.value[a.value.size() - i] > (index - i < b.value.size() ? b.value[index - i] : 0);
        }
    }
    return true;
}

void difference(big_integer &a, big_integer const &b, size_t index) {
    size_t start = a.value.size() - index;
    bool borrow = false;
    for (size_t i = 0; i < index; ++i) {
        uint32_t x = a.value[start + i];
        uint32_t y = i < b.value.size() ? b.value[i] : 0;
        uint64_t c = (uint64_t) x - y - borrow;
        borrow = y + borrow > x;
        c &= UINT32_MAX;
        a.value[start + i] = c;
    }
}
