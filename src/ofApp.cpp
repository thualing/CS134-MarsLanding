
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like 
//  the input interface to be the same for each student's 
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
//  Student Name:   <Hua Tong, Le Dao, Luis Arevalo>
//  Date: <05-10-2018>


#include "ofApp.h"
#include "Util.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {

	background.load("images/stars.jpg");

	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bRoverLoaded = false;
	bTerrainSelected = true;
	level = 0;
	//	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	mars.loadModel("geo/mars-low-v2.obj");
	mars.setScale(4, 4, 4);
	lander.loadModel("geo/lander.obj");
	lander.setScale(0.2, 0.2, 0.2);

	// Camera
	//
	theCam = &cam;
	ground.setOrientation(ofVec3f(-90, 0, 0));
	ground.setNearClip(.1);
	side.setNearClip(.1);
	track.setPosition(0, 1, 0);
	track.setNearClip(.1);


	mars.setScaleNormalization(false);
	lander.setScaleNormalization(false);
	tree.create(mars.getMesh(0), 7);
	collision = false;
	frameCounter = 0;

	// this part from rocketBall
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetBackgroundColor(ofColor::black);

	// set up sound
	//
	if (soundPlayer.load("sounds/rocket.wav")) {
		soundPlayer.setLoop(true);
		soundFileLoaded = true;
	}

	//  setup emitter (for engine)
	//
	engine.setRate(600);
	engine.setParticleRadius(.010);
    engine.setEmitterType(DiscEmitter);
    engine.setLifespan(1);
	engine.visible = false;
	ship.radius = 0.010;

	// create our one lonely particle
	//
	ship.lifespan = 10000;
	ship.position.set(0, 10, 0);
	lander.setPosition(ship.position.x, ship.position.y, ship.position.z);
	sys.add(ship);

	sys.addForce(&thruster);
	sys.addForce(&impulseForce);
    
    //adds turbulence to the partlces, adds gravity, impulse radial force and cyclic force
    engine.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
    engine.sys->addForce(new ImpulseRadialForce(10));
    engine.sys->addForce(new CyclicForce(20));
    engine.setGroupSize(10);
    
    //this code adds gravity to the ship. the middle paramters makes it fall
    sys.addForce(new GravityForce(ofVec3f(0, -.01, 0)));

}


//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {

	sys.update();
	engine.update();
	engine.setPosition(sys.particles[0].position);
	lander.setPosition(sys.particles[0].position.x, sys.particles[0].position.y, sys.particles[0].position.z);
	lander.update();
	collisionDetect();

	// Camera
	//
	ground.setPosition(sys.particles[0].position + ofVec3f(0.1, 0, 0.1));
	side.setPosition(sys.particles[0].position + ofVec3f(-1.5, 0, 0));
	track.lookAt(lander.getPosition());
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	ofDisableDepthTest();
	background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	ofEnableDepthTest();

	theCam->begin();
	ofPushMatrix();

	// draw particle system
	//
	sys.draw();

	// draw engine output
	//
	engine.draw();


	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::red);
		mars.drawWireframe();
		lander.drawWireframe();
		if (bRoverLoaded) {
			rover.drawWireframe();
//            if (!bTerrainSelected) drawAxis(rover.getPosition());
		}
//        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		lander.drawFaces();

		if (bRoverLoaded) {
			rover.drawFaces();

//            if (!bTerrainSelected) drawAxis(rover.getPosition());
		}
//        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}


//    if (bDisplayPoints) {                // display points as an option
//        glPointSize(3);
//        ofSetColor(ofColor::green);
//        mars.drawVertices();
//        rover.drawVertices();
//        lander.drawVertices();
//    }
//
//    // highlight selected point (draw sphere around selected point)
//    //
//    if (bPointSelected) {
//        ofSetColor(ofColor::red);
//        ofDrawSphere(selectedPoint, .1);
//    }
//
    ofNoFill();
//    ofSetColor(ofColor::red);
//    drawBox(boundingBox);

//    ofSetColor(ofColor::red);
//    for (int i = 0; i < level1.size(); i++)
//        drawBox(level1[i]);
//    ofPopMatrix();
//    if (collision) {
//        cout << "landing" << endl;
//    }

	theCam->end();

    //print frame rate
//    string str;
//    str += "Frame Rate:" + std::to_string(ofGetFrameRate());
//    ofSetColor(ofColor::white)
//        ;
//    ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);
}

// 

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_UP:
		playSound();
		thruster.add(ofVec3f(0, .5, 0));
		engine.setVelocity(ofVec3f(0, -5, 0));
		engine.start();
		soundPlayer.play();
		break;
	case OF_KEY_DOWN:
		playSound();
		thruster.add(ofVec3f(0, -.5, 0));
		engine.setVelocity(ofVec3f(0, 5, 0));
		engine.start();
		soundPlayer.play();
		break;
	case OF_KEY_LEFT:
		playSound();
		thruster.add(ofVec3f(-.5, 0, 0));
		engine.setVelocity(ofVec3f(5, 0, 0));
		engine.start();
		soundPlayer.play();
		break;
	case OF_KEY_RIGHT:
		playSound();
		thruster.add(ofVec3f(.5, 0, 0));
		engine.setVelocity(ofVec3f(-5, 0, 0));
		engine.start();
		soundPlayer.play();
		break;
	case 'z':
		playSound();
		thruster.add(ofVec3f(0, 0, 0.5));
		engine.setVelocity(ofVec3f(0, 0, -5));
		engine.start();
		soundPlayer.play();
		break;
	case 'x':
		playSound();
		thruster.add(ofVec3f(0, 0, -0.5));
		engine.setVelocity(ofVec3f(0, 0, 5));
		engine.start();
		soundPlayer.play();
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F2:
		theCam = &ground;
		break;
	case OF_KEY_F3:
		theCam = &side;
		break;
	case OF_KEY_F4:
		theCam = &track;
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	soundPlayer.stop();
	engine.stop();
	thruster.set(ofVec3f(0, 0, 0));
	switch (key) {

	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));
	if (boundingBox.intersect(ray, -100, 100)) cout << "intersects" << endl;
}


//draw a box from a "Box" class  
//
void ofApp::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
	// get vertex gets 8 points, and returns first point as the starting point of the loop
	int n = mesh.getNumVertices();
	// access each vertex of the box
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}


//
//  Select Target Point on Terrain by comparing distance of mouse to 
//  vertice points projected onto screenspace.
//  if a point is selected, return true, else return false;
//
bool ofApp::doPointSelection() {

	ofMesh mesh = mars.getMesh(0);
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	int nearestIndex = 0;

	bPointSelected = false;

	ofVec2f mouse(mouseX, mouseY);
	vector<ofVec3f> selection;

	// We check through the mesh vertices to see which ones
	// are "close" to the mouse point in screen space.  If we find 
	// points that are close, we store them in a vector (dynamic array)
	//
	for (int i = 0; i < n; i++) {
		ofVec3f vert = mesh.getVertex(i);
		ofVec3f posScreen = cam.worldToScreen(vert);
		float distance = posScreen.distance(mouse);
		if (distance < selectionRange) {
			selection.push_back(vert);
			bPointSelected = true;
		}
	}

	//  if we found selected points, we need to determine which
	//  one is closest to the eye (camera). That one is our selected target.
	//
	if (bPointSelected) {
		float distance = 0;
		for (int i = 0; i < selection.size(); i++) {
			ofVec3f point = cam.worldToCamera(selection[i]);

			// In camera space, the camera is at (0,0,0), so distance from 
			// the camera is simply the length of the point vector
			//
			float curDist = point.length();

			if (i == 0 || curDist < distance) {
				distance = curDist;
				selectedPoint = selection[i];
			}
		}
	}
	return bPointSelected;
}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{ 5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);

	if (rover.loadModel(dragInfo.files[0])) {
		rover.setScaleNormalization(false);
		rover.setScale(.005, .005, .005);
		rover.setPosition(point.x, point.y, point.z);
		bRoverLoaded = true;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

void ofApp::playSound() {
	if (soundFileLoaded) soundPlayer.play();
}

//--------------------------------------------------------------
//
// collision detection
//
void ofApp::collisionDetect() {
	contactPt = sys.particles[0].position;
	ofVec3f vel = sys.particles[0].velocity;
	// check if the ship is moving up
	if (vel.y > 0) {
		return;
	}
	if (tree.intersect(contactPt, tree.root)) {
		collision = true;
        cout << "land" <<endl;
		//        cout << "velocity: " << vel << endl;
		impulseForce.apply(1.5 * (-vel * 3.6));
	}
}
