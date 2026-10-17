#include "DeviceManager.h"

/**
 * @brief Constructs a Device with ID and type.
 */
Device::Device(string id, DeviceType type)
    : id(id), type(type), active(true) {}

/**
 * @brief Returns the device ID.
 */
string Device::getID() { return id; }

/**
 * @brief Returns the device type tag.
 */
DeviceType Device::getType() { return type; }

/**
 * @brief Returns whether the device is active.
 */
bool Device::isActive() { return active; }

/**
 * @brief Sets the active state.
 */
void Device::setActive(bool state) { active = state; }

/**
 * @brief Compares devices by ID.
 */
bool Device::operator==(const Device& other) const {
    return this->id == other.id;
}

/**
 * @brief Prints ID, type, and active status.
 */
void Device::print(ostream& out) const {
    out << "ID: " << id
        << ", Type: " << (type == SENSOR ? "SENSOR" : "ACTUATOR")
        << ", Active: " << (active ? "true" : "false");
}

/**
 * @brief Device destructor (does nothing).
 */
Device::~Device() {}

/**
 * @brief Stream output operator for Device.
 */
ostream& operator<<(ostream& out, const Device& d) {
    d.print(out);
    return out;
}

// =======================
// Sensor Implementation
// =======================

/**
 * @brief Constructs a Sensor with ID and threshold.
 */
Sensor::Sensor(string id, double threshold)
    : Device(id, SENSOR), currentReading(0.0), threshold(threshold) {}

/**
 * @brief Sets the current reading.
 */
void Sensor::setReading(double value) { currentReading = value; }

/**
 * @brief Returns the current reading.
 */
double Sensor::getReading() { return currentReading; }

/**
 * @brief Checks if reading exceeds threshold.
 */
bool Sensor::isOverThreshold() {
    return currentReading > threshold;
}

/**
 * @brief Sensor destructor (does nothing).
 */
Sensor::~Sensor() {}


// =======================
// Actuator Implementation
// =======================

/**
 * @brief Constructs an Actuator with ID and maximum output.
 */
Actuator::Actuator(string id, double maxOutput)
    : Device(id, ACTUATOR), outputLevel(0.0), maxOutput(maxOutput) {}

/**
 * @brief Sets the output level (clamped to maxOutput).
 */
void Actuator::setOutput(double level) {
    if (level > maxOutput)
        outputLevel = maxOutput;
    else
        outputLevel = level;
}

/**
 * @brief Returns the current output level.
 */
double Actuator::getOutput() { return outputLevel; }

/**
 * @brief Adds delta to outputLevel (clamped to maxOutput).
 */
Actuator& Actuator::operator+=(double delta) {
    outputLevel += delta;
    if (outputLevel > maxOutput)
        outputLevel = maxOutput;
    return *this;
}

/**
 * @brief Actuator destructor (does nothing).
 */
Actuator::~Actuator() {}


// =======================
// DeviceManager Implementation
// =======================

/**
 * @brief Constructs a DeviceManager with fixed capacity.
 *
 * Ensures capacity is at least 1 to avoid zero-sized allocations.
 */
DeviceManager::DeviceManager(int cap)
    : capacity(cap), count(0)
{
    if (capacity <= 0) capacity = 1;
    devices = new Device * [capacity];
}

/**
 * @brief Copy constructor (shallow copy).
 *
 * Copies pointers only; does not clone devices.
 */
DeviceManager::DeviceManager(const DeviceManager& other)
    : capacity(other.capacity), count(other.count)
{
    int allocSize = capacity > count ? capacity : count;
    devices = new Device * [allocSize];
    capacity = allocSize;

    for (int i = 0; i < count; i++)
        devices[i] = other.devices[i];
}

/**
 * @brief Assignment operator (shallow copy).
 *
 * Deletes owned devices, then copies pointers from other.
 */
DeviceManager& DeviceManager::operator=(const DeviceManager& other) {
    if (this == &other) return *this;

    // Delete existing devices
    for (int i = 0; i < count; i++) {
        if (devices[i]->getType() == SENSOR)
            delete static_cast<Sensor*>(devices[i]);
        else
            delete static_cast<Actuator*>(devices[i]);
    }
    delete[] devices;

    // Copy new values
    capacity = other.capacity;
    count = other.count;

    int allocSize = capacity > count ? capacity : count;
    devices = new Device * [allocSize];
    capacity = allocSize;

    for (int i = 0; i < count; i++)
        devices[i] = other.devices[i];

    return *this;
}

/**
 * @brief Adds a device if capacity allows.
 */
void DeviceManager::addDevice(Device* d) {
    if (count < capacity) {
        devices[count] = d;
        count++;
    }
}

/**
 * @brief Returns device pointer at index.
 */
Device* DeviceManager::getDevice(int index) {
    return devices[index];
}

/**
 * @brief Returns number of stored devices.
 */
int DeviceManager::getCount() { return count; }

/**
 * @brief Destructor.
 *
 * Deletes each stored device using type-tag downcasting,
 * then deletes the array itself.
 */
DeviceManager::~DeviceManager() {
    for (int i = 0; i < count; i++) {
        if (devices[i]->getType() == SENSOR)
            delete static_cast<Sensor*>(devices[i]);
        else
            delete static_cast<Actuator*>(devices[i]);
    }
    delete[] devices;
}
