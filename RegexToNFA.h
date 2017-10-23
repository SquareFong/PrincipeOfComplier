//
// Created by square on 17-10-20.
//

#ifndef PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_REGEXTONFA_H
#define PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_REGEXTONFA_H

#include <vector>
#include <set>
#include <string>
using namespace std;

string regexText;

class RegexToNFA {
protected:
    class node{
    public:
        vector<set<char>> edgeToNext;
        vector<node*> pointerToNext;
        ~node(){
//            auto it = pointerToNext.begin();
//            while (it != pointerToNext.end()) {
//                if(*it != nullptr) {
//                    delete (*it);
//                    (*it) = nullptr;
//                }
//            }

        }
    };

    node root;
    //static string regexText;
    node endPoint;


public:
//    bool readRegex(string text){
//        regexText = text;
//    }

    RegexToNFA(string text=""){
        regexText=text;
    }

    bool startBuildingNFA(){
        int sub=0;
        return buildNFA(sub, &root, &endPoint);
    }

    //递归调用，碰到括号调用
    bool buildNFA(int &subscript, node *father, node *endPoint){
        static const set<char> varEpsilon;
        node *kid1 = new node();
        node *kid2 = new node();
        father->edgeToNext.push_back(varEpsilon);
        father->pointerToNext.push_back(kid1);

        while(subscript < regexText.size()) {
            //匹配例如[1-9]这类表达式
            if (regexText[subscript] == '[') {
                if (regexText[subscript + 4] == ']' && regexText[subscript + 2] == '-' &&
                    (regexText[subscript + 1] < regexText[subscript + 3])) {
                    set<char> edgeTemp;
                    for (char i = regexText[subscript + 1]; i <= regexText[subscript + 3]; ++i) {
                        edgeTemp.insert(i);
                    }
                    kid1->edgeToNext.push_back(edgeTemp);
                    kid1->pointerToNext.push_back(kid2);
                    subscript += 5;
//                    //碰到星星则回环
//                    if (regexText[subscript] == '*') {
//                        ++subscript;
//                        kid2->edgeToNext.push_back(varEpsilon);
//                        kid2->pointerToNext.push_back(kid1);
//                    }
                }
            }
                //碰到或将kid1，kid2重定向到直接接到父节点
            else if (*(regexText.begin()+subscript) == '|') {
                kid1->edgeToNext.push_back(varEpsilon);
                kid1->pointerToNext.push_back(kid2);
                kid2->edgeToNext.push_back(varEpsilon);
                kid2->pointerToNext.push_back(endPoint);
                kid1 = new node();
                kid2 = new node();
                father->edgeToNext.push_back(varEpsilon);
                father->pointerToNext.push_back(kid1);
//                kid1 = father;
//                kid2 = new node();
                ++subscript;
                continue;
            }
                //碰到左括号
            else if (regexText[subscript] == '(') {
                ++subscript;
                buildNFA(subscript, kid1, kid2);
            }
                //碰到反括号结束
            else if (regexText[subscript] == ')') {

                kid1->edgeToNext.push_back(varEpsilon);
                kid1->pointerToNext.push_back(kid2);
                kid2->edgeToNext.push_back(varEpsilon);
                kid2->pointerToNext.push_back(endPoint);
                ++subscript;
//                kid2->edgeToNext.push_back(varEpsilon);
//                kid2->pointerToNext.push_back(kid1);
                break;
            }
                //普通字符结果
            else {
                set<char> t;
                char temp(regexText[subscript]);
                if(temp == '\\') {
                    temp = regexText[++subscript];
                    switch (temp) {
                        case 'd':
                            for(char i = '0'; i <= '9'; ++i){
                                t.insert(i);
                            }
                            break;
                        case 'n':t.insert('\n');
                            break;
                        case 'r':t.insert('\r');
                            break;
                        case 't':t.insert('\t');
                            break;
                        case 'v':t.insert('\v');
                            break;
                        case 's':
                            t.insert('\f');
                            t.insert('\n');
                            t.insert('\r');
                            t.insert('\t');
                            t.insert('\v');
                            break;
                        case '*':t.insert('*');
                            break;
//                        case '.':t.insert('.');
//                            break;
                        default:
                            ++subscript;
                            break;
                    }
                    break;
                } else {
                    t.insert(temp);
                }
                kid1->pointerToNext.push_back(kid2);
                kid1->edgeToNext.push_back(t);
                ++subscript;
            }
            //碰到星号，回环
            if (regexText[subscript] == '*') {
                ++subscript;
                kid2->edgeToNext.push_back(varEpsilon);
                kid2->pointerToNext.push_back(kid1);
                kid1->edgeToNext.push_back(varEpsilon);
                kid1->pointerToNext.push_back(kid2);
            }
            //状态转移
            node *tempNode = new node();
            kid1 = kid2;
            kid2 = tempNode;

        }
        //最后把kid2指向终点
        kid1->pointerToNext.push_back(kid2);
        kid1->edgeToNext.push_back(varEpsilon);
        kid2->edgeToNext.push_back(varEpsilon);
        kid2->pointerToNext.push_back(endPoint);

        return true;
    }

    void nfaDisplay(){

    }

};


#endif //PRINCIPLEOFCOMPLIER_FIRSTPROGRAM_REGEXTONFA_H
