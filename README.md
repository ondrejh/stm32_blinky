# stm32_blinky
stm32f0 and eclipse

openocd: -f board/stm32f0discovery.cfg -c "program Release/stm32_blinky.hex verify reset exit"
