#include<iostream>
#include<sstream>
#include<string>
using std::cin;
using std::cout;
using std::string;
using std::endl;


//Result类也是线性表 用于储存结果
class Result
{
  private:
    int *data_;
    int capacity_;  
    int size_;
  public:
    Result(int inicapa = 20)    // 默认构造函数
    {
      capacity_ = inicapa;
      data_ = new int[capacity_];
      size_ = 0;
    }

    ~Result()
    {
      delete[] data_;
    }

    void Insert(int val)
    {
      if(size_ == capacity_)       //大小不够时 先扩充
      {
        int *newdata = new int[capacity_*2];
        for(int i = 0; i < size_; i++)
        {
          newdata[i] = data_[i];
        }
        delete[] data_;
        data_ = newdata;
        capacity_ *= 2;
      }
      data_[size_] = val;
      size_++;
    }

    void output()
    {
      for(int i = 0; i < size_; i++)
      {
        cout << data_[i] << endl;
      }
    }
};


namespace {
 std::ostringstream s;     // 所有将要输出的内容被插入到 s 中
 Result res;
};


class A_Number_
{
 private:
   string name_;
   string phone_number_;
   int    class_number_;
   int    domitory_;
 public:
    A_Number_(string name,string phonenum,int classnum, int domitory)
    {
      name_ = name;
      phone_number_ = phonenum;
      class_number_ = classnum;
      domitory_ = domitory;  
    }
    A_Number_()  { ;}

    A_Number_(const A_Number_& from)  //复制构造
    {
      name_ = from.name_;
      phone_number_ = from.phone_number_;
      class_number_ = from.class_number_;
      domitory_ = from.domitory_;
    }
    
    void set(string name,string phonenum,int classnum, int domitory)
    {
      name_ = name;
      phone_number_ = phonenum;
      class_number_ = classnum;
      domitory_ = domitory;  
    }
    string GetName() const   {return name_;}
    int  GetClass() const    {return class_number_;}
    int  GetDomitory() const     {return domitory_;}
    void Update();       
    A_Number_& operator = (const A_Number_& from)   // to = from 
    {
      name_ = from.name_;
      phone_number_ = from.phone_number_;
      class_number_ = from.class_number_;
      domitory_ = from.domitory_;
      return *this;
    }
};

void A_Number_::Update()
{
  int operation_num;      //1 电话 2 班级 3 宿舍
  std::cin >> operation_num;

  string new_phonenum;             //
  int new_class;                   //
  int new_domi;                    // 临时储存更改后的值

  switch(operation_num)             //判断进行的更改操作
  {
    case 1 :
      
      cin >> new_phonenum;
      phone_number_ = new_phonenum;
      break;
    case 2 :
      
      cin >> new_class;
      class_number_ = new_class;
      break;
    case 3 :
       
       std::cin >> new_domi;
       domitory_ = new_domi;
       break;
  }

}


// 0 插入一条记录
// 1 根据姓名删除一条记录
// 2 根据姓名编辑
// 3 根据姓名查找
// 4 输出班级所有成员宿舍号^

class Contacts
{
 private:
   A_Number_  *list_;
   int  capacity_;
   int  now_size_;

 public:
   Contacts(int initial_size = 20);
   ~Contacts()     {delete[] list_;}
  void Update(string name);
  void Delete(string name);
  void Insert(string name, string phonenum, int classnum, int domi);
  void  Exist(string name) const;
  void  GetXorValue(int theClass) const;
 private:
   void DoubleCapa();
   int IndexOf(string name) const;
   
};

Contacts::Contacts(int ini_num)
{
    list_ = new A_Number_[ini_num];
    capacity_ = ini_num;
    now_size_ = 0;
}

void Contacts::DoubleCapa()     //将容量扩充至双倍
{
    capacity_ = capacity_ * 2;
    A_Number_  *newlist = new A_Number_[capacity_];    //需要创建新的数组
    for(int i = 0; i < now_size_; i++)
    {
      newlist[i] = list_[i];
    }
    delete[] list_;
    list_ = newlist;
}

void Contacts::Insert(string name, string phonenum, int classnum, int domi)
{
  //至少需要一个空余位置 达到最大容量后先扩充
  if(now_size_ == capacity_-1)
    DoubleCapa();
  
  list_[now_size_].set(name,phonenum,classnum,domi);
  now_size_++;
}

int Contacts::IndexOf(string name) const  //按照姓名查找其成员所在下标
{
  //采用遍历顺序查找的方法
  for(int i = 0; i != now_size_; i++)
  {
    if(name == list_[i].GetName())
    {
      return i;       // return 0 ~ now_size_-1
    }
  }
  return -1;         //查找失败返回-1
}

void Contacts::Delete(string name)    //按名字 删除
{
  int theIndex = IndexOf(name);
  if(theIndex == -1)
    return;
  
  //将该index后的成员都 向前移动一位，同时覆盖掉index位置的数据
  for(int i = theIndex+1; i != now_size_; i++)
  {
    list_[i-1] = list_[i];
  }
  now_size_--;
}

//姓名 编辑项目 ：1 电话 2 班级 3 宿舍   新值
void Contacts::Update(string name)
{
  int theIndex = IndexOf(name);
  if(theIndex == -1)
    return ;
  
  //调用　的函数实现
  list_[theIndex].Update();
}

void Contacts::Exist(string name) const
{
  // 查找成功返回1 失败返回0
  int theIndex = IndexOf(name);
  
  if(theIndex == -1)
  {
    // s << 0 << endl;
    res.Insert(0);
  }
  else 
  {
    // s << 1 << endl;
    res.Insert(1);
  }
}

void Contacts::GetXorValue(int theClass) const
{
  int result = 0;
  // 遍历查找是否班级相同
  for(int i = 0; i != now_size_; i++)
  {
    if(list_[i].GetClass() == theClass)
    {
      result ^= list_[i].GetDomitory();
    }
  }
  // s << result << endl;
  res.Insert(result);
}



int main()
{
  // 共ope_times 次操作
  int ope_times;
  cin >> ope_times;

  Contacts icontacts;

  string newname;             //
  string newphonenum;         //
  int newclass;               //
  int newdomitory;            //将要插入的数据
  string delete_name;            //将要删除的名字
  string update_name;               //将要更新的名字
  string find_name;                     //将要查找的名字
  int xor_class;                            //将要计算异或值的班级
  
  while(ope_times--)
  {
    int operation;
    cin >> operation;
    switch(operation)
    {
      case 0 :
        cin >> newname >> newphonenum >> newclass >> newdomitory;
        icontacts.Insert(newname, newphonenum, newclass, newdomitory);
        break;
      case 1 :
        cin >> delete_name;
        icontacts.Delete(delete_name);
        break;
      case 2 :
        cin >> update_name;
        icontacts.Update(update_name);
        break;
      case 3 :
        cin >> find_name;
        icontacts.Exist(find_name);
        break;
      case 4 :
        cin >> xor_class;
        icontacts.GetXorValue(xor_class);
        break;
    }
  }
  //将所有结果插入s后输出
  //cout << s.str();
  res.output();
  return 0;
}
