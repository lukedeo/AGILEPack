#ifndef AGILE__DEBUG__HH
#define AGILE__DEBUG__HH

namespace agile {
    namespace debug {
        template<class... T> void unused(T&&...) {}
    }
}

#endif
