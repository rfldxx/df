// печатает польскую нотацию в латехную формулу $a+b$

// #include "polka.h"
#include "df.h"


// .second - приоритет операции в этом аргументе
pair<string, int> latex_helper(vector<token>::iterator p) {
    if( !p->is_operation ) return {p->str(), 9};

    
    auto a = latex_helper(p-1);

    if( p->val == 'D' ) {
        if(a.second < 3) a.first = "(" + a.first + ")";
        return {a.first + "'", 3};
    }

    auto b = latex_helper(find(p-1) - 1);

    string result = a.first;
    int curr = 1 + (p->val == '*');

    if(a.second < curr) result = "(" + result + ")";
    result += " " + p->str() + " ";
    if(b.second < curr) result += "(" + b.first + ")";
    else result += b.first;
    
    return {result, curr}; 
}

// опять рекурсия - опять неэфективно (я обленился думать)
string latex(vector<token> f) { return latex_helper(f.end()-1).first; }

int main() {
    cout << latex(df(df(init("aaa**")))) << endl;
// out: (a'' * a + a' * a' + a' * a' + a * a'') * a + (a' * a + a * a') * a' + (a' * a + a * a') * a' + a * a * a''

    cout << latex(df(df(df(init("aaa**"))))) << endl;
}