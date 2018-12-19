#include "day15.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cerrno>

tile_type_t map(char c, struct tag_tile_type) {
	switch(c) {
		case '#': return tile_type_t::wall;
		case '.': return tile_type_t::none;
		case 'E': case 'G': return tile_type_t::ent;
	}
	printf("%s: CAN'T MAP %c TO ANY ENUM!\n", __func__, c);
	assert(0);
	return tile_type_t::none;
}

tile_type_t map(char c, struct tag_ent_type) {
	switch(c) {
		case 'E': return tile_type_t::elf;
		case 'G': return tile_type_t::goblin;
	}
	printf("%s: CAN'T MAP %c TO ANY ENUM!\n", __func__, c);
	assert(0);
	return tile_type_t::none;
}

map_t parse_map(const char* pszFilename) {
	char cbuf;
	map_t ret;
	FILE* file = fopen(pszFilename, "r");

	assert(file);
	if(!file) {
		ret.tiles = nullptr;
		return ret;
	}

	ret.ent_count = 0;

	ret.tiles = (tile_t*)calloc(grid_size * grid_size, sizeof(tile_t));
	assert(ret.tiles);

	for(int y = 0; y < grid_size; y++) {
		for(int x = 0; x < grid_size; x++) {
			auto t = &ret.tiles[y * grid_size + x];
			fread(&cbuf, 1, 1, file);
			t->type = map(cbuf, tag_tile_type{});
			t->x = x;
			t->y = y;
			t->visited = false;
			if(t->type == tile_type_t::ent) {
				t->ent = (ent_t*)calloc(1, sizeof(ent_t));
				assert(t->ent);
				t->ent->tile = t;
				t->ent->type = map(cbuf, tag_ent_type{});
				t->ent->hp = 200;
				t->ent->ap = 3;
				ret.ents[ret.ent_count++] = t->ent;
			}
		}
		fread(&cbuf, 1, 1, file);
		assert(cbuf == '\n');
	}

	fprintf(stderr, "MAP PARSE\nent_count = %d\n", ret.ent_count);
}
