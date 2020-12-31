#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	ofIcoSpherePrimitive ico_sphere = ofIcoSpherePrimitive(400, 2);
	vector<ofMeshFace> triangles = ico_sphere.getMesh().getUniqueFaces();

	for (int i = 0; i < triangles.size(); i++) {

		auto average = (triangles[i].getVertex(0) + triangles[i].getVertex(1) + triangles[i].getVertex(2)) / 3;
		this->location_list.push_back(average);
	}

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	int span = 240;
	this->face.clear();
	this->frame.clear();

	if(ofGetFrameNum() % span == 0){

		for (int i = 0; i < this->location_list.size(); i += 1) {

			vector<glm::vec3> log;
			log.push_back(this->location_list[i]);
			this->log_list.push_back(log);
			this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
			this->life_list.push_back(span);
			glm::vec3 param;
			int p = ofRandom(3);
			if (p < 1) param = glm::vec3(15, 0, 0);
			if (p >= 1 && p < 2) param = glm::vec3(0, 15, 0);
			if (p >= 2 && p < 3) param = glm::vec3(0, 0, 15);
			this->param_list.push_back(param);
		}
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->param_list.erase(this->param_list.begin() + i);

			continue;
		}

		auto x = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.002, this->noise_seed.x + ofGetFrameNum() * 0.008)), 0, 1, -25, 25);
		auto y = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.002, this->noise_seed.y + ofGetFrameNum() * 0.008)), 0, 1, -25, 25);
		auto z = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.002, this->noise_seed.z + ofGetFrameNum() * 0.008)), 0, 1, -25, 25);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(x, y, z));
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		auto start_index = this->frame.getNumVertices();
		auto alpha = this->life_list[i] > 60 ? 255 : ofMap(this->life_list[i], 0, 60, 0, 255);
		for (int k = 0; k < this->log_list[i].size(); k++) {

			int index = this->face.getNumVertices();

			this->face.addVertex(this->log_list[i][k] + this->param_list[i]);
			this->face.addVertex(this->log_list[i][k] - this->param_list[i]);
			this->face.addColor(ofColor(this->color_list[i], alpha));
			this->face.addColor(ofColor(this->color_list[i], alpha));

			this->frame.addVertex(this->log_list[i][k] + this->param_list[i]);
			this->frame.addVertex(this->log_list[i][k] - this->param_list[i]);
			this->frame.addColor(ofColor(139, alpha));
			this->frame.addColor(ofColor(139, alpha));

			if (k > 0) {

				this->face.addIndex(index + 0); this->face.addIndex(index - 1); this->face.addIndex(index - 2);
				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index - 1);

				this->frame.addIndex(index); this->frame.addIndex(index - 2);
				this->frame.addIndex(index + 1); this->frame.addIndex(index - 1);
			}
		}

		this->frame.addIndex(start_index + 0); this->frame.addIndex(start_index + 1);
		this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum() * 0.4);

	this->face.drawFaces();
	this->frame.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
