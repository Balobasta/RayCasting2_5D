#include "source.h"
#include <QDebug>
#include <QTime>

Source::Source(Point pos, Vec2 dir, Vec2 plane, int numOfRays)
{
    this->pos = pos;
    this->dir = dir;
    this->plane = plane;
    this->numOfRays = numOfRays;
    this->calculateRays();
    buildRandomWalls(7);
    buildWalls();
    rayCasting();
}

void Source::calculateRays()
{
    this->deleteRays();
    int n = this->numOfRays/2;
    double w = this->plane.length() / n;
    for( int i = n; i > 0; i--){
        Vec2 *r = new Vec2{this->dir.dirX - this->plane.dirX * w *i, this->dir.dirY - this->plane.dirY * w *i};
        Ray *t = new Ray{*r};
        delete r;
        rays.append(t);
    }
    for( int i = 0; i <= n; i++){
        Vec2 *r = new Vec2{this->dir.dirX + this->plane.dirX * w *i, this->dir.dirY + this->plane.dirY * w *i};
        Ray *t = new Ray{*r};
        delete r;
        rays.append(t);
    }
    this->numOfRays = rays.size();
}

void Source::move(int d)
{
    int moveX = static_cast<int>(step * dir.dirX);
    int moveY = static_cast<int>(step * dir.dirY);
    if (d == 2){
        moveX *= -1;
        moveY *= -1;
    }
    this->pos.x += moveX;
    this->pos.y += moveY;
    qDebug() << "  dirX " << this->dir.dirX << "   dirY  " << this->dir.dirY;
    rayCasting();
}

void rot(double cosa, double sina, Vec2 &vec){
    double new_x = vec.dirX * cosa + vec.dirY * sina;
    double new_y = -vec.dirX * sina + vec.dirY * cosa;
    vec.dirX = new_x;
    vec.dirY = new_y;
}

void Source::rotation(int d)
{
    double cosa;
    double sina;
    if (d == 1){
        cosa = cos(-rotationAngle);
        sina = sin(-rotationAngle);
    }else if ( d == 2){
        cosa = cos(rotationAngle);
        sina = sin(rotationAngle);
    }else return;
    rot(cosa, sina, dir);
    rot(cosa,sina,plane);
    for ( int i = 0; i < rays.size(); i++){
        Vec2 *r = new Vec2{rays[i]->direction.dirX, rays[i]->direction.dirY};
        rot(cosa, sina, *r);
        rays[i]->direction.dirX = r->dirX;
        rays[i]->direction.dirY = r->dirY;
        delete r;
    }
    rayCasting();
}

void Source::rayCasting()
{
    scene.clear();
    for( int i = 0; i < rays.size(); i++){
        double dmin = INFINITY;
        Point nearestPoint;
        int wallnum;
        for( int j = 0; j < walls.size(); j++){
            Point p = rays[i]->cast(walls[j],this->pos,rays[i]->direction);
            double dist = sqrt(pow(p.x - this->pos.x,2) + pow(p.y - this->pos.y,2));
            if (dist < dmin){
                dmin = dist;
                nearestPoint = p;
                wallnum = j;
            }
        }
        if (nearestPoint.x == static_cast<int>(INFINITY)){
            Scene s;
            s.dist = INFINITY;
            scene.append(s);
        }else{
            Vec2 fromPosToNearestPoint = Vec2{(nearestPoint.x - this->pos.x)*1.0, (nearestPoint.y - this->pos.y)*1.0};
            double cosa = (fromPosToNearestPoint * this->dir) / (dmin * this->dir.length());
            scene.append(Scene{cosa*dmin/64, walls[wallnum]->color}); // расстояние проецируется на вектор направления наблюдателя( во избежание рыбьего глаза),
            // для адекватного маштаба значение делится на 64, т.к поле все расстояния в пикселях
        }
    }
}

void Source::deleteRays()
{
    if (rays.size() == 0) return;
    for ( int i = 0; i < rays.size(); i++){
       if(rays[i] != nullptr) delete rays[i];
    }
    rays.clear();
}

void Source::drawWalls(QPainter &painter)
{
    for( int i = 0; i < walls.size(); i++){
        painter.drawLine(walls[i]->begin.x, walls[i]->begin.y, walls[i]->end.x, walls[i]->end.y);
    }
}

void Source::changeNumOfRays(int num)
{
    this->numOfRays = num;
    this->calculateRays();
    this->rayCasting();
}

void Source::buildRandomWalls(int numOfWalls)
{
    QTime *time = new QTime;
    srand(static_cast<uint>(time->msecsSinceStartOfDay()));
    for(int i = 0; i < numOfWalls; i++){
        int x1 = rand() % 1080;
        int y1 = rand() % 720;
        int x2 = rand() % 1080;
        int y2 = rand() % 720;
        Wall *wall = new Wall;
        wall->begin = Point{x1, y1};
        wall->end = Point{x2, y2};
        wall->color = QColor(130,90,50);
        walls.append(wall);
    }
}

void Source::buildWalls()
{
    Wall *wall = new Wall{Point{100,100}, Point{300,100}, QColor(Qt::red)};
    walls.append(wall);
    Wall *wall1 = new Wall{Point{300,100}, Point{300,500}, QColor(Qt::yellow)};
    walls.append(wall1);
    Wall *wall2 = new Wall{Point{400,200}, Point{300,900}, QColor(Qt::gray)};
    walls.append(wall2);
    Wall *wall3 = new Wall{Point{600,100}, Point{800,400}, QColor(136,10,80)};
    walls.append(wall3);
}

int Source::getNumOfRays(){
    return this->numOfRays;
}

QList<Scene> Source::getScene()
{
    return this->scene;
}

