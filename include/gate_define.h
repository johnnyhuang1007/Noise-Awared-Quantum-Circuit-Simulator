#ifndef GATE_DEFINE_H
#define GATE_DEFINE_H

#define H {1.0/sqrt(2.0),0.0}, {1.0/sqrt(2.0),0}, {1.0/sqrt(2.0),0}, {-1.0/sqrt(2.0),0}
#define X {0,0}, {1,0}, {1,0},{0,0}
#define Y {0,0}, {0,-1}, {0,1},{0,0}
#define Z {1,0}, {0,0}, {0,0},{-1,0}
#define I {1,0}, {0,0}, {0,0},{0,1}
#define T {1,0}, {0,0}, {0,0},{0.70710678118,0.70710678118}
#define SX {0.5,0.5}, {0.5,-0.5}, {0.5,-0.5},{0.5,0.5}
#define R(a) {1,0}, {0,0}, {0,0}, ei(a)

#endif