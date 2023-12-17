# Ngspice Power-Delay Product Analysis of Adder Circuits using Ngspice and a custom-written C++ code.

This project was originally conducted as part of the **Spring 2021 Low Power Design** course.

## Description
In this project, we aim to implement and analyze two types of adder circuits: the ripple carry adder (a basic chain of Full Adders) and the carry skip adder. Using SPICE simulations, we will determine the Power-Delay Product (PDP) for these two methods. Instead of writing the SPICE code from scratch, we will leverage software that allows for the drawing of transistor-level schematic circuits from which SPICE code can be extracted. We will also examine how scaling the adder size from 8 to 12 and 16 bits affects the PDP ratio between the two methods.

## Implementation Details

### Schematic Design
For the schematic design of the adder circuits at the transistor level, we used LayoutEditor. We started by constructing Full Adder units at the transistor level using 90nm BSIM4 CMOS transistors, which can be retrieved from [ptm.asu.edu/latest.html](https://ptm.asu.edu/latest.html) and are included in the mosfet.pm file alongside the netlists.

### Full Adder Design Variations
There are various CMOS circuit designs for Full Adders, each with different power consumption, Sum output delay, and Cout output delay. Some designs include a propagate signal in part of the Full Adder circuit, useful for carry skip adders as they eliminate the need for a separate circuit to generate the propagate signal. Conversely, ripple carry adders do not require a propagate signal, and instead, reducing the Cout output delay plays a critical role in minimizing the critical path delay in such adders.

### Circuit Selection and Simulation
After considering various circuit designs, we decided to use the conventional Full Adder model employed in ripple carry adders and added an XOR circuit for use in carry skip adders. The designed Full Adder circuit for the ripple carry adder, which does not require a propagate signal, is shown below (Image 1), and the corresponding file is located in the SimpleFullAdder folder within the FA.les file.

![image](https://github.com/SamanMohseni/NgspicePDPAnalysis/assets/51726090/b11f966e-e6fb-4c4e-beb1-e01e5a203bd9)
*Image 1. Full Adder schematic designed in LayoutEditor software*

The ripple carry adder was simulated using ngspice version 34, which can be downloaded from [sourceforge.net/projects/ngspice/files/ng-spice-rework](https://sourceforge.net/projects/ngspice/files/ng-spice-rework).

The schematic for all designed components can be found in the Schematics folder.

### Delay Analysis
To calculate delay, we applied inputs to the circuit at specific time intervals using ngspice and stored the output voltages for the entire simulation duration. A C++ program, MaxDelayFinder.cpp, was used to analyze the output file and calculate the maximum delay, which is the intrval between new input and stabilization.

### Power Consumption Estimation
Relative power consumption for each adder was estimated by simulating the 4-bit blocks with a 1-ohm resistor in the power supply path and measuring the voltage across it. By analyzing the vdd.txt file using PowerMeasure.cpp, we calculated the average power consumption for the 4-bit ripple carry adder and the 4-bit carry skip adder.

### PDP Calculation
The PDP values for different adders were obtained by multiplying the power consumption by the respective delays, as shown in the table below:

| Adder Size | Carry Skip Adder PDP | Ripple Carry Adder PDP |
|------------|----------------------|------------------------|
| 8-bit      | 102.2                | 51.85                  |
| 12-bit     | 171.02               | 121.06                 |
| 16-bit     | 251.24               | 219.24                 |

These results indicate that carry skip adders perform better in terms of power consumption for higher bit calculations, although the improvement may not justify the energy cost for smaller bit sizes. It's worth noting that the carry skip adder circuits were not optimized and produced significant overhead, which could be improved upon.

## Conclusion
This project report implements and analyses the two types of digital adder circuits, highlighting the trade-offs between speed and power consumption in different adder architectures and provide insights into the scalability of these designs for larger bit-width operations.

## Acknowledgments
Course instructor: [Dr. Alireza Ejlali](https://scholar.google.com/citations?user=H7G8s68AAAAJ&hl=en)
