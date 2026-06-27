#ifndef PLOTRONIX_SERIAL_PROTOCOL_H
#define PLOTRONIX_SERIAL_PROTOCOL_H

#include <Arduino.h>

/* ============================================================================
 * SERIAL PROTOCOL DEFINITIONS
 * Protocol contract between firmware and GUI application
 * ============================================================================ */

/**
 * Protocol Overview:
 * 
 * HOST -> FIRMWARE (Commands):
 *   - "START"  : Begin streaming measurement data
 *   - "STOP"   : Stop streaming measurement data
 *   - All commands are newline-terminated
 * 
 * FIRMWARE -> HOST (Data Frames):
 *   - "DATA,<TAG>,<VOLTAGE>,<CURRENT>"
 *   - Format: comma-separated values, newline-terminated
 *   - TAG examples: DIODE, ZENER, BJT_CB, MOSFET, etc.
 *   - VOLTAGE: float with 3 decimal places
 *   - CURRENT: float with 5 decimal places
 * 
 * Baud Rate: 9600
 */

#define CMD_START       "START"   ///< Command to begin data streaming
#define CMD_STOP        "STOP"    ///< Command to stop data streaming
#define FRAME_PREFIX    "DATA"    ///< Prefix for all data frames

#endif // PLOTRONIX_SERIAL_PROTOCOL_H
