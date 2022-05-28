#include "iostream"
#include "algorithm"
#include "vector"

using namespace std;

struct node {
	int v, height;//v数据，height当前子树的高度
	node* lchild, * rchild;//左子树，右子树
};

node* newNode(int v) {//创建新节点，v是权值
	node* Node = new node;//申请空间
	Node->v = v;//结点权值
	Node->height = 1;//初始高度为1
	Node->lchild = Node->rchild = NULL;//初始无左右子树
	return Node;
}

int getHeight(node* root) {//获取节点高度
	if (root == NULL) {//空结点高度为0
		return 0;
	}
	return root->height;
}

int getBalanceFactor(node* root) {//结点的平衡因子（左右子树高度差）
	return getHeight(root->lchild) - getHeight(root->rchild);//
}

void updateHeight(node* root) {//更新结点高度，左右子树中较大的+1
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
}

void search(node* root, int x) {//查找
	if (root == NULL) {
		cout << "查找失败" << endl;
		return;
	}
	if (x == root->v) {//找到
		cout << root->v << "查找成功"  << endl;
	}
	else if (x < root->v) {//比根节点小，访问左子树
		search(root->lchild, x);
	}
	else {//反之访问右子树
		search(root->rchild, x);
	}
}

void L(node*& root) {//左旋
	node* temp = root->rchild;
	root->rchild = temp->lchild;
	temp->lchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void R(node*& root) {//右旋
	node* temp = root->lchild;
	root->lchild = temp->rchild;
	temp->rchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void insert(node*& root, int v) {//插入
	if (root == NULL) {
		root = newNode(v);//找到插入位置，创建结点
		return;
	}
	if (v < root->v) {//v小于根节点，向左子树查询
		insert(root->lchild, v);//插入
		updateHeight(root);//更新树高
		if (getBalanceFactor(root) == 2) {
			if (getBalanceFactor(root->lchild) == 1) {//LL
				R(root);
			}
			else if (getBalanceFactor(root->lchild) == -1) {//LR
				L(root->lchild);
				R(root);
			}
		}
	}
	else  {//v比根节点大
		insert(root->rchild, v);//插入
		updateHeight(root);//更新树高
		if (getBalanceFactor(root) == -2) {
			if (getBalanceFactor(root->rchild) == -1) {//RR
				L(root);
			}
			else if (getBalanceFactor(root->rchild) == 1) {//RL
				R(root->rchild);
				L(root);
			}
		}
	}
}

int findMin(node*& root) {//找最小的元素
	if (root->lchild == NULL) {
		return root->v;
	}
	else {
		return findMin(root->lchild);
	}
}

node* Delete(node*& root, int v) {//删除指定元素
	if (root == NULL) {
		return root;
	}
	else if (v < root->v) {
		root->lchild = Delete(root->lchild, v);
	}
	else if (v > root->v) {
		root->rchild = Delete(root->rchild, v);
	}
	else {
		if (root->lchild != NULL && root->rchild != NULL) {
			root->v = findMin(root->rchild);//将右子树中最小的值放入该结点，保证满足平衡二叉树性质
			root->rchild = Delete(root->rchild, root->v);
		}
		else {
			root = (root->lchild != NULL) ? root->lchild : root->rchild;
		}
	}
	if (root == NULL) {
		return root;
	}
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
	if (getBalanceFactor(root) > 1) {
		if (getBalanceFactor(root->lchild) >= 0) {//LL
			R(root);
			return root;
		}
		if (getBalanceFactor(root->lchild) < 0)//LR
		{
			L(root->lchild);
			R(root);
			return root;
		}
	}
	if (getBalanceFactor(root) < -1) {
		if (getBalanceFactor(root->rchild) <= 0) {//RR
			L(root);
			return root;
		}
		if (getBalanceFactor(root->rchild) > 0){//RL
			R(root->rchild);
			L(root);
			return root;
		}
	}
	return root;
}

node* create(vector<int> data, int n) {//初始化创建一棵平衡二叉树
	node* root = NULL;
	for (int i = 0; i < n; i++) {
		insert(root, data[i]);
	}
	return root;
}

void out(node*& root) {//输出一个有序数组
	if (root == NULL) {
		return;
	}
	out(root->lchild);
	cout << root->v << " ";
	out(root->rchild);
	return;
}

int main() {
	vector<int> data;//存放元素
	int n;
	cout << "元素个数:";
	cin >> n;//数组大小
	cout << n << "个元素，以空格分格:";
	for (int i = 0; i < n; i++) {//输入值
		int x;
		cin >> x;
		data.push_back(x);
	}
	node* root = create(data, n);//创建平衡二叉树
	cout << endl << "请输入x y（x:1插入;x:2删除;x:3查询,y为值;x:4输出有序数组，输入0 0结束）" << endl;
	int x = 1, y = 1;//x:1查询;x:2插入;x:3查询
	while (x != 0) {
		cin >> x >> y;
		if (x == 1) {
			insert(root, y);
			cout << "插入成功" << endl;
		}
		else if (x == 2) {
			Delete(root, y);
			cout << "删除成功" << endl;
		}
		else if (x == 3) {
			search(root, y);
		}
		else if (x == 4) {
			out(root);
			cout << endl;
		}
	}
	cout << "结束";
	return 0;
}