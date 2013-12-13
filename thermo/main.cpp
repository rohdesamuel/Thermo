#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;

#include "nfilter.h"
#include "pfilter.h"
#include "layer.h"
#include "dsets.h"
#include "cmwc.h"

#if 0
int main()
{
	/*vector<Point<float, 5> *> means;
	for (int i = 0; i < 2; i ++)
	{
		float data[5] = {
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10)
			};
		means.push_back(new Point<float, 5>(data));
	}
	KMeans<float, 5> test(true, means);

	for (int i = 0; i < 2; i ++)
	{
		float data[5] = {
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10),
				(float)(rand()%10)
			};
		test.add(data);
	}*/
	//cout<<boost::math::binomial_coefficient<double>(1024, 512);
	/*LARGE_INTEGER start, end, freq;

	double lookup [513];
	for (int i = 0; i < 513; i ++)
		lookup[i] = boost::math::binomial_coefficient<double>(1024, i);

	while(1)
	{
		unsigned long long c1 = 0, c2 = 0;
		unsigned long long c = 0;

		for (int i = 0; i < 64; i ++)
		{
			c1 += rand() % 2;
			c2 += rand() % 2;
			c1 <<= 1;
			c2 <<= 1;
		}

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
		int dis = 0;
		c = ~(c1^c2);
		while(c)
		{
			dis += c & 0x1;
			c >>= 1;
		}
		QueryPerformanceCounter(&end);
		cout<< (double)(end.QuadPart - start.QuadPart)*1000000000 / (double)(freq.QuadPart)<<endl;
	}
	while(1) { }*/
	
	const int dim = 16;
	int active = 4;
	const int sample = 1000;
	const int radius = 4;
	//srand(time(NULL));
	PFilter<dim> a(radius, active);



	//while(1)
	for (int i = 0; i < sample; i ++)
	{
		bitset<dim> data;
		while(data.count() < active)
		{
			data[rand()%dim] = rand() % 2;
		}
		a.add(Point<dim>(data));
		//cout<< a.get().size() <<endl;
	}
	cout<< a.get().size() <<endl;
	//a();
	cout<<"done";
	for (int i = 0; i <  a.get().size(); i ++)
		for (int j = i; j < a.get().size(); j ++)
		{
			if (i!=j)
			{
			//cout <<i << " " << j << ": " ;
			//cout << a.get()[i]->distance(a.get()[j]) << endl;
			}
		}
		
	while(1);

	return 0;
}
#endif

#include "bitset.h"

#define DEBUG_INTEGRATION
#ifdef DEBUG_INTEGRATION
int main()
{
	const int dim = 1024;
	const int rfield = 16;
	const int active = 8;
	const int radius = 2;

	const int width = 1;
	const int height = 16;
	const int dis = 16;
	const int input_data_size = 4;
	const int num_temp_patterns = 2;

	Point<dim> inputData[num_temp_patterns][input_data_size];
	for (int i = 0; i < input_data_size; i ++)
	{
		int space = 8;
		for (int j = 0; j < active; j ++)
		{
			inputData[0][i].set((i*space) + j, true);
		}
	}

	for (int i = 0; i < input_data_size; i ++)
	{
		int space = 16;
		for (int j = 0; j < active*2; j ++)
		{
			inputData[1][i].set((i*space) + j, true);
		}
	}


	Point<dim> input;
	filters::PFilter<rfield> pfilter(radius, active);
	filters::NFilter<rfield> nfilter;
	
	Layer<filters::PFilter<rfield>, dim, rfield> layer_low(&input, &pfilter);
	Layer<filters::NFilter<rfield>, dim, rfield> layer_high(&input, &nfilter);

	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);
	double avg = 0;
	int count = 0;
	cmwc::srand(0);//time(NULL));
	for (int j = 0; j < 1; j ++)
	{
		for (int i = 0; i < 1000; i ++)
		{
			Bitset<dim> noise;
			int noise_count = 0; //cmwc::rand() % 128;
			while(noise.count() < noise_count)
			{
				noise(cmwc::rand()%dim, true);
			}
			Bitset<dim>::iterator it = noise.begin();
			input = inputData[j][i%input_data_size];
			while(it != noise.end())
			{
				input.flip(*it);
				it ++;
			}
		
			QueryPerformanceCounter(&start);
			layer_low.learn();
			QueryPerformanceCounter(&end);
			avg += end.QuadPart - start.QuadPart;
			count ++;
		}
	}

	cout << "Avg. Clock Cycles : " << (avg/count) <<endl;
	cout << "Avg. Layer Time   : " << (avg/count) / (double)freq.QuadPart <<endl;
	cout << "Avg. Node Time    : " << (avg/count) / (double)freq.QuadPart / (double)(dim/rfield)<<endl;

	while(1);
	return 0;
}
#endif

//#define DEBUG_HASHGRAPH
#ifdef DEBUG_HASHGRAPH

#include "HashGraph.h"

int main()
{
	Point<5> a, b, c;
	a.set(0, true);
	b.set(1, true);
	c.set(2, true);

	HashGraph<5> h;
	h.insertVertex(&a);
	h.insertVertex(&b);
	h.insertVertex(&c);

	h.insertEdge(&b, &a);
	h.insertEdge(&a, &b);
	
	h.insertEdge(&b, &c);
	h.insertEdge(&c, &b);

	h.insertEdge(&c, &a);
	h.insertEdge(&a, &c);


	h.getIn(&b);
	h.getOut(&b);

	HashGraph<5>::next_iterator it = h.beginOut(&a);
	while(it != h.endOut())
	{
		*it;
		it ++;
	}


	h.normalize();
	h.normalize(&a);
	while(1);

	return 0;
}

#endif

//#define DEBUG_DSETS
#ifdef DEBUG_DSETS

int main()
{
	DSets<char> dsets_1;
	DSets<char> dsets_2;
	dsets_1.add('a');
	dsets_1.add('b');
	dsets_1.add('c');
	dsets_1.setunion('a', 'b');

	dsets_2.add('a');
	dsets_2.add('b');
	dsets_2.add('c');
	dsets_2.setunion('b', 'c');
	dsets_2.setunion('b', 'b');

	dsets_1 = dsets_2;

	while(1);
	return 0;
}

#endif

#if 0

struct __comp
{
	bool operator()(int * a, int * b){return (*a<*b);};
};

#include "priorityqueue.h"
int main()
{
	PriorityQueue<int, float> a;
	PriorityQueue<int, float> *b = new PriorityQueue<int, float>();
	a.push(0, 0.0f);
	a.push(1, 1.0f);
	a.push(2, 2.0f);
	a.push(3, 3.0f);

	b->push(4, 4.0f);
	b->push(5, 5.0f);
	b->push(6, 6.0f);
	b->push(7, 7.0f);
	
	a = PriorityQueue<int, float>(*b);
	//PriorityQueue<int, float> c(*b);
	//PriorityQueue<int, float>::iterator it = a.begin();
	delete b;
	while(1);
	return 0;
}
#endif

#if 0

#include "bitset.h"
#include <bitset>

int main()
{
	Bitset<10> bs1, bs2, bs3;

	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);

	double bitset_avg = 0, sdr_avg = 0;
	int test_size = 1000;
	for(int j = 0; j < test_size; j ++)
	{
		const int dim = 16000;
		int active = 1024;

		bitset<dim> b1, b2; 
		while(b1.count() < active)
			b1[rand() % dim] = true;

		while(b2.count() < active)
			b2[rand() % dim] = true;

		Bitset<dim> s1, s2;
		for (int i = 0; i < active; i ++)
		{
			if (b1[i])
				s1(i, true);
			if (b2[i])
				s2(i, true);
		}
		
		QueryPerformanceCounter(&start);
		(b1 & b2).count();
		QueryPerformanceCounter(&end);
		bitset_avg += (double)(end.QuadPart - start.QuadPart);

		QueryPerformanceCounter(&start);
		s1.distance(s2);
		QueryPerformanceCounter(&end);
		sdr_avg += (double)(end.QuadPart - start.QuadPart);

		if (j % 10 == 0) cout<<"count: " << j << endl;
	}
	cout<<(bitset_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;
	cout<<(sdr_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;

	
	while(1);
	return 0;
}
#endif


#if 0

#include "sparserep.h"

int main()
{
	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);

	double bitset_avg = 0, sdr_avg = 0;
	int test_size = 1000;
	for(int j = 0; j < test_size; j ++)
	{
		const int dim = 16000;
		int active = 1024;

		vector<bool> b1(dim, false); 
		vector<int> indices;

		int on = 0;
		while(true)
		{
			int index = rand() % dim;
			if (!b1[index])
			{
				indices.push_back(index);
				b1[index] = true;
				++on;
			}
			if (on == active)
				break;
		}

		SparseRep<bool, dim> s1;
		for (int i = 0; i < active; i ++)
		{
			if (b1[i])
				s1[i] = true;
		}
		
		int index = indices[rand() % active];

		QueryPerformanceCounter(&start);
		b1[index];
		QueryPerformanceCounter(&end);
		bitset_avg += (double)(end.QuadPart - start.QuadPart);

		QueryPerformanceCounter(&start);
		s1[index];
		QueryPerformanceCounter(&end);
		sdr_avg += (double)(end.QuadPart - start.QuadPart);

		if (j % 10 == 0) cout<<"count: " << j << endl;
	}
	cout<<(bitset_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;
	cout<<(sdr_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;

	while(1);
	return 0;
}

#endif


#if 0
#include "priorityqueue.h"

int main()
{
	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);

	double bitset_avg = 0, sdr_avg = 0;
	int test_size = 1000;
	for(int j = 0; j < test_size; j ++)
	{
		const int dim = 16000;
		int active = 1024;

		vector<bool> b1(dim, false); 
		vector<int> indices;

		int on = 0;
		while(true)
		{
			int index = rand() % dim;
			if (!b1[index])
			{
				indices.push_back(index);
				b1[index] = true;
				++on;
			}
			if (on == active)
				break;
		}

		PriorityQueue<int, int> s1;
		for (int i = 0; i < active; i ++)
		{
			if (b1[i])
				s1.push(i, i);
		}
		
		int index = indices[rand() % active];

		QueryPerformanceCounter(&start);
		cout << b1[index]<<endl;
		QueryPerformanceCounter(&end);
		bitset_avg += (double)(end.QuadPart - start.QuadPart);

		QueryPerformanceCounter(&start);
		cout << s1.getPriority(index)<<endl;
		QueryPerformanceCounter(&end);

		sdr_avg += (double)(end.QuadPart - start.QuadPart);

		if (j % 10 == 0) cout<<"count: " << j << endl;
	}
	cout<<(bitset_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;
	cout<<(sdr_avg / test_size)<< endl;// / (double)freq.QuadPart << endl;

	while(1);
	return 0;
}
#endif