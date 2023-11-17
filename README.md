# Proyecto2_SistemasOperativos

## Driver
Para cargar el driver se utiliza el comando insmod
``` sudo insmod driver.ko ```

Para ver el modulo en kernel use el comando
``` lsmod | grep driver ```

Para ver el log del driver use el comando
``` sudo dmesg | tail```

Para retirar el module use el comando
``` sudo rmmod driver ```

Para que el modulo eliminado no se vuelva a cargar corra
```echo "blacklist nombre_del_modulo" | sudo tee -a /etc/modprobe.d/blacklist.conf``
```sudo update-initramfs -u```
```sudo reboot```



## Librería
Para crear una librería use el comando (en la carpete Library)
```gcc -c commulib.c```
Luego, con el archivo compilado, cree la librería
```ar -rcs libcommulib.a commulib.o```
Finalmente compile el programa en el que usa la librería
```gcc -o main main.c Library/libcommulib.a```
