// находит аналитическую производную выражения по польской нотации

#include "polka.h"


// эхх-х cначала версия с рекурсией (уж слишком на ней проще придумать короткое решение)
vector<token> df(vector<token> f) {
    if( f.size() == 1 || f.back().val == 'D' ) {
        f.push_back(token{1, 'D'});
        return f;
    }

    // пока только три операции доступны: '+' и '*' и 'D'
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

    // ab* -> aDb* abD* +
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

// int main() {
//     // abc*+d+rk+*: (a+b*c+d)*(r+k)
//     // > df: (a), [D], (b), [D], (c), [*], (b), (c), [D], [*], [+], [+], 
//     //       (d), [D], [+], (r), (k), [+], [*], (a), (b), (c), [*], [+], 
//     //       (d), [+], (r), [D], (k), [D], [+], [*], [+]
//     cout << "df: " << df(init("abc*+d+rk+*")) << endl;
// }

