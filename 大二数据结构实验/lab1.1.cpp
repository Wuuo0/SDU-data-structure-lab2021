#include<iostream>
using namespace std;

int GetValue(int* list,bool* subset,int n)
{
    //value记录总价值
    int value = 0;
    //index代表子集中存在元素的下标 
    int index = 1;
    for(int i = 0; i < n; i++)
    {
        if(subset[i] == true)   
        {
            value += index * list[i];
            index++;
        }
    }
    return value;
}
  
void GetAllSubset(int *list,bool* subset,int start,int end,int& result)
//   生成list所有子集，按原顺序将某一元素是否存在于某子集中用0/1记录在subset中，
//   生成范围list[start:end-1],所有value运算结果存在result中
{
    if(start == end)
    //start == end 时已完成子集的0/1排列
    {
        result = result ^ GetValue(list,subset,end);
    }
    if(start < end)
    {
        //递归得到0/1序列代表子集中元素是否存在
        //将subset中每个位置分别用0 1 填充，start++确定下一个元素
        subset[start] = 0;
        GetAllSubset(list,subset,start+1,end,result);
        subset[start] = 1;
        GetAllSubset(list,subset,start+1,end,result);
    }
}

int main()
{
    int n;
    cin>>n;

    int *list = new int[n];
    for(int i = 0; i < n; i++)
    {
        cin>>list[i];
    }
    bool *subset = new bool[n]();
    int result = 0;

    GetAllSubset(list,subset,0,n,result);
    cout<<result;

    delete[] list;    
    delete[] subset;
    return 0;
}