#include "q_state.h"

using namespace std;

int main() {
    Q_state q(3);
    q.print();
    cout<<"AFTER APPLYING U GATE\n";
    q.apply_UGate(2, {-1,0}, {1,0}, {1,0}, {-1,0});
    q.apply_UGate(1, {1,0}, {1,0}, {1,0}, {-1,0});
    q.apply_UGate(0, {-1,0}, {1,0}, {1,0}, {-1,0});
    q.print();
    return 0;
}