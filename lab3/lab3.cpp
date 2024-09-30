#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "myfunction.hpp"


using namespace cv;
using namespace std;

#define lines 0
#define polylines 1
#define fillpolylines 2

bool show_lines = true;
bool show_polylines = true;
bool show_fillpolylines = true;

int main() {
 
    int width = 1280;
    int height = 900;

    Mat canvases[3];
    //0 - для линий
    //1 - для полигонов
    //2 - для закраски полигонов
    for (int i = 0; i < 3; ++i) {
        canvases[i] = Mat::zeros(height, width, CV_8UC3);
        canvases[i] = WHITE;
    }
    
    if (show_lines) {
        
        int width = 15;

        myDrawLineMod(canvases[lines], Point(25, 50), Point(25, 300), BLACK, 10, "Square");

        myDrawLineMod(canvases[lines], Point(50, 50), Point(50, 300), GREEN, width,  "Square");
        myDrawLineMod(canvases[lines], Point(100, 300), Point(100, 50), GREEN, width, "Square");
        myDrawLineMod(canvases[lines], Point(150, 50), Point(150, 300), GREEN, width, "Round");
        myDrawLineMod(canvases[lines], Point(200, 300), Point(200, 50), GREEN, width, "Round");

        myDrawLineMod(canvases[lines], Point(250, 50), Point(450, 50), RED, width, "Square");
        myDrawLineMod(canvases[lines], Point(450, 100), Point(250, 100), RED, width, "Square");
        myDrawLineMod(canvases[lines], Point(250, 150), Point(450, 150), RED, width, "Round");
        myDrawLineMod(canvases[lines], Point(450, 200), Point(250, 200), RED, width, "Round");

        myDrawLineMod(canvases[lines], Point(50, 500), Point(250, 700), BLUE, width, "Square");
        myDrawLineMod(canvases[lines], Point(250, 800), Point(50, 600), BLUE, width, "Square");
        myDrawLineMod(canvases[lines], Point(300, 500), Point(500, 700), BLUE, width, "Round");
        myDrawLineMod(canvases[lines], Point(500, 800), Point(300, 600), BLUE, width, "Round");

        myDrawLineMod(canvases[lines], Point(600, 800), Point(800, 600), BLACK, width, "Square");
        myDrawLineMod(canvases[lines], Point(800, 500), Point(600, 700), BLACK, width, "Square");
        myDrawLineMod(canvases[lines], Point(850, 800), Point(1050, 600), BLACK, width, "Round");
        myDrawLineMod(canvases[lines], Point(1050, 500), Point(850, 700), BLACK, width, "Round");
        
        myDrawLine(canvases[lines], Point(600, 450), Point(800, 50), RED);
        myDrawLine(canvases[lines], Point(900, 50), Point(700, 450), RED);

        myDrawDashedLine(canvases[lines], Point(800, 450), Point(1000, 50), GREEN, 5, 5, 2);
        myDrawDashedLine(canvases[lines], Point(1100, 50), Point(900, 450), GREEN, 5, 5, 2);

        

        imshow("lines", canvases[lines]);
        imwrite("src/result/lines.png", canvases[lines]);
    }
    
    if (show_polylines) {
        vector<Point> square = { Point(200, 500), Point(200, 700), Point(400, 700), Point(400, 500) };
        vector<Point> bad_square = { Point(450, 500),Point(550, 600) ,Point(450, 700), Point(650, 700), Point(650, 500) };
        vector<Point> triangle = { Point(1000, 700), Point(1000, 500), Point(800, 600)};
        vector<Point> trapezoid = { Point(800, 300), Point(1000, 300), Point(1100, 400), Point(700, 400) };
        vector<Point> star = { Point(400, 400), Point(500, 100), Point(200, 300), Point(600, 300), Point(300, 100)};

        myPolylines(canvases[polylines], square, BLACK);
        myPolylines(canvases[polylines], bad_square, RED);
        myPolylines(canvases[polylines], triangle, RED);
        myPolylines(canvases[polylines], trapezoid, BLUE);
        myPolylines(canvases[polylines], star, GREEN);
        myDrawCircle(canvases[polylines], Point(1000, 110), 100, BLACK, false);
        myDrawCircle(canvases[polylines], Point(0, 0), 150, BLACK, false);

        printf("\n\n");
        cout << "square " << myChelPolyLine(square);
        cout << "\nbad_square " << myChelPolyLine(bad_square);
        cout << "\ntriangl e" << myChelPolyLine(triangle);
        cout << "\ntrapezoid " << myChelPolyLine(trapezoid);
        cout << "\nstar " << myChelPolyLine(star);
        printf("\n\n\n");

        imshow("polylines", canvases[polylines]);
        imwrite("src/result/polylines.png", canvases[polylines]);
       

    }

    if (show_fillpolylines) {
        vector<Point> square = { Point(50, 500), Point(50, 700), Point(250, 700), Point(250, 500) };
        vector<Point> bad_square = { Point(300, 500),Point(400, 650) ,Point(300, 700), Point(500, 700), Point(500, 500) };
        vector<Point> bad_square_2 = { Point(1050, 500),Point(1150, 650) ,Point(1050, 700), Point(1250, 700), Point(1250, 500) };
        vector<Point> trapezoid = { Point(800, 100), Point(1000, 100), Point(1100, 200), Point(700, 200) };
        vector<Point> star1 = { Point(400, 400), Point(500, 100), Point(200, 300), Point(600, 300), Point(300, 100)};
        vector<Point> star2 = { Point(800, 600), Point(900, 300), Point(600, 500), Point(1000, 500), Point(700, 300) };

        myPolylines(canvases[fillpolylines], square, BLACK);
        myPolylines(canvases[fillpolylines], bad_square, BLACK);
        myPolylines(canvases[fillpolylines], bad_square_2, BLACK);
        myPolylines(canvases[fillpolylines], trapezoid, BLACK);
        myPolylines(canvases[fillpolylines], star1, RED);
        myPolylines(canvases[fillpolylines], star2, BLUE);


        fillPolygonNonZeroWinding(canvases[fillpolylines], star1, RED);
        fillPolygonEvenOdd(canvases[fillpolylines], star2, BLUE); 
        fillPolygonEvenOdd(canvases[fillpolylines], trapezoid, BLUE);
        fillPolygonEvenOdd(canvases[fillpolylines], bad_square, BLUE);
        fillPolygonEvenOddBAD(canvases[fillpolylines], bad_square_2, GREEN);
        fillPolygonEvenOdd(canvases[fillpolylines], square, BLUE);

        myDrawCircle(canvases[fillpolylines], Point(1000, 30), 20, GREEN, true);
        myDrawCircle(canvases[fillpolylines], Point(0, 0), 150, GREEN, true);
        

        printf("\n\n");
        printf("RED IS NonZeroWinding\n");
        printf("BLUE IS EvenOdd\n");
        printf("GREEN IS fillPolygonEvenOddBAD");
        printf("\n\n");
        

        imshow("fillpolylines", canvases[fillpolylines]);
        imwrite("src/result/fillpolylines.png", canvases[fillpolylines]);

    }

    
    waitKey(0);

    return 0;
}