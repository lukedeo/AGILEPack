#include "architecture.hh"

// references
//----------------------------------------------------------------------------
template <class L, class R>
inline architecture operator+ (const L &J, const R &K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}

template <class T>
inline architecture operator+ (const T &J, const T &K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}

// pointers and references
//----------------------------------------------------------------------------
template <class L, class R>
inline architecture operator+ (const L &J, R* K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}


template <class L, class R>
inline architecture operator+ (L* J, const R &K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}

template <class T>
inline architecture operator+ (const T &J, T* K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}

template <class T>
inline architecture operator+ (T* J, const T &K)
{
    architecture _arch;
    _arch.add_layer(J);
    _arch.add_layer(K);
    return _arch;
}

template <class T>
inline architecture operator+ (architecture arch, const T &L)
{
    arch += L;
    return arch;
}

template <class T>
inline architecture operator+ (const T &L, architecture arch)
{
    auto _tmp(arch);
    arch.clear();
    arch.add_layer(L);
    arch += _tmp;
    return arch;
}