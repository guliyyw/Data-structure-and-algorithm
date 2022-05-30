#include "iostream"
#include "algorithm"
#include "vector"

using namespace std;

struct node {
	int v, height;//v���ݣ�height��ǰ�����ĸ߶�
	node* lchild, * rchild;//��������������
};

node* newNode(int v) {//�����½ڵ㣬v��Ȩֵ
	node* Node = new node;//����ռ�
	Node->v = v;//���Ȩֵ
	Node->height = 1;//��ʼ�߶�Ϊ1
	Node->lchild = Node->rchild = NULL;//��ʼ����������
	return Node;
}

int getHeight(node* root) {//��ȡ�ڵ�߶�
	if (root == NULL) {//�ս��߶�Ϊ0
		return 0;
	}
	return root->height;
}

int getBalanceFactor(node* root) {//����ƽ�����ӣ����������߶Ȳ
	return getHeight(root->lchild) - getHeight(root->rchild);//
}

void updateHeight(node* root) {//���½��߶ȣ����������нϴ��+1
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
}

void search(node* root, int x) {//����
	if (root == NULL) {
		cout << "����ʧ��" << endl;
		return;
	}
	if (x == root->v) {//�ҵ�
		cout << root->v << "���ҳɹ�"  << endl;
	}
	else if (x < root->v) {//�ȸ��ڵ�С������������
		search(root->lchild, x);
	}
	else {//��֮����������
		search(root->rchild, x);
	}
}

void L(node*& root) {//����
	node* temp = root->rchild;
	root->rchild = temp->lchild;
	temp->lchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void R(node*& root) {//����
	node* temp = root->lchild;
	root->lchild = temp->rchild;
	temp->rchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void insert(node*& root, int v) {//����
	if (root == NULL) {
		root = newNode(v);//�ҵ�����λ�ã��������
		return;
	}
	if (v < root->v) {//vС�ڸ��ڵ㣬����������ѯ
		insert(root->lchild, v);//����
		updateHeight(root);//��������
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
	else  {//v�ȸ��ڵ��
		insert(root->rchild, v);//����
		updateHeight(root);//��������
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

int findMin(node*& root) {//����С��Ԫ��
	if (root->lchild == NULL) {
		return root->v;
	}
	else {
		return findMin(root->lchild);
	}
}

node* Delete(node*& root, int v) {//ɾ��ָ��Ԫ��
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
			root->v = findMin(root->rchild);//������������С��ֵ����ý�㣬��֤����ƽ�����������
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

node* create(vector<int> data, int n) {//��ʼ������һ��ƽ�������
	node* root = NULL;
	for (int i = 0; i < n; i++) {
		insert(root, data[i]);
	}
	return root;
}

void out(node*& root) {//���һ����������
	if (root == NULL) {
		return;
	}
	out(root->lchild);
	cout << root->v << " ";
	out(root->rchild);
	return;
}

int main() {
	vector<int> data;//���Ԫ��
	int n;
	cout << "Ԫ�ظ���:";
	cin >> n;//�����С
	cout << n << "��Ԫ�أ��Կո�ָ�:";
	for (int i = 0; i < n; i++) {//����ֵ
		int x;
		cin >> x;
		data.push_back(x);
	}
	node* root = create(data, n);//����ƽ�������
	cout << endl << "������x y��x:1����;x:2ɾ��;x:3��ѯ,yΪֵ;x:4����������飬����0 0������" << endl;
	int x = 1, y = 1;//x:1��ѯ;x:2����;x:3��ѯ
	while (x != 0) {
		cin >> x >> y;
		if (x == 1) {
			insert(root, y);
			cout << "����ɹ�" << endl;
		}
		else if (x == 2) {
			Delete(root, y);
			cout << "ɾ���ɹ�" << endl;
		}
		else if (x == 3) {
			search(root, y);
		}
		else if (x == 4) {
			out(root);
			cout << endl;
		}
	}
	cout << "����";
	return 0;
}