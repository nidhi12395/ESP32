# Simple HTTPD Server Example

The Example consists of HTTPD server demo with demostration of URI handling :
    1. URI \api\mpu6050\rawvalues for GET command returns raw values of acclerometer (x, y, z axis), gyroscope (x, y, z axis) and temperature.

* Open the project configuration menu (`idf.py menuconfig`) to configure Wi-Fi or Ethernet. 

* In order to test the HTTPD server persistent sockets demo :
    1. compile and burn the firmware `idf.py -p PORT flash`
    2. run `idf.py -p PORT monitor` and note down the IP assigned to your ESP module. The default port is 80
    3. test the example :
        Open postman: GET <id_address>\api\mpu6050\rawvalues.
