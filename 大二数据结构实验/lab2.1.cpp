#include<iostream>
using namespace std;

template<class T>
class  MySort{
 private:
    T    *list_;    //储存数组
    int  size_;     //数组当前大小

 public:
    MySort(int size);   //创建一个size大小的数组并为其一一赋值
    //void SetValue();    //重新为数组填写数据
    void RankSort();
    void SelectionSort();
    void BubbleSort();
    void InsertSort();

    void output();
    ~MySort();
 private:
    void swap(T& ,T&);
};

template<class T>
MySort<T>::MySort(int size)    // 创建数组并赋值
{
    size_ = size;
    list_ = new T[size_]();
    for(int i = 0; i < size_; i++)
    {
        cin>>list_[i];
    }
}

template<class T>
MySort<T>::~MySort()         // 删除申请的空间
{
   delete[] list_;
}

template<class T>
void MySort<T>::output()        // 将所有元素按顺序输出
{
    for(int i = 0; i < size_; i++)
      cout<<list_[i]<<" ";
}

template<class T>
void MySort<T>::swap(T &a, T& b)
{
    T mid;
    mid = a;
    a = b;
    b = mid;
}

template<class T>
void MySort<T>::RankSort()
{
      // 完成排名计算
    int *rank = new T[size_](); 
    for(int i = 1; i < size_; i++)
    {
        //任意两数之间都进行一次比较
        for(int j = 0; j < i; j++)
        {
            if(list_[j] <= list_[i])
                rank[i]++;
            else 
                rank[j]++;
        }    //排名由[0 : n-1]，数越大 排名数越大
    }        


    for(int i = 0; i < size_; i++)
    {
        while(rank[i] != i)  //直到将第i个数排好
        {
            //已知第i个数应该在rank[i]的位置
            int thisrank = rank[i];  
            swap(list_[i], list_[thisrank]);
            swap(rank[i], rank[thisrank]);
        }    //结果为rank正确的数一直不会改变
    }
    delete rank;
}

template<class T>
void MySort<T>::SelectionSort()
{
    bool isok = true;  //isok == 1 时，此数不用排列
    for(int i = size_ - 1; i >= 1; i--)
    {
        int indexOfmax = i;

        //找出[0 : i]中最大的index作为 indexOfmax
        for(int j = 0; j < i; j++)
        {
            if(list_[j] > list_[indexOfmax])
            {
                indexOfmax = j;
                isok = false;    //indexOfmax 改变说明 i不是最大的，数组需要排列
            }
        }

        if(isok == true)
          continue;    //isok == 1 ,则 list_[i]就是当前最大的无需交换
        else{
           swap(list_[i], list_[indexOfmax]);
           isok = true;
        }
    }
}

template<class T>
void MySort<T>::BubbleSort()
{
    //sorted 记录是否已经按顺序排序
    bool sorted = false;
    for(int i = size_; i > 1 && !sorted; i--)
    {
        sorted = true;    //默认假设已经有序
        for(int j = 0; j < i-1; j++)
        {
            if(list_[j] > list_[j+1])    // 条件满足 表示还是无序
                {
                    swap(list_[j], list_[j+1]);
                    sorted = false;      // 改变sorted状态
                }
        }
    }
}

template<class T>
void MySort<T>::InsertSort()
{
    for(int i = 1; i < size_; i++)  //将list_[i]插入到list_[0 : i-1]中
    {
        int insert_index = 0;     // 记录将要插入的位置，默认比序列中所有数都小
        T insert_num = list_[i];
        for(int j = i-1; j >= 0; j--)    // 从最大的数开始比较
        {
            if(insert_num < list_[j])
            {
                list_[j+1] = list_[j];   // 若insert_num更小，将数列中的数向后移
            }
            else
            {
                insert_index = j+1;      // 若insert_num更大，则插入在它的后面，此次插入完成，记录下index
                break;
            }
        }
        list_[insert_index] = insert_num;  
    }
}


int main()
{
    int size;
    cin>>size;

    MySort<int> sort1(size);
    sort1.InsertSort();
    sort1.output();
    
    return 0;
}