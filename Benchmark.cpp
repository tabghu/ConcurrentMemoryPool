#include "ConcurrentAlloc.h"
#include "ObjectPool.h"
void BenchmarkMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&, k]() {
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(malloc(16));
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					free(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += end1 - begin1;
				free_costtime += end2 - begin2;
			}
		});
	}

	for (auto& t : vthread)
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次malloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, malloc_costtime);

	printf("%u个线程并发执行%u轮次，每轮次free %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, free_costtime);

	printf("%u个线程并发malloc&free %u次，总计花费：%u ms\n",
		nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}


// 单轮次申请释放次数 线程数 轮次
void BenchmarkConcurrentMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	size_t malloc_costtime = 0;
	size_t free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&]() {
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(ConcurrentAlloc(16));
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					ConcurrentFree(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += end1 - begin1;
				free_costtime += end2 - begin2;
			}
		});
	}

	for (auto& t : vthread)
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次concurrent alloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, malloc_costtime);

	printf("%u个线程并发执行%u轮次，每轮次concurrent dealloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, free_costtime);

	printf("%u个线程并发concurrent alloc&dealloc %u次，总计花费：%u ms\n",
		nworks, nworks*rounds*ntimes, malloc_costtime + free_costtime);
}
//void TestObjectPool()
//{
//	/*ObjectPool<TreeNode> tnPool;
//	std::vector<TreeNode*> v;
//	for (size_t i = 0; i < 100; ++i)
//	{
//		TreeNode* node = tnPool.New();
//		cout << node << endl;
//		v.push_back(node);
//	}
//
//	for (auto e : v)
//	{
//		tnPool.Delete(e);
//	}*/
//
//	/*ObjectPool<TreeNode> tnPool;
//	TreeNode* node1 = tnPool.New();
//	TreeNode* node2 = tnPool.New();
//	TreeNode* node3 = tnPool.New();
//	TreeNode* node4 = tnPool.New();
//	cout << node1 << endl;
//	cout << node2 << endl;
//	cout << node3 << endl;
//	cout << node4 << endl;
//	tnPool.Delete(node1);
//	tnPool.Delete(node4);
//
//
//	TreeNode* node10 = tnPool.New();
//	cout << node10 << endl;
//
//	TreeNode* node11 = tnPool.New();
//	cout << node11 << endl;*/
//
//	size_t begin1 = clock();
//	std::vector<TreeNode*> v1;
//	for (int i = 0; i < 100000; ++i)
//	{
//		v1.push_back(new TreeNode);
//	}
//	for (int i = 0; i < 100000; ++i)
//	{
//		delete v1[i];
//	}
//	v1.clear();
//
//	for (int i = 0; i < 100000; ++i)
//	{
//		v1.push_back(new TreeNode);
//	}
//
//	for (int i = 0; i < 100000; ++i)
//	{
//		delete v1[i];
//	}
//	v1.clear();
//	size_t end1 = clock();
//
//
//	ObjectPool<TreeNode> tnPool;
//	size_t begin2 = clock();
//	std::vector<TreeNode*> v2;
//	for (int i = 0; i < 10; ++i)
//	{
//		v2.push_back(tnPool.New());
//	}
//	for (int i = 0; i < 10; ++i)
//	{
//		tnPool.Delete(v2[i]);
//	}
//	v2.clear();
//
//	for (int i = 0; i < 10; ++i)
//	{
//		v2.push_back(tnPool.New());
//	}
//	for (int i = 0; i < 10; ++i)
//	{
//		tnPool.Delete(v2[i]);
//	}
//	v2.clear();
//
//	size_t end2 = clock();
//
//	cout << end1 - begin1 << endl;
//	cout << end2 - begin2 << endl;
//
//
//	ObjectPool<char> chPool;
//
//}
int main()
{
	cout << "==========================================================" << endl;
	BenchmarkMalloc(10000, 4, 10);
	cout << endl << endl;

	BenchmarkConcurrentMalloc(10000, 4, 10);
	cout << "==========================================================" << endl;

	return 0;

	//TestObjectPool();
}