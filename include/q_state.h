#ifndef _Q_STATE_H_
#define _Q_STATE_H_

#include "gate_define.h"
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
    int layer;
    int pointed = 0;
    node* next[4];
    cplx weight[4];
    bool abs_zero()
    {
        if(layer == -1)
            return true;
        if(layer == -2)
            return false;
        int cnt = 0;
        for(int i=0;i<4;i++)
        {
            if(next[i]->layer == -1)
                cnt++;
        }
        return cnt == 4;
    }
    node(int layer):layer(layer)
    {
        for(int i=0;i<4;i++)
        {
            next[i]=nullptr;
            weight[i]={1,0};
        }
        this->layer = layer;
        this->idx = -1;
    }
    void operator=(node a)
    {
        pointed = 0;    
        layer = a.layer;
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
        node* mult_rev( node,  node);
        void clean_up_redundant_zeros();
        void node_apply_UGate(node* cur, cplx u00, cplx u01, cplx u10 , cplx u11);
    public:
        Q_state(){}
        Q_state(int);
        void apply_UGate(int, cplx, cplx, cplx, cplx);
        void apply_Control_UGate(int, int, cplx, cplx, cplx, cplx);
        void reduce();
        void print();
        void print_node(node*);
        void print_matrix();
        void swap(int,int);
        
};

cplx ei(double deg);

#endif