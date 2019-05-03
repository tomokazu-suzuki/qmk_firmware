#include "ok60_hhkb.h"
#ifdef IDLE_TIMER_ENABLE
#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
#endif
#ifdef BACKLIGHT_ENABLE
extern backlight_config_t backlight_config;
#endif

// Variables for idle timer
#ifdef RGBLIGHT_ENABLE
uint32_t rgblight_idle_timer;
uint16_t rgblight_sleep_animation_timer;
uint16_t rgblight_resume_animation_timer;
bool is_rgblight_on;
uint8_t rgblight_resume_animation_start_val;
uint8_t current_rgblight_val;
uint8_t tmp_rgblight_val;
ok60_hhkb_light_status_t hhkb_rgblight_status;
bool is_rgblight_resume_trigger;
#endif
#ifdef BACKLIGHT_ENABLE
uint32_t backlight_idle_timer;
uint16_t backlight_sleep_animation_timer;
uint16_t backlight_resume_animation_timer;
bool is_backlight_on;
uint8_t backlight_resume_animation_start_level;
uint8_t current_backlight_level;
uint8_t tmp_backlight_val;
ok60_hhkb_light_status_t hhkb_backlight_status;
bool is_backlight_resume_trigger;
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

void backlight_level_noeeprom(uint8_t level)
{
#ifdef BACKLIGHT_ENABLE
    if (level > BACKLIGHT_LEVELS)
        level = BACKLIGHT_LEVELS;
    backlight_config.level = level;
    backlight_config.enable = !!backlight_config.level;
    backlight_set(backlight_config.level);
#endif
}

float easeout(float progress) { return progress * (2 - progress); }
float easein(float progress) { return pow(progress, 2); }

float lerp(float x0, float x1, float x) { return x0 + ((x1 - x0) * x); }

void matrix_init_kb(void)
{
#ifdef RGBLIGHT_ENABLE
    is_rgblight_on = true;
    hhkb_rgblight_status = RESUME;
    is_rgblight_resume_trigger = false;
    rgblight_resume_animation_start_val = 0;
#endif
#ifdef BACKLIGHT_ENABLE
    is_backlight_on = true;
    hhkb_backlight_status = RESUME;
    is_backlight_resume_trigger = false;
    backlight_resume_animation_start_level = 0;
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
        if (is_rgblight_resume_trigger)
        {
            rgblight_resume_animation_start_val = rgblight_config.val;
            rgblight_idle_timer = timer_read32();
        }

        switch (hhkb_rgblight_status)
        {
            uint8_t val;
        case SLEEP:
            if (is_rgblight_resume_trigger)
            {
                hhkb_rgblight_status = RESUMING;
                rgblight_resume_animation_timer = timer_read();
            }
            break;
        case SLEEPING:
            if (is_rgblight_resume_trigger)
            {
                hhkb_rgblight_status = RESUMING;
                rgblight_resume_animation_timer = timer_read();
            }
            else
            {
                val = roundf(lerp(current_rgblight_val, 0, easein((float)timer_elapsed(rgblight_sleep_animation_timer) / SLEEP_ANIMATION_DURATION)));
                if (val != tmp_rgblight_val)
                {
                    rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, val);
                    tmp_rgblight_val = val;
                }
                if (val == 0)
                {
                    rgblight_sethsv(rgblight_config.hue, rgblight_config.sat, 0);
                    hhkb_rgblight_status = SLEEP;
                }
            }
            break;
        case RESUME:
            if (timer_elapsed32(rgblight_idle_timer) > RGBLIGHT_SLEEP_TIME_MS)
            {
                hhkb_rgblight_status = SLEEPING;
                current_rgblight_val = rgblight_config.val;
                rgblight_sleep_animation_timer = timer_read();
            }
            break;
        case RESUMING:
            val = roundf(lerp(rgblight_resume_animation_start_val, current_rgblight_val, easein((float)timer_elapsed(rgblight_resume_animation_timer) / RESUME_ANIMATION_DURATION)));
            if (val != tmp_rgblight_val)
            {
                rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, val);
                tmp_rgblight_val = val;
            }
            if (val == current_rgblight_val)
            {
                rgblight_sethsv(rgblight_config.hue, rgblight_config.sat, current_rgblight_val);
                hhkb_rgblight_status = RESUME;
            }
            break;
        default:
            break;
        }

        if (is_rgblight_resume_trigger)
        {
            is_rgblight_resume_trigger = false;
        }
    }
#endif
#ifdef BACKLIGHT_ENABLE
    if (is_backlight_on)
    {
        if (is_backlight_resume_trigger)
        {
            backlight_resume_animation_start_level = get_backlight_level();
            backlight_idle_timer = timer_read32();
        }

        switch (hhkb_backlight_status)
        {
            uint8_t val;
        case SLEEP:
            if (is_backlight_resume_trigger)
            {
                hhkb_backlight_status = RESUMING;
                backlight_resume_animation_timer = timer_read();
            }
            break;
        case SLEEPING:
            if (is_backlight_resume_trigger)
            {
                hhkb_backlight_status = RESUMING;
                backlight_resume_animation_timer = timer_read();
            }
            else
            {
                val = roundf(lerp(current_backlight_level, 0, easein((float)timer_elapsed(backlight_sleep_animation_timer) / SLEEP_ANIMATION_DURATION)));
                if (val != tmp_backlight_val)
                {
                    backlight_level_noeeprom(val);
                    tmp_backlight_val = val;
                }
                if (val == 0)
                {
                    backlight_level(0);
                    hhkb_backlight_status = SLEEP;
                }
            }
            break;
        case RESUME:
            if (timer_elapsed32(backlight_idle_timer) > BACKLIGHT_SLEEP_TIME_MS)
            {
                hhkb_backlight_status = SLEEPING;
                current_backlight_level = get_backlight_level();
                backlight_sleep_animation_timer = timer_read();
            }
            break;
        case RESUMING:
            val = roundf(lerp(backlight_resume_animation_start_level, current_backlight_level, easeout((float)timer_elapsed(backlight_resume_animation_timer) / RESUME_ANIMATION_DURATION)));
            if (val != tmp_backlight_val)
            {
                backlight_level_noeeprom(val);
                tmp_backlight_val = val;
            }
            if (val == current_backlight_level)
            {
                backlight_level(current_backlight_level);
                hhkb_backlight_status = RESUME;
            }
            break;
        default:
            break;
        }

        if (is_backlight_resume_trigger)
        {
            is_backlight_resume_trigger = false;
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
        is_rgblight_resume_trigger = true;
    }
#endif

#ifdef BACKLIGHT_ENABLE
    if (is_backlight_on)
    {
        is_backlight_resume_trigger = true;
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
                uint8_t level = get_backlight_level();
                if (level + BACKLIGHT_LEVEL_STEP > BACKLIGHT_LEVELS)
                {
                    level = BACKLIGHT_LEVELS;
                }
                else
                {
                    level += BACKLIGHT_LEVEL_STEP;
                }
                backlight_level(level);
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
                uint8_t level = get_backlight_level();
                if (level - BACKLIGHT_LEVEL_STEP < 0)
                {
                    level = 0;
                }
                else
                {
                    level -= BACKLIGHT_LEVEL_STEP;
                }
                backlight_level(level);
            }
        }
#endif
        return false;
        break;
    }
    return process_record_user(keycode, record);
}
#endif
