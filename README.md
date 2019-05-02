# qmk_firmware

![OK60 HHKB](images/hhkb.png)

## OK60 HHKB

    * Caps lock key assigned to MO(1) + Tab key
    * Turn the backlight off automatically in 30 seconds.
    * Turn LEDs off automatically in 5 minutes.

    `make ok60_hhkb:default`

    * If you do not need an idle timer, comment out IDLE_TIMER_ENABLE in rules.mk as shown below.

        * Backlight and LEDs idle timer is disable.
            ```
            #IDLE_TIMER_ENABLE = yes
            ```

## Keymap

![LAYOUT](images/layout.png)
