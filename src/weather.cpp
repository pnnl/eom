#include "weather.hpp"

double Temperature::fahrenheit() const { return fahrenheit_; }
double Temperature::celsius() const { return celsius_; }
Temperature Temperature::Fahrenheit(double fahrenheit) { return Temperature((fahrenheit-32)*5/9., fahrenheit); }
Temperature Temperature::Celsius(double celsius) { return Temperature(celsius, celsius*9/5.+32); }
Temperature::Temperature(double celsius, double fahrenheit) : celsius_(celsius), fahrenheit_(fahrenheit) { }

double Pressure::pascals() const { return pascals_; }
Pressure Pressure::Pascals(double pascals) { return Pressure(pascals); }
Pressure::Pressure(double pascals) : pascals_(pascals) { }

const Temperature & Weather::temperature(void) const { return temperature_; }
const Pressure & Weather::pressure(void) const { return pressure_; }
Weather::Weather(Temperature temperature, Pressure pressure) : temperature_(temperature), pressure_(pressure) { }

WeatherSeries::~WeatherSeries() { }
