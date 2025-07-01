#pragma once

#include <osgViewer/Viewer>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>
#include <osgDB/WriteFile>
#include <QObject>
#include <QtWidgets/QVBoxLayout>

#include <osg/NodeVisitor>
#include <osg/Geometry>


#include "../../osgQt/include/osgQOpenGLWidget.h"

class FormSettings  : public QObject
{
	Q_OBJECT

public:
	FormSettings(QVBoxLayout* layout, QWidget* parent = nullptr);
	~FormSettings();

    //set color background
    const osg::Vec4& getBackGroundColor();
    void setBackGroundColor(osg::Vec4 backGroundColor);
    //show matching node
    void showMatchingNode(osg::ref_ptr<osg::Group> inputNode, std::vector<std::string> IDOfExt, std::vector<std::vector<double>> postionOfExt,std::vector<std::string> IDOfRef, std::vector<std::vector   <double>> postionOfRef, std::vector<std::pair<size_t, size_t>> trueMatching, std::vector<size_t> resOfExt, std::vector<size_t> resOFRef);
    //dete tree node of file management
    void deleteTreeNode(osg::ref_ptr<osg::Group> Node);
    //cheeck if node is loaded
    bool isfinished() { return _finished; };
    void initWorkProgress() { _finished = false; };

protected slots:
    //initialize the osg window with osgQT
    void initVisualizationWindow();

private:
    //osgQT
    std::shared_ptr<osgQOpenGLWidget> _osgQOpenGLWidget;
    //osg viewer
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    // root node of file management
    osg::ref_ptr<osg::Group> _root = new osg::Group();

    bool _finished = false;

};

class ColorVisitor : public osg::NodeVisitor
{
public:
    ColorVisitor(osg::Vec4 color) : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) { _color = color; }

    virtual void apply(osg::Geode& geode)
    {
        for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
        {
            osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
            if (geometry)
            {
                osg::Vec4Array* colors = new osg::Vec4Array;
                colors->push_back(_color); // red 

                geometry->setColorArray(colors);
                geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
            }
        }

        traverse(geode);
    }

private:
    osg::Vec4 _color;
};
