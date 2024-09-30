#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


using namespace cv;
using namespace std;



#define WHITE Scalar(255, 255, 255)
#define GRAY Scalar(200, 200, 200)
#define BLACK Scalar(0, 0, 0)
#define BLUE Scalar(200, 0, 0)
#define GREEN Scalar(0, 200, 0)
#define RED Scalar(0, 0, 200)

enum CLPointType { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };
enum IntersectType { SAME, PARALLEL, SKEW, SKEW_CROSS, SKEW_NO_CROSS };
enum EType { TOUCHING, CROSS_LEFT, CROSS_RIGHT, INESSENTIAL };
enum PType { INSIDE, OUTSIDE };


void SetPixel(Mat& image, int x, int y, Scalar color);
void SetPixel(Mat& image, Point P, Scalar color);
double crossProduct(Point p1, Point p2, Point p3);
int orientation(Point p, Point q, Point r);
void myDrawLine(Mat& image, Point p1, Point p2, Scalar color);
void myDrawCircle(Mat& image, Point center, int radius, Scalar color, bool filled);
void myDrawDashedLine(Mat& image, Point p1, Point p2, Scalar color, int width, int dashLength, int gapLength);
void myPolylines(Mat& image, vector<Point>& points, Scalar color);
bool myIsContourConvex(const vector<Point>& points);
bool myHasSelfIntersections(const vector<Point>& points);
string myChelPolyLine(vector<Point>& points);
bool isPointInPolygonEvenOdd(const vector<Point>& polygon, Point pt);
void fillPolygonEvenOdd(Mat& image, const vector<Point>& polygon, Scalar color);
void fillPolygonNonZeroWinding(Mat& image, const vector<Point>& polygon, Scalar color);
void myDrawLineModOLD(Mat& image, Point p1, Point p2, Scalar color, int width, const string& capType);
void myDrawLineMod(Mat& image, Point p1, Point p2, Scalar color, int width, const string& capType);
CLPointType Classify(double x1, double y1, double x2, double y2, double x, double y);
IntersectType Intersect(double ax, double ay, double bx, double by, double cx, double cy,
    double dx, double dy, double* t);
IntersectType Cross(double ax, double ay, double bx, double by, double cx, double cy,
    double dx, double dy, double* tab, double* tcd);
EType EdgeType(double ox, double oy, double dx, double dy, double ax, double ay);

PType PInPolygonEOMode(double x, double y, double* px, double* py, int n);


//просто прямая от p1 до p2
void myDrawLine(Mat &image, Point p1, Point p2, Scalar color) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;  
    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;  
    dx = abs(dx);
    dy = abs(dy);

    
    if (dx >= dy) {
        int e = 2 * dy - dx;  
        for (int i = 0; i <= dx; i++) {
            SetPixel(image, x, y, color);  
            if (e >= 0) {  
                y += iy;  
                e -= 2 * dx;  
            }
            x += ix;  
            e += 2 * dy;  
        }
    }
    
    else {
        int e = 2 * dx - dy; 
        for (int i = 0; i <= dy; i++) {
            SetPixel(image, x, y, color);  
            if (e >= 0) {  
                x += ix;   
                e -= 2 * dy;  
            }
            y += iy; 
            e += 2 * dx; 
        }
    }
}

//прямая с толищной от p1 до p2 с заданием cap (круглый(круг до границы!) или квадрат(точнее не квадрат, а просто прямая по границе))
void myDrawLineMod(Mat& image, Point p1, Point p2, Scalar color, int width, const string& capType) {
    //это костыль :(
    //реализовал пока что так, так как иначе алгоритм ниже работает не корректно, пока оставил так.
    if (p2.x < p1.x and p2.y < p1.y) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }
    if (p2.x < p1.x and p2.y > p1.y) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }
    if (p1.y == p2.y and p1.x > p2.x) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }


    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    float lineLength = sqrtf(dx * dx + dy * dy);
    float X = static_cast<float>(dx) / lineLength;  // x направления прямой
    float Y = static_cast<float>(dy) / lineLength; // y напрявления прямой
    float X_ = -Y;  // x направления нормали
    float Y_ = X; // y напрявления нормали
    int halfWidth = width / 2;
    printf("%d", halfWidth);
    if(width % 2 == 1){
        halfWidth = (width-1) / 2;
    }
    float W = halfWidth;
    float rad;
    if (X == 0) {
        rad = halfWidth;
    }
    else {
        rad = halfWidth * X; //длина нашей линии если смотреть её вдоль нормали. Могу на семинаре обосновать что это такое
    }
    

    // Рассчитываем вершины для линии

    if (capType == "Square") {
        Point p_1(p1.x - W * X_, p1.y - W * Y_);  //по нормали вверх начало
        Point p_2(p2.x - W * X_, p2.y - W * Y_);  //по нормали вверх конец
        Point p_3(p2.x + W * X_, p2.y + W * Y_);  //по нормали вниз конец
        Point p_4(p1.x + W * X_, p1.y + W * Y_);  //по нормали вниз конец

        vector<Point> line = { p_1, p_2, p_3, p_4 };

        fillPolygonNonZeroWinding(image, line, color);
        return;
    }
    if (capType == "Round") {
        Point p_1(p1.x - W * X_ + X * W, p1.y - W * Y_ + W * Y);  //по нормали вверх по прямой вправо НАЧАЛО
        Point p_2(p2.x - W * X_ - X * W, p2.y - W * Y_ - W * Y);  //по нормали вверх по прямой влево КОНЕЦ
        Point p_3(p2.x + W * X_ - X * W, p2.y + W * Y_ - W * Y);  //по нормали вниз  по прямой влево КОНЕЦ
        Point p_4(p1.x + W * X_ + X * W, p1.y + W * Y_ + W * Y);  //по нормали вниз  по прямой вправо НАЧАЛО

        Point center1(((p1.x - W * X_ + X * W) + (p1.x + W * X_ + X * W)) / 2, ((p1.y - W * Y_ + W * Y) + (p1.y + W * Y_ + W * Y)) / 2);
        Point center2(((p2.x - W * X_ - X * W) + (p2.x + W * X_ - X * W)) / 2, ((p2.y - W * Y_ - W * Y) + (p2.y + W * Y_ - W * Y)) / 2);
        vector<Point> line = { p_1, p_2, p_3, p_4 };

        fillPolygonNonZeroWinding(image, line, color);
        myDrawCircle(image, center1, W, color, true);
        myDrawCircle(image, center2, W, color, true);

        return;
    }
}

//рисует окружности с заданым центром и радиусом аргумент filled определяет закрашивать или нет окружность
void myDrawCircle(Mat& image, Point center, int radius, Scalar color, bool filled) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;  

    while (x >= y) {
        if (filled) {
            
            for (int i = center.x - x; i <= center.x + x; i++) {
                SetPixel(image, i, center.y + y, color);  
                SetPixel(image, i, center.y - y, color);  
            }
            for (int i = center.x - y; i <= center.x + y; i++) {
                SetPixel(image, i, center.y + x, color);  
                SetPixel(image, i, center.y - x, color);
            }
        }
        else {
            
            SetPixel(image, center.x + x, center.y + y, color);  // Первая четверть
            SetPixel(image, center.x + y, center.y + x, color);  // Вторая четверть
            SetPixel(image, center.x - x, center.y + y, color);  // Третья четверть
            SetPixel(image, center.x - y, center.y + x, color);  // Четвертая четверть
            SetPixel(image, center.x + x, center.y - y, color);  // Пятая четверть
            SetPixel(image, center.x + y, center.y - x, color);  // Шестая четверть
            SetPixel(image, center.x - x, center.y - y, color);  // Седьмая четверть
            SetPixel(image, center.x - y, center.y - x, color);  // Восьмая четверть
        }

        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;
        }
        else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
        }
    }
}


//пока не разобрался как сделать нормально - рисует штрихи
void myDrawDashedLine(Mat& image, Point p1, Point p2, Scalar color, int width, int dashLength, int gapLength) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;  
    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;  
   

    if (dx == 0 && dy == 0) {
        for (int w = -width / 2; w <= width / 2; w++) {
            SetPixel(image, x1, y1 + w, color); 
        }
        return;
    }

    int steps = max(abs(dx), abs(dy));
    double xIncrement = static_cast<double>(dx) / steps;
    double yIncrement = static_cast<double>(dy) / steps;

    int stepCount = 0; 
    double x = x1;
    double y = y1;

    for (int i = 0; i <= steps; i++) {
        if (stepCount < dashLength) {
            
            for (int w = -width / 2; w <= width / 2; w++) {
                SetPixel(image, static_cast<int>(x), static_cast<int>(y) + w, color);
            }
        }

        stepCount++;

        
        x += xIncrement;
        y += yIncrement;

        if (stepCount >= dashLength + gapLength) {
            stepCount = 0; 
        }
    }
}

//вывод полигона
void myPolylines(Mat& image, vector<Point>& points, Scalar color) {
    int n = points.size();
    if (n < 3) return;

   
    for (int i = 0; i < n - 1; ++i) {
        myDrawLine(image, points[i], points[i + 1], color);
    }

    
    if (points[0] != points[n - 1]) {
        myDrawLine(image, points[n - 1], points[0], color);
    }
}

//проверка на выпукость 
bool myIsContourConvex(const vector<Point>& points) {
    int n = points.size();
    if (n < 3) return false; 

    bool isPositive = false;
    bool isNegative = false;

    for (int i = 0; i < n; ++i) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];  
        Point p3 = points[(i + 2) % n];  

        double cross = crossProduct(p1, p2, p3);

        if (cross > 0) isPositive = true; 
        if (cross < 0) isNegative = true;  

        
        if (isPositive && isNegative) {
            return false;
        }
    }

    return true;  
}

//проверка на самопересения
bool myHasSelfIntersections(const vector<Point>& points) {
    int n = points.size();
    if (n < 4) return false;  // Многоугольник с < 4 вершинами не может самопересекаться

    double t1, t2; // Параметры для пересечения

    for (int i = 0; i < n; ++i) {
        Point p1 = points[i];
        Point q1 = points[(i + 1) % n];

        for (int j = i + 2; j < n; ++j) {
            // Игнорируем случаи, когда отрезки соединяются в одной точке
            if (i == 0 && j == n - 1) continue;

            Point p2 = points[j];
            Point q2 = points[(j + 1) % n];

            // Проверяем пересечение отрезка p1-q1 с отрезком p2-q2
            double tab, tcd; // Параметры для пересечения
            IntersectType intersectType = Cross(p1.x, p1.y, q1.x, q1.y, p2.x, p2.y, q2.x, q2.y, &tab, &tcd);

            if (intersectType == SKEW_CROSS) {
                return true;
            }
        }
    }

    return false;
}


//сборка из 2-х фуункций выше, просто в красивой удобной оболочке
string myChelPolyLine(vector<Point>& points) {
    string answer = "Polyline is ";
    if (myIsContourConvex(points)) {
        answer += "convex, ";
    }
    else {
        answer += "NOT convex, ";
    }
    if (myHasSelfIntersections(points)) {
        answer += "has self intersections.";
    }
    else {
        answer += "has NOT self intersections.";
    }
    return answer;
}


//переделал, как в лекции. Этот не использую, так как неверно работает при пересечении вершин (на рисунке 3 зеленым цветом это видно)
bool isPointInPolygonEvenOdd(const vector<Point>& polygon, Point pt) {
    int intersections = 0;
    double t1, t2; // Параметры для пересечения
    Point outsidePoint(pt.x + 10000, pt.y); // Точка на "бесконечности" в правую сторону

    for (size_t i = 0; i < polygon.size(); i++) {
        Point v1 = polygon[i];
        Point v2 = polygon[(i + 1) % polygon.size()];

        // Проверяем пересечение отрезка v1-v2 с лучом от pt
        IntersectType intersectType = Cross(v1.x, v1.y, v2.x, v2.y, pt.x, pt.y, outsidePoint.x, outsidePoint.y, &t1, &t2);

        if (intersectType == SKEW_CROSS) {
            
            intersections++;
        }
    }
    return (intersections % 2) == 1; // Четное количество пересечений - вне многоугольника
}
//переделал, как в лекции. Этот не использую, так как неверно работает при пересечении вершин (на рисунке 3 зеленым цветом это видно)
void fillPolygonEvenOddBAD(Mat& image, const vector<Point>& polygon, Scalar color) {
   
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (const auto& pt : polygon) {
        minX = min(minX, pt.x);
        minY = min(minY, pt.y);
        maxX = max(maxX, pt.x);
        maxY = max(maxY, pt.y);
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            Point pt(x, y);
            if (isPointInPolygonEvenOdd(polygon, pt)) {
                SetPixel(image, x, y, color);
            }
        }
    }
}


void fillPolygonEvenOdd(Mat& image, const vector<Point>& polygon, Scalar color) {
    int n = polygon.size();
    vector<double> px(n), py(n); // массивы для координат

    // Заполняем массивы координат многоугольника
    for (int i = 0; i < n; i++) {
        px[i] = polygon[i].x;
        py[i] = polygon[i].y;
    }

    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (const auto& pt : polygon) {
        minX = min(minX, pt.x);
        minY = min(minY, pt.y);
        maxX = max(maxX, pt.x);
        maxY = max(maxY, pt.y);
    }

    // Проходим по каждому пикселю в ограниченной области
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (PInPolygonEOMode(x, y, px.data(), py.data(), n) == INSIDE) {
                image.at<Vec3b>(y, x) = Vec3b(color[0], color[1], color[2]); // Устанавливаем пиксель в цвет
            }
        }
    }
}

bool isPointInPolygonNonZeroWinding(const vector<Point>& polygon, Point pt) {
    int windingNumber = 0;

    for (size_t i = 0; i < polygon.size(); i++) {
        Point v1 = polygon[i];
        Point v2 = polygon[(i + 1) % polygon.size()];
        if (v1.y <= pt.y) {
            if (v2.y > pt.y) {
                
                if (crossProduct(v1, v2, pt) > 0) {
                    windingNumber++;  
                }
            }
        }
        else {
            if (v2.y <= pt.y) {
                
                if (crossProduct(v1, v2, pt) < 0) {
                    windingNumber--;
                }
            }
        }
    }
    return windingNumber != 0;  
}

void fillPolygonNonZeroWinding(Mat& image, const vector<Point>& polygon, Scalar color) {
    
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (const auto& pt : polygon) {
        minX = min(minX, pt.x);
        minY = min(minY, pt.y);
        maxX = max(maxX, pt.x);
        maxY = max(maxY, pt.y);
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            Point pt(x, y);
            if (isPointInPolygonNonZeroWinding(polygon, pt)) {
                SetPixel(image, x, y, color);
            }
        }
    }
}




//вспомогательные функции
void SetPixel(Mat& image, int x, int y, Scalar color) {
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
        image.at<Vec3b>(y, x) = Vec3b(color[0], color[1], color[2]);
    }
}

void SetPixel(Mat& image, Point P, Scalar color) {
    int x = P.x;
    int y = P.y;
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
        image.at<Vec3b>(y, x) = Vec3b(color[0], color[1], color[2]);
    }
}

double crossProduct(Point p1, Point p2, Point p3) {
    int x1 = p2.x - p1.x;
    int y1 = p2.y - p1.y;
    int x2 = p3.x - p2.x;
    int y2 = p3.y - p2.y;
    return x1 * y2 - y1 * x2;
}

int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // Коллинеарные
    return (val > 0) ? 1 : -1;  // 1 — по часовой, -1 — против часовой
}


CLPointType Classify(double x1, double y1, double x2, double y2, double x, double y) {
    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x - x1;
    double by = y - y1;
    double s = ax * by - bx * ay;
    if (s > 0) return LEFT;
    if (s < 0) return RIGHT;
    if ((ax * bx < 0) || (ay * by < 0)) //противоположно направлению
        return BEHIND; //позади
    if ((ax * ax + ay * ay) < (bx * bx + by * by))
        return BEHIND; //впереди
    if (x1 == x && y1 == y) //совпадает с началом
        return ORIGIN;
    if (x2 == x && y2 == y) //совпадает с концом
        return DESTINATION;
    return BETWEEN; //между
}

IntersectType Intersect(double ax, double ay, double bx, double by, double cx, double cy,
    double dx, double dy, double* t) {
    double nx = dy - cy; 
    double ny = cx - dx;
    CLPointType type;
    double denom = nx * (bx - ax) + ny * (by - ay);
    if (denom == 0) { 
        type = Classify(cx, cy, dx, dy, ax, ay); 
        if (type == LEFT || type == RIGHT)
            return PARALLEL;
        else return SAME;
    }
    double num = nx * (ax - cx) + ny * (ay - cy); //𝑛 ∗ 𝑎 − 𝑐
    *t = -num / denom; // по значению t можно сделать вывод о пересечении отрезка 𝑎𝑏
    return SKEW;
}

IntersectType Cross(double ax, double ay, double bx, double by, double cx, double cy,
    double dx, double dy, double* tab, double* tcd) {
    IntersectType type = Intersect(ax, ay, bx, by, cx, cy, dx, dy, tab);
    if (type == SAME || type == PARALLEL)
        return type;
    if ((*tab < 0) || (*tab > 1))
        return SKEW_NO_CROSS;
    Intersect(cx, cy, dx, dy, ax, ay, bx, by, tcd);
    if ((*tcd < 0) || (*tcd > 1))
        return SKEW_NO_CROSS;
    return SKEW_CROSS;
}



//это не используется, просто оставил - это я так изначально пытался сделать. В моменте понял, что проще всю линию делать как полигон
//А потом понял, что это Вы и сказали сделать в письме на почту :)
void myDrawLineModOLD(Mat& image, Point p1, Point p2, Scalar color, int width, const string& capType) {

    if (p2.x < p1.x and p2.y < p1.y) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }
    if (p2.x < p1.x and p2.y > p1.y) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }
    if (p1.y == p2.y and p1.x > p2.x) {
        int tempx = p1.x;
        int tempy = p1.y;
        p1.x = p2.x;
        p1.y = p2.y;
        p2.x = tempx;
        p2.y = tempy;
    }

    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;


    int dx = x2 - x1;
    int dx_ = x2 - x1;
    int dy = y2 - y1;
    int dy_ = y2 - y1;
    int ix = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int iy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    dx = abs(dx);
    dy = abs(dy);





    // Основная часть алгоритма Брезенхема для рисования линии
    if (dx >= dy) {
        int e = 2 * dy - dx;

        for (int i = 0; i <= dx; i++) {
            for (int w = -width / 2; w <= width / 2; w++) {
                SetPixel(image, x1, y1 + w, color); // Рисуем с учётом ширины
            }

            if (e >= 0) {
                y1 += iy;
                e -= 2 * dx;
            }
            x1 += ix;
            e += 2 * dy;
        }
    }
    else {
        int e = 2 * dx - dy;

        for (int i = 0; i <= dy; i++) {
            for (int w = -width / 2; w <= width / 2; w++) {
                SetPixel(image, x1 + w, y1, color); // Рисуем с учётом ширины
            }

            if (e >= 0) {
                x1 += ix;
                e -= 2 * dy;
            }
            y1 += iy;
            e += 2 * dx;
        }
    }


    if (capType == "Square") {
        float lineLength = sqrtf(dx * dx + dy * dy);  // Длина линии
        float X = static_cast<float>(dx_) / lineLength;  // x направления прямой
        float Y = static_cast<float>(dy_) / lineLength; // y напрявления прямой
        float X_ = -Y;  // x направления нормали
        float Y_ = X; // y напрявления нормали
        int halfWidth = width / 2;
        float W;
        if (X == 0) {
            W = halfWidth;
        }
        else {
            W = halfWidth * X; //длина нашеё линии если смотреть её вдоль нормали. Могу на семинаре обосновать что это такое
        }

        // Рассчитываем четыре вершины для квадрата

        vector<Point> square_start = {
            Point(p1.x - W * X_ - X * W, p1.y - W * Y_ - W * Y), //по нормали вверх по прямой влево
            Point(p1.x - W * X_ + X * W, p1.y - W * Y_ + W * Y), //по нормали вверх по прямой вправо
            Point(p1.x + W * X_ + X * W, p1.y + W * Y_ + W * Y), //по нормали вниз по прямой вправо
            Point(p1.x + W * X_ - X * W, p1.y + W * Y_ - W * Y)  //по нормали вниз по прямой влево
        };

        vector<Point> square_end = {
            Point(p2.x - W * X_ - X * W, p2.y - W * Y_ - W * Y),
            Point(p2.x - W * X_ + X * W, p2.y - W * Y_ + W * Y),
            Point(p2.x + W * X_ + X * W, p2.y + W * Y_ + W * Y),
            Point(p2.x + W * X_ - X * W, p2.y + W * Y_ - W * Y)
        };

        fillPolygonNonZeroWinding(image, square_start, color);
        fillPolygonNonZeroWinding(image, square_end, color);
    }
    // Рисуем закругленные окончания, если выбран тип Round
    if (capType == "Round") {
        float lineLength = sqrtf(dx * dx + dy * dy);  // Длина линии
        float X = static_cast<float>(dx_) / lineLength;  // x направления прямой
        float Y = static_cast<float>(dy_) / lineLength; // y напрявления прямой
        float X_ = -Y;  // x направления нормали
        float Y_ = X; // y напрявления нормали
        int radius = width / 2;
        float W;
        if (X == 0) {
            W = radius;
        }
        else {
            W = radius * X; //длина нашеё линии если смотреть её вдоль нормали. Могу на семинаре обосновать что это такое
        }
        vector<Point> square_start = {
            Point(p1.x - W * X_ - X * W, p1.y - W * Y_ - W * Y), //по нормали вверх по прямой влево
            Point(p1.x - W * X_ + X * W, p1.y - W * Y_ + W * Y), //по нормали вверх по прямой вправо
            Point(p1.x + W * X_ + X * W, p1.y + W * Y_ + W * Y), //по нормали вниз по прямой вправо
            Point(p1.x + W * X_ - X * W, p1.y + W * Y_ - W * Y)  //по нормали вниз по прямой влево
        };

        vector<Point> square_end = {
            Point(p2.x - W * X_ - X * W, p2.y - W * Y_ - W * Y),
            Point(p2.x - W * X_ + X * W, p2.y - W * Y_ + W * Y),
            Point(p2.x + W * X_ + X * W, p2.y + W * Y_ + W * Y),
            Point(p2.x + W * X_ - X * W, p2.y + W * Y_ - W * Y)
        };

        fillPolygonNonZeroWinding(image, square_start, color);
        fillPolygonNonZeroWinding(image, square_end, color);

        Point center1(((p1.x - W * X_ - X * W) + (p1.x + W * X_ - X * W)) / 2, ((p1.y - W * Y_ - W * Y) + (p1.y + W * Y_ - W * Y)) / 2);
        Point center2(((p2.x - W * X_ + X * W) + (p2.x + W * X_ + X * W)) / 2, ((p2.y - W * Y_ + W * Y) + (p2.y + W * Y_ + W * Y)) / 2);
        myDrawCircle(image, center1, W, color, true); // Закругленное начало
        myDrawCircle(image, center2, W, color, true); // Закругленный конец
    }

}


EType EdgeType(double ox, double oy, double dx, double dy, double ax, double ay) {
    switch (Classify(ox, oy, dx, dy, ax, ay)) {
    case LEFT:
        if (ay > oy && ay <= dy) return CROSS_LEFT; // пересекающая, A слева
        return INESSENTIAL; // безразличная
    case RIGHT:
        if (ay > dy && ay <= oy) return CROSS_RIGHT; // пересекающая, A справа
        return INESSENTIAL; // безразличная
    case BETWEEN:
    case ORIGIN:
    case DESTINATION:
        return TOUCHING; // касающаяся
    default:
        return INESSENTIAL; // безразличная
    }
}


PType PInPolygonEOMode(double x, double y, double* px, double* py, int n) {
    int param = 0;
    for (int i = 0; i < n; i++) {
        switch (EdgeType(px[i], py[i], px[(i + 1) % n], py[(i + 1) % n], x, y)) {
        case TOUCHING: // если лежит на полигоне, то заведомо принадлежит
            return INSIDE;
        case CROSS_LEFT:
        case CROSS_RIGHT:
            param = 1 - param; // изменяем значение четности
        }
    }
    return (param == 1) ? INSIDE : OUTSIDE; // нечетное количество пересечений - внутри, четное - снаружи
}

