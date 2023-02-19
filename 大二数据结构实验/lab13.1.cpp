#include<iostream>
using namespace std;


template<class T> 
struct Edge
{
    int v1;
    int v2;
    T   weight;
    Edge(int v11 = -1, int v22 = -1,T wei = 0)
    {
      v1 = v11;
      v2 = v22;
      weight = wei;
    }
    ~Edge(){}
    template<class U>
    friend istream& operator >> (istream& in,Edge<U>& edge);
    Edge<T>& operator = (Edge edge)
    {
      v1 = edge.v1;
      v2 = edge.v2;
      weight = edge.weight;
      return *this;
    }
    bool operator <  (const Edge edge)
    {
      return weight < edge.weight;
    }
    bool operator <= (const Edge edge)
    {
      return weight <= edge.weight;
    }
    bool operator >  (const Edge edge)
    {
      return weight > edge.weight;
    }
    bool operator >= (const Edge edge)
    {
      return weight >= edge.weight;
    }
    bool operator == (const Edge edge)
    {
      if(weight==edge.weight && v1==edge.v1 && v2==edge.v2)
        return true;
      if(weight==edge.weight && v2==edge.v1 && v1==edge.v2)
        return true;
      return false;
    }
    bool operator != (const Edge edge)
    {
      if(weight==edge.weight && v1==edge.v1 && v2==edge.v2)
        return false;
      if(weight==edge.weight && v2==edge.v1 && v1==edge.v2)
        return false;
      return true;
    }
};

template<class T>
istream& operator >> (istream& in,Edge<T>& edge)
{
  in >> edge.v1 >> edge.v2 >> edge.weight;
  return in;
}


template<class T>
class minHeap
{//是最小堆
    private:
      T *list;                    //list[0]位置空出
      int list_size;              //
      int capacity;

      void doubleCapacity()
      {
        T *newlist = new T[capacity * 2];
        for(int i = 1; i < capacity; ++i)
            newlist[i] = list[i];
        delete[] list;
        list = newlist;
        capacity *= 2;
      }
    public:
      minHeap(int inicapacity = 10);
      ~minHeap();
      void insert(const T&);      //向堆中插入元素
      void erase();               //删除堆中最小的元素
      void iniHeap(int n);        //初始化一个元素数为n的堆
      T top()const             //返回堆顶元素
        {if(!empty())  return list[1]; else throw('0');}
      bool empty()const       
        {return list_size == 0;}      
};
template<class T>
minHeap<T>::minHeap(int ini)
{
    capacity = ini;
    list_size = 0;
    list = new T[capacity];
}
template<class T>
minHeap<T>::~minHeap()
{
    delete[] list;
}
template<class T>
void minHeap<T>::insert(const T& x)
{
    //如果列表已经满的话，先加倍容量
    if(list_size == capacity - 1)
    {
      doubleCapacity();
    }
    
    //
    int ins_index = ++list_size;     //插入到完全二叉树应该在的位置
    //然后保持小根堆的特点，对新插入点进行移动
    while(ins_index != 1 && x < list[ins_index / 2])      //
    {
        //如果小于其根节点，那么交换位置
        list[ins_index] = list[ins_index / 2]; 
        ins_index = ins_index / 2; 
    }
    list[ins_index] = x;
}
template<class T>
void minHeap<T>::erase()
{
    //堆是否为空，为空不能删除
    if(list_size == 0)
      return;

    //首先根据完全二叉树的特点记录会删去的节点
    T erase_num = list[list_size--]; 
    //每次删除最小的元素，一定是根节点的值
    // ~T(list[1]);
    //找到应该将erase_num交换到的位置
    int curr = 1;
    int curr_child = curr * 2;
    while(curr_child <= list_size)
    {
        //找到curr子节点中更小的一个
        if(curr_child < list_size && list[curr_child + 1] < list[curr_child])
          ++curr_child;
        //如果插入位置合适，就退出循环
        if(erase_num < list[curr_child])
          break;
        else
        {//否则将较小的子节点填入，向下寻找
          list[curr] = list[curr_child];
          curr = curr_child;
          curr_child = curr * 2;
        }
    }
    list[curr] = erase_num;
}
template<class T>
void minHeap<T>::iniHeap(int n)
{
    delete[] list;
    capacity = n + 1;
    list = new T[capacity];
    list_size = n;
    T temp;
    for(int i = 1; i <= n; ++i)
    {
      cin >> temp;
      list[i] = temp;
    }

    T curr;
    //将每一个有根节点的数与其子节点中的数作比较
    for(int root = list_size / 2; root >= 1; --root)
    {
        curr = list[root];
        int child = root * 2;

        while (child <= list_size)
        {
            //child等于当前根最小的子节点
            if(child < list_size && list[child + 1] < list[child])
              ++child;
            //如果根节点确实是最小的，该子树已经是最小堆
            if(curr < list[child])
              break;
            //否则交换使根成为最小的子节点值
            list[child / 2] = list[child];
            child *= 2;
        }

        list[child / 2] = curr;
    }
}


template<class T>             //T是权重weight的类型
class NEGraph
{
    protected:
      int numOfv;        //总结点数
      int numOfe;        //总边数
      minHeap<Edge<T> > edges; 
      int*  parent;       //并查集时使用,父节点为0代表没有父节点,也就是代表元素
      bool* selected;       //代表某一结点是否为所在并查集的代表元素
      int sub;            //连通分量的个数
      void inisub()                     //初始化并查集相关数据
      {
        parent = new int[numOfv+1]();
        selected = new bool[numOfv+1]();
        for(int i = 0; i <= numOfv; ++i)
        {
            selected[i] = true;
            parent[i] = 0;
        }
        //每个点都看作一个并查集
        //此时假设单独一个节点也是，如果某节点没有连通任何节点，那么-1
        sub = numOfv;
      }
      int  Findp(int x)                 //找到x节点所在并查集的代表元素
      {
        int result = x;
        while(parent[result] != 0)
        {
            result = parent[result];
        }
        return result;
      }
      void unitesub(int A,int B)        //合并两个节点
      {
        //合并时保持代表元素是并查集中最小的元素
        int topA = Findp(A);
        int topB = Findp(B);
        if(topA == topB)
            return;
        if(topA < topB)
        {
            selected[topB] = false;
            parent[topB] = topA;
        }
        else
        {  
            selected[topA] = false;
            parent[topA] = topB;
        }
        --sub;
      }
    
    public:
      NEGraph(int n = 0, int e = 0)
      {
        numOfv = n;
        numOfe = e;
      }
      void input();
      void Kruskal();
      void Prim();
      void output()
      {
        for(int i = 0; i < numOfe; ++i)
        {
            cout << edges.top().weight;
            edges.erase();
        }
      }
};

template<class T>
void NEGraph<T>::input()
{
    int n,e;
    cin >> n >> e;
    numOfv = n;
    numOfe = e;
    edges.iniHeap(e);
}

template<class T>
void NEGraph<T>::Kruskal()
{
  inisub();
  long long int result = 0;
  Edge<T> temp;
  int times = 0;          //记录选择的边数，选够n-1条边时完成
  while(!edges.empty() && times < numOfv-1)
  {
    //取出最小的边
    temp = edges.top();
    edges.erase();
    //检查两点是不是已经连通（通过并查集）
    int p1 = Findp(temp.v1);
    int p2 = Findp(temp.v2);
    if(p1 != p2)
    {
      unitesub(p1,p2);
      result += temp.weight;
      ++times;
    }
  }
  cout << result;
}



int main()
{
    NEGraph<int> gra;
    gra.input();
    gra.Kruskal();
    //gra.output();
    return 0;
}