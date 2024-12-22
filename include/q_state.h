#ifndef _Q_STATE_H_
#define _Q_STATE_H_

#include<iostream>
#include<map>
#include<set>
#include<list>
#include<vector>
#include <complex>
#include<cmath>
#include<algorithm>
using namespace std;

#define idx_node_vec pair<int, vector<node*>>
#define cplx complex<double>

struct node
{
    int idx;
    node* next[4];
    cplx weight[4];
    node(int idx):idx(idx)
    {
        for(int i=0;i<4;i++)
        {
            next[i]=nullptr;
            weight[i]={1,0};
        }
        this->idx = idx;
    }
    void operator=(node a)
    {
        idx = a.idx;
        for(int i=0;i<4;i++)
        {
            next[i] = a.next[i];
            weight[i] = a.weight[i];
        }
    }

};


class Q_state
{
    private:
        node* root;
        node* const_0;
        node* const_1;
        int qb_num;
        cplx header_weight = {1,0};
        vector<vector<node*>> idx2node;   //index : matched nodes
        void layer_reduce(int);
        void expend_leaf(int);
        node* add( node,  node);
        node* mult( node,  node);
    public:
        Q_state(){}
        Q_state(int);
        void apply_UGate(int, cplx, cplx, cplx, cplx);
        void apply_XGate(int);
        void reduce();
        void print();
        
};

#endif