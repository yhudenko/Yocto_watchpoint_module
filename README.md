# Yocto_watchpoint_module

Для установки:
modprobe watchpoint_module.ko watch_address=0x12345

Для проверки:
Запись: echo "Test" > /dev/watchpoint
Чтение: cat /dev/watchpoint
