# ADC – ATSAML21 Xplained Pro MCU Embedded C
Sensors, Basic Analog-to-Digital Conversion, and the ATSAML21J18B’s 12-bit ADC.  

Programmed on Atmel Studio 8 for the ATSAML21J18B Atmel 32-bit microcontroller

Many embedded systems measure real-world quantities that are continuous, such as temperature, humidity, light intensity, and pressure. The physical quantity, property, or condition that is measured is called the measurand. Each measurand requires a sensor that generates an output that is related to the measurand. Fortunately, there are relatively low cost integrated circuit (IC) sensors available for most measurands. The output of many IC sensors is a voltage proportional to the measurand. This voltage is continuous across some range and is referred to as an analog voltage. A microcontroller’s CPU cannot process an analog voltage directly. The analog voltage must first be converted to a digital value. This digital value is often represented as an unsigned binary number. Conversion of an analog voltage to a digital value is accomplished using an analog-to-digital converter (ADC). Some microcontrollers have a built-in ADC. Alternatively, an external ADC IC can be used. ADC’s require a reference voltage to carry out their conversion process. The reference voltage may be built into the ADC or it may be a separate voltage reference IC. In this project, you will gain familiarity with a trimpot and a temperature sensor. You will use the 12-bit successive approximation ADC in the ATSAML21J18B’s to measure the outputs of these sensors.

Uses MCU as analog input voltage measurement for trimpot as well as LM34 analog temperature sensor, a pushbutton switch is used to toggle the output on the LCD display.


