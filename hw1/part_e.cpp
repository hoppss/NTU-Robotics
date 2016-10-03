#include "Aria.h"
#include <math.h>

void adjustPose(ArRobot *robot, double tarX, double tarY, double tarTheta);

typedef enum __SPEED {SLOW = 1, MEDIUM = 25, FAST = 100} SPEED;

#define CLOSE 1000.0
#define FAR 50000.0

int main(int argc, char **argv)
{
	ArRobot robot;
	ArSonarDevice sonar;

	robot.addRangeDevice(&sonar);

	Aria::init();
	
	ArSimpleConnector connector(&argc,argv);

	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);
	
	// Enter target pose by standard input
	double x, y, theta;
	printf("Enter target pose >> x y theta\n >> ");
	scanf("%lf %lf %lf", &x, &y, &theta);
	printf("%lf %lf %lf", x, y, theta);
	
	robot.runAsync(false);

  	// Used to perform actions when keyboard keys are pressed
  	ArKeyHandler keyHandler;
  	Aria::setKeyHandler(&keyHandler);

 	// ArRobot contains an exit action for the Escape key. It also 
  	// stores a pointer to the keyhandler so that other parts of the program can
  	// use the same keyhandler.
  	robot.attachKeyHandler(&keyHandler);
  	printf("You may press escape to exit\n");

	// Start of controling
	
	// 1. Lock the robot
	robot.lock();

	// 2. Write your control code here, 
	//    e.g. robot.setVel(150);  

	// 3. Unlock the robot
	robot.unlock();

	// 4. Sleep a while and let the robot move
	while(true){
		printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		adjustPose(&robot, x, y, theta);	
		ArUtil::sleep(30);
	}

	// End of controling

	Aria::shutdown();

	Aria::exit(0);
}

void adjustPose(ArRobot *robot, double tarX, double tarY, double tarTheta)
{
	double curX = robot->getX(), curY =robot->getY();
	double orientation, distance;
	SPEED speed;

	distance = (tarX - curX) * (tarX - curX) + (tarY - curY) * (tarY - curY);

	if (distance < CLOSE)
		speed = SLOW;
	else if (distance < FAR)
		speed = MEDIUM;
	else
		speed = FAST;

	if (curX == tarX) {
		if (curY == tarY) {	
			robot->setVel(0);
			robot->setHeading(tarTheta * 180 / M_PI);;
		}
		else {
			if(tarY > curY) {
				robot->setVel(speed);
				robot->setHeading(90);
			}
			else {
				robot->setVel(speed);
				robot->setHeading(270);
			}
		}
		return;
	}
	
	orientation = atan((tarY - curY) / (tarX - curX)) * 180 / M_PI;
	
	if (tarX > curX) 
		robot->setHeading(orientation);
	else
		robot->setHeading(orientation + 180);
	
	robot->setVel(speed);
}
