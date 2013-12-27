//
// C++ ģ�漼��ʵ�ּ�ѭ������ʾ��.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdexcept>

//
// ѭ��������ģ��.
//
// ����ѭ�����ж�β��ԶΪ�գ���������ռ�Ӧ��ָ�������߼��ռ�� 1��
// �����صĶ��д�С size Ӧ��Ϊ�߼���С.
//
// ���/����ʱ��������/���罫�׳��쳣.
//
template <typename T>
class CircularQueue
{
private:
	T *_pQueue;
	size_t _front, _rear, _size;
	static const size_t _MAX_QUEUE_SIZE = 20;

public:
	CircularQueue(const size_t size = _MAX_QUEUE_SIZE)
		: _front(0)
		, _rear(0)
		, _size(size + 1)
	{ _pQueue = new T[_size]; }


	~CircularQueue(void)
	{ delete[] _pQueue; }


	size_t getSize(void) const
	{ return _size - 1; }


	T getFront(void) const
	{
		if (isEmpty()) {
			throw std::underflow_error("����Ϊ�� !");
		}
		return _pQueue[_front];
	}


	bool isEmpty(void) const
	{ return _front == _rear; }


	bool isFull(void) const
	{ return (_rear + 1) % _size == _front; }


	void enqueue(const T &val)
	{
		if (isFull()) {
			throw std::overflow_error("�������� !");
		}
		_pQueue[_rear] = val;
		_rear = (_rear + 1) % _size;
	}


	T dequeue(void)
	{
		if (isEmpty()) {
			throw std::underflow_error("�ӿ����� !");
		}
		T value = _pQueue[_front];
		_front = (_front + 1) % _size;

		return value;
	}
};


//
// ���в���
//
int main(void)
{
	CircularQueue<int> queue;
	const size_t QUEUE_SIZE = queue.getSize();

	for (size_t i = 0; i < QUEUE_SIZE; ++i)
	{
		queue.enqueue(i);
	}

	// queue.enqueue(QUEUE_SIZE);

	std::cout << std::endl
		<< "����" << (queue.isFull() ? "" : "��") << "Ϊ��."
		<< std::endl
		<< "����" << (queue.isEmpty() ? "" : "��") << "Ϊ��."
		<< std::endl;

	for (size_t i = 0; i < QUEUE_SIZE; ++i)
	{
		std::cout << std::setw(3) << queue.dequeue();
	}

	// std::cout << std::setw(3) << queue.dequeue();

	std::cout << std::endl
		<< "����" << (queue.isFull() ? "" : "��") << "Ϊ��."
		<< std::endl
		<< "����" << (queue.isEmpty() ? "" : "��") << "Ϊ��."
		<< std::endl;



	return EXIT_SUCCESS;
} 