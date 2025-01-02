#include "q_state.h"

using namespace std;


int main(int argc, char* argv[]) 
{
    int qb_num = stoi(argv[1]);
    Q_state q(qb_num);
    q.set_precision(stoi(argv[2]));


    
    /*  //simple test
    q.apply_UGate(0, H);
    q.apply_Control_UGate(1,0, X);
    q.apply_Control_UGate(0,1, X);
    */


    
    //Quantum Fourier Transform
    
    for(int i = 0 ; i < qb_num ; i++)
    {
        if(rand()%2)
            q.apply_UGate(i, X);
    }
    for(int i = 0 ; i < qb_num ; i++)
    {
        q.apply_UGate(i, H);
        for(int j = i+1 ; j < qb_num ; j++)
        {
            q.swap(i+1,j);
            q.apply_Control_UGate(i,i+1, R(M_PI/pow(2.0,j-i)));
            q.swap(i+1,j);
        }
    }
    


    
    
    cout<<"OUTPUT NODE INFO"<<endl;
    q.print();  
    if(qb_num < 10)
    {
        ofstream out("output.txt");
        q.print_matrix(out); //the function is pretty slow at this moment since it is not dynamic programming based
    }

    
    return 0;
}