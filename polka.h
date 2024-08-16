#include <iostream>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

struct token {
    bool is_operation; 
    char val;

    string str() const{
        return {val};
    }
};

vector<token> init(string s) {
    vector<token>   result;
    for(char e : s) result.push_back(token{!!strchr("+*D", e), e});
    return result;
}

ostream& operator << (ostream& out, const token t) {
    return ( out  << "(["[t.is_operation] << t.str() << ")]"[t.is_operation] );
}

template<typename T>
ostream& operator << (ostream& out, vector<T> t) {
    if( !t.size() ) return out;
    out << t[0];
    for(int i = 1; i < t.size(); i++) out << ", " << t[i];
    return out;
}



// копирование вектора
// ----------------------------------------------------------------------------
// почему-то стандартные vector::assign возвращает void, а не свой результат
// и поидеи приходится писать по миллиону раз одно и тоже (и к тому же придумывать название для переменной)
template<typename Container>
Container cringe_copy(typename Container::const_iterator first, typename Container::const_iterator last) {
    Container result;
    result.assign(first, last);
    return result;
}



// находит "аргумент" к данной позиции
// ----------------------------------------------------------------------------
// пример:   
//    polka: "... a b + c d - * ..."
//                     ^   ^   ^
//                     p1  p2  p3
//    getarg(p1) ~ "a b +"
//    getarg(p2) ~ "d"
//    getarg(p3) ~ "a b + c d - *"
vector<token>::iterator find(vector<token>::iterator p, int h = 1) {
    int ch = 0; // current height

    while( ch++ != h ) {
        if( p-- -> is_operation ) return find(p, h-ch+ 1 + ((p+1)->val != 'D')); // страшно..
    }

    return p+1;
}

// захардкожено, что тут же меняется итератор
vector<token> get_arg( vector<token>::iterator& pos ) {
    auto neww = pos; pos = find(pos);
    return cringe_copy<vector<token>>(pos, neww+1);
}

