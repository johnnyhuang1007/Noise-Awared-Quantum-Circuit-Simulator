#include "q_state.h"

using namespace std;

#define zero_node -1
#define one_node -2

bool approx_equal(cplx a, cplx b)
{
    return abs(a-b) < 1e-6;
}

Q_state::Q_state(int qb_num)
{
    idx2node.reserve(qb_num);
    this->qb_num = qb_num;
    root = new node(0);
    const_0 = new node(-1);
    const_1 = new node(-2);
    idx2node[0] = vector<node*>{root};

    for(int i = 1 ; i < qb_num ; i++)
    {
        node* cur_node = new node(i);

        idx2node[i-1][0]->next[0] = cur_node;
        for(int other = 1 ; other < 4 ; other++)
            idx2node[i-1][0]->next[other] = const_0;
        idx2node[i-1][0]->weight[0] = {1,0};
        
        idx2node[i] = vector<node*>{cur_node};
    }
    
    idx2node[qb_num-1][0]->next[0] = const_1;
    idx2node[qb_num-1][0]->weight[0] = {1,0};
    for(int other = 1 ; other < 4 ; other++)
        idx2node[qb_num-1][0]->next[other] = const_0;
}

void Q_state::clean_up_redundant_zeros()
{
    for(int i = qb_num-2 ; i >= 0 ; i--)
    {
        for(int j = 0 ; j < idx2node[i].size() ; j++)
        {
            node* cur = idx2node[i][j];
            for(int k = 0 ; k < 4 ; k++)
            {
                if(cur->next[k]->abs_zero())
                {
                    cur->next[k] = const_0;
                    cur->weight[k] = {1,0};
                }
            }
        }
        for(int j = 0 ; j < idx2node[i+1].size() ; j++)
        {
            if(idx2node[i+1][j]->abs_zero())
            {
                delete idx2node[i+1][j];
                idx2node[i+1][j] = idx2node[i+1].back();
                idx2node[i+1].pop_back();
                j--;
            }
        }
    }
}

void Q_state::reduce()
{
    
    

    for(int i = qb_num-1 ; i >= 0 ; i--)
    {
        for(int j = 0 ; j < idx2node[i].size() ; j++)
        {
            node* cur = idx2node[i][j];
            for(int k = 0 ; k < 4 ; k++)
            {
                if(approx_equal(cur->weight[k],{0,0}))
                {
                    cur->next[k] = const_0;
                    cur->weight[k] = {1,0};
                }
                if(cur->next[k] == const_0)
                {
                    cur->weight[k] = {1,0};
                }
            }
        }
    }
    for(int i = qb_num-1 ; i >= 0 ; i--)
    {
        layer_reduce(i);
    }

    for(int i = qb_num-1 ; i > 0 ; i--)
    {
        for(int j = 0 ; j < idx2node[i].size() ; j++)
        {
            node* find_node = idx2node[i][j];

            
            bool found = false;
            for(int k = 0 ; k < idx2node[i-1].size() ; k++)
            {
                node* cur = idx2node[i-1][k];
                for(int dir = 0 ; dir < 4 ; dir++)
                {
                    if(find_node == cur->next[dir])
                    {
                        found = true;
                        break;
                    }
                }
                if(found) break;
            }

            if(!found)
            {
                delete idx2node[i][j];
                idx2node[i][j] = idx2node[i].back();
                idx2node[i].pop_back();
                j--;
            }
            
        }

        for(int j = 0 ; j < idx2node[i].size() ; j++)
        {
            node* find_node = idx2node[i][j];

            
            bool found = false;
            for(int k = 0 ; k < idx2node[i-1].size() ; k++)
            {
                node* cur = idx2node[i-1][k];
                for(int dir = 0 ; dir < 4 ; dir++)
                {
                    if(find_node == cur->next[dir])
                    {
                        found = true;
                        break;
                    }
                }
                if(found) break;
            }

            if(!found)
            {
                delete idx2node[i][j];
                idx2node[i][j] = idx2node[i].back();
                idx2node[i].pop_back();
                j--;
            }
            
        }
    }
}

void Q_state::layer_reduce(int layer)
{
    vector<node*>& cur_layer = idx2node[layer];
    vector<node*>& prev_layer = idx2node[layer-1];
    
    for(int i = cur_layer.size()-2 ; i >= 0 ; i--)
    {
        node* cur = cur_layer[i];
        for(int j = 0 ; j < 4 ; j++)
        {
            if(cur->next[j]->abs_zero())
            {
                cur->next[j] = const_0;
                cur->weight[j] = {1,0};
            }
        }
    }

    for(int i = 0 ; i < cur_layer.size() ; i++)
    {
        list<node*> same_dir_nodes = {cur_layer[i]}; //the header is the one to save
        node* cur_node = cur_layer[i];
        for(int j = i+1 ; j < cur_layer.size() ; j++)
        {
            if(cur_node->next[0] == cur_layer[j]->next[0] && approx_equal(cur_node->weight[0],cur_layer[j]->weight[0])\
             && cur_node->next[1] == cur_layer[j]->next[1] && approx_equal(cur_node->weight[1],cur_layer[j]->weight[1])\
              && cur_node->next[2] == cur_layer[j]->next[2] && approx_equal(cur_node->weight[2],cur_layer[j]->weight[2])\
               && cur_node->next[3] == cur_layer[j]->next[3] && approx_equal(cur_node->weight[3],cur_layer[j]->weight[3]))
            {
                same_dir_nodes.push_back(cur_layer[j]);
                cur_layer[j] = cur_layer.back();
                cur_layer.pop_back();
                j--;
            }
        }   //at this point we find the node to be merged
        if(same_dir_nodes.size() == 1)
            continue;

        for(auto& prev_node : prev_layer)
        {
            for(int dir = 0 ; dir < 4 ; dir++)
            {
                if(find(same_dir_nodes.begin(),same_dir_nodes.end(),prev_node->next[dir]) != same_dir_nodes.end())  //found the node to merge
                {
                    prev_node->next[dir] = cur_node;
                }
            }
        }

        same_dir_nodes.pop_front();
        for(auto& node_to_del : same_dir_nodes)
        {
            delete node_to_del;
        }

    }
}

void Q_state::print()
{
    for(int i = 0 ; i < qb_num ; i++)
    {
        for(int k = 0 ; k < idx2node[i].size() ; k++)
        {
            idx2node[i][k]->idx = k;
        }
    }
    for(int i = 0 ; i < qb_num ; i++)
    {
        cout << "Layer " << i << ":\n";
        
        
        for(int k = 0 ; k < idx2node[i].size() ; k++)
        {
            cout << "Node (" << i<<", "<<k << "):\n";

            cout << "next: ";
            for(int j = 0 ; j < 4 ; j++)
            {
                if(idx2node[i][k]->next[j] == NULL)
                    cout << "NULL    ";
                else
                if(idx2node[i][k]->next[j]->layer == -1)
                    cout << "0_node ";
                else if(idx2node[i][k]->next[j]->layer == -2)
                    cout << "1_node ";
                else
                    cout << "("<< i+1<<", "<<idx2node[i][k]->next[j]->idx<<")" << "    ";
            }
            cout << "\n";
            cout << "weight: ";
            double sum = 0;
            for(int j = 0 ; j < 4 ; j++)
            {
                sum += abs(idx2node[i][k]->weight[j])*abs(idx2node[i][k]->weight[j]);
            }
            sum = sqrt(sum);
            for(int j = 0 ; j < 4 ; j++)
            {

                cout << real(idx2node[i][k]->weight[j]) <<" + "<< imag(idx2node[i][k]->weight[j]) << "i  ";
            }
            cout << "\n\n";
        }
    }

}



void Q_state::apply_Control_UGate(int target_layer, int control_layer, cplx u00, cplx u01, cplx u10, cplx u11)
{
    

    if(target_layer > control_layer)    
    {
        node gate_node(target_layer);
        gate_node.next[0] = const_1;
        gate_node.weight[0] = u00;
        gate_node.next[1] = const_1;
        gate_node.weight[1] = u01;
        gate_node.next[2] = const_1;
        gate_node.weight[2] = u10;
        gate_node.next[3] = const_1;
        gate_node.weight[3] = u11;

        for(auto& cur : idx2node[control_layer])
        {
            cur->next[2] = mult(gate_node, *cur->next[2]); //G*QBN
            reduce();
            cur->next[3] = mult(gate_node, *cur->next[3]); //G*QBN
            reduce();
            cur->next[1] = mult_rev(gate_node, *cur->next[1]); //G*QBN
            reduce();
            cur->next[3] = mult_rev(gate_node, *cur->next[3]); //G*QBN
            reduce();
        }
    }
    else
    {
        node U1(control_layer);
        U1.next[0] = const_1;
        U1.weight[0] = {1,0};
        U1.next[1] = const_1;
        U1.weight[1] = {0,0};
        U1.next[2] = const_1;
        U1.weight[2] = {0,0};
        U1.next[3] = const_1;
        U1.weight[3] = u00;

        node U2(control_layer);
        U2.next[0] = const_1;
        U2.weight[0] = {0,0};
        U2.next[1] = const_1;
        U2.weight[1] = {0,0};
        U2.next[2] = const_1;
        U2.weight[2] = {0,0};
        U2.next[3] = const_1;
        U2.weight[3] = u01;

        node U3(control_layer);
        U3.next[0] = const_1;
        U3.weight[0] = {0,0};
        U3.next[1] = const_1;
        U3.weight[1] = {0,0};
        U3.next[2] = const_1;
        U3.weight[2] = {0,0};
        U3.next[3] = const_1;
        U3.weight[3] = u10;

        node U4(control_layer);
        U4.next[0] = const_1;
        U4.weight[0] = {1,0};
        U4.next[1] = const_1;
        U4.weight[1] = {0,0};
        U4.next[2] = const_1;
        U4.weight[2] = {0,0};
        U4.next[3] = const_1;
        U4.weight[3] = u11;

        for(auto& cur : idx2node[target_layer])
        {
            node n1 = *cur->next[0];
            node n2 = *cur->next[1];
            node n3 = *cur->next[2];
            node n4 = *cur->next[3];
            node* dummy1 = mult(U1, n1);
            node* dummy2 = mult(U2, n3);
            cur->next[0] = add(*dummy1,*dummy2);
             //G*QBN
            dummy1 = mult(U1, n2);
            dummy2 = mult(U2, n4);
            cur->next[1] = add(*dummy1,*dummy2); //G*QBN

            cur->next[2] = add(*mult(U3, n1),*mult(U4, n3));
            cur->next[3] = add(*mult(U3, n2),*mult(U4, n4));
            n1 = *cur->next[0];
            n2 = *cur->next[1];
            n3 = *cur->next[2];
            n4 = *cur->next[3];
            cur->next[0] = add(*mult_rev(U1, n1),*mult_rev(U2, n2)); 
            cur->next[1] = add(*mult_rev(U3, n1),*mult_rev(U4, n2));
            cur->next[2] = add(*mult_rev(U1, n3),*mult_rev(U2, n4));
            cur->next[3] = add(*mult_rev(U3, n3),*mult_rev(U4, n4));
            
            reduce();
            clean_up_redundant_zeros();
        }
    }
}


void Q_state::swap(int a,int b)
{
    if(a == b)
        return;
    if(a > b)
    {
        int tmp = a;
        a = b;
        b = tmp;
    }

    for(int i = a ; i < b-1 ; i++)
    {
        apply_Control_UGate(i,i+1, {0,0}, {1,0}, {1,0},{0,0});
        apply_Control_UGate(i+1,i, {0,0}, {1,0}, {1,0},{0,0});
    }
    return;
}





void Q_state::apply_UGate(int qubit, cplx u00, cplx u01, cplx u10 , cplx u11)
{
    if(qubit >= qb_num)
    {
        cout << "Error: qubit index out of range\n";
        return;
    }

    node gate_node(qubit);
    gate_node.next[0] = const_1;
    gate_node.weight[0] = u00;
    gate_node.next[1] = const_1;
    gate_node.weight[1] = u01;
    gate_node.next[2] = const_1;
    gate_node.weight[2] = u10;
    gate_node.next[3] = const_1;
    gate_node.weight[3] = u11;

    vector<node*>& cur_layer = idx2node[qubit];
    for(auto cur : cur_layer)
    {
        *cur = *mult(gate_node, *cur); //G*QBN
        reduce();
    }
    for(auto cur : cur_layer)
    {

        
        *cur = *mult_rev(gate_node, *cur);//QBN*G
        reduce();
    }

}




void Q_state::expend_leaf(int layer)
{
    
    if(layer == qb_num-1)
        return;
    vector<node*>& cur_layer = idx2node[layer];
    vector<node*> next_layer = idx2node[layer+1];

    idx2node[layer+1].clear();
    idx2node[layer+1].reserve(cur_layer.size()*4);

    for(auto& cur : cur_layer)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(cur->next[i] == const_0 || cur->next[i] == const_1)
                continue;

            node* new_node = new node(layer+1);
            node* prev = cur->next[i];
            new_node->next[0] = prev->next[0];
            new_node->next[1] = prev->next[1];
            new_node->next[2] = prev->next[2];
            new_node->next[3] = prev->next[3];
            new_node->weight[0] = prev->weight[0];
            new_node->weight[1] = prev->weight[1];
            new_node->weight[2] = prev->weight[2];
            new_node->weight[3] = prev->weight[3];
            cur->next[i] = new_node;
            idx2node[layer+1].push_back(new_node);
        }
    }
    for(int i = 0 ; i < next_layer.size() ; i++)
        delete next_layer[i];
    
}
node* Q_state::mult(node gate, node cur)    //confirmed
{


    node RHS(cur.layer);
    node LHS(cur.layer);

    RHS.next[0] = cur.next[2];
    RHS.next[1] = cur.next[3];
    RHS.next[2] = cur.next[2];
    RHS.next[3] = cur.next[3];
    RHS.weight[0] = gate.weight[1]*cur.weight[2];
    RHS.weight[1] = gate.weight[1]*cur.weight[3];
    RHS.weight[2] = gate.weight[3]*cur.weight[2];
    RHS.weight[3] = gate.weight[3]*cur.weight[3];

    LHS.next[0] = cur.next[0];
    LHS.next[1] = cur.next[1];
    LHS.next[2] = cur.next[0];
    LHS.next[3] = cur.next[1];
    LHS.weight[0] = gate.weight[0]*cur.weight[0];
    LHS.weight[1] = gate.weight[0]*cur.weight[1];
    LHS.weight[2] = gate.weight[2]*cur.weight[0];
    LHS.weight[3] = gate.weight[2]*cur.weight[1];
    
    node* res = add(LHS, RHS);
    return res;
}

node* Q_state::mult_rev(node gate, node cur)
{
    node RHS(cur.layer);
    node LHS(cur.layer);

    for(int i = 0 ; i < 4 ; i++)
    {
        gate.weight[i] = conj(gate.weight[i]);
    }

    RHS.next[0] = cur.next[0];
    RHS.next[1] = cur.next[0];
    RHS.next[2] = cur.next[2];
    RHS.next[3] = cur.next[2];
    RHS.weight[0] = gate.weight[0]*cur.weight[0];
    RHS.weight[1] = gate.weight[1]*cur.weight[0];
    RHS.weight[2] = gate.weight[0]*cur.weight[2];
    RHS.weight[3] = gate.weight[1]*cur.weight[2];

    LHS.next[0] = cur.next[1];
    LHS.next[1] = cur.next[1];
    LHS.next[2] = cur.next[3];
    LHS.next[3] = cur.next[3];
    LHS.weight[0] = gate.weight[2]*cur.weight[1];
    LHS.weight[1] = gate.weight[3]*cur.weight[1];
    LHS.weight[2] = gate.weight[2]*cur.weight[3];
    LHS.weight[3] = gate.weight[3]*cur.weight[3];
    
    node* res = add(LHS, RHS);
    
    return res;
}

node* Q_state::add( node LHS,  node RHS)
{
    int layer;

    layer = max(LHS.layer, RHS.layer);
    node* res = new node(layer);
    if(layer >= 0)
        idx2node[layer].push_back(res);


    
    
    if(LHS.layer == -1 && RHS.layer == -1)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            res->next[i] = const_0;
            res->weight[i] = {1,0};
        }
        return res;
    }
    if(LHS.layer == -1)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            res->next[i] = RHS.next[i];
            res->weight[i] = RHS.weight[i];
        }
        return res;
    }
    if(RHS.layer == -1)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            res->next[i] = LHS.next[i];
            res->weight[i] = LHS.weight[i];
        }
        return res;
    }

    for(int i = 0 ; i < 4 ; i++)
    {
        if((RHS.next[i] == const_0 || (real(RHS.weight[i]) == 0 && imag(RHS.weight[i]) == 0))&&\
            (LHS.next[i] == const_0 || (real(LHS.weight[i]) == 0 && imag(LHS.weight[i]) == 0)))
        {
            res->next[i] = const_0;
            res->weight[i] = {1,0};
            continue;
        }
        if(RHS.next[i] == const_0 || (real(RHS.weight[i]) == 0 && imag(RHS.weight[i]) == 0))
        {
            res->next[i] = LHS.next[i];
            res->weight[i] = LHS.weight[i];
            continue;
        }
        else if(LHS.next[i] == const_0 || (real(LHS.weight[i]) == 0 && imag(LHS.weight[i]) == 0))
        {
            res->next[i] = RHS.next[i];
            res->weight[i] = RHS.weight[i];


            continue;
        }
        else if(LHS.next[i] == RHS.next[i])
        {
            res->next[i] = LHS.next[i];
            res->weight[i] = LHS.weight[i] + RHS.weight[i];
            continue;
        }

        for(int j = 0 ; j < 4 ; j++)
            RHS.next[i]->weight[j] = RHS.weight[i] * RHS.next[i]->weight[j] / LHS.weight[i];
        RHS.weight[i] = LHS.weight[i];
        
        res->next[i] = add(*LHS.next[i], *RHS.next[i]);
        
    }

    return res;
}