#ifndef VECTORTYPES_H
#define VECTORTYPES_H

#include <vector>
using namespace std;

// One dimensional vector of doubles, etc..., using std::vector template

typedef vector<char>           cvector;

typedef vector<unsigned char>  ucvector;

typedef vector<short>          sivector; // avoid conflict svector in SpWx

typedef vector<unsigned short> usvector;

typedef vector<int>            ivector;

typedef vector<unsigned int>   uivector;

// Two dimensional vector of ints

typedef vector<ivector>        vivector;

// Three dimensional vector of ints

typedef vector<vivector>       vvivector;

typedef vector<bool>           bvector;

// Two dimensional vector of unsigned ints

typedef vector<uivector>       vuivector;

typedef vector<float>          fvector;

typedef vector<double>         dvector;

// Two dimensional vector of doubles

typedef vector<dvector>        vdvector;

// Three dimensional vector of doubles

typedef vector<vdvector>       vvdvector;

#endif
