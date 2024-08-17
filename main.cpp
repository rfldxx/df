#include "latex.h"
#include "df.h"


#ifdef TT
#include "old_colapse.h"
#else
#include "colapse.h"
#endif

int main() {
    // (a+b)*c*(d+e)
    // cout << latex(colapse(df(df(init("ab+cxd++*de+*"))))) << endl;
    
    // cout << latex(init("abyz++xdRT++++p*qw+ww+**c**")) << endl;
    cout << (latex(colapse(df((df(df(init("abyz++xdRT++++p*qw+ww+**c**"))))))).size() //find('(') == string::npos 
       ) << endl;
      
    // cout << latex(colapse(df(df(init("aaa**"))))) << endl;
}
