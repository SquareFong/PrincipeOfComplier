#include <iostream>
#include "RegexToNFA.h"
#include "NFAtoDFA.h"
#include <map>
using namespace std;
class wordAnalyse {
    NFAtoDFA n;
    map<char, vector<char>> modifiedDFA;
    set<char> modifiedNFA_endStates;
public:
    wordAnalyse(string regex):n(regex){

    }
    void minimizeDFA() {
        //NFA
        n.startBuildingNFA();
        //DFA
        n.startBuilding();
        n.display();
        vector<vector<int>> DFA = n.getDFA();
        /*
         * minimize
         * */
        //给状态命名
        for (int i = 0; i < DFA.size(); ++i) {
            vector<int> &tempDFA = DFA[i];
            vector<char> tempModified(128, ' ');
            for (int j(0); j < tempDFA.size(); ++j) {
                if (tempDFA[j] < 0)
                    continue;
                else
                    tempModified[j] = (char) (tempDFA[j] + 'A');
            }
            modifiedDFA.insert(make_pair((char) (i + 'A'), tempModified));
        }
        auto it_endstates = n.allEndStates.begin();
        while (it_endstates != n.allEndStates.end()) {
            modifiedNFA_endStates.insert((char) ((*it_endstates) + 'A'));
            ++it_endstates;
        }
        auto it1 = modifiedDFA.begin();
        //查重
        while (it1 != modifiedDFA.end()) {
            auto it2 = it1;
            ++it2;
            //存储重复的名字
            set<char> sameState;

            while (it2 != modifiedDFA.end()) {
                vector<char> t1 = (*it1).second;
                vector<char> t2 = (*it2).second;
                int tn = 0;
                for (; tn < t1.size(); ++tn) {
                    if (t1[tn] != t2[tn])
                        break;
                }
                //如果能循环完，说明it1 it2完全相等，那么it2的状态名加入重复状态列表，以待清除
                if (tn == t1.size())
                    sameState.insert((*it2).first);
                ++it2;
            }

            //逐行扫描，将所有叫it2的状态名的改成it1
            it2 = modifiedDFA.begin();
            while (it2 != modifiedDFA.end()) {
                vector<char> &t = (*it2).second;
                for (int i = 0; i < t.size(); ++i) {
                    if (sameState.find(t[i]) != sameState.end()) {
                        t[i] = (*it1).first;
                    }
                }
                ++it2;
            }

            auto iterator1 = sameState.begin();
            while (iterator1 != sameState.end()) {
                modifiedDFA.erase((*iterator1));
                ++iterator1;
            }

            if (sameState.empty())
                ++it1;
            else
                it1 = modifiedDFA.begin();
        }

        cout << "Simplified DFA: \n";
        cout << ' ' << '\t' << '+' << '\t' << '-' << '\t' << 'x' << '\t' << '/' << '\t';
        for (int i(0); i <= 9; ++i)
            cout << i << '\t';
        cout << endl;


    }
    void display(){
        auto p = modifiedDFA.begin();
        while (p != modifiedDFA.end()) {
            cout << (*p).first << '\t';
            vector<char> tempDFA = (*p).second;
            cout << tempDFA['+'] << '\t' << tempDFA['-'] << '\t' << tempDFA['x'] << '\t' << tempDFA['/'] << '\t';
            for (char i('0'); i <= '9'; ++i) {
                cout << tempDFA[i] << '\t';
            }
//        for(int i(0); i < tempDFA.size(); ++i){
//            cout << tempDFA[i] << '\t';
//        }
            cout << endl;
            ++p;
        }
        cout <<endl;
        cout << "终结符集： \n";
        auto pme = modifiedNFA_endStates.begin();
        while ( pme != modifiedNFA_endStates.end()){
            cout << (*pme) << ' ' ;
            ++pme;
        }
        cout <<endl;
    }

    bool isAccept(string text) {
        auto it = text.begin();
        char cDFAStateName = 'A';
        vector<char> cDFAState = (*modifiedDFA.find(cDFAStateName)).second;
        while (it != text.end()) {
            char nextStateName = cDFAState[*it];
            if (nextStateName != ' ') {
                cDFAStateName = nextStateName;
                cDFAState = (*modifiedDFA.find(cDFAStateName)).second;
            } else {
                break;
            }
            ++it;
        }
        if ((it == text.end()) && (modifiedNFA_endStates.find(cDFAStateName) != modifiedNFA_endStates.end())) {
            return true;
        } else {
            return false;
        }

    }
};


int main() {



    wordAnalyse w("[1-9][0-9]*(x|+|/|-)[1-9][0-9]*|[1-9][0-9]*");
    string reg;
    cout << "请输入正则表达式,如：[1-9][0-9]*(x|+|/|-)[1-9][0-9]*|[1-9][0-9]*：（请勿带空格） \n";
    cin >>  reg;

    if(reg.length())
        w = wordAnalyse(reg);

    w.minimizeDFA();

    w.display();

    cout << "自动机构造完成，请输入式子匹配： ";
    string text;
    while (cin >> text) {
        cout << "表达式：" << text;
        if(w.isAccept(text))
        {
            cout << " 匹配成功\n";
        } else {
            cout << " 匹配失败\n";
        }

        cout << "请继续输入以匹配：";
    }
    //std::cout << "Hello, World!" << std::endl;
    return 0;
}