#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

struct pid_gain_roll
{
	double P;
	double I;
	double D;
};

struct pid_gain_Pitch
{
	double P;
	double I;
	double D;
};


class PID
{
	private:
		double pid_p_gain_roll;               
		double pid_i_gain_roll;			
		double pid_d_gain_roll;					

		double pid_p_gain_pitch;               
		double pid_i_gain_pitch;	
		double pid_d_gain_pitch;	

		double gyro_roll_input = 0, gyro_pitch_input = 0;
		double pid_error_temp = 0;
		double pid_i_mem_roll = 0, pid_i_mem_pitch = 0;
		double pid_output_roll = 0, pid_output_pitch = 0;
		double pid_last_d_error = 0, pid_last_pitch_d_error = 0;

    public:
	
		void setUpPidValues(pid_gain_roll roll_Gains, pid_gain_Pitch pitch_Gains);

		double CalcPitchPID(double Y_Pitch); 
		double CalcRollPID(double X_Roll);
};


void PID::setUpPidValues(pid_gain_roll roll_Gains, pid_gain_Pitch pitch_Gains) {
	pid_p_gain_roll = roll_Gains.P;
	pid_i_gain_roll = roll_Gains.I;
	pid_d_gain_roll = roll_Gains.D;

	pid_p_gain_pitch = pitch_Gains.P;
	pid_i_gain_pitch = pitch_Gains.I;
	pid_d_gain_pitch = pitch_Gains.D;
}

double PID::CalcPitchPID(double Y_Pitch) {
	gyro_pitch_input = Y_Pitch; 
							  // inizio a calcolare l errore tra la richiesta e il dovuto
	pid_error_temp = gyro_pitch_input - 0; // il secondo e il radiocomando

	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	//forse qui con un if devo limitare questi calcoli ad un max di 400 ?
	pid_output_pitch = (double)pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);

	pid_last_pitch_d_error = pid_error_temp;

	return	pid_output_pitch;
}
double PID::CalcRollPID(double X_Roll) {
	
	gyro_roll_input = X_Roll; 
							// inizio a calcolare l errore tra la richiesta e il dovuto
	pid_error_temp = gyro_roll_input - 0; // il secondo e il radiocomando

	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	//forse qui con un if devo limitare questi calcoli ad un max di 400 ?
	pid_output_roll = (double)pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_d_error);

	pid_last_d_error = pid_error_temp;

	return pid_output_roll;
}
