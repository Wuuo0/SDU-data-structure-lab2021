#include<iostream>
using std::cin;
using std::cout;
using std::endl;

template<class T>
struct BinaryTreeNode
{
    T element_;
    BinaryTreeNode<T> *left_child_,   //左子树
                      *right_child_;  //右子树

    BinaryTreeNode()
    {
        left_child_ = right_child_ = nullptr;
    }
    BinaryTreeNode(const T& element)
    {
        element_ = element;
        left_child_ = right_child_ = nullptr;
    }
    BinaryTreeNode(const T& element, BinaryTreeNode<T> *leftchild, BinaryTreeNode<T> *rightchild)
    {
        element_(element);
        left_child_  =  leftchild;
        right_child_ = rightchild;
    }
};

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
    bool empty() const  //队列为空时返回true
    {
    if(queue_front_ == queue_back_)
      return true;
      return false;
    }
    int size() const   //返回队列当前大小
    {
      return (queue_back_ - queue_front_ + capacity_) % capacity_;
    }
    T& front()          //返回队头元素的引用
    {
      if(empty())
      {
        cout <<" Empty Queue ! "<<endl;
        throw ('-1');
     }
     return array_[(queue_front_ + 1)%capacity_];
    }
    void pop()          //删除队头元素
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
    void push(const T& theElement)   //将元素加入队尾
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
    
    ArrayQueue(int initialCapa = 10)
    {
      capacity_ = initialCapa;
      array_ = new T[capacity_];
      queue_front_ = 0;
      queue_back_ = 0; 
    }
    virtual ~ArrayQueue()  
      {delete[] array_;}
};


template<class T>
struct ChainNode
{
  T element_;
  ChainNode<T> *next_;
  ChainNode(const T& element) 
  {
    element_ = element;
    next_ = nullptr;
  }
};

template<class T>
class priorChain
{
  protected:
    ChainNode<T>* head_;        //头指针

  public:
  //ADT
    T&  front()          //返回头节点
    {
      return head_->element_;
    }
    void pop()                     //删除队头元素,不析构
    {
      head_ = head_->next_;
    }
    void push(const T& theElement)   //将元素加入链表中
    {
      //空表
      ChainNode<T>* ins = new ChainNode<T>(theElement);
      if(head_ == nullptr)
      {
        head_ = ins;
        return;
      }
      //插入为头指针
      if(head_->element_->element_ > theElement->element_)
      {
        ins->next_ = head_;
        head_ = ins;
      }
      else
      {
      //不是头指针
      ChainNode<T>* ins_front = head_;    //为ins插入位置的前节点
      while(ins_front->element_->element_ < theElement->element_)
      {
        if(ins_front->next_ == nullptr)
          break;
        if(ins_front->next_->element_->element_ > theElement->element_)
          break;
      ins_front = ins_front->next_;
      }
      ins->next_ = ins_front->next_;
      ins_front->next_ = ins;
    }
  }
    bool empty()const  {return head_ == nullptr;}
    priorChain()   {head_ = nullptr;}
    virtual ~priorChain()
    {
      ChainNode<T> *temp = nullptr;
      while(head_ != nullptr)
      {  
        temp = head_;
        head_ = head_->next_;
        delete temp;
      }
    }
};


template<class E>
class LinkedBinaryTree
{
  protected:
    BinaryTreeNode<E> *root;    //根节点

    static void  (*visit) (BinaryTreeNode<E>*);    //函数指针,对节点指针的访问方式visit
    static void output(BinaryTreeNode<E> *t)
        { std::cout << t->element_ << ' '; }
    static void dispose(BinaryTreeNode<E>* t)    
        {delete t;}  
    
    static void postOrder (BinaryTreeNode<E>* t)     
    {
      if(t != nullptr)
      {
        postOrder(t->left_child_);
        postOrder(t->right_child_);
        LinkedBinaryTree<E>::visit(t);
      }
    }
    static void levelOrder (BinaryTreeNode<E>* t)     //
    {
      ArrayQueue<BinaryTreeNode<E>*> q(size(t));
      BinaryTreeNode<E>* temp = t;
      while(temp != nullptr)
      {
        visit(temp);  //访问t

      //将t的child插入队列中
      if(temp->left_child_ != nullptr)
        q.push(temp->left_child_);
      if(temp->right_child_ != nullptr)
        q.push(temp->right_child_);
    
      //提取下一个要访问的点
      if(!q.empty())
      {
        temp = q.front();
        q.pop();
      }  
      else return;
      }
    }
    static void Order     (BinaryTreeNode<E>* t);

    static int  height   (BinaryTreeNode<E>* t);         //以t为根的子树的高度
    static void outHeight(BinaryTreeNode<E>* t);     //输出以t为根的子树高度
    static int  size     (BinaryTreeNode<E>* t);               //返回以t为根的子树元素数量
    static void outsize  (BinaryTreeNode<E>* t);           //输出以t为根的子树元素数量
  public:
    LinkedBinaryTree()  
        {root = nullptr;}
    LinkedBinaryTree(BinaryTreeNode<E>* t)           //将t节点作为树的根
        {root = t;}
    ~LinkedBinaryTree() 
        {LinkedBinaryTree<E>::erese();}
    
    //清空树
    void erese()
    {
      postOrder(dispose);    //一定要是后序遍历
      root = nullptr; 
    }
    //是否为空树
    bool empty() const  {return root == nullptr;}
    
    //遍历方式
    void postOrder(void(*theVisit)(BinaryTreeNode<E>*))      //
        {visit = theVisit; postOrder(root);} 
    void levelOrder(void(*theVisit)(BinaryTreeNode<E>*))      //需要一个队列
        {visit = theVisit; levelOrder(root);} 
    void Order(void(*theVisit)(BinaryTreeNode<E>*))    //顺序遍历
        { visit = theVisit; Order(root);}
    //输出

    void postOrderOutput()
        { postOrder(output);  cout << endl;}
    void levelOrderOutput()
        { levelOrder(output); cout << endl;}
    
     void input(int n);                                            //输入一个有n个节点的二叉树
     void levelSize();
     void levelHeight();
     void topost();
     BinaryTreeNode<E>* construct(E* pre,E* in,int n,int pre_start,int pre_end);             //由前序和中序遍历的结果pre，in构建起一棵树，返回根节点
};

//static成员:
template<class E>
void (*LinkedBinaryTree<E>::visit)(BinaryTreeNode<E>*) = nullptr;


template<class E>
int LinkedBinaryTree<E>::height(BinaryTreeNode<E>* t)
{
  if(t == nullptr)
    return 0;
  int hl = height(t->left_child_);
  int hr = height(t->right_child_);
  if(hl > hr)
    return  ++hl;
  else
    return  ++hr;
}


template<class E>
void LinkedBinaryTree<E>::Order(BinaryTreeNode<E>* t)
{//层次遍历以t为根节点的树
  priorChain<BinaryTreeNode<E>*> q;
  BinaryTreeNode<E>* temp = t;
  while(temp != nullptr)
  {
    visit(temp);  //访问t

    //将t的child插入队列中
    if(temp->left_child_ != nullptr)
      q.push(temp->left_child_);
    if(temp->right_child_ != nullptr)
      q.push(temp->right_child_);
    
    //提取下一个要访问的点
    if(!q.empty())
    {
      temp = q.front();
      q.pop();
    }  
    else return;
  }
}
template<class E>
void LinkedBinaryTree<E>::outHeight(BinaryTreeNode<E>* t)
{
    std::cout << height(t) << ' ';
}
template<class E>
int LinkedBinaryTree<E>::size(BinaryTreeNode<E>* t)
{
  if(t == nullptr)
    return 0;
  
  int lsize = size(t->left_child_);
  int rsize = size(t->right_child_);
  return lsize + rsize + 1;
}
template<class E>
void LinkedBinaryTree<E>::outsize(BinaryTreeNode<E>* t)
{
    std::cout << size(t) << ' ';
}
//非static成员：


//共有n个有效节点[10,100000]
//根节点值为1,之后n行输入两个数a，b代表左右孩子的节点值，-1代表空节点
//完成一棵树的构建
template<class E>
void LinkedBinaryTree<E>::input(int n)
{
  //创建根节点
  root = new BinaryTreeNode<E>(1);

  //使用队列结构，为每一个节点赋左右孩子节点的值
  priorChain<BinaryTreeNode<E>* > nodeQueue;    //queue弹出节点时不删除节点中内容
  nodeQueue.push(root);
  
  //为每一个节点左右孩子赋值
  E left,right;
  BinaryTreeNode<E> *temp;
  for(int i = 1; i <= n; ++i)
  {
    //取队列中第一个节点
    temp = nodeQueue.front();
    nodeQueue.pop();

    //如果输入非空节点，与父节点连接且入队
    cin >> left >> right;
    if(left != -1)
    {
      temp->left_child_ = new BinaryTreeNode<E>(left);
      nodeQueue.push(temp->left_child_);
    }
    if(right != -1)
    {
      temp->right_child_ = new BinaryTreeNode<E>(right);
      nodeQueue.push(temp->right_child_);
    }
  }
}
template<class E>
void LinkedBinaryTree<E>::levelSize() 
{
  visit = outsize;
  Order(visit);
  cout << endl;
}
template<class E>
void LinkedBinaryTree<E>::levelHeight() 
{
  visit = outHeight;
  Order(visit);
  cout << endl;
}


int main()
{
    LinkedBinaryTree<int> tree;
    int n;
    cin >> n;
    tree.input(n);

    tree.levelOrderOutput();
    tree.levelSize();
    tree.levelHeight();
    
    return 0;
}