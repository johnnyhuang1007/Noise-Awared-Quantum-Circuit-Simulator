#include "q_state.h"

void Q_state::print_matrix()
{
    vector<vector<cplx>> mat;
    mat.resize(1<<qb_num, vector<cplx>(1<<qb_num, {0,0}));
    for(auto cur : idx2node[qb_num-1])
    {
        int idx = cur->idx;
        for(int i = 0 ; i < 4 ; i++)
        {
            if(cur->next[i] == const_1)
            {
                mat[idx][idx] = cur->weight[i];
            }
            else
            {
                mat[idx][cur->next[i]->idx] = cur->weight[i];
            }
        }
    }

}