//
// C++ 模版技术实现简单循环队列示例.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdexcept>

//
// 循环队列类模版.
//
// 由于循环队列队尾永远为空，队列物理空间应比指定队列逻辑空间大 1，
// 而返回的队列大小 size 应该为逻辑大小.
//
// 入队/出队时当发生上/下溢将抛出异常.
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
			throw std::underflow_error("队列为空 !");
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
			throw std::overflow_error("队满上溢 !");
		}
		_pQueue[_rear] = val;
		_rear = (_rear + 1) % _size;
	}


	T dequeue(void)
	{
		if (isEmpty()) {
			throw std::underflow_error("队空下溢 !");
		}
		T value = _pQueue[_front];
		_front = (_front + 1) % _size;

		return value;
	}
};


//
// 队列测试
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
		<< "队列" << (queue.isFull() ? "" : "不") << "为满."
		<< std::endl
		<< "队列" << (queue.isEmpty() ? "" : "不") << "为空."
		<< std::endl;

	for (size_t i = 0; i < QUEUE_SIZE; ++i)
	{
		std::cout << std::setw(3) << queue.dequeue();
	}

	// std::cout << std::setw(3) << queue.dequeue();

	std::cout << std::endl
		<< "队列" << (queue.isFull() ? "" : "不") << "为满."
		<< std::endl
		<< "队列" << (queue.isEmpty() ? "" : "不") << "为空."
		<< std::endl;



	return EXIT_SUCCESS;
} 