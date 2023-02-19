#include<iostream>
using std::cin;
using std::cout;
using std::endl;


template<class T>
class minHeap
{//是最小堆
    protected:
      T*  list;                   //list[0]位置空出
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
      T    top()  const                //返回堆顶元素
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
    list[1].~T();
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


struct huffmanNode
{
      int len;       //霍夫曼编码的长度
      int fre;       //出现的频率
      char c;   
      huffmanNode *left,*right;

      huffmanNode(int frequency = 0, char cc = '*')
      {
        fre = frequency;
        len = 0;
        c = cc;
        left = right = nullptr;
      }
      bool operator <(huffmanNode x)        {return fre < x.fre;}
      bool operator <(const huffmanNode x)const  {return fre < x.fre;}
      /*bool operator >(huffmanNode& x)        {return fre > x.fre;}
      bool operator >(const huffmanNode& x)  {return fre > x.fre;}
      bool operator ==(huffmanNode& x)        {return fre == x.fre;}
      bool operator ==(const huffmanNode& x)  {return fre == x.fre;}*/

};

class huffmanTree:public minHeap<huffmanNode>
{
  public:
    huffmanTree(int ini_cap):minHeap(ini_cap)  {;}
    ~huffmanTree();
    void input();            //完成霍夫曼树的初始化
    void  height()const;     //计算每个结点的高度
    void  res()const;        //得到编码后的总长度
  private:
    void  height(huffmanNode*)const;
    int  res(huffmanNode*)const;
    void postOrder (huffmanNode* t)     
    {
      if(t != nullptr)
      {
        postOrder(t->left);
        postOrder(t->right);
        if(t != list+1)
          delete t;
      }
    }
};
huffmanTree::~huffmanTree()
{
  postOrder(list + 1);
  //delete[] list;
}

void huffmanTree::input()
{
  //输入需要转换的字符串
  char alph[1000000];
  cin >> alph;
  //fre记录做字母出现的频率
  int freq[26] = {0};
  int i = 0;   
  //遍历字符串
  while(alph[i] != '\0')
  {
    //按照ascII码将字母 a-z 映射到数组的 0-25 位置上    'a'=97
    ++freq[ alph[i] - 97 ];
    ++i;
  }
  
  //使用统计好的频率创建节点
  for(i = 0; i < 26; ++i)
  {
    if(freq[i] != 0)
    {//left,right指针都为空，可以直传
      huffmanNode node(freq[i], 'a'+i);
      insert(node);
    }
  }

  //完成构造霍夫曼树需要进行size-1次这样的操作：取两个最小的节点合并后插入树中
  huffmanNode comb;
  int size = list_size;
  for(i = 0; i < size-1; ++i)
  {
    huffmanNode* temp1 =new huffmanNode(top());
    erase();
    huffmanNode* temp2 =new huffmanNode(top());
    erase();
    comb.fre = temp1->fre + temp2->fre;
    comb.left  = temp1;
    comb.right = temp2;
	insert(comb);
  }
}

void huffmanTree::height(huffmanNode *node)const
{
  if(node != nullptr)
  {
	if(node->left != nullptr)
	{
		node->left->len = node->len+1;
		height(node->left);
	}
	if(node->right != nullptr)
	{
		node->right->len = node->len+1;
		height(node->right);
	}
  }
}

void huffmanTree::height()const
{
  height(list+1);
}

int huffmanTree::res(huffmanNode* node)const
{
  if(node == nullptr)
    return 0;

  static int result = 0;
  if(node->c >= 'a' && node->c <= 'z')
  {
    result += node->fre * node->len;
  }
  res(node->left);
  res(node->right);
  return result;
}

void huffmanTree::res()const
{
  cout << res(list+1);
}






int main()
{
  huffmanTree hftree(26);
  hftree.input();
  hftree.height();
  hftree.res();
  return 0;
}