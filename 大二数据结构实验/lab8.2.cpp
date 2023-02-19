#include<iostream>
using namespace std;

template<class T>
struct ChainNode
{
    ChainNode<T>* next_;
    T   val_;
    ChainNode(const T& val = 0,ChainNode<T>* next = nullptr)
    {
        next_ = next;
        val_ = val;
    }
};

template<class T> class HashChain;
template<class T>
class Chain
{  
    friend class HashChain<T>;
 protected:
   ChainNode<T>* first_node_;
   int  listsize_;              //链表当前大小
 public:
   Chain();
   ~Chain();
   //插入
   void Push_back(const T& val);
   int size()  {return listsize_;}
};



template<class T>
Chain<T>::Chain()
{
  listsize_ = 0;
  first_node_ = nullptr;
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
class HashChain
{
    private:
      int divisor_;
      ChainNode<T> **list_;
      int *size_;
    public:
      HashChain(int divisor);
      ~HashChain();
      void insert(const T& x);
      int  find(const T& x);
      void erase(const T& x);
};

template<class T>
HashChain<T>::HashChain(int divisor)
{
    divisor_ = divisor;
    list_ = new ChainNode<T>*[divisor_]();
    size_ = new int[divisor_]();
}

template<class T>
HashChain<T>::~HashChain()
{
    delete [] size_;
    ChainNode<T> *deletenode,*deletefront;
    for(int i = 0; i < divisor_; ++i)
    {
      deletenode = deletefront = list_[i];
      while(deletenode != nullptr)
      {
        deletenode = deletenode->next_;
        delete deletefront;
        deletefront = deletenode;
      }
    }
    delete [] list_;
}

template<class T>
void HashChain<T>::insert(const T& x)
{
    //先查找是否已经存在
    if(find(x) != -1)
    {
        cout << "Existed" << endl;
        return;
    }
    
    ChainNode<T> *newnode = new ChainNode<T>(x);
    int bucketOfx = x % divisor_;
    ChainNode<T> *last = list_[bucketOfx];
    if(last == nullptr)
    {
      list_[bucketOfx] = newnode;
      ++size_[bucketOfx];
      return;
    }
    while(last->next_ != nullptr)
    {  
      last = last->next_;
    }
    last->next_ = newnode;
    ++size_[bucketOfx];
}

template<class T>
int HashChain<T>::find(const T& x)
{
    int bucketOfx = x % divisor_;
    ChainNode<T> *p = list_[bucketOfx];
  
    while(p != nullptr)
    {
      if(p->val_ == x)      //查找到了该元素
        return size_[bucketOfx];
      
      p = p->next_;
    }
    return -1;
}

template<class T>
void HashChain<T>::erase(const T& x)
{
    if(find(x) == -1)
    {
        cout << "Delete Failed" <<endl;
        return;
    }

    int bucketOfx = x % divisor_;
    ChainNode<T> *head = list_[bucketOfx];
    ChainNode<T> *deleteNode = head;
     ChainNode<T> *deleteFront = head;
    //执行删除操作
    if(head->val_ == x) //为头节点
    {
      list_[bucketOfx] = head->next_;
    }
    else
    {
      while(deleteFront->next_->val_ != x)
      {
        deleteFront = deleteFront->next_;
      }
      deleteNode = deleteFront->next_;
      deleteFront->next_ = deleteNode->next_;
    }

    delete deleteNode;
    --size_[bucketOfx];
    cout << size_[bucketOfx] <<endl;
}

int main()
{
    int D,m;
    cin >> D >> m;
    HashChain<int> hash(D);
    int ope,x;
    for(int i = 0; i < m; ++i)
    {
       cin >> ope >> x;
       switch(ope)
       {
          case 0 :
            hash.insert(x);
            break;
          case 1 :
            if(hash.find(x) == -1) cout << "Not Found" << endl;
            else cout << hash.find(x) <<endl;
            break;
          case 2 :
            hash.erase(x);
            break;
       }
    }
    return 0;
}