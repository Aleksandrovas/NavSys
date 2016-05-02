mkdir src
javac com\atmel\atusbhid\*.java -classpath AtUsbHidJni.jar -d ./src/
jar -cvfm AtUsbHidGui.jar com/AtUsbHidGui.MF -C src com
jar -cvfm AtUsbHidSimple.jar com/AtUsbHidSimple.MF -C src com
Pause