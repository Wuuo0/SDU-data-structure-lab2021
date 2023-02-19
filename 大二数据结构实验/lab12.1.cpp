#include<iostream>
using namespace std;

//引入队列arrayQueue
template<class T>
class ArrayQueue
{
  protected:
    T*  array_;       //数组
    int capacity_;    //最大容量
    int queue_front_; //队首元素索引,环形表，表头为空
    int queue_back_;  //队尾元素索引,指向队尾元素而不是下一位

  public:
  //ADT
    bool empty() const;  //队列为空时返回true
    int size() const;    //返回队列当前大小
    T& front();          //返回队头元素的引用
    T& back();          //返回队尾元素的引用
    void pop();          //删除队头元素
    void push(const T& theElement);   //将元素加入队尾
    
    ArrayQueue(int initialCapa = 10);
    virtual ~ArrayQueue();
};
//ArrayQueue的方法实现
template<class T>
ArrayQueue<T>::ArrayQueue(int initialCapa)
{
  capacity_ = initialCapa;
  array_ = new T[capacity_];
  queue_front_ = 0;
  queue_back_ = 0; 
}
template<class T>
void ArrayQueue<T>::push(const T& theElement)
{
  //队列是否已经满了，是否需要加倍容量
  if((queue_back_+1)%capacity_ == queue_back_)
  {
    //todo加倍数组
    T* newarray = new T[capacity_ * 2];
    //没有成环
    if(queue_front_ == 0 || queue_front_ == capacity_-1)
    {
      for(int i = 1; i < capacity_; ++i)
      {
        newarray[i] = array_[(queue_front_ + i) % capacity_];
      }
    }
    else
    {
      int i = 1, index = queue_front_+1;
      while(index != capacity_)
      {
        newarray[i++] = array_[index++];
      }    //复制array[front+1 : capacity-1]
      index = 0;
      while(index != queue_front_)
      {
        newarray[i++] = array_[index++];
      }    //复制array[0 : front-1]
    }
    delete[] array_;
    int nowsize = size();
    array_ = newarray;          //更新array_
    queue_front_ = 0;           //更新front
    queue_back_ = queue_front_ + nowsize;  //更新back
    capacity_ *= 2;             //更新capacity 
  }
  //插入元素
  queue_back_ = (queue_back_+1) % capacity_;
  array_[queue_back_] = theElement;
}
template<class T>
bool ArrayQueue<T>::empty()const
{
  if(queue_front_ == queue_back_)
    return true;
  return false;
}
template<class T>
void ArrayQueue<T>::pop()
{
  //判断队列是否为空
  if(empty())
  {
    cout << "Empty Queue !" <<endl;
    return;
  }
  
  //非空时，从队头删除
  queue_front_ = (queue_front_+1)%capacity_;
  array_[queue_front_].~T();
}
template<class T>
int ArrayQueue<T>::size() const
{
  return (queue_back_ - queue_front_ + capacity_) % capacity_;
}
template<class T>
T& ArrayQueue<T>::front()
{
  if(empty())
  {
    cout <<" Empty Queue ! "<<endl;
    throw ('-1');
  }
  return array_[(queue_front_ + 1)%capacity_];
}
template<class T>
T& ArrayQueue<T>::back()
{
  if(empty())
  {
    cout <<" Empty Queue ! "<<endl;
    throw ('-1');  
  }
  return array_[queue_front_];
}
template<class T>
ArrayQueue<T>::~ArrayQueue()
{
  delete[] array_;
}



//图节点
template<class T>
struct GNode
{
    T element_;
    GNode<T>* next_;
    GNode(T ele, GNode<T>* next = nullptr)
    {
        element_ = ele;
        next_ = next;
    }
};
//前向声明
class LinkedGraph;
//图的链表
template<class T>
class GChain
{
  friend class LinkedGraph;
    protected:
      GNode<T>* head_;
      int size_;
    public:
      GChain()
      {
        size_ = 0;
        head_ = nullptr;
      }
      ~GChain()
      {
        GNode<T>* temp = nullptr;
        while(head_ != nullptr)
        {
            temp = head_;
            head_ = head_->next_;
            delete temp;
        }
      }
      void push(T element)           //按照大小排序
      {
        //已经存在，直接返回
        if(find(element) != nullptr)
          return;

        //不存在该点时插入
        GNode<T>* temp = new GNode<T>(element);
        //链表为空
        if(head_ == nullptr)
          head_ = temp;
        //链表不为空
       
        else
        {
           //1.插入头节点
          if(element < head_->element_)
          {
            temp->next_ = head_;
            head_  = temp;
          }
          //2.插入非头节点 
          else
          {
            GNode<T>*pp = head_,
                    *p  = head_->next_;
            while(element > pp->element_)
            {
              //如果element大小在pp < ele < p,或者p为空
              if(p == nullptr || element < p->element_)
              {
                pp->next_ = temp;
                temp->next_ = p;
                break;
              }
              pp = pp->next_;
              p = p->next_;
            
            }
          }
        }
        ++size_;
      }
      GNode<T>* find(T element)     //返回该节点的指针，找不到返回空指针null
      {
        GNode<T>* temp = head_;
        while(temp != nullptr)
        {
          if(temp->element_ == element)
          {
            break;
          }
          temp = temp->next_;
        }
        return temp;
      }
      void erase(T element)
      {
        GNode<T>* temp = find(element);
        //不存在该节点
        if(temp == nullptr)
        {
          return;
        }
        //存在该节点
        //删除头节点
        if(temp == head_)
        {
          head_ = head_->next_;
        }
        //不是头节点
        else
        {
          GNode<T>* p = head_;
          while(p->next_ != temp)
          {
            p = p->next_;
          }
          p->next_ = temp->next_;
        }

        --size_;
        delete temp;
      }
};
//邻接链表图
class LinkedGraph
{
  protected:
    int numOfv;      //点数
    int numOfe;      //边数
    GChain<int>* list; //邻接链表
    int s,t;
    int*  parent;       //并查集时使用,父节点为0代表没有父节点,也就是代表元素
    bool* selected;       //代表某一结点是否为所在并查集的代表元素
    int  sub;          //连通分量的个数
    void inisub()                     //初始化并查集相关数据
    {
      parent = new int[numOfv+1]();
      isroot = new bool[numOfv+1]();
      for(int i = 1; i <= numOfv; ++i)
      {
        isroot[i] = true;
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
        isroot[topB] = false;
        parent[topB] = topA;
      }
      else
      {  
        isroot[topA] = false;
        parent[topA] = topB;
      }
      --sub;
    }
    
  public:
    LinkedGraph()
    {
      ;
    }
    ~LinkedGraph()
    {
      delete[] list;
      delete[] parent;
      delete[] isroot;
    }
    void insert(int u, int v);         //插入一条边(u,v)
    void erase(int u, int v);          //删除一条边(u,v)
    void input();                      //输入数据
    void getsub();                     //输出连通子图数  和  子图中最小的编号
    void dfs();                        //深度优先
    void rDFS(int v, int* lable, ArrayQueue<int>& res, int Reached = 1);                 //
    void bfs();
};


void LinkedGraph::insert(int u, int v)
{
  //若果已经存在了这条边，那么不插入
  if(list[u].find(v) != nullptr)
    return;
  //自环
  if(u == v)
    return;
  //在list[u],list[v]都插入一条边，边数e加一
  list[u].push(v);
  list[v].push(u);
  ++numOfe;
}

void LinkedGraph::erase(int u, int v)
{
  //若果不存在这条边，无法删除
  if(list[u].find(v) == nullptr)
    return;
  //自环
  if(u == v)
    return;
  //在list[u],list[v]都删去，边数e减一
  list[u].erase(v);
  list[v].erase(u);
  --numOfe;
}

void LinkedGraph::input()
{
  //更新邻接链表信息
  int n,m,s,t;
  cin >> n >> m >> s >> t;
  numOfv = n;
  list = new GChain<int>[n+1];
  this->s = s;
  this->t = t;

  int op;
  int u,v;
  for(int i = 0; i < m; ++i)
  {
    cin >> op >> u >> v;
    switch (op)
    {
    case 0:
      insert(u,v);
      break;
    case 1:
      erase(u,v);
      break;
    default:
      break;
    }
  }
}

void LinkedGraph::getsub()
{
  //初始化
  inisub();
  //完成并查集的合并
  for(int i = 1; i <= numOfv; ++i)
  {
    GNode<int>* temp = list[i].head_;
    if(temp == nullptr)
    {
      --sub;
      isroot[i] = false;
      continue;
    }
    while(temp != nullptr)
    {
      unitesub(i,temp->element_);
      temp = temp->next_;
    }
  }
  //输出有多少连通分量
  cout << sub << endl;
  //输出所有连通子图中最小点的编号（升序）
  for(int i = 1; i <= numOfv; ++i)
  {
    if(isroot[i] == true)
    {
      cout << i << ' ';
    }
  }
  cout << endl;
}

void LinkedGraph::dfs()
{
  //初始化一个标签记录，该点是否到达过
  int* label = new int[numOfv+1];
  for(int i = 1; i <= numOfv; ++i)
    label[i] = 0;
  //记录到达的顺序的队列
  ArrayQueue<int> result(numOfv+1);
  //递归调用rDFS
  rDFS(this->s,label,result,1);
  cout << result.size() << endl;
  //输出dfs序列
  while(!result.empty())
  {
    cout << result.front() << ' ';
    result.pop();
  }
  cout << endl;
  
  delete[] label;
}

void LinkedGraph::rDFS(int v,int* label, ArrayQueue<int>& res, int reached)
{
  //标记该点已到达，并加入队列中
  label[v] = reached;
  res.push(v);
  //寻找它的邻节点
  GNode<int>* temp = list[v].head_;
  int next;
  while(temp != nullptr)
  {
    next = temp->element_;
    //如果没有到达过,就进入递归
    if(label[next] == 0)
    {
      rDFS(next,label,res,reached);
    }
    temp = temp->next_;
  }
}

void LinkedGraph::bfs()
{

  int unreached = -1;
  int reached = 0;
  int* label = new int[numOfv+1];
  for(int i = 0; i <= numOfv; ++i)
  {
    label[i] = unreached;
  }
  ArrayQueue<int> que(numOfv+1);
  ArrayQueue<int> res(numOfv+1);     //记录结果
  label[t] = reached;
  que.push(t);
  res.push(t);
  while(!que.empty())
  {
    int curr = que.front();
    que.pop();

    for(GNode<int>* p = list[curr].head_; p != nullptr; p = p->next_)
    {
      if(label[p->element_] ==  unreached)
      {      
        que.push(p->element_);
        res.push(p->element_);
        label[p->element_] = label[curr]+1;
      }
    }    
  }
  //输出数据
  cout << res.size() << endl;
  while(!res.empty())
  {
    cout << res.front() << ' ';
    res.pop();
  }
  cout << endl << label[s];

  delete[] label;
}

int main()
{
  LinkedGraph graph;
  graph.input();
  graph.getsub();
  graph.dfs();
  graph.bfs();
  return 0;
}