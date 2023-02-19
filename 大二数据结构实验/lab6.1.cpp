#include<iostream>
#include<string.h>
using std::cin;
using std::cout;
using std::endl;

template<class T>
class Stack       //环形数组描述
{
    protected:
      T  *list_;
      int capacity_;
      int top_;    //栈顶位置，在数组中的索引[0 : capacity-1], -1表示空栈
    public:
      Stack(int capa = 20)
      {
        list_ = new T[capa]();
        top_ = -1;
        capacity_ = capa;
      }
      ~Stack()
      {
        delete[] list_;
      }
      
      T&    top();                //返回栈顶的引用
      void push(const T& val);   //压栈
      T    pop();                //出栈
      int  size() const;         //返回当前数组大小      
      bool empty()const;         //数组是否为空          
    protected:
      void DoubleCapacity();     //容量
};

template<class T>
void Stack<T>::DoubleCapacity()
{
    T *newlist = new T[capacity_ * 2];
    int size = this->size();
    for(int i = 0; i < size; ++i)
    {
        newlist[i] = list_[i];
    }
    capacity_ *= 2;
    delete[] list_;
    list_ = newlist;
}

template<class T>
void Stack<T>::push(const T& val)
{
  //是否栈满
  if(top_ == capacity_-1)
    DoubleCapacity();
  
  list_[++top_] = val;
}

template<class T>
T Stack<T>::pop()
{
  //是否为空栈
  if(empty() == true)
  {
    cout << "Empty Stack!" <<endl;
    return -1;
  }

  T result = top();
  list_[top_--].~T();
  return result;
}

template<class T>
T& Stack<T>::top()
{
    return list_[top_];
}

template<class T>
bool Stack<T>::empty()const           //数组是否为空 
{
    if(top_ == -1)   
      return true;
    return false;
}

template<class T>
int Stack<T>::size() const
{
  return top_ + 1;
}

template<class T = double >
class Calculator
{
    private:  
      Stack<T> operand_;     //操作数
      Stack<char> operator_;    //操作符
    public:
      //默认构造
      //析构
      void Compute(const char * str);
    private:
      bool prior(const char& a,const char& b)    //比较a,b运算符的优先级，a比b优先或a与b同级返回true，b比a优先返回false
      {
        bool res = true;
        switch(a)
        {
          case '*' :
          case '/' :
            res = true;
            break;          //乘除的优先级不会更低
          case '+' :
          case '-' :
            switch (b)
            {
              case '+' :
              case '-' :
                  res = true;
                  break;
            
              case '*' :
              case '/' :
                  res = false;
                  break;
              default : 
                  res = true;
                  break;
            }
            break;
          default :
            res = true;
        }
        return res;
      }
      T a_ope_b(const T& a,const char& ope, const T& b)
      {
        T res;
        switch(ope)
        {
            case '+' :
              res = a+b;
              break;
            case '-' :
              res = a-b;
              break;
            case '*' :
              res = a*b;
              break;
            case '/' :
              res = a/b;
              break;
        }
        return res;
      }
};

template<class T>
void Calculator<T>::Compute(const char* str)
{
  int length = strlen(str);  //字符串的长度
  int flag = 0;     //作为标志，偶数时压入操作数，奇数压入操作符

  T res;            //储存一次运算的结果
  T ope1, ope2;     //一次运算的两个操作数
  int i = 0;
  while(i < length)    //遍历字符串
  {
    //1.压栈  todo:压入运算符与压入一个操作数捆绑（没有括号时）
    //每一个 运算符 和 操作数 都由一个字符构成，压栈时 flag % 2 间隔压栈
    //什么时候压栈？
    //1.1 不考虑（）,操作数栈空，操作符栈空，或操作符比操作数多
    while(operator_.empty() || operand_.size() <= operator_.size())
    {
      if(flag % 2 == 0)    
      {
        //str[i]为第一个没有被压栈的元素
        operand_.push(str[i++] - 48);    //按照ASIⅡ码转换字符为数字 '0' = 48
        flag = 1;
        continue;
      }
      else if(flag % 2 == 1)
      {
        operator_.push(str[i++]);
        flag = 0;
        continue;
      }
    }

    //2.运算
    //根据下一个将要填进来的运算符 与 操作符栈顶 的优先级比较确定操作
    
    while(pri  or(operator_.top(), str[i]) == false)    //栈顶运算优先级低，不能运算，继续压栈
    {
      //压入一个操作数和操作符
      operator_.push(str[i++]);
      operand_.push(str[i++] - 48);
    }
    

    while(!operator_.empty() && prior(operator_.top(), str[i]) == true)  //同级 || 栈顶操作符更优先,执行栈顶操作符运算
    {
        res = 0;
        ope2 = operand_.pop();
        ope1 = operand_.pop();    //弹出两个相邻元素,不要搞错顺序
        res = a_ope_b(ope1, operator_.pop(), ope2);
        operand_.push(res);

    } 

  }
    
    cout << operand_.top();
}

int main()
{
    Calculator<double> ical;
    char s[200];
    cin >> s;
    ical.Compute(s);
    return 0;
}


