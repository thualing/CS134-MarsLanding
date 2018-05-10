//
//  Kevin M. Smith - Basic Octree Class - CS134/CS235 4/18/18
//
//  (c) Kevin M. Smith - 2018 -  Not for Redistribution without Permsission
//
//  
//   Example use:
//
//   Octree octree;
//   octree.create(mesh, 20);
//
//   For rendering;
//
//   int level;
//   octree.draw(5, level);
//
//   For selection;
//   
//   TreeNode node;
//   bool hit = octree.intersect(ray, octree.root, node);
//
//
#pragma once
#include "box.h"
#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"

class TreeNode {
public:
    Box box;
    vector<TreeNode> children;
    vector<int> points;
};

class Octree{
public:
    
    TreeNode root;
    ofMesh mesh;
    
    void create(const ofMesh &mesh, int numLevels);
    void draw(TreeNode &node, int numLevels, int level);
    void draw(int numLevels, int level)
    {
        draw(root, numLevels, level);
    }
    Box meshBounds(const ofMesh &);
    
    void drawBox(const Box &box);
    int getMeshPointsInBox(const vector<int> & points, Box & box, vector<int> & pointsRtn);
    void subDivideBox8(const Box &box, vector<Box> & boxList);
    void subdivide(TreeNode &rootNode, int numLevels, int level);
    bool intersect(const ofVec3f &point, TreeNode & node);
};
