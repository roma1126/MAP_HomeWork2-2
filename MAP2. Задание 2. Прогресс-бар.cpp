#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include<windows.h>
#include<vector>
using namespace std::chrono_literals;

auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
std::mutex m;

void set_cursor(int x, int y)
{
	SetConsoleCursorPosition(handle, { static_cast<SHORT>(x), static_cast<SHORT>(y) });
}

void sim(int num, int length)
{
	auto start = std::chrono::high_resolution_clock::now();
	int count{};

	while (true)
	{
		m.lock();
		set_cursor(count + 18, num + 2);
		if (!count)
			std::cout << "\r поток " << num << " #id " << std::this_thread::get_id() << " ";
		else
			std::cout << "|";
		m.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(100 * (num + 2)));
		if (++count > length)
			break;
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double>time = end - start;
	set_cursor(length + 20, num + 2);
	std::cout << " " << time.count() << std::endl;
	
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int numOfThreads;
	int length;
	std::cout << "Введите количество потоков: ";
	std::cin >> numOfThreads;
	std::cout << "Введите количество итераций: ";
	std::cin >> length;

	
	std::vector<std::thread>threads(numOfThreads);

	for (int i = 0; i < numOfThreads; ++i)
	{
		threads[i] = std::thread(sim, i, length);
	}
	for (auto& entry : threads)
	{
		entry.join();
	}
	return 0;
}
