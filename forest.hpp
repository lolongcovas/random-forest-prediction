/*
    Copyright (c) 2014, Long Long Yu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Stanford University nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Philipp Krähenbühl ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Philipp Krähenbühl BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __FOREST_HPP__
#define __FOREST_HPP__

#include <istream>
#include <iostream>
#include <string>
#include <fstream>
#include <iosfwd>
#include <vector>

#include <omp.h>

namespace RandomForest{

class Tree{
public:

    Tree(){
        this->nFids = 0;
        this->fids = NULL;

        this->thrs = NULL;
        this->nThrs = 0;

        this->child = NULL;
        this->nChild = 0;

        this->distr = NULL;
        this->rDistr = 0;
        this->cDistr = 0;

        this->hs = NULL;
        this->nHs = 0;

        this->count = NULL;
        this->nCount = 0;

        this->depth = NULL;
        this->nDepth = 0;
    }

    ~Tree(){
        release(this->fids);
        release(this->thrs);
        release(this->child);
        release(this->distr);
        release(this->hs);
        release(this->count);
        release(this->depth);
    }

    void read_data_uint32(std::ifstream &is, unsigned int &n, unsigned int **data){
        is >> n;
        *data = (unsigned int*)malloc(sizeof(unsigned int) * n);
        for (int i=0; i<n; ++i)
            is >> (*data)[i];
    }

    void read_data_f(std::ifstream &is, unsigned int &n, float **data){
        is >> n;
        *data = (float*)malloc(sizeof(float) * n);
        for (int i=0; i<n; ++i)
            is >> (*data)[i];
    }


    void read_data(std::ifstream &is, unsigned int &n, unsigned int &m, float **data){
        is >> n;
        is >> m;
        *data = (float*)malloc(sizeof(float) * n * m);
        for (int j=0; j<m; ++j)// cols
            for (int i=0; i<n; ++i) // rows
                is >> (*data)[i*m+j];
    }

    void read(std::ifstream &is){

        std::string aux;
        int aux2;
        is >>aux;
        is >> aux2;

        if (this->fids)
            free(this->fids);
        read_data_uint32(is, this->nFids, &this->fids);

        if (this->thrs)
            free(this->thrs);
        read_data_f(is, this->nThrs, &this->thrs);

        if (this->child)
            free(this->child);
        read_data_uint32(is, this->nChild, &this->child);

        if (this->distr)
            free(this->distr);
        read_data(is, this->rDistr, this->cDistr, &this->distr);

        if (this->hs)
            free(this->hs);
        read_data_uint32(is, this->nHs, &this->hs);

        if (this->count)
            free(this->count);
        read_data_uint32(is, this->nCount, &this->count);

        if (this->depth)
            free(this->depth);
        read_data_uint32(is, this->nDepth, &this->depth);
    }


    unsigned int predict(const float *data, float *prob){
        unsigned int k = 0;
        while (this->child[k]){
            if (data[this->fids[k]] < this->thrs[k]){
                k = child[k]-1;
            }
            else{
                k = child[k];
            }
        }

        float p = 0.f;
        int label = 0;
        for (int i=0; i<this->cDistr; ++i){
            prob[i] = this->distr[k*this->cDistr + i];
            if (p < this->distr[k*this->cDistr + i]){
                p = this->distr[k*this->cDistr + i];
                label = i;
            }
        }
        return label;
    }


private:

    void release(void *data){
        if (data)
            free(data);
        data = NULL;
    }

    unsigned int *fids;
    unsigned int nFids;

    float *thrs;
    unsigned int nThrs;

    unsigned int *child;
    unsigned int nChild;

    float *distr;
    unsigned int rDistr, cDistr;

    unsigned int *hs;
    unsigned int nHs;

    unsigned int *count;
    unsigned int nCount;

    unsigned int *depth;
    unsigned int nDepth;
};


class Forest{
public:

    std::vector<Tree> trees;
    int nLabels;

    void read(const char *filename){
        std::ifstream is(filename);
        std::string str_trees;
        int nTrees;
        is >> str_trees;
        is >> nTrees;
        trees.resize(nTrees);

        std::string str_nLabels;
        is >> str_nLabels;
        is >> nLabels;

        for (int i=0; i<nTrees; ++i){
            trees[i].read(is);
        }
        is.close();
    }

    int predict(const float *data, float *prob){

        float *probs = (float*)malloc(sizeof(float) * trees.size() * nLabels);
        for (int j=0; j<trees.size(); j++){
            trees[j].predict(data, probs + j * nLabels);
        }

        for (int k=0; k<nLabels; k++){
            for (int j=0; j<trees.size(); j++){
                prob[k] += probs[j * nLabels + k];
            }
            prob[k] /= trees.size();
        }

        int label = 0;
        float maxProb = 0.f;
        for (int j=0; j<nLabels; j++){
            if (maxProb < prob[j]){
                maxProb = prob[j];
                label = j;
            }
        }
        free(probs);
        return label;
    }
};

}

#endif
