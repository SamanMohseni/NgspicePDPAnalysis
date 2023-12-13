/*
*   Author: Seyed Saman Mohseni Sangtabi
*   Student number: 99210067
*   Final exam, question 6
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Sample {
	double time;
	double voltage;
};

Sample* signal;
int signalSize;

bool loadSignal(string dir){
	ifstream signalFile;
	signalFile.open(dir, ios::in);
	if (!signalFile.is_open()){
		return 0;
	}

	string s;
	bool pointsFound = false;
	while (signalFile >> s){
		if (s == "Points:"){
			signalFile >> signalSize;
			signal = new Sample[signalSize];
			pointsFound = true;
		}
		if (pointsFound && s == "Values:"){
			break;
		}
	}

	while (true){
		int index;
		signalFile >> index;
		signalFile >> signal[index].time;
		signalFile >> signal[index].voltage;

		if (signalFile.eof()){
			break;
		}
	}
	signalFile.close();
}

void edgeTimeToBitPattern(double initialDelay, double period, double edgeTime, double duration){
	int LSB_bits = (duration - initialDelay) / period;
	bool powerOfTwo = !(LSB_bits == 0) && !(LSB_bits & (LSB_bits - 1));
	if (!powerOfTwo){
		cerr << "WARNING: All input space is not covered." << endl;
	}

	double correctedEdgeTime = edgeTime + period / 4 - initialDelay;

	int bit = 0;
	double p = period;
	while (LSB_bits > 1){
		cout << "bit" << bit << ": ";
		int pulseNumber = correctedEdgeTime / p;
		cout << (pulseNumber % 2 == 0) << endl;

		bit++;
		LSB_bits = LSB_bits >> 1;
		p *= 2;
	}
}

int main() {
	double minHigh, maxLow;
	double initialDelay, period;
	string signalFileDir;

	cout << "Lowest high voltage: ";
	cin >> minHigh;
	cout << "Highest low voltage: ";
	cin >> maxLow;

	cout << "Initial delay: ";
	cin >> initialDelay;
	cout << "Period: ";
	cin >> period;

	cout << "Signal file directory: ";
	cin >> signalFileDir;

	while (!loadSignal(signalFileDir)){
		cout << "No such file! Try again." << endl;
		cout << "Signal file directory: ";
		cin >> signalFileDir;
	}

	int startPoint = 0;
	for (int i = 0;; i++){
		if (signal[i].time >= initialDelay){
			startPoint = i;
			break;
		}
	}

	double nextEdge = initialDelay + period;
	double lastEntrance = initialDelay;
	bool preVoltageOutOfChannel = false;

	double maxDelay = 0;
	double maxDelayEdgeTime = 0;

	for (int i = startPoint; i < signalSize; i++){
		double t = signal[i].time;
		double v = signal[i].voltage;

		if (t >= nextEdge){
			if (preVoltageOutOfChannel){
				cerr << "ERROR: Period shorter than maximum delay." << endl;
				exit(1);
			}
			double delay = period / 2 - (nextEdge - lastEntrance);
			if (delay > maxDelay){
				maxDelay = delay;
				maxDelayEdgeTime = nextEdge - period;
			}
			lastEntrance = nextEdge;
			nextEdge += period;
		}

		bool voltageOutOfChannel = (v > maxLow && v < minHigh);

		if (preVoltageOutOfChannel && !voltageOutOfChannel){
			lastEntrance = t;
		}

		preVoltageOutOfChannel = voltageOutOfChannel;
	}

	cout << "Max delay = " << maxDelay << endl;
	cout << "Occurs after edge at time " << maxDelayEdgeTime << endl;
	cout << "Corresponding bit pattern:" << endl;
	edgeTimeToBitPattern(initialDelay, period, maxDelayEdgeTime, signal[signalSize - 1].time);

}