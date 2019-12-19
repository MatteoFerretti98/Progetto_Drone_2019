// Most of the functionality of this library is based on the VL53L1X API
// provided by ST (STSW-IMG007), and some of the explanatory comments are quoted
// or paraphrased from the API source code, API user manual (UM2356), and
// VL53L1X datasheet.
#if 1
#include "Altimetro.h"


void Sensor_Init(vl53l1x* sens)
{
	sens->address = AddressDefault;
	sens->io_timeout = 0; // no timeout
	sens->did_timeout = false;
	sens->calibrated = false;
	sens->saved_vhv_init = 0;
	sens->saved_vhv_timeout = 0;
	sens->distance_mode = Unknown;
	//sens->fast_osc_frequency = readReg16Bit(OSC_MEASURED__FAST_OSC__FREQUENCY, sens); //QUI SCRIVE, CON IL READ PERO' NO
	//sens->osc_calibrate_val = readReg16Bit(RESULT__OSC_CALIBRATE_VAL, sens); //IDEM

}

// Public Methods //////////////////////////////////////////////////////////////


uint8_t getAddress(vl53l1x* sens) { return sens->address;};

enum DistanceMode getDistanceMode(vl53l1x*sens) { return sens->distance_mode; }

bool dataReady(vl53l1x* sens) { return (readReg(GPIO__TIO_HV_STATUS, sens) & 0x01) == 0; }

void setTimeout(uint16_t timeout, vl53l1x* sens) { sens->io_timeout = timeout; }

void startTimeout(vl53l1x* sens) {sens->timeout_start_ms = millis();}

bool checkTimeoutExpired(vl53l1x* sens) {return (sens->io_timeout > 0) && ((uint16_t)(millis() - sens->timeout_start_ms) > sens->io_timeout); }

uint16_t getTimeout(vl53l1x* sens) { return sens->io_timeout; }

uint16_t readRangeContinuousMillimeters(vl53l1x* sens) { return read(sens); } // alias of read()

float countRateFixedToFloat(uint16_t count_rate_fixed) { return (float)count_rate_fixed / (1 << 7); }

unsigned long millis() {return getTime();};

void setAddress(uint8_t new_addr, vl53l1x* sens)
{
  writeReg(I2C_SLAVE__DEVICE_ADDRESS, new_addr, sens);
  sens->address = new_addr;
}


// Initialize sensor using settings taken mostly from VL53L1_DataInit() and
// VL53L1_StaticInit().
// If io_2v8 (optional) is true or not given, the sensor is configured for 2V8
// mode.
bool init(vl53l1x* sens)
{
	bool io_2v8 = true;
  // check model ID and module type registers (values specified in datasheet)

if (readReg16Bit(IDENTIFICATION__MODEL_ID, sens) != 0xEACC) { lcd_display(LCD_LINE7, "___ID: Err__");return false; }
lcd_display(LCD_LINE7, "___ID: OK___");

  // VL53L1_software_reset() begin

  writeReg(SOFT_RESET, 0x00,sens);
  ms_delay(1);
  writeReg(SOFT_RESET, 0x01, sens);

  // give it some time to boot; otherwise the sensor NACKs during the readReg()
  // call below and the Arduino 101 doesn't seem to handle that well
  //delay(1);

  // VL53L1_poll_for_boot_completion() begin

  startTimeout(sens);

  // check last_status in case we still get a NACK to try to deal with it correctly
  while ((readReg(FIRMWARE__SYSTEM_STATUS,sens) & 0x01) == 0 || sens->last_status != 0)
  {
    if (checkTimeoutExpired(sens))
    {
      sens->did_timeout = true;
      lcd_display(LCD_LINE6, "___FW: Err__");
      return false;
    }
  }

  lcd_display(LCD_LINE8, "___FW: Ok___");
  // VL53L1_poll_for_boot_completion() end

  // VL53L1_software_reset() end

  // VL53L1_DataInit() begin

  // sensor uses 1V8 mode for I/O by default; switch to 2V8 mode if necessary
  if (io_2v8)
  {
	  writeReg(PAD_I2C_HV__EXTSUP_CONFIG, readReg(PAD_I2C_HV__EXTSUP_CONFIG,sens) | 0x01, sens);
  }

  // store oscillator info for later use
  sens->fast_osc_frequency = readReg16Bit(OSC_MEASURED__FAST_OSC__FREQUENCY, sens);
  sens->osc_calibrate_val = readReg16Bit(RESULT__OSC_CALIBRATE_VAL, sens);

  // VL53L1_DataInit() end

  // VL53L1_StaticInit() begin

  // Note that the API does not actually apply the configuration settings below
  // when VL53L1_StaticInit() is called: it keeps a copy of the sensor's
  // register contents in memory and doesn't actually write them until a
  // measurement is started. Writing the configuration here means we don't have
  // to keep it all in memory and avoids a lot of redundant writes later.

  // the API sets the preset mode to LOWPOWER_AUTONOMOUS here:
  // VL53L1_set_preset_mode() begin

  // VL53L1_preset_mode_standard_ranging() begin

  // values labeled "tuning parm default" are from vl53l1_tuning_parm_defaults.h
  // (API uses these in VL53L1_init_tuning_parm_storage_struct())

  // static config
  // API resets PAD_I2C_HV__EXTSUP_CONFIG here, but maybe we don't want to do
  // that? (seems like it would disable 2V8 mode)
  writeReg16Bit(DSS_CONFIG__TARGET_TOTAL_RATE_MCPS, TargetRate,sens); // should already be this value after reset
  writeReg(GPIO__TIO_HV_STATUS, 0x02,sens);
  writeReg(SIGMA_ESTIMATOR__EFFECTIVE_PULSE_WIDTH_NS, 8,sens); // tuning parm default
  writeReg(SIGMA_ESTIMATOR__EFFECTIVE_AMBIENT_WIDTH_NS, 16,sens); // tuning parm default
  writeReg(ALGO__CROSSTALK_COMPENSATION_VALID_HEIGHT_MM, 0x01,sens);
  writeReg(ALGO__RANGE_IGNORE_VALID_HEIGHT_MM, 0xFF,sens);
  writeReg(ALGO__RANGE_MIN_CLIP, 0,sens); // tuning parm default
  writeReg(ALGO__CONSISTENCY_CHECK__TOLERANCE, 2,sens); // tuning parm default

  // general config
  writeReg16Bit(SYSTEM__THRESH_RATE_HIGH, 0x0000,sens);
  writeReg16Bit(SYSTEM__THRESH_RATE_LOW, 0x0000,sens);
  writeReg(DSS_CONFIG__APERTURE_ATTENUATION, 0x38,sens);

  // timing config
  // most of these settings will be determined later by distance and timing
  // budget configuration
  writeReg16Bit(RANGE_CONFIG__SIGMA_THRESH, 360,sens); // tuning parm default
  writeReg16Bit(RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS, 192,sens); // tuning parm default

  // dynamic config

  writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_0, 0x01, sens);
  writeReg(SYSTEM__GROUPED_PARAMETER_HOLD_1, 0x01,sens);
  writeReg(SD_CONFIG__QUANTIFIER, 2,sens); // tuning parm default

  // VL53L1_preset_mode_standard_ranging() end

  // from VL53L1_preset_mode_timed_ranging_*
  // GPH is 0 after reset, but writing GPH0 and GPH1 above seem to set GPH to 1,
  // and things don't seem to work if we don't set GPH back to 0 (which the API
  // does here).
  writeReg(SYSTEM__GROUPED_PARAMETER_HOLD, 0x00,sens);
  writeReg(SYSTEM__SEED_CONFIG, 1,sens); // tuning parm default

  // from VL53L1_config_low_power_auto_mode
  writeReg(SYSTEM__SEQUENCE_CONFIG, 0x8B,sens); // VHV, PHASECAL, DSS1, RANGE
  writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 200 << 8,sens);
  writeReg(DSS_CONFIG__ROI_MODE_CONTROL, 2, sens); // REQUESTED_EFFFECTIVE_SPADS

  // VL53L1_set_preset_mode() end

  // default to long range, 50 ms timing budget
  // note that this is different than what the API defaults to
  setDistanceMode(Long, sens);
  setMeasurementTimingBudget(50000, sens);

  // VL53L1_StaticInit() end

  // the API triggers this change in VL53L1_init_and_start_range() once a
  // measurement is started; assumes MM1 and MM2 are disabled
  writeReg16Bit(ALGO__PART_TO_PART_RANGE_OFFSET_MM,readReg16Bit(MM_CONFIG__OUTER_OFFSET_MM,sens) * 4,sens);
  return true;
}

// Write an 8-bit register
void writeReg(uint16_t reg, uint8_t value, vl53l1x *sens)
{
  /*Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  Wire.write(value);
  last_status = Wire.endTransmission();*/

  sens->last_status= i2c_write_Alt(VL53L1X_ADDRESS, reg, 1, &value, 3);

}

// Write a 16-bit register
void writeReg16Bit(uint16_t reg, uint16_t value, vl53l1x *sens)
{
  /*Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  Wire.write((value >> 8) & 0xFF); // value high byte
  Wire.write( value       & 0xFF); // value low byte
  last_status = Wire.endTransmission();*/
  uint8_t data_buffer[2];
  data_buffer[0]= (value>>8) & 0xFF;
  data_buffer[1]= value & 0xFF;


  sens->last_status= i2c_write_Alt(VL53L1X_ADDRESS, reg, 2, data_buffer,3);
}

// Write a 32-bit register
void writeReg32Bit(uint16_t reg, uint32_t value, vl53l1x *sens)
{
  /*Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  Wire.write((value >> 24) & 0xFF); // value highest byte
  Wire.write((value >> 16) & 0xFF);
  Wire.write((value >>  8) & 0xFF);
  Wire.write( value        & 0xFF); // value lowest byte
  last_status = Wire.endTransmission();*/

  uint8_t data_buffer[4];
  data_buffer[0]= (value>>24) & 0xFF;
  data_buffer[1]= (value>>16) & 0xFF;
  data_buffer[2]= (value>>8) & 0xFF;
  data_buffer[3]= value & 0xFF;

  sens->last_status= i2c_write_Alt(VL53L1X_ADDRESS, reg, 4, data_buffer, 3);
}

//Read an 8-bit register
uint8_t readReg(uint16_t reg, vl53l1x *sens)
{
  uint8_t value;
  /*
  Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  last_status = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)1);
  value = Wire.read();*/


  sens->last_status = i2c_read_Alt (VL53L1X_ADDRESS, reg, 2, &value, 3);


  return value;
}

// Read a 16-bit register
uint16_t readReg16Bit(uint16_t reg, vl53l1x *sens)
{
  uint16_t value;

/*
  Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  last_status = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)2);
  value  = (uint16_t)Wire.read() << 8; // value high byte
  value |=           Wire.read();      // value low byte
*/
  uint8_t data_buffer[2] = {0,0};
  sens->last_status = i2c_read_Alt(VL53L1X_ADDRESS, reg, 2, data_buffer, 3);
  value = (uint16_t)data_buffer[0]<<8;
  value |= (uint16_t)data_buffer[1];

  return value;
}

// Read a 32-bit register
uint32_t readReg32Bit(uint16_t reg, vl53l1x *sens)

{
  uint32_t value;
/*
  Wire.beginTransmission(address);
  Wire.write((reg >> 8) & 0xFF); // reg high byte
  Wire.write( reg       & 0xFF); // reg low byte
  last_status = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)4);
  value  = (uint32_t)Wire.read() << 24; // value highest byte
  value |= (uint32_t)Wire.read() << 16;
  value |= (uint16_t)Wire.read() <<  8;
  value |=           Wire.read();       // value lowest byte
*/
  uint8_t data_buffer[4];
  sens->last_status = i2c_read_Alt (VL53L1X_ADDRESS, reg, 4, data_buffer, 3);
  value = (uint32_t)data_buffer[0]<<24;
  value |= (uint32_t)data_buffer[1]<<16;
  value |= (uint32_t)data_buffer[2]<<8;
  value |= (uint32_t)data_buffer[3];

  return value;
}


// set distance mode to Short, Medium, or Long
// based on VL53L1_SetDistanceMode()
bool setDistanceMode(enum DistanceMode mode, vl53l1x *sens)
{
  // save existing timing budget
  uint32_t budget_us = getMeasurementTimingBudget(sens);


  switch (mode)
  {
    case Short:
      // from VL53L1_preset_mode_standard_ranging_short_range()

      // timing config
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x07, sens);
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x05, sens);
      writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0x38, sens);

      // dynamic config
      writeReg(SD_CONFIG__WOI_SD0, 0x07, sens);
      writeReg(SD_CONFIG__WOI_SD1, 0x05, sens);
      writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 6, sens); // tuning parm default
      writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 6,sens); // tuning parm default
      lcd_display(LCD_LINE4, "***SHORT****");
      break;

    case Medium:
      // from VL53L1_preset_mode_standard_ranging()

      // timing config
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x0B,sens);
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x09,sens);
      writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0x78,sens);

      // dynamic config
      writeReg(SD_CONFIG__WOI_SD0, 0x0B,sens);
      writeReg(SD_CONFIG__WOI_SD1, 0x09,sens);
      writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 10,sens); // tuning parm default
      writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 10,sens); // tuning parm default

      break;

    case Long: // long
      // from VL53L1_preset_mode_standard_ranging_long_range()

      // timing config
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_A, 0x0F,sens);
      writeReg(RANGE_CONFIG__VCSEL_PERIOD_B, 0x0D,sens);
      writeReg(RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8,sens);

      // dynamic config
      writeReg(SD_CONFIG__WOI_SD0, 0x0F,sens);
      writeReg(SD_CONFIG__WOI_SD1, 0x0D,sens);
      writeReg(SD_CONFIG__INITIAL_PHASE_SD0, 14,sens); // tuning parm default
      writeReg(SD_CONFIG__INITIAL_PHASE_SD1, 14,sens); // tuning parm default

      break;

    default:
      // unrecognized mode - do nothing
      return false;
  }

  // reapply timing budget
  setMeasurementTimingBudget(budget_us,sens);

  // save mode so it can be returned by getDistanceMode()
  sens->distance_mode = mode;

  return true;
}

// Set the measurement timing budget in microseconds, which is the time allowed
// for one measurement. A longer timing budget allows for more accurate
// measurements.
// based on VL53L1_SetMeasurementTimingBudgetMicroSeconds()
bool setMeasurementTimingBudget(uint32_t budget_us,vl53l1x *sens)
{

	uint32_t range_config_timeout_us = budget_us -= TimingGuard;
	uint32_t macro_period_us;
	uint32_t phasecal_timeout_mclks = timeoutMicrosecondsToMclks(1000, macro_period_us);

  // assumes PresetMode is LOWPOWER_AUTONOMOUS
  if (budget_us <= TimingGuard) { return false; }

  if (range_config_timeout_us > 1100000) { return false; } // FDA_MAX_TIMING_BUDGET_US * 2

  range_config_timeout_us /= 2;
  // VL53L1_calc_timeout_register_values() begin

  // "Update Macro Period for Range A VCSEL Period"
  macro_period_us = calcMacroPeriod(readReg(RANGE_CONFIG__VCSEL_PERIOD_A,sens),sens);

  // "Update Phase timeout - uses Timing A"
  // Timeout of 1000 is tuning parm default (TIMED_PHASECAL_CONFIG_TIMEOUT_US_DEFAULT)
  // via VL53L1_get_preset_mode_timing_cfg().

  if (phasecal_timeout_mclks > 0xFF) { phasecal_timeout_mclks = 0xFF; }
  writeReg(PHASECAL_CONFIG__TIMEOUT_MACROP, phasecal_timeout_mclks,sens);

  // "Update MM Timing A timeout"
  // Timeout of 1 is tuning parm default (LOWPOWERAUTO_MM_CONFIG_TIMEOUT_US_DEFAULT)
  // via VL53L1_get_preset_mode_timing_cfg(). With the API, the register
  // actually ends up with a slightly different value because it gets assigned,
  // retrieved, recalculated with a different macro period, and reassigned,
  // but it probably doesn't matter because it seems like the MM ("mode
  // mitigation"?) sequence steps are disabled in low power auto mode anyway.
  writeReg16Bit(MM_CONFIG__TIMEOUT_MACROP_A, encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)),sens);

  // "Update Range Timing A timeout"
  writeReg16Bit(RANGE_CONFIG__TIMEOUT_MACROP_A, encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)),sens);

  // "Update Macro Period for Range B VCSEL Period"
  macro_period_us = calcMacroPeriod(readReg(RANGE_CONFIG__VCSEL_PERIOD_B, sens), sens);

  // "Update MM Timing B timeout"
  // (See earlier comment about MM Timing A timeout.)
  writeReg16Bit(MM_CONFIG__TIMEOUT_MACROP_B, encodeTimeout(timeoutMicrosecondsToMclks(1, macro_period_us)),sens);

  // "Update Range Timing B timeout"
  writeReg16Bit(RANGE_CONFIG__TIMEOUT_MACROP_B, encodeTimeout(timeoutMicrosecondsToMclks(range_config_timeout_us, macro_period_us)),sens);

  // VL53L1_calc_timeout_register_values() end

  return true;
}

// Get the measurement timing budget in microseconds
// based on VL53L1_SetMeasurementTimingBudgetMicroSeconds()
uint32_t getMeasurementTimingBudget(vl53l1x *sens)
{
  // assumes PresetMode is LOWPOWER_AUTONOMOUS and these sequence steps are
  // enabled: VHV, PHASECAL, DSS1, RANGE

  // VL53L1_get_timeouts_us() begin

  // "Update Macro Period for Range A VCSEL Period"
  uint32_t macro_period_us = calcMacroPeriod(readReg(RANGE_CONFIG__VCSEL_PERIOD_A, sens), sens);

  // "Get Range Timing A timeout"

  uint32_t range_config_timeout_us = timeoutMclksToMicroseconds(decodeTimeout(readReg16Bit(RANGE_CONFIG__TIMEOUT_MACROP_A, sens)), macro_period_us);

  // VL53L1_get_timeouts_us() end

  return  2 * range_config_timeout_us + TimingGuard;
}

// Start continuous ranging measurements, with the given inter-measurement
// period in milliseconds determining how often the sensor takes a measurement.
bool startContinuous(uint32_t period_ms, vl53l1x *sens)
{
  // from VL53L1_set_inter_measurement_period_ms()
  uint32_t tmp = period_ms * sens->osc_calibrate_val;
  if(tmp == 0) return false;
  else{
  writeReg32Bit(SYSTEM__INTERMEASUREMENT_PERIOD, tmp,sens);
  writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01,sens); // sys_interrupt_clear_range
  writeReg(SYSTEM__MODE_START, 0x40,sens); // mode_range__timed
  return true;
  }
}

// Stop continuous measurements
// based on VL53L1_stop_range()
void stopContinuous(vl53l1x *sens)
{
  writeReg(SYSTEM__MODE_START, 0x80, sens); // mode_range__abort

  // VL53L1_low_power_auto_data_stop_range() begin

  sens->calibrated = false;

  // "restore vhv configs"
  if (sens->saved_vhv_init != 0)
  {
    writeReg(VHV_CONFIG__INIT, sens->saved_vhv_init, sens);
  }
  if (sens->saved_vhv_timeout != 0)
  {
     writeReg(VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, sens->saved_vhv_timeout, sens);
  }

  // "remove phasecal override"
  writeReg(PHASECAL_CONFIG__OVERRIDE, 0x00, sens);

  // VL53L1_low_power_auto_data_stop_range() end
}

// Returns a range reading in millimeters when continuous mode is active
// (readRangeSingleMillimeters() also calls this function after starting a
// single-shot range measurement)
uint16_t Read(vl53l1x *sens)
{
	bool blocking = true;
  if (blocking)
  {
    startTimeout(sens);
    while (!dataReady(sens))
    {
      if (checkTimeoutExpired(sens))
      {
        sens->did_timeout = true;
        sens->ranging_data.range_status = None;
        sens->ranging_data.range_mm = 0;
        sens->ranging_data.peak_signal_count_rate_MCPS = 0;
        sens->ranging_data.ambient_count_rate_MCPS = 0;
        return sens->ranging_data.range_mm;
      }
    }
  }

  readResults(sens);

  if (!sens->calibrated)
  {
    setupManualCalibration(sens);
    sens->calibrated = true;
  }

  updateDSS(sens);

  getRangingData(sens);

  writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01, sens); // sys_interrupt_clear_range

  return sens->ranging_data.range_mm;
}

// convert a RangeStatus to a readable string
// Note that on an AVR, these strings are stored in RAM (dynamic memory), which
// makes working with them easier but uses up 200+ bytes of RAM (many AVR-based
// Arduinos only have about 2000 bytes of RAM). You can avoid this memory usage
// if you do not call this function in your sketch.
const char * rangeStatusToString(uint8_t status )
{
  switch (status)
  {
    case RangeValid:
      return "range valid";

    case SigmaFail:
      return "sigma fail";

    case SignalFail:
      return "signal fail";

    case RangeValidMinRangeClipped:
      return "range valid, min range clipped";

    case OutOfBoundsFail:
      return "out of bounds fail";

    case HardwareFail:
      return "hardware fail";

    case RangeValidNoWrapCheckFail:
      return "range valid, no wrap check fail";

    case WrapTargetFail:
      return "wrap target fail";

    case XtalkSignalFail:
      return "xtalk signal fail";

    case SynchronizationInt:
      return "synchronization int";

    case MinRangeFail:
      return "min range fail";

    case None:
      return "no update";

    default:
      return "unknown status";
  }
}

// Did a timeout occur in one of the read functions since the last call to
// timeoutOccurred()?
bool timeoutOccurred(vl53l1x *sens)
{
  bool tmp = sens->did_timeout;
  sens->did_timeout = false;
  return tmp;
}

// Private Methods /////////////////////////////////////////////////////////////

// "Setup ranges after the first one in low power auto mode by turning off
// FW calibration steps and programming static values"
// based on VL53L1_low_power_auto_setup_manual_calibration()
void setupManualCalibration(vl53l1x *sens)
{
  // "save original vhv configs"
  sens->saved_vhv_init = readReg(VHV_CONFIG__INIT, sens);
  sens->saved_vhv_timeout = readReg(VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, sens);

  // "disable VHV init"
  writeReg(VHV_CONFIG__INIT, sens->saved_vhv_init & 0x7F,sens);

  // "set loop bound to tuning param"
  writeReg(VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,(sens->saved_vhv_timeout & 0x03) + (3 << 2),sens); // tuning parm default (LOWPOWERAUTO_VHV_LOOP_BOUND_DEFAULT)

  // "override phasecal"
  writeReg(PHASECAL_CONFIG__OVERRIDE, 0x01,sens);
  writeReg(CAL_CONFIG__VCSEL_START, readReg(PHASECAL_RESULT__VCSEL_START,sens),sens);
}

// read measurement results into buffer
void readResults(vl53l1x* sens)
{
	/*
  Wire.beginTransmission(address);
  Wire.write((RESULT__RANGE_STATUS >> 8) & 0xFF); // reg high byte
  Wire.write( RESULT__RANGE_STATUS       & 0xFF); // reg low byte
  last_status = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)17);

  results.range_status = Wire.read();

  Wire.read(); // report_status: not used

  results.stream_count = Wire.read();

  results.dss_actual_effective_spads_sd0  = (uint16_t)Wire.read() << 8; // high byte
  results.dss_actual_effective_spads_sd0 |=           Wire.read();      // low byte

  Wire.read(); // peak_signal_count_rate_mcps_sd0: not used
  Wire.read();

  results.ambient_count_rate_mcps_sd0  = (uint16_t)Wire.read() << 8; // high byte
  results.ambient_count_rate_mcps_sd0 |=           Wire.read();      // low byte

  Wire.read(); // sigma_sd0: not used
  Wire.read();

  Wire.read(); // phase_sd0: not used
  Wire.read();

  results.final_crosstalk_corrected_range_mm_sd0  = (uint16_t)Wire.read() << 8; // high byte
  results.final_crosstalk_corrected_range_mm_sd0 |=           Wire.read();      // low byte

  results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0  = (uint16_t)Wire.read() << 8; // high byte
  results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 |=           Wire.read();      // low byte
*/

	/*uint8_t data_buffer[17];
	sens->last_status = i2c_read(VL53L1X_ADDRESS, RESULT__RANGE_STATUS, 18, data_buffer, 3);
	sens->results.range_status = data_buffer[0];

	//report_status not used

	sens->results.stream_count = data_buffer[2];

	sens->results.dss_actual_effective_spads_sd0  = (uint16_t)data_buffer[3] << 8;
	sens->results.dss_actual_effective_spads_sd0 |= (uint16_t)data_buffer[4];

	//peak_signal_count_rate_mcps_sd0_high not used
	//peak_signal_count_rate_mcps_sd0_low not used

	sens->results.ambient_count_rate_mcps_sd0  = (uint16_t)data_buffer[7] << 8; // high byte
	sens->results.ambient_count_rate_mcps_sd0 |= (uint16_t)data_buffer[8]; // low byte

	// sigma_sd0_high not used
	// sigma_sd0_high not used

	//phase_sd0_high not used
	//phase_sd0_low not used

	sens->results.final_crosstalk_corrected_range_mm_sd0  = (uint16_t)data_buffer[13] << 8; // high byte
	sens->results.final_crosstalk_corrected_range_mm_sd0 |= (uint16_t)data_buffer[14]; // low byte

	sens->results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0  = (uint16_t)data_buffer[15] << 8; // high byte
	sens->results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 |= (uint16_t)data_buffer[16];// low byte  */

	/* *****  DA QUI IN POI E' UNA PROVA ********/
		sens->results.range_status = readReg(RESULT__RANGE_STATUS,sens);

		sens->results.stream_count = readReg(RESULT__STREAM_COUNT,sens);

		sens->results.dss_actual_effective_spads_sd0  = readReg16Bit(RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0,sens);

		sens->results.ambient_count_rate_mcps_sd0 = readReg16Bit(RESULT__AMBIENT_COUNT_RATE_MCPS_SD0,sens);

		sens->results.final_crosstalk_corrected_range_mm_sd0 = readReg16Bit(RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0,sens);

		sens->results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 = readReg16Bit(RESULT__PEAK_SIGNAL_COUNT_RATE_CROSSTALK_CORRECTED_MCPS_SD0,sens);


	/* ****** FINE PROVA ***** */

}


// perform Dynamic SPAD Selection calculation/update
// based on VL53L1_low_power_auto_update_DSS()
void updateDSS(vl53l1x *sens)
{
  uint16_t spadCount = sens->results.dss_actual_effective_spads_sd0;

  if (spadCount != 0)
  {
    // "Calc total rate per spad"

    uint32_t totalRatePerSpad = (uint32_t)sens->results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 + sens->results.ambient_count_rate_mcps_sd0;

    // "clip to 16 bits"
    if (totalRatePerSpad > 0xFFFF) { totalRatePerSpad = 0xFFFF; }

    // "shift up to take advantage of 32 bits"
    totalRatePerSpad <<= 16;

    totalRatePerSpad /= spadCount;

    if (totalRatePerSpad != 0)
    {
      // "get the target rate and shift up by 16"
      uint32_t requiredSpads = ((uint32_t)TargetRate << 16) / totalRatePerSpad;

      // "clip to 16 bit"
      if (requiredSpads > 0xFFFF) { requiredSpads = 0xFFFF; }

      // "override DSS config"
      writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, requiredSpads, sens);
      // DSS_CONFIG__ROI_MODE_CONTROL should already be set to REQUESTED_EFFFECTIVE_SPADS

      return;
    }
  }

  // If we reached this point, it means something above would have resulted in a
  // divide by zero.
  // "We want to gracefully set a spad target, not just exit with an error"

   // "set target to mid point"
   writeReg16Bit(DSS_CONFIG__MANUAL_EFFECTIVE_SPADS_SELECT, 0x8000,sens);
}

// get range, status, rates from results buffer
// based on VL53L1_GetRangingMeasurementData()
void getRangingData(vl53l1x *sens)
{
  // VL53L1_copy_sys_and_core_results_to_range_results() begin

  uint16_t range = sens->results.final_crosstalk_corrected_range_mm_sd0;

  // "apply correction gain"
  // gain factor of 2011 is tuning parm default (VL53L1_TUNINGPARM_LITE_RANGING_GAIN_FACTOR_DEFAULT)
  // Basically, this appears to scale the result by 2011/2048, or about 98%
  // (with the 1024 added for proper rounding).
  sens->ranging_data.range_mm = ((uint32_t)range * 2011 + 0x0400) / 0x0800;

  // VL53L1_copy_sys_and_core_results_to_range_results() end

  // set range_status in ranging_data based on value of RESULT__RANGE_STATUS register
  // mostly based on ConvertStatusLite()
  switch(sens->results.range_status)
  {
    case 17: // MULTCLIPFAIL
    case 2: // VCSELWATCHDOGTESTFAILURE
    case 1: // VCSELCONTINUITYTESTFAILURE
    case 3: // NOVHVVALUEFOUND
      // from SetSimpleData()
      sens->ranging_data.range_status = HardwareFail;
      break;

    case 13: // USERROICLIP
     // from SetSimpleData()
    	sens->ranging_data.range_status = MinRangeFail;
      break;

    case 18: // GPHSTREAMCOUNT0READY
    	sens->ranging_data.range_status = SynchronizationInt;
      break;

    case 5: // RANGEPHASECHECK
    	sens->ranging_data.range_status =  OutOfBoundsFail;
      break;

    case 4: // MSRCNOTARGET
    	sens->ranging_data.range_status = SignalFail;
      break;

    case 6: // SIGMATHRESHOLDCHECK
    	sens->ranging_data.range_status = SignalFail;
      break;

    case 7: // PHASECONSISTENCY
    	sens->ranging_data.range_status = WrapTargetFail;
      break;

    case 12: // RANGEIGNORETHRESHOLD
    	sens->ranging_data.range_status = XtalkSignalFail;
      break;

    case 8: // MINCLIP
    	sens->ranging_data.range_status = RangeValidMinRangeClipped;
      break;

    case 9: // RANGECOMPLETE
      // from VL53L1_copy_sys_and_core_results_to_range_results()
      if (sens->results.stream_count == 0)
      {
    	  sens->ranging_data.range_status = RangeValidNoWrapCheckFail;
      }
      else
      {
    	  sens->ranging_data.range_status = RangeValid;
      }
      break;

    default:
    	sens->ranging_data.range_status = None;
  }

  // from SetSimpleData()
  sens->ranging_data.peak_signal_count_rate_MCPS = countRateFixedToFloat(sens->results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0);
  sens->ranging_data.ambient_count_rate_MCPS = countRateFixedToFloat(sens->results.ambient_count_rate_mcps_sd0);
}

// Decode sequence step timeout in MCLKs from register value
// based on VL53L1_decode_timeout()
uint32_t decodeTimeout(uint16_t reg_val)
{
  return ((uint32_t)(reg_val & 0xFF) << (reg_val >> 8)) + 1;
}

// Encode sequence step timeout register value from timeout in MCLKs
// based on VL53L1_encode_timeout()
uint16_t encodeTimeout(uint32_t timeout_mclks)
{
  // encoded format: "(LSByte * 2^MSByte) + 1"

  uint32_t ls_byte = 0;
  uint16_t ms_byte = 0;

  if (timeout_mclks > 0)
  {
    ls_byte = timeout_mclks - 1;

    while ((ls_byte & 0xFFFFFF00) > 0)
    {
      ls_byte >>= 1;
      ms_byte++;
    }

    return (ms_byte << 8) | (ls_byte & 0xFF);
  }
  else { return 0; }
}

// Convert sequence step timeout from macro periods to microseconds with given
// macro period in microseconds (12.12 format)
// based on VL53L1_calc_timeout_us()
uint32_t timeoutMclksToMicroseconds(uint32_t timeout_mclks, uint32_t macro_period_us)
{
  return ((uint64_t)timeout_mclks * macro_period_us + 0x800) >> 12;
}

// Convert sequence step timeout from microseconds to macro periods with given
// macro period in microseconds (12.12 format)
// based on VL53L1_calc_timeout_mclks()
uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_us, uint32_t macro_period_us)
{
  return (((uint32_t)timeout_us << 12) + (macro_period_us >> 1)) / macro_period_us;
}

// Calculate macro period in microseconds (12.12 format) with given VCSEL period
// assumes fast_osc_frequency has been read and stored
// based on VL53L1_calc_macro_period_us()
uint32_t calcMacroPeriod(uint8_t vcsel_period, vl53l1x *sens)
{
  // from VL53L1_calc_pll_period_us()
  // fast osc frequency in 4.12 format; PLL period in 0.24 format
  uint32_t pll_period_us = ((uint32_t)0x01 << 30) / sens->fast_osc_frequency;

  // from VL53L1_decode_vcsel_period()
  uint8_t vcsel_period_pclks = (vcsel_period + 1) << 1;

  // VL53L1_MACRO_PERIOD_VCSEL_PERIODS = 2304
  uint32_t macro_period_us = (uint32_t)2304 * pll_period_us;
  macro_period_us >>= 6;
  macro_period_us *= vcsel_period_pclks;
  macro_period_us >>= 6;

  return macro_period_us;
}

#endif
