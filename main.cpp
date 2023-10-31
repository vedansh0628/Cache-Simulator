#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bits/stdc++.h>
using namespace std;

int main() {
    int cache;
    int block;
    cout << "Enter the cache size!" << endl;
    cin >> cache;
    cout << "Enter the block size!" << endl;
    cin >> block;

    string temp;
    string hex;
    vector<string> binary;
    ifstream file("trace/gcc.trace");
    if(file.is_open()) {
        while(getline(file, hex)) {
            temp = "";
            hex = hex.substr(hex.find(' ') + 3,8);
            for(int i = 0; i < hex.length(); i++) {
                char single = hex[i];
                if(single == '0') {
                    temp = temp + "0000";
                }
                else if(single == '1') {
                    temp = temp + "0001";
                }
                else if(single == '2') {
                    temp = temp + "0010";
                }
                else if(single == '3') {
                    temp = temp + "0011";
                }
                else if(single == '4') {
                    temp = temp + "0100";
                }
                else if(single == '5') {
                    temp = temp + "0101";
                }
                else if(single == '6') {
                    temp = temp + "0110";
                }
                else if(single == '7') {
                    temp = temp + "0111";
                }
                else if(single == '8') {
                    temp = temp + "1000";
                }
                else if(single == '9') {
                    temp = temp + "1001";
                }
                else if(single == 'A' || single == 'a') {
                    temp = temp + "1010";
                }
                else if(single == 'B' || single == 'b') {
                    temp = temp + "1011";
                }
                else if(single == 'C' || single == 'c') {
                    temp = temp + "1100";
                }
                else if(single == 'D' || single == 'd') {
                    temp = temp + "1101";
                }
                else if(single == 'E' || single == 'e') {
                    temp = temp + "1110";
                }
                else if(single == 'F' || single == 'f') {
                    temp = temp + "1111";
                }
            }
            binary.push_back(temp);
        }
        file.close();
    }
    else {
        cout << "file not opening" << endl;
    }

    int numLines = cache/block;
    double lineLength = log2(numLines);
    double offset = log2(block);

    map<string,string> directMapped;
    int dmHit = 0;
    for(int i = 0; i < binary.size(); i++) {
        string line = binary[i].substr(binary[i].length() - lineLength - offset, lineLength);
        string tag = binary[i].substr(0, binary[i].length() - lineLength - offset);
        if(directMapped.find(line) != directMapped.end()) {
            if(directMapped[line] == tag) {
                dmHit++;
            }
            else {
                directMapped[line] = tag;
            }
        }
        else {
            directMapped[line] = tag;
        }
    }
    cout << "Direct Mapped: " << (double)dmHit / binary.size() << endl;

    map<string, int> fullyAssoc;
    int faHit = 0;
    for(int i = 0; i < binary.size(); i++) {
        string line = binary[i].substr(binary[i].length() - lineLength - offset, lineLength);
        string tag = binary[i].substr(0, binary[i].length() - offset);
        if(fullyAssoc.find(tag) == fullyAssoc.end()) {
            if(fullyAssoc.size() <= numLines) {
                fullyAssoc[tag] = i;
            }
            else {
                int temp = binary.size();
                string tempTag;
                for(auto it = fullyAssoc.begin(); it != fullyAssoc.end(); it++) {
                    if(it->second < temp) {
                        temp = it->second;
                        tempTag = it->first;
                    }
                }
                fullyAssoc.erase(tempTag);
                fullyAssoc[tag] = i;
            }
        }
        else {
            faHit++;
        }
    }
    cout << "Fully Associative (FIFO): " << (double)faHit / binary.size() << endl;

    map<string, int> fullyAssoc2;
    int faHit2 = 0;
    for(int i = 0; i < binary.size(); i++) {
        string line = binary[i].substr(binary[i].length() - lineLength - offset, lineLength);
        string tag = binary[i].substr(0, binary[i].length() - offset);
        if(fullyAssoc2.find(tag) == fullyAssoc2.end()) {
            if(fullyAssoc2.size() <= numLines) {
                fullyAssoc2[tag] = i;
            }
            else {
                int temp = binary.size();
                string tempTag;
                for(auto it = fullyAssoc2.begin(); it != fullyAssoc2.end(); it++) {
                    if(it->second < temp) {
                        temp = it->second;
                        tempTag = it->first;
                    }
                }
                fullyAssoc2.erase(tempTag);
                fullyAssoc2[tag] = i;
            }
        }
        else {
            faHit2++;
            fullyAssoc2[tag] = i;
        }
    }
    cout << "Fully Associative (LRU): " << (double)faHit2 / binary.size() << endl;

    for(int setType = 2; setType <= 8; setType *= 2) {
        int numSet = numLines / setType;
        double setBit = log2(numSet);

        map<string, map<string, int>> setAssoc;
        int setHit = 0;
        for(int i = 0; i < binary.size(); i++) {
            string line = binary[i].substr(binary[i].length() - setBit - offset, setBit);
            string tag = binary[i].substr(0, binary[i].length() - offset - setBit);
            if(setAssoc.find(line) == setAssoc.end()) {
                setAssoc[line][tag] = i;
            }
            else {
                if(setAssoc[line].find(tag) != setAssoc[line].end()) {
                    setHit++;
                }
                else {
                    if(setAssoc[line].size() < setType) {
                        setAssoc[line][tag] = i;
                    }
                    else {
                        int temp = binary.size();
                        string tempTag;
                        for(auto it = setAssoc[line].begin(); it != setAssoc[line].end(); it++) {
                            if(it->second < temp) {
                                temp = it->second;
                                tempTag = it->first;
                            }
                            setAssoc[line].erase(tempTag);
                            setAssoc[line][tag] = i;
                        }
                    }
                }
            }
        }
        cout << setType << "-Way Set Associative (FIFO): " << (double)setHit / binary.size() << endl;
    }

    for(int setType = 2; setType <= 8; setType *= 2) {
        int numSet = numLines / setType;
        double setBit = log2(numSet);

        map<string, map<string, int>> setAssoc;
        int setHit = 0;
        for(int i = 0; i < binary.size(); i++) {
            string line = binary[i].substr(binary[i].length() - setBit - offset, setBit);
            string tag = binary[i].substr(0, binary[i].length() - offset - setBit);
            if(setAssoc.find(line) == setAssoc.end()) {
                setAssoc[line][tag] = i;
            }
            else {
                if(setAssoc[line].find(tag) != setAssoc[line].end()) {
                    setHit++;
                    setAssoc[line][tag] = i;
                }
                else {
                    if(setAssoc[line].size() < setType) {
                        setAssoc[line][tag] = i;
                    }
                    else {
                        int temp = binary.size();
                        string tempTag;
                        for(auto it = setAssoc[line].begin(); it != setAssoc[line].end(); it++) {
                            if(it->second < temp) {
                                temp = it->second;
                                tempTag = it->first;
                            }
                            setAssoc[line].erase(tempTag);
                            setAssoc[line][tag] = i;
                        }
                    }
                }
            }
        }
        cout << setType << "-Way Set Associative (LRU): " << (double)setHit / binary.size() << endl;
    }
    return 0;
}
