#include "day15.h"
#include <cstdlib>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <cassert>
#include <cmath>

bool damage(const ent_t* attacker, ent_t* defender) {
	defender->hp -= attacker->ap;
	if(defender->hp <= 0) {
		defender->tile->ent = nullptr;
		free(defender);
		return true;
	}
	return false;
}

// For all neighbors, find which leads to the shortest path

using node = std::pair<int, int>;
int& score_or_default(std::map<node, int>& m, node n) {
	if(!m.count(n)) {
		m.emplace(n, -1);
	}
	return m[n];
}

inline int heurdist(const node& s, const node& e) {
	auto x = e.first - s.first;
	auto y = e.second - s.second;

	return sqrt(x * x + y * y);
}

std::vector<node> neighbors(const map_t& m, const node& n) {
#define TLA(x, y) \
	auto t = (m.tiles[(n.second + (y)) * grid_size + n.first + (x)]); \
	if(t.type == tile_type_t::none) { \
		ret.push_back({n.first + (x), n.second + (y)}); \
	}

	std::vector<node> ret;
	// sx sy-1
	if(n.second != 0) {
		TLA(0, -1);
	}
	// sx-1 sy
	if(n.first != 0) {
		TLA(-1, 0);
	}
	// sx + 1, sy
	if(n.first != grid_size - 1) {
		TLA(1, 0);
	}
	// sx, sy + 1
	if(n.second != grid_size - 1) {
		TLA(0, 1);
	}
	return ret;
#undef TLA
}

inline node get_current(std::set<node>& open_set, std::map<node, int>& fscore) {
	node ret;
	int min = -1;


	for(const auto& n : open_set) {
		auto ns = score_or_default(fscore, n);
		if(ns < min || min == -1) {
			ns = min;
			ret = n;
		}
	}

	open_set.erase(ret);
	return ret;
}

int write_result(const std::map<node, node>& came_from, node n, int* rx, int* ry) {
	int ret = 0;
	while(came_from.count(n)) {
		//printf("%d, %d\n", n.first, n.second);
		*rx = n.first;
		*ry = n.second;
		n = came_from.at(n);
		ret++;
	}
	return ret;
}

int shortest_path(map_t& map, node start, node goal, int* rx, int* ry) {
	auto closed_set = std::set<node>();
	auto open_set = std::set<node>();
	auto came_from = std::map<node, node>();
	auto gscore = std::map<node, int>();
	auto fscore = std::map<node, int>();

	open_set.insert(start);
	score_or_default(gscore, start) = 0;
	score_or_default(fscore, start) = heurdist(start, goal);

	while(open_set.size()) {
		auto current = get_current(open_set, fscore);
		if(current == goal) {
			return write_result(came_from, current, rx, ry);
		}
		closed_set.insert(current);

		for(auto neighbor : neighbors(map, current)) {
			if(closed_set.count(neighbor)) {
				continue;
			}

			auto tentative_gscore = score_or_default(gscore, current) + 1;
			if(!open_set.count(neighbor)) {
				open_set.emplace(neighbor);
			} else if(tentative_gscore >= score_or_default(gscore, neighbor)) {
				continue;
			}

			came_from[neighbor] = current;
			score_or_default(gscore, neighbor) = tentative_gscore;
			score_or_default(fscore, neighbor) = tentative_gscore + heurdist(neighbor, goal);
		}
	}
	return -1;
}

int shortest_path(map_t& map, int sx, int sy, int ex, int ey, int* rx, int* ry) {
	return shortest_path(map, {sx, sy}, {ex, ey}, rx, ry);
}
