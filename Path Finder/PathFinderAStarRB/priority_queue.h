#pragma once

template<class T>
//simple priority queue with elements with the lowest value being first
//implemented as list
class Mpriority_queue
{
private:
	struct QNode
	{
		T data;
		QNode* next;

		QNode(const T& val):data(val), next(nullptr){}
		QNode(): next(nullptr){}
		~QNode() { next = nullptr; }
	};

	QNode* front = nullptr;

public:

	void push(const T& val)
	{
		QNode* current;
		QNode* previous=nullptr;
		QNode* temp = new QNode(val);

		if (front == nullptr || val <= front->data)
		{
			temp->next = front;
			front = temp;
		}
		else
		{
			current = front;
			while (current != nullptr && val > current->data)
			{
				previous = current;
				current = current->next;
			}
			temp->next = previous->next;
			previous->next = temp;

		}
	}

	T& top()
	{
		return front->data;
	}

	void pop()
	{
		if (front == nullptr)
			return;

		QNode* temp;
		temp = front;

		front = temp->next;
		delete temp;
	}

	bool empty()
	{
		return front == nullptr;
	}
};