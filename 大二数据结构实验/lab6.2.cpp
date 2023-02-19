#include<iostream>
#include<iomanip>
using namespace std;

template<class T> class Calculator;

template<class T>
class Stack       //环形数组描述
{
  friend class Calculator<T>;
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

template<class T>
class Calculator
{
    public:
      //默认构造
      //析构
      T Compute(const char * str,int& );
    
};

template<class T>
T Calculator<T>::Compute(const char* str,int &i)    //i记录字符串开始位置
{
  Stack<T> opnum;
  Stack<char> ope;
  int flag = 0;        //标记压入操作数 或者 操作符
  T res,a,b;
  char op;
  
  while(str[i] != '\0')
  {
    //压栈
    while((ope.empty() || opnum.size() <= ope.size() || ope.top() == '+' || ope.top() == '-') && str[i] != '\0')
    {
      if(flag % 2 == 0)    //插入数字
      {
        //如果将要遇到左括号
        if(str[i] == '(')
        {
          //将括号中算出结果作为一个操作数压栈
          opnum.push(Compute(str,++i));
        }
        else
        {
          //正常压入数字,按ASCⅡ码对照
          opnum.push(str[i++] - 48);
        }
        //下一个接受运算符
        flag = 1;
      }
      else if(flag % 2 == 1)
      {
        //如果遇到后括号
        if(str[i] == ')')
        {
          ++i;
          //停止压栈操作，运算出结果
          goto a;
        }
        else
        {
          //正常压入运算符
          ope.push(str[i++]);
        }

        flag = 0;
      }
    }

    //进行一次乘除操作
    if(ope.top() == '*')
    {
      ope.pop();              //弹出一个操作符
      b = opnum.pop();        //
      a = opnum.pop();        //弹出两个操作数
      res = a * b;
      opnum.push(res);
    }
    else if(ope.top() == '/')
    {
      ope.pop();              //弹出一个操作符
      b = opnum.pop();        //
      a = opnum.pop();        //弹出两个操作数
      res = a / b;
      opnum.push(res);
    }
  }

  //将栈内剩余加减操作进行到底
  a : while(!ope.empty())
  {
    b = opnum.pop();        //
    a = opnum.pop();        //弹出两个操作数
    op = ope.pop();         //弹出一个操作符


    //若之前一个符号为+，则不需要变号
    if(ope.empty() || ope.top() == '+')
    {
      if(op == '+')
      {
        res = a + b;
      }
      else if(op == '-')
      {
        res = a - b;
      }
    }
    else   //else前面为减号，改变运算顺序要变号
    {
      if(op == '+')
      {
        res = a - b;
      }
      else if(op == '-')
      {
        res = a + b;
      }
    }
    opnum.push(res);
  }
  return opnum.top();
}

int main()
{
    Calculator<double> ical;
    char s[2000]="";
    int n,start = 0;
    cin >> n;
    for(int i = 0; i < n; ++i)
    {
      cin >> s;
      cout << fixed << setprecision(2) << ical.Compute(s,start) <<endl;
      start = 0;
    }
    return 0;
}

//  0-4-1/6*(1-(6/7)-4+6+2+6*1)-1*7+2-8*2+0-(4+6-6*1+(3-8*6/4-6-5)*6/4/8+7-1*4/9*5)-0/6+1-0-2+7-2+6*4-3*6+2/8+6+1*6*2
// 0-4-   10.16