// находит аналитическую производную выражения по польской нотации

#include <iostream>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

struct token {
    bool is_operation; 
    char val;
};

vector<token> init(string s) {
    vector<token>   result;
    for(char e : s) result.push_back(token{!!strchr("+*", e), e});
    return result;
}

ostream& operator << (ostream& out, const token t) {
    return ( out << "(["[t.is_operation] << t.val << ")]"[t.is_operation] );
}

template<typename T>
ostream& operator << (ostream& out, vector<T> t) {
    if( !t.size() ) return out;
    out << t[0];
    for(int i = 1; i < t.size(); i++) out << ", " << t[i];
    return out;
}

vector<token>::iterator st;
vector<token>::iterator find(vector<token>::iterator p, int h = 1) {
    int ch = 0; // current height

    while( ch++ != h ) {
        if( p-- -> is_operation ) return find(p, h-ch+2); // страшно..
    }

    return p+1;
}

// почему-то стандартные vector::assign возвращает void, а не свой результат
// и поидеи приходится писать по миллиону раз одно и тоже (и к тому же придумывать название для переменной)
template<typename Container>
Container cringe_copy(typename Container::const_iterator first, typename Container::const_iterator last) {
    Container result;
    result.assign(first, last);
    return result;
}

// захардкожено, что тут же меняется итератор
vector<token> get_arg( vector<token>::iterator& pos ) {
    auto neww = pos; pos = find(pos);
    return cringe_copy<vector<token>>(pos, neww+1);
}


// эхх-х cначала версия с рекурсией (уж слишком на ней проще придумать короткое решение)
vector<token> df(vector<token> f) {
    if( f.size() == 1 ) {
        f.push_back(token{1, 'D'});
        return f;
    }

    // пока только две операции доступны: '+' и '*'
    auto p = f.end()-2;
    vector<token> b = get_arg(p); p--;
    vector<token> a = get_arg(p);

    // cout << "a: " << a << " | b: " << b << endl;

    vector<token> result;
    #define ADD(name) { auto gt = name; result.insert(result.end(), gt.begin(), gt.end()); }
    if( f.back().val == '+' ) { // ab+ -> aDbD+
        ADD(df(a));
        ADD(df(b));
        result.push_back(token{1, '+'});
        // cout << "@[+]\n  a: " << a << "\n  b: " << b << "\n  >  " << result << endl;
        return result;
    }

    // ab* -> adb* abd* +
    ADD(df(a))
    ADD(b)
    result.push_back(token{1, '*'});

    ADD(a)
    ADD(df(b))
    result.push_back(token{1, '*'});

    result.push_back(token{1, '+'});
    // cout << "@[*]\n  a: " << a << "\n  b: " << b << "\n  >  " << result << endl;
    return result;
}

int main() {
    auto s = init("ab+c*");
    cout << s << endl;
    auto p = s.end()-1;

    st = s.begin();

    cout << "df: " << df(s) << endl;
}
