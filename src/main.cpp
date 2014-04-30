#include "headers.h"
#include "diver/SeaDiver.h"
#include "diver/Pipe.h"
#include "terrain.h"
#include "SeaDiver.h"
#include "Pipe.h"
#include "Rectangle.h"
#include "log.h"
#include "program.h"
#include "globals.h"
#include "cube.h"
#include "cudaUtils.h"
#include "texture.h"
#include "renderRoot.h"
#include "particleGroup.h"
#include "rand.h"
#include "waves.h"
#include "pousseeArchimede.h"
#include "constantForce.h"
#include "constantMassForce.h"
#include "frottementFluide.h"
#include "frottementFluideAvance.h"
#include "dynamicScheme.h"
#include "seaFlow.h"
#include "attractor.h"
#include "springsSystem.h"
#include "audible.h"
#include "splines/CardinalSpline.h"
#include "skybox.h"
#include "ObjLoader.h"
#include "object.h"
#include "marchingCubes.h"

#include <qapplication.h>
#include <QWidget>
#include <vector>
#include <ctime>

#include <ostream>
#include <cassert>
#include <sstream>

using namespace std;
using namespace log4cpp;

int main(int argc, char** argv) {
   
	//random
	srand(time(NULL));

	//logs
    log4cpp::initLogs();
	
	//cuda
    CudaUtils::logCudaDevices(log_console);

    log_console.infoStream() << "[Rand Init] ";
    log_console.infoStream() << "[Logs Init] ";

    // glut initialisation (mandatory) 
    glutInit(&argc, argv);
    log_console.infoStream() << "[Glut Init] ";

    // ObjLoader
    //ObjLoader *cube = new ObjLoader("obj_files/cube2");
    //ObjLoader *mouette = new ObjLoader("obj_files/Sea_Gul/SEAGUL", "obj_files/Sea_Gul/");
    //ObjLoader *shark = new ObjLoader("obj_files/White_Shark/wshark", "obj_files/White_Shark/");

    // Read command lines arguments.
    QApplication application(argc,argv);
    log_console.infoStream() << "[Qt Init] ";

    // Instantiate the viewer (mandatory)
    Viewer *viewer = new Viewer();
    viewer->setWindowTitle("Sea diver");
    viewer->show();
	Globals::viewer = viewer;

    //glew initialisation (mandatory)
    log_console.infoStream() << "[Glew Init] " << glewGetErrorString(glewInit());
		
	//openal 
	Audible::initOpenALContext();
	alutInit(&argc, argv);
	log_console.infoStream() << "[Alut Init] ";

	//global vars
    Globals::init();
    Globals::print(std::cout);
    Globals::check();
	
	//texture manager
    Texture::init();

    log_console.infoStream() << "Running with OpenGL " << Globals::glVersion << " and glsl version " << Globals::glShadingLanguageVersion << " !";
	//FIN INIT//
	
	RenderRoot *root = new RenderRoot(); 
	
	//Skybox
	//Note sur la pack 'SkyboxSet1' il faut retourner les textures selon l'horizontale a cause du convertToGLFormat
	Skybox *skybox = new Skybox("textures/skybox/SkyboxSet1/SunSet/", 
			"SunSetRight2048.png SunSetLeft2048.png SunSetUp2048.png SunSetDown2048.png SunSetBack2048.png SunSetFront2048.png",
			"png");
	skybox->scale(50);
	root->addChild("skybox", skybox);
	
	//Waves
	Waves *waves = new Waves(0.0,0.0,100.0,100.0, 10.0, skybox->getCubeMap());
	waves->translate(0,0,0);
	root->addChild("vagues", waves);
	
	//Terrain
	MarchingCubes *terrain = new MarchingCubes(128,128,128,100.0f/127);
	terrain->translate(-50,-65,-50);
	root->addChild("terrain", terrain);

	
	//Terrain2
	//Terrain *terrain = new Terrain(black_img, rgb_heightmap.width(), rgb_heightmap.height(), true); 
	//terrain->rotate(qglviewer::Quaternion(qglviewer::Vec(1,0,0), 3.14/2)); 
	//root->addChild("terrain", terrain);
	    
	// Diver
	//SeaDiver *diver = new SeaDiver(); 
	//root->addChild("diver", diver); 
	
    //ObjLoader test
    /*vector<Object*> vecc = cube->getObjects();
    for (unsigned int i = 0; i < vecc.size(); i++) {
        log_console.infoStream() << "Adding child: cube2_" << i;
        stringstream s;
        s << "cube2_" << i;
        root->addChild(s.str(), vecc[i]);
    }*/
    //vector<Object*> vecm = mouette->getObjects();
    //for (unsigned int i = 0; i < vecm.size(); i++) {
        //log_console.infoStream() << "Adding child: mouette_" << i;
        //stringstream s;
        //s << "mouette_" << i;
        //root->addChild(s.str(), vecm[i]);
        //vecm[i]->scale(10.0);
        //vecm[i]->translate(0.0,50.0,0.0);
    //}
    /*vector<Object*> vecs = shark->getObjects();
    for (unsigned int i = 0; i < vecs.size(); i++) {
        log_console.infoStream() << "Adding child: shark_" << i;
        stringstream s;
        s << "shark_" << i;
        root->addChild(s.str(), vecs[i]);
        vecs[i]->scale(0.01);
        vecs[i]->translate(20.0,0.0,0.0);
    }*/
    
    // Pipe
    // TODO : put this in Dimensions
    /* std::vector<Vec> tmp; */
    /* tmp.push_back(Vec(PIPE_FIXED_PART_X, PIPE_FIXED_PART_Y, PIPE_FIXED_PART_Z)); */
    /* tmp.push_back(Vec(0, 2, 4)); */
    /* tmp.push_back(Vec(0, 1, 1)); */
    /* tmp.push_back(Vec(0, 0, 0)); */

    /* Pipe *pipe = new Pipe(tmp); */
    /* tmp.clear(); */
    /* viewer.addRenderable(pipe); */

	unsigned int nParticles = 1000;

	qglviewer::Vec g = 0.00002*Vec(0,+9.81,0);
	ParticleGroup *p = new ParticleGroup(nParticles, 1);
	ParticleGroupKernel *archimede = new ConstantForce(g);
	ParticleGroupKernel *dynamicScheme = new DynamicScheme();

	stringstream name;
	for (unsigned int i = 0; i < nParticles; i++) {
			qglviewer::Vec pos = Vec(Random::randf(), Random::randf(), Random::randf());
			qglviewer::Vec  vel = Vec(0,0,0);
			float r = Random::randf(0.02,0.1);
			float m = 4.0f/3.0f*3.1415f*r*r*r;
			p->addParticle(new Particule(pos, vel, m, r, false));	
	}
	
	p->addKernel(archimede);
	p->addKernel(dynamicScheme);
	p->releaseParticles();

	root->addChild("zzparticules", p);

    
	//ObjLoader test
    //vector<Object*> vecc = cube->getObjects();
    //for (unsigned int i = 0; i < vecc.size(); i++) {
        //log_console.infoStream() << "Adding child: cube2_" << i;
        //stringstream s;
        //s << "cube2_" << i;
        //root->addChild(s.str(), vecc[i]);
    //}
    //vector<Object*> vecm = mouette->getObjects();
    //for (unsigned int i = 0; i < vecm.size(); i++) {
        //log_console.infoStream() << "Adding child: mouette_" << i;
        //stringstream s;
        //s << "mouette_" << i;
        //root->addChild(s.str(), vecm[i]);
        //vecm[i]->scale(10.0);
        //vecm[i]->translate(0.0,50.0,0.0);
    //}

    //vector<Object*> vecs = shark->getObjects();
    //for (unsigned int i = 0; i < vecs.size(); i++) {
        //log_console.infoStream() << "Adding child: shark_" << i;
        //stringstream s;
        //s << "shark_" << i;
        //root->addChild(s.str(), vecs[i]);
        //vecs[i]->scale(0.01);
        //vecs[i]->translate(20.0,0.0,0.0);
    //}


	// Diver
	//SeaDiver *diver = new SeaDiver(); 
	//root->addChild("diver", diver); 
	
	//Configure viewer
	viewer->addRenderable(root);
	
	//Run main loop.
	application.exec();

	//Exit
	Audible::closeOpenALContext();
	
	alutExit();

	return EXIT_SUCCESS;
}

