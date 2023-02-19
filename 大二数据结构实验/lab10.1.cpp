#include<iostream>
#include<sstream>
using std::cin;
using std::cout;
using std::endl;

std::ostringstream s;

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
    ~T(list[1]);
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
    T x;
    for(int i = 1; i <= n; ++i)
    {
      cin >> x;
      list[i] = x;
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

template<class T>
void sort() 
{
    int n;             //待排序元素个数
    cin >> n;
    minHeap<T> sortHeap(n+1);
    T x;
    //将n个数插入堆中
    for(int i = 0; i < n; ++i)
    {
        cin >> x;
        sortHeap.insert(x);
    }
    //再将n个数从堆中删除
    for(int i = 0; i < n; ++i)
    {
        s << sortHeap.top() << ' ';
        sortHeap.erase();
    }
    s << endl;
}

int main()
{
    int n;
    cin >> n;
    minHeap<int> heap;
    heap.iniHeap(n);
    s << heap.top() << endl;

    int m;
    int ope;
    int ins;

    cin >> m;
    for(int i = 0; i < m; ++i)
    {
      cin >> ope;
      switch (ope)
      {
      case 1 :
        cin >> ins;
        heap.insert(ins);
        s << heap.top() << endl;
        break;
      case 2 :
        heap.erase();
        s << heap.top() << endl;
        break;
      case 3 :
        sort<int>();
        break;
      default:
        break;
      }
    }

    cout << s.str();
    return 0;
}
