#pragma once
#include <cmath>

enum class tile_type_t {
	none, wall, ent, elf, goblin
};

struct tile_t;
struct ent_t;

struct ent_t {
	tile_t* tile;
	tile_type_t type;
	int hp;
	int ap;
};

struct tile_t {
	int x, y;
	tile_type_t type;
	ent_t* ent;

	// PF
	bool visited;
};

constexpr int grid_size = 32;

struct map_t {
	tile_t* tiles;
	ent_t* ents[grid_size * grid_size];
	int ent_count;
};


constexpr tile_t* idx(tile_t* map, int x, int y) {
	return &map[y * grid_size + x];
}

constexpr tile_t* idx(const map_t& map, int x, int y) {
	return idx(map.tiles, x, y);
}

constexpr bool is_entity(const tile_t* tile) {
	return tile->type == tile_type_t::elf || tile->type == tile_type_t::goblin;
}

constexpr int distance(const tile_t* lhs, const tile_t* rhs) {
	auto x = rhs->x - lhs->x;
	auto y = rhs->y - lhs->y;
	return sqrt(x * x + y * y);
}

map_t parse_map(const char* pszFilename);

struct tag_tile_type{};
struct tag_ent_type{};
tile_type_t map(char c, struct tag_tile_type);
tile_type_t map(char c, struct tag_ent_type);

// Returns whether the defender has died
bool damage(const ent_t* attacker, ent_t* defender);
// Returns the shortest path's distance and writes the next step to rx and ry
int shortest_path(map_t& map, int sx, int sy, int ex, int ey, int* rx, int* ry);
