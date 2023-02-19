#include<iostream>
using namespace std;

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


int main()
{
    int n = 0;
    int mid;
    cin >> n;
    ArrayQueue<int> que(n+1);
    for(int i = 1; i <= n; ++i)
    {
        que.push(i);
    }
    while(que.size() >= 2)
    {
        que.pop();
        mid = que.front();
        que.pop();
        que.push(mid);
    }

    while(!que.empty())    //输出剩下的牌
    {
      mid = que.front();
      que.pop();
      cout << mid << ' ';
    }

    return 0;
}