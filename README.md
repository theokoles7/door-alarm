# ESP-WROOM-32 Door Alarm Project

## Materials
* ESP-WROOM-32 Dev Board
* USB to Micro-USB cable
* Magnetic Reed Switch
* 3.3v Piezo Buzzer
* 3-pin Slide Switch
* 1x Red LED
* 1x Blue LED
* 1x Green LED

## Wiring Up Components
Follow the wiring diagram included in the repository to place your components on the bread board.

## IFTTT
After you've signed in/created an account on https://ifttt.com/explore, select "Create"

![image](https://user-images.githubusercontent.com/61356329/172697535-0a2e6052-b770-4155-85f7-d2e75c30c97e.png)

On the "Create your own" screen, select "Add" on the "If This" block

![image](https://user-images.githubusercontent.com/61356329/172697425-8d9ae951-a469-4497-8f44-067e7b181973.png)

For "Choose a service", search and select "webhooks"

![image](https://user-images.githubusercontent.com/61356329/172697723-4e452c1f-6884-4683-bdb8-4b3f15ed21f8.png)

Select "Receive a web request"

![image](https://user-images.githubusercontent.com/61356329/172697894-58457c14-c063-4d2d-93d2-cc0585c4bccf.png)

Enter a name for your service and click "Create trigger"

![image](https://user-images.githubusercontent.com/61356329/172699695-25b65caf-b23e-4143-b36c-c2ab978c44dd.png)

Back on the "Create your own" screen, click "Add" on the "Then That" block.
Search and select "email" (not "email digest")

![image](https://user-images.githubusercontent.com/61356329/172700060-3303b100-be04-443f-947f-469b5bdb20f9.png)

Select "Send me an email"

![image](https://user-images.githubusercontent.com/61356329/172700664-8e2ba6e5-b1e3-4bc6-97be-4e1c3d6b55b8.png)

Edit the email template to your liking

![image](https://user-images.githubusercontent.com/61356329/172700596-45930e41-ea7a-4367-baef-17cc76324c07.png)

Back on the "Create your own" screen, click "Continue"
On the "Review and finish" screen, enter a name for your applet and click "Finish"
Go to this URL: https://ifttt.com/maker_webhooks and click "Documentation"
At the top of the page, copy the key given after "Your key is:" (You will need this key in your code later).

![image](https://user-images.githubusercontent.com/61356329/172702089-7bd57547-323c-4954-8c2e-9ce1ab3703ac.png)


## Code
Copy and paste the code in main/main.ino into the Arduino IDE

### Configurable Variables
Enter your Wifi's SSID and password into the appropriate fields (lines 23 & 24).
On line 102, paste your IFTTT key in-between the quotation marks. This key is used in the HTTP GET request to trigger your applet service.

### Installing ESP32 boards
If you've not already installed the ESP32 boards in your Arduino IDE, go to File > Preferences, paste
`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
into the "Additional Boards Manager URLs" text field, and click "OK".

Go to Tools > Board > Boards Manager, search for ESP32, and click install. Once the library has finished intsalling, click "Close"

### Set up
Go to Tools > Board > ESP32 Arduino > NodeMCU-32S
Go to Tools > Port and select the appropriate port at which your board is connected. For Linux this will typically be `/dev/ttyUSB0` and for Windows, it can be a couple of different COM ports.
Go to Tools and click "Serial Monitor". (Note: For Linux, if you get a permissions error, open a terminal and run `sudo chmod 777 /dev/ttyUSB0`)
In the serial monitor window, set the BAUD rate to 9600.

### Execution
Click the "Compile" button (check mark) at the top left of the Arduino IDE.
Once the code has completed compilation, click the "Upload" button (right-pointing-arrow) to upload the code to the board.

## Testing & Troubleshooting
First and foremost, watch the console and the serial monitor for messages.

### WiFi
The first thing that you should see in the serial monitor is a message saying that the Wifi has connected. On the board, you should see the blue light (LED pin 32) flashing while it's attempting to connect, then solid illumination once it has connected.

### Arming Alarm
Next, if your arming switch is active (pin 13 is connected to ground), you should see the green light illuminated, indicating that the sensor is armed. If not, activate the switch and verify.

### Triggering Alarm
Finally, while your sensor is armed, separate the magnet from the reed switch sensor. If all went well, the red light (LED pin 33) should flash and the buzzer should beep intermittently.

### Troubleshooting
* Ensure that all polarized leads are properly connected to ground/active pins
* Double check pin numbers
* Double check that you've set up the proper board type (ESP32 boards may differ depending on manufacturer), port, and baud rate properly
* If you receive a "brown out triggered" message, you will likely need a powered USB hub, as Wifi operations consume more power than a PC's onboard USB ports can provide
