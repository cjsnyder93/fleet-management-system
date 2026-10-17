#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <iostream>
#include <string>
using namespace std;

/**
 * @enum DeviceType
 * @brief Enumeration used as a type tag for non-polymorphic downcasting.
 *
 * SENSOR   - Device actually points to a Sensor
 * ACTUATOR - Device actually points to an Actuator
 */
enum DeviceType { SENSOR, ACTUATOR };

/**
 * @class Device
 * @brief Base class for Sensor and Actuator.
 *
 * Stores ID, type tag, and active status.
 * No virtual functions allowed (reminder).
 * Members are protected so derived classes can access them directly.
 */
class Device {
protected:
    string id;          ///< Unique identifier for the device
    DeviceType type;    ///< Type tag: SENSOR or ACTUATOR
    bool active;        ///< Whether the device is currently active

public:
    /**
     * @brief Constructs a Device with ID and type.
     * @param id Unique identifier
     * @param type Type tag (SENSOR or ACTUATOR)
     */
    Device(string id, DeviceType type);

    /**
     * @brief Returns the device ID.
     * @return ID string
     */
    string getID();

    /**
     * @brief Returns the device type tag.
     * @return DeviceType enum value
     */
    DeviceType getType();

    /**
     * @brief Returns whether the device is active.
     * @return true if active, false otherwise
     */
    bool isActive();

    /**
     * @brief Sets the active state.
     * @param state true to activate, false to deactivate
     */
    void setActive(bool state);

    /**
     * @brief Compares devices by ID.
     * @param other Device to compare with
     * @return true if IDs match, false otherwise
     */
    bool operator==(const Device& other) const;

    /**
     * @brief Prints ID, type, and active status.
     * @param out Output stream
     */
    void print(ostream& out) const;

    /**
     * @brief Destructor (does nothing).
     */
    ~Device();
};

/**
 * @brief Stream output operator for Device.
 * @param out Output stream
 * @param d Device to print
 * @return Reference to output stream
 */
ostream& operator<<(ostream& out, const Device& d);

/**
 * @class Sensor
 * @brief Represents a sensor with a reading and threshold.
 */
class Sensor : public Device {
private:
    double currentReading; ///< Most recent reading (starts at 0.0)
    double threshold;      ///< Threshold value set in constructor

public:
    /**
     * @brief Constructs a Sensor.
     * @param id Unique identifier
     * @param threshold Threshold value
     */
    Sensor(string id, double threshold);

    /**
     * @brief Sets the current reading.
     * @param value New reading value
     */
    void setReading(double value);

    /**
     * @brief Returns the current reading.
     * @return Current reading value
     */
    double getReading();

    /**
     * @brief Checks if reading exceeds threshold.
     * @return true if currentReading > threshold
     */
    bool isOverThreshold();

    /**
     * @brief Destructor (does nothing).
     */
    ~Sensor();
};

/**
 * @class Actuator
 * @brief Represents an actuator with output clamped to a maximum.
 */
class Actuator : public Device {
private:
    double outputLevel; ///< Current output (starts at 0.0)
    double maxOutput;   ///< Maximum allowed output

public:
    /**
     * @brief Constructs an Actuator.
     * @param id Unique identifier
     * @param maxOutput Maximum allowed output
     */
    Actuator(string id, double maxOutput);

    /**
     * @brief Sets the output level (clamped to maxOutput).
     * @param level Desired output level
     */
    void setOutput(double level);

    /**
     * @brief Returns the current output level.
     * @return Output level
     */
    double getOutput();

    /**
     * @brief Adds delta to outputLevel (clamped to maxOutput).
     * @param delta Amount to add
     * @return Reference to this Actuator
     */
    Actuator& operator+=(double delta);

    /**
     * @brief Destructor (does nothing).
     */
    ~Actuator();
};

/**
 * @class DeviceManager
 * @brief Manages a dynamically allocated array of Device pointers.
 *
 * No STL allowed (reminder).
 * Uses type-tag downcasting to delete correct derived type.
 */
class DeviceManager {
private:
    Device** devices; ///< Array of Device pointers
    int count;        ///< Number of devices stored
    int capacity;     ///< Maximum number of devices

public:
    /**
     * @brief Constructs a DeviceManager with fixed capacity.
     * @param cap Maximum number of devices
     */
    DeviceManager(int cap);

    /**
     * @brief Copy constructor (shallow copy).
     * @param other DeviceManager to copy
     */
    DeviceManager(const DeviceManager& other);

    /**
     * @brief Assignment operator (shallow copy).
     * @param other DeviceManager to assign from
     * @return Reference to this DeviceManager
     */
    DeviceManager& operator=(const DeviceManager& other);

    /**
     * @brief Adds a device if capacity allows.
     * @param d Pointer to Device (Sensor or Actuator)
     */
    void addDevice(Device* d);

    /**
     * @brief Returns device pointer at index.
     * @param index Valid index (caller guarantees correctness)
     * @return Device pointer
     */
    Device* getDevice(int index);

    /**
     * @brief Returns number of stored devices.
     * @return Count of devices
     */
    int getCount();

    /**
     * @brief Destructor.
     *
     * Deletes each stored device using type-tag downcasting,
     * then deletes the array itself.
     */
    ~DeviceManager();
};

#endif
