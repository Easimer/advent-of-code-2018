#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum cart_dir {
	dleft, dright, dup, ddown, dstraight, dinvalid
};

enum tile_type {
	none,
	track_horizontal, track_vertical,
	curve_a, curve_b,
	intersection,
	invalid
};

struct cart {
	int X, Y;
	cart_dir dir, next_turn;
};

struct tile {
	int X, Y;
	tile_type type;
	bool skip;
	cart* current_cart;
};

struct network {
	tile* tiles;
	int width, height;
	int carts;
};

#define BOLD_ON "\033[1m"
#define BOLD_ON
#define PGREEN "\033[97;42m"
#define BOLD_OFF "\033[0m"


void print_network(const network& net) {
	for(int y = 0; y < net.height; y++) {
		for(int x = 0; x < net.width; x++) {
			const tile& t = net.tiles[y * net.width + x];
			if(t.current_cart) {
				if(t.current_cart->dir == dup) {
					printf(PGREEN BOLD_ON "^" BOLD_OFF);
				}
				if(t.current_cart->dir == ddown) {
					printf(PGREEN BOLD_ON "v" BOLD_OFF);
				}
				if(t.current_cart->dir == dleft) {
					printf(PGREEN BOLD_ON "<" BOLD_OFF);
				}
				if(t.current_cart->dir == dright) {
					printf(PGREEN BOLD_ON ">" BOLD_OFF);
				}
			} else {
				if(t.type == track_horizontal) {
					printf("-");
				}
				if(t.type == track_vertical) {
					printf("|");
				}
				if(t.type == curve_a) {
					printf("/");
				}
				if(t.type == curve_b) {
					printf("\\");
				}
				if(t.type == intersection) {
					printf("+");
				}
				if(t.type == none) {
					printf(" ");
				}
			}
		}
		printf("\n");
	}
}

cart_dir next_turn(cart_dir d) {
	switch(d) {
		case dleft:
			return dstraight;
		case dstraight:
			return dright;
		case dright:
			return dleft;
	}
	return dleft;
}

cart_dir map_cart(char c) {
	switch(c) {
		case '<':
			return dleft;
		case '>':
			return dright;
		case '^':
			return dup;
		case 'v':
			return ddown;
	}
	return dinvalid;
}

tile_type map_tile(char c) {
	switch(c) {
		case ' ':
			return none;
		case '-':
		case '>':
		case '<':
			return track_horizontal;
		case '|':
		case '^':
		case 'v':
			return track_vertical;
		case '/':
			return curve_a;
		case '\\':
			return curve_b;
		case '+':
			return intersection;
	}
	return invalid;
}

constexpr bool is_cart(const char c) {
	return (c == '<' || c == '>' || c == 'v' || c == '^');
}

network parse_input(FILE* file) {
	network ret;
	char line[512];
	int line_i = 0;
	char cbuf;
	int width = -1;
	int height = 0;
	int res;

	while(!feof(file)) {
		res = fread(&cbuf, 1, 1, file);
		if(res != 1) {
			break;
		}
		if(cbuf == '\n') {
			if(width == -1) {
				width = line_i;
			}
			height++;
		}
		line_i++;
	}

	ret.tiles = (tile*)calloc(width * height, sizeof(tile));
	ret.width = width;
	ret.height = height;
	ret.carts = 0;

	printf("Map size is %dx%d\n", ret.width, ret.height);

	fseek(file, 0, SEEK_SET);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			res = fread(&cbuf, 1, 1, file);
			assert(res == 1);
			ret.tiles[y * width + x].type = map_tile(cbuf);
			assert(ret.tiles[y * width + x].type != invalid);
			ret.tiles[y * width + x].current_cart = nullptr;
			ret.tiles[y * width + x].X = x;
			ret.tiles[y * width + x].Y = y;
			if(is_cart(cbuf)) {
				cart* c = (cart*)calloc(1, sizeof(cart));
				c->X = x;
				c->Y = y;
				c->dir = map_cart(cbuf);
				c->next_turn = dleft;
				ret.tiles[y * width + x].current_cart = c;
				ret.carts++;
			}
		}
		res = fread(&cbuf, 1, 1, file);
		assert(res == 1);
		assert(cbuf == '\n');
	}

	return ret;
}

tile* cart_next_tile(const network& net, int x, int y) {
	assert(!(x < 0 || x >= net.width));
	assert(!(y < 0 || y >= net.height));
	tile* t = &net.tiles[y * net.width + x];
	cart* c = t->current_cart;
	if(!c) {
		return nullptr;
	}

	if(c->dir == dright) {
		return &net.tiles[y * net.width + x + 1];
	}
	if(c->dir == dleft) {
		return &net.tiles[y * net.width + x - 1];
	}
	if(c->dir == dup) {
		return &net.tiles[(y - 1) * net.width + x];
	}
	if(c->dir == ddown) {
		return &net.tiles[(y + 1) * net.width + x];
	}

	return nullptr;
}

bool is_curved(const tile* t) {
	return t->type == curve_a || t->type == curve_b || t->type == intersection;
}

void apply_rotate(cart* c) {
	auto r = c->next_turn;
	if(r == dstraight) {
		c->next_turn = dright;
		return;
	}
	if(r == dleft) {
		if(c->dir == dleft) {
			c->dir = ddown;
		} else if(c->dir == dup) {
			c->dir = dleft;
		} else if(c->dir == dright) {
			c->dir = dup;
		} else {
			c->dir = dright;
		}
		c->next_turn = dstraight;
	}
	else if(r == dright) {
		if(c->dir == dleft) {
			c->dir = dup;
		} else if(c->dir == dup) {
			c->dir = dright;
		} else if(c->dir == dright) {
			c->dir = ddown;
		} else {
			c->dir = dleft;
		}
		c->next_turn = dleft;
	}
}

void rotate_cart(tile_type t, cart* c) {
	if(t == intersection) {
		apply_rotate(c);
	}
	else if(c->dir == dup) {
		if(t == curve_a) {
			c->dir = dright;
			return;
		} else if(t == curve_b) {
			c->dir = dleft;
			return;
		}
	} else if(c->dir == ddown) {
		if(t == curve_a) {
			c->dir = dleft;
			return;
		} else if(t == curve_b) {
			c->dir = dright;
			return;
		}
	} else if(c->dir == dleft) {
		if(t == curve_a) {
			c->dir = ddown;
			return;
		} else if(t == curve_b) {
			c->dir = dup;
		}
	} else if(c->dir == dright) {
		if(t == curve_a) {
			c->dir = dup;
			return;
		} else if(t == curve_b) {
			c->dir = ddown;
			return;
		}
	}
}

bool simulate(network& net) {
	for(int y = 0; y < net.height; y++) {
		for(int x = 0; x < net.width; x++) {
			tile* t = &net.tiles[y * net.width + x];
			if(t->skip) continue;
			assert(!(t->type == none && t->current_cart));
			if(t->current_cart) {
				auto c = t->current_cart;
				auto tn = cart_next_tile(net, x, y);
				assert(tn);
				auto c2 = tn->current_cart;
				if(c2) {
					// CRASH!
					printf("Type 1 Crash on X:%d,%d\n", tn->X, tn->Y);
					free(t->current_cart);
					free(tn->current_cart);
					t->current_cart = nullptr;
					tn->current_cart = nullptr;
					return true;
				} else {
					//printf("Cart goes from %d,%d to %d,%d\n", t->X, t->Y, tn->X, tn->Y);
					tn->current_cart = c;
					tn->skip = true;
					if(is_curved(tn)) {
						rotate_cart(tn->type, c);
						//printf("Track was curved, rotated\n");
					}
					t->current_cart = nullptr;

				}

			}
		}
	}
	for(int y = 0; y < net.height; y++) {
		for(int x = 0; x < net.width; x++) {
			tile* t = &net.tiles[y * net.width + x];
			t->skip = false;
		}
	}
	return false;
}

void find_first_cart(const network& net) {
	for(int y = 0; y < net.height; y++) {
		for(int x = 0; x < net.width; x++) {
			tile* t = &net.tiles[y * net.width + x];
			if(t->current_cart) {
				printf("Part 2: %d,%d\n", x, y);
				return;
			}
		}
	}
	printf("Part 2: FUCK\n");
}

int main(int argc, char** argv) {
	const char* filename = "input.txt";
	if(argc > 1) {
		filename = argv[1];
	}
	FILE* file = fopen(filename, "r");
	if(!file) {
		return -1;
	}
	auto net = parse_input(file);
	fclose(file);

	//for(int i = 0; i < 16; i++) {
	for(;;) {
		//print_network(net);
		if(simulate(net)) {
			if(net.carts == 3) {
				break;
			} else {
				net.carts -= 2;
				printf("Carts remaining: %d\n", net.carts);
			}
		}
	}
	print_network(net);
	find_first_cart(net);
}

