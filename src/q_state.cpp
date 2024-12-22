#include "q_state.h"

using namespace std;

#define zero_node -1
#define one_node -2


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

void Q_state::reduce()
{
    for(int i = qb_num-1 ; i >= 0 ; i--)
    {
        layer_reduce(i);
    }

}

void Q_state::layer_reduce(int layer)
{
    vector<node*>& cur_layer = idx2node[layer];
    vector<node*>& prev_layer = idx2node[layer-1];
    
    for(int i = 0 ; i < cur_layer.size() ; i++)
    {
        list<node*> same_dir_nodes = {cur_layer[i]}; //the header is the one to save
        node* cur_node = cur_layer[i];
        for(int j = i+1 ; j < cur_layer.size() ; j++)
        {
            if(cur_node->next[0] == cur_layer[j]->next[0]\
             && cur_node->next[1] == cur_layer[j]->next[1]\
              && cur_node->next[2] == cur_layer[j]->next[2]\
               && cur_node->next[3] == cur_layer[j]->next[3])
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
        cout << "Layer " << i << ":\n";
        
        
        for(int k = 0 ; k < idx2node[i].size() ; k++)
        {
            cout << "Node " << idx2node[i][k] << ":\n";

            cout << "next: ";
            for(int j = 0 ; j < 4 ; j++)
            {
                if(idx2node[i][k]->next[j]->idx == -1)
                    cout << "0_node ";
                else if(idx2node[i][k]->next[j]->idx == -2)
                    cout << "1_node ";
                else
                    cout << idx2node[i][k]->next[j] << "    ";
            }
            cout << "\n";
            cout << "weight: ";
            for(int j = 0 ; j < 4 ; j++)
            {
                cout << idx2node[i][k]->weight[j] << " ";
            }
            cout << "\n\n";
        }
    }

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
        *cur = *mult(gate_node, *cur);
        reduce();
    }
    for(auto cur : cur_layer)
    {
    //    *cur = mult_rev(gate_node, *cur);
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
node* Q_state::mult(node gate, node cur)
{

    node RHS(cur.idx);
    node LHS(cur.idx);

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

node* Q_state::add( node LHS,  node RHS)
{
    node* res = new node(LHS.idx);
    idx2node[LHS.idx].push_back(res);


    for(int i = 0 ; i < 4 ; i++)
    {
        if(RHS.next[i] == const_0)
        {
            res->next[i] = LHS.next[i];
            res->weight[i] = LHS.weight[i];
            continue;
        }
        else if(LHS.next[i] == const_0)
        {
            res->next[i] = RHS.next[i];
            res->weight[i] = RHS.weight[i];
            continue;
        }
        else if(LHS.next[i] == RHS.next[i] && LHS.next[i] == const_1)
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