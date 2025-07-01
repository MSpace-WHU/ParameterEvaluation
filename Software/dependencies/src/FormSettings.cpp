#include "../include/FormSettings.h"
#include <filesystem>
#include <osg/ShapeDrawable>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //事件监听
#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制

#include <osgText/Text>
#include <osg/AutoTransform>
#include <osg/Material>
#include <osg/LineWidth>
#include <osg/MatrixTransform>


FormSettings::FormSettings(QVBoxLayout* layout, QWidget* parent)
{
    _osgQOpenGLWidget = std::make_shared<osgQOpenGLWidget>();
    layout->addWidget(_osgQOpenGLWidget.get());
    connect(_osgQOpenGLWidget.get(), &osgQOpenGLWidget::initialized, this, &FormSettings::initVisualizationWindow);
}

FormSettings::~FormSettings()
{
}

const osg::Vec4& FormSettings::getBackGroundColor()
{
    return _viewer->getCamera()->getClearColor();
}

void FormSettings::setBackGroundColor(osg::Vec4 backGroundColor)
{
    if (_viewer != nullptr)
    {
        _viewer->getCamera()->setClearColor(backGroundColor);
    }

}
void FormSettings::initVisualizationWindow()
{
    _viewer = _osgQOpenGLWidget->getOsgViewer();
    //调整一些参数
    QSize _size = _osgQOpenGLWidget->sizeHint();
    float _aspectRatio = (float)_size.width() / (float)_size.height();
    _viewer->getCamera()->setProjectionMatrixAsPerspective(30.f, _aspectRatio, 0.001f, FLT_MAX);
    //_viewer->getCamera()->setClearColor(osg::Vec4(0.9412, 0.9412, 0.9412,1.0));
    _viewer->getCamera()->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    _root->setDataVariance(osg::Object::DYNAMIC);
    _root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    _viewer->setSceneData(_root.get());
    //_viewer->addEventHandler(new osgGA::StateSetManipulator(_viewer->getCamera()->getOrCreateStateSet()));

}

void FormSettings::showMatchingNode(osg::ref_ptr<osg::Group> _matching3DRelationship, std::vector<std::string> IDOfExt, std::vector<std::vector<double>> postionOfExt, std::vector<std::string> IDOfRef, std::vector<std::vector<double>> postionOfRef, std::vector<std::pair<size_t, size_t>> trueMatching, std::vector<size_t> resOfExt, std::vector<size_t> resOfRef)
{
    //数据格式转化为osg格式
    std::vector<osg::Vec3> posOfExt;
    std::vector<osg::Vec3> posOfRef;

    if (postionOfExt.size() == 0)
        return;
    if (postionOfExt[0].size() == 3)
    {
        for (size_t i = 0;i < postionOfExt.size(); i++)
        {
            posOfExt.push_back(osg::Vec3(postionOfExt[i][0], postionOfExt[i][1], postionOfExt[i][2]));
        }
        for (size_t i = 0; i < postionOfRef.size(); i++)
        {
            posOfRef.push_back(osg::Vec3(postionOfRef[i][0], postionOfRef[i][1], postionOfRef[i][2]));
        }
    }
    if (postionOfExt[0].size() == 2)
    {
        for (size_t i = 0; i < postionOfExt.size(); i++)
        {
            posOfExt.push_back(osg::Vec3(postionOfExt[i][0], postionOfExt[i][1], 1.0));
        }
        for (size_t i = 0; i < postionOfRef.size(); i++)
        {
            posOfRef.push_back(osg::Vec3(postionOfRef[i][0], postionOfRef[i][1], 1.0));
        }
    }
    if (postionOfExt[0].size() == 1)
    {
        for (size_t i = 0; i < postionOfExt.size(); i++)
        {
            posOfExt.push_back(osg::Vec3(postionOfExt[i][0], 1.0, 1.0));
        }
        for (size_t i = 0; i < postionOfRef.size(); i++)
        {
            posOfRef.push_back(osg::Vec3(postionOfRef[i][0], 1.0, 1.0));
        }
    }
    //设置球自动近小远大，恒定不变
    osg::ref_ptr<osg::AutoTransform> autoTransform = new osg::AutoTransform;
    // 设置AutoTransform为基于视点的缩放模式
    //autoTransform->setScale(20.0);              // 初始缩放比例，确保物体不会太小
    autoTransform->setAutoScaleToScreen(true); // 启用基于视点的缩放
    autoTransform->setMinimumScale(0.001);       // 最小缩放比例
    autoTransform->setMaximumScale(10);
    //autoTransform->setAutoScaleTransitionWidthRatio(0.0);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;


    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    //osg::ref_ptr<osg::Node> tree = osgDB::readNodeFile("./resource/tree.obj");
    //double scaleFactor = 1.0;
    float radius = 0.3f;
    float textSize = 0.1f;
    float lineWid = 0.05f;
    for (size_t i = 0; i < trueMatching.size(); i++)
    {
        //osg::ref_ptr<osg::Node> modelOfExt = osgDB::readNodeFile("./resource/tree.obj");
        //osg::ref_ptr<osg::MatrixTransform> transformOfExt = new osg::MatrixTransform;
        //ColorVisitor colorVisitorOfExt(osg::Vec4(0.0, 0.8, 0.0, 1.0));
        //modelOfExt->accept(colorVisitorOfExt);
        //transformOfExt->setMatrix(osg::Matrix::scale(0.2, 0.2, 0.2) * osg::Matrix::translate(posOfExt[trueMatching[i].first]));
        //transformOfExt->addChild(modelOfExt);
        //geode->addChild(transformOfExt);

        //osg::ref_ptr<osg::Node> modelOfRef = osgDB::readNodeFile("./resource/tree.obj");
        //osg::ref_ptr<osg::MatrixTransform> transformOfRef = new osg::MatrixTransform;
        //ColorVisitor colorVisitorOfRef(osg::Vec4(0.8, 0.8, 0.0, 1.0));
        //modelOfRef->accept(colorVisitorOfRef);
        //transformOfRef->setMatrix(osg::Matrix::scale(0.2, 0.2, 0.2) * osg::Matrix::translate(posOfRef[trueMatching[i].second]));
        //transformOfRef->addChild(modelOfRef);
        //geode->addChild(transformOfRef);

        osg::ref_ptr<osg::Sphere> sphereOfExt = new osg::Sphere(posOfExt[trueMatching[i].first], radius);
        // 创建一个形状绘制对象，并设置其形状数据
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawableOfExt = new osg::ShapeDrawable(sphereOfExt);
        // 设置球的颜色
        osg::ref_ptr<osg::Vec4Array> colorsOfExt = new osg::Vec4Array;
        colorsOfExt->push_back(osg::Vec4(0.0, 0.8, 0.0, 1.0)); // 设置为绿色
        sphereDrawableOfExt->setColorArray(colorsOfExt);
        sphereDrawableOfExt->setColorBinding(osg::Geometry::BIND_OVERALL);

        // 创建一个Geode节点，并添加形状绘制对象
        geode->addDrawable(sphereDrawableOfExt);

        osg::ref_ptr<osg::Sphere> sphereOfRef = new osg::Sphere(posOfRef[trueMatching[i].second], radius);
        // 创建一个形状绘制对象，并设置其形状数据
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawableOfRef = new osg::ShapeDrawable(sphereOfRef);
        // 设置球的颜色
        osg::ref_ptr<osg::Vec4Array> colorsOfRef = new osg::Vec4Array;
        colorsOfRef->push_back(osg::Vec4(0.8, 0.8, 0.0, 1.0)); // 设置为黄色
        sphereDrawableOfRef->setColorArray(colorsOfRef);
        sphereDrawableOfRef->setColorBinding(osg::Geometry::BIND_OVERALL);
        // 创建一个Geode节点，并添加形状绘制对象
        geode->addDrawable(sphereDrawableOfRef);


        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(posOfExt[trueMatching[i].first]);
        vertices->push_back(posOfRef[trueMatching[i].second]);
        osg::ref_ptr<osg::Geometry> lineGeom = new osg::Geometry;
        lineGeom->setVertexArray(vertices);
        lineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size()));

        osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth(lineWid);// 设置线的宽度
        //lineWidth->setWidth(0.05); 
        lineGeom->getOrCreateStateSet()->setAttribute(lineWidth);

        geode->addDrawable(lineGeom);

        osg::Vec3 deltaD(0.3, 0.3, 0.3);
        deltaD.x() = (posOfExt[trueMatching[i].first] - posOfRef[trueMatching[i].second]).x();
        deltaD.y() = (posOfExt[trueMatching[i].first] - posOfRef[trueMatching[i].second]).y();
        deltaD.z() = (posOfExt[trueMatching[i].first] - posOfRef[trueMatching[i].second]).z();
        deltaD.normalize();
        //if (dis > 1)
        //{
        //    deltaD.x() = 0.0;
        //    deltaD.y() = 0.0;
        //    deltaD.z() = 0.0;
        //}
        //if (dis < 0.3)
        //{
        //    deltaD.x() = 0.3;
        //    deltaD.y() = 0.3;
        //    deltaD.z() = 0.3;
        //}

        osg::ref_ptr<osgText::Text> textOfExt = new osgText::Text;
        textOfExt->setText("E_" + IDOfExt[trueMatching[i].first]);
        textOfExt->setCharacterSize(textSize); // 设置文本的大小
        textOfExt->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
        textOfExt->setAxisAlignment(osgText::TextBase::SCREEN);
        textOfExt->setPosition(posOfExt[trueMatching[i].first] + (deltaD * radius));

        // 创建Camera节点，设置其为后渲染
        camera->addChild(textOfExt);

        osg::ref_ptr<osgText::Text> textOfRef = new osgText::Text;
        textOfRef->setText("R_" + IDOfRef[trueMatching[i].second]);
        textOfRef->setCharacterSize(textSize); // 设置文本的大小
        textOfRef->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
        textOfRef->setAxisAlignment(osgText::TextBase::SCREEN);
        textOfRef->setPosition(posOfRef[trueMatching[i].second] - (deltaD * radius));

        // 创建Camera节点，设置其为后渲染
        camera->addChild(textOfRef);

    }

    for (size_t i = 0; i < resOfExt.size(); i++)
    {
        //osg::ref_ptr<osg::Node> modelOfUExt = osgDB::readNodeFile("./resource/tree.obj");
        //osg::ref_ptr<osg::MatrixTransform> transformOfUExt = new osg::MatrixTransform;
        //ColorVisitor colorVisitor(osg::Vec4(0.8, 0.0, 0.0, 1.0));
        //modelOfUExt->accept(colorVisitor);
        //transformOfUExt->setMatrix(osg::Matrix::scale(0.2, 0.2, 0.2)* osg::Matrix::translate(posOfExt[resOfExt[i]]));
        //transformOfUExt->addChild(modelOfUExt);
        //geode->addChild(transformOfUExt);


        osg::ref_ptr<osg::Sphere> sphereOfUExt = new osg::Sphere(posOfExt[resOfExt[i]], radius);
        // 创建一个形状绘制对象，并设置其形状数据
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawableOfUExt = new osg::ShapeDrawable(sphereOfUExt);
        // 设置球的颜色
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0)); // 设置为红色
        sphereDrawableOfUExt->setColorArray(colors);
        sphereDrawableOfUExt->setColorBinding(osg::Geometry::BIND_OVERALL);
        // 创建一个Geode节点，并添加形状绘制对象
        geode->addDrawable(sphereDrawableOfUExt);

        osg::ref_ptr<osgText::Text> textOfUExt = new osgText::Text;
        textOfUExt->setText("UE_" + IDOfExt[resOfExt[i]]);
        textOfUExt->setCharacterSize(textSize); // 设置文本的大小
        textOfUExt->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
        textOfUExt->setAxisAlignment(osgText::TextBase::SCREEN);
        textOfUExt->setPosition(posOfExt[resOfExt[i]]);

        // 创建Camera节点，设置其为后渲染
        camera->addChild(textOfUExt);
    }
    for (size_t i = 0; i < resOfRef.size(); i++)
    {
        //osg::ref_ptr<osg::Node> modelOfURef = osgDB::readNodeFile("./resource/tree.obj");
        //osg::ref_ptr<osg::MatrixTransform> transformOfURef = new osg::MatrixTransform;
        //ColorVisitor colorVisitor(osg::Vec4(0.8, 0.0, 0.0, 1.0));
        //modelOfURef->accept(colorVisitor);
        //transformOfURef->setMatrix(osg::Matrix::scale(0.2, 0.2, 0.2)* osg::Matrix::translate(posOfRef[resOfRef[i]]));
        //transformOfURef->addChild(modelOfURef);
        //geode->addChild(transformOfURef);

        osg::ref_ptr<osg::Sphere> sphereOfURef = new osg::Sphere(posOfRef[resOfRef[i]], radius);
        // 创建一个形状绘制对象，并设置其形状数据
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawableOfURef = new osg::ShapeDrawable(sphereOfURef);
        // 设置球的颜色
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0, 0.0, 1.0, 1.0)); // 设置为粉色
        sphereDrawableOfURef->setColorArray(colors);
        sphereDrawableOfURef->setColorBinding(osg::Geometry::BIND_OVERALL);
        // 创建一个Geode节点，并添加形状绘制对象
        geode->addDrawable(sphereDrawableOfURef);

        osg::ref_ptr<osgText::Text> textOfURef = new osgText::Text;
        textOfURef->setText("UR_" + IDOfRef[resOfRef[i]]);
        textOfURef->setCharacterSize(textSize); // 设置文本的大小
        textOfURef->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
        textOfURef->setAxisAlignment(osgText::TextBase::SCREEN);
        textOfURef->setPosition(posOfRef[resOfRef[i]]);

        // 创建Camera节点，设置其为后渲染
        camera->addChild(textOfURef);
    }

    autoTransform->addChild(geode);
    autoTransform->addChild(camera);
    _matching3DRelationship->addChild(autoTransform);

    _matching3DRelationship->setName("TreeMatchingNode");

    _root->addChild(_matching3DRelationship);
    osg::BoundingSphere boundingSphere = geode->getBound();
    //std::cout << boundingSphere.center().x() << "," << boundingSphere.center().y() << "," << boundingSphere.center().z() << std::endl;
    //std::cout << boundingSphere.radius() << std::endl;
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;

    autoTransform->setPosition(boundingSphere.center());
    // 计算相机的位置：节点中心 + 半径在Z轴方向
    osg::Vec3 cameraPosition = boundingSphere.center() + osg::Vec3(0.0, 0.0, boundingSphere.radius());
    // 设置相机的视图矩阵
    manipulator->setHomePosition(cameraPosition, boundingSphere.center(), osg::Vec3(0.0, 1.0, 0.0));
    _viewer->setCameraManipulator(manipulator);
}

void FormSettings::deleteTreeNode(osg::ref_ptr<osg::Group> Node)
{
    _root->removeChild(Node);
}
