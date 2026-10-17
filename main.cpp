#include <iostream>
#include <string>
using namespace std;
#include "DeviceManager.h"
// ---------------------------------------------------------------------
// main(): exercises every requirement in the exam spec.
// ---------------------------------------------------------------------
int main() {
	cout << "=== Test 1: operator== and operator<< ===" << endl;
	{
		Sensor s1("S-001", 50.0);
		Sensor s2("S-001", 75.0); // same id, different threshold
		cout << "s1 == s2 (same id)? " << (s1 == s2 ? "true" : "false")
			<< " (expected true)" << endl;
		cout << "s1 prints as: " << s1 << endl;
	}
	cout << "\n=== Test 2: Sensor.isOverThreshold ===" << endl;
	{
		Sensor s("S-002", 50.0);
		s.setReading(45.0);
		cout << "reading=45, threshold=50 -> isOverThreshold: "
			<< s.isOverThreshold() << " (expected 0)" << endl;
		s.setReading(60.0);
		cout << "reading=60, threshold=50 -> isOverThreshold: "
			<< s.isOverThreshold() << " (expected 1)" << endl;
	}
	cout << "\n=== Test 3: Actuator clamping and operator+= ===" << endl;
	{
		Actuator a("A-001", 100.0);
		a.setOutput(150.0);
		cout << "setOutput(150), max=100 -> output=" << a.getOutput()
			<< " (expected 100)" << endl;
		a.setOutput(40.0);
		a += 30.0;
		cout << "40 += 30 -> output=" << a.getOutput()
			<< " (expected 70)" << endl;
		a += 1000.0;
		cout << "70 += 1000 (clamped) -> output=" << a.getOutput()
			<< " (expected 100)" << endl;
	}
	cout << "\n=== Test 4: DeviceManager storage + type-tag downcast on read ==="
		<< endl;
	{
		DeviceManager manager(5);
		manager.addDevice(new Sensor("S-100", 20.0));
		manager.addDevice(new Actuator("A-100", 80.0));
		manager.addDevice(new Sensor("S-101", 30.0));
		cout << "count after 3 adds: " << manager.getCount()
			<< " (expected 3)" << endl;
		for (int i = 0; i < manager.getCount(); i++) {
			Device* d = manager.getDevice(i);
			cout << " " << *d;
			if (d->getType() == SENSOR) {
				cout << " reading=" << static_cast<Sensor*>(d)->getReading();
			}
			else {
				cout << " output=" << static_cast<Actuator*>(d)->getOutput();
			}
			cout << endl;
		}
		cout << "(manager going out of scope -- watch for ~Sensor()/~Actuator() below)" << endl;
	}
	cout << "(manager destroyed cleanly)" << endl;
	cout << "\n=== Test 5: capacity limit is enforced ===" << endl;
	{
		DeviceManager tiny(1);
		tiny.addDevice(new Sensor("S-200", 10.0));
		Actuator rejected("A-200", 10.0); // stack object, never owned by tiny
		tiny.addDevice(&rejected); // should be ignored, tiny is full
		cout << "count after exceeding capacity (cap=1): " << tiny.getCount()
			<< " (expected 1)" << endl;
	}
	cout << "\nAll tests completed." << endl;
	return 0;
}