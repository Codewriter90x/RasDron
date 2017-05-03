/*
* MotorsController.h
*/

#include <thread>
#include <string>
#include <mutex>
#include <unistd.h>	
#include "PCA9685.h"

#define steps 1

class MotorsController
{
private:
	PCA9685 pwm; //chip montato sulla board adafruit

	double INI_SPEED;
	double MAX_SPEED;
	double OFF_SPEED;

	int IO_ALPHA, IO_BRAVO, IO_CHARLIE, IO_DELTA;
	double MotorASpeed, MotorBSpeed, MotorCSpeed, MotorDSpeed; // variabile globale per passare la velocitá al sub thread

	void MotorA(); void MotorB(); void MotorC(); void MotorD();

public:
	MotorsController(double INI_SPEED, double MAX_SPEED, double OFF_SPEED, int MotorsPins[4], double MotorStartSpeed[4]);    // costruttore
	~MotorsController();   // il distruttore

	void ChangeSingularMotorSpeed(int motorNum, double speed) {
		switch (motorNum)
		{
		case 1:
			MotorASpeed = speed;
			break;
		case 2:
			MotorBSpeed = speed;
			break;
		case 3:
			MotorCSpeed = speed;
			break;
		case 4:
			MotorDSpeed = speed;
			break;
		default:
			break;
		}
	}
	void changeAllMotorsSpeed(double speed) {
		MotorASpeed = speed;
		MotorBSpeed = speed;
		MotorCSpeed = speed;
		MotorDSpeed = speed;
	}
	void changeAllMotorsSpeed(double MotorSpeed[4]) {
		MotorASpeed = MotorSpeed[0];
		MotorBSpeed = MotorSpeed[1];
		MotorCSpeed = MotorSpeed[2];
		MotorDSpeed = MotorSpeed[3];
	}
	void turnOFFMotors();

};

// Creo il costruttore
MotorsController::MotorsController(double ini_speed, double max_speed, double OFF_SPEED, int MotorsPins[4], double MotorStartSpeed[4]) {

	//importo i valori passati attraverso il costruttore dei vari parametri x l'utilizzo dei motori
	INI_SPEED = ini_speed;
	MAX_SPEED = max_speed;
	IO_ALPHA = MotorsPins[0]; IO_BRAVO = MotorsPins[1]; IO_CHARLIE = MotorsPins[2]; IO_DELTA = MotorsPins[3];
	MotorASpeed = MotorStartSpeed[0]; MotorBSpeed = MotorStartSpeed[1]; MotorCSpeed = MotorStartSpeed[2]; MotorDSpeed = MotorStartSpeed[3];

	// inizializzo la scheda dei motori ....
	pwm.init(1, 0x40);
	usleep(1000 * 100);
	pwm.setPWMFreq(50); //setto la frequenza dei motori
	usleep(1000 * 100);

	for (double speed = 0; speed < INI_SPEED; speed = speed + 5) {
		pwm.setPWM(IO_ALPHA, 0, speed);
		pwm.setPWM(IO_BRAVO, 0, speed);
		pwm.setPWM(IO_CHARLIE, 0, speed);
		pwm.setPWM(IO_DELTA, 0, speed);
		usleep(1000 * 100);
	}

	//avvio i vari motori in sub thread
	std::thread t1(&MotorsController::MotorA, this);
	std::thread t2(&MotorsController::MotorB, this);
	std::thread t3(&MotorsController::MotorC, this);
	std::thread t4(&MotorsController::MotorD, this);

	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();
}
MotorsController::~MotorsController() {
	usleep(80000);
}
void MotorsController::MotorA() 
{
	while (true)
	{
		pwm.setPWM(IO_ALPHA, 0, MotorASpeed);
	}
}
void MotorsController::MotorB()
{
	while (true)
	{
		pwm.setPWM(IO_BRAVO, 0, MotorBSpeed);
	}
}
void MotorsController::MotorC()
{
	while (true)
	{
		pwm.setPWM(IO_CHARLIE, 0, MotorCSpeed);
	}
}
void MotorsController::MotorD()
{
	while (true)
	{
		pwm.setPWM(IO_DELTA, 0, MotorDSpeed);
	}
}
void MotorsController::turnOFFMotors() {
	MotorASpeed = OFF_SPEED; MotorBSpeed = OFF_SPEED; MotorCSpeed = OFF_SPEED; MotorDSpeed = OFF_SPEED;
	usleep(150000);
}

