#include "q_state.h"

using namespace std;

#define H {1.0/sqrt(2.0),0.0}, {1.0/sqrt(2.0),0}, {1.0/sqrt(2.0),0}, {-1.0/sqrt(2.0),0}
#define X {0,0}, {1,0}, {1,0},{0,0}
#define Y {0,0}, {0,-1}, {0,1},{0,0}
#define Z {1,0}, {0,0}, {0,0},{-1,0}
#define S {1,0}, {0,0}, {0,0},{0,1}
#define T {1,0}, {0,0}, {0,0},{0.70710678118,0.70710678118}
#define SX {0.5,0.5}, {0.5,-0.5}, {0.5,-0.5},{0.5,0.5}

int main() {
    int qb_num = 10;
    Q_state q(qb_num);
    /*
    q.print();
    q.apply_UGate(0, {1.0/sqrt(2),0}, {1.0/sqrt(2),0}, {1.0/sqrt(2),0},{-1.0/sqrt(2),0});
    q.apply_Control_UGate(1,0, {0,0}, {0,0}, {0,0},{0,0});
    q.print();
    */

    for(int i = 0 ; i < qb_num ; i++)
    {
        q.apply_UGate(i, X);
    }
    for(int i = 0 ; i < qb_num ; i++)
    {
        q.apply_UGate(i, H);
        for(int j = i+1 ; j < qb_num ; j++)
        {
            q.swap(i+1,j);
            q.apply_Control_UGate(i+1,i, R(M_PI/pow(2.0,j-i)));
            q.swap(i+1,j);
        }
    }
    
    q.print();
    

    
    return 0;
}