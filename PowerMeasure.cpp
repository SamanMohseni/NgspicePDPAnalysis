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

int main() {
	double initialDelay;
	string signalFileDir;

	cout << "Initial delay: ";
	cin >> initialDelay;

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

	double energy = 0;

	for (int i = startPoint; i < signalSize - 1; i++){
		double t = signal[i].time;
		double v = signal[i].voltage;

		energy += (1.2 - v)*1.2*(signal[i + 1].time - t);
	}

	energy /= signal[signalSize - 1].time - initialDelay;

	cout << "Average Power = " << energy << endl;

}