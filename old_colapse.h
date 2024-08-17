// раскрывает скобки в польской нотации

#include "polka.h"


// раскрывает по втором аргументу, при этом меняя порядок:
//   "b a1 a2 + *"  ->  "a1 b * a2 b * +"
// и потом опять вызывается, чтобы добить
vector<token> colapse(vector<token> f, bool end = 0) {
    // cout << "in:  " << f << endl;
    // cout << "     " << latex(f) << endl;

    if( !f.back().is_operation ) {
        return f;
    }
    
// #define return return ( cout << "out: " << latex(result) << endl ),

    vector<token> result;
    #define ADD(name) { auto gt = name; result.insert(result.end(), gt.begin(), gt.end()); }

    auto p = f.end() - 2;
    auto a = colapse(get_arg(p)); p--;

    // операция с одним аргументом
    if( f.back().val == 'D' ) {
        ADD(a);
        result.push_back({1, 'D'});
        return result;
    }

    auto b = colapse(get_arg(p));


    // создается массив "ссылок":  vect& aa[2] = ap ? {a1, a2} : {a, a}
    #define ARGET(name)                                                 \
        bool name##p = name.back().val == '+';                          \
        p =  name.end()-2;                                              \
        auto name##1 = name##p ? get_arg(p) : vector<token>{}; p--;     \
        auto name##2 = name##p ? get_arg(p) : vector<token>{};          \
        vector<token>* name##name[2];                                   \
        if(name##p) name##name[0] = &name##1, name##name[1] = &name##2; \
        else        name##name[0] = &name,    name##name[1] = &name;
    
    ARGET(a)
    ARGET(b)

    if( f.back().val == '+' || !(ap || bp) ) { // без изменений
        ADD(a);
        ADD(b);
        result.push_back(token{1, f.back().val});
        return result;
    }


    // раскрытие
    #define CRINGE_PART(name1, name2) {                             \
        auto temp = (name1);                                        \
        temp.insert(temp.end(), (name2).begin(), (name2).end());    \
        temp.push_back({1, '*'});                                   \
        ADD(colapse(temp));                                         \
    }

    CRINGE_PART(*aa[0], *bb[0])
    CRINGE_PART(*aa[1], *bb[1])
    result.push_back({1, '+'});

    if( ap && bp ) {
        CRINGE_PART(*aa[0], *bb[1])
        CRINGE_PART(*aa[1], *bb[0])
        result.push_back({1, '+'});
        result.push_back({1, '+'});
    }

    #undef ADD
    #undef ARGET
    #undef CRINGE_PART
    
    return result;
}
#undef return

