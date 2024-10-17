#include "base.h"

class Derived : public Base {
    public:
        int y;

        Derived() {
            y = 0;
        }
        int Method3() {
            return y;
        }
        int Method4() {
            return y+1;
        }
};