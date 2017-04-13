//S.hh
#ifndef S_HH_GUARD
#define S_HH_GUARD
struct S{};
#endif

//T.hh
#ifndef T_HH_GUARD
#define T_HH_GUARD
#include "S.hh"
struct T {S s;};
#endif

//T.cc
#include "S.hh" //ci sono due definizioni di S viola ODR
#include "T.hh"
T t;
S s;


