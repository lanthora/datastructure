#include<iostream>
#include<stack>
#include<vector>
#include<queue>
#include<map>
#include<algorithm>
using namespace std;

class Map{

public:
    int m[9][9];

public:
    void init();
    int get(int n);
    int get(int x,int y);
    void set(int n,int value);
    void set(int x,int y,int value);
    void show();
};
class Sudoku{
public:
    /* 定义地图,并将所有点初始化为0 */
    Map m;
    /* 保存状态 */
    stack<Map> s;
    /* 判断在(x,y)点放value是否冲突 */
    bool conflict(const int x,const int y,const int value);
    /* 不冲突的个数 */
    int numOfNoConflict(const int x,const int y);
    /* 不冲突的值是几 */
    int valueOfNoConflict(const int x,const int y);
    /* 保存当前图状态 */
    void save();
    /* 恢复图状态 */
    void restore();
    /* 标识成功 */
    bool success();
    /* 一定失败 */
    bool wrong();    
    /* 基于数独定义的填表 */
    bool strategy1();
    bool strategy2();
    /* 假定某个点的值,并继续填表,如果出现错误,则回退到一正确状态 */
    bool strategy3();
    /* 尝试填表 */
    void solve();

    /* 该行能放value的位置数 */
    int numOfThisRowCanPutValue(int value,int row);
    /* 在row行放置value */
    void putValueOnThisRow(int value,int row);
    
    /* 该列能放value的位置数 */
    int numOfThisColCanPutValue(int value,int col);
    /* 在col列放置value */
    void putValueOnThisCol(int value,int col);

    /* 该块能放value的位置数 */
    int numOfThisBlockCanPutValue(int value,int block);
    /* 在block放置value */
    void putValueOnThisBlock(int value,int block);

    vector<int> sortedValues();
    queue<pair<int,int> > allLocateOfValues(int value);

public:

    void init();
    void show();

};
int main(){
    Sudoku sudoku;
    sudoku.init();
    while(sudoku.strategy1()||sudoku.strategy2());
    sudoku.strategy3();
    sudoku.show();
 }
void Sudoku::init(){
    m.init();
}
bool Sudoku::conflict(const int x,const int y,const int value){
    // 本身有值
    if(m.get(x,y)>0){
        return true;
    }

    // 行列判断
    for(int i=0;i<9;i++){
        if(m.get(x,i)==value||m.get(i,y)==value){
            return true;
        }
    }

    // 小方格判断
    for(int i=3*(x/3);i<3*(x/3)+3;i++){
        for(int j=3*(y/3);j<3*(y/3)+3;j++){
            if(m.get(i,j)==value){
                return true;
            }
        }
    }

    return false;
}
int Sudoku::numOfNoConflict(const int x,const int y){
    int num=0;
    for(int i=1;i<=9;i++){
        if(!conflict(x,y,i)){
            num++;
        }
    }
    return num;
}
int Sudoku::valueOfNoConflict(const int x,const int y){
    for(int i=1;i<=9;i++){
        if(!conflict(x,y,i)){
            return i;
        }
    }
    return -1;
}
void Sudoku::show(){
    m.show();
}
void Map::show(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(get(i,j)){
                cout<<get(i,j)<<" ";
            }else{
                cout<<"  ";
            }
            if(j!=8&&j%3==2){
                cout<<"|";
            }
        }
        cout<<endl;
        if(i!=8&&i%3==2){
            cout<<"--------------------"<<endl;
        }
    }
}
bool Sudoku::strategy1(){
    bool change=false;
    int flag=1;
    while(flag){
        flag=0;
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(numOfNoConflict(i,j)==1){
                    flag=1;
                    change=true;
                    m.set(i,j,valueOfNoConflict(i,j));
                    //cout<<'['<<i<<','<<j<<"]="<<m.get(i,j)<<endl;
                    continue;
                }
            }
        }
    }
    return change;
}
int Sudoku::numOfThisRowCanPutValue(int value,int row){
    int cnt=0;
    // 给出行,遍历列
    for(int col=0;col<9;col++){
        if(!conflict(row,col,value)){
            cnt++;
        }
    }
    return cnt;
}
int Sudoku::numOfThisColCanPutValue(int value,int col){
    int cnt=0;
    // 给出列,遍历行
    for(int row=0;row<9;row++){
        if(!conflict(row,col,value)){
            cnt++;
        }
    }
    return cnt;
}
int Sudoku::numOfThisBlockCanPutValue(int value,int block){
    int cnt=0;
    int x=3*(block/3);
    int y=3*(block%3);

    for(int row=x;row<x+3;row++){
        for(int col=y;col<y+3;col++){
            if(!conflict(row,col,value)){
                cnt++;
            }
        }
    }
    return cnt;
}
void Sudoku::putValueOnThisRow(int value,int row){
    for(int col=0;col<9;col++){
        if(!conflict(row,col,value)){
            m.set(row,col,value);
            //cout<<"["<<row<<','<<col<<"]="<<m.get(row,col)<<endl;

        }
    }
}
void Sudoku::putValueOnThisCol(int value,int col){
    for(int row=0;row<9;row++){
        if(!conflict(row,col,value)){
            m.set(row,col,value);
            //cout<<"["<<row<<','<<col<<"]="<<m.get(row,col)<<endl;
        }
    }
}
void Sudoku::putValueOnThisBlock(int value,int block){
    
    int x=3*(block/3);
    int y=3*(block%3);

    for(int row=x;row<x+3;row++){
        for(int col=y;col<y+3;col++){
            if(!conflict(row,col,value)){
                m.set(row,col,value);
                //cout<<"["<<row<<','<<col<<"]="<<m.get(row,col)<<endl;
            }
        }
    }
}
bool Sudoku::strategy2(){
    bool change=false;
    int flag=1;
    while(flag){
        flag=0;
        for(int value=1;value<=9;value++){
            // 第row行可以填i的位置数
            for(int row=0;row<9;row++){
                if(numOfThisRowCanPutValue(value,row)==1){
                    flag=1;
                    change=true;
                    putValueOnThisRow(value,row);
                }
            }
            // 第col列可以填i的位置数
            for(int col=0;col<9;col++){
                if(numOfThisColCanPutValue(value,col)==1){
                    flag=1;
                    change=true;
                    putValueOnThisCol(value,col);
                }
            }

            // 测试未通过
            for(int block=0;block<9;block++){
                if(numOfThisBlockCanPutValue(value,block)==1){
                    flag=1;
                    change=true;
                    putValueOnThisBlock(value,block);
                }
            }
        }
    }
    return change;
}
bool Sudoku::strategy3(){
    if(success()) return true;
    vector<int> values=sortedValues();
    for(int i=0;i<(int)values.size();i++){
        queue<pair<int,int> > q = allLocateOfValues(values[i]);
        while(!q.empty()){
            //cout<<q.front().first<<"\t"<<q.front().second<<endl;
            pair<int,int> p=q.front();
            q.pop();
            save();
            m.set(p.first,p.second,values[i]);
            while(strategy1()||strategy2());
            //show();
            if(success()){
                return true;
            }
            if(wrong()){
                restore();
                continue;
            }
            if(q.empty()&&i==(int)values.size()-1){
                return false;
            }
            if(strategy3()){
                return true;
            } else{
                restore();
            }
        }
    }
    return false;
}

bool cmp(pair<int,int> a,pair<int,int> b){
    return a.second>b.second;
}

vector<int> Sudoku::sortedValues(){
    vector<int> ret;
    ret.push_back(1);
    ret.push_back(2);
    ret.push_back(3);
    ret.push_back(4);
    ret.push_back(5);
    ret.push_back(6);
    ret.push_back(7);
    ret.push_back(8);
    ret.push_back(9);
    return ret;
}

queue<pair<int,int> > Sudoku::allLocateOfValues(int value){
    queue<pair<int,int> > v;
    pair<int,int> p;
    for(int i=0;i<81;i++){
        if(!conflict(i/9,i%9,value)){
            p.first=i/9;
            p.second=i%9;
            v.push(p);
        } 
    }
    return v;
}

void Sudoku::save(){
    Map t;
    for(int i=0;i<81;i++){
        t.set(i,m.get(i));
    }
    s.push(t);
}
void Sudoku::restore(){
    Map t=s.top();
    for(int i=0;i<81;i++){
        m.set(i,t.get(i));
    }
    s.pop();
}
bool Sudoku::success(){
    for(int i=0;i<81;i++){
        if(m.get(i)==0){
            return false;
        }
    }
    return true;
}
bool Sudoku::wrong(){
    for(int i=0;i<81;i++){
        int x=i/9;
        int y=i%9;
        if(!(m.get(i)&&numOfNoConflict(x,y))){
            return true;
        }
    }
    return false;
}
void Sudoku::solve(){
}

int Map::get(int n){
    int x=n/9;
    int y=n%9;
    return m[x][y];
}
int Map::get(int x,int y){
    return m[x][y];
}
void Map::set(int n,int value){
    int x=n/9;
    int y=n%9;
    m[x][y]=value;
}
void Map::set(int x,int y,int value){
    m[x][y]=value;
}
void Map::init(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin>>m[i][j];
        }
    }
}

