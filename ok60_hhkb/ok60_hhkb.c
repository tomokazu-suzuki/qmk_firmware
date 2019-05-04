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
ok60_hhkb_light_t hhkb_rgblight;
#endif
#ifdef BACKLIGHT_ENABLE
ok60_hhkb_light_t hhkb_backlight;
#endif

void rgblight_toggle_ok60_hhkb(void)
{
#ifdef RGBLIGHT_ENABLE
    if (hhkb_rgblight.is_on)
    {
        rgblight_disable();
    }
    else
    {
        rgblight_enable();
    }
    hhkb_rgblight.is_on = !hhkb_rgblight.is_on;
#endif
}

void backlight_toggle_ok60_hhkb(void)
{
#ifdef BACKLIGHT_ENABLE
    if (hhkb_backlight.is_on)
    {
        backlight_disable();
    }
    else
    {
        backlight_enable();
    }
    hhkb_backlight.is_on = !hhkb_backlight.is_on;
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
    hhkb_rgblight.is_on = true;
    hhkb_rgblight.status = RESUME;
    hhkb_rgblight.is_resume_trigger = true;
    hhkb_rgblight.resume_animation_start_level = 0;
#endif
#ifdef BACKLIGHT_ENABLE
    hhkb_backlight.is_on = true;
    hhkb_backlight.status = RESUME;
    hhkb_backlight.is_resume_trigger = true;
    hhkb_backlight.resume_animation_start_level = 0;
#endif
    matrix_init_user();
}

void matrix_scan_kb(void)
{
    // put your looping keyboard code here
    // runs every cycle (a lot)
#ifdef RGBLIGHT_ENABLE
    if (hhkb_rgblight.is_on)
    {
        if (hhkb_rgblight.is_resume_trigger)
        {
            hhkb_rgblight.resume_animation_start_level = rgblight_config.val;
            hhkb_rgblight.idle_timer = timer_read32();
        }

        switch (hhkb_rgblight.status)
        {
            uint8_t val;
        case SLEEP:
            if (hhkb_rgblight.is_resume_trigger)
            {
                hhkb_rgblight.status = RESUMING;
                hhkb_rgblight.resume_animation_timer = timer_read();
            }
            else if (rgblight_config.val > 0)
            {
                rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, 0);
            }
            break;
        case SLEEPING:
            if (hhkb_rgblight.is_resume_trigger)
            {
                hhkb_rgblight.status = RESUMING;
                hhkb_rgblight.resume_animation_timer = timer_read();
            }
            else
            {
                val = roundf(lerp(hhkb_rgblight.current_level, 0, easein((float)timer_elapsed(hhkb_rgblight.sleep_animation_timer) / SLEEP_ANIMATION_DURATION)));
                if (val != hhkb_rgblight.tmp_val)
                {
                    rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, val);
                    hhkb_rgblight.tmp_val = val;
                }
                if (val == 0)
                {
                    rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, 0);
                    hhkb_rgblight.status = SLEEP;
                }
            }
            break;
        case RESUME:
            if (timer_elapsed32(hhkb_rgblight.idle_timer) > RGBLIGHT_SLEEP_TIME_MS)
            {
                hhkb_rgblight.status = SLEEPING;
                hhkb_rgblight.current_level = rgblight_config.val;
                hhkb_rgblight.sleep_animation_timer = timer_read();
            }
            break;
        case RESUMING:
            val = roundf(lerp(hhkb_rgblight.resume_animation_start_level, hhkb_rgblight.current_level, easein((float)timer_elapsed(hhkb_rgblight.resume_animation_timer) / RESUME_ANIMATION_DURATION)));
            if (val != hhkb_rgblight.tmp_val)
            {
                rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, val);
                hhkb_rgblight.tmp_val = val;
            }
            if (val == hhkb_rgblight.current_level)
            {
                rgblight_sethsv_noeeprom(rgblight_config.hue, rgblight_config.sat, hhkb_rgblight.current_level);
                hhkb_rgblight.status = RESUME;
            }
            break;
        default:
            break;
        }

        if (hhkb_rgblight.is_resume_trigger)
        {
            hhkb_rgblight.is_resume_trigger = false;
        }
    }
#endif
#ifdef BACKLIGHT_ENABLE
    if (hhkb_backlight.is_on)
    {
        if (hhkb_backlight.is_resume_trigger)
        {
            hhkb_backlight.resume_animation_start_level = get_backlight_level();
            hhkb_backlight.idle_timer = timer_read32();
        }

        switch (hhkb_backlight.status)
        {
            uint8_t val;
        case SLEEP:
            if (hhkb_backlight.is_resume_trigger)
            {
                hhkb_backlight.status = RESUMING;
                hhkb_backlight.resume_animation_timer = timer_read();
            }
            else if (get_backlight_level() > 0)
            {
                backlight_level_noeeprom(0);
            }
            break;
        case SLEEPING:
            if (hhkb_backlight.is_resume_trigger)
            {
                hhkb_backlight.status = RESUMING;
                hhkb_backlight.resume_animation_timer = timer_read();
            }
            else
            {
                val = roundf(lerp(hhkb_backlight.current_level, 0, easein((float)timer_elapsed(hhkb_backlight.sleep_animation_timer) / SLEEP_ANIMATION_DURATION)));
                if (val != hhkb_backlight.tmp_val)
                {
                    backlight_level_noeeprom(val);
                    hhkb_backlight.tmp_val = val;
                }
                if (val == 0)
                {
                    backlight_level_noeeprom(0);
                    hhkb_backlight.status = SLEEP;
                }
            }
            break;
        case RESUME:
            if (timer_elapsed32(hhkb_backlight.idle_timer) > BACKLIGHT_SLEEP_TIME_MS)
            {
                hhkb_backlight.status = SLEEPING;
                hhkb_backlight.current_level = get_backlight_level();
                hhkb_backlight.sleep_animation_timer = timer_read();
            }
            break;
        case RESUMING:
            val = roundf(lerp(hhkb_backlight.resume_animation_start_level, hhkb_backlight.current_level, easeout((float)timer_elapsed(hhkb_backlight.resume_animation_timer) / RESUME_ANIMATION_DURATION)));
            if (val != hhkb_backlight.tmp_val)
            {
                backlight_level_noeeprom(val);
                hhkb_backlight.tmp_val = val;
            }
            if (val == hhkb_backlight.current_level)
            {
                backlight_level_noeeprom(hhkb_backlight.current_level);
                hhkb_backlight.status = RESUME;
            }
            break;
        default:
            break;
        }

        if (hhkb_backlight.is_resume_trigger)
        {
            hhkb_backlight.is_resume_trigger = false;
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
    if (hhkb_rgblight.is_on)
    {
        hhkb_rgblight.is_resume_trigger = true;
    }
#endif

#ifdef BACKLIGHT_ENABLE
    if (hhkb_backlight.is_on)
    {
        hhkb_backlight.is_resume_trigger = true;
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
            if (hhkb_backlight.is_on)
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
            if (hhkb_backlight.is_on)
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
