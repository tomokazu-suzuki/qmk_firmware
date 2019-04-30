#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT_60_hhkb(
		KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_GRV,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
		KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,
		KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RO, KC_RSFT, MO(1),
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RALT, KC_RGUI, KC_RALT, KC_RCTL),

	[1] = LAYOUT_60_hhkb(
		RESET, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_INS,
		KC_CAPS, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_DEL,
		KC_TRNS, KC_VOLD, KC_VOLU, KC_MUTE, KC_TRNS, KC_TRNS, KC_ASTR, KC_SLSH, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_APP, BL_DEC, BL_TOGG, BL_INC, KC_PLUS, KC_MINS, KC_END, KC_PGDN, KC_DOWN, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
};

// Variables for idle timer
#ifdef RGBLIGHT_ENABLE
uint32_t rgblight_idle_timer;
bool is_rgblight_on = false;
bool is_rgblight_sleeping = false;
#endif
#ifdef BACKLIGHT_ENABLE
uint16_t backlight_idle_timer;
bool is_backlight_on = false;
bool is_backlight_sleeping = false;
uint8_t init_backlight_level;
uint8_t current_backlight_level;
#endif

void rgblight_toggle_ok60_hhkb(void)
{
#ifdef RGBLIGHT_ENABLE
	if (is_rgblight_on)
	{
		rgblight_disable();
	}
	else
	{
		rgblight_enable();
	}
	is_rgblight_on = !is_rgblight_on;
#endif
}

void backlight_toggle_ok60_hhkb(void)
{
#ifdef BACKLIGHT_ENABLE
	if (is_backlight_on)
	{
		backlight_disable();
	}
	else
	{
		backlight_enable();
	}
	is_backlight_on = !is_backlight_on;
#endif
}

void matrix_init_kb(void)
{
#ifdef RGBLIGHT_ENABLE
	is_rgblight_on = true;
	is_rgblight_sleeping = false;
#endif
#ifdef BACKLIGHT_ENABLE
	is_backlight_on = true;
	is_backlight_sleeping = false;
	init_backlight_level = get_backlight_level();
	current_backlight_level = init_backlight_level;
	backlight_enable();
#endif
	matrix_init_user();
}

void matrix_scan_kb(void)
{
	// put your looping keyboard code here
	// runs every cycle (a lot)
#ifdef RGBLIGHT_ENABLE
	if (is_rgblight_on)
	{
		if (!is_rgblight_sleeping && (timer_elapsed32(rgblight_idle_timer) > 300000))
		{
			rgblight_disable_noeeprom();
			is_rgblight_sleeping = true;
		}
	}
#endif
#ifdef BACKLIGHT_ENABLE
	if (is_backlight_on)
	{
		if (!is_backlight_sleeping && (timer_elapsed(backlight_idle_timer) > 30000))
		{
			backlight_disable();
			current_backlight_level = get_backlight_level();
			backlight_level(0);
			is_backlight_sleeping = true;
		}
	}
#endif

	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware

	// Update the timer for idle
#ifdef RGBLIGHT_ENABLE
	if (is_rgblight_on)
	{
		if (is_rgblight_sleeping)
		{
			rgblight_enable_noeeprom();
			is_rgblight_sleeping = false;
		}
		rgblight_idle_timer = timer_read32();
	}
#endif

#ifdef BACKLIGHT_ENABLE
	if (is_backlight_on)
	{
		if (is_backlight_sleeping)
		{
			backlight_level(current_backlight_level);
			is_backlight_sleeping = false;
		}
		backlight_idle_timer = timer_read();
	}
#endif

	switch (keycode)
	{
	case RGB_TOG:
#ifdef RGBLIGHT_ENABLE
		if (record->event.pressed)
		{
			rgblight_toggle_ok60_hhkb();
		}
#endif
		return false;
		break;
	case BL_TOGG:
#ifdef BACKLIGHT_ENABLE
		if (record->event.pressed)
		{
			backlight_toggle_ok60_hhkb();
		}
#endif
		return false;
		break;
	case BL_INC:
#ifdef BACKLIGHT_ENABLE
		if (record->event.pressed)
		{
			if (is_backlight_on)
			{
				backlight_increase();
			}
		}
#endif
		return false;
		break;
	case BL_DEC:
#ifdef BACKLIGHT_ENABLE
		if (record->event.pressed)
		{
			if (is_backlight_on)
			{
				backlight_decrease();
			}
		}
#endif
		return false;
		break;
	}
	return process_record_user(keycode, record);
}
