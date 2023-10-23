# Proyecto2_SistemasOperativos

En la carpeta Demo hay código de ejemplo para la elaboración del Driver
con el comando make se compila el código y se genera el archivo .ko

Para cargar el driver se utiliza el comando insmod
``` sudo insmod driver.ko ```

Para ver el modulo en kernel use el comando
``` lsmod | grep driver ```

Para ver el log del driver use el comando
``` sudo dmesg | tail```