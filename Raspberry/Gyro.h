//richiede le librerie -lm I2Cdev.o MPU6050.o

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>
#include <iostream>

#define P_GRECO 3.1415926535

struct X_Y_rotationValues
{
	 double x;
	 double y;
};
struct roll_pitch_gyro
{
	double gyro_roll;
	double gyro_pitch;
};

class Gyro {
private:
	MPU6050 accelgyro; // chip montato sul Gy-521

	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t xAdjustment =-1 , yAdjustment = -0.085;
	float gyro_roll_input, gyro_pitch_input;
	double scaled_ax, scaled_ay, scaled_az;

	double dist(double a, double b) {return sqrt((a*a) + (b*b));}

	double get_y_rotation(double x, double y, double z) {
		double radians = atan2(x, dist(y, z));
		return (180 * radians / P_GRECO);
	}
	double get_x_rotation(double x, double y, double z) {
		double radians = atan2(y, dist(x, z));
		return (180 * radians / P_GRECO);
	}

public:
	Gyro();

	int16_t* getAG_Motion() {
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

		return new int16_t[6]{ ax, ay, az, gx, gy, gz };
	}	

	void get_roll_pitch(roll_pitch_gyro & roll_pitch_gyro) {
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		
	
		roll_pitch_gyro.gyro_roll = (gyro_roll_input * 0.8) + (((gx - xAdjustment) / 57.14286) * 0.2);
		roll_pitch_gyro.gyro_pitch = (gyro_pitch_input * 0.8) + (((gy - yAdjustment) / 57.14286) * 0.2);
	}

	void get_X_Y_rotation(X_Y_rotationValues & X_Y_rotationValues) {
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		scaled_ax = ax / 16384.0; scaled_ay = ay / 16384.0; scaled_az = az / 16384.0;

		X_Y_rotationValues.x = get_x_rotation(scaled_ax, scaled_ay, scaled_az) / 57.14286;
		X_Y_rotationValues.y = get_y_rotation(scaled_ax, scaled_ay, scaled_az) / 57.14286;

	}
};

Gyro::Gyro() {
	std::cout << accelgyro.testConnection() << std::endl;
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	xAdjustment = gx;
	yAdjustment = gy;
}
