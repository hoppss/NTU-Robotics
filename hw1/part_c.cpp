#include "Aria.h"

typedef enum __Closeness {STOP, SLOWDOWN, REGULAR} Closeness;

Closeness checkObstacle(ArRobot *robot);

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
	int speed = 500;
	robot.setVel(speed);

	keyHandler.addKeyHandler('w', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setHeading, 0));
	keyHandler.addKeyHandler('a', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setHeading, 90));
	keyHandler.addKeyHandler('s', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setHeading, 180));
	keyHandler.addKeyHandler('d', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setHeading, -90));
	keyHandler.addKeyHandler('h', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setVel, 0));
	keyHandler.addKeyHandler('z', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setVel, 500));
	keyHandler.addKeyHandler('x', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setVel, 50));
	keyHandler.addKeyHandler('c', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setRotVel, 50));
	keyHandler.addKeyHandler('v', new ArFunctor1C<ArRobot, double>(robot, &ArRobot::setRotVel, 0));
	
	// 3. Unlock the robot
	robot.unlock();

	// 4. Sleep a while and let the robot move
	while(true){
		printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		if(checkObstacle(&robot) == STOP)
			robot.setVel(0);
		else if(checkObstacle(&robot) == SLOWDOWN)
			robot.setVel(50);
		else
			robot.setVel(speed);
		ArUtil::sleep(300);
	}

	// End of controling


	Aria::shutdown();

	Aria::exit(0);
}

Closeness checkObstacle(ArRobot *robot) 
{
	int i;
	ArSensorReading *sonarReading;
	
	/* slow down or halt if close to obstacles */
	for (i=1; i<7; i++) {
		sonarReading = robot->getSonarReading(i);
		if (sonarReading->getRange() < 500)
			return STOP;
		else if (sonarReading->getRange() < 1000)
			return SLOWDOWN;
	//	printf("Sonar reading %d = %d\n", i, sonarReading->getRange());
	}

	return REGULAR;
}

