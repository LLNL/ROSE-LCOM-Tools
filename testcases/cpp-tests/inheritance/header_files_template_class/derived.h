#include "base.h"

template <typename T>
class Derived : public Base {
    public:
        int y;

        int Method3() {
            return y;
        }
        int Method4() {
            return y+1;
        }
};