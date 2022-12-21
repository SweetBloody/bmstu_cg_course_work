#include "facade.hpp"

#include "QDebug"
#include "QPen"

#include <limits>

#include "math.h"
#include "config.hpp"

Facade::Facade() { drawer = new Drawer; }

void Facade::setSceneInf(size_t width_, size_t height_)
{
    if (scene)
        delete scene;

    scene = new SceneInf(width_, height_);
}

void Facade::changeSceneInf(size_t newWidth, size_t newheight)
{
    scene->changeSize(newWidth, newheight);
}

bool Facade::isSceneSet() { return scene; }

QGraphicsScene *Facade::drawScene(QRectF rect)
{
    QGraphicsScene *retScene = nullptr;
    if (isSceneSet())
        retScene = drawer->drawScene(scene, rect);

    return retScene;
}

QGraphicsScene *Facade::moveUpScene(double value, QRectF rect)
{
    scene->moveUp(value);

    return drawScene(rect);
}

QGraphicsScene *Facade::moveDownScene(double value, QRectF rect)
{
    scene->moveDown(value);

    return drawScene(rect);
}

QGraphicsScene *Facade::moveRightScene(double value, QRectF rect)
{
    scene->moveRight(value);

    return drawScene(rect);
}

QGraphicsScene *Facade::moveLeftScene(double value, QRectF rect)
{
    scene->moveLeft(value);

    return drawScene(rect);
}

QGraphicsScene *Facade::scaleScene(double value, QRectF rect)
{
    scene->scale(value);

    return drawScene(rect);
}

QGraphicsScene *Facade::rotateXScene(double angle, QRectF rect)
{
    scene->rotateX(angle);

    return drawScene(rect);
}

QGraphicsScene *Facade::rotateYScene(double angle, QRectF rect)
{
    scene->rotateY(angle);

    return drawScene(rect);
}

QGraphicsScene *Facade::rotateZScene(double angle, QRectF rect)
{
    scene->rotateZ(angle);

    return drawScene(rect);
}

QGraphicsScene *Facade::toCenter(QRectF rect)
{
    scene->toCenter();

    return drawScene(rect);
}

SceneInf *Facade::getScene() { return scene; }

void Facade::addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2,
    int x3, int y3, int z3, int x4, int y4, int z4)
{
    Dot3D dot;
    std::vector<size_t> vec;

    size_t size = facets.size();

    dot = Dot3D(x1, y1, z1);
    vec = {size, size + 1};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x2, y2, z2);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x3, y3, z3);
    vec = {size, size + 1};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x4, y4, z4);
    vec = {size + 1};
    vertices.push_back(Vertex(dot, vec));


    size = vertices.size();
    vec = {size - 4, size - 3, size - 2};
    facets.push_back(vec);
    vec = {size - 4, size - 1, size - 2};
    facets.push_back(vec);
}

void Facade::addTriangle(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3)
{
    Dot3D dot;
    std::vector<size_t> vec;

    size_t size = facets.size();

    dot = Dot3D(x1, y1, z1);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x2, y2, z2);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x3, y3, z3);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    size = vertices.size();
    vec = {size - 3, size - 2, size - 1};
    facets.push_back(vec);
}

void Facade::addLight(int xAngle, int yAngle)
{
    Eigen::Matrix4f transMat;
    double radianXAngle = (double) xAngle * M_PI / 180.0;
    double radianYAngle = (double) yAngle * M_PI / 180.0;

    transMat << 1,          0,          0,               0,
               0, cos(radianXAngle), -sin(radianXAngle), 0,
               0, sin(radianXAngle),  cos(radianXAngle), 0,
               0,           0,          0,               1;

    Eigen::Matrix4f prodMat;
    prodMat << cos(radianYAngle), 0, sin(radianYAngle), 0,
                0,                1,    0,              0,
              -sin(radianYAngle), 0, cos(radianYAngle), 0,
                0,                0,    0,              1;

    transMat *= prodMat;

    Light illum(transMat);
    illum.setAngles(xAngle, yAngle);
    scene->addLight(illum);
}

void Drawer::borderPut(int x, int y, double z)
{
    if (x < 0 || x >= (int) depthBuffer.size() || \
        y < 0 || y >= (int) depthBuffer.at(0).size())
        return;
    if (std::fabs(z - depthBuffer.at(x).at(y)) < 1 || z > depthBuffer.at(x).at(y))
        borderBuffer.at(x).at(y) = 3;
}

void Drawer::bordersForPolygon(
    int xStart, int yStart, double zStart, int xEnd, int yEnd, double zEnd)
{
    if (xStart == xEnd && yStart == yEnd)
    {
        borderPut(xStart, yStart, zStart);
        return;
    }

    double deltaX = xEnd - xStart;
    double deltaY = yEnd - yStart;
    double deltaZ = zEnd - zStart;

    int trX = abs(deltaX);
    int trY = abs(deltaY);

    int length;
    if (trX > trY)
        length = trX;
    else
        length = trY;

    deltaX /= length;
    deltaY /= length;
    deltaZ /= length;

    double curX = xStart;
    double curY = yStart;
    double curZ = zStart;

    for (int i = 0; i < length; i++)
    {
        borderPut(round(curX), round(curY), curZ);
        curX += deltaX;
        curY += deltaY;
        curZ += deltaZ;
    }
}

void Drawer::interRowIntoShadowMap(
    std::vector<std::vector<double>> *map, int xA, int xB, double zA, double zB, int curY)
{
    for (int curX = xA; curX <= xB; curX++)
    {
        double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);
        if (curX >= (int) map->size() || curX < 0 || curY >= (int) map->at(0).size() ||
            curY < 0)
            continue;

        if (curZ > map->at(curX).at(curY))
            map->at(curX).at(curY) = curZ;
    }
}

void Drawer::shadowMapForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
    Eigen::Matrix4f &transMat, Light *illum, size_t bufWidth, size_t bufHeight)
{
    std::array<Dot3D, 3> dotsArr;
    Eigen::Matrix4f toCenter;

    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -BASE_Z - 5, 1;

    Eigen::Matrix4f backToStart;

    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, BASE_Z + 5, 1;

    std::vector<std::vector<double>> *shadowMap = &illum->getShadowMap();
    Eigen::Matrix4f illumMat = illum->getTransMat();

    Eigen::Matrix4f dotTransMat = toCenter * transMat * illumMat * backToStart;

    for (size_t curFaceNum = 0; curFaceNum < facets.size(); curFaceNum++)
    {
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getVertices();
        dotsArr[0] = vertices.at(curDots.at(0)).getPosition();
        dotsArr[1] = vertices.at(curDots.at(1)).getPosition();
        dotsArr[2] = vertices.at(curDots.at(2)).getPosition();

        coordinatesVec <<
            dotsArr[0].getXCoordinate(), dotsArr[0].getYCoordinate(), dotsArr[0].getZCoordinate(), 1,
            dotsArr[1].getXCoordinate(), dotsArr[1].getYCoordinate(), dotsArr[1].getZCoordinate(), 1,
            dotsArr[2].getXCoordinate(), dotsArr[2].getYCoordinate(), dotsArr[2].getZCoordinate(), 1;

        coordinatesVec *= dotTransMat;
        dotsArr[0] =
            Dot3D(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot3D(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot3D(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getYCoordinate() > dotsArr[1].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[1]);
        if (dotsArr[0].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[2]);
        if (dotsArr[1].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[1], dotsArr[2]);

        int x1 = round(dotsArr[0].getXCoordinate());
        int x2 = round(dotsArr[1].getXCoordinate());
        int x3 = round(dotsArr[2].getXCoordinate());

        double z1 = dotsArr[0].getZCoordinate();
        double z2 = dotsArr[1].getZCoordinate();
        double z3 = dotsArr[2].getZCoordinate();

        int y1 = round(dotsArr[0].getYCoordinate());
        int y2 = round(dotsArr[1].getYCoordinate());
        int y3 = round(dotsArr[2].getYCoordinate());

//#pragma omp parallel for
        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            interRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
        }

//#pragma omp parallel for
        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            interRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
        }
    }
}

void Drawer::zBufForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
    Eigen::Matrix4f &transMat, size_t color, SceneInf *scene, size_t bufWidth,
    size_t bufHeight)
{
    std::array<Dot3D, 3> dotsArr;
    Eigen::Matrix4f toCenter;

    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -BASE_Z - 5, 1;

    Eigen::Matrix4f backToStart;

    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, BASE_Z + 5, 1;

    Eigen::Matrix4f dotTransMat;
    dotTransMat = toCenter * transMat * backToStart;
    std::vector<Eigen::Matrix4f> illumDotMatrices;

    for (size_t i = 0; i < scene->getLightNum(); i++)
        illumDotMatrices.push_back(
            toCenter * scene->getLight(i).getTransMat() * backToStart);

    for (size_t curFaceNum = 0; curFaceNum < facets.size(); curFaceNum++)
    {
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getVertices();
        dotsArr[0] = vertices.at(curDots.at(0)).getPosition();
        dotsArr[1] = vertices.at(curDots.at(1)).getPosition();
        dotsArr[2] = vertices.at(curDots.at(2)).getPosition();

        coordinatesVec <<
            dotsArr[0].getXCoordinate(), dotsArr[0].getYCoordinate(), dotsArr[0].getZCoordinate(), 1,
            dotsArr[1].getXCoordinate(), dotsArr[1].getYCoordinate(), dotsArr[1].getZCoordinate(), 1,
            dotsArr[2].getXCoordinate(), dotsArr[2].getYCoordinate(), dotsArr[2].getZCoordinate(), 1;

        coordinatesVec *= dotTransMat;

        dotsArr[0] =
            Dot3D(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot3D(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot3D(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getYCoordinate() > dotsArr[1].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[1]);
        if (dotsArr[0].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[2]);
        if (dotsArr[1].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[1], dotsArr[2]);

        int x1 = round(dotsArr[0].getXCoordinate());
        int x2 = round(dotsArr[1].getXCoordinate());
        int x3 = round(dotsArr[2].getXCoordinate());

        double z1 = dotsArr[0].getZCoordinate();
        double z2 = dotsArr[1].getZCoordinate();
        double z3 = dotsArr[2].getZCoordinate();

        int y1 = round(dotsArr[0].getYCoordinate());
        int y2 = round(dotsArr[1].getYCoordinate());
        int y3 = round(dotsArr[2].getYCoordinate());

        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);

                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);

                    for (size_t i = 0; i < scene->getLightNum() && !visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getLight(i).getShadowMap();

                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));

                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getLightNum())
                    {
                        borderBuffer.at(curX).at(curY) = color + visible;
                    }
                    else
                        borderBuffer.at(curX).at(curY) = color + 1;
                }
            }
        }
        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);

                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short is_visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);
                    for (size_t i = 0; i < scene->getLightNum() && !is_visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getLight(i).getShadowMap();
                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));

                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            is_visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getLightNum())
                    {
                        borderBuffer.at(curX).at(curY) = color + is_visible;
                    }
                    else
                        borderBuffer.at(curX).at(curY) = color + 1;
                }
            }
        }
        bordersForPolygon(x1, y1, z1, x2, y2, z2);
        bordersForPolygon(x1, y1, z1, x3, y3, z3);
        bordersForPolygon(x2, y2, z2, x3, y3, z3);

    }
}

void Drawer::zBufferAlg(SceneInf *scene, size_t bufHeight, size_t bufWidth)
{
    depthBuffer.erase(depthBuffer.begin(), depthBuffer.end());
    borderBuffer.erase(borderBuffer.begin(), borderBuffer.end());

    for (size_t i = 0; i < bufWidth; i++)
    {
        depthBuffer.push_back(std::vector<double>(bufHeight, 0));
        borderBuffer.push_back(std::vector<size_t>(bufHeight, 0));
    }

    PolygonModel model;
    std::vector<Facet> facets;
    std::vector<Vertex> vertices;
    PolygonModel::model_t typeModel;

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        facets = model.getFacets();
        vertices = model.getVertices();
        for (size_t j = 0; j < scene->getLightNum(); j++)
            shadowMapForModel(facets, vertices, scene->getTransMatrix(),
                &scene->getLight(j), bufWidth, bufHeight);
    }

    model = scene->getBaseModel();
    facets = model.getFacets();
    vertices = model.getVertices();
    for (size_t j = 0; j < scene->getLightNum(); j++)
        shadowMapForModel(facets, vertices, scene->getTransMatrix(),
            &scene->getLight(j), bufWidth, bufHeight);

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        facets = model.getFacets();
        vertices = model.getVertices();
        typeModel = model.getModelType();
        zBufForModel(
            facets, vertices, scene->getTransMatrix(), 4 + typeModel * 2, scene, bufWidth, bufHeight);
    }
    model = scene->getBaseModel();
    facets = model.getFacets();
    vertices = model.getVertices();
    zBufForModel(
        facets, vertices, scene->getTransMatrix(), 1, scene, bufWidth, bufHeight);

    for (size_t i = 0; i < scene->getLightNum(); i++)
        scene->getLight(i).clearShadowMap();
}

QGraphicsScene *Drawer::drawScene(SceneInf *scene, QRectF rect)
{
    size_t width = scene->getWidth() * SIZE_SC;
    size_t height = scene->getHeight() * SIZE_SC;

    scene->buildBaseModel(Dot3D(BASE_START), Dot3D(width, height, BASE_Z));

    using namespace std::chrono;
    nanoseconds start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    zBufferAlg(scene, rect.size().height(), rect.size().width());
    nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

    qDebug() << "Время выполнения алгоритма" << size_t((end - start).count() / 1000000);

    QGraphicsScene *outScene = new QGraphicsScene;
    outScene->setSceneRect(rect);

    QImage *image =
        new QImage(rect.size().width(), rect.size().height(), QImage::Format_RGB32);
    image->fill(Qt::white);

    uint baseColor              = qRgb(BASE_COLOR);
    uint baseColorShadow        = qRgb(BASE_COLOR_SHADOW);
    uint borderColor            = qRgb(BLACK_COLOR);

    uint brickColor             = qRgb(BRICK_COLOR);
    uint brickColorShadow       = qRgb(BRICK_COLOR_SHADOW);

    uint plateColor             = qRgb(PLATE_COLOR);
    uint plateColorShadow       = qRgb(PLATE_COLOR_SHADOW);

    uint tileColor              = qRgb(TILE_COLOR);
    uint tileColorShadow        = qRgb(TILE_COLOR_SHADOW);

    uint arcColor               = qRgb(ARC_COLOR);
    uint arcColorShadow         = qRgb(ARC_COLOR_SHADOW);

    uint cylinderColor          = qRgb(CYLINDER_COLOR);
    uint cylinderColorShadow    = qRgb(CYLINDER_COLOR_SHADOW);

    nanoseconds start2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

    for (size_t i = 0; i < rect.size().width() - 1; i++)
        for (size_t j = 0; j < rect.size().height() - 1; j++)
        {
            if (borderBuffer.at(i).at(j) == 1)
            {
                image->setPixel(i, j, baseColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 2)
            {
                image->setPixel(i, j, baseColor);
            }
            else if (borderBuffer.at(i).at(j) == 3)
            {
                image->setPixel(i, j, borderColor);
            }
            else if (borderBuffer.at(i).at(j) == 4)
            {
                image->setPixel(i, j, brickColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 5)
            {
                image->setPixel(i, j, brickColor);
            }
            else if (borderBuffer.at(i).at(j) == 6)
            {
                image->setPixel(i, j, plateColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 7)
            {
                image->setPixel(i, j, plateColor);
            }
            else if (borderBuffer.at(i).at(j) == 8)
            {
                image->setPixel(i, j, tileColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 9)
            {
                image->setPixel(i, j, tileColor);
            }
            else if (borderBuffer.at(i).at(j) == 10 || borderBuffer.at(i).at(j) == 12)
            {
                image->setPixel(i, j, arcColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 11 || borderBuffer.at(i).at(j) == 13)
            {
                image->setPixel(i, j, arcColor);
            }
            else if (borderBuffer.at(i).at(j) == 14 || borderBuffer.at(i).at(j) == 16)
            {
                image->setPixel(i, j, cylinderColorShadow);
            }
            else if (borderBuffer.at(i).at(j) == 15 || borderBuffer.at(i).at(j) == 17)
            {
                image->setPixel(i, j, cylinderColor);
            }
        }

    nanoseconds end2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    qDebug() << "Отрисовка" << size_t((end2 - start2).count() / 1000000);
    qDebug() << "Общее время"     << size_t((end2 - start2).count() / 1000000) +
                                     size_t((end - start).count()   / 1000000);


    outScene->addPixmap(QPixmap::fromImage(*image));
    delete image;
    for (size_t i = 0; i < scene->getLightNum(); i++)
        scene->getLight(i).clearShadowMap();

    return outScene;
}

int Facade::addBrick(int xCell, int yCell, int modelLength, int modelWidth)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].back();
            }

            if (scene->getUsedCellsZ()[i][j].back() == -1.0 && scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2);
            }
        }
    }


    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "i = " << i << "j = " << j << "ячейка занята плиткой";
                    return 1;
                }
            }
        }
    }


    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;

    //стены кирпичика
    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor                        , yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor + SIZE_SC,
                xFactor                        , yFactor, zFactor + SIZE_SC);
        xFactor = xCell * SIZE_SC + 10;
        yFactor += SIZE_SC * modelWidth;
    }

    yFactor = yCell * SIZE_SC + 10;


    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor, yFactor                       , zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor + SIZE_SC,
                xFactor, yFactor                       , zFactor + SIZE_SC);
        yFactor = yCell * SIZE_SC + 10;
        xFactor += SIZE_SC * modelLength;
    }

    xFactor = xCell * SIZE_SC + 10;

    // Низ кирпичика
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            addQuad(vertices, facets,
                    xFactor          , yFactor               , zFactor,
                    xFactor + SIZE_SC, yFactor               , zFactor,
                    xFactor + SIZE_SC, yFactor + SIZE_SC, zFactor,
                    xFactor          , yFactor + SIZE_SC, zFactor);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    xFactor = xCell * SIZE_SC + 10;
    yFactor = yCell * SIZE_SC + 10;
    zFactor += SIZE_SC;

    // Верх кирпичика
    addQuad(vertices, facets,
            xFactor                        , yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor + SIZE_SC * modelWidth, zFactor,
            xFactor                        , yFactor + SIZE_SC * modelWidth, zFactor);

    // Верх кирпичика (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    PolygonModel Brick(vertices, facets, "Кирпичик");
    Brick.setUsedCell(xCell, yCell, zCell + SIZE_SC);
    Brick.setWidthModel(modelWidth);
    Brick.setLengthModel(modelLength);
    Brick.setModelType(PolygonModel::Brick);
    Brick.setModelNum(scene->getModelsNum());
    scene->addModel(Brick);

    return 0;
}


int Facade::addPlate(int xCell, int yCell, int modelLength, int modelWidth)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].back();
            }

            if (scene->getUsedCellsZ()[i][j].back() == -1.0 && scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2);
            }
        }
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;

    //стенки пластинки
    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor                        , yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor + PLATE_HEIGHT,
                xFactor                        , yFactor, zFactor + PLATE_HEIGHT);
        xFactor = xCell * SIZE_SC + 10;
        yFactor += SIZE_SC * modelWidth;
    }

    yFactor = yCell * SIZE_SC + 10;


    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor, yFactor                       , zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor + PLATE_HEIGHT,
                xFactor, yFactor                       , zFactor + PLATE_HEIGHT);
        yFactor = yCell * SIZE_SC + 10;
        xFactor += SIZE_SC * modelLength;
    }

    xFactor = xCell * SIZE_SC + 10;

    // Низ пластинки
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            addQuad(vertices, facets,
                    xFactor          , yFactor          , zFactor,
                    xFactor + SIZE_SC, yFactor          , zFactor,
                    xFactor + SIZE_SC, yFactor + SIZE_SC, zFactor,
                    xFactor          , yFactor + SIZE_SC, zFactor);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    xFactor = xCell * SIZE_SC + 10;
    yFactor = yCell * SIZE_SC + 10;
    zFactor += PLATE_HEIGHT;

    // Верх пластинки
    addQuad(vertices, facets,
            xFactor                        , yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor + SIZE_SC * modelWidth, zFactor,
            xFactor                        , yFactor + SIZE_SC * modelWidth, zFactor);

    // Верх пластинки (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    PolygonModel Plate(vertices, facets, "Пластинка");
    Plate.setUsedCell(xCell, yCell, zCell + PLATE_HEIGHT);
    Plate.setWidthModel(modelWidth);
    Plate.setLengthModel(modelLength);
    Plate.setModelType(PolygonModel::Plate);
    Plate.setModelNum(scene->getModelsNum());
    scene->addModel(Plate);

    return 0;
}


int Facade::addTile(int xCell, int yCell, int modelLength, int modelWidth)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].back();
            }

            if (scene->getUsedCellsZ()[i][j].back() == -1.0 && scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2);
            }
        }
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;

    //стенки плитки
    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor                        , yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor + PLATE_HEIGHT,
                xFactor                        , yFactor, zFactor + PLATE_HEIGHT);
        xFactor = xCell * SIZE_SC + 10;
        yFactor += SIZE_SC * modelWidth;
    }

    yFactor = yCell * SIZE_SC + 10;


    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor, yFactor                       , zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor + PLATE_HEIGHT,
                xFactor, yFactor                       , zFactor + PLATE_HEIGHT);
        yFactor = yCell * SIZE_SC + 10;
        xFactor += SIZE_SC * modelLength;
    }

    xFactor = xCell * SIZE_SC + 10;

    // Низ плитки
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            addQuad(vertices, facets,
                    xFactor          , yFactor          , zFactor,
                    xFactor + SIZE_SC, yFactor          , zFactor,
                    xFactor + SIZE_SC, yFactor + SIZE_SC, zFactor,
                    xFactor          , yFactor + SIZE_SC, zFactor);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    yFactor = yCell * SIZE_SC + 10;
    xFactor = xCell * SIZE_SC + 10;
    zFactor += PLATE_HEIGHT;

    addQuad(vertices, facets,
            xFactor                        , yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor + SIZE_SC * modelWidth, zFactor,
            xFactor                        , yFactor + SIZE_SC * modelWidth, zFactor);

    PolygonModel Tile(vertices, facets, "Плитка");
    Tile.setUsedCell(xCell, yCell, zCell + PLATE_HEIGHT);
    Tile.setWidthModel(modelWidth);
    Tile.setLengthModel(modelLength);
    Tile.setModelType(PolygonModel::Tile);
    Tile.setModelNum(scene->getModelsNum());
    scene->addModel(Tile);

    return 0;
}



int Facade::addArc41(int xCell, int yCell)
{
    xCell -= 1;
    yCell -= 1;
    int modelLength = 4;
    int modelWidth = 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    if (scene->getUsedCellsZ()[yCell][xCell].back() > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell][xCell].back();
    }
    if (scene->getUsedCellsZ()[yCell][xCell].back() == -1.0 && scene->getUsedCellsZ()[yCell][xCell].at(scene->getUsedCellsZ()[yCell][xCell].size() - 2) > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell][xCell].at(scene->getUsedCellsZ()[yCell][xCell].size() - 2);
    }
    if (scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back() > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back();
    }
    if (scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back() == -1.0 && scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].at(scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].size() - 2) > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].at(scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].size() - 2);
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;

    //стенки арки
    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor          , yFactor, zFactor,
                xFactor + SIZE_SC, yFactor, zFactor,
                xFactor + SIZE_SC, yFactor, zFactor + SIZE_SC,
                xFactor          , yFactor, zFactor + SIZE_SC);
        addTriangle(vertices, facets,
                    xFactor + SIZE_SC      , yFactor, zFactor,
                    xFactor + SIZE_SC      , yFactor, zFactor + SIZE_SC,
                    xFactor + 1.5 * SIZE_SC, yFactor, zFactor + 0.4 * SIZE_SC);
        addQuad(vertices, facets,
                xFactor + SIZE_SC      , yFactor, zFactor + SIZE_SC,
                xFactor + 1.5 * SIZE_SC, yFactor, zFactor + 0.4 * SIZE_SC,
                xFactor + 2 * SIZE_SC  , yFactor, zFactor + 0.5 * SIZE_SC,
                xFactor + 2 * SIZE_SC  , yFactor, zFactor + SIZE_SC);
        addQuad(vertices, facets,
                xFactor + 3 * SIZE_SC  , yFactor, zFactor + SIZE_SC,
                xFactor + 2.5 * SIZE_SC, yFactor, zFactor + 0.4 * SIZE_SC,
                xFactor + 2 * SIZE_SC  , yFactor, zFactor + 0.5 * SIZE_SC,
                xFactor + 2 * SIZE_SC  , yFactor, zFactor + SIZE_SC);
        addTriangle(vertices, facets,
                    xFactor + 3 * SIZE_SC  , yFactor, zFactor,
                    xFactor + 3 * SIZE_SC  , yFactor, zFactor + SIZE_SC,
                    xFactor + 2.5 * SIZE_SC, yFactor, zFactor + 0.4 * SIZE_SC);
        addQuad(vertices, facets,
                xFactor + 4 * SIZE_SC, yFactor, zFactor,
                xFactor + 3 * SIZE_SC, yFactor, zFactor,
                xFactor + 3 * SIZE_SC, yFactor, zFactor + SIZE_SC,
                xFactor + 4 * SIZE_SC, yFactor, zFactor + SIZE_SC);
        xFactor = xCell * SIZE_SC + 10;
        yFactor += SIZE_SC * modelWidth;
    }

    yFactor = yCell * SIZE_SC + 10;

    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor, yFactor                       , zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor,
                xFactor, yFactor + SIZE_SC * modelWidth, zFactor + SIZE_SC,
                xFactor, yFactor                       , zFactor + SIZE_SC);
        xFactor += SIZE_SC * modelLength;
    }

    xFactor = xCell * SIZE_SC + 10;

    // Низ арки
    addQuad(vertices, facets,
            xFactor          , yFactor          , zFactor,
            xFactor          , yFactor + SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor          , zFactor);
    addQuad(vertices, facets,
            xFactor + 3 * SIZE_SC, yFactor          , zFactor,
            xFactor + 3 * SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor + 4 * SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor + 4 * SIZE_SC, yFactor          , zFactor);
    addQuad(vertices, facets,
            xFactor + SIZE_SC      , yFactor          , zFactor,
            xFactor + SIZE_SC      , yFactor + SIZE_SC, zFactor,
            xFactor + 1.5 * SIZE_SC, yFactor + SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + 1.5 * SIZE_SC, yFactor          , zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor + 1.5 * SIZE_SC, yFactor          , zFactor + 0.4 * SIZE_SC,
            xFactor + 1.5 * SIZE_SC, yFactor + SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + 2 * SIZE_SC  , yFactor + SIZE_SC, zFactor + 0.5 * SIZE_SC,
            xFactor + 2 * SIZE_SC  , yFactor          , zFactor + 0.5 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor + 2 * SIZE_SC  , yFactor          , zFactor + 0.5 * SIZE_SC,
            xFactor + 2 * SIZE_SC  , yFactor + SIZE_SC, zFactor + 0.5 * SIZE_SC,
            xFactor + 2.5 * SIZE_SC, yFactor + SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + 2.5 * SIZE_SC, yFactor          , zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor + 3 * SIZE_SC  , yFactor          , zFactor,
            xFactor + 3 * SIZE_SC  , yFactor + SIZE_SC, zFactor,
            xFactor + 2.5 * SIZE_SC, yFactor + SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + 2.5 * SIZE_SC, yFactor          , zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor + 4 * SIZE_SC, yFactor          , zFactor,
            xFactor + 4 * SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor + 3 * SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor + 3 * SIZE_SC, yFactor          , zFactor);

    yFactor = yCell * SIZE_SC + 10;
    xFactor = xCell * SIZE_SC + 10;
    zFactor += SIZE_SC;

    // верх
    addQuad(vertices, facets,
            xFactor                        , yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor + SIZE_SC * modelWidth, zFactor,
            xFactor                        , yFactor + SIZE_SC * modelWidth, zFactor);

    // Верх арки (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }


    PolygonModel Arc(vertices, facets, "Арка (4x1)");
    Arc.setUsedCell(xCell, yCell, zCell + SIZE_SC);
    Arc.setWidthModel(modelWidth);
    Arc.setLengthModel(modelLength);
    Arc.setModelType(PolygonModel::Arc41);
    Arc.setModelNum(scene->getModelsNum());
    scene->addModel(Arc);

    return 0;
}




int Facade::addArc14(int xCell, int yCell)
{
    xCell -= 1;
    yCell -= 1;
    int modelLength = 1;
    int modelWidth = 4;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    if (scene->getUsedCellsZ()[yCell][xCell].back() > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell][xCell].back();
    }
    if (scene->getUsedCellsZ()[yCell][xCell].back() == -1.0 && scene->getUsedCellsZ()[yCell][xCell].at(scene->getUsedCellsZ()[yCell][xCell].size() - 2) > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell][xCell].at(scene->getUsedCellsZ()[yCell][xCell].size() - 2);
    }
    if (scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back() > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back();
    }
    if (scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].back() == -1.0 && scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].at(scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].size() - 2) > zCell)
    {
        zCell = scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].at(scene->getUsedCellsZ()[yCell + modelWidth - 1][xCell + modelLength - 1].size() - 2);
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;

    //стенки арки
    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor                        , yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor,
                xFactor + SIZE_SC * modelLength, yFactor, zFactor + SIZE_SC,
                xFactor                        , yFactor, zFactor + SIZE_SC);
        yFactor += SIZE_SC * modelWidth;
    }

    yFactor = yCell * SIZE_SC + 10;

    for (int k = 0; k < 2; k++)
    {
        addQuad(vertices, facets,
                xFactor, yFactor          , zFactor,
                xFactor, yFactor + SIZE_SC, zFactor,
                xFactor, yFactor + SIZE_SC, zFactor + SIZE_SC,
                xFactor, yFactor          , zFactor + SIZE_SC);
        addTriangle(vertices, facets,
                    xFactor, yFactor + SIZE_SC      , zFactor,
                    xFactor, yFactor + SIZE_SC      , zFactor + SIZE_SC,
                    xFactor, yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC);
        addQuad(vertices, facets,
                xFactor, yFactor + SIZE_SC      , zFactor + SIZE_SC,
                xFactor, yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
                xFactor, yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC,
                xFactor, yFactor + 2 * SIZE_SC  , zFactor + SIZE_SC);
        addQuad(vertices, facets,
                xFactor, yFactor + 3 * SIZE_SC  , zFactor + SIZE_SC,
                xFactor, yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
                xFactor, yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC,
                xFactor, yFactor + 2 * SIZE_SC  , zFactor + SIZE_SC);
        addTriangle(vertices, facets,
                    xFactor, yFactor + 3 * SIZE_SC  , zFactor,
                    xFactor, yFactor + 3 * SIZE_SC  , zFactor + SIZE_SC,
                    xFactor, yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC);
        addQuad(vertices, facets,
                xFactor, yFactor + 4 * SIZE_SC, zFactor,
                xFactor, yFactor + 3 * SIZE_SC, zFactor,
                xFactor, yFactor + 3 * SIZE_SC, zFactor + SIZE_SC,
                xFactor, yFactor + 4 * SIZE_SC, zFactor + SIZE_SC);
        xFactor += SIZE_SC * modelLength;
    }

    xFactor = xCell * SIZE_SC + 10;

    // Низ арки
    addQuad(vertices, facets,
            xFactor          , yFactor          , zFactor,
            xFactor + SIZE_SC, yFactor          , zFactor,
            xFactor + SIZE_SC, yFactor + SIZE_SC, zFactor,
            xFactor          , yFactor + SIZE_SC, zFactor);
    addQuad(vertices, facets,
            xFactor          , yFactor + 3 * SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor + 3 * SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor + 4 * SIZE_SC, zFactor,
            xFactor          , yFactor + 4 * SIZE_SC, zFactor);
    addQuad(vertices, facets,
            xFactor          , yFactor + SIZE_SC      , zFactor,
            xFactor + SIZE_SC, yFactor + SIZE_SC      , zFactor,
            xFactor + SIZE_SC, yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor          , yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor          , yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + SIZE_SC, yFactor + 1.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor + SIZE_SC, yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC,
            xFactor          , yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor          , yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC,
            xFactor + SIZE_SC, yFactor + 2 * SIZE_SC  , zFactor + 0.5 * SIZE_SC,
            xFactor + SIZE_SC, yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor          , yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor          , yFactor + 3 * SIZE_SC  , zFactor,
            xFactor + SIZE_SC, yFactor + 3 * SIZE_SC  , zFactor,
            xFactor + SIZE_SC, yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC,
            xFactor          , yFactor + 2.5 * SIZE_SC, zFactor + 0.4 * SIZE_SC);
    addQuad(vertices, facets,
            xFactor          , yFactor + 4 * SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor + 4 * SIZE_SC, zFactor,
            xFactor + SIZE_SC, yFactor + 4 * SIZE_SC, zFactor,
            xFactor          , yFactor + 4 * SIZE_SC, zFactor);

    zFactor += SIZE_SC;

    // верх
    addQuad(vertices, facets,
            xFactor                        , yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor                       , zFactor,
            xFactor + SIZE_SC * modelLength, yFactor + SIZE_SC * modelWidth, zFactor,
            xFactor                        , yFactor + SIZE_SC * modelWidth, zFactor);

    // Верх арки (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    PolygonModel Arc(vertices, facets, "Арка (1x4)");
    Arc.setUsedCell(xCell, yCell, zCell + SIZE_SC);
    Arc.setWidthModel(modelWidth);
    Arc.setLengthModel(modelLength);
    Arc.setModelType(PolygonModel::Arc14);
    Arc.setModelNum(scene->getModelsNum());
    scene->addModel(Arc);

    return 0;
}



int Facade::addCylinder1(int xCell, int yCell)
{
    xCell -= 1;
    yCell -= 1;
    double Radius = 0.5;
    int modelWidth = 1;
    int modelLength = 1;
    int polygons_num = 24;
    Radius *= SIZE_SC;


    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].back();
            }

            if (scene->getUsedCellsZ()[i][j].back() == -1.0 && scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2);
            }
        }
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }


    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;
    double rad, next_rad;
    int xr, yr, xr_n, yr_n;

    //стены цилиндра
    int step = 360 / polygons_num;
    for (int i = 0; i < 360 - step; i += step)
    {
        rad = i * PI / 180;
        next_rad = (i + step) * PI / 180;
        xr = Radius * (cos(rad) + 1);
        yr = Radius * (sin(rad) + 1);
        xr_n = Radius * (cos(next_rad) + 1);
        yr_n = Radius * (sin(next_rad) + 1);

        // стенка
        addQuad(vertices, facets,
                xFactor + xr  , yFactor + yr  , zFactor,
                xFactor + xr  , yFactor + yr  , zFactor + SIZE_SC,
                xFactor + xr_n, yFactor + yr_n, zFactor + SIZE_SC,
                xFactor + xr_n, yFactor + yr_n, zFactor);
        //верх
        addTriangle(vertices, facets,
                xFactor + Radius, yFactor + Radius, zFactor + SIZE_SC,
                xFactor + xr    , yFactor + yr    , zFactor + SIZE_SC,
                xFactor + xr_n  , yFactor + yr_n  , zFactor + SIZE_SC);
        //низ
        addTriangle(vertices, facets,
                xFactor + Radius, yFactor + Radius, zFactor,
                xFactor + xr    , yFactor + yr    , zFactor,
                xFactor + xr_n  , yFactor + yr_n  , zFactor);

        // последняя стенка, низ и верх
        if (i == 360 - 2 * step)
        {
            xr = Radius * (cos(next_rad) + 1);
            yr = Radius * (sin(next_rad) + 1);
            xr_n = Radius * (cos(0) + 1);
            yr_n = Radius * (sin(0) + 1);
            addQuad(vertices, facets,
                    xFactor + xr  , yFactor + yr  , zFactor,
                    xFactor + xr  , yFactor + yr  , zFactor + SIZE_SC,
                    xFactor + xr_n, yFactor + yr_n, zFactor + SIZE_SC,
                    xFactor + xr_n, yFactor + yr_n, zFactor);

            addTriangle(vertices, facets,
                    xFactor + Radius, yFactor + Radius, zFactor + SIZE_SC,
                    xFactor + xr    , yFactor + yr    , zFactor + SIZE_SC,
                    xFactor + xr_n  , yFactor + yr_n  , zFactor + SIZE_SC);

            addTriangle(vertices, facets,
                    xFactor + Radius, yFactor + Radius, zFactor,
                    xFactor + xr    , yFactor + yr    , zFactor,
                    xFactor + xr_n  , yFactor + yr_n  , zFactor);
        }
    }

    zFactor += SIZE_SC;

    // Верх цилиндра (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    PolygonModel Cylinder(vertices, facets, "Цилиндр (1x1)");
    Cylinder.setUsedCell(xCell, yCell, zCell + SIZE_SC);
    Cylinder.setWidthModel(modelWidth);
    Cylinder.setLengthModel(modelLength);
    Cylinder.setModelType(PolygonModel::Cylinder1);
    Cylinder.setModelNum(scene->getModelsNum());
    scene->addModel(Cylinder);

    return 0;
}

int Facade::addCylinder2(int xCell, int yCell)
{
    xCell -= 1;
    yCell -= 1;
    double Radius = 1;
    int modelWidth = 2;
    int modelLength = 2;
    int polygons_num = 24;
    Radius *= SIZE_SC;


    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelWidth - 1 >= (int) scene->getHeight())
        return 2;

    double zCell = 0;
    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].back();
            }

            if (scene->getUsedCellsZ()[i][j].back() == -1.0 && scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) > zCell)
            {
                zCell = scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2);
            }
        }
    }

    for (int i = yCell; i < yCell + modelWidth; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCellsZ()[i][j].back() == -1.0)
            {
                if (scene->getUsedCellsZ()[i][j].at(scene->getUsedCellsZ()[i][j].size() - 2) == zCell)
                {
                    qDebug() << "На ячейке " << i << ", " << j << "стоит плитка";
                    return 1;
                }
            }
        }
    }

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SIZE_SC + 10;
    int yFactor = yCell * SIZE_SC + 10;
    int zFactor = BASE_Z + zCell;
    double rad, next_rad;
    int xr, yr, xr_n, yr_n;

    //стены цилиндра
    int step = 360 / polygons_num;
    for (int i = 0; i < 360 - step; i += step)
    {
        rad = i * PI / 180;
        next_rad = (i + step) * PI / 180;
        xr = Radius * (cos(rad) + 1);
        yr = Radius * (sin(rad) + 1);
        xr_n = Radius * (cos(next_rad) + 1);
        yr_n = Radius * (sin(next_rad) + 1);

        // стенка
        addQuad(vertices, facets,
                xFactor + xr  , yFactor + yr  , zFactor,
                xFactor + xr  , yFactor + yr  , zFactor + SIZE_SC,
                xFactor + xr_n, yFactor + yr_n, zFactor + SIZE_SC,
                xFactor + xr_n, yFactor + yr_n, zFactor);
        //верх
        addTriangle(vertices, facets,
                xFactor + Radius, yFactor + Radius, zFactor + SIZE_SC,
                xFactor + xr    , yFactor + yr    , zFactor + SIZE_SC,
                xFactor + xr_n  , yFactor + yr_n  , zFactor + SIZE_SC);
        //низ
        addTriangle(vertices, facets,
                xFactor + Radius, yFactor + Radius, zFactor,
                xFactor + xr    , yFactor + yr    , zFactor,
                xFactor + xr_n  , yFactor + yr_n  , zFactor);

        // последняя стенка, низ и верх
        if (i == 360 - 2 * step)
        {
            xr = Radius * (cos(next_rad) + 1);
            yr = Radius * (sin(next_rad) + 1);
            xr_n = Radius * (cos(0) + 1);
            yr_n = Radius * (sin(0) + 1);
            addQuad(vertices, facets,
                    xFactor + xr  , yFactor + yr  , zFactor,
                    xFactor + xr  , yFactor + yr  , zFactor + SIZE_SC,
                    xFactor + xr_n, yFactor + yr_n, zFactor + SIZE_SC,
                    xFactor + xr_n, yFactor + yr_n, zFactor);

            addTriangle(vertices, facets,
                    xFactor + Radius, yFactor + Radius, zFactor + SIZE_SC,
                    xFactor + xr    , yFactor + yr    , zFactor + SIZE_SC,
                    xFactor + xr_n  , yFactor + yr_n  , zFactor + SIZE_SC);

            addTriangle(vertices, facets,
                    xFactor + Radius, yFactor + Radius, zFactor,
                    xFactor + xr    , yFactor + yr    , zFactor,
                    xFactor + xr_n  , yFactor + yr_n  , zFactor);
        }
    }

    zFactor += SIZE_SC;

    // Верх цилиндра (крепежи)
    for (int i = 0; i < modelLength; i++)
    {
        yFactor = yCell * SIZE_SC + 10;

        for (int j = 0; j < modelWidth; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor,
                        xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC, zFactor + BRICK_TOP_HEIGHT);

                yFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            yFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);

            // крышечка
            addQuad(vertices, facets,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT,
                    xFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

            for (int k = 0; k < 2; k++)
            {
                addQuad(vertices, facets,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * (1 - BRICK_TOP_SIZE_SC), zFactor + BRICK_TOP_HEIGHT,
                        xFactor + SIZE_SC * BRICK_TOP_SIZE_SC, yFactor + SIZE_SC * BRICK_TOP_SIZE_SC      , zFactor + BRICK_TOP_HEIGHT);

                xFactor += SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            }

            xFactor -= 2 * SIZE_SC * (1 - 2 * BRICK_TOP_SIZE_SC);
            yFactor += SIZE_SC;
        }
        xFactor += SIZE_SC;
    }

    PolygonModel Cylinder(vertices, facets, "Цилиндр (2x2)");
    Cylinder.setUsedCell(xCell, yCell, zCell + SIZE_SC);
    Cylinder.setWidthModel(modelWidth);
    Cylinder.setLengthModel(modelLength);
    Cylinder.setModelType(PolygonModel::Cylinder2);
    Cylinder.setModelNum(scene->getModelsNum());
    scene->addModel(Cylinder);

    return 0;
}
