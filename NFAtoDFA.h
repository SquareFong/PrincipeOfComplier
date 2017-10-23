#ifndef PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_NFATODFA_H
#define PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_NFATODFA_H

#include <queue>
#include <map>
#include <iostream>
#include "RegexToNFA.h"
//假装\a是epsilon的样子
const char varEPSILON = 7;

class NFAtoDFA: public RegexToNFA {
protected:
    //define set<node*> is a state, 这里存储所有的状态
    set<set<node*>> states;
    //给状态编号
    map<set<node*>,int> stateNumber;
    map<int,set<node*>> numberState;
    //状态转换表
    vector<vector<int>> mat;
public:
    vector<int> allEndStates;

    NFAtoDFA(string t):RegexToNFA(t){

    }

    void startBuilding(){
        set<node*> currentState;
        currentState.insert(&root);
        currentState = epsilonClosure(currentState);

        int counter=0;
        //等待被处理的队列
        queue<int> waitingStates;
        waitingStates.push(counter);



        //加入0号状态
        stateNumber.insert(make_pair(currentState, (int)stateNumber.size()));
        numberState.insert(make_pair((int)numberState.size(), currentState));
        states.insert(currentState);

        while (!waitingStates.empty()) {
            vector<int> tmp(128, -1);
            int currentNumber=waitingStates.front();
            currentState = (*numberState.find(currentNumber)).second;
            for(char i = 0; i<127; ++i){
                set<node*> tempState = epsilonClosure(move_(currentState,i));
                auto it = states.find(tempState);
                if(it == states.end()) {
                    if(tempState.size()) {
                        waitingStates.push((int) stateNumber.size());
                        stateNumber.insert(make_pair(tempState, (int) stateNumber.size()));
                        numberState.insert(make_pair((int) numberState.size(), tempState));
                        states.insert(tempState);
                        tmp[i] = waitingStates.back();
                    } else {
                        continue;
                    }
                } else {
                    tmp[i] = stateNumber[tempState];
                }
            }
            mat.push_back(tmp);
            waitingStates.pop();
        }

        auto psn = stateNumber.begin();
        while (psn != stateNumber.end()){
            if((*psn).first.find(&endPoint) != (*psn).first.end())
                allEndStates.push_back((*psn).second);
            ++psn;
        }

    }

    //相当于书上move函数
    set<node*> move_(const set<node*> &state, const char next){
        set<node*> result;
        auto it = state.begin();
        while (it != state.end()) {
            node *current=*it;
            //将每一个节点中，能通过next到达的节点添加到result
            for(int i = 0; i < current->edgeToNext.size(); ++i){

                if(current->edgeToNext[i].find(next) != current->edgeToNext[i].end()) {
                    result.insert(current->pointerToNext[i]);
                }
            }
            ++it;
        }
        return result;
    }
    //相当于书上epsilon-closure运算
    set<node*> epsilonClosure(const set<node*> &state){
        set<node*> result;
        queue<node*> epsilons;
        auto it = state.begin();

        //先将每一个状态添加到queue中方便遍历
        while (it != state.end()) {
            node *current=*it;
            epsilons.push(current);
            ++it;
        }

        //所有通过epsilon能到的节点并遍历之
        while (!epsilons.empty()) {
            node *current= epsilons.front();
            result.insert(current);
            for(int i = 0; i < current->edgeToNext.size(); ++i){
                if(current->edgeToNext[i].size() == 0)
                    if(result.find(current->pointerToNext[i]) == result.end())
                        epsilons.push(current->pointerToNext[i]);
            }
            epsilons.pop();
        }
        return result;
    }

    vector<vector<int>> getDFA( ){
        return mat;
    }

    void display(){
        //for(char ch=42; ch <= 57; ++ch)
            cout << ' ' <<'\t' << '+' << '\t'<< '-' << '\t'<< 'x' << '\t'<< '/' << '\t';
        for(int i(0); i<=9; ++i)
            cout << i << '\t';
        cout << endl;
        for(int i = 0; i < mat.size(); ++i){
            cout << i << '\t' << mat[i]['+'] << '\t'<< mat[i]['-'] << '\t'<< mat[i]['x'] << '\t'<< mat[i]['/'] << '\t';
            for(int j='0'; j <= '9'; ++j) {
                cout << mat[i][j] << '\t';
            }
            cout << endl;
        }
    }
};


#endif //PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_NFATODFA_H
