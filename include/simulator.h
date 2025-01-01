#include "q_state.h"


class Gate
{
    //furthur implementation
};

class U_GATE: public Gate
{
    private:
        double theta;
        double phi;
        double lambda;
    public:
        U_GATE(double theta, double phi, double lambda);
        ~U_GATE();
        void apply(Q_state &state, int qubit);
};

class CU_GATE: public Gate
{
    public:
        CU_GATE();
        ~CU_GATE();
        void apply(Q_state &state, int qubit1, int qubit2);
};

class simulator
{
    private:
        vector<>
        compile_control_gate();
        vector<pair<int,node>>  
    public:
        simulator();
        ~simulator();
        void read_qasm(std::string filename);
        void run();
        void print_results();
};