#ifndef OBJS_HPP
#define OBJS_HPP

#include "mathelems.hpp"
#include <iostream>
#include <vector>

#include "placeobjects.h"

class Vertex
{
public:
    Vertex() {}
    Vertex(Dot3D &position_, std::vector<size_t> &facets_)
        : position(position_), facets(facets_) { }

    Vertex(Dot3D &position_) : position(position_) { }

    const Dot3D &getPosition() const;
    void setPosition(Dot3D &position_);
    const std::vector<size_t> getFacets();
    void setFacets(std::vector<size_t> facets_);

private:
    Dot3D position;
    std::vector<size_t> facets;
};

class Facet
{
public:
    Facet() {}
    Facet(std::vector<size_t> vertices_) : vertices(vertices_) {}

    const std::vector<size_t> getVertices();
//    void setUsedVertices(std::vector<size_t> usedVertices_);

private:
    std::vector<size_t> vertices;
};

class PolygonModel
{
public:
    PolygonModel() {}
    PolygonModel(std::vector<Vertex> vertices_, std::vector<Facet> facets_)
        : vertices(vertices_), facets(facets_) { }

    PolygonModel(std::vector<Vertex> vertices_, std::vector<Facet> facets_, QString name_)
        : vertices(vertices_), facets(facets_), modelName(name_) { }

    ~PolygonModel() {}

    const std::vector<Vertex> getVertices();
//    void setVertices(std::vector<Vertex> &vertices_);
    const std::vector<Facet> getFacets();
//    void setFacets(std::vector<Facet> facets_);

//    void moveTo(int newXCell, int newYCell);
//    void rotateZ(int angle);

    void setUsedCell(int xCell_, int yCell_, double zCell_);
    int getUsedXCell();
    int getUsedYCell();
    int getUsedZCell();

    void setLengthModel(int widthModel_);
    int  getLengthModel();
    void setWidthModel(int heightModel_);
    int  getWidthModel();

    void setModelNum(size_t modelNum_);
    size_t getModelNum();

    QString getName();
    void setName(QString modelName_);

    enum model_t
    {
        Brick,
        Plate,
        Tile,
        Arc41,
        Arc14,
        Cylinder1,
        Cylinder2
    };

    void setModelType(model_t modelType_);
    model_t getModelType();

private:
    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    QString modelName;

    int xCell, yCell;
    double zCell;
    int lengthModel, widthModel;

    size_t modelNum;
    model_t modelType;
};

class Light
{
public:
    Light(Eigen::Matrix4f &transMat);
    Light(){};

    void setShadowMap(std::vector<std::vector<double>> &setShadowMap);
    std::vector<std::vector<double>> &getShadowMap();
    void clearShadowMap();

    void setAngles(int xAngle_, int yAngle_);
    int getXAngle();
    int getYAngle();

    void setTransMat(Eigen::Matrix4f &mat);
    Eigen::Matrix4f &getTransMat();

private:
    std::vector<std::vector<double>> shadowMap;
    Eigen::Matrix4f transMatrix;
    int xAngle, yAngle;
};


class SceneInf
{
public:
    SceneInf();
    SceneInf(size_t width_, size_t height_);

    operator bool() const;

    size_t getWidth();
    size_t getHeight();

    void buildBaseModel(Dot3D startOfPlate_, Dot3D endOfPlate_);
    PolygonModel &getBaseModel();

    void changeSize(size_t newWidth, size_t newHeight);

    void moveUp(double value);
    void moveDown(double value);
    void moveLeft(double value);
    void moveRight(double value);

    void scale(double value);

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    void toCenter();

    size_t getModelsNum();
    PolygonModel &getModel(size_t num);
    void setModel(size_t num, PolygonModel &newModel);
    void addModel(PolygonModel &model);
    void deleteModel(size_t num);

    size_t getLightNum();
    Light &getLight(size_t num);
    void setLight(Light &light, size_t i);
    void addLight(Light &light);
    void deleteLight(size_t num);

    Eigen::Matrix4f &getTransMatrix();
    void multToTrans(Eigen::Matrix4f &newTrans);

    std::vector<std::vector<std::vector<double>>> &getUsedCellsZ();
    void initUsedCellsZ();

    void markUsedCellsZ(size_t num);
    void clearUsedCellsZ(size_t num);
    void printUsedCellsZ();

private:
    void addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
                 int x1, int y1, int z1,
                 int x2, int y2, int z2,
                 int x3, int y3, int z3,
                 int x4, int y4, int z4);

    size_t width, height;

    PolygonModel *plateModel = nullptr;

    Eigen::Matrix4f transMatrix;

    size_t modelsNum = 0;
    std::vector<PolygonModel> models;

    size_t lightNum = 0;
    std::vector<Light> Lights;

    Dot3D centerDot;

    std::vector<std::vector<std::vector<double>>> usedCellsZ;

};

#endif
