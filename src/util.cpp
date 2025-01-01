#include "q_state.h"
#include <queue>
#include <iomanip>

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

void Q_state::print_matrix(ostream& out)
{
    vector<vector<cplx>> mat = obtain_matrix(root);
    out << fixed << setprecision(3); 

  
    string horizontal_line(mat[0].size() * 20 + mat[0].size(), '-');

    for (int i = 0; i < mat.size(); i++)
    {
        out << horizontal_line << endl; 

        for (int j = 0; j < mat[i].size(); j++)
        {
            double real = mat[i][j].real();
            double imag = mat[i][j].imag();

            if (abs(real) < 1e-6) real = 0;
            if (abs(imag) < 1e-6) imag = 0;

            out << "| " << setw(8) << real;
            if (imag >= 0)
                out << " + " << setw(6) << imag << "i ";
            else
                out << " - " << setw(6) << abs(imag) << "i ";
        }
        out << "|" << endl; 
    }
    out << horizontal_line << endl; 
}

vector<vector<cplx>> Q_state::obtain_matrix(node* cur)
{
    if(cur->layer == -1)
    {
        return {{0}};
    }
    if(cur->layer == -2)
    {
        return {{1}};
    }

    int size = 1<<(qb_num - cur->layer);

    vector<vector<cplx>> mat;
    mat.resize(size, vector<cplx>(size, {0,0}));

    vector<bool> obtained;
    vector<vector<cplx>> sub_mat[4];
    obtained.resize(4,0);
    for(int i = 0 ; i < 2 ; i++)
    {
        for(int j = 0 ; j < 2 ; j++)
        {
            sub_mat[(2*i)+j] = obtain_matrix(cur->next[(2*i)+j]);   //need optimization
            for(int k = 0 ; k < size/2 ; k++)
            {
                for(int l = 0 ; l < size/2 ; l++)
                {
                    mat[(size/2)*i+k][(size/2)*j+l] = cur->weight[(2*i)+j]*sub_mat[(2*i)+j][k][l];
                }
            }
        }
    }


    return mat;
}
