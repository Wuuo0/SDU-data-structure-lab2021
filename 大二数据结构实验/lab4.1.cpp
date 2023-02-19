#include<iostream>
#include<string>
using std::cout;
using std::cin;
using std::endl;

class illegalIndex
{
    private:
        std::string message_;
    public:
        illegalIndex():message_("illgal Index ! ")              {;}
        illegalIndex(char* mes)                                 {message_=mes;}
        void Outputmessage()                                    {std::cout<<message_;}
};

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
   //ChainNode<T>* last_node_;
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
     for(int i = 0; i < N; i++)
     {
       cin >> val;
       Insert(i, val);         //listsize增加
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
   Chainiterator End()   {return Chainiterator(NULL);}
   
   void set(int, int);
   //插入
   void Insert(int index, const T& val);
   //删除
   void Erase(const T& val);
   //find
   int Find(const T& val) const;
   //原地逆转
   void Reverse();
   //输出索引与数值异或和
   void Output() ;
 protected:
   void swap(ChainNode<T>* a,ChainNode<T>* b)
   {
    ChainNode<T>* temp;
    temp = a;
    a = b;
    b = temp;
   }
};

// todo : listsize--重复
template<class T>
void Chain<T>::Erase(const T& val)
{
  if(listsize_ != 0)    //链表非空，开始查询
  {
   ChainNode<T>* deleteNode = first_node_;
   ChainNode<T>* temp = first_node_;
   int index = Find(val);
   if(index == -1)    //没有找到
       {std::cout << -1 << endl;  return;}

   //删除节点为头节点
   if(index == 0)
   {
     first_node_ = first_node_->next_;
   }
   //不是头节点
   else
   {
     for(int i = 1; i < index; i++)
     {
      temp = temp->next_;         //temp指向其前节点
     }
      deleteNode = temp->next_;
      temp->next_ = deleteNode->next_;
   }
   delete deleteNode;
   listsize_--;
  }
  else
    cout << -1 << endl;
}

template<class T>
void Chain<T>::set(int theIndex,int theElement)
{
  if(theIndex > listsize_ || theIndex < 1)
  {
    throw(illegalIndex());
  }
  
  ChainNode<T>* temp = first_node_;
  for(int i = 1; i <= listsize_; ++i)
  {
    if(i == theIndex)
    {
        temp->val_ = theElement;
        break;
    }
    temp = temp->next_;
  }
}


template<class T>
void Chain<T>::Insert(int index, const T& val)
{ 
    ChainNode<T>* insertNode = new ChainNode<T>(val);
    ChainNode<T>* temp = first_node_;
    //插入在头节点
    if(index == 0)
    {
        insertNode->next_ = first_node_;
        first_node_ = insertNode;
    }
    else
    {
      for(int i = 1; i < index; i++)
      {
        temp = temp->next_; 
      }       //temp指向index-1的前一个元素
      insertNode->next_ = temp->next_;
      temp->next_ = insertNode;
    }
    listsize_++;
}

//输出第一个val 的索引，不存在输出-1
template<class T>
int Chain<T>::Find(const T& val) const
{
  ChainNode<T>* temp = first_node_;    //从头节点开始
  int theIndex = 0;       //theindex 储存结果索引
  for( ; theIndex < listsize_; theIndex++)    //temp为空节点 或 找到时 停止
  {
    if(temp->val_ == val)  
      break;
    temp = temp->next_;
  }
  if(theIndex == listsize_)  return -1;       //为空说明不存在
  return theIndex;
}

//原地逆转
template<class T>
void Chain<T>::Reverse()
{
  ChainNode<T> *newhead, *newhead_front;    //每次使一个节点逆转，每次记录下新的逆转后的 头节点 以及 相邻节点
  newhead_front = NULL;
  newhead = first_node_;      //初始化为前两个节点和null，每次把newhead节点逆转，成为当前逆链表的头节点

  for( newhead = first_node_; newhead != NULL; )
  //while(newhead != NULL)
  {
       first_node_ = newhead->next_;       //firstNode作为正向部分的头节点
       newhead->next_ = newhead_front;   //newhead指针逆向
       newhead_front = newhead;
       newhead = first_node_;
  }
   //newhead->next_ = newhead_front;
  first_node_ = newhead_front;
}

template<class T>
void Chain<T>::Output() 
{
    Chain<T>::Chainiterator i = Begin();
    int index = 0;
    int result = 0;
    for( ; index < listsize_; index++,i++)
    {
       result += index ^ (*i);
    }
    cout << result<<endl;
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

int main()
{
    int N,Q;      //N个整数作为节点元素值创建链表
    cin >> N >> Q;
    Chain<int> myChain(N);

    int operation_num =  0;           //作为操作的临时参数
    int insert_index,insert_val = 0;  //
    int delete_val = 0;               //
    int find_val = 0;                 //
    
    for(int i = 0; i < Q; i++)
    {
      cin >> operation_num;
      switch (operation_num)
      {
      case 1 :
        cin >> insert_index >> insert_val;
        myChain.Insert(insert_index, insert_val);
        break;
      case 2 :
        cin >> delete_val;
        myChain.Erase(delete_val);
        break;
      case 3 :
        myChain.Reverse();
        break;
      case 4 :
        cin >> find_val;
        cout << myChain.Find(find_val)<<endl;
        break;
      case 5 :
        myChain.Output();
        break;
      default:
        break;
      }
    }
    return 0;
}