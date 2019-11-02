#ifndef SOURCE_H
#define SOURCE_H

#include <QList>
#include <QPainter>

struct Point{
    int x;
    int y;
};

struct Wall{
    Point begin;
    Point end;
    QColor color;
//    color
};

struct Scene{
    double dist;
    QColor color;
};

struct Vec2{
    double dirX;
    double dirY;
    Vec2 operator + (Vec2 vec){
        return Vec2{dirX + vec.dirX, dirY + vec.dirY};
    }
    double operator * (Vec2 vec){
        return (dirX * vec.dirX + dirY * vec.dirY);
    }
    double length(){
        return sqrt(pow(dirX,2) + pow(dirY,2));
    }
};

struct Ray{
    Vec2 direction;
    Point cast(Wall* wall, Point pos, Vec2 dir){
        if(wall == nullptr) return Point{static_cast<int>(INFINITY),static_cast<int>(INFINITY)};
        double x1 = wall->begin.x;
        double y1 = wall->begin.y;
        double x2 = wall->end.x;
        double y2 = wall->end.y;
        double x3 = pos.x;
        double y3 = pos.y;
        double x4 = x3 + dir.dirX;
        double y4 = y3 + dir.dirY;
        double den = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
        if (den!= 0.0){
            double t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / den;
            double u = -((x1 - x2)*(y1 - y3) - (y1 - y2)*(x1 - x3)) / den;
            if ((t > 0.0) && (t < 1.0) && (u > 0.0)){
                int Px = static_cast<int>(x1 + t*(x2 - x1));
                int Py = static_cast<int>(y1 + t*(y2 - y1));
                return Point{Px,Py};
            }
        }
        return Point{static_cast<int>(INFINITY),static_cast<int>(INFINITY)};
    }
};


class Source
{
public:
    Source(Point pos = Point{100,100}, Vec2 dir = Vec2{1,0}, Vec2 plane = Vec2{0,0.66},int numOfRays = 300);
    void move(int d);
    void rotation(int d);
    void drawWalls(QPainter &painter);
    void changeNumOfRays(int num);
    int getNumOfRays();
    QList<Scene> getScene();
private:
    const double pi = 3.14;
    const int step = 20;
    const double rotationAngle = pi/32;
    void rayCasting();
    void calculateRays();
    void buildWalls();
    void buildRandomWalls(int numOfWalls);
    void deleteRays();
    int numOfRays;
    Vec2 dir;
    Vec2 plane;
    Point pos;
    QList<Ray *> rays;
    QList<Wall *> walls;
    QList<Scene> scene;
};

#endif // SOURCE_H
