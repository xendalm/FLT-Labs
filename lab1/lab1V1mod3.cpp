#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;
map<char, set<string>> rules;
map<char, set<string>> forms;
map<char, int> classes;
map<string, vector<char>> classesCheckMap;
unsigned long long checker = 0;
void updateClasses(){
    int classNum = 1;
    checker = 0;
    for(auto elem : classesCheckMap) {
        checker |= (unsigned long long) 1 << (elem.second[0] - 'A');
        for(char nont : elem.second) classes[nont] = classNum;
        classNum++;
    }
}
void checkClasses(){
    classesCheckMap.clear();
    for(auto elem : rules){
        string tempS;
        string prevS;
        for(auto rule : elem.second){
            string newS = rule;
            for(char & c : newS){
                auto t = classes.find(c);
                if(t != classes.end()){
                    c = char('0' + classes[c]);
                }
            }
            if(newS != prevS)
                tempS += newS + "#";
            prevS = newS;
        }
        classesCheckMap[tempS].push_back(elem.first);
    }
}
int main() {
    char ch;
    scanf("nonterminals = %c", &ch);
    classes[ch] = 0;
    cin >> noskipws;
    while(true){
        cin >> ch;
        if(ch == ',' || ch == ' ') continue;
        if(ch == '\n') break;
        classes[ch] = 0;
    }
    scanf("terminals = %c", &ch);
    while(true){
        cin >> ch;
        if(ch == ',' || ch == ' ') continue;
        if(ch == '\n') break;
    }
    while(true){
        char t;
        cin >> t;
        if(cin.eof()) break;
        scanf(" -> ");
        string s;
        string s1;
        while(true){
            cin >> ch;
            if(ch == '\n') break;
            if(ch == ' ') continue;
            if(classes.find(ch) != classes.end()) {
                s1 += '_';
            }else{
                s1 += ch;
            }
            s += ch;
        }
        rules[t].insert(s);
        forms[t].insert(s1);
    }
    //preparations
    map<set<string>, vector<char>> rulesToName;
    for(auto & elem : forms) rulesToName[elem.second].push_back(elem.first);
    int classNum = 1;
    checker = 0;
    for(auto elem : rulesToName) {
        checker |= (unsigned long long) 1 << (elem.second[0] - 'A');
        for(char nont : elem.second) classes[nont] = classNum;
        classNum++;
    }
    vector<char> emptyNonterms;
    for(auto & elem : classes)
        if(elem.second == 0) emptyNonterms.push_back(elem.first);
    //checker
    while(true) {
        unsigned long long temp = checker;
        checkClasses();
        updateClasses();
        if(checker == temp) break;
    }
    //output
    if(!emptyNonterms.empty()) {
        cout << "{";
        for (int i = 0; i < emptyNonterms.size() - 1; i++) cout << emptyNonterms[i] << ",";
        cout << emptyNonterms[emptyNonterms.size() - 1] << "}";
    }
    for(auto & elem : classesCheckMap) {
        cout << "{";
        for(int i = 0; i < elem.second.size() - 1; i++)  cout << elem.second[i] << ",";
        cout <<  elem.second[elem.second.size() - 1] << "}";
    }
    cout << endl;
    map<int, char> classToName;
    for(auto & elem : classes){
        auto t = classToName.find(elem.second);
        if(t == classToName.end()) classToName[elem.second] = elem.first;
    }
    for(auto & elem : classesCheckMap) {
        char curNonTerm = elem.second[0];
        set<string> tempSet;
        for (string rule: rules[curNonTerm]) {
            string tempS = rule;
            for (char &c: tempS) {
                auto t = classes.find(c);
                if (t != classes.end()) {
                    c = classToName[t->second];
                }
            }
            tempSet.insert(tempS);
        }
        for(string s : tempSet){
            cout << curNonTerm << " -> " << s << endl;
        }
    }
    return 0;
}