#ifndef OK60_HHKB_H
#define OK60_HHKB_H

#include "quantum.h"

#define LAYOUT_60_hhkb( \
	K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K014, \
	K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113,       \
	K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K213,       \
	K300, K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K312, K313, K314, \
	K400, K401, K402,                   K406,                   K410, K411, K412, K413  \
) { \
	{ K000,  K001,  K002,  K003,  K004,  K005,  K006,  K007,  K008,  K009,  K010,  K011,  K012,  K013,  K014 }, \
	{ K100,  K101,  K102,  K103,  K104,  K105,  K106,  K107,  K108,  K109,  K110,  K111,  K112,  K113,  KC_NO }, \
	{ K200,  K201,  K202,  K203,  K204,  K205,  K206,  K207,  K208,  K209,  K210,  K211,  K212,  K213,  KC_NO }, \
	{ K300,  K301,  K302,  K303,  K304,  K305,  K306,  K307,  K308,  K309,  K310,  K311,  K312,  K313,  K314 }, \
	{ K400,  K401,  K402,  KC_NO, KC_NO, KC_NO, K406,  KC_NO, KC_NO, KC_NO, K410,  K411,  K412,  K413,  KC_NO }  \
}

typedef enum { 
	SLEEP    = 0,
	SLEEPING = 1,
	RESUME   = 2,
	RESUMING = 3 
} ok60_hhkb_light_status_t;

typedef struct {
	uint32_t idle_timer;
	uint16_t sleep_animation_timer;
	uint16_t resume_animation_timer;
	bool is_on;
	uint8_t resume_animation_start_level;
	uint8_t current_level;
	uint8_t tmp_val;
	ok60_hhkb_light_status_t status;
	bool is_resume_trigger;
} ok60_hhkb_light_t;

#endif
