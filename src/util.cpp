#include "q_state.h"
#include <queue>

void Q_state::print_node(node* cur)
{
    cout << "idx : " << cur->idx << " layer : " << cur->layer << " pointed : " << cur->pointed << endl;
    set<node*> q_list;
    q_list.insert(cur);

    while(!q_list.empty())
    {
        node* cur = *q_list.begin();
        q_list.erase(q_list.begin());
        cout << "idx : " << cur->idx << " layer : " << cur->layer << " pointed : " << cur->pointed << endl;
        for(int i = 0 ; i < 4 ; i++)
        {
            if(cur->next[i] != nullptr)
            {
                q_list.insert(cur->next[i]);
            }
        }
    }

}

cplx ei(double deg)
{
    return {cos(deg), sin(deg)};
}

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