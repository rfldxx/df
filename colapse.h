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
    #define ADD(name) { auto gt = colapse(name); result.insert(result.end(), gt.begin(), gt.end()); }

    auto p = f.end() - 2;
    auto a = get_arg(p); p--;


    if( f.back().val == 'D' ) {
        ADD(a);
        result.push_back({1, 'D'});
        return result;
    }

    auto b = get_arg(p);

    if( f.back().val == '+' ) { // без изменений
        ADD(a);
        ADD(b);
        result.push_back(token{1, f.back().val});
        return result;
    }

    a = colapse(a);
    bool ap = strchr("+", a.back().val);
    bool bp = 0; // strchr("+*", b.back().val);

    // if( !ap ) {
    //     b = colapse(b);
    //     bp = strchr("+", b.back().val);
    // }

    if( !(ap || bp) ) { // без изменений
        // if(!end && f.back().val == '*') 
        result.insert(result.end(), a.begin(), a.end());
        // else 
        // ADD(a);

        if(!end && f.back().val == '*') 
        result.insert(result.end(), b.begin(), b.end());
        else 
        ADD(b);
        
        result.push_back(token{1, f.back().val});
        return !end && f.back().val == '*' ? colapse(result, 1) : result;
    }

    // раскрытие
    p =  a.end()-2;
    auto a1 = get_arg(p); p--;
    auto a2 = get_arg(p);

    // p =  b.end()-2;
    // auto b1 = get_arg(p); p--;
    // auto b2 = get_arg(p);

    

    // a1 b *  a2 b *  +
    #define CRINGE_PART(name1, name2) {                         \
        auto temp = name1;                                      \
        temp.insert(temp.end(), name2.begin(), name2.end());    \
        temp.push_back({1, '*'});                               \
        ADD(temp);                                              \
    }

    CRINGE_PART(a1, b)
    CRINGE_PART(a2, b)
    result.push_back({1, '+'});

    #undef ADD
    
    return end ? result : colapse(result, 1);
}
#undef return

