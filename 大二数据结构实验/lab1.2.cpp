#include<iostream>
using std::cin;
using std::cout;


int GetValue(int* list,int n)
{
    //计算输入序列list的价值value
    int value = 0;
    for(int i = 0; i < n; i++)
    {
        value += (i+1) ^ list[i];
    }
    return value;   
}

template<class T>
void swap(T& a,T& b)
{
    //交换两个数
    int mid;
    mid = a;
    a = b;
    b = mid;
}

int  Permutation(int *list,int start,int end)
//生成list[start,end-1]的全排列，计算所有的排列value的 | 运算结果存在result
{
    static int result = 0;
    if(start == end)
    //start == end 时已完成全排列
    {
        //计算完成排列的序列的Value进行|运算
        result = result | GetValue(list,end);
    }
    if(start < end)  
    //递归的生成[start:end-1]的排序
    {
        for(int i = start; i < end; i++)
        {
        //分别将每个元素作为第一个元素，对剩下元素进行全排列即可递归得到结果
        swap(list[i],list[start]);                      //选择开头元素
        Permutation(list,start+1,end);           //剩下元素全排列
        swap(list[i],list[start]);                      //交换回去保持list不变
        }
     }
    return result;
}

int main()
{
    int n;
    cin>>n;

    int* list = new int[n];
    for(int i = 0; i < n; i++)
    {
        cin>>list[i];
    }
    int result = 0;
    result = Permutation(list,0,n);
    cout<<result;

    delete[] list;    
    return 0;
}