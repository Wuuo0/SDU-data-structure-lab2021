#include<iostream>
#include<sstream>
#define ini 0
using std::cout;
using std::endl;
using std::cin;

std::stringstream s;
template<class T>
class HashTable
{
    private:
      T  *list_;        //作为桶存元素
      int size_;
      int divisor_;
    public:
      HashTable(int divisor);
      ~HashTable();
      void insert(const T& x);
      int  find(const T& x) const;
      void erase(const T& x);
};

template<class T>
HashTable<T>::HashTable(int divisor)
{
    divisor_ = divisor;
    size_ = 0;
    list_ = new T[divisor];
    for(int i = 0; i < divisor_; ++i)
      list_[i] = ini;
}

template<class T>
HashTable<T>::~HashTable()
{
  delete[] list_;
}

template<class T>
void HashTable<T>::insert(const T& x)
{
  int ins_index = x % divisor_;    //起始桶

  //初始桶不为空，从ins_index+1开始向后线性探查，找到插入位置
  while(list_[ins_index % divisor_] != ini)
  {
    if(list_[ins_index % divisor_] == x)
    {
      cout << "Existed" << std::endl;
      return;
    }
    ++ins_index;
  }

  list_[ins_index % divisor_] = x;
  cout << ins_index % divisor_ << std::endl;       //输出下标
}

template<class T>
int HashTable<T>::find(const T& x) const 
{
  int index = x % divisor_;            //起始查询位置
  for(int i = 0; i < divisor_; ++i)    //从初始位置开始遍历
  {
    if(list_[(index+i) % divisor_] == x)    //查询成功
    {
      return (index+i) % divisor_;
    }

    if(list_[(index+i) % divisor_] == ini)    //查询结束，没有查到
    {
      break;
    }
  }
  //遍历完成没有查到 或者 遇到空桶，查找失败
  return -1;
}

template<class T>
void HashTable<T>::erase(const T& x)    //删除时不影响前面 %divisor相同的元素，移动后面的 %divisor相同的元素
{
  int deleteIndex = find(x);    //记录需要删除的位置
  int move = 0;               //移动的元素数

  if(deleteIndex == -1)          //不存在该元素
  {
    cout << "Not Found" << endl;
    return;
  }

  int moveIndex = deleteIndex + 1;              //记录当前正在移动的元素的位置
  int moveSupposed;                            //记录正在移动元素应该在的位置
  int beginIndex = find(x);

  for(int i = 0; i < divisor_; ++i)
  {
    moveIndex = moveIndex % divisor_;
    deleteIndex = deleteIndex % divisor_;
    //与moveIndex一同更新   
    moveSupposed = list_[moveIndex] % divisor_;
   
    
    if(list_[moveIndex] == ini)    //检索到空节点时结束
      break;

    if(moveIndex > deleteIndex)     //都未循环 或 都已循环
    {
      if(moveIndex > beginIndex)    //都未循环
      {
        if(moveSupposed <= deleteIndex || moveSupposed > moveIndex)    //可以移动
        {
          list_[deleteIndex] = list_[moveIndex];
          deleteIndex = moveIndex;
          ++moveIndex;
          ++move;
          continue;
        }
        else    //不可移动
        {
          ++moveIndex;
          continue;
        }
      }
      else   //都已循环
      {
        if(moveSupposed <= deleteIndex || moveSupposed > moveIndex)
        {
          list_[deleteIndex] = list_[moveIndex];
          deleteIndex = moveIndex;
          ++moveIndex;
          ++move;
          continue;
        }
        else    //不可移动
        {
          ++moveIndex;
          continue;
        }
      }
    }
    else   //move已循环，erase未循环
    {
      if(moveSupposed > moveIndex && moveSupposed <= deleteIndex)
      {
          list_[deleteIndex] = list_[moveIndex];
          deleteIndex = moveIndex;
          ++moveIndex;
          ++move;
          continue;
        }
        else    //不可移动
        {
          ++moveIndex;
          continue;
        }
    }
  }
  list_[deleteIndex] = ini;
  cout << move <<endl;
}

int main()
{
    int D,m;
    cin >> D >> m;
    HashTable<int> hash(D);
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
            cout << hash.find(x) <<endl;
            break;
          case 2 :
            hash.erase(x);
            break;
       }
    }
    //cout << s.str();
    return 0;
}
