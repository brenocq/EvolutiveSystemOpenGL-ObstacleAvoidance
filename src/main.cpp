#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Classes/manager.h"

using namespace std;

// Global variables
Manager manager ;
char fileName[30];
ofstream outputFile;

// Manager state (used to print to file)
int lastEnvironment = -1;
int lastRepetition = -1;
int lastPopulation = 0;

void draw();
void timer(int);
//----- Data generation -----//
void setFileName(string name);
void writeHeaderFile();

int main(int argc, char** argv){
  srand(time(0));

  if(argc>1){
    string input(argv[1]);
    setFileName(input);
  }else{
    setFileName("temp.txt");
  }
   writeHeaderFile();// Write header of output file
  //----- Create window -----//
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition((screenWidth/2)-(windowWidth/2), (screenHeight/2)-(windowHeight/2));
  glutCreateWindow("Obstacle Avoidance Simulation");
  glClearColor(1.0, 1.0, 1.0, 1.0);// Clear window
  glutDisplayFunc(draw);// Set the draw function
  glutTimerFunc(0, timer, 0);// Define timer function (loop function)
  glutMainLoop();

  return 0;
}

void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  for (int i = 0; i < qtdEnvironments; i++) {
    manager.draw();
  }
  glutSwapBuffers();
}

void timer(int){
  for(int rep=0;rep<displaySteps;rep++){
    for (int i = 0; i < qtdEnvironments; i++) {
      manager.updateEnvironment(0.0200);// Update as 200ms
      if(manager.getCurrEnvironment()!=lastEnvironment){
        lastEnvironment = manager.getCurrEnvironment();
        outputFile.open(fileName, ios::out | ios::app);
        outputFile<< "Environment "<< lastEnvironment;
        outputFile<< "\t h: " << manager.environment[lastEnvironment].genes[0];
        outputFile<< "\t mr: " << manager.environment[lastEnvironment].genes[1];
        outputFile<< "\t nc: " << manager.environment[lastEnvironment].genes[2];
        outputFile<< "\t nm: " << manager.environment[lastEnvironment].genes[3];
        outputFile<< "\t bmp: " << manager.environment[lastEnvironment].genes[4];
        outputFile<< "\t cc: " << manager.environment[lastEnvironment].genes[5];
        outputFile<< "\n";
        outputFile.close();
      }
      if(manager.getCurrRepetition()!=lastRepetition){
        lastRepetition = manager.getCurrRepetition();
        outputFile.open(fileName, ios::out | ios::app);
        outputFile<<"\tRepetition "<< lastRepetition <<"\n";
        outputFile.close();
      }
      if(manager.environment[manager.getCurrEnvironment()].getFinished()){
        outputFile.open(fileName, ios::out | ios::app);
        outputFile<< "\tfinalFitness: "<< manager.environment[lastEnvironment].fitness.back();
        outputFile<< "\n";
        outputFile.close();
      }
      if(manager.environment[lastEnvironment].getCurrentPopulation()!=lastPopulation){
        lastPopulation = manager.environment[lastEnvironment].getCurrentPopulation();
        outputFile.open(fileName, ios::out | ios::app);

        if(lastPopulation!=0){

          outputFile<<"\t\tPopulation "<< lastPopulation-1 <<"\n";
          for (int i = 0; i < qtdRobots; i++) {
            Robot *robot = &manager.environment[lastEnvironment].robot[i];

            outputFile<<"\t\t\tRobot "<< i;
            outputFile<<fixed<<setprecision(2);
            outputFile<<" \tf: "  << robot->fitness[robot->fitness.size()-2];
            //outputFile<<" \tmf: " << robot->meanFitness[robot->meanFitness.size()-1];// Core Dumped

            if(printEverything){
              int i=0;
              for(float value : robot->genesAnatomy){
                outputFile<<" \tga"<<i<<": " << value;
                i++;
              }
              i=0;
              for(float value : robot->genesBrain){
                outputFile<<" \tgb"<<i<<": " << value;
                i++;
              }
            }
            outputFile<<"\n";

          }

        }
        outputFile.close();
      }
    }
  }

  glutPostRedisplay();

  glutTimerFunc(1000/60, timer, 0);// Call timer function as fast as possible
}

//----- Data generation -----//
void setFileName(string name){
  string fileNameStr;
  if(name.find(".txt")!= string::npos){
    fileNameStr = "Data/"+name;
  }else{
    fileNameStr = "Data/"+name+".txt";
  }
  fileNameStr.copy(fileName, fileNameStr.size()+1);
  cout << "This simulation will be saved to: " << fileName<<endl;
  // Clean file
  outputFile.open(fileName);
  outputFile.close();
}

void writeHeaderFile(){
  outputFile.open(fileName, ios::out | ios::app);
  outputFile<<"QtdRobots: " << qtdRobots << endl;
  outputFile<<"QtdEnvironments: " << qtdEnvironments << endl;
  outputFile<<"QtdRepetitions: " << qtdRepetitions << endl;
  outputFile<<"QtdObstacles: " << qtdObstacles << endl;
  outputFile<<"PointsCollision: " << pointsCollision << endl;
  outputFile<<"PointsMoving: " << pointsMoving << endl;
  outputFile<<"MaxPopulation: " << maxPopulation << endl;
  outputFile<<"MaxTime: " << maxTime << endl;
  outputFile<<"EnvMutationRate: " << envMutationRate << endl << endl;
  outputFile.close();
}
