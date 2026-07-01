#ifndef PLOTRONIX_SENSOR_DATA_H
#define PLOTRONIX_SENSOR_DATA_H

/* ============================================================================
 * SENSOR DATA STRUCTURES
 * Data models for sensor readings from the hardware
 * ============================================================================ */

/**
 * @struct SensorData
 * Represents a single complete measurement from all analog inputs
 */
struct SensorData {
    float voltage;          ///< Voltage across device under test (DUT)
    float current;          ///< Measured current through DUT (derived from shunt)
    uint8_t selectorId;     ///< Selector ID encoded from four digital switches
};

#endif // PLOTRONIX_SENSOR_DATA_H
