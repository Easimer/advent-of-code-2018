#include <stdio.h>
#include <pthread.h>
#include <xmmintrin.h>

constexpr int SERIAL = 7315;
constexpr int GRID_SIZE = 300;
using num_t = long long int;

constexpr num_t power_level(const int x, const int y) noexcept {
	auto rack = x + 10;
	return (((rack * (rack * y + SERIAL)) / 100) % 10) - 5;
}

void part1(int* mx, int* my, int* mp, int* ms, int dim) {
	auto dimc = dim / 2;
	for(int y = 0 + dimc; y < GRID_SIZE - dimc; y++) {
		for(int x = 0 + dimc; x < GRID_SIZE  - dimc; x++) {
			num_t power = 0;

			for(int dy = 0; dy < dim; dy++) {
				for(int dx = 0; dx < dim; dx++) {
					power += power_level(x + dx, y + dy);
				}
			}
			if(power > *mp || *mp == -1) {
				*mp = power;
				*mx = x;
				*my = y;
				*ms = dim;
			}
		}
	}
}

struct thread_param {
	int size;
	pthread_mutex_t* mutex;
	num_t *mx, *my, *mp, *ms;
	int* thread_count;
};

void* thread_entry(void* pArg) {
	thread_param* pParam = (thread_param*)pArg;
	int mx, my, mp, ms;
	part1(&mx, &my, &mp, &ms, pParam->size);

	pthread_mutex_lock(pParam->mutex);

	if(*pParam->mp < mp || *pParam->mp == -1) {
		*pParam->mx = mx;
		*pParam->my = my;
		*pParam->mp = mp;
		*pParam->ms = ms;
		printf("New maximum found: %d %d %d %d\n", mx, my, mp, ms);
	}

	printf("Thread %d has finished\n", pParam->size);
	(*pParam->thread_count)--;
	pthread_mutex_unlock(pParam->mutex);
	delete pParam;

	return nullptr;
}

int main() {
	{
		int mx, my, mp, ms;
		mx = my = ms = 0;
		mp = -1;
		part1(&mx, &my, &mp, &ms, 3);
		printf("Part 1: %d,%d\n", mx, my);
	}
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, nullptr);
	{
		num_t mx, my, mp, ms;
		mx = my = ms = 0;
		mp = -1;
		constexpr int THREADS = 4;
		pthread_t threads[THREADS];
		int thread_count = 0;
		int s = 1;
		thread_param param;
		param.mx = &mx;
		param.my = &my;
		param.mp = &mp;
		param.ms = &ms;
		param.mutex = &mutex;
		param.thread_count = &thread_count;
		while(1) {
			pthread_mutex_lock(&mutex);
			if(thread_count < THREADS && s <= GRID_SIZE) {
				auto pParam = new thread_param(param);
				pParam->size = s;
				if(pthread_create(&threads[s - 1], nullptr, &thread_entry, pParam) != 0) {
					printf("Thread creation has failed!!\n");
				}
				printf("Thread %d created\n", s);
				s++;
				thread_count++;
				pthread_mutex_unlock(&mutex);
				continue;
			}
			if(thread_count <= 0 && s >= GRID_SIZE) {
				break;
			}
			pthread_mutex_unlock(&mutex);
			_mm_pause();
		}
		printf("Part 2: %d,%d,%d\n", mx, my, ms);
	}
}
