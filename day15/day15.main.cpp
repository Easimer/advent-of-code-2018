#include "day15.h"
#include <cstdio>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <map>

auto neighboring_tiles(const map_t& m, const tile_t* t) {
	std::vector<tile_t*> tiles;

	auto left = idx(m, t->x - 1, t->y);
	auto right = idx(m, t->x + 1, t->y);
	auto up = idx(m, t->x, t->y - 1);
	auto down = idx(m, t->x, t->y + 1);

	if(left->type == tile_type_t::none) {
		tiles.push_back(left);
	}
	if(right->type == tile_type_t::none) {
		tiles.push_back(right);
	}
	if(up->type == tile_type_t::none) {
		tiles.push_back(up);
	}
	if(down->type == tile_type_t::none) {
		tiles.push_back(down);
	}

	return tiles;
}

void print_map(const map_t& map) {
	for(int y = 0; y < grid_size; y++) {
		for(int x = 0; x < grid_size; x++) {
			const auto& t = map.tiles[y * grid_size + x];
			if(t.type == tile_type_t::wall) {
				printf("#");
			} else if(t.type == tile_type_t::ent) {
				if(t.ent) {
					if(t.ent->type == tile_type_t::elf) {
						printf("E");
					} else if(t.ent->type == tile_type_t::goblin) {
						printf("G");
					} else {
						printf("?");
					}
				} else {
					printf("!");
				}
			} else {
				printf(".");
			}
		}
		printf("\n");
	}
}

template<typename T>
struct sort_reading_order;

template<>
struct sort_reading_order<tile_t> {
	bool operator()(const tile_t* const lhs, const tile_t* const rhs) const noexcept {
		if(lhs->y < rhs->y) {
			return true;
		}
		if(lhs->y == rhs->y && lhs->x < rhs->x) {
			return true;
		}
		return false;
	}
};

int main(int argc, char** argv) {
	auto map = parse_map("input.txt");
	map = parse_map("test.txt");
	print_map(map);

	int cx = 19;
	int cy = 1;
	int ex = 19;
	int ey = 8;
	int d;

	std::vector<ent_t*> entities, elves, goblins;
	entities.reserve(grid_size * grid_size);
	elves.reserve(grid_size * grid_size);
	goblins.reserve(grid_size * grid_size);
	for(;;) {
		entities.clear(); // entities in read order
		elves.clear();
		goblins.clear();

		for(int y = 0; y < grid_size; y++) {
			for(int x = 0; x < grid_size; x++) {
				auto t = idx(map.tiles, x, y);
				if(t->ent) {
					entities.push_back(t->ent);
					if(t->ent->type == tile_type_t::elf) {
						elves.push_back(t->ent);
					}
					else if(t->ent->type == tile_type_t::goblin) {
						goblins.push_back(t->ent);
					} else {
						assert(!"unknown entity type");
					}
				}
			}
		}

		printf("FOR ALL ENTITIES\n");
		for(const auto entity : entities) {
			std::priority_queue<tile_t*, std::vector<tile_t*>, sort_reading_order<tile_t>> nearest_tiles;
			std::set<tile_t*> in_range_tiles;
			auto& enemies = entity->type == tile_type_t::elf ? goblins : elves;
			auto curt = entity->tile;

			for(const auto enemy : enemies) {
				for(auto tile : neighboring_tiles(map, enemy->tile)) {
					in_range_tiles.insert(tile);
				}
			}

			std::map<tile_t*, int> distances;

			printf("\tIN-RANGE\n");
			for(const auto tile : in_range_tiles) {
				printf("\t\t%d,%d\n", tile->x, tile->y);
				distances[tile] = distance(entity->tile, tile);
			}
			int mindist = -1;

			printf("\tREACHABLE\n");
			for(const auto& kv : distances) {
				int z;
				if(kv.second < mindist || mindist == -1) {
					if(shortest_path(map, curt->x, curt->y, kv.first->x, kv.first->y, &z, &z) != -1) {
						printf("\t\t%d,%d\n", kv.first->x, kv.first->y);
						mindist = kv.second;
					}
				}
			}

			printf("\tNEAREST\n");
			for(const auto& kv : distances) {
				if(kv.second == mindist) {
					printf("\t\t%d,%d %d\n", kv.first->x, kv.first->y, kv.second);
					nearest_tiles.push(kv.first);
				} else {
					printf("\t\tREJECT %d,%d %d\n", kv.first->x, kv.first->y, kv.second);
				}
			}

		}
	}

	return 0;
}
