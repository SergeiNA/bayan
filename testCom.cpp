#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <algorithm>

using namespace std;

class comp{
public:
    comp() =default;
    std::vector<std::string> update(std::vector<std::set<std::string>> data){
        if(cont.empty())
            cont = std::move(data);
        std::vector<std::set<std::string>> new_con;
        for(auto p:cont){
            for (auto d : data){
                
            }
        }

    }
private:
    std::vector<std::set<std::string>> cont;
};

int main(){

}