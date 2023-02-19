#include<iostream>
using std::cin;
using std::cout;
using std::endl;
template<class T>class Chain;   //Chain前置声明

template<class T>
class ChainNode
{
  friend  class Chain<T>;        //Chain为友元，便于访问
  private:
    ChainNode<T>* next_;
    T   val_;
  public:
    ChainNode()    {;}
    //ChainNode(const T& val)  {val_ = val;;}
    ChainNode(const T& val,ChainNode<T>* next = NULL)
    {
        next_ = next;
        val_ = val;
    }
};

template<class T>
class Chain
{
 protected:
   ChainNode<T>* first_node_;
   int  listsize_;
 public:
 //构造
   Chain()
   {
    listsize_ = 0;
    first_node_ = NULL;
   }
   Chain(int N)
   {
    first_node_ = NULL;
    listsize_ = 0;
    int val;
     for(int i = 0; i < N; ++i)
     {
       cin >> val;
       Push_back(val);         //listsize增加
     }
    
   }
   //复制构造 todo
   Chain(const Chain& theChain)
   {

   }
   //析构
   ~Chain();

   //迭代器
    class Chainiterator
   {
    protected:
      ChainNode<T>* node_;
    public:
      //前向迭代器声明
      //typedef Chain<T>::iterator iterator;
      // todo :

      //构造
      Chainiterator(ChainNode<T>* theNode)
          { node_ = theNode; }
      //解引用
      T& operator * () const    { return node_->val_; }
      T* operator -> () const   { return &node_->val_;}
      //迭代器加法操作
      Chainiterator& operator ++()  {node_ = node_->next_;  return *this;}
      Chainiterator operator ++(int)  
      {
        Chainiterator old = *this;
        node_ = node_->next_;
        return old;
      } 
      //相等检验
      bool operator != (const Chainiterator& right) const
         {return node_ != right.node_;}
      bool operator == (const Chainiterator& right) const
         {return node_ == right.node_;}
      //Chainiterator Next() {return Chainiterator(node_->next_);}
   };
   //迭代器方法
   Chainiterator Begin() {return Chainiterator(first_node_);}
   Chainiterator End()   {return Chainiterator(nullptr);}
   
   //插入
   void Push_back(const T& val);
   //将两个链表合并赋给自己
   void Merge(Chain<T>&,  Chain<T>&);
   //输出异或值
   void OutputXor() ;
   //将链表排序
   void Sort();
   void out();
 protected:
   void swap(T& a,T& b)
   {
    T temp;
    temp = a;
    a = b;
    b = temp;
   }
};

template<class T>
void Chain<T>::Sort()
{
    if(listsize_ == 0)   return;     //链表为空

    bool sorted = false;
    Chainiterator front = Begin();
    Chainiterator behind = Begin();     ++behind;
    for(int i = listsize_; i >= 2 && !sorted; --i)    //第i个数排好 且未有序
    {
       sorted = true;               //默认假设已经有序
       front = behind =  Begin();   // 
       ++behind;                    //将front，behind调到前两个节点
       for(int j = 2; j <= i; j++)   //j的位置与behind的索引一致
       {
        if(*front > *behind)
        {
            swap(*front, *behind);   //将大的数调到后边
            sorted = false;          //发生交换说明仍未有序
        }
        ++front;
        ++behind;
       }
    }
}

template<class T>
void Chain<T>::OutputXor()
{
    Chainiterator ite = Begin();
    int result = 0;
    for(int i = 0; i < listsize_; ++i,++ite)
    {
       result += i ^ *ite;
    }
    cout << result <<endl;
}

template<class T>
void Chain<T>::Push_back(const T& val)
{
    ChainNode<T>* newnode = new ChainNode<T>(val);
    if(listsize_ == 0)
    {
       first_node_ = newnode;
    }
    else
    {
        ChainNode<T>* lastnode = first_node_;
        for(int i = 2; i <= listsize_; ++i)
        {
           lastnode = lastnode->next_;
        }
        lastnode ->next_ = newnode;
    }
    ++listsize_;
}

template<class T>
void  Chain<T>::Merge(Chain<T>& chain1, Chain<T>& chain2)
{
  Chainiterator ite1 = chain1.Begin(), ite2 = chain2.Begin();
  while(ite1 != nullptr && ite2 != nullptr)
  {
    if(*ite1 > *ite2)      //将小的插入back
       {
        Push_back(*ite2);
        ++ite2;               //插入后迭代器++
       }
    else
    {
        Push_back(*ite1);
        ++ite1;
    }
  }                       //结束时至少一个ite为空
  
  while(ite1 != nullptr)             //若ite2为空，将ite1剩下的全部加入
  {
    Push_back(*ite1);
    ite1++;
  }
  
  while(ite2 != nullptr)             //若ite1为空，将ite2剩下的加入
  {
    Push_back(*ite2);
    ite2++;
  }
}

template<class T>
Chain<T>::~Chain()
{
    ChainNode<T>* temp = first_node_;
    for(int i = 0; i < listsize_; i++)
    {
        temp = first_node_->next_;
        delete first_node_;
        first_node_ = temp;
    }
    listsize_ = 0;
}

template<class T>
void Chain<T>::out()
{
    Chainiterator ite = Begin();
    while(ite != nullptr)
    {
        cout << *ite << "  ";
        ++ite;
    }
    cout << endl;
}

int main()
{
    int N,M;
    cin >> N >> M;
    Chain<int> chain1(N);
    Chain<int> chain2(M);
    Chain<int> chain3;
    chain1.Sort();
    chain2.Sort();
    //chain1.out();
    //chain2.out();
    chain1.OutputXor();
    chain2.OutputXor();
    chain3.Merge(chain1, chain2);
    chain3.OutputXor();
    return 0;
}