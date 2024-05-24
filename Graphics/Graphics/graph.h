#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

const int maxSize = 20;
int amountVerts = 0;
int n;
int** help;
int* result;
int*** mat;
int R;
int WinW;
int WinH;

struct vertCoord//��������� ��������� ��������� 
{
	int x, y;
};
vertCoord vertC[20];

template<class T>
class Graph
{
	vector<T> vetrexList;
	vector<T> labelList;
	int size;
	bool* visitedVerts = new bool[vetrexList.size()];
public:
	vector<vector<int>> adjMatrix;
	Graph();
	Graph<T>(const int& ksize);
	~Graph();
	void DrawGraph();
	void InsertEdge(const T& vertex1, const T& vertex2, int weight); //������ �����, ����� �������� ��������� �������
	inline void insertVertex(const T& vertex);
	void removeVertex(const T& vertex);
	inline int GetVertPos(const T& vertex);
	inline bool isEmpty();
	inline bool IsFull();
	inline int GetAmountVerts();
	int GetAmountEdges();
	inline int GetWeight(const T& vertex1, const T& vertex2);
	vector<T> GetNbrs(const T& vertex);
	void PrintMatrix();
	void removeEdge(const T& vertex1, const T& vertex2);
	void editEdgeWeight(const T& vertex1, const T& vertex2, int newWeight);
};

Graph<int> graph(20);
template<class T>
vector<T> Graph<T> ::GetNbrs(const T& vetrex) {//������� ��� ��������� ������� �������
	vector<T> nbrsList;
	int pos = this->GetVertPos(vetrex);
	if (pos != -1) {
		for (int i = 0; i < this->vetrexList.size(); i++) {
			if (this->adjMatrix[pos][i] != 0) {
				nbrsList.push_back(this->vetrexList[i]);
			}
		}
	}
	return nbrsList;
}

template<class T>
inline void Graph<T>::insertVertex(const T& vert) {//�������, ������� ��������� �������
	if (this->IsFull()) {
		cout << "���������� �������� �������." << endl;
		return;
	}
	this->vetrexList.push_back(vert);
}
template<class T>
void Graph<T>::removeEdge(const T& vertex1, const T& vertex2) {//�������, ������� ������� �����
	int pos1 = GetVertPos(vertex1);
	int pos2 = GetVertPos(vertex2);

	if (pos1 == -1 || pos2 == -1) {
		cout << "����� �� ������ ��� � �����." << endl;
		return;
	}

	if (adjMatrix[pos1][pos2] == 0) {
		cout << "����� ����� ��������� " << vertex1 << " � " << vertex2 << " ���." << endl;
		return;
	}

	adjMatrix[pos1][pos2] = 0;
	adjMatrix[pos2][pos1] = 0;

	cout << "����� ����� ��������� " << vertex1 << " � " << vertex2 << " �������." << endl;
}

template<class T>
void Graph<T>::removeVertex(const T& vertex) {////�������, ������� ������� �������
	int pos = GetVertPos(vertex);
	if (pos == -1) {
		cout << "������� " << vertex << " ��� � �����." << endl;
		return;
	}

	for (int i = 0; i < size; i++) {
		if (adjMatrix[pos][i] != 0) removeEdge(vertex, vetrexList[i]);
		if (adjMatrix[i][pos] != 0) removeEdge(vetrexList[i], vertex);
	}

	vetrexList.erase(vetrexList.begin() + pos);

	// ������� ������� pos �� ������ ������ �������
	for (int i = 0; i < size; i++) {
		adjMatrix[i].erase(adjMatrix[i].begin() + pos);
	}
	// ������� ������ pos �� �������
	adjMatrix.erase(adjMatrix.begin() + pos);

	size--;

	cout << "������� " << vertex << " �������." << endl;
}

template<class T>
int Graph<T>::GetAmountEdges() {//������� ��� ��������� ���������� ����� ��� ������������������ �����
	int amount = 0;
	if (!this->isEmpty()) {
		for (int i = 0; i < this->vetrexList.size(); i++) {
			for (int j = 0; j < this->vetrexList.size(); j++) {
				if (this->adjMatrix[i][j] != 0) {
					amount++;
				}
			}
		}
	}
	return amount / 2;
}

template<class T>
inline int Graph<T>::GetWeight(const T& g1, const T& g2) {//��������� ���� ����� ���������
	if (this->isEmpty()) {
		return 0;
	}
	int g1_p = this->GetVertPos(g1);
	int g2_p = this->GetVertPos(g2);
	if (g1_p == -1 || g2_p == -1) {
		cout << "������ �� ��������� ����� � ����� �� ����������!";
		return 0;
	}
	return this->adjMatrix[g1_p][g2_p];
}

template<class T>
inline int Graph<T>::GetAmountVerts() {//��������� ���������� ������
	return this->vetrexList.size();
}
template<class T>
inline bool Graph<T>::isEmpty() {//�������� ����� �� ��, ��� �� ����
	return this->vetrexList.size() == 0;
}
template<class T>//�������� ����� �� ��, ��� �� ��������
inline bool Graph<T>::IsFull() {
	return (vetrexList.size() == maxSize);
}

template <class T>
inline int Graph<T>::GetVertPos(const T& g) {//��������� ������� ������
	for (int i = 0; i < vetrexList.size(); i++) {
		if (this->vetrexList[i] == g) {
			return i;
		}
	}
	return -1;
}
template<class T>
Graph<T>::Graph() {
	size = maxSize;
	labelList.resize(size, 1000000);
	adjMatrix.resize(size, vector<int>(size, 0));
	visitedVerts = new bool[size];
}

template<class T>
Graph<T>::Graph(const int& ksize) {
	size = ksize;
	labelList.resize(size, 1000000);
	adjMatrix.resize(size, vector<int>(size, 0));
	visitedVerts = new bool[size];
}

template<class T>
Graph<T>::~Graph() {//���������� �����

}

template<class T>
void Graph<T>::InsertEdge(const T& vetrex1, const T& vetrex2, int weight) {//������� ����� ��� ������������������ �����
	if (GetVertPos(vetrex1) != (-1) && this->GetVertPos(vetrex2) != (-1)) {
		int vertPos1 = GetVertPos(vetrex1);
		int vertPos2 = GetVertPos(vetrex2);
		if (this->adjMatrix[vertPos1][vertPos2] != 0 && this->adjMatrix[vertPos2][vertPos1] != 0) {
			cout << "����� ����� ��������� ��� ����" << endl;
			return;
		}
		else {
			this->adjMatrix[vertPos1][vertPos2] = weight;
			this->adjMatrix[vertPos2][vertPos1] = weight;
		}
	}
	else {
		cout << "�����-���� ������� ��� � �����" << endl;
		return;
	}
}

template<class T>
void Graph<T>::PrintMatrix() {//������ ������� ��������� �����
	if (!this->isEmpty()) {
		cout << "������� ���������: " << endl;
		cout << "- ";
		for (int i = 0; i < vetrexList.size(); i++) {
			cout << " " << vetrexList[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < this->vetrexList.size(); i++) {
			cout << this->vetrexList[i] << " ";
			for (int j = 0; j < this->vetrexList.size(); j++) {
				cout << " " << this->adjMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}
	else {
		cout << "���� ����" << endl;
	}
}
template<class T>
void Graph<T>::editEdgeWeight(const T& vertex1, const T& vertex2, int newWeight) {//�������, ������� ������ ��� ����� ����� ���������
	int pos1 = GetVertPos(vertex1);
	int pos2 = GetVertPos(vertex2);

	if (pos1 == -1 || pos2 == -1) {
		cout << "����� �� ������ ��� � �����." << endl;
		return;
	}

	if (adjMatrix[pos1][pos2] == 0) {
		cout << "����� ����� ��������� " << vertex1 << " � " << vertex2 << " ���." << endl;
		return;
	}

	adjMatrix[pos1][pos2] = newWeight;
	adjMatrix[pos2][pos1] = newWeight;

	cout << "��� ����� ����� ��������� " << vertex1 << " � " << vertex2 << " ������� �� " << newWeight << "." << endl;
}