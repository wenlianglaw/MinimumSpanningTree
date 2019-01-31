#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <functional>
#include <numeric>
#include <istream>
#include <chrono>
#include <string>
#include <set>

using namespace std;
using namespace std::chrono;

// vertices count, if it is -1, then generate one.  If not, use it.
int global_m = -1;

class Edge{
    public:
        int to, weight;
};
class Vertice{
    public:
        int no;
        list<Edge> adj;
};
class MST{
    private:
        const int INF = 0x3fffffff;
        // m vertices, n edges
        int m, n;
        vector<Vertice> vertices;
        vector<vector<int>> matrix;
    public:
        void ReadData(string filename){
            ifstream in(filename, ifstream::in);
            if(!in){
                cout<<"Cannot open file!"<<endl;
                exit(0);
            }
            ReadData(in);
            in.close();
        }

        void ReadData(istream  &stream = cin){
            stream>>m>>n;
            vertices = vector<Vertice>(m,Vertice());
            matrix = vector<vector<int>>(m,vector<int>(m,INF));
            for(int i=0;i<n;i++){
                int from, to, weight;
                stream>>from>>to>>weight;
                vertices[from].adj.push_back({to,weight});
                vertices[to].adj.push_back({from,weight});
                matrix[from][to] = weight;
                matrix[to][from] = weight;
            }
        }

        /* Using priority queue.
            O(E * log(E)) = O(E*logV)
         */
        int MstWeightPQ(){
            int rst = 0;
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> mst; 

            vector<int> key(m,INF);
            vector<bool> inMst(m,false);

            int src = vertices[0].no;
            key[src] = 0;
            mst.push({0,src});
            
            while(!mst.empty()){
                int to = mst.top().second;
                mst.pop();
                if(inMst[to]) continue;

                inMst[to] = true;

                for(auto v : vertices[to].adj){
                    if(!inMst[v.to] && key[v.to] > v.weight){
                        key[v.to] = v.weight;
                        mst.push({v.weight, v.to});
                    }
                }
            }

            rst = accumulate(key.begin(),key.end(),0);
            return rst;
        }

        /* O(V^2) */
        int MstWeightV2(){
            int rst = 0;
            vector<bool> inMst(m,false);
            vector<int> key(m,INF);
            key[vertices[0].no] = 0;
            for(int i=0;i<m;i++){
                int newVertice = 0;
                int min = INF;
                for(int i=0;i<m;i++){
                    if(!inMst[i] && key[i] < min)
                        newVertice = i, min = key[i];
                }
                rst += key[newVertice];
                inMst[newVertice] = true;
                for(int i=0;i<m;i++){
                    if(matrix[newVertice][i] != INF && !inMst[i]
                            && key[i] > matrix[newVertice][i])
                        key[i] = matrix[newVertice][i];
                }
            }
            return rst;
        }

        void RunPQ(){
            cout<<m<<" "<<n<<endl;
            cout<<MstWeightPQ()<<endl;
        }

        void RunV2(){
            cout<<m<<" "<<n<<endl;
            cout<<MstWeightV2()<<endl;
        }
};

void Timer(function<void(void)> func, string someComments=string()){
    cout<<someComments<<endl;
    chrono::high_resolution_clock::time_point t1 = high_resolution_clock::now();  
    func();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2-t1).count();
    cout<<duration<<"ms"<<endl;
}

void MakeData(string filename){
    cout<<"Generating data..."<<endl;
    fstream out(filename,fstream::out|fstream::in|fstream::trunc);
    int m=0;
    if(global_m != -1) m = global_m;
    while(!m) m = rand()%1000;
    // Try to write n edges, but will discard the invalid one.  So the actual edges amount will be LE n
    int n=0;
    while(!n) n = rand()%(m*m/2);
    if(n<m-1) n = m+10;
    out<<m<<" "<<n<<endl;

    set<int> selected;
    for(int i=0;i<m-1;i++){
        out<<i<<" "<<i+1<<" "<<rand()%10000<<endl;
        selected.insert(i*m+i+1);
        selected.insert((i+1)*m+i);
    }

    // The real wrote edges
    int realN = m-1;
    for(int i=0;i<n+1-m;i++){
        int from=0,to=0;
        while(from==to) from = rand()%m,to=rand()%m;
        if(selected.find(from*m+to) != selected.end()
                || selected.find(to*m+from) != selected.end()) continue;
        out<<from<<" "<<to<<" "<<rand()%10000<<endl;
        realN ++;
        selected.insert(to*m+from);
        selected.insert(from*m+to);
    }
    out.seekg(0,ios::beg);
    int a,b;
    out>>a>>b;
    out.seekp(0, ios::beg);
    out<<a<<" "<<realN<<endl;
    out.close();
    cout<<"Generate Done!"<<endl;
}

void ShowHelp(){
    cout<<"mst {Vertics Count:int} {Regen Data:1/0}"<<endl;
    cout<<endl;
}


int main(int argc, char **args){
    ShowHelp();
    bool regenData = true;
    // See Help function for help.
    if(argc >= 2) {
        global_m = std::stoi(args[1]);
        if(argc == 3)
            regenData = args[2][0] == '1';
    }
    
    srand(time(0));

    string dataFile = "testingData.dat";
    if(regenData) MakeData(dataFile);

    MST mst;
    mst.ReadData(dataFile);
    
    Timer(bind(&MST::RunPQ, mst));
    Timer(bind(&MST::RunV2, mst));
    
    return 0;
}
