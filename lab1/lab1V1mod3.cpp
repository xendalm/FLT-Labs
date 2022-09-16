#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;
map<char, set<string>> rules;
map<char, set<string>> forms;
map<char, int> classes;
map<string, vector<char>> classesCheckMap;
vector<pair<char, string>> nontermsToCheck;
string checkString;
void setNontermsToCheck(){
    for(auto elem : classes) {
        string tempS;
        for(string rule :  rules[elem.first]){
            bool f = false;
            for(char & c : rule){
                if(classes.find(c) != classes.end()) {
                    tempS += c;
                    f = true;
                }
            }
            if(!f) tempS += rule;
            tempS += '#';
        }
        nontermsToCheck.emplace_back(elem.first, tempS);
    }
}
void updateClasses(){
    int classNum = 1;
    for(auto elem : classesCheckMap) {
        if(elem.first == "") continue;
        for(char nont : elem.second){
            classes[nont] = classNum;
        }
        classNum++;
    }
    checkString = "";
    for(auto & elem : classes) {
        checkString += char('0' + elem.second);
    }
}
void checkClasses(){
    classesCheckMap.clear();
    for(auto elem : nontermsToCheck){
        string tempS = elem.second;
        for(char & c : tempS){
            auto t = classes.find(c);
            if(t != classes.end()){
                c = char('0' + classes[c]);
            }
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
    for(auto & elem : rulesToName) {
        for(char nont : elem.second){
            classes[nont] = classNum;
        }
        classNum++;
    }
    checkString = "";
    for(auto & elem : classes) {
        checkString += char('0' + elem.second);
    }
    setNontermsToCheck();
    //checker
    while(true) {
        string temp = checkString;
        checkClasses();
        updateClasses();
        if(checkString == temp) break;
    }
    //output
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
