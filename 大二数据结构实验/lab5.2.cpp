#include<iostream>
#include<vector>
#include<sstream>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

namespace {
    std::stringstream s;
}

class MatrixTerm
{
    friend class SparseMatrix;
    private:
      int row_;    // [0 : row-1]
      int col_;    // [0 : col-1]
      int val_;
    public:
      MatrixTerm(int row = 0, int col = 0, int val = 0);
      void set(int row, int col, int val);
};
MatrixTerm::MatrixTerm(int row, int col, int val)
{
    row_ = row;
    col_ = col;
    val_ = val;
}
void MatrixTerm::set(int row, int col, int val)
{
    row_ = row;
    col_ = col;
    val_ = val;
}


class SparseMatrix
{
    private:
      int rows_;      //从1开始
      int cols_;
      vector<MatrixTerm> terms_;

    public:
      //重置矩阵为rows行，cols列 并按行输入；
      void Reset(int rows, int cols); 

      void Push_Back(int row, int col, int val);
      void Output();

      void Transpose();

      void add(SparseMatrix& addend);

      void Mutiply(SparseMatrix& B);
      void SetSize(int rows, int cols);    //重置col，row 并且清空数据(只有加法，乘法用得到)
      //

      void test();
    protected:
      void swap(int &a,int &b)  {int temp = a; a = b; b = temp;}
};

void SparseMatrix::test()
{
    //terms_.clear();
    vector<MatrixTerm>::iterator b = terms_.begin(),e = terms_.end();
    cout << 1 ;

}

void SparseMatrix::Reset(int rows, int cols)
{
    rows_ = rows;
    cols_ = cols;
    terms_.clear();

    int val = 0;
    for(int i = 0; i < rows_*cols_; ++i)
    {
      cin >> val;
      if(val != 0)   //输入非0元素时，加入terms中
      {
        terms_.push_back(MatrixTerm(i/cols_, i%cols_, val));      //每一行的索引从0开始
      }
    }
}

void SparseMatrix::SetSize(int rows,int cols)
{
  rows_ = rows;
  cols_ = cols;
  terms_.clear();
}

void SparseMatrix::Push_Back(int row, int col, int val)
{
  terms_.push_back(MatrixTerm(row,col,val));
}

void SparseMatrix::Output() 
{
    s << rows_ << " " << cols_ << endl;  //先输出行列
    vector<MatrixTerm>::iterator ite = terms_.begin();
    
    int nowindex = rows_*cols_;              //nowindex作为terms中的非0元素 投射到一维数组中的索引[0 : rows*cols-1]
    
    for(int i = 0; i < rows_*cols_; ++i)
    {
      /*if(i != 0 && i%cols_ == 0)
      {
        s << endl;
      }*/

      if(ite != terms_.end())
        nowindex = ite->row_ * cols_ + ite->col_;

      if(i == nowindex)
      {
        s << ite->val_ << " ";
        ++ite;
      }
      else 
        s << 0 << " ";

    if((i+1)%cols_ == 0)
      s << endl;
    }
    //s << endl ;
    
}

void SparseMatrix::Transpose()
{
  vector<MatrixTerm> temp_terms;      //临时保存逆转后的terms
  temp_terms.resize(terms_.size());
  int col_size[cols_] = {0};         //保存逆置前每一列 也就是 逆置后每一行 的元素个数 colsize[0 : cols-1]
  int row_next[cols_] = {0};        //保存逆置后每一行将要填充的元素在temp_terms中的索引，
                                    //给一个元素的ite，则它应该被插入在temp_terms中的row_next[ite->col_]此索引中
  for(vector<MatrixTerm>::iterator ite = terms_.begin(); ite != terms_.end(); ++ite)
  {
     ++col_size[ite->col_];   //遍历一遍，将每一元素所在列得到col_size+1
  }
  
  for(int i = 1; i < cols_; ++i)    //第一行自然从0开始
  {
     row_next[i] = row_next[i-1] + col_size[i-1];    //上一行开始的位置 + 上一行的大小 = 下一行开始的位置
  }
  
  MatrixTerm temp;
  for(vector<MatrixTerm>::iterator ite = terms_.begin(); ite != terms_.end(); ++ite)
  {
    int pos = row_next[ite->col_];    //记录插入的索引
    ++row_next[ite->col_];            //如果有对该行的下一个插入，插入到一位

    temp.set(ite->col_, ite->row_, ite->val_);    //将terms中的行列逆转
    temp_terms[pos] = temp;
  }    //完成temp_terms的赋值
  
  swap(cols_, rows_);         //  
  terms_.swap(temp_terms);    //将自己逆置
}

void SparseMatrix::add(SparseMatrix& addend)    //*this += addend
{
  //判断相加是否合法
  if(cols_ != addend.cols_ || rows_ != addend.rows_)
  {
    //若不合法，将 addend 赋给 *this
    rows_ = addend.rows_;
    cols_ = addend.cols_;
    //terms_.swap(addend.terms_);
    terms_.clear();
    terms_.resize(addend.terms_.size());
    std::copy(addend.terms_.begin(),addend.terms_.end(),terms_.begin());
    
    s << -1 << endl;
    return;
  }
  
  vector<MatrixTerm> result;   //储存结果

  vector<MatrixTerm>::iterator this_ite = terms_.begin(), add_ite = addend.terms_.begin(),mid;  //mid指向this_ite之前
  int this_index = 0, add_index = 0;      //分别对应两个矩阵中规则矩阵的索引
  while(this_ite != terms_.end() && add_ite != addend.terms_.end())
  {
    this_index = this_ite->row_ * cols_ + this_ite->col_;
    add_index  = add_ite->row_ * cols_+  add_ite->col_;

    if(this_index == add_index)    //相等的话。两数相加
    {
      result.push_back(MatrixTerm(this_ite->row_, this_ite->col_,this_ite->val_ + add_ite->val_));
      ++this_ite;
      ++add_ite;
    }

    else if(this_index > add_index)    //如果加数的索引在前，将其插入
    {
      result.push_back(MatrixTerm(add_ite->row_, add_ite->col_, add_ite->val_));
      ++add_ite;  
    }

    else if(this_index < add_index)    //如果加数索引更小，不插入
    {
      result.push_back(MatrixTerm(this_ite->row_, this_ite->col_,this_ite->val_));
      ++this_ite;
    }
  }

  //将另一个没有到end（）的terms中剩下的元素push_back
  if(this_ite != terms_.end())
  {
    while(this_ite != terms_.end())
    {
      result.push_back(MatrixTerm(this_ite->row_, this_ite->col_, this_ite->val_));
      ++this_ite;
    }
  }
  else
  {
    while(add_ite != addend.terms_.end())
    {
      result.push_back(MatrixTerm(add_ite->row_, add_ite->col_, add_ite->val_));
      ++add_ite;  
    }
  }

  terms_.swap(result);
}

void SparseMatrix::Mutiply(SparseMatrix& B)  // *this ->  A , A * B = C 
{
  /*
  //判断运算是否合法
  if(cols_ != B.rows_)
    {
      //不合法将 B 赋给 *this
      rows_ = B.rows_;
      cols_ = B.cols_;
      terms_.swap(B.terms_); 
      s << -1 << endl; 
      return;
    }

  //首先将B转置
  B.Transpose();

  //C用来存放结果
  SparseMatrix C;
  C.SetSize(rows_, B.rows_);
  
  //存下A每一行开始元素的迭代器  start_ite[0 : rows_-1]
  vector<MatrixTerm>::iterator start_ite[rows_], temp = terms_.begin();
  for(int i = 0; i < rows_; ++i)
  { 
    start_ite[i] = temp;
    while(temp != terms_.end() && temp->row_ == i)
    { ++temp; }
  }
  
  //计算部分
  vector<MatrixTerm>::iterator A_ite = terms_.begin(), B_ite = B.terms_.begin();
  int c = 0;    //c作为结果的一个值
  for(int i = 0; i < rows_; ++i)
  {
    B_ite = B.terms_.begin();

    for(int j = 0; j < B.rows_; ++j)
    {
      //A_ite指向第i行开始的位置
      A_ite = start_ite[i];

      c = 0;
      //AB_ite在可以运算的范围内时
      while(B_ite!=B.terms_.end() && A_ite!=terms_.end() && A_ite->row_==i && B_ite->row_==j)
      {
        if(A_ite->col_ == B_ite->col_)
        {
          c += A_ite->val_ * B_ite->val_;
          ++A_ite;  
          ++B_ite;
        }
        else if(A_ite->col_ < B_ite->col_)    ++A_ite;
        else if(A_ite->col_ > B_ite->col_)    ++B_ite;
      }
      
      //插入C运算的结果
      if(c != 0)
        C.terms_.push_back(MatrixTerm(i, j, c));
      //分析跳出while的原因
      //1.
      if(B_ite == B.terms_.end())
      {
        break;  //++i
      }
      //2.
      else if(B_ite->row_ != j)
      {
        continue;
      }
      //3.
      else
      {
        while(B_ite->row_ != j && B_ite != B.terms_.end())
        {
          ++B_ite;
        }
      }

    }
  }
  terms_.swap(C.terms_);
  */
}

int main()
{
  SparseMatrix matrix,temp;
  //matrix.test();

  int ope_num, times;
  cin >> times;     //有几次操作

  int reset_row, reset_col;

  int t;                          //加法乘法时temp需要几行数据
  int temp_row, temp_col;         //temp的行数，列数
  int ins_row, ins_col, ins_val;  //要temp中terms的数据

  for(int i = 0; i < times; ++i)
  {
    cin >> ope_num;

    switch(ope_num)
    {
      case 1 :   //重置矩阵
        cin >> reset_row >> reset_col;
        matrix.Reset(reset_row, reset_col);
        break;
      case 2 :    //乘法
        cin >> temp_row >> temp_col >> t;
        temp.SetSize(temp_row, temp_col);
        for(int i = 0; i < t; ++i)
        {
          cin >> ins_row >> ins_col >> ins_val;
          temp.Push_Back(ins_row -1, ins_col -1, ins_val);    //输入的行列序号从1开始，需要减去1
        }
        matrix.Mutiply(temp);
        break;
      case 3 :    //加法
        cin >> temp_row >> temp_col >> t;
        temp.SetSize(temp_row, temp_col);
        for(int i = 0; i < t; ++i)
        {
          cin >> ins_row >> ins_col >> ins_val;
          temp.Push_Back(ins_row -1, ins_col -1, ins_val);    //输入的行列序号从1开始，需要减去1
        }
        matrix.add(temp);
        break;
      case 4 :
        matrix.Output();
        break;
      case 5 :
        matrix.Transpose();
        break;
    }
  }
  cout << s.str();
  return 0;
}