#include<iostream>
using namespace std;

//代表一条边
struct Edge {
	int i, j, w;
	Edge(const int& i = 0, const int& j = 0, const int& w = 0) {
		this->i = i;
		this->j = j;
		this->w = w;
	}
	bool operator<(const Edge& e)const
	{ //重载<
		return w < e.w;
	}
	bool operator<=(const Edge& e)const
	{ //重载<=
		return w <= e.w;
	}
};

//最小堆
template<class T>
class minHeap
{
public:
	//构造函数
	minHeap(int inital = 10) {
		arrayLength = inital;
		heapSize = 0;
		heap = new T[arrayLength];
	}
	//数组加倍函数
	void changeLength();
	//插入函数
	void push(const T& theElement);
	//删除函数
	void pop();
	//返回堆顶元素
	T top() { return heap[1]; }
	//判断是否为空
	bool empty() { return heapSize == 0; }
private:
	T* heap; //存放堆的数组
	int arrayLength; //数组容量
	int heapSize; //堆元素个数
};

//数组加倍函数
template<class T>
void minHeap<T>::changeLength()
{
	T* temp = new T[arrayLength * 2];
	for (int i = 1; i <= heapSize; i++)
		temp[i] = heap[i];
	delete[] heap;
	heap = temp;
	arrayLength *= 2;
}

//插入函数
template<class T>
void minHeap<T>::push(const T& theElement)
{
	// 判断需不需要将数组长度加倍
	if (heapSize == arrayLength - 1)
		changeLength();

	//为插入的元素寻找位置
	int currentNode = ++heapSize;
	while (currentNode != 1 && theElement < heap[currentNode / 2])
	{
		//从子节点向根节点移动
		heap[currentNode] = heap[currentNode / 2];
		currentNode /= 2;
	}
	heap[currentNode] = theElement;
}

//删除函数
template<class T>
void minHeap<T>::pop()
{
	//如果堆为空则异常
	if (heapSize == 0)
		return;

	heap[1].~T(); //删除最小元素
	T lastElement = heap[heapSize--]; //记录最后一个元素，并删除最后一个位置

	int currentNode = 1; //从根节点开始遍历
	int child = 2; //currentNode的孩子
	while (child <= heapSize) //判断是否到叶节点
	{
		if (child < heapSize/*有右孩子*/ && heap[child + 1] < heap[child]) //child应该是左右孩子中最小的一个
			child++;

		if (lastElement <= heap[child]) //判断lastElement是否可以放在currentNode
			break;

		//不可以
		heap[currentNode] = heap[child];
		currentNode = child;
		child *= 2;
	}

	heap[currentNode] = lastElement;
}

//链表节点，新增加了weight
template<class T>
struct chainNode
{
	//数据成员
	T element;                 //相邻节点的序号
	int weight;                //边的权重
	chainNode<T>* next;

	//方法
	chainNode() {}
	chainNode(const T& element, const int& weight)
	{
		this->element = element;
		this->weight = weight;
		this->next = nullptr;
	}
	chainNode(const T& element, const int& weight, chainNode<T>* next)
	{
		this->element = element;
		this->weight = weight;
		this->next = next;
	}
};

//链表类
template<class T>
class Chain
{
friend class linkedGraph;
protected:
	chainNode<T>* firstNode;
	int listSize;
public:
	//构造函数
	Chain() {
		firstNode = nullptr;
		listSize = 0;
	}
	//复制构造函数
	Chain(const Chain<T>& theList);
	//析构函数
	~Chain();
	//插入操作
	void insert(const T& theElement, const int& theWeight);
	//删除操作
	int erase(const T& theElement);
	//查询操作
	int search(const T& theElement);
  //返回链表大小
  int size()const
  {
    return listSize;
  }
};

//复制构造函数
template<class T>
Chain<T>::Chain(const Chain<T>& theList)
{
	listSize = theList.listSize;

	if (listSize == 0)//如果链表长度为空
	{
		firstNode = nullptr;
		return;
	}

	chainNode<T>* sourceNode = theList.firstNode;
	firstNode = new chainNode<T>(sourceNode->element, sourceNode->weight);
	sourceNode = sourceNode->next;
	chainNode<T>* targtNode = firstNode;

	while (sourceNode != nullptr)
	{
		targtNode->next = new chainNode<T>(sourceNode->element, sourceNode->weight);
		targtNode = targtNode->next;
		sourceNode = sourceNode->next;
	}

	targtNode->next = nullptr;
}

//析构函数
template<class T>
Chain<T>::~Chain()
{
	while (firstNode != nullptr)
	{
		chainNode<T>* nextNode = firstNode->next;
		delete firstNode;
		firstNode = nextNode;
	}
}

//插入
template<class T>
void Chain<T>::insert(const T& theElement, const int& theWeight)
{
	if (listSize == 0)//如果插入在链表的首元素位置
		firstNode = new chainNode<T>(theElement, theWeight, firstNode);
	else
	{
		chainNode<T>* p = firstNode;
		p->next = new chainNode<T>(theElement, theWeight, p->next);
	}
	listSize++;
}

//删除
template<class T>
int Chain<T>::erase(const T& theElement)
{
	//查找到目标元素所在的位置
	chainNode<T>* p = firstNode;
	int location = 0;
	while (1)
	{
		if (p->element == theElement)
			break;
		location++;
		p = p->next;
		if (p == nullptr)
			return -1;
	}

	chainNode<T>* deleteNode;
	if (location == 0)//删除首节点
	{
		deleteNode = firstNode;
		firstNode = firstNode->next;
	}
	else
	{
		chainNode<T>* p = firstNode;
		//p指向要删除节点的前驱节点
		for (int i = 0; i < location - 1; i++)
			p = p->next;

		deleteNode = p->next;
		p->next = p->next->next;
	}
	listSize--;
	delete deleteNode;
	return 0;
}

//查询操作
template<class T>
int Chain<T>::search(const T& theElement)
{
	chainNode<T>* p = firstNode;
	int location = 0;
	while (p != nullptr)
	{
		if (p->element == theElement)
			return location;
		p = p->next;
		location++;
	}
	return -1;
}

//链表描述无向图
class linkedGraph
{
private:
	int numOfv; //顶点个数
	int numOfe; //边个数
	Chain<int>* aList; //邻接链表
public:
	linkedGraph() {;}
	~linkedGraph() 
	{
		for(int i = 0; i <= numOfv; ++i)
			aList[i].~Chain();
		delete[] aList;
	} //析构函数
	void insertEdge(int i, int j, int w);      //插入边
	void eraseEdge(int i, int j);              //删除边
	void input();
  void prim();                                //Prim算法

};

//插入边
void linkedGraph::insertEdge(int i, int j, int w)
{
	aList[i].insert(j, w);
	aList[j].insert(i, w);
	//numOfe++;
}

//删除边
void linkedGraph::eraseEdge(int i, int j)
{
	if (aList[i].search(j) == -1 || i == j)
		return;
	aList[i].erase(j);
	aList[j].erase(i);
	//numOfe--;
}

void linkedGraph::input()
{
    cin >> numOfv >> numOfe;
    aList = new Chain<int>[numOfv+1];
    int u,v,weight;
    //插入吧边
    for(int i = 0; i < numOfe; ++i)
    {
      cin >> u >> v >> weight;
      insertEdge(u,v,weight); 
    }
}

void linkedGraph::prim()
{
  long long int result = 0;        //记录结果
  bool* selected = new bool[numOfv+1];      //selected记录该点是否已经被选择中了
  for(int i = 1; i <= numOfv; ++i)
    selected[i] = false;
  selected[1] = true;                        //加入1
  int times = 1;    //记录已经加入的点的数量
  minHeap<Edge> edges;
  //初始时取出1可以到达的所有边
  chainNode<int>* temp = aList[1].firstNode;
  while(temp != nullptr)
  {
    edges.push(Edge(1,temp->element,temp->weight));
    temp = temp->next;
  }
  //之后每次从edges中取出一条边进行判断
  while(times < numOfv)
  {
    Edge e = edges.top();
		edges.pop();
    //如果该点第一次到达，标记他
    if(selected[e.j] == false)
    {
      selected[e.j] = true;
      result += e.w;
      ++times;
      //把加入该点后新可以到达的边加入
      chainNode<int>* temp = aList[e.j].firstNode;
      while(temp != nullptr)
      {
        //两个已经标记的点之间的线段不再加入
        if(selected[temp->element] == false)
          edges.push(Edge(e.j,temp->element,temp->weight));
        temp = temp->next;
      }
    }
    
  }
  delete[] selected;
  cout << result;
}


int main()
{
  linkedGraph gra;
  gra.input();
  gra.prim();
  return 0;
}