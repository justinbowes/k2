//
//  view.c
//  K2
//
//  Created by Justin Bowes on 2013-08-23.
//  Copyright (c) 2013 Informi Software Inc. All rights reserved.
//

#include "view.h"
#include <SDL2/SDL.h>
#include <string.h>
#include "app.h"
#include "k2_log.h"

#define VIEW_STACK_MAX	64

static void default_view_activate(void) {
	LOG_WARN("Default view was activated");
}

static void default_view_deactivate(void) {
}

static void default_view_update(void) {
}

static void default_view_render(void) {
	SDL_SetRenderDrawColor(display.renderer, game.ticks % 0xFF, (game.ticks + 86) % 0xFF, (game.ticks + 172) % 0xFF, 0xFF);
	SDL_RenderClear(display.renderer);
}

typedef struct {
	view_t views[VIEW_STACK_MAX];
	size_t current;
} view_stack_t;

view_stack_t stack;

static view_t *current_view(void) {
	return &stack.views[stack.current];
}

void view_init(void) {
	memset(&stack, 0, sizeof(stack));
	stack.views[0] = (view_t) {
		default_view_activate,
		default_view_deactivate,
		default_view_update,
		default_view_render
	};
}

void view_push(view_t *new_view) {
	current_view()->deactivate();
	++stack.current;
	stack.views[stack.current] = *new_view;
	current_view()->activate();
}

void view_update(void) {
	current_view()->update();
}

void view_render(void) {
	current_view()->render();
	SDL_RenderPresent(display.renderer);
}