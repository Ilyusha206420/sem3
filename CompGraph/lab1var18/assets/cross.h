#pragma once
#include "line.h"
#include <cmath>

class Cross {
private:
    Line line1, line2;
    Point center;
    Point originalStart1, originalEnd1;
    Point originalStart2, originalEnd2;
    float angle;
    float scale;
    
public:
    Cross(Point start1, Point end1, Point start2, Point end2, 
          float startAngle = 0.0f, float startScale = 1.0f) 
        : originalStart1(start1), originalEnd1(end1),
          originalStart2(start2), originalEnd2(end2),
          angle(startAngle), scale(startScale)
    {
        center.x = (start1.x + end1.x + start2.x + end2.x) / 4;
        center.y = (start1.y + end1.y + start2.y + end2.y) / 4;
        updateLines();
    }
    
    Cross(Point center, int size, float startAngle = 0.0f, float startScale = 1.0f) 
        : center(center), angle(startAngle), scale(startScale)
    {
        int halfSize = size / 2;
        originalStart1 = Point(center.x - halfSize, center.y - halfSize);
        originalEnd1 = Point(center.x + halfSize, center.y + halfSize);
        originalStart2 = Point(center.x - halfSize, center.y + halfSize);
        originalEnd2 = Point(center.x + halfSize, center.y - halfSize);
        updateLines();
    }
    
    const Line& getLine1() const { return line1; }
    const Line& getLine2() const { return line2; }
    
    Point getCenter() const { return center; }
    float getAngle() const { return angle; }
    float getScale() const { return scale; }
    
    void move(int dx, int dy) {
        center.x += dx;
        center.y += dy;
        
        originalStart1.x += dx;
        originalStart1.y += dy;
        originalEnd1.x += dx;
        originalEnd1.y += dy;
        originalStart2.x += dx;
        originalStart2.y += dy;
        originalEnd2.x += dx;
        originalEnd2.y += dy;
        
        updateLines();
    }
    
    void moveTo(Point newCenter) {

        int dx = newCenter.x - center.x;
        int dy = newCenter.y - center.y;

        move(dx, dy);
    }
    
    void rotateLeft() {
        angle -= 0.05f;
        updateLines();
    }
    
    void rotateRight() {
        angle += 0.05f;
        updateLines();
    }
    
    void scaleUp() {
        scale *= 1.02f;
        updateLines();
    }
    
    void scaleDown() {
        scale *= 0.98f;
        updateLines();
    }
    
    void reset() {
        angle = 0.0f;
        scale = 1.0f;
        updateLines();
    }
    
private:
    void updateLines() {
        Point start1 = transformPoint(originalStart1);
        Point end1 = transformPoint(originalEnd1);
        Point start2 = transformPoint(originalStart2);
        Point end2 = transformPoint(originalEnd2);
        
        line1 = Line(start1, end1, 255, 0, 0);
        line2 = Line(start2, end2, 0, 255, 0);
    }
    
    Point transformPoint(const Point& original) const {
        Point scaled;
        scaled.x = center.x + (original.x - center.x) * scale;
        scaled.y = center.y + (original.y - center.y) * scale;
        
        float s = sin(angle);
        float c = cos(angle);
        
        int x = scaled.x - center.x;
        int y = scaled.y - center.y;
        
        int newX = round(x * c - y * s);
        int newY = round(x * s + y * c);
        
        return Point(center.x + newX, center.y + newY);
    }
};