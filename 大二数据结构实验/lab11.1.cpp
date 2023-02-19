#include<iostream>
#include<sstream>
using namespace std;
ostringstream s;

template<class T>
struct IndexedTreeNode
{
    T element;
    int leftsize;
    IndexedTreeNode<T> *left, *right;
    IndexedTreeNode(const T& ele)
    {
        element = ele;
        leftsize = 0;
        left = right = nullptr;
    }
};


//索引二叉搜索树
//输出执行操作的过程中 依次比较的元素值 的 异或值。
template<class T>
class IndexedBSTree
{
    private:
      IndexedTreeNode<T>* root;
      int size;

      bool exist(T x) const;
      void postOrder(IndexedTreeNode<T> *t)
      {
        if(t != nullptr)
        {
            postOrder(t->left);
            postOrder(t->right);
            delete t;
        }
      }
    public:
      IndexedBSTree();
      ~IndexedBSTree();
      bool empty()const
        {return root == nullptr;}
      T insert(const T& x);              //将x插入到树中
      T find  (const T& x);                //查找x关键字
      T erase (const T& x);               //删除x元素
      T indexedFind (int rank);    //查找名次为rank的元素
      T indexedErase(int rank);   //删除名次为rank的元素
      void Solution();
};

template<class T>
IndexedBSTree<T>::IndexedBSTree()
{
    root = nullptr;
    size = 0;
}
template<class T>
IndexedBSTree<T>::~IndexedBSTree()
{
  postOrder(root);
}
template<class T>
bool IndexedBSTree<T>::exist(T x)const
{
  IndexedTreeNode<T>* curr = root;
  //能找到, 如果存在的话在在while中一定能找到
  while(curr != nullptr)
  {
    //找到就返回
    if(curr->element == x)
      return true;
    //继续向下找时，将父节点的值相加
    if(curr->element < x)
    {//向右查找
      curr = curr->right;
    }
    else
    {
      curr = curr->left;
    }
  }
  //找不到
  return false;
}

template<class T>
T IndexedBSTree<T>::insert(const T& x)
{//需要维护索引
    //每次都插入到叶子节点
    //x已经存在的话
    if(exist(x))    
      return 0;

    
    //x还不存在树中
    //1.
    //将ins指向将要插入的位置（为空节点)，ins_p指向ins的父节点
    //维护leftsize：插入到左子树时将父节点的leftsize++
    T result = 0;    //记录输出的异或值
    IndexedTreeNode<T>* ins = root;         
    IndexedTreeNode<T>* ins_p = nullptr;    
    while(ins != nullptr)
    {
      ins_p = ins;
      result = result ^ ins_p->element;      //记录异或值的和
      
      if(x > ins->element)
      {
        ins = ins->right;
      }
      else
      {
        ins = ins->left;
        ++ins_p->leftsize;
      }
    }

    //2.
    //创建新节点插入,ins_p是新插入节点的父节点
    IndexedTreeNode<T>* newnode = new IndexedTreeNode<T>(x);
    //如果插入根节点
    if(ins_p == nullptr)
    {
      root = newnode;
    }
    else if(x > ins_p->element)
      ins_p->right = newnode;
    else  
      ins_p->left = newnode;
    
    ++size;
    return result;
}


//find输出0不一定是没有找到吧？
template<class T>
T IndexedBSTree<T>::find(const T& x)
{
  //如果不存在的话
  if(!exist(x))
    return 0;
  
  //存在x
  T result = x;         //result只用加到x的父节点
  IndexedTreeNode<T>* curr  = root;
  while(curr != nullptr)
  {
    //找到就返回
    if(curr->element == x)
      break;
    
    //继续向下找时，将父节点的值相加
    if(curr->element < x)
    {//向右查找
      result = result ^ curr->element;
      curr = curr->right;
    }
    else
    {//向左查找.
      result = result ^ curr->element;
      curr = curr->left;
    }
  }
  return result;
}

template<class T>
T IndexedBSTree<T>::erase(const T& x)
{
  //不存在x
  if(!exist(x))
    return 0;
  
  //存在x
  //1.
  //使用curr记录想要删除的节点，pcurr为其父节点
  T result = x;         //result只用加到x的父节点
  IndexedTreeNode<T>* curr = root,         
                    * pcurr = nullptr;    
  //将curr和pcurr指向最终位置
  while(curr != nullptr)
  {
    if(curr->element == x)
    {
      break;
    }
    pcurr = curr;
    //继续向下找时，将父节点的值相加
    if(curr->element < x)
    {//向右查找
      curr = curr->right;
      result = result ^ pcurr->element;
    }
    else    //将要删除pcurr左边的元素，将其leftsize--
    {
      --pcurr->leftsize;
      curr = curr->left;
      result = result ^ pcurr->element;
    }
  }
  //2.
  //接下来有两个子节点时找节点替换到现在的curr
  //当且仅当要删除的节点有两个子节点时选择右子树中最小的替换到删除节点，然后删除右子树中最小节点
  if(curr->left != nullptr && curr->right != nullptr)
  {
    IndexedTreeNode<T>* rmin = curr->right;
    IndexedTreeNode<T>* rmin_p = curr;
    //一直向左寻找就找到最小的
    while(rmin->left != nullptr)
    {
      rmin_p = rmin;
      rmin = rmin->left;
      --rmin_p->leftsize;
    }
    //替换掉删除节点
    curr->element = rmin->element;
    //之后就需要删除掉rmin节点
    curr = rmin;
    pcurr = rmin_p;
  }
  //3.
  //接下来删除掉需要删除的节点,该节点最多只有一个节点
  //newchild为curr删除后，pcurr的新孩子节点
    IndexedTreeNode<T>* newchild = nullptr;
  //如果删除root
  if(curr == root)
  {
    //没有子节点
    if(curr->left == nullptr && curr->right == nullptr)
    {
      root = nullptr;
    }
    //只有左节点
    else if(curr->left != nullptr && curr->right == nullptr)
    {
      root = root->left;
    }
    else
    {//只有右节点
      root = root->right;
    }
    delete curr;
    return result;
  }
  //如果不是root
  else 
  {
    //没有子节点
    if(curr->left == nullptr && curr->right == nullptr)
    {
      newchild = nullptr;
    }
    //只有左节点
    else if(curr->left != nullptr && curr->right == nullptr)
    {
      newchild = curr->left;
    }
    //只有右节点
    else
    {
      newchild = curr->right;
    }
  }
  if(curr == pcurr->left)
    pcurr->left = newchild;
  else 
    pcurr->right = newchild;
  --size;
  return result;
}

template<class T>
T IndexedBSTree<T>::indexedFind(int rank)
{
  T result = root->element;
  IndexedTreeNode<T>* curr = root;         //当前正在被查找的节点
  while(curr != nullptr)
  {
    if(rank == curr->leftsize + 1)
      return result;
    
    if(rank > curr->leftsize + 1)
    {
      rank = rank - (curr->leftsize + 1);
      curr = curr->right;
      result ^= curr->element;
    }
    else
    {
      curr = curr->left;
      result ^= curr->element;
    }
  }
  return 0;
}

template<class T>
T IndexedBSTree<T>::indexedErase(int rank)
{
  //不存在rank索引的元素
  if(rank > size)
    return 0;

  //存在x
  //1.
  //使用curr记录想要删除的节点，pcurr为其父节点
  T result = 0;         //result只用加到x的父节点
  IndexedTreeNode<T>* curr = root,         
                    * pcurr = nullptr;    
  //将curr和pcurr指向最终位置
  while(curr != nullptr)
  {
    if(rank == curr->leftsize + 1)
    {
      result = result ^ curr->element;
      break;
    }
    pcurr = curr;
    //继续向下找时，将父节点的值相加
    if(rank > curr->leftsize + 1)
    {//向右查找
      rank = rank - curr->leftsize - 1;
      curr = curr->right;
      result = result ^ pcurr->element;
    }
    else    //将要删除pcurr左边的元素，将其leftsize--
    {
      --pcurr->leftsize;
      curr = curr->left;
      result = result ^ pcurr->element;
    }
  }
  //2.
  //接下来有两个子节点时找节点替换到现在的curr
  //当且仅当要删除的节点有两个子节点时选择右子树中最小的替换到删除节点，然后删除右子树中最小节点
  if(curr->left != nullptr && curr->right != nullptr)
  {
    IndexedTreeNode<T>* rmin = curr->right;
    IndexedTreeNode<T>* rmin_p = curr;
    //一直向左寻找就找到最小的
    while(rmin->left != nullptr)
    {
      rmin_p = rmin;
      rmin = rmin->left;
      --rmin_p->leftsize;
    }
    //替换掉删除节点
    curr->element = rmin->element;
    //之后就需要删除掉rmin节点
    curr = rmin;
    pcurr = rmin_p;
  }
  //3.
  //接下来删除掉需要删除的节点,该节点最多只有一个节点
  //newchild为curr删除后，pcurr的新孩子节点
    IndexedTreeNode<T>* newchild = nullptr;
  //如果删除root
  if(curr == root)
  {
    //没有子节点
    if(curr->left == nullptr && curr->right == nullptr)
    {
      root = nullptr;
    }
    //只有左节点
    else if(curr->left != nullptr && curr->right == nullptr)
    {
      root = root->left;
    }
    else
    {//只有右节点
      root = root->right;
    }
    delete curr;
    return result;
  }
  //如果不是root
  else 
  {
    //没有子节点
    if(curr->left == nullptr && curr->right == nullptr)
    {
      newchild = nullptr;
    }
    //只有左节点
    else if(curr->left != nullptr && curr->right == nullptr)
    {
      newchild = curr->left;
    }
    //只有右节点
    else
    {
      newchild = curr->right;
    }
  }
  if(curr == pcurr->left)
    pcurr->left = newchild;
  else 
    pcurr->right = newchild;
  --size;
  return result;
}

template<class T>
void IndexedBSTree<T>::Solution()
{
  int num;       //操作的个数
  int ope;       //操作的类型
  T x;           //输入的第二个数b 
  cin >> num;
  for(int i = 0; i < num; ++i)
  {
    cin >> ope >> x;
    switch (ope)
    {
    case 0:
      s << insert(x) << endl;
      break;
    case 1:
      s << find(x) << endl;
      break;
    case 2:
      s << erase(x) << endl;
      break;
    case 3:
      s << indexedFind(x) << endl;
      break;
    case 4:
      s << indexedErase(x) << endl;
      break;
    default:
      break;
    }
  }
  cout << s.str();
}



int main()
{
  IndexedBSTree<int> tree;
  tree.Solution();
  return 0;
}