#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <inttypes.h>
#define PLAYERS (419)
#define LAST_MARBLE (7105200)

using num_t = uint64_t;

struct marble;

struct marble {
	marble *prev, *next;
	num_t N;
};

struct {
	marble arena[LAST_MARBLE];
	num_t i = 0;

	marble* alloc() {
		return arena + (i++);
	}
} allocator;

marble* create_marble(num_t N) {
	auto m = allocator.alloc();
	m->prev = m->next = nullptr;
	m->N = N;
	return m;
}

inline marble* clockwise(marble* m, int N) {
	auto ret = m;

	while(N--) {
		ret = ret->next;
	}

	return ret;
}

inline marble* cclockwise(marble* m, int N) {
	auto ret = m;

	while(N--) {
		ret = ret->prev;
	}

	return ret;
}

int main() {
	num_t scores[PLAYERS];
	memset(scores, 0, sizeof(num_t) * PLAYERS);
	marble* current = create_marble(0);
	auto zero = current;
	current->prev = current;
	current->next = current;
	num_t next_marble = 1;
	num_t player = 0;

	for(int next_marble = 1; next_marble <= LAST_MARBLE; next_marble++) {
		player = (player + 1) % PLAYERS;
		if(next_marble % 23 == 0) {
			scores[player] += next_marble;
			auto removant = cclockwise(current, 7);
			scores[player] += removant->N;
			removant->prev->next = removant->next;
			removant->next->prev = removant->prev;
			current = removant->next;
		} else {
			marble* nb = create_marble(next_marble);
			auto left = current->next;
			auto right = left->next;
			left->next = nb;
			right->prev = nb;
			nb->prev = left;
			nb->next = right;
			current = nb;
		}
		//auto cursor = zero;
		//printf("[%d] ", player);
		//do {
		//	if(cursor == current) {
		//		printf("(%d) ", cursor->N);
		//	} else {
		//		printf("%d ", cursor->N);
		//	}
		//	cursor = cursor->next;
		//} while(zero != cursor);
		//printf("\n");
	}

	num_t max = 0;
	for(int i = 0; i < PLAYERS; i++) {
		if(scores[i] > max) {
			max = scores[i];
		}
	}
	printf("\n%" PRIu64 "\n", max);
}
