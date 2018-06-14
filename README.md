# ELCO-PulseraMedidoraDeConstantes

**MUY IMPORTANTE:** BioCare es un prototipo y su funcionamiento práctico real actualmente está ligado a las siguientes medidas específicas:

En el interior de la pulsera se encuentran dos interruptores numerados (1 y 2), si el interruptor se encuentra bajo, éste estará conectado, 
si se encuentra alto estará desconectado. 
El interruptor 1 conecta la batería con la etapa de carga.
El interruptor 2 conecta la etapa de carga con el resto del circuito.

* Durante el funcionamiento normal de BioCare, ambos interruptores deben estar presionados.

* En caso de querer alimentar completamente la batería, deberá desconectar el interruptor 2 y dejar el 1 conectado. A continuación podrá
conectar el cable micro USB al puerto de carga situado en la parte superior de la carcasa, un LED rojo se iluminará en el frontal. 
Espere a que se vuelva verde para una carga completa. Este proceso durará algunas horas.

* En caso de querer recompilar el programa en la Wemos D1 mini (para ajustar la WiFi o el Servidor), desconecte ambos interruptores. A continuación,
conecte el cable micro USB desde su PC hasta el puerto micro USB que se encuentra en la placa Wemos D1 mini (no confundir con el puerto de carga). 



Configuración del Software:

    Arduino:
    En la función setup_wifi completar las siguientes asignaciones de variables:
      1) ssid: el String con la SSID de la WiFi a la que se conecta la placa de la pulsera.
      2) password: el String con la contraseña asociada a la SSID indicada previamente.
      3) host: el String con la dirección IP del servidor (ordenador que ejecuta el programa processing)
      4) port: el String con el puerto del servidor

    Processing:
      Asignar el String de la ruta raíz del directorio del programa processing
 
Una vez realizados los cambios anteriores ejecutar primero el servidor (programa Processing) y posteriormente pulsar el botón de reset de la
pulsera (una vez que el programa Arduino esté cargado).

Programa Arduino final: elco_final/elco_final.ino

Programa Processing: Processing_User_Interface/PROCESSINGWIFI_EDIT/PROCESSINGWIFI_EDIT.pde
