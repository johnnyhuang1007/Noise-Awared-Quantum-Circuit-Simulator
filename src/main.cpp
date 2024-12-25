#include "q_state.h"

using namespace std;

int main() {
    Q_state q(2);
    q.print();
    cout<<"AFTER APPLYING U GATE\n";
    q.apply_UGate(1, {0,0}, {1.0,0}, {1.0,0}, {0,0});
    q.print();
    cout<<"REVERSE\n";
    q.apply_Control_UGate(0, 1, {0,0}, {1.0,0}, {1.0,0}, {0,0});
    
    q.print();

    
    return 0;
}