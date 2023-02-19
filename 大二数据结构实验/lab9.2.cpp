#include<iostream>
using namespace std;

template<class T>
class ArrayList{
 private:
   T   *list_ ;                 //储存元素的一维数组
   int array_capacity_ ;           //数组的容量
   int list_size_ ;              //数组当前的元素个数
   
   void doubleCapacity()
   {
    array_capacity_ *= 2;
    T* newlist = new T[array_capacity_];
    for(int i = 0; i < list_size_; ++i)
      newlist[i] = list_[i];
    delete []list_;
    list_ = newlist;
   }
 public:
   ArrayList(int initialCapacity = 100)
   {
    array_capacity_ = initialCapacity;
    list_ = new T[array_capacity_];
    list_size_ = 0;
   }
   //~ArrayList()     {delete[] list_;} 
   void erase()   {delete []list_;}
   //Abstract Data Type
   bool Empty() const {return list_size_ == 0;}    //数组是否为空
   int  Size() const {return list_size_;}          //返回数组当前大小
   void Push_back(const T& x)
   {
      if(list_size_ == array_capacity_)
        doubleCapacity();
      list_[list_size_++] = x; 
   }
   T& operator [](int index)
     {return list_[index];}
};


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
class BinaryTree
{
    private:
      BinaryTreeNode<T> *root;
      static void erase(BinaryTreeNode<T>* t) 
        {delete t;}
      static void output(BinaryTreeNode<T> *t)
        { std::cout << t->element_ << ' '; }
      static void  (*visit) (BinaryTreeNode<T>*);
    public:
      BinaryTree()
        {
            root = nullptr;
            visit = nullptr;
        }
      ~BinaryTree()
      {
        visit = erase;
        postOrder(erase);
      }
    void postOrder(BinaryTreeNode<T> *t)
    {
        if(t != nullptr)
        {
            postOrder(t->left_child_);
            postOrder(t->right_child_);
            BinaryTree<T>::visit(t);
        }
    }
    void postOrder(void (*theVisit)(BinaryTreeNode<T>*))
    {
        visit = theVisit;
        postOrder(root);
    }
    BinaryTreeNode<T>* construct(ArrayList<T> pre, ArrayList<T> in,int n);
    void to_post();
};
template<class E>
void (*BinaryTree<E>::visit)(BinaryTreeNode<E>*) = nullptr;

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::construct(ArrayList<T> pre, ArrayList<T> in,int n)
{//由前序，中序遍历结果pre，in构建起一个二叉树
 //应保持pre，in的元素数目相同；
 //前序遍历的每一个节点将中序排列中所有节点分为 左子树 根 右子树 三部分
 //递归调用函数将所有节点联系起来

  //基础部分
  if(in.Empty())
    return nullptr;
  
  //递归部分
  int pre_index = 0;    //记录进行到了哪个位置
  BinaryTreeNode<T> *croot = new BinaryTreeNode<T>(pre[0]);
  //mid为pre[pre_index]在in中的索引，将左右子树分开
  int mid = 0;
  for(mid = 0; in[mid] != pre[0]; ++mid)  {;}

  ArrayList<T> leftIn,rightIn,leftPre,rightPre;
  for(int i = 0; i < mid; ++i)
  {
    leftIn.Push_back(in[i]);
    leftPre.Push_back(pre[i+1]);
  }
  for(int i = mid+1; i < in.Size(); ++i)
  {
    rightIn.Push_back(in[i]);
    rightPre.Push_back(pre[i]);
  }
  croot->left_child_ = construct(leftPre,leftIn,n);
  croot->right_child_ = construct(rightPre,rightIn,n);

  return croot; 
}

template<class T>
void BinaryTree<T>::to_post()
{
    int n;
    cin >> n;
    
    ArrayList<T> pre,in;
    int x;
    for(int i = 0; i < n; ++i)
    {
        cin >> x;
        pre.Push_back(x);
    }
    for(int i = 0; i < n; ++i)
    {
        cin >> x;
        in.Push_back(x);
    }
    root = construct(pre,in,n);
    postOrder(output);
}

int main()
{
    BinaryTree<int> tree;
    tree.to_post();
}